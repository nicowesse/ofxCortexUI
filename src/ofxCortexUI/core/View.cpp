#include "View.h"

namespace ofxCortex { namespace ui {

#pragma mark - Public
View::View(const std::string & name)
{
//  ofLogVerbose(toString(__FUNCTION__)) << "Name = " << name;
  
  setName(name);
  enableMask();
  
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
    
//    kiwi::Constraint { intrinsic_height == Styling::defaultRowHeight() | kiwi::strength::required }, // Could be problematic
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
  this->bindWindowResizedListener();
}

View::~View()
{
  this->unbindEvents();
  
  LayoutEngine::removeConstraints(baseConstraints);
  this->clearConstraints();
  subviews.clear();
  
//  View::everyView.erase(std::remove(View::everyView.begin(), View::everyView.end(), getSelf()), View::everyView.end());
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
  ofPushStyle();
  if (this->isFocused()) { Styling::drawFocusBorder(this->frame); }
  else { Styling::drawBorder(this->frame); }
  ofPopStyle();
  
  if (this->isDebugEnabled) { this->onDebug(); }
}

void View::onDrawMask()
{
  ofPushStyle();
  ofFill();
  ofSetColor(255, 128);
  ofDrawRectangle(this->frame);
  ofPopStyle();
}

void View::onDrawSubviews()
{
  for (auto & view : subviews) view->draw();
}

void View::onDebug()
{
  this->onDrawMask();
  
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
  
  this->setNeedsLayout();
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
  ofLogVerbose(toString(__FUNCTION__)); // << kiwi::debug::dumps(constraint);
  
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

void View::clearConstraints()
{
  ofLogVerbose(toString(__FUNCTION__)) << "Constraints = " << constraints.size();
  
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
  ofLogVerbose(_getLogModule(__FUNCTION__));
}

void View::layoutSubviews()
{
  ofLogVerbose(_getLogModule(__FUNCTION__));
}

ofRectangle View::getFrame() const
{
  return ofRectangle(left.value(), top.value(), width.value(), height.value());
}

ofRectangle View::getContentFrame() const
{
  return ofRectangle(content_left.value(), content_top.value(), content_width.value(), content_height.value());
}

ofRectangle View::getLocalFrame() const {
  return ofRectangle(0, 0, width.value(), height.value());
}

ofRectangle View::getLocalContentFrame() const {
  return ofRectangle(0, 0, content_width.value(), content_height.value());
}

std::string View::_getLogModule(const std::string & func) const
{
  std::stringstream logModule;
  logModule << _getLevel() << _getComponentName() << "('" << getName() << "')[" << ofGetFrameNum() << "]::" << func << "()";
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
  ofLogVerbose("View('" + _name + "')::bindEvents()");
  
  this->bindUpdateListener();
  this->bindDrawListener();
  this->bindMouseListeners();
  this->bindKeyListeners();
  this->bindWindowResizedListener();
}

void View::unbindEvents()
{
  ofLogVerbose("View('" + _name + "')::unbindEvents()");
  
  this->unbindUpdateListener();
  this->unbindDrawListener();
  this->unbindMouseListeners();
  this->unbindKeyListeners();
  this->unbindWindowResizedListener();
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
  ofRemoveListener(ofEvents().mouseMoved, this, &View::mouseMovedHandler);
  ofRemoveListener(ofEvents().mouseDragged, this, &View::mouseDraggedHandler);
  ofRemoveListener(ofEvents().mouseScrolled, this, &View::mouseScrolledHandler);
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
  ofLogVerbose("View('" + _name + "')") << "onMousePressed(ofMouseEventArgs & e) " << e.x << "," << e.y;
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
  ofLogVerbose("View('" + _name + "')") << "onMouseReleased(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::onMouseReleasedOutside(const MouseEventArgs &e)
{
  ofLogVerbose("View('" + _name + "')") << "onMouseReleasedOutside(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::mouseMovedHandler(ofMouseEventArgs & e)
{
  //  ofLogVerbose("View('" + name + "')") << "mouseReleasedHandler(ofMouseEventArgs & e)";
  
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
  //  ofLogVerbose("View('" + name + "')") << "onMouseMoved(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::onMouseEnter(const MouseEventArgs &e)
{
  ofLogVerbose("View('" + _name + "')") << "onMouseEnter(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::onMouseExit(const MouseEventArgs &e)
{
  ofLogVerbose("View('" + _name + "')") << "onMouseExit(ofMouseEventArgs & e) " << e.x << "," << e.y;
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
  //  ofLogVerbose("View('" + name + "')") << "onMouseDragged(ofMouseEventArgs & e) " << e.x << "," << e.y;
}

void View::mouseScrolledHandler(ofMouseEventArgs & e)
{
  //  ofLogVerbose("View('" + name + "')") << "mouseReleasedHandler(ofMouseEventArgs & e)";
  
  bool isInsideView = this->inside(e);
  
  MouseEventArgs customE(e);
  customE.isOverlapped = false;
  
  if (isInsideView) { onMouseScrolled(customE); }
  this->setNeedsLayout();
}

void View::onMouseScrolled(const MouseEventArgs &e)
{
  ofLogVerbose("View('" + _name + "')") << "onMouseScroll(ofMouseEventArgs & e): Delta = " << e.scrollX << "," << e.scrollY;
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
  ofLogVerbose("View('" + _name + "')") << "onKeyPressed(ofKeyEventArgs & e): Key = " << (char)e.key;
}

void View::keyReleasedHandler(ofKeyEventArgs & e)
{
  if (isFocused()) { onKeyReleased(e); }
}

void View::onKeyReleased(const ofKeyEventArgs & e)
{
//  ofLogVerbose("View('" + name + "')") << "onKeyReleased(ofKeyEventArgs & e): Key = " << (char)e.key;
}

void View::charTypedHandler(uint32_t & c)
{
  if (isFocused()) { onCharTyped(c); }
}

void View::onCharTyped(const uint32_t & c)
{
//  ofLogVerbose("View('" + name + "')") << "onKeyReleased(ofKeyEventArgs & e): Key = " << (char)e.key;
}

void View::windowResizedHandler(ofResizeEventArgs & e)
{
  this->onWindowResized(e);
  this->setNeedsLayout();
}

void View::onWindowResized(const ofResizeEventArgs & e)
{
  ofLogNotice(_getLogModule(__FUNCTION__));
  
  this->setIntrinsicSize(Styling::getScaled(288), Styling::getRowHeight(1));
}

}}
