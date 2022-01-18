#pragma once

#include "core/View.h"
#include "components/Background.h"
#include "components/Label.h"
#include "components/Value.h"
#include "core/Draggable.h"
#include "components/RangeSlider.h"
#include "utils/Utils.h"
#include "ofxTweenzor.h"

#define STRINGIFY(A) #A

namespace ofxCortex { namespace ui {

class Handle : public Draggable {
public:
  Handle(ofRectangle dragBounds) : Draggable(dragBounds) {
    setName("Handle");
    ignoreY();
  };
  
  void drawHandle(ofFloatColor acc = ofFloatColor::white, ofFloatColor bg = ofFloatColor::black, float diameter = 6);
  
protected:
  virtual void _draw() override
  {
    this->drawHandle();
  }
};

template<typename T>
class Slider : public ofxCortex::ui::View {
  static_assert(std::is_arithmetic<T>::value, "Slider: T must be numeric!");
  
public:
  Slider(ofParameter<T> param)
  {
    setName(param.getName());
    
    parameter.makeReferenceTo(param);
    this->disableChildRendering();
  };
  
  static shared_ptr<Slider<T>> create(ofParameter<T> param) {
    auto ptr = make_shared<Slider<T>>(param);
    ptr->_init();
    return ptr;
  }
  
  void drawSlider();
  void drawZero();
  
protected:
  virtual string _getModule() const override { return "Slider"; };
  
  virtual void _init();
  
  virtual void _update() override;
  virtual void _draw() override;
  virtual void _debug() override;
  virtual void _adjustLayout() override;
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override;
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  virtual void _keyPressed(const ofKeyEventArgs & e) override;
  
  ofParameter<T> parameter;
  T getNormalizedParameter() { return ofMap(parameter.get(), parameter.getMin(), parameter.getMax(), 0, 1, true); }
  
  shared_ptr<ui::Background> background;
  shared_ptr<ui::Label<T>> label;
  shared_ptr<ui::Value<T>> value;
  shared_ptr<Handle> handle;
  
  float _textOpacity { 0.65f };
  float _handleOpacity{1.0f};
  
  static ofShader & _getShader();
  
  friend class RangeSlider;
  
};

}}
