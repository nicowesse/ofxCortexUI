#pragma once

#include "ofxCortexUI/core/DisplayObject.h"

namespace ofxCortex { namespace ui {
  
class Draggable : public DisplayObject {
public:
  struct DraggableEventArgs : public ofMouseEventArgs {
    glm::vec2 delta;
  };
  
public:
  
  Draggable(ofRectangle dragBounds = ofRectangle(0, 0, 1, 1));
  static shared_ptr<Draggable> create(ofRectangle dragBounds = ofRectangle(0, 0, 1, 1)) { return make_shared<Draggable>(dragBounds); }
  
  ofEvent<DraggableEventArgs> onDragE;
  void onDrag(function<void(DraggableEventArgs)> callback)
  { _eventListeners.push(onDragE.newListener(callback, 0)); };
  
  const ofRectangle & getDragBounds() const { return _dragBounds; }
  void setDragBounds(const ofRectangle & rect);
  
  glm::vec2 getNormalizedPosition();
  void setFromNormalizedCoord(const glm::vec2 & coord);
  void setFromNormalizedX(float x) { this->setX(ofMap(x, 0, 1, _dragBounds.getLeft(), _dragBounds.getRight() - this->getWidth(), true)); };
  void setFromNormalizedY(float y) { this->setY(ofMap(y, 0, 1, _dragBounds.getTop(), _dragBounds.getBottom() - this->getHeight(), true)); };
  
  void ignoreX() { _ignoreX = true; }
  void ignoreY() { _ignoreY = true; }
  
protected:
  virtual string _getModule() const override { return "Draggable"; };
  
  virtual void _update(float time, float delta) override;
  virtual void _debug() override;
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override;
  virtual void _mouseDragged(const ofMouseEventArgs & e) override;
  virtual void _mouseReleased(const ofMouseEventArgs & e) override { _isDragging = false; }
  virtual void _mouseExit(const ofMouseEventArgs & e) override { _isDragging = false; };
  
  virtual void _onDrag(const ofMouseEventArgs & e, glm::vec2 delta) {};
  
  ofRectangle _dragBounds;
  bool _isDragging { false };
  bool _ignoreX { false };
  bool _ignoreY { false };
  glm::vec2 _lastDragPosition;
  glm::vec2 _pressedOffset;
};

}}
