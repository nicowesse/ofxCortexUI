#pragma once
#include "View.h"
#include "ofEvent.h"
#include "ofEventUtils.h"

namespace ofxCortex { namespace ui {

class ResizeableView : public View {
public:
  ResizeableView(std::string name) : View(name)
  {
    ofLogVerbose(_getLogModule(__FUNCTION__));
    
//    this->setIntrinsicSize(28, 20);
  }
  
  static std::shared_ptr<ResizeableView> create(const std::string & name, ofRectangle rect = ofRectangle(0, 0, 288, 40))
  {
    auto instance = std::make_shared<ResizeableView>(name);
    instance->_setup();
    return instance;
  }
  
  void enableX()  { _enableX = true; }
  void disableX() { _enableX = false; }
  void enableY()  { _enableY = true; }
  void disableY() { _enableY = false; }
  
  void constrainToView(std::shared_ptr<View> view)
  {
    this->constrainingView = view;
    
    this->addConstraints({
      kiwi::Constraint { this->left >= view->content_left | kiwi::strength::strong },
      kiwi::Constraint { this->right <= view->content_right | kiwi::strength::strong },
      kiwi::Constraint { this->top >= view->content_top | kiwi::strength::strong },
      kiwi::Constraint { this->bottom <= view->content_bottom | kiwi::strength::strong },
    });
  }
  
  glm::vec2 getMinSize() const { return { _minWidth, _minHeight}; }
  void setGrabAreaDimensions(glm::vec2 dimensions) { _grabAreaDimensions = dimensions; }
  
  struct ResizeEvent : public ofEventArgs {
    ofRectangle bounds;
  };
  ofEvent<ResizeEvent> onResizedE;
  void onResized(std::function<void(ResizeEvent)> callback) { _eventListeners.push(onResizedE.newListener(callback, 0)); }
  
protected:
  virtual std::string _getComponentName() const override { return "ResizeableView"; };
  
  void _setup()
  {
    this->_updateConstraintsImmediately();
  }
  
  virtual void _draw() override
  {
    const auto & BB = this->getBounds();
    ofPushStyle();
    
    ofFill();
    ofSetColor(ofColor::white, 20);
    ofDrawRectRounded(BB, 3);
    
    float verticalHintHeight = MIN(BB.height - 12, 12);
    float horizontalHintWidth = MIN(BB.width - 12, 12);
    
    ofSetColor(ofxCortex::ui::Styling::getForegroundColor());
    if (_enableX)
    {
      ofDrawRectRounded(BB.getLeft() + 4, BB.getCenter().y - (verticalHintHeight / 2.0), 2, verticalHintHeight, 2); // Left Hint
      ofDrawRectRounded(BB.getRight() - 4, BB.getCenter().y - (verticalHintHeight / 2.0), -2, verticalHintHeight, 2); // Right Hint
    }
    
    if (_enableY)
    {
      ofDrawRectRounded(BB.getCenter().x - (horizontalHintWidth / 2.0f), BB.getTop() + 4, horizontalHintWidth, 2, 2); // Top Hint
      ofDrawRectRounded(BB.getCenter().x - (horizontalHintWidth / 2.0f), BB.getBottom() - 4, horizontalHintWidth, -2, 2);
    }
    
    ofPopStyle();
  }
  
  virtual void _updateConstraints() override
  {
    View::_updateConstraints();
    
    this->addConstraints({
      { width >= _minWidth | kiwi::strength::strong },
      { height >= _minHeight | kiwi::strength::strong }
    });
  }
  
  void _translateLeft(float value)
  {
    if (_limitLeft(value)) return;
    
    LayoutEngine::suggestValue(this->left, this->left.value() + value);
    LayoutEngine::suggestValue(this->width, this->width.value() - value);
    this->setNeedsLayout();
    
    ResizeEvent e;
    e.bounds.x = this->left.value() + value;
    e.bounds.y = this->top.value();
    e.bounds.width = this->width.value() - value;
    e.bounds.height = this->height.value();
    onResizedE.notify(e);
  }
  bool _limitLeft(float deltaX) { return this->constrainingView && this->left.value() + deltaX < constrainingView->content_left.value(); }
  
  void _translateTop(float value)
  {
    if (_limitTop(value)) return;
    
    LayoutEngine::suggestValue(this->top, this->top.value() + value);
    LayoutEngine::suggestValue(this->height, this->height.value() - value);
    this->setNeedsLayout();
    
    ResizeEvent e;
    e.bounds.x = this->left.value();
    e.bounds.y = this->top.value() + value;
    e.bounds.width = this->width.value();
    e.bounds.height = this->height.value() - value;
    onResizedE.notify(e);
  }
  bool _limitTop(float deltaY) { return this->constrainingView && this->top.value() + deltaY < constrainingView->content_top.value(); }
  
  void _translateRight(float value)
  {
    if (_limitRight(value)) return;
    
    LayoutEngine::suggestValue(this->width, this->width.value() + value);
    this->setNeedsLayout();
    
    ResizeEvent e;
    e.bounds.x = this->left.value();
    e.bounds.y = this->top.value();
    e.bounds.width = this->width.value() + value;
    e.bounds.height = this->height.value();
    onResizedE.notify(e);
  }
  
  bool _limitRight(float deltaX) { return this->constrainingView && this->left.value() + this->width.value() + deltaX > constrainingView->content_right.value(); }
  
  void _translateBottom(float value)
  {
    if (_limitBottom(value)) return;
    
    LayoutEngine::suggestValue(this->height, this->height.value() + value);
    this->setNeedsLayout();
    
    ResizeEvent e;
    e.bounds.x = this->left.value();
    e.bounds.y = this->top.value();
    e.bounds.width = this->width.value();
    e.bounds.height = this->height.value() + value;
    onResizedE.notify(e);
  }
  bool _limitBottom(float deltaY) { return this->constrainingView && this->top.value() + this->height.value() + deltaY > constrainingView->content_bottom.value(); }
  
  ofAlignHorz dragAnchorHorz = OF_ALIGN_HORZ_IGNORE;
  ofAlignVert dragAnchorVert = OF_ALIGN_VERT_IGNORE;
  bool _enableX { true };
  bool _enableY { true };
  float _minWidth { 28 };
  float _minHeight { 20 };
  std::shared_ptr<View> constrainingView;
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override
  {
    if (abs(e.x - this->getLeft()) < _grabAreaDimensions.x && _enableX) dragAnchorHorz = OF_ALIGN_HORZ_LEFT;
    if (abs(e.x - this->getRight()) < _grabAreaDimensions.x && _enableX) dragAnchorHorz = OF_ALIGN_HORZ_RIGHT;
    if (abs(e.y - this->getTop()) < _grabAreaDimensions.y && _enableY) dragAnchorVert = OF_ALIGN_VERT_TOP;
    if (abs(e.y - this->getBottom()) < _grabAreaDimensions.y && _enableY) dragAnchorVert = OF_ALIGN_VERT_BOTTOM;
  }
  
  glm::vec2 _grabAreaDimensions { 8, 8 };
  
  virtual void _mouseDragged(const DeltaMouseEvent & e) override
  {
    if (dragAnchorHorz == OF_ALIGN_HORZ_IGNORE && dragAnchorVert == OF_ALIGN_VERT_IGNORE) {
      this->translate(e.delta * glm::vec2(_enableX, _enableY));
      
      ResizeEvent resizeE;
      resizeE.bounds.x = this->left.value() + e.delta.x;
      resizeE.bounds.y = this->top.value() + e.delta.y;
      resizeE.bounds.width = this->width.value();
      resizeE.bounds.height = this->height.value();
      onResizedE.notify(resizeE);
    }
    
    if (dragAnchorHorz == OF_ALIGN_HORZ_LEFT) _translateLeft(e.delta.x);
    else if (dragAnchorHorz == OF_ALIGN_HORZ_RIGHT) _translateRight(e.delta.x);
    
    if (dragAnchorVert == OF_ALIGN_VERT_TOP) _translateTop(e.delta.y);
    else if (dragAnchorVert == OF_ALIGN_VERT_BOTTOM) _translateBottom(e.delta.y);
  }
  
  virtual void _mouseReleased(const ofMouseEventArgs & e) override
  {
    dragAnchorHorz = OF_ALIGN_HORZ_IGNORE;
    dragAnchorVert = OF_ALIGN_VERT_IGNORE;
  }
};

}}
