#pragma once

#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortexUI/components/SliderView.h"

namespace ofxCortex { namespace ui {

template<typename T> struct is_vector : std::false_type {};
template<> struct is_vector<glm::vec2> : std::true_type {};
template<> struct is_vector<glm::vec3> : std::true_type {};

template<typename T>
class VectorView : public ofxCortex::ui::ParameterView<T> {
  static_assert(is_vector<T>::value, "MyClass only supports glm::vec2 and glm::vec3.");
  
protected:
  VectorView(const ofAbstractParameter & param) : ofxCortex::ui::ParameterView<T>(param) {};
  
public:
  template<typename ... F>
  static std::shared_ptr<VectorView<T>> create(F&& ... f) {
    struct EnableMakeShared : public VectorView<T> { EnableMakeShared(F&&... arg) : VectorView<T>(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    ofLogVerbose(p->toString(__FUNCTION__));
    
    p->viewDidLoad();
    
    ofxCortex::ui::View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string getComponentName() const override { return "VectorView<T>"; };
  
  virtual void viewDidLoad() override {
    using namespace ofxCortex::ui;
    
    label = LabelView::create(ParameterView<T>::getParameter());
    label->disableBackground();
    
    x.set("X", ParameterView<T>::getParameterValue().x, ParameterView<T>::getParameterMin().x, ParameterView<T>::getParameterMax().x);
    sliderListeners.push(x.newListener([this](float & value){
      T v = ParameterView<T>::getParameterValue();
      v.x = value;
      ParameterView<T>::setParameter(v);
    }));
    sliderX = SliderView<float>::create(x);
    
    y.set("Y", ParameterView<T>::getParameterValue().y, ParameterView<T>::getParameterMin().y, ParameterView<T>::getParameterMax().y);
    sliderListeners.push(y.newListener([this](float & value){
      T v = ParameterView<T>::getParameterValue();
      v.y = value;
      ParameterView<T>::setParameter(v);
    }));
    sliderY = SliderView<float>::create(y);
    
    z.set("Z", ParameterView<T>::getParameterValue().z, ParameterView<T>::getParameterMin().z, ParameterView<T>::getParameterMax().z);
    sliderListeners.push(z.newListener([this](float & value){
      T v = ParameterView<T>::getParameterValue();
      v.z = value;
      ParameterView<T>::setParameter(v);
    }));
    sliderZ = SliderView<float>::create(z);
    
    sliderListeners.push(ParameterView<T>::getParameter().newListener([this](T & value) {
      x.setWithoutEventNotifications(value.x);
      y.setWithoutEventNotifications(value.y);
      z.setWithoutEventNotifications(value.z);
    }));
    
    
    this->addSubviews({ label, sliderX, sliderY, sliderZ });
  }
  
  virtual void onPreDraw() override
  {
    using namespace ofxCortex::ui;
    
    ofPushStyle();
    {
      Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(Styling::State::IDLE), this->level);
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
  
  std::shared_ptr<ofxCortex::ui::LabelView> label;
  
  ofEventListeners sliderListeners;
  
  ofParameter<float> x;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderX;
  
  ofParameter<float> y;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderY;
  
  ofParameter<float> z;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderZ;
};

template<>
class VectorView<glm::vec2> : public ofxCortex::ui::ParameterView<glm::vec2> {
protected:
  VectorView(const ofAbstractParameter & param) : ofxCortex::ui::ParameterView<glm::vec2>(param) {};
  
public:
  template<typename ... F>
  static std::shared_ptr<VectorView<glm::vec2>> create(F&& ... f) {
    struct EnableMakeShared : public VectorView<glm::vec2> { EnableMakeShared(F&&... arg) : VectorView<glm::vec2>(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    ofLogVerbose(p->toString(__FUNCTION__));
    
    p->viewDidLoad();
    
    ofxCortex::ui::View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual void viewDidLoad() override {
    using namespace ofxCortex::ui;
    
    label = LabelView::create(ParameterView<glm::vec2>::getParameter());
    label->disableBackground();
    
    x.set("X", ParameterView<glm::vec2>::getParameterValue().x, ParameterView<glm::vec2>::getParameterMin().x, ParameterView<glm::vec2>::getParameterMax().x);
    sliderListeners.push(x.newListener([this](float & value){
      auto v = ParameterView<glm::vec2>::getParameterValue();
      v.x = value;
      ParameterView<glm::vec2>::setParameter(v);
    }));
    sliderX = SliderView<float>::create(x);
    
    y.set("Y", ParameterView<glm::vec2>::getParameterValue().y, ParameterView<glm::vec2>::getParameterMin().y, ParameterView<glm::vec2>::getParameterMax().y);
    sliderListeners.push(y.newListener([this](float & value){
      auto v = ParameterView<glm::vec2>::getParameterValue();
      v.y = value;
      ParameterView<glm::vec2>::setParameter(v);
    }));
    sliderY = SliderView<float>::create(y);
    
    sliderListeners.push(ParameterView<glm::vec2>::getParameter().newListener([this](glm::vec2 & value) {
      x.setWithoutEventNotifications(value.x);
      y.setWithoutEventNotifications(value.y);
    }));
    
    this->addSubviews({ label, sliderX, sliderY });
  }
  
  virtual void onPreDraw() override
  {
    using namespace ofxCortex::ui;
    
    ofPushStyle();
    {
      Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(Styling::State::IDLE), this->level);
    }
    ofPopStyle();
  }
  
  virtual void onDraw() override {};
  
  virtual void updateConstraints() override
  {
    using namespace ofxCortex::ui;
    
    if (View::subviews.size() == 0) return;
    
    View::clearConstraints();
    
    auto alignmentConstraints = LayoutHelpers::alignment(View::getSelf(), View::subviews, LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Alignment::FILL_SPACE);
    auto stackConstraints = LayoutHelpers::stack(View::subviews, LayoutHelpers::Axis::VERTICAL);
    auto attachConstraints = LayoutHelpers::attachEnds(View::getSelf(), View::subviews, LayoutHelpers::Axis::VERTICAL);
    
    this->addConstraints(alignmentConstraints);
    this->addConstraints(stackConstraints);
    this->addConstraints(attachConstraints);
  }
  
  std::shared_ptr<ofxCortex::ui::LabelView> label;
  
  ofEventListeners sliderListeners;
  
  ofParameter<float> x;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderX;
  
  ofParameter<float> y;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderY;
};

}}
