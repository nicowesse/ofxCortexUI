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
  public:
    Bar() : ResizeableView("Bar") {
      this->setIntrinsicSize(0, 0);
    };
    Bar(std::string name) : ResizeableView(name) {};
    
    static std::shared_ptr<Bar> create(const std::string & name)
    {
      auto instance = std::make_shared<Bar>(name);
      instance->_setup();
      return instance;
    }
    
    virtual void _update(double time, double delta) override;
    virtual void _draw() override;
    virtual void _postDraw() override {};
    
    float _intensity { 1.0f };
  };
  
public:
  RangeSliderView(ofParameter<ofxCortex::core::types::Range> & param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
//    parameter.addListener(this, &RangeSliderView::onParameterChange);
  };
  
  static std::shared_ptr<RangeSliderView> create(ofParameter<ofxCortex::core::types::Range> & param);
  
protected:
  virtual std::string _getComponentName() const override { return "RangeSliderView"; };
  
  void _setup();
  virtual void _draw() override;
  virtual void _updateConstraints() override;
  
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  virtual void _keyPressed(const ofKeyEventArgs & e) override;
  virtual void _windowResized(ofResizeEventArgs & e) override;
  
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
