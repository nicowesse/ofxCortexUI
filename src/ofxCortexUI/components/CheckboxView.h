#pragma once

#include "ofxTweenzor.h"

namespace ofxCortex { namespace ui {

class CheckboxView : public ofxCortex::ui::View {
protected:
  CheckboxView(ofParameter<bool> & param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  };
  
public:
  template<typename ... F>
  static std::shared_ptr<CheckboxView> create(F&& ... f) {
    struct EnableMakeShared : public CheckboxView { EnableMakeShared(F&&... arg) : CheckboxView(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual string _getComponentName() const override { return "Checkbox"; };
  
  virtual void viewDidLoad() override
  {
    outerRing.circle(0, 0, 12 * 0.5);
    outerRing.circle(0, 0, 10 * 0.5);
    outerRing.setFillColor(Styling::getAccentColor());
    
    innerAnimation = parameter.get();
  };
  
  virtual void onDraw() override
  {
    Styling::drawBackground(this->getFrame(), View::getMouseState());
    Styling::drawLabel(parameter.getName(), this->getFrame());
    
    _drawCheckbox();
  }
  
  virtual void _drawCheckbox()
  {
    const auto & rect = this->getContentFrame();
    
    float diameter = 10.0f;
    float radius = diameter * 0.5;
    
    float innerRadius = radius * 0.5 * innerAnimation;
    
    float x = rect.getRight() - radius - 12.0;
    float y = rect.getCenter().y;
    
    ofPushStyle();
    {
      ofSetColor(Styling::getAccentColor());
      outerRing.draw(x, y);
      
      ofFill();
      ofDrawCircle(x, y, innerRadius);
    }
    ofPopStyle();
  }
  
  virtual void onMousePressed(const MouseEventArgs & e) override
  {
    View::onMousePressed(e);
    
    parameter.set(!parameter.get());
    
    Tweenzor::add(&innerAnimation, innerAnimation, parameter.get(), 0.0f, 200.0 / 1000.0, EASE_IN_OUT_QUINT);
  }
  
  // Members
  ofParameter<bool> parameter;
  ofEventListener onParameterTrigger;
  
  ofPath outerRing;
  float innerAnimation { 0.0f };
};

}}
