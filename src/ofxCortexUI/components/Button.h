#pragma once

#include "ofxCortexUI/core/ParameterView.h"
#include "ofxCortexUI/components/Label.h"

namespace ofxCortex { namespace ui {

class Button : public ofxCortex::ui::ParameterView<void> {
public:
  Button(std::string name, std::function<void()> func)
  : ParameterView<void>()
  {
    setName(name);
    getParameter().setName(name);
    
    onParameterTrigger = getParameter().newListener(func);
  };
  static shared_ptr<Button> create(string name, function<void()> func) {
    auto btn = std::make_shared<Button>(name, func);
    btn->_init();
    return btn;
  }
  
  Button(ofAbstractParameter & param)
  : ParameterView<void>(param)
  {
  };
  static shared_ptr<Button> create(ofParameter<void> param) {
    auto btn = make_shared<Button>(param);
    btn->_init();
    return btn;
  }
  
protected:
  virtual string _getModule() const override { return "Button"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("Button::Background");
    background->disableEvents();
    
    backgroundColor = style->foregroundColor;
    
    label = ui::Label::create(getParameter().cast<string>());
    label->setName("Button::Label");
    label->disableEvents();
    
    _adjustLayout();
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
    
    View::_adjustLayout();
  }
  
  virtual void _mouseEnter(const ofMouseEventArgs & e) override
  {
    View::_mouseEnter(e);
    
    backgroundColor = style->foregroundColor * 1.2;
  }
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
    backgroundColor = style->foregroundColor * 1.4;
    
    getParameter().trigger();
  }
  
  virtual void _mouseReleased(const ofMouseEventArgs & e) override
  {
    View::_mouseReleased(e);
    
    backgroundColor = style->foregroundColor;
  }
  
  virtual void _mouseReleasedOutside(const ofMouseEventArgs & e) override
  {
    View::_mouseReleasedOutside(e);
    
    backgroundColor = style->foregroundColor;
  }
  
  virtual void _mouseExit(const ofMouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
    backgroundColor = style->foregroundColor;
  }
  
  // Members
  ofEventListener onParameterTrigger;
  
  shared_ptr<ui::Background> background;
  ofFloatColor backgroundColor;
  shared_ptr<ui::Label> label;
};

}}
