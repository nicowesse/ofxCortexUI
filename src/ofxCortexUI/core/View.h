#pragma once

#include "ofxCortexUI/core/DisplayObject.h"
#include "ofxCortexUI/core/Style.h"

namespace ofxCortex { namespace ui {

class View : public ofxCortex::ui::DisplayObject {
  
public:
  
  View(string name = "");
  static shared_ptr<View> create(string name = "") { return make_shared<View>(name); }
  
  virtual void setStyle(shared_ptr<Style> style, bool affectChildren = true);
  shared_ptr<Style> getStyle() { return style; }
  
  ofEvent<ofRectangle> onResizedE;
  void onResized(function<void(ofRectangle)> callback);
  
#pragma mark - Positioning and Sizing
  
  virtual View* setWidthWithEvent(float width, bool adjustLayout = true);
  virtual View* setHeightWithEvent(float height, bool adjustLayout = true);
  virtual View* setSizeWithEvent(glm::vec2 size, bool adjustLayout = true);
  virtual View* setSizeWithEvent(float width, float height, bool adjustLayout = true);
  virtual View* setRectWithEvent(const ofRectangle & rect, bool adjustLayout = true);
  
  const ofRectangle & getRenderRect() const { return _renderRect; }
  
#pragma mark - Focus
  void enableFocus() { _isFocusEnabled = true; }
  void disableFocus() { _isFocusEnabled = false; }
  void setFocused();
  void removeFocus();
  bool hasFocus() { return (this == _focused.get()); }
  static shared_ptr<View> getFocused() { return _focused; }
  
#pragma mark - Parameter
  virtual bool hasParameter() const { return false; }
  
protected:
  static shared_ptr<View> _focused;
  bool _hasFocus { false };
  bool _isFocusEnabled { true };
  
#pragma mark - Helpers and Utils
  
  virtual string _getModule() const override { return "View"; };
  
#pragma mark - Rendering and Loops
  
  virtual void _draw() override;
  virtual void _drawFocusOutline();
  virtual void _update(float time, float delta) override;
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
