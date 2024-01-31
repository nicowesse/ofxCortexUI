#pragma once

#include "ofxTweenzor.h"

namespace ofxCortex { namespace ui {

class CheckboxView : public ofxCortex::ui::View {
public:
  
  CheckboxView(ofParameter<bool> & param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  };
  
  static shared_ptr<CheckboxView> create(ofParameter<bool> & param) {
    auto ptr = make_shared<CheckboxView>(param);
    ptr->_setup();
    return ptr;
  }
  
protected:
  virtual string _getComponentName() const override { return "Checkbox"; };
  
  void _setup()
  {
    outerRing.circle(0, 0, 12 * 0.5);
    outerRing.circle(0, 0, 10 * 0.5);
    outerRing.setFillColor(Styling::getAccentColor());
    
    innerAnimation = parameter.get();
  };
  
  virtual void _draw() override
  {
    Styling::drawBackground(this->getBounds(), View::getMouseState());
    Styling::drawLabel(parameter.getName(), this->getBounds());
    
    _drawCheckbox();
  }
  
  virtual void _drawCheckbox()
  {
    const auto & rect = this->getContentBounds();
    
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
  
  virtual void _mousePressed(const MouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
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
