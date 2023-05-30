#include "View.h"

namespace ofxCortex { namespace ui {

std::vector<std::shared_ptr<View>> View::everyView;

#pragma mark - Public
View::View(std::string name)
{
  setName(name);
  enableMask();
  
  this->addConstraints({
    kiwi::Constraint { right >= left | kiwi::strength::required },
    kiwi::Constraint { bottom >= top | kiwi::strength::required },
    
    kiwi::Constraint { width >= 0 | kiwi::strength::required },
    kiwi::Constraint { height >= 0 | kiwi::strength::required },
    kiwi::Constraint { right == left + width | kiwi::strength::required },
    kiwi::Constraint { bottom == top + height | kiwi::strength::required },
    kiwi::Constraint { centerX == left + 0.5 * width | kiwi::strength::required },
    kiwi::Constraint { centerY == top + 0.5 * height | kiwi::strength::required },
  });
    
  this->addConstraints({
//    kiwi::Constraint { width == intrinsic_width | kiwi::strength::medium },
//    kiwi::Constraint { height == intrinsic_height | kiwi::strength::medium },
    kiwi::Constraint { width >= intrinsic_width | kiwi::strength::medium },
    kiwi::Constraint { height >= intrinsic_height | kiwi::strength::medium },
//    kiwi::Constraint { width <= intrinsic_width | kiwi::strength::weak },
//    kiwi::Constraint { height <= intrinsic_height | kiwi::strength::weak }
  });
  
  // Content Constraints
  this->addConstraints(
   {
     kiwi::Constraint { content_left >= left },
     kiwi::Constraint { content_top >= top },
     kiwi::Constraint { content_right <= right },
     kiwi::Constraint { content_bottom <= bottom },
     
     kiwi::Constraint { content_left == left + Styling::paddingLeft() | kiwi::strength::required },
     kiwi::Constraint { content_right == right - Styling::paddingRight() | kiwi::strength::required },
     kiwi::Constraint { content_top == top + Styling::paddingTop() | kiwi::strength::required },
     kiwi::Constraint { content_bottom == bottom - Styling::paddingBottom() | kiwi::strength::required },
     
     
     kiwi::Constraint { content_width == content_right - content_left | kiwi::strength::required },
     kiwi::Constraint { content_height == content_bottom - content_top | kiwi::strength::required },
     kiwi::Constraint { content_centerX == content_left + 0.5 * content_width | kiwi::strength::required },
     kiwi::Constraint { content_centerY == content_top + 0.5 * content_height | kiwi::strength::required },
   });
  
  LayoutEngine::addEditVariable(left, kiwi::strength::weak);
  LayoutEngine::addEditVariable(top, kiwi::strength::weak);
  LayoutEngine::addEditVariable(width, kiwi::strength::weak);
  LayoutEngine::addEditVariable(height, kiwi::strength::weak);
  LayoutEngine::addEditVariable(intrinsic_width, kiwi::strength::medium);
  LayoutEngine::addEditVariable(intrinsic_height, kiwi::strength::medium);
  
  this->setIntrinsicSize(288, Styling::getRowHeight());
  
  this->enableAutoUpdate();
  this->enableEvents();
  this->_addWindowResizeEventListener();
  
  this->_updateLayoutImmediately();
}

//View::View(std::string name) : View()
//{
//  setName(name);
//  ofLogNotice(_getLogModule(__FUNCTION__));
//}

View::~View()
{
  this->disableAutoUpdate();
  this->disableEvents();
  this->_removeWindowResizeEventListener();
  
  subviews.clear();
}

std::shared_ptr<View> View::create(const std::string & name)
{
  auto instance = std::make_shared<View>(name);
  instance->_setup();
  return instance;
}

void View::_setup()
{
  this->_updateConstraintsImmediately();
}

void View::update()
{
  this->_update(ofGetElapsedTimef(), ofGetLastFrameTime());
}

void View::update(double time, double delta)
{
  this->_update(time, delta);
}

void View::_update(double time, double delta)
{
  this->_updateConstraintsIfNeeded();
  this->_layoutIfNeeded();
  
  _mouseHoverIntensity = ofLerp(_mouseHoverIntensity, _isMouseOver, 1.0 - pow(0.005, delta));
  _mouseActiveIntensity = ofLerp(_mouseActiveIntensity, _wasMousePressedInside, 1.0 - pow(0.005, delta));
}

void View::enableAutoUpdate()
{
  _addUpdateEventListener();
}

void View::disableAutoUpdate()
{
  _removeUpdateEventListener();
}

void View::draw()
{
  ofPushMatrix();
  {
    if (_enableMask)
    {
      Stencil::beginDrawingMask();
      _drawMask();
      Stencil::endDrawingMask();
      Stencil::beginUsingMask();
    }
    
    ofPushStyle();
    {
      _preDraw();
      _draw();
      _postDraw();
    }
    ofPopStyle();
    
    if (_shouldDrawSubviews) _drawSubviews();
    
    if (_enableMask) Stencil::endUsingMask();
  }
  ofPopMatrix();
}

void View::_preDraw()
{
  
}

void View::_draw()
{
  ofxCortex::ui::Styling::drawBackground(this->getBounds());
}

void View::_postDraw()
{
  if (_isFocused())
  {
    ofPushStyle();
    Styling::drawBackground(getBounds(), ofColor(0, 0), Styling::getBorderColor(Styling::State::FOCUS));
    ofPopStyle();
  }
  
  if (_debugBounds)
  {
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::tomato, 128);
    ofDrawRectangle(this->getBounds());
    
    ofSetColor(Styling::getAccentColor(), 128);
    ofDrawRectangle(this->getContentBounds());
    ofPopStyle();
  }
}

void View::_drawMask()
{
  ofDrawRectangle(getBounds());
}

void View::_drawSubviews()
{
  for (auto & view : subviews) view->draw();
}

void View::setNeedsLayout(bool affectSubviews) {
  this->_needsLayout = true;
  
  if (affectSubviews)
  {
    for (auto & view :subviews) view->setNeedsLayout(true);
  }
}

void View::setIntrinsicSize(float width, float height)
{
  LayoutEngine::suggestValue(intrinsic_width, width);
  LayoutEngine::suggestValue(intrinsic_height, height);
  this->setNeedsLayout();
}

void View::setIntrinsicWidth(float width)
{
  LayoutEngine::suggestValue(intrinsic_width, width);
  this->setNeedsLayout();
}

void View::setIntrinsicHeight(float height)
{
  LayoutEngine::suggestValue(intrinsic_height, height);
  this->setNeedsLayout();
}

void View::setLeft(float value)
{
  LayoutEngine::suggestValue(this->left, value);
  this->setNeedsLayout();
}

void View::setTop(float value)
{
  LayoutEngine::suggestValue(this->top, value);
  this->setNeedsLayout();
}

void View::setRight(float value)
{
  LayoutEngine::suggestValue(this->width, value - this->left.value());
  this->setNeedsLayout();
}

void View::setBottom(float value)
{
  LayoutEngine::suggestValue(this->height, value - this->top.value());
  this->setNeedsLayout();
}

void View::setPosition(float x, float y)
{
  ofLogVerbose(_getLogModule(__FUNCTION__));
  
  LayoutEngine::suggestValue(this->left, x);
  LayoutEngine::suggestValue(this->top, y);
  this->setNeedsLayout();
}

void View::setPosition(glm::vec2 position)
{
  this->setPosition(position.x, position.y);
}

void View::setWidth(float value)
{
  LayoutEngine::suggestValue(this->width, value);
  this->setNeedsLayout();
}
void View::setHeight(float value)
{
  LayoutEngine::suggestValue(this->height, value);
  this->setNeedsLayout();
}


void View::setRect(const ofRectangle & rect)
{
  LayoutEngine::suggestValue(this->left, rect.x);
  LayoutEngine::suggestValue(this->top, rect.y);
  LayoutEngine::suggestValue(this->width, rect.width);
  LayoutEngine::suggestValue(this->height, rect.height);
  
  this->setNeedsLayout();
}

void View::setCenter(float x, float y)
{
  LayoutEngine::suggestValue(this->left, x - this->width.value() * 0.5);
  LayoutEngine::suggestValue(this->top, y - this->height.value() * 0.5);
  
  this->setNeedsLayout();
  
}

void View::setCenter(glm::vec2 center)
{
  this->setCenter(center.x, center.y);
}

void View::translate(float dX, float dY)
{
  LayoutEngine::suggestValue(this->left, this->left.value() + dX);
  LayoutEngine::suggestValue(this->top, this->top.value() + dY);
  
  this->setNeedsLayout();
}

void View::translate(const glm::vec2 & delta)
{
  this->translate(delta.x, delta.y);
}

bool View::isInside(const glm::vec2 & point)
{
  return getBounds().inside(point);
}

bool View::isOverlapped(const glm::vec2 & point)
{
  //  return false;
  
  for (auto & view : everyView)
  {
    if (view.get() == this || !view->isInside(point) || view->_includeInOverlap == false) continue;
    
    return view->_getActualZ() > this->_getActualZ(); // || view->getZ() > this->getZ();
  }
  
  return false;
}


void View::addSubviewAt(std::shared_ptr<View> subview, size_t index)
{
  subview->superview = shared_from_this();
  subview->level = this->level + 1;
  subviews.insert(subviews.begin() + index, subview);
  everyView.push_back(subview);
}

void View::addSubview(std::shared_ptr<View> subview)
{
  this->addSubviewAt(subview, subviews.size());
}


void View::addSubviews(std::vector<std::shared_ptr<View>> views)
{
  for (auto & view : views) this->addSubview(view);
}

void View::removeSubview(std::shared_ptr<View> subview)
{
  subviews.erase(std::remove(std::begin(subviews), std::end(subviews), subview), std::end(subviews));
}


void View::addConstraint(kiwi::Constraint & constraint)
{
  LayoutEngine::addConstraint(constraint);
  this->constraints.push_back(constraint);
  
  this->setNeedsLayout();
}

void View::addConstraints(std::vector<kiwi::Constraint> constraints)
{
  for (auto& constraint : constraints) this->addConstraint(constraint);
  //  this->constraints.insert(this->constraints.end(), constraints.begin(), constraints.end());
}

void View::removeConstraint(kiwi::Constraint & constraint)
{
  LayoutEngine::removeConstraint(constraint);
  std::remove(std::begin(constraints), std::end(constraints), constraint);
  
  this->setNeedsLayout();
}

void View::removeConstraints(std::vector<kiwi::Constraint> constraints)
{
  for (auto& constraint : constraints) this->removeConstraint(constraint);
}

#pragma mark - Protected

std::shared_ptr<View> View::getRoot()
{
  if (!hasParent()) return shared_from_this();
  
  std::shared_ptr<View> currentNode = getParent();
  
  while (currentNode->hasParent()) currentNode = currentNode->getParent();
  
  return currentNode;
}


std::vector<std::shared_ptr<View>> View::flatten(const std::shared_ptr<View> & node)
{
  std::vector<std::shared_ptr<View>> result;
  std::stack<std::shared_ptr<View>> stack;
  
  stack.push(node);
  
  while (!stack.empty()) {
    auto current = stack.top();
    stack.pop();
    
    result.push_back(current);
    
    for (auto it = current->subviews.rbegin(); it != current->subviews.rend(); ++it) {
      stack.push(*it);
    }
  }
  
  return result;
}

int View::_getDepth() const {
  if (!hasParent()) return 0;
  
  int depth = 0;
  
  std::shared_ptr<const View> currentNode = getParent();
  while (currentNode->hasParent())
  {
    currentNode = currentNode->getParent();
    depth++;
  }
  
  return depth;
}






void View::_updateConstraints()
{
  ofLogVerbose(_getLogModule(__FUNCTION__));
}

void View::_updateConstraintsIfNeeded()
{
  if (this->_needsUpdateConstraints)
  {
    this->_updateConstraints();
    this->_needsUpdateConstraints = false;
  }
}

void View::_updateConstraintsImmediately()
{
  this->_setNeedsUpdateConstraints();
  this->_updateConstraintsIfNeeded();
}

void View::_setNeedsUpdateConstraints()
{
  this->_needsUpdateConstraints = true;
}

void View::_updateLayoutImmediately()
{
  this->setNeedsLayout();
  this->_layoutIfNeeded();
}

void View::_layoutIfNeeded(bool layoutSubviews)
{
  if (this->_needsLayout) {
    this->_updateLayout();
    this->_needsLayout = false;
  };
}

void View::_updateLayout()
{
  ofLogVerbose(_getLogModule(__FUNCTION__));
  
  this->_updateBounds();
}

ofRectangle View::getBounds() const
{
  ofRectangle bounds;
  
  bounds.x = left.value();
  bounds.y = top.value();
  bounds.width = width.value();
  bounds.height = height.value();
  
  return bounds;
}

ofRectangle View::getContentBounds() const
{
  ofRectangle contentBounds;
  
  contentBounds.x = content_left.value();
  contentBounds.y = content_top.value();
  contentBounds.width = content_width.value();
  contentBounds.height = content_height.value();
  
  return contentBounds;
}

void View::_updateBounds()
{
  ofLogVerbose(_getLogModule(__FUNCTION__));
  
//  bounds.x = left.value();
//  bounds.y = top.value();
//  bounds.width = width.value();
//  bounds.height = height.value();
//
//  contentBounds.x = content_left.value();
//  contentBounds.y = content_top.value();
//  contentBounds.width = content_width.value();
//  contentBounds.height = content_height.value();
}

ofRectangle View::_getLocalBounds() const {
  return ofRectangle(0, 0, width.value(), height.value());
}

ofRectangle View::_getLocalContentBounds() const {
  return ofRectangle(0, 0, content_width.value(), content_height.value());
}

ofMatrix4x4 View::_getTransformationMatrix() const
{
  ofMatrix4x4 m;
  m = ofMatrix4x4::newTranslationMatrix(getPosition()) * m; //.setTranslation(getPosition());
                                                            //  m = ofMatrix4x4::newScaleMatrix(glm::vec3(_scale.x, _scale.y, 0)) * m;
                                                            //  m = ofMatrix4x4::newRotationMatrix(_rotation, 0, 0, 1) * m;
  
  return m;
}

ofMatrix4x4 View::_getInverseTransformationMatrix() const
{
  ofMatrix4x4 m;
  //  m = ofMatrix4x4::newRotationMatrix(_rotation * -1.0, 0, 0, 1) * m;
  //  m = ofMatrix4x4::newScaleMatrix(glm::vec3(1.0f / _scale.x, 1.0f / _scale.y, 1.0f)) * m;
  m = ofMatrix4x4::newTranslationMatrix(getPosition() * -1.0) * m;
  //  if (hasParent()) m = ofMatrix4x4::newTranslationMatrix(getParent()->getPosition() * -1.0) * m;
  
  return m;
}

std::string View::_getLogModule(const std::string & func)
{
  std::stringstream logModule;
  logModule << _getComponentName() << "('" << _name << "')::" << func << "()";
  return logModule.str();
}



void View::_addDrawEventListener()
{
  ofAddListener(ofEvents().draw, this, &View::_drawHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_removeDrawEventListener()
{
  ofRemoveListener(ofEvents().draw, this, &View::_drawHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_addUpdateEventListener()
{
  ofAddListener(ofEvents().update, this, &View::_updateHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_removeUpdateEventListener()
{
  ofRemoveListener(ofEvents().update, this, &View::_updateHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_addWindowResizeEventListener()
{
  ofAddListener(ofEvents().windowResized, this, &View::_windowResized, OF_EVENT_ORDER_AFTER_APP);
}

void View::_removeWindowResizeEventListener()
{
  ofRemoveListener(ofEvents().windowResized, this, &View::_windowResized, OF_EVENT_ORDER_AFTER_APP);
}

void View::_addMouseEventListeners()
{
  ofAddListener(ofEvents().mousePressed, this, &View::_mousePressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseReleased, this, &View::_mouseReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseMoved, this, &View::_mouseMovedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseDragged, this, &View::_mouseDraggedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseScrolled, this, &View::_mouseScrolledHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_removeMouseEventListeners()
{
  ofRemoveListener(ofEvents().mousePressed, this, &View::_mousePressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseReleased, this, &View::_mouseReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseMoved, this, &View::_mouseMovedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseDragged, this, &View::_mouseDraggedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseScrolled, this, &View::_mouseScrolledHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_addKeyEventListeners()
{
  ofAddListener(ofEvents().keyPressed, this, &View::_keyPressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().keyReleased, this, &View::_keyReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().charEvent, this, &View::_charHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_removeKeyEventListeners()
{
  ofRemoveListener(ofEvents().keyPressed, this, &View::_keyPressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().keyReleased, this, &View::_keyReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().charEvent, this, &View::_charHandler, OF_EVENT_ORDER_AFTER_APP);
}

void View::_updateHandler(ofEventArgs &e)
{
  _update(ofGetElapsedTimef(), ofGetLastFrameTime());
}

void View::_drawHandler(ofEventArgs &e)
{
  _draw();
}

void View::_mousePressedHandler(ofMouseEventArgs &e)
{
  bool isPressedInside = isInside(e) && !isOverlapped(e);
  
  if (isPressedInside) {
    _triggerMousePressed(e);
    if (_includeInOverlap) View::focused = shared_from_this();
  }
  
  _isMousePressed = true;
  _wasMousePressedInside = isPressedInside;
  _lastMousePosition = e;
}

void View::_mouseReleasedHandler(ofMouseEventArgs &e)
{
  bool isReleasedInside = isInside(e) && !isOverlapped(e);
  
  if (_wasMousePressedInside) _triggerMouseReleased(e);
  if (_wasMousePressedInside && !isReleasedInside) _triggerMouseReleasedOutside(e);
  
  _isMousePressed = false;
  _wasMousePressedInside = false;
  _wasMouseReleasedInside = isReleasedInside;
}

void View::_mouseMovedHandler(ofMouseEventArgs &e)
{
  //  ofLogVerbose(getLogModule(__FUNCTION__));
  bool isMovedInside = isInside(e) && !isOverlapped(e);
  
  auto deltaEvent = DeltaMouseEvent(e);
  deltaEvent.delta = e - _lastMousePosition;
  
  if (isMovedInside) _triggerMouseMoved(deltaEvent);
  
  if (!_isMouseOver && isMovedInside) {
    _triggerMouseEnter(e);
  }
  
  if (_isMouseOver && !isMovedInside) {
    _triggerMouseExit(e);
  }
  
  _isMouseOver = isMovedInside;
  _lastMousePosition = e;
}

void View::_mouseDraggedHandler(ofMouseEventArgs &e)
{
  auto deltaEvent = DeltaMouseEvent(e);
  deltaEvent.delta = e - _lastMousePosition;
  
  bool isMovedInside = isInside(e) && !isOverlapped(e);
  
  if (_wasMousePressedInside) _triggerMouseDragged(deltaEvent);
  
  if (!_isMouseOver && isMovedInside) {
    _triggerMouseEnter(e);
  }
  
  if (_isMouseOver && !isMovedInside) {
    _triggerMouseExit(e);
  }
  
  _isMouseOver = isMovedInside;
  _lastMousePosition = e;
}

void View::_mouseScrolledHandler(ofMouseEventArgs &e)
{
  if (_isMouseOver) _triggerMouseScrolled(e);
}


void View::_keyPressedHandler(ofKeyEventArgs &e)
{
  if (_isFocused()) _triggerKeyPressed(e);
}

void View::_keyReleasedHandler(ofKeyEventArgs &e)
{
  if (_isFocused()) _triggerKeyReleased(e);
}

void View::_charHandler(uint32_t &c)
{
  if (_isFocused()) _triggerCharTyped(c);
}

}}
