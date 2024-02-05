#include "View.h"

namespace ofxCortex { namespace ui {

#pragma mark - Public
View::View(const std::string & name)
{
  setName(name);
//  enableMask();
  
  baseConstraints = {
    kiwi::Constraint { right >= left | kiwi::strength::required },
    kiwi::Constraint { bottom >= top | kiwi::strength::required },
    
    kiwi::Constraint { width >= 0 | kiwi::strength::required },
    kiwi::Constraint { height >= 0 | kiwi::strength::required },
    kiwi::Constraint { right == left + width | kiwi::strength::required },
    kiwi::Constraint { bottom == top + height | kiwi::strength::required },
    kiwi::Constraint { centerX == left + 0.5 * width | kiwi::strength::required },
    kiwi::Constraint { centerY == top + 0.5 * height | kiwi::strength::required },
    
    kiwi::Constraint { width == intrinsic_width | kiwi::strength::medium },
    kiwi::Constraint { height == intrinsic_height | kiwi::strength::medium },
    
    kiwi::Constraint { content_left >= left | kiwi::strength::required },
    kiwi::Constraint { content_top >= top | kiwi::strength::required },
    kiwi::Constraint { content_right <= right | kiwi::strength::required },
    kiwi::Constraint { content_bottom <= bottom | kiwi::strength::required },
    
    kiwi::Constraint { content_left == left + Styling::paddingLeft() | kiwi::strength::required },
    kiwi::Constraint { content_right == right - Styling::paddingRight() | kiwi::strength::required },
    kiwi::Constraint { content_top == top + Styling::paddingTop() | kiwi::strength::required },
    kiwi::Constraint { content_bottom == bottom - Styling::paddingBottom() | kiwi::strength::required },
    
    
    kiwi::Constraint { content_width == content_right - content_left | kiwi::strength::required },
    kiwi::Constraint { content_height == content_bottom - content_top | kiwi::strength::required },
    kiwi::Constraint { content_centerX == content_left + 0.5 * content_width | kiwi::strength::required },
    kiwi::Constraint { content_centerY == content_top + 0.5 * content_height | kiwi::strength::required },
  };
  
  LayoutEngine::addConstraints(baseConstraints);
  
  LayoutEngine::addEditVariable(left, kiwi::strength::weak);
  LayoutEngine::addEditVariable(top, kiwi::strength::weak);
  LayoutEngine::addEditVariable(width, kiwi::strength::weak);
  LayoutEngine::addEditVariable(height, kiwi::strength::weak);
  LayoutEngine::addEditVariable(intrinsic_width, kiwi::strength::medium);
  LayoutEngine::addEditVariable(intrinsic_height, kiwi::strength::medium);
  
  this->setIntrinsicSize(288 * Styling::getScale(), Styling::getRowHeight());
  
  this->bindUpdateListener();
  this->bindMouseListeners();
  this->bindKeyListeners();
}

View::~View()
{
  this->unbindEvents();
  
  LayoutEngine::removeConstraints(baseConstraints);
  this->removeAllConstraints();
  subviews.clear();
}

void View::internalUpdate(double time, double delta)
{
  this->updateConstraintsIfNeeded();
  this->layoutIfNeeded();
  
  mouseHoverIntensity = ofLerp(mouseHoverIntensity, isMouseInside, 1.0 - pow(0.005, delta));
  mouseActiveIntensity = ofLerp(mouseActiveIntensity, isMousePressed, 1.0 - pow(0.005, delta));
  
  this->onUpdate(time, delta);
}

void View::internalDraw()
{
  ofPushMatrix();
  {
    ofPushStyle();
    {
      if (this->shouldDraw) this->onPreDraw();
      
      if (this->isMaskEnabled)
      {
        Stencil::beginDrawingMask();
        this->onDrawMask();
        Stencil::endDrawingMask();
        Stencil::beginUsingMask();
      }
      
      if (this->shouldDraw) this->onDraw();
      
      if (this->shouldDrawSubviews) this->onDrawSubviews();
      if (this->isMaskEnabled) Stencil::endUsingMask();
      
      if (this->shouldDraw) this->onPostDraw();
    }
    ofPopStyle();
  }
  ofPopMatrix();
}

void View::onDraw()
{
  ofPushStyle();
  ofSetColor(255);
  Styling::drawBackground(this->frame, this->getMouseState());
  Styling::drawBorder(this->frame);
  ofPopStyle();
}

void View::onPostDraw()
{
  if (this->isFocused())
  {
    ofPushStyle();
    Styling::drawFocusBorder(this->frame);
    ofPopStyle();
  }
  
  if (this->isDebugEnabled) { this->onDebug(); }
}

void View::onDrawMask()
{
  ofDrawRectangle(this->frame);
}

void View::onDrawSubviews()
{
  for (auto & view : subviews) view->draw();
}

void View::onDebug()
{
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::red);
  ofDrawRectangle(this->frame);
  
  ofSetColor(ofColor::green);
  ofDrawRectangle(this->contentFrame);
  
//  ofFill();
//  ofSetColor(Styling::getBackgroundColor(getMouseState()));
//  ofDrawRectangle(this->contentFrame);
  
  ofSetColor(255, 192);
  ofDrawBitmapString(getName() + " - " + ofToString(frame), this->getContentFrame().x + 2, this->getContentFrame().y + 11 + 2);
  ofPopStyle();
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
  LayoutEngine::suggestValue(this->left, x);
  LayoutEngine::suggestValue(this->top, y);
  this->setNeedsLayout();
}

void View::setPosition(const glm::vec2 & position)
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


void View::setFrame(const ofRectangle & rect)
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

bool View::inside(const glm::vec2 & point)
{
  return getFrame().inside(point);
}

bool View::isOverlapped(const glm::vec2 & point)
{
  //  return false;
  
  for (auto & view : everyView)
  {
    if (view.get() == this || !view->inside(point) || view->includeInOverlap == false) continue;
    
    return view->getActualZ() >= this->getActualZ(); // || view->getZ() > this->getZ();
  }
  
  return false;
}


void View::addSubviewAt(const std::shared_ptr<View> & subview, size_t index)
{
  subview->setParent(shared_from_this());
  subview->setLevel(this->level + 1);
  subviews.insert(subviews.begin() + index, subview);
}

void View::addSubview(const std::shared_ptr<View> & subview)
{
  this->addSubviewAt(subview, subviews.size());
}


void View::addSubviews(std::vector<std::shared_ptr<View>> views)
{
  if (views.size() == 0) return;
  
  for (auto & view : views) this->addSubview(view);
}

void View::removeSubview(std::shared_ptr<View> subview)
{
  subviews.erase(std::remove(std::begin(subviews), std::end(subviews), subview), std::end(subviews));
}


void View::addConstraint(kiwi::Constraint & constraint)
{
  ofLogVerbose(_getLogModule(__FUNCTION__)) << kiwi::debug::dumps(constraint) << std::endl;
  
  LayoutEngine::addConstraint(constraint);
  this->constraints.push_back(constraint);
  
  this->setNeedsLayout();
}

void View::addConstraints(std::vector<kiwi::Constraint> constraints)
{
  for (auto & constraint : constraints) this->addConstraint(constraint);
}

void View::removeConstraint(kiwi::Constraint & constraint)
{
  LayoutEngine::removeConstraint(constraint);
  std::remove(std::begin(this->constraints), std::end(this->constraints), constraint);
  
  this->setNeedsLayout();
}

void View::removeConstraints(std::vector<kiwi::Constraint> constraints)
{
  for (auto& constraint : constraints) this->removeConstraint(constraint);
}

void View::removeAllConstraints()
{
  for (auto& constraint : this->constraints)
  {
    this->removeConstraint(constraint);
  }
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

int View::getDepth() const {
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






void View::updateConstraints()
{
  ofLogNotice(_getLogModule(__FUNCTION__));
}

void View::layoutSubviews()
{
  ofLogNotice(_getLogModule(__FUNCTION__));
  
  frame = getFrame();
  contentFrame = getContentFrame();
}

ofRectangle View::getFrame() const
{
  return ofRectangle(left.value(), top.value(), width.value(), height.value());
}

ofRectangle View::getContentFrame() const
{
  return ofRectangle(content_left.value(), content_top.value(), content_width.value(), content_height.value());
}

void View::updateBounds()
{
  ofLogVerbose(_getLogModule(__FUNCTION__));
}

ofRectangle View::getLocalFrame() const {
  return ofRectangle(0, 0, width.value(), height.value());
}

ofRectangle View::getLocalContentFrame() const {
  return ofRectangle(0, 0, content_width.value(), content_height.value());
}

std::string View::_getLogModule(const std::string & func)
{
  std::stringstream logModule;
  logModule << _getComponentName() << "('" << _name << "')[" << ofGetFrameNum() << "]::" << func << "()";
  return logModule.str();
}



#pragma mark - Event Handling
void View::enableInteraction()
{
  this->enableMouseEvents();
  this->enableKeyEvents();
  this->isInteractionEnabled = true;
}

void View::disableInteraction()
{
  this->disableMouseEvents();
  this->disableKeyEvents();
  this->isInteractionEnabled = false;
}

void View::bindEvents()
{
  ofLogNotice("View('" + _name + "')::bindEvents()");
  
  this->bindUpdateListener();
  this->bindDrawListener();
  this->bindMouseListeners();
  this->bindKeyListeners();
}

void View::unbindEvents()
{
  ofLogNotice("View('" + _name + "')::unbindEvents()");
  
  this->unbindUpdateListener();
  this->unbindDrawListener();
  this->unbindMouseListeners();
  this->unbindKeyListeners();
}

#pragma mark - Mouse Handling
void View::bindMouseListeners()
{
  ofAddListener(ofEvents().mousePressed, this, &View::mousePressedHandler);
  ofAddListener(ofEvents().mouseReleased, this, &View::mouseReleasedHandler);
  ofAddListener(ofEvents().mouseMoved, this, &View::mouseMovedHandler);
  ofAddListener(ofEvents().mouseDragged, this, &View::mouseDraggedHandler);
  ofAddListener(ofEvents().mouseScrolled, this, &View::mouseScrolledHandler);
}

void View::unbindMouseListeners()
{
  ofRemoveListener(ofEvents().mousePressed, this, &View::mousePressedHandler);
  ofRemoveListener(ofEvents().mouseReleased, this, &View::mouseReleasedHandler);
}


void View::mousePressedHandler(ofMouseEventArgs & e)
{
  bool isInsideView = this->inside(e);
  
  MouseEventArgs customE(e);
  customE.isOverlapped = false;
  
  if (isInsideView)
  {
    onMousePressed(customE);
    
    View::focused = shared_from_this();
  }
  
  this->wasMousePressedInside = isInsideView;
  this->isMousePressed = isInsideView;
}

void View::onMousePressed(const MouseEventArgs &e)
{
  ofLogNotice("View('" + _name + "')") << "onMousePressed(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::mouseReleasedHandler(ofMouseEventArgs & e)
{
  bool isInsideView = this->inside(e);
  
  MouseEventArgs customE(e);
  customE.isOverlapped = false;
  
  if (isInsideView) { onMouseReleased(customE); }
  else if (this->wasMousePressedInside && !isInsideView) {
    onMouseReleasedOutside(customE);
  }
  
  this->isMousePressed = false;
}

void View::onMouseReleased(const MouseEventArgs &e)
{
  ofLogNotice("View('" + _name + "')") << "onMouseReleased(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::onMouseReleasedOutside(const MouseEventArgs &e)
{
  ofLogNotice("View('" + _name + "')") << "onMouseReleasedOutside(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::mouseMovedHandler(ofMouseEventArgs & e)
{
  //  ofLogNotice("View('" + name + "')") << "mouseReleasedHandler(ofMouseEventArgs & e)";
  
  bool isInsideView = this->inside(e);
  
  auto customE = MouseEventArgs(e);
  customE.isOverlapped = false;
  customE.delta = e - this->lastMousePosition;
  
  if (isInsideView) { onMouseMoved(customE); }
  
  if (!this->isMouseInside && isInsideView) { onMouseEnter(customE); }
  else if (this->isMouseInside && !isInsideView) { onMouseExit(customE); }
  
  this->isMouseInside = isInsideView;
  this->lastMousePosition = e;
}

void View::onMouseMoved(const MouseEventArgs & e)
{
  //  ofLogNotice("View('" + name + "')") << "onMouseMoved(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::onMouseEnter(const MouseEventArgs &e)
{
  ofLogNotice("View('" + _name + "')") << "onMouseEnter(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::onMouseExit(const MouseEventArgs &e)
{
  ofLogNotice("View('" + _name + "')") << "onMouseExit(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::mouseDraggedHandler(ofMouseEventArgs & e)
{
  bool isInsideView = this->inside(e);
  
  auto customE = MouseEventArgs(e);
  customE.isOverlapped = false;
  customE.delta = e - this->lastMousePosition;
  
  if (isInsideView) { onMouseDragged(customE); }
  
  if (!this->isMouseInside && isInsideView) { onMouseEnter(customE); }
  else if (this->isMouseInside && !isInsideView) { onMouseExit(customE); }
  
  this->isMouseInside = isInsideView;
  this->lastMousePosition = e;
}

void View::onMouseDragged(const MouseEventArgs &e)
{
  //  ofLogNotice("View('" + name + "')") << "onMouseDragged(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::mouseScrolledHandler(ofMouseEventArgs & e)
{
  //  ofLogNotice("View('" + name + "')") << "mouseReleasedHandler(ofMouseEventArgs & e)";
  
  bool isInsideView = this->inside(e);
  
  MouseEventArgs customE(e);
  customE.isOverlapped = false;
  
  if (isInsideView) { onMouseScrolled(customE); }
}

void View::onMouseScrolled(const MouseEventArgs &e)
{
  ofLogNotice("View('" + _name + "')") << "onMouseScroll(ofMouseEventArgs & e): Delta = " << e.scrollX << "," << e.scrollY;
}

#pragma mark - Mouse Handling
void View::bindKeyListeners()
{
  ofAddListener(ofEvents().keyPressed, this, &View::keyPressedHandler);
  ofAddListener(ofEvents().keyReleased, this, &View::keyReleasedHandler);
  ofAddListener(ofEvents().charEvent, this, &View::charTypedHandler);
}

void View::unbindKeyListeners()
{
  ofRemoveListener(ofEvents().keyPressed, this, &View::keyPressedHandler);
  ofRemoveListener(ofEvents().keyReleased, this, &View::keyReleasedHandler);
  ofRemoveListener(ofEvents().charEvent, this, &View::charTypedHandler);
}


void View::keyPressedHandler(ofKeyEventArgs & e)
{
  if (isFocused()) { onKeyPressed(e); }
}

void View::onKeyPressed(const ofKeyEventArgs & e)
{
  ofLogNotice("View('" + _name + "')") << "onKeyPressed(ofKeyEventArgs & e): Key = " << (char)e.key;
}

void View::keyReleasedHandler(ofKeyEventArgs & e)
{
  if (isFocused()) { onKeyReleased(e); }
}

void View::onKeyReleased(const ofKeyEventArgs & e)
{
//  ofLogNotice("View('" + name + "')") << "onKeyReleased(ofKeyEventArgs & e): Key = " << (char)e.key;
}

void View::charTypedHandler(uint32_t & c)
{
  if (isFocused()) { onCharTyped(c); }
}

void View::onCharTyped(const uint32_t & c)
{
//  ofLogNotice("View('" + name + "')") << "onKeyReleased(ofKeyEventArgs & e): Key = " << (char)e.key;
}

//
//
//void View::_mousePressedHandler(ofMouseEventArgs &e)
//{
//  bool isPressedInside = isInside(e);
//  bool isPressedInsideParent = hasParent() ? getParent()->isInside(e) : true;
//  
//  bool shouldTrigger = isPressedInside && (isPressedInsideParent || _enableInteractionOutsideParent);
//  
//  ofLogVerbose(_getLogModule(__FUNCTION__)) << std::boolalpha << "Pressed inside = " << isPressedInside << " Inside parent = " << isPressedInsideParent << " => Should trigger = " << shouldTrigger;
//  
//  MouseEventArgs customE(e);
//  customE.isOverlapped = isOverlapped(e);
//  
//  if (shouldTrigger) {
//    _triggerMousePressed(customE);
//    if (_includeInOverlap) View::focused = shared_from_this();
//  }
//  
//  _isMousePressed = true;
//  _wasMousePressedInside = isPressedInside;
//  _lastMousePosition = e;
//}
//
//void View::_mouseReleasedHandler(ofMouseEventArgs &e)
//{
//  bool isReleasedInside = isInside(e);
//  bool isReleasedInsideParent = hasParent() ? getParent()->isInside(e) : true;
//  bool shouldTrigger = isReleasedInside && (isReleasedInsideParent || _enableInteractionOutsideParent);
//  
//  MouseEventArgs customE(e);
//  customE.isOverlapped = isOverlapped(e);
//  
//  if (shouldTrigger) _triggerMouseReleased(customE);
//  if (_wasMousePressedInside && !isReleasedInside) _triggerMouseReleasedOutside(customE);
//  
//  _isMousePressed = false;
//  _wasMousePressedInside = false;
//  _wasMouseReleasedInside = isReleasedInside;
//}
//
//void View::_mouseMovedHandler(ofMouseEventArgs &e)
//{
//  bool isMovedInside = isInside(e);
//  bool isMovedInsideParent = hasParent() ? getParent()->isInside(e) : true;
//  bool shouldTrigger = isMovedInside && (isMovedInsideParent || _enableInteractionOutsideParent);
//  
//  auto deltaEvent = DeltaMouseEvent(e);
//  deltaEvent.isOverlapped = isOverlapped(e);
//  deltaEvent.delta = e - _lastMousePosition;
//  
//  if (shouldTrigger) _triggerMouseMoved(deltaEvent);
//  
//  if (!_isMouseOver && isMovedInside) {
//    _triggerMouseEnter(deltaEvent);
//  }
//  
//  if (_isMouseOver && !isMovedInside) {
//    _triggerMouseExit(deltaEvent);
//  }
//  
//  _isMouseOver = isMovedInside;
//  _lastMousePosition = e;
//}
//
//void View::_mouseDraggedHandler(ofMouseEventArgs &e)
//{
//  auto deltaEvent = DeltaMouseEvent(e);
//  deltaEvent.isOverlapped = isOverlapped(e);
//  deltaEvent.delta = e - _lastMousePosition;
//  
//  bool isDraggedInside = isInside(e);
//  bool isDraggedInsideParent = hasParent() ? getParent()->isInside(e) : true;
//  bool shouldTrigger = isDraggedInside && (isDraggedInsideParent || _enableInteractionOutsideParent);
//  
//  
//  if (shouldTrigger) _triggerMouseDragged(deltaEvent);
//  
//  if (!_isMouseOver && isDraggedInside) {
//    _triggerMouseEnter(deltaEvent);
//  }
//  
//  if (_isMouseOver && !isDraggedInside) {
//    _triggerMouseExit(deltaEvent);
//  }
//  
//  _isMouseOver = isDraggedInside;
//  _lastMousePosition = e;
//}
//
//void View::_mouseScrolledHandler(ofMouseEventArgs &e)
//{
//  bool isScrolledInside = isInside(e);
//  bool isScrolledInsideParent = hasParent() ? getParent()->isInside(e) : true;
//  bool shouldTrigger = isScrolledInside && (isScrolledInsideParent || _enableInteractionOutsideParent);
//  
//  MouseEventArgs event(e);
//  event.isOverlapped = isOverlapped(e);
//  if (shouldTrigger) _triggerMouseScrolled(event);
//}
//
//
//void View::_keyPressedHandler(ofKeyEventArgs &e)
//{
//  if (_isFocused()) _triggerKeyPressed(e);
//}
//
//void View::_keyReleasedHandler(ofKeyEventArgs &e)
//{
//  if (_isFocused()) _triggerKeyReleased(e);
//}
//
//void View::_charHandler(uint32_t &c)
//{
//  if (_isFocused()) _triggerCharTyped(c);
//}

}}
