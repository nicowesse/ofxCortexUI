#pragma once

#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/core/LayoutHelpers.h"
#include "ofxCortexUI/components/ValueView.h"
#include "ofxCortexUI/components/SliderView.h"

namespace ofxCortex { namespace ui {

template<typename T> struct is_vector : std::false_type {};
template<> struct is_vector<glm::vec2> : std::true_type {};
template<> struct is_vector<glm::vec3> : std::true_type {};

template<typename T>
class VectorView : public ofxCortex::ui::ParameterView {
  static_assert(is_vector<T>::value, "MyClass only supports glm::vec2 and glm::vec3.");
  
protected:
  VectorView(const ofAbstractParameter & param) : ofxCortex::ui::ParameterView(param) {};
  
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
    
    heading = ValueView<T>::create(ParameterView::getParameter<T>());
    heading->disableBackground();
    
    x.set("X", ParameterView::getParameterValue<T>().x, ParameterView::getParameterMin<T>().x, ParameterView::getParameterMax<T>().x);
    sliderListeners.push(x.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<T>();
      v.x = value;
      ParameterView::setParameter<T>(v);
    }));
    sliderX = SliderView<float>::create(x);
    
    y.set("Y", ParameterView::getParameterValue<T>().y, ParameterView::getParameterMin<T>().y, ParameterView::getParameterMax<T>().y);
    sliderListeners.push(y.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<T>();
      v.y = value;
      ParameterView::setParameter<T>(v);
    }));
    sliderY = SliderView<float>::create(y);
    
    z.set("Z", ParameterView::getParameterValue<T>().z, ParameterView::getParameterMin<T>().z, ParameterView::getParameterMax<T>().z);
    sliderListeners.push(z.newListener([this](float & value){
      T v = ParameterView::getParameterValue<T>();
      v.z = value;
      ParameterView::setParameter<T>(v);
    }));
    sliderZ = SliderView<float>::create(z);
    
    sliderListeners.push(ParameterView::getParameter<T>().newListener([this](T & value) {
      x.setWithoutEventNotifications(value.x);
      y.setWithoutEventNotifications(value.y);
      z.setWithoutEventNotifications(value.z);
    }));
    
    
    this->addSubviews({ heading, sliderX, sliderY, sliderZ });
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
  
  std::shared_ptr<ofxCortex::ui::ValueView<T>> heading;
  
  ofEventListeners sliderListeners;
  
  ofParameter<float> x;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderX;
  
  ofParameter<float> y;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderY;
  
  ofParameter<float> z;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderZ;
};

template<>
class VectorView<glm::vec2> : public ofxCortex::ui::ParameterView {
protected:
  VectorView(const ofAbstractParameter & param) : ofxCortex::ui::ParameterView(param) {};
  
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
    
    heading = ValueView<glm::vec2>::create(ParameterView::getParameter<glm::vec2>());
    heading->disableBackground();
    
    x.set("X", ParameterView::getParameterValue<glm::vec2>().x, ParameterView::getParameterMin<glm::vec2>().x, ParameterView::getParameterMax<glm::vec2>().x);
    sliderListeners.push(x.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<glm::vec2>();
      v.x = value;
      ParameterView::setParameter<glm::vec2>(v);
    }));
    sliderX = SliderView<float>::create(x);
    
    y.set("Y", ParameterView::getParameterValue<glm::vec2>().y, ParameterView::getParameterMin<glm::vec2>().y, ParameterView::getParameterMax<glm::vec2>().y);
    sliderListeners.push(y.newListener([this](float & value){
      auto v = ParameterView::getParameterValue<glm::vec2>();
      v.y = value;
      ParameterView::setParameter(v);
    }));
    sliderY = SliderView<float>::create(y);
    
    sliderListeners.push(ParameterView::getParameter<glm::vec2>().newListener([this](glm::vec2 & value) {
      x.setWithoutEventNotifications(value.x);
      y.setWithoutEventNotifications(value.y);
    }));
    
    this->addSubviews({ heading, sliderX, sliderY });
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
  
  std::shared_ptr<ofxCortex::ui::ValueView<glm::vec2>> heading;
  
  ofEventListeners sliderListeners;
  
  ofParameter<float> x;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderX;
  
  ofParameter<float> y;
  std::shared_ptr<ofxCortex::ui::SliderView<float>> sliderY;
};

}}
