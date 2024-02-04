#pragma once

#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class ButtonView : public ofxCortex::ui::View {
protected:
  ButtonView(ofParameter<void> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  };
  
  ButtonView(std::string name, const std::function<void()> & func = {}) : View(name)
  {
    parameter.setName(name);
    
    if (func) onParameterTrigger = parameter.newListener(func);
  };
  
public:
  template<typename ... T>
  static std::shared_ptr<ButtonView> create(T&& ... t) {
    struct EnableMakeShared : public ButtonView { EnableMakeShared(T&&... arg) : ButtonView(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "ButtonView"; };
  
  virtual void onDraw() override
  {
    Styling::drawBackground(this->getBounds(), getMouseState());
    Styling::drawLabel(parameter.getName(), this->getContentBounds(), OF_ALIGN_HORZ_CENTER);
  }
  
  virtual void onMousePressed(const MouseEventArgs & e) override
  {
    View::onMousePressed(e);
    
    parameter.trigger();
  }
  
  // Members
  ofParameter<void> parameter;
  ofEventListener onParameterTrigger;
  Styling::State currentState;
};

}}
