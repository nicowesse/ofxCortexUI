#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortexUI/core/View.h"
#include "ofxTweenzor.h"

#define STRINGIFY(A) #A

namespace ofxCortex { namespace ui {

template<typename T>
class SliderView : public ofxCortex::ui::View {
  static_assert(std::is_arithmetic<T>::value, "SliderView: T must be numeric!");
  
public:
  SliderView(ofParameter<T> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  };
  
  static shared_ptr<SliderView<T>> create(ofParameter<T> param) {
    auto ptr = make_shared<SliderView<T>>(param);
    ptr->_setup();
    return ptr;
  }
  
  
  
protected:
  virtual string _getComponentName() const override { return "SliderView"; };
  
  
  virtual void _draw() override;
  void _drawSlider();
  void _drawZero();
  void _drawDot();
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override;
  virtual void _mouseDragged(const View::DeltaMouseEvent & e) override;
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  virtual void _keyPressed(const ofKeyEventArgs & e) override;
  
  ofParameter<T> parameter;
  T getNormalizedParameter() { return ofMap(parameter.get(), parameter.getMin(), parameter.getMax(), 0, 1, true); }
  
  static ofShader & _getShader();
  float _inset { 8 };
  float interpolation { 0.0f };
  ofFloatColor _sliderColor;
};

}}
