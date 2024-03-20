#pragma once

#include "ofColor.h"

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortexUI/components/SliderView.h"

namespace ofxCortex { namespace ui {

class ColorHeadingView : public ValueView<ofColor> {
protected:
  ColorHeadingView(ofAbstractParameter & param)
  : ValueView<ofColor>(param) {}
  
public:
  template<typename ... T>
  static std::shared_ptr<ColorHeadingView> create(T&& ... t) {
    struct EnableMakeShared : public ColorHeadingView { EnableMakeShared(T&&... arg) : ColorHeadingView(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
  void setLabelColor(const ofFloatColor & c) { labelColor = c; }
  void setLabelBrightness(float brightness) { labelColor.setBrightness(brightness); }
  
protected:
  virtual void onDraw() override
  {
    const auto & BB = this->getContentFrame();
    
    ofPushStyle();
    {
      if (shouldDrawBackground) Styling::drawBackground(this->getFrame());
      
      ofSetColor(labelColor);
      Styling::drawLabel(getParameterName(), this->getContentFrame());
      
      ofSetColor(Styling::getAccentColor());
      ofxCortex::ui::Styling::drawValue(getFormattedString(), BB);
    }
    ofPopStyle();
  }
  
  ofFloatColor labelColor;
};

class ColorParameterView : public ofxCortex::ui::ParameterView {
protected:
  ColorParameterView(const ofParameter<ofFloatColor> & param) : ofxCortex::ui::ParameterView(param) {};
  
public:
  template<typename ... F>
  static std::shared_ptr<ColorParameterView> create(F&& ... f) {
    struct EnableMakeShared : public ColorParameterView { EnableMakeShared(F&&... arg) : ColorParameterView(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    ofLogVerbose(p->toString(__FUNCTION__));
    
    p->viewDidLoad();
    
    ofxCortex::ui::View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string getComponentName() const override { return "ColorParameterView"; };
  
  virtual void viewDidLoad() override {
    using namespace ofxCortex::ui;
    
    label = ColorHeadingView::create(ParameterView::getParameter<ofFloatColor>());
    label->disableBackground();
    
    hue.set("Hue", ParameterView::getParameterValue<ofFloatColor>().getHue(), 0.0f, ofFloatColor::limit());
    sliderListeners.push(hue.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<ofFloatColor>();
      v.setHue(value);
      ParameterView::setParameter(v);
      
      label->setLabelBrightness(1.0 - round(v.getBrightness()));
    }));
    sliderHue = SliderView<float>::create(hue);
    
    saturation.set("Saturation", ParameterView::getParameterValue<ofFloatColor>().getSaturation(), 0.0f, ofFloatColor::limit());
    sliderListeners.push(saturation.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<ofFloatColor>();
      v.setSaturation(value);
      ParameterView::setParameter(v);
      
      label->setLabelBrightness(1.0 - round(v.getBrightness()));
    }));
    sliderSaturation = SliderView<float>::create(saturation);
    
    brightness.set("Brightness", ParameterView::getParameterValue<ofFloatColor>().getBrightness(), 0.0f, ofFloatColor::limit());
    sliderListeners.push(brightness.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<ofFloatColor>();
      v.setBrightness(value);
      ParameterView::setParameter(v);
      
      label->setLabelBrightness(1.0 - round(value));
    }));
    sliderBrightness = SliderView<float>::create(brightness);
    
    alpha.set("Alpha", ParameterView::getParameterValue<ofFloatColor>().a, 0.0f, ofFloatColor::limit());
    sliderListeners.push(alpha.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<ofFloatColor>();
      v.a = value;
      ParameterView::setParameter(v);
      
      label->setLabelBrightness(1.0 - round(value));
    }));
    sliderAlpha = SliderView<float>::create(alpha);
    
    sliderListeners.push(ParameterView::getParameter<ofFloatColor>().newListener([this](ofFloatColor & value) {
      hue.setWithoutEventNotifications(value.getHue());
      saturation.setWithoutEventNotifications(value.getSaturation());
      brightness.setWithoutEventNotifications(value.getBrightness());
      alpha.setWithoutEventNotifications(value.a);
      
      label->setLabelBrightness(1.0 - round(value.getBrightness()));
    }));
    
    
    this->addSubviews({ label, sliderHue, sliderSaturation, sliderBrightness, sliderAlpha });
  }
  
  virtual void onPreDraw() override
  {
    using namespace ofxCortex::ui;
    
    ofPushStyle();
    {
      Styling::drawContainerBackground(this->getFrame(), ofFloatColor(ParameterView::getParameterValue<ofFloatColor>()), Styling::getBorderColor(Styling::State::IDLE), this->level);
    }
    ofPopStyle();
  }
  
  virtual void onDraw() override {};
  
  virtual void updateConstraints() override
  {
    using namespace ofxCortex::ui;
    
    if (View::subviews.size() == 0) return;
    
    View::clearConstraints();
    
    this->addConstraints(LayoutHelpers::alignment(View::getSelf(), View::subviews, LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Alignment::FILL_SPACE));
    this->addConstraints(LayoutHelpers::stack(View::subviews, LayoutHelpers::Axis::VERTICAL));
    this->addConstraints(LayoutHelpers::attachEnds(View::getSelf(), View::subviews, LayoutHelpers::Axis::VERTICAL));
  }
  
  std::shared_ptr<ColorHeadingView> label;
  
  ofEventListeners sliderListeners;
  
  ofParameter<float> hue;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderHue;
  
  ofParameter<float> saturation;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderSaturation;
  
  ofParameter<float> brightness;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderBrightness;
  
  ofParameter<float> alpha;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderAlpha;
};

}}
