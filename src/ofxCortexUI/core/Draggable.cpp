#include "Draggable.h"

namespace ofxCortex { namespace ui {
  
Draggable::Draggable(ofRectangle dragBounds) :
_dragBounds(dragBounds)
{
  
}

glm::vec2 Draggable::getNormalizedPosition()
{
  glm::vec2 norm = this->getPosition() / glm::vec2(_dragBounds.width - getWidth(), _dragBounds.height - getHeight());
  float x = ofMap(getX(), _dragBounds.getLeft(), _dragBounds.getRight() - getWidth(), 0, 1, true);
  float y = ofMap(getY(), _dragBounds.getTop(), _dragBounds.getBottom() - getHeight(), 0, 1, true);
  return glm::vec2(x, y); //glm::clamp(glm::vec2(, glm::vec2(0, 0), glm::vec2(1, 1));
}

void Draggable::setDragBounds(const ofRectangle & rect)
{
  _dragBounds = rect;
  
//  if (selfRect.x < _dragBounds.x) setX(_dragBounds.x);
//  if (selfRect.y < _dragBounds.y) setY(_dragBounds.y);
  
//  if (selfRect.width > _dragBounds.width) setWidth(_dragBounds.width);
//  if (selfRect.height > _dragBounds.height) setHeight(_dragBounds.height);
//
//  if (selfRect.getLeft() < _dragBounds.getLeft()) setX(_dragBounds.getLeft());
//  if (selfRect.getRight() > _dragBounds.getRight()) setX(_dragBounds.getRight() - selfRect.width);
//  if (selfRect.getTop() < _dragBounds.getTop()) setY(_dragBounds.getTop());
//  if (selfRect.getBottom() > _dragBounds.getBottom()) setY(_dragBounds.getBottom() - selfRect.height);
}

void Draggable::setFromNormalizedCoord(const glm::vec2 & coord)
{
  this->setFromNormalizedX(coord.x);
  this->setFromNormalizedY(coord.y);
//  this->setPosition(coord * glm::vec2(_dragBounds.width - this->getWidth(), _dragBounds.height - this->getHeight()));
}

void Draggable::_debug()
{
  DisplayObject::_debug();
  
  ofPushMatrix();
  {
    ofMultMatrix(_getInverseTransformationMatrix());
    ofPushStyle();
    {
      ofNoFill();
      ofSetColor(ofColor::yellow, 255);
      ofDrawRectangle(_dragBounds);
    }
    ofPopStyle();
  }
  ofPopMatrix();
}

void Draggable::_mousePressed(const ofMouseEventArgs & e)
{
  DisplayObject::_mousePressed(e);
  
  _lastDragPosition = e;
  _pressedOffset = globalToLocal(e);
  _isDragging = true;
}

void Draggable::_mouseDragged(const ofMouseEventArgs & e)
{
  DisplayObject::_mouseDragged(e);
  
//  DraggableEventArgs event;
//  event.type = e.type;
//  event.button = e.button;
//  event.x = e.x;
//  event.y = e.y;
//  event.modifiers = e.modifiers;
//
//  glm::vec2 delta = e - _lastDragPosition;
//  event.delta = delta;
//
//  if (_ignoreX) delta.x = 0;
//  if (_ignoreY) delta.y = 0;
//  ofRectangle potentialRect = this->getRect();
//  potentialRect.translate(delta);
//
//  float epsilon = 0.5f;
//  if (_isDragging && potentialRect.getLeft() >= _dragBounds.getLeft() - epsilon && potentialRect.getRight() <= _dragBounds.getRight() + epsilon && potentialRect.getTop() >= _dragBounds.getTop() - epsilon && potentialRect.getBottom() <= _dragBounds.getBottom() + epsilon)
//  {
//    this->translate(delta);
//
//    _onDrag(e, event);
//    onDragE.notify(this, event);
//  }
//
//  _lastDragPosition = e;
}

void Draggable::_update(float time, float deltaTime)
{
  DisplayObject::_update(time, deltaTime);

  glm::vec2 localMouse = globalToLocal(glm::vec2(ofGetMouseX(), ofGetMouseY()));
  ofMouseEventArgs e(ofMouseEventArgs::Dragged, localMouse.x, localMouse.y, OF_MOUSE_BUTTON_LEFT);

  if (!ofGetCurrentViewport().inside(e)) return;

  glm::vec2 delta = e - _lastDragPosition;
  if (_ignoreX) delta.x = 0;
  if (_ignoreY) delta.y = 0;
  ofRectangle potentialRect = this->getRect();
//  potentialRect.translate(delta);
  
  if (!_ignoreX) potentialRect.setX(e.x - _pressedOffset.x);
  if (!_ignoreY) potentialRect.setY(e.y - _pressedOffset.y);
  
  cout << "Mouse down: " << _isMouseDown << " Delta: " << delta << " New position: " << e - _pressedOffset << endl;

  float epsilon = 0.5f;
  if (_isMouseDown && potentialRect.getLeft() >= _dragBounds.getLeft() - epsilon && potentialRect.getRight() <= _dragBounds.getRight() + epsilon && potentialRect.getTop() >= _dragBounds.getTop() - epsilon && potentialRect.getBottom() <= _dragBounds.getBottom() + epsilon)
  {
    this->translate(delta);

    _onDrag(e, delta);
//    onDragE.notify(this, delta);
  }
  
  _lastDragPosition = e;
}

}}
