#include "DisplayObject.h"

namespace ofxCortex { namespace ui {

shared_ptr<DisplayObject> DisplayObject::superview { nullptr };

DisplayObject::DisplayObject()
:
_parent(NULL),
_uuid(core::UUID()),
_isBaseObject(false),
_rotation(0.0f),
_scale(1.0f, 1.0f)
{
  _addMouseEventListeners();
  _addKeyEventListeners();
  
  setRect(ofRectangle(0, 0, 100, 32));
}

DisplayObject::~DisplayObject()
{
  _removeMouseEventListeners();
  _removeKeyEventListeners();
}

DisplayObject* DisplayObject::setPosition(const glm::vec2 & position)
{
  _rect.setPosition(glm::vec3(position, 0)); return this;
}

DisplayObject* DisplayObject::setLeft(float left, bool adjustLayout)
{
  ofRectangle originalRect = getRect();
  this->setX(left);
  
  float widthDiff = originalRect.getRight() - getRect().getRight();
  this->setWidth(this->getWidth() + widthDiff, adjustLayout);
  
  return this;
}

DisplayObject* DisplayObject::setTop(float top, bool adjustLayout)
{
  ofRectangle originalRect = getRect();
  this->setY(top);
  this->setHeight(this->getHeight() + (originalRect.getTop() - getRect().getTop()), adjustLayout);
  
  return this;
}

DisplayObject* DisplayObject::setWidth(float width, bool adjustLayout)
{
  _localRect.width = _rect.width = width;
  if (adjustLayout) this->_adjustLayout();
  
  return this;
}

DisplayObject* DisplayObject::setRight(float right, bool adjustLayout)
{
  ofRectangle originalRect = getRect();
  float diff = right - originalRect.getRight();
  float w = this->getWidth() + diff;
  this->setWidth(w, adjustLayout);
  
  return this;
}

DisplayObject* DisplayObject::alignRight(float right)
{
  this->setX(right - this->getWidth());
  
  return this;
}

DisplayObject* DisplayObject::setHeight(float height, bool adjustLayout)
{
  _localRect.height = _rect.height = height;
  if (adjustLayout) this->_adjustLayout();
  
  return this;
}

DisplayObject* DisplayObject::setBottom(float bottom, bool adjustLayout)
{
  ofRectangle originalRect = getRect();
  float h = bottom - originalRect.getBottom();
  this->setHeight(h, adjustLayout);
  
  return this;
}

DisplayObject* DisplayObject::alignBottom(float bottom)
{
  this->setY(bottom - this->getHeight());
  
  return this;
}

DisplayObject* DisplayObject::centerX(float x, bool adjustLayout)
{
  this->setX(x - (getWidth() * 0.5));
  if (adjustLayout) this->_adjustLayout();

  return this;
}

DisplayObject* DisplayObject::centerY(float y, bool adjustLayout)
{
  this->setY(y - (getHeight() * 0.5));
  if (adjustLayout) this->_adjustLayout();

  return this;
}

DisplayObject* DisplayObject::center(glm::vec2 pos, bool adjustLayout)
{
  this->setX(pos.x - (getWidth() * 0.5));
  this->setY(pos.y - (getHeight() * 0.5));
  if (adjustLayout) this->_adjustLayout();

  return this;
}

DisplayObject* DisplayObject::setSize(glm::vec2 size, bool adjustLayout)
{
  _localRect.width = _rect.width = size.x;
  _localRect.height = _rect.height = size.y;
  if (adjustLayout) this->_adjustLayout();
  
  return this;
}

DisplayObject* DisplayObject::setRect(const ofRectangle & rect, bool adjustLayout)
{
  _rect = rect;
  _localRect.width = rect.width;
  _localRect.height = rect.height;
  
  if (adjustLayout) this->_adjustLayout();
  
  return this;
}

ofRectangle DisplayObject::getGlobalRect()
{
  ofRectangle rect = getRect();
  rect.setPosition(glm::vec3(localToGlobal(glm::vec2()), 0));
  return rect;
}

glm::vec2 DisplayObject::localToGlobal(glm::vec2 pos) const
{
  auto parent = getParent();
  
  while (parent)
  {
    pos += parent->getPosition();
    parent = parent->getParent();
  }
  
  pos += getPosition();
  
  return pos;
}

glm::vec2 DisplayObject::globalToLocal(glm::vec2 pos, bool includeSelf) const
{
  auto parent = getParent();
  
  while (parent)
  {
    pos -= parent->getPosition();
    parent = parent->getParent();
  }
  
  if (includeSelf) pos -= getPosition();
  
  return pos;
}

void DisplayObject::_updateRect()
{
//  _rect.x = left.value();
//  _rect.y = top.value();
//  _rect.width = width.value();
//  _rect.height = height.value();
  
  _localRect.width = _rect.width;
  _localRect.height = _rect.height;
}

ofMatrix4x4 DisplayObject::_getTransformationMatrix() const
{
  ofMatrix4x4 m;
//  if (hasParent()) m = ofMatrix4x4::newTranslationMatrix(getParent()->getPosition()) * m;
  m = ofMatrix4x4::newTranslationMatrix(getPosition()) * m; //.setTranslation(getPosition());
  m = ofMatrix4x4::newScaleMatrix(glm::vec3(_scale.x, _scale.y, 0)) * m;
  m = ofMatrix4x4::newRotationMatrix(_rotation, 0, 0, 1) * m;

  return m;
}

ofMatrix4x4 DisplayObject::_getInverseTransformationMatrix() const
{
  ofMatrix4x4 m;
  m = ofMatrix4x4::newRotationMatrix(_rotation * -1.0, 0, 0, 1) * m;
  m = ofMatrix4x4::newScaleMatrix(glm::vec3(1.0f / _scale.x, 1.0f / _scale.y, 1.0f)) * m;
  m = ofMatrix4x4::newTranslationMatrix(getPosition() * -1.0) * m;
//  if (hasParent()) m = ofMatrix4x4::newTranslationMatrix(getParent()->getPosition() * -1.0) * m;

  return m;
}

glm::vec2 DisplayObject::_getNormalizedCoordinates(const glm::vec2 & pos) const
{
  glm::vec2 localPos = globalToLocal(pos);
  return { localPos.x / _localRect.width, localPos.y / _localRect.height };
}


#pragma mark - Rendering and Layout

void DisplayObject::draw()
{
  ofPushMatrix();
  ofMultMatrix(_getTransformationMatrix());
  {
    if (_useMask)
    {
      Stencil::beginDrawingMask();
      this->_drawMask();
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
    
    if (_doDrawChildren) _drawChildren();
    
    if (_useMask) Stencil::endUsingMask();
  }
  ofPopMatrix();
}

void DisplayObject::update(float time, float deltaTime)
{
  _update(time, deltaTime);
  
  for (auto & child : _children) child->update(_time, _deltaTime);
}

void DisplayObject::debug(bool debugChildren)
{
  ofPushMatrix();
  ofMultMatrix(_getTransformationMatrix());
  {
    ofPushStyle();
    {
      _debug();
    }
    ofPopStyle();
    if (debugChildren) for (auto & child : _children) child->debug(debugChildren);
  }
  ofPopMatrix();
}

void DisplayObject::debugLocal(bool debugChildren)
{
  ofPushStyle();
  {
    _debug();
  }
  ofPopStyle();
  if (debugChildren) for (auto & child : _children) child->debugLocal(debugChildren);
}

void DisplayObject::_draw()
{
  
}

void DisplayObject::_drawChildren()
{
  for (auto & child : _children) child->draw();
}

void DisplayObject::_drawMask()
{
  ofPushStyle();
  {
    ofFill();
    ofSetColor(255);
    ofDrawRectangle(_localRect);
  }
  ofPopStyle();
}

void DisplayObject::_update(float time, float deltaTime)
{
  _time = time;
  _deltaTime = deltaTime;
}

void DisplayObject::_adjustLayout()
{
  for (auto & child : _children) child->_adjustLayout();
}

void DisplayObject::_debug()
{
  ofNoFill();
  (_isMouseDown && _isMouseOver) ? ofSetLineWidth(2) : ofSetLineWidth(1);
  (_isMouseOver) ? ofSetColor(ofColor::red, 255 * 0.4) : ofSetColor(ofColor::red, 255 * 0.25);
  ofDrawRectangle(_localRect);
  
  string ID = (_name != "") ? _name : _uuid;
  ofDrawBitmapString(ID, 8, 13);
}



#pragma mark - The Family

void DisplayObject::makeSuperview()
{
  this->superview = shared_from_this();
  
  _addMouseEventListeners();
  _addKeyEventListeners();
}

int DisplayObject::getLevel() const
{
  if (!hasParent()) return 0;
  
  int level = hasParent();
  
  auto parent = getParent();
  while (parent->hasParent())
  {
    parent = parent->getParent();
    level++;
  }
  
  return level;
}

void DisplayObject::addChild(shared_ptr<DisplayObject> child)
{
  this->addChildAt(child, _children.size());
}

void DisplayObject::addChildren(vector<shared_ptr<DisplayObject>> children)
{
  for (auto & child : children) addChild(child);
}

void DisplayObject::addChildAt(shared_ptr<DisplayObject> child, unsigned int index)
{
  if (child->getParent().get() == this) return; // Child is already a child of this
  if (child->hasParent()) child->getParent()->removeChild(child);
  
  _children.insert(_children.begin() + ofClamp(index, 0, _children.size()), child);
  child->_setParent(shared_from_this());
}

void DisplayObject::removeChild(shared_ptr<DisplayObject> child)
{
  child->_setParent(nullptr);
  _children.erase(remove(_children.begin(), _children.end(), child), _children.end());
}

void DisplayObject::removeChildAt(unsigned int index)
{
  if (index < 0 || index >= _children.size()) { ofLogWarning("DisplayObject") << "Index is out of bounds. Returning.."; return; }
  
  _children[index]->_setParent(nullptr);
  _children.erase(_children.begin() + index);
}

void DisplayObject::removeAllChildren()
{
  for (auto & child : _children) child->_setParent(nullptr);
  _children.clear();
}

vector<shared_ptr<DisplayObject>> DisplayObject::children(bool recursive) const
{
  if (!recursive) return _children;
  
  vector<shared_ptr<DisplayObject>> children = _children;
  for (auto & child : _children)
  {
    auto grandchildren = child->children(recursive);
    children.insert(children.end(), grandchildren.begin(), grandchildren.end());
  }
  
  return children;
}

size_t DisplayObject::numChildren(bool recursive) const
{
  size_t count = _children.size();

  if (recursive)
  {
    for (const auto & child : children())
    {
      count += child->numChildren(true);
    }
  }

  return count;
}

#pragma mark - Events and Interaction

void DisplayObject::enableMouseEvents()
{
  _addMouseEventListeners();
}

void DisplayObject::disableMouseEvents()
{
  _removeMouseEventListeners();
}

void DisplayObject::enableKeyEvents()
{
  _addKeyEventListeners();
}

void DisplayObject::disableKeyEvents()
{
  _removeKeyEventListeners();
}

void DisplayObject::enableEvents(bool propagate)
{
  enableMouseEvents();
  enableKeyEvents();
  _isEventsEnabled = true;
  
  if (propagate) {
    for (auto & child : _children) child->enableEvents(propagate);
  }
}

void DisplayObject::disableEvents(bool propagate)
{
  disableMouseEvents();
  disableKeyEvents();
  _isEventsEnabled = false;
  
  if (propagate) {
    for (auto & child : _children) child->disableEvents(propagate);
  }
}

void DisplayObject::onMousePressed(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMousePressedE.newListener(callback, 0));
}

void DisplayObject::onMouseReleased(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMouseReleasedE.newListener(callback, 0));
}

void DisplayObject::onMouseReleasedOutside(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMouseReleasedOutsideE.newListener(callback, 0));
}

void DisplayObject::onMouseDragged(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMouseDraggedE.newListener(callback, 0));
}

void DisplayObject::onMouseEnter(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMouseEnterE.newListener(callback, 0));
}

void DisplayObject::onMouseExit(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMouseExitE.newListener(callback, 0));
}

void DisplayObject::onMouseScrolled(function<void (ofMouseEventArgs)> callback)
{
  _eventListeners.push(onMouseScrolledE.newListener(callback, 0));
}

void DisplayObject::onKeyPressed(function<void (ofKeyEventArgs)> callback)
{
  _eventListeners.push(onKeyPressedE.newListener(callback, 0));
}


void DisplayObject::_addDrawEventListener()
{
  ofAddListener(ofEvents().draw, this, &DisplayObject::_drawHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_removeDrawEventListener()
{
  ofRemoveListener(ofEvents().draw, this, &DisplayObject::_drawHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_addUpdateEventListener()
{
  ofAddListener(ofEvents().update, this, &DisplayObject::_updateHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_removeUpdateEventListener()
{
  ofRemoveListener(ofEvents().update, this, &DisplayObject::_updateHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_addMouseEventListeners()
{
  ofAddListener(ofEvents().mousePressed, this, &DisplayObject::_mousePressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseReleased, this, &DisplayObject::_mouseReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseMoved, this, &DisplayObject::_mouseMovedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseDragged, this, &DisplayObject::_mouseDraggedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseScrolled, this, &DisplayObject::_mouseScrolledHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_removeMouseEventListeners()
{
  ofRemoveListener(ofEvents().mousePressed, this, &DisplayObject::_mousePressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseReleased, this, &DisplayObject::_mouseReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseMoved, this, &DisplayObject::_mouseMovedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseDragged, this, &DisplayObject::_mouseDraggedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().mouseScrolled, this, &DisplayObject::_mouseScrolledHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_addKeyEventListeners()
{
  ofAddListener(ofEvents().keyPressed, this, &DisplayObject::_keyPressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().keyReleased, this, &DisplayObject::_keyReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().charEvent, this, &DisplayObject::_charHandler, OF_EVENT_ORDER_AFTER_APP);
}

void DisplayObject::_removeKeyEventListeners()
{
  ofRemoveListener(ofEvents().keyPressed, this, &DisplayObject::_keyPressedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().keyReleased, this, &DisplayObject::_keyReleasedHandler, OF_EVENT_ORDER_AFTER_APP);
  ofRemoveListener(ofEvents().charEvent, this, &DisplayObject::_charHandler, OF_EVENT_ORDER_AFTER_APP);
}


void DisplayObject::_updateHandler(ofEventArgs &e)
{
  update(ofGetElapsedTimef(), ofGetLastFrameTime());
}

void DisplayObject::_drawHandler(ofEventArgs &e)
{
  draw();
}

void DisplayObject::_mousePressedHandler(ofMouseEventArgs &e)
{
  if (ofGetCurrentViewport().inside(e))
  {
    _isMouseDown = true;
    
    bool isInsideRect = isInsideRectangle(e, true);
    bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e, true) : false;
    bool isInteractionOutsideParentAllowed = (hasParent()) ? getParent()->_isChildInteractionOutsideRectEnabled : true;
    bool isCovered = this->_isCovered(e);
    
    if (isInsideRect)
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        if (ofGetElapsedTimeMillis() - _lastMousePressTime <= _doublePressInterval) // Double press
        {
          _mouseDoublePressed(e);
          onMousePressedE.notify(this, e);
        }
        else // Single press
        {
          _mousePressed(e);
          onMousePressedE.notify(this, e);
        }
        
        _isMousePressedInside = true;
        _lastMousePressTime = ofGetElapsedTimeMillis();
      }
    }
  }
}

void DisplayObject::_mouseReleasedHandler(ofMouseEventArgs &e)
{
  bool isInsideRect = isInsideRectangle(e);
  bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e, true) : false;
  bool isInteractionOutsideParentAllowed = (hasParent()) ? getParent()->_isChildInteractionOutsideRectEnabled : true;
  
  if (isInsideRect)
  {
    if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
    else
    {
      _mouseReleased(e);
      onMouseReleasedE.notify(this, e);
    }
  }
  else if (!isInsideRect && (_isMousePressedInside || _hasEnteredWhileDragged))
  {
    if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
    else
    {
      _mouseReleasedOutside(e);
      onMouseReleasedOutsideE.notify(this, e);
    }
  }
  
  _isMouseDown = false;
  _isMousePressedInside = false;
  _hasEnteredWhileDragged = false;
}

void DisplayObject::_mouseMovedHandler(ofMouseEventArgs &e)
{
  if (ofGetCurrentViewport().inside(e))
  {
    bool isInsideRect = isInsideRectangle(e, true);
    bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e, true) : false;
    bool isInteractionOutsideParentAllowed = (hasParent()) ? getParent()->_isChildInteractionOutsideRectEnabled : true;
    
    if (isInsideRect && !_isMouseOver) // Trigger mouseEnterEvents;
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseEnter(e);
        onMouseEnterE.notify(this, e);
      }
    }
    
    if (!isInsideRect && _isMouseOver)
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseExit(e);
        onMouseExitE.notify(this, e);
      }
    }
    
    if (isInsideRect)
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseMoved(e);
        onMouseMovedE.notify(this, e);
      }
    }
    
    _isMouseOver = isInsideRect;
  }
}

void DisplayObject::_mouseDraggedHandler(ofMouseEventArgs &e)
{
  if (ofGetCurrentViewport().inside(e))
  {
    bool isInsideRect = isInsideRectangle(e, true);
    bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e, true) : false;
    bool isInteractionOutsideParentAllowed = (hasParent()) ? getParent()->_isChildInteractionOutsideRectEnabled : true;
    
    _isMouseDown = true;
    
    if (isInsideRect && !_isMouseOver) // Trigger mouseEnterEvents;
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseEnter(e);
        onMouseEnterE.notify(this, e);
        _hasEnteredWhileDragged = true;
      }
    }
    
    if (!isInsideRect && _isMouseOver)
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseExit(e);
        onMouseExitE.notify(this, e);
      }
    }
    
    if (isInsideRect)
    {
      _isMouseOver = true;
      
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseDragged(e);
        onMouseDraggedE.notify(this, e);
      }
    }
    else _isMouseOver = false;
  }
  else
  {
    if (_isMouseOver)
    {
      _mouseExit(e);
      onMouseExitE.notify(this, e);
      _isMouseOver = false;
    }
    
  }
}

void DisplayObject::_mouseScrolledHandler(ofMouseEventArgs &e)
{
  if (ofGetCurrentViewport().inside(e))
  {
    bool isInsideRect = isInsideRectangle(e, true);
    bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e, true) : false;
    bool isInteractionOutsideParentAllowed = (hasParent()) ? getParent()->_isChildInteractionOutsideRectEnabled : true;
    
    if (isInsideRect)
    {
      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
      else
      {
        _mouseScrolled(e);
        onMouseScrolledE.notify(this, e);
      }
    }
    
  }
}


void DisplayObject::_keyPressedHandler(ofKeyEventArgs &e)
{
  _keyPressed(e);
  onKeyPressedE.notify(this, e);
}

void DisplayObject::_keyReleasedHandler(ofKeyEventArgs &e)
{
  _keyReleased(e);
  onKeyReleasedE.notify(this, e);
}

void DisplayObject::_charHandler(uint32_t &c)
{
  _charTyped(c);
  onCharTypedE.notify(this, c);
}


// Hooks
void DisplayObject::_mousePressed(const ofMouseEventArgs &args)
{
  ofLogVerbose(_getLogModule()) << "Mouse Pressed! Pos: " << args << " Button: " << args.button;
}

void DisplayObject::_mouseDoublePressed(const ofMouseEventArgs &args)
{
  ofLogVerbose(_getLogModule()) << "Mouse Double Pressed! Pos: " << args << " Button: " << args.button;
}

void DisplayObject::_mouseReleased(const ofMouseEventArgs & args)
{
  ofLogVerbose(_getLogModule()) << "Mouse Released! Pos: " << args << " Button: " << args.button;
}

void DisplayObject::_mouseReleasedOutside(const ofMouseEventArgs & args)
{
  ofLogVerbose(_getLogModule()) << "Mouse Released Outside! Pos: " << args << " Button: " << args.button;
}

void DisplayObject::_mouseMoved(const ofMouseEventArgs & args)
{
//  ofLogVerbose(_getLogModule()) << "Mouse Moved! Pos: " << args << " Button: " << args.button;
}

void DisplayObject::_mouseDragged(const ofMouseEventArgs & args)
{
  ofLogVerbose(_getLogModule()) << "Mouse Dragged! Pos: " << args << " Button: " << args.button;
}

void DisplayObject::_mouseScrolled(const ofMouseEventArgs & args)
{
  ofLogVerbose(_getLogModule()) << "Mouse Scrolled! Delta: " << args.scrollX << ", " << args.scrollY;
}

void DisplayObject::_mouseEnter(const ofMouseEventArgs &e)
{
  ofLogVerbose(_getLogModule()) << "Mouse Entered! Pos: " << e;
}

void DisplayObject::_mouseExit(const ofMouseEventArgs &e)
{
  ofLogVerbose(_getLogModule()) << "Mouse Exit! Pos: " << e;
}

void DisplayObject::_keyPressed(const ofKeyEventArgs &e)
{
  ofLogVerbose(_getLogModule()) << "Key Pressed! Key: " << e.key << " Keycode: " << e.keycode;
}

void DisplayObject::_keyReleased(const ofKeyEventArgs &e)
{
  ofLogVerbose(_getLogModule()) << "Key Released! Key: " << e.key << " Keycode: " << e.keycode;
}

void DisplayObject::_charTyped(const uint32_t &c)
{
  ofLogVerbose(_getLogModule()) << "Char Typed! Char: " << c;
}

bool DisplayObject::isInsideRectangle(glm::vec2 pos, bool useGlobalRect)
{
  ofRectangle rect = (useGlobalRect) ? getGlobalRect() : getRect();
  return (pos.x >= rect.getLeft() && pos.x <= rect.getRight() && pos.y >= rect.getTop() && pos.y <= rect.getBottom());
}

bool DisplayObject::isInsideRectangle(const ofRectangle & incoming, bool useGlobalRect)
{
  ofRectangle rect = (useGlobalRect) ? getGlobalRect() : getRect();
  return (incoming.getLeft() >= rect.getLeft() && incoming.getRight() <= rect.getRight() && incoming.getTop() >= rect.getTop() && incoming.getBottom() <= rect.getBottom());
}

bool DisplayObject::_isInsideLocalRect(glm::vec2 pos)
{
  ofRectangle rect = _localRect;
  return (pos.x >= rect.getLeft() && pos.x <= rect.getRight() && pos.y >= rect.getTop() && pos.y <= rect.getBottom());
}

bool DisplayObject::_isHovering(glm::vec2 pos, bool isCovered)
{
  return false;
}

bool DisplayObject::_isCovered(const glm::vec2 & pos)
{
  for (const auto & child : children(true))
  {
    if (child->isInsideRectangle(pos, true)) return true;
  }
  
  return false;
}

}}

