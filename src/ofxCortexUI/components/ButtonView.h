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
  template<typename ... F>
  static std::shared_ptr<ButtonView> create(F&& ... f) {
    struct EnableMakeShared : public ButtonView { EnableMakeShared(F&&... arg) : ButtonView(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    
    ofLogVerbose(p->toString(__FUNCTION__));
    
    p->viewDidLoad();
    
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "ButtonView"; };
  
  virtual void onDraw() override
  {
    Styling::drawBackground(this->getFrame(), getMouseState());
    Styling::drawLabel(parameter.getName(), this->getContentFrame(), OF_ALIGN_HORZ_CENTER);
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
