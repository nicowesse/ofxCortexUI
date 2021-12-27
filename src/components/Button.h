#pragma once

#include "core/Component.h"
#include "components/Label.h"

namespace ofxCortex { namespace ui {

class Button : public ofxCortex::ui::Component {
public:
  
  Button(ofParameter<void> param)
  {
    setName(param.getName());
    parameter.makeReferenceTo(param);
    
    _init();
    _adjustLayout();
  };
  
  static shared_ptr<Button> create(ofParameter<void> param) { return make_shared<Button>(param); }
  
protected:
  virtual string _getModule() const override { return "Button"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("Button::Background");
    background->disableEvents();
    
    backgroundColor = style->backgroundColor;
    
    label = ui::Label<void>::create(parameter);
    label->setName("Button::Label");
    label->disableEvents();
  };
  
  virtual void _draw() override
  {
    background->drawBackground(backgroundColor);
    label->drawLabel(style->labelFontColor, OF_ALIGN_HORZ_CENTER);
  }
  
  virtual void _adjustLayout() override
  {
    background->setRect(this->getRect());
    label->setRect(this->getRect());
    
    Component::_adjustLayout();
  }
  
  virtual void _mouseEnter(const ofMouseEventArgs & e) override
  {
    Component::_mouseEnter(e);
    
    backgroundColor = style->foregroundColor;
  }
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override
  {
    Component::_mousePressed(e);
    
    backgroundColor *= 1.4;
    parameter.trigger();
  }
  
  virtual void _mouseReleased(const ofMouseEventArgs & e) override
  {
    Component::_mouseReleased(e);
    
    backgroundColor = style->foregroundColor;
  }
  
  virtual void _mouseExit(const ofMouseEventArgs & e) override
  {
    Component::_mousePressed(e);
    
    backgroundColor = style->backgroundColor;
  }
  
  // Members
  ofParameter<void> parameter;
  
  shared_ptr<ui::Background> background;
  ofFloatColor backgroundColor;
  shared_ptr<ui::Label<void>> label;
};

}}
