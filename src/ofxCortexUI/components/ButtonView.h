#pragma once

#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class ButtonView : public ofxCortex::ui::View {
public:
  
  ButtonView(ofParameter<void> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  };
  
  static std::shared_ptr<ButtonView> create(ofParameter<void> param) {
    auto instance = std::make_shared<ButtonView>(param);
    instance->_setup();
    return instance;
  }
  
  
  ButtonView(std::string name, const std::function<void()> & func = {}) : View(name)
  {
    parameter.setName(name);
    
    if (func) onParameterTrigger = parameter.newListener(func);
  };
  
  static std::shared_ptr<ButtonView> create(std::string name, const std::function<void()> & func = {}) {
    auto instance = std::make_shared<ButtonView>(name, func);
    instance->_setup();
    return instance;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "ButtonView"; };
  
  void _setup()
  {
    
  };
  
  virtual void _draw() override
  {
    Styling::drawBackground(this->getBounds(), getMouseState());
    Styling::drawLabel(parameter.getName(), this->getContentBounds(), OF_ALIGN_HORZ_CENTER);
  }
  
  virtual void _postDraw() override
  {
    if (_isFocused()) Styling::drawFocusBorder(this->getBounds());
  }
  
  virtual void _mousePressed(const MouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
    parameter.trigger();
  }
  
  // Members
  ofParameter<void> parameter;
  ofEventListener onParameterTrigger;
  Styling::State currentState;
};

}}
