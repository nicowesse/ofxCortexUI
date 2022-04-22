#pragma once

#include "ofxTweenzor.h"

namespace ofxCortex { namespace ui {

class Checkbox : public ofxCortex::ui::View {
public:
  
  Checkbox(ofParameter<bool> & param)
  {
    setName(param.getName());
    parameter.makeReferenceTo(param);
    
    _init();
    _adjustLayout();
  };
  
//  ~Checkbox()
//  {
//    Tweenzor::removeTween(&innerAnimation);
//  }
  
  static shared_ptr<Checkbox> create(ofParameter<bool> & param) { return make_shared<Checkbox>(param); }
  
protected:
  virtual string _getModule() const override { return "Checkbox"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("Checkbox::Background");
    background->disableEvents();
    
    label = ui::Label::create(parameter);
    label->setName("Checkbox::Label");
    label->disableEvents();
    
    outerRing.circle(0, 0, 12 * 0.5);
    outerRing.circle(0, 0, 10 * 0.5);
    outerRing.setFillColor(style->accentColor);
    
    innerAnimation = parameter.get();
  };
  
  virtual void _draw() override
  {
    background->drawBackground(style->backgroundColor);
    label->drawLabel(style->labelFontColor);
    
    _drawCheckbox();
  }
  
  virtual void _drawCheckbox()
  {
    const auto & rect = getRenderRect();
    
    float diameter = 10.0f;
    float radius = diameter * 0.5;
    
    float innerRadius = radius * 0.5 * innerAnimation;
    
    float x = rect.getRight() - radius - 12.0;
    float y = rect.getCenter().y;
    
    ofPushStyle();
    {
      ofSetColor(style->accentColor);
      outerRing.draw(x, y);
      
      ofFill();
      ofDrawCircle(x, y, innerRadius);
    }
    ofPopStyle();
  }
  
  virtual void _adjustLayout() override
  {
    background->setRect(this->getRect());
    label->setRect(this->getRect());
    
    View::_adjustLayout();
  }
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override
  {
    View::_mousePressed(e);
    
    parameter.set(!parameter.get());
    
    Tweenzor::add(&innerAnimation, innerAnimation, parameter.get(), 0.0f, 200.0 / 1000.0, EASE_IN_OUT_QUINT);
  }
  
  // Members
  ofParameter<bool> parameter;
  ofEventListener onParameterTrigger;
  
  shared_ptr<ui::Background> background;
  shared_ptr<ui::Label> label;
  
  ofPath outerRing;
  float innerAnimation { 0.0f };
};

}}
