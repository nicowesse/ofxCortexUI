#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortexUI/core/View.h"
#include "ofxTweenzor.h"

#define STRINGIFY(A) #A

namespace ofxCortex { namespace ui {

template<typename T>
class SliderView : public ofxCortex::ui::ParameterView<T> {
  static_assert(std::is_arithmetic<T>::value, "SliderView: T must be numeric!");
  
protected:
  SliderView(const ofAbstractParameter & param) : ParameterView<T>(param)
  {
//    parameter.makeReferenceTo(param);
//    parameterRef = param.newReference();
  };
  
public:
  template<typename ... F>
  static std::shared_ptr<SliderView<T>> create(F&& ... f) {
    struct EnableMakeShared : public SliderView<T> { EnableMakeShared(F&&... arg) : SliderView<T>(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    ofLogVerbose(p->toString(__FUNCTION__));
    
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
  
  
protected:
  virtual std::string _getComponentName() const override { return "SliderView"; };
  
  
  virtual void onDraw() override;
  void drawSlider();
  void drawZero();
  void drawDot();
  
  virtual void onMousePressed(const View::MouseEventArgs & e) override;
  virtual void onMouseDragged(const View::MouseEventArgs & e) override;
  virtual void onMouseScrolled(const View::MouseEventArgs & e) override;
  virtual void onKeyPressed(const ofKeyEventArgs & e) override;
  
//  std::shared_ptr<ofAbstractParameter> parameterRef;
//  ofParameter<T> getParameter() { return parameterRef->cast<T>(); }
  T getNormalizedParameter() { return ofMap(ParameterView<T>::getParameterValue(), ParameterView<T>::getParameterMin(), ParameterView<T>::getParameterMax(), 0, 1, true); }
  
  static ofShader & getShader();
  float interpolation { 0.0f };
};

}}
