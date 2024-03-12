#pragma once

#include "ofxTweenzor.h"
#include "ofxCortex/utils/NumberUtils.h"

namespace ofxCortex { namespace ui {

class CheckboxView : public ofxCortex::ui::ParameterView<bool> {
protected:
  CheckboxView(ofAbstractParameter & param) : ParameterView(param)
  {
//    parameter.makeReferenceTo(param);
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
  virtual ~CheckboxView() 
  {
//    Tweenzor::removeCompleteListener(&innerAnimation);
  }
  
  virtual string getComponentName() const override { return "Checkbox"; };
  
  virtual void viewDidLoad() override
  {
    outerRing.circle(0, 0, 12 * 0.5);
    outerRing.circle(0, 0, 10 * 0.5);
    outerRing.setFillColor(Styling::getAccentColor());
    
    innerAnimation = getParameter();
    innerAnimation.setSmoothing(0.001);
  };
  
  virtual void onDraw() override
  {
    Styling::drawBackground(this->getFrame(), View::getMouseState());
    Styling::drawLabel(getParameterName(), this->getContentFrame());
    
    this->drawCheckbox();
  }
  
  virtual void drawCheckbox()
  {
    const auto & rect = this->getContentFrame();
    
    float diameter = 10.0f;
    float radius = diameter * 0.5;
    
    float innerRadius = radius * 0.4 * innerAnimation;
    
    float x = rect.getRight() - radius - 12.0;
    float y = rect.getCenter().y;
    
    ofPushStyle();
    {
      ofSetColor(Styling::getAccentColor());
      ofNoFill();
      ofDrawCircle(x, y, radius);
      
      ofFill();
      ofDrawCircle(x, y, innerRadius);
    }
    ofPopStyle();
  }
  
  virtual void onMousePressed(const MouseEventArgs & e) override
  {
    View::onMousePressed(e);
    
    setParameter(!getParameterValue());
    innerAnimation = (float) getParameterValue();
    
//    Tweenzor::add(&innerAnimation, innerAnimation, getParameterValue(), 0.0f, 200.0 / 1000.0, EASE_IN_OUT_QUINT);
  }
  
  // Members
//  ofParameter<bool> parameter;
//  ofEventListener onParameterTrigger;
  
  ofPath outerRing;
  ofxCortex::core::utils::Lerped<float> innerAnimation { 0.0f };
};

}}
