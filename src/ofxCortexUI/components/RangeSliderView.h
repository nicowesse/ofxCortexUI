#pragma once
#include "View.h"
#include "ResizeableView.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "ofParameter.h"
#include "ofxCortex/types/Range.h"

namespace ofxCortex { namespace ui {

class RangeSliderView : public View {
public:
  class Bar : public ResizeableView {
  protected:
    Bar() : ResizeableView("Bar") {
      this->setIntrinsicSize(0, 0);
    };
    Bar(std::string name) : ResizeableView(name) {};
    
  public:
    template<typename ... T>
    static std::shared_ptr<Bar> create(T&& ... t) {
      struct EnableMakeShared : public Bar { EnableMakeShared(T&&... arg) : Bar(std::forward<T>(arg)...) {} };
      
      auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
      p->viewDidLoad();
      
      View::everyView.insert(p);
      return p;
    }
    
  protected:
    
    virtual void onUpdate(float time, float delta) override;
    virtual void onDraw() override;
    
    float _intensity { 1.0f };
  };
  
protected:
  RangeSliderView(ofParameter<ofxCortex::core::types::Range> & param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
//    parameter.addListener(this, &RangeSliderView::onParameterChange);
  };
  
public:
  template<typename ... T>
  static std::shared_ptr<RangeSliderView> create(T&& ... t) {
    struct EnableMakeShared : public RangeSliderView { EnableMakeShared(T&&... arg) : RangeSliderView(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "RangeSliderView"; };
  
  virtual void viewDidLoad() override;
  virtual void onDraw() override;
  virtual void updateConstraints() override;
  
  virtual void onMouseScrolled(const MouseEventArgs & e) override;
  virtual void onKeyPressed(const ofKeyEventArgs & e) override;
  virtual void onWindowResized(const ofResizeEventArgs & e) override;
  
  float _getNormalizedLeftValue() const { return ofMap(bar->getLeft(), this->content_left.value(), this->content_right.value() - 16, 0, 1, true); }
  void _setLeftFromNormalized(float value) { bar->setLeft(ofMap(value, 0, 1, this->content_left.value(), this->content_right.value() - bar->getMinSize().x)); };
  float _getNormalizedRightValue() const { return ofMap(bar->getRight(), this->content_left.value() + 16, this->content_right.value(), 0, 1, true); }
  void _setRightFromNormalized(float value) { bar->setRight(ofMap(value, 0, 1, this->content_left.value() + bar->getMinSize().x, this->content_right.value())); };
  
  std::shared_ptr<ResizeableView> bar;
  ofParameter<ofxCortex::core::types::Range> parameter;
  bool _parameterSetInternally { false };
  ofEventListener onParameterChanged;
};

}}
