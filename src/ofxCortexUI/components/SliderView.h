#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortexUI/core/View.h"
#include "ofxTweenzor.h"

#define STRINGIFY(A) #A

namespace ofxCortex { namespace ui {

template<typename T>
class SliderView : public ofxCortex::ui::View {
  static_assert(std::is_arithmetic<T>::value, "SliderView: T must be numeric!");
  
protected:
  SliderView(ofParameter<T> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  };
  
public:
  
  
  template<typename ... F>
  static std::shared_ptr<SliderView<T>> create(F&& ... f) {
    struct EnableMakeShared : public SliderView<T> { EnableMakeShared(F&&... arg) : SliderView<T>(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
  
  
protected:
  virtual string _getComponentName() const override { return "SliderView"; };
  
  
  virtual void onDraw() override;
  void _drawSlider();
  void _drawZero();
  void _drawDot();
  
  virtual void onMousePressed(const MouseEventArgs & e) override;
  virtual void onMouseDragged(const MouseEventArgs & e) override;
  virtual void onMouseScrolled(const MouseEventArgs & e) override;
  virtual void onKeyPressed(const ofKeyEventArgs & e) override;
  
  ofParameter<T> parameter;
  T getNormalizedParameter() { return ofMap(parameter.get(), parameter.getMin(), parameter.getMax(), 0, 1, true); }
  
  static ofShader & _getShader();
  float _inset { 8 };
  float interpolation { 0.0f };
  ofFloatColor _sliderColor;
};

}}
