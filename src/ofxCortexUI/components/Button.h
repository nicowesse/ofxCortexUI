#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Label.h"

namespace ofxCortex { namespace ui {

class Button : public ofxCortex::ui::View {
public:
  
  Button(ofParameter<void> param)
  {
    setName(param.getName());
    parameter.makeReferenceTo(param);
    
    _init();
    _adjustLayout();
  };
  
  Button(string name, function<void()> func)
  {
    setName(name);
    parameter.setName(name);
    
    onParameterTrigger = parameter.newListener(func);
    
    _init();
    _adjustLayout();
  };
  
  static shared_ptr<Button> create(ofParameter<void> param) { return make_shared<Button>(param); }
  static shared_ptr<Button> create(string name, function<void()> func) { return make_shared<Button>(name, func); }
  
  virtual bool hasParameter() const override { return true; }
  virtual ofParameter<void> & getParameter() { return parameter; }
  
protected:
  virtual string _getModule() const override { return "Button"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("Button::Background");
    background->disableEvents();
    
    backgroundColor = style->backgroundColor;
    
    label = ui::Label::create(parameter);
    label->setName("Button::Label");
    label->disableEvents();
  };
  
  virtual void _draw() override
  {
    background->drawBackground(backgroundColor, style->borderColor);
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
    
    backgroundColor = style->backgroundColor * 1.2;
  }
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
    backgroundColor = style->backgroundColor * 1.4;
    
    parameter.trigger();
  }
  
  virtual void _mouseReleased(const ofMouseEventArgs & e) override
  {
    View::_mouseReleased(e);
    
    backgroundColor = style->backgroundColor;
  }
  
  virtual void _mouseReleasedOutside(const ofMouseEventArgs & e) override
  {
    View::_mouseReleasedOutside(e);
    
    backgroundColor = style->backgroundColor;
  }
  
  virtual void _mouseExit(const ofMouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
    backgroundColor = style->backgroundColor;
  }
  
  // Members
  ofParameter<void> parameter;
  ofEventListener onParameterTrigger;
  
  shared_ptr<ui::Background> background;
  ofFloatColor backgroundColor;
  shared_ptr<ui::Label> label;
};

}}
