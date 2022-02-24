#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Background.h"
#include "ofxCortexUI/components/Label.h"
#include "ofxCortexUI/components/Value.h"
#include "ofxCortexUI/components/Slider.h"
#include "ofxCortexUI/core/Draggable.h"
#include "ofxCortex/utils/Helpers.h"
#include "ofxTweenzor.h"

#define STRINGIFY(A) #A

namespace ofxCortex { namespace ui {

class RangeSlider : public ofxCortex::ui::View {
public:
  class Bar : public Draggable {
  public:
    Bar(ofRectangle dragBounds) : Draggable(dragBounds) {
      setName("Bar");
      ignoreY();
    };
    
    void drawBar(ofFloatColor acc = ofFloatColor::white, float horizontalPadding = 0.0f);
    
    virtual DisplayObject* setLeft(float x, bool adjustLayout = false) override;
    void setLeftFromNormalized(float x) { this->setLeft(ofMap(x, 0, 1, _dragBounds.getLeft(), _dragBounds.getRight() - this->getWidth(), true)); }
    
    virtual DisplayObject* setRight(float x, bool adjustLayout = false) override;
    void setRightFromNormalized(float x) { this->setRight(ofMap(x, 0, 1, _dragBounds.getLeft(), _dragBounds.getRight() - this->getWidth(), true)); }
    
  protected:
    virtual void _draw() override { this->drawBar(); }
    
  };
public:
  
  RangeSlider(ofParameter<ofxCortex::types::Range> param)
  {
    setName(param.getName());
    
    parameter.makeReferenceTo(param);
    this->disableChildRendering();
  };
  
  static shared_ptr<RangeSlider> create(ofParameter<ofxCortex::types::Range> param) {
    auto ptr = make_shared<RangeSlider>(param);
    ptr->_init();
    return ptr;
  }
  
  void drawSlider();
  void drawZero();
  
protected:
  virtual string _getModule() const override { return "RangeSlider"; };
  
  virtual void _init();
  
  virtual void _update() override;
  virtual void _draw() override;
  virtual void _debug() override;
  virtual void _adjustLayout() override;
  
  virtual void _mousePressed(const ofMouseEventArgs & e) override;
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  virtual void _keyPressed(const ofKeyEventArgs & e) override;
  
  ofParameter<ofxCortex::types::Range> parameter;
  
  shared_ptr<ui::Background> background;
  shared_ptr<ui::Label> label;
  shared_ptr<ui::Value<ofxCortex::types::Range>> value;
  shared_ptr<Handle> fromHandle;
  shared_ptr<Handle> toHandle;
  shared_ptr<Bar> bar;
  
  shared_ptr<Handle> _getMinHandle() { return fromHandle; } //(fromHandle->getX() <= toHandle->getX()) ? fromHandle : toHandle; }
  shared_ptr<Handle> _getMaxHandle() { return toHandle; } //(fromHandle->getX() > toHandle->getX()) ? fromHandle : toHandle; }
  
//  static ofShader & _getShader();
  
};

}}
