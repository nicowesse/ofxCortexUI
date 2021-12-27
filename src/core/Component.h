#pragma once

#include "core/DisplayObject.h"
#include "core/Style.h"

namespace ofxCortex { namespace ui {

class Component : public ofxCortex::ui::DisplayObject {
  
public:
  
  Component(string name = "");
  static shared_ptr<Component> create(string name = "") { return make_shared<Component>(name); }
  
  virtual void setStyle(shared_ptr<Style> style, bool affectChildren = true);
  
  ofEvent<ofRectangle> onResizedE;
  void onResized(function<void(ofRectangle)> callback);
  
#pragma mark - Positioning and Sizing
  
  virtual Component* setWidthWithEvent(float width, bool adjustLayout = true);
  virtual Component* setHeightWithEvent(float height, bool adjustLayout = true);
  virtual Component* setSizeWithEvent(glm::vec2 size, bool adjustLayout = true);
  virtual Component* setSizeWithEvent(float width, float height, bool adjustLayout = true);
  virtual Component* setRectWithEvent(const ofRectangle & rect, bool adjustLayout = true);
  
  const ofRectangle & getRenderRect() const { return _renderRect; }
  
#pragma mark - Focus
  void enableFocus() { _isFocusEnabled = true; }
  void disableFocus() { _isFocusEnabled = false; }
  void setFocused();
  void removeFocus();
  bool hasFocus() { return (this == _focused.get()); }
  
protected:
  static shared_ptr<Component> _focused;
  bool _hasFocus { false };
  bool _isFocusEnabled { true };
  
#pragma mark - Helpers and Utils
  
  virtual string _getModule() const override { return "Component"; };
  
#pragma mark - Rendering and Loops
  
  virtual void _draw() override;
  virtual void _drawFocusOutline();
  virtual void _update() override;
  virtual void _adjustLayout() override;
  virtual void _debug() override;
  
  ofRectangle _renderRect;
  void _recalculateRenderRect();
  
  shared_ptr<ofxCortex::ui::Style> style;
  
  
  
#pragma mark - Interaction
  virtual void _mousePressed(const ofMouseEventArgs & e) override;
  virtual void _onResize(const ofRectangle & rect);
};

}}
