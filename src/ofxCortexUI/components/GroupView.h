#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortex/utils/ContainerUtils.h"
#include "ofxCortex/utils/VectorUtils.h"
#include "ofxCortex/utils/NumberUtils.h"

#include "ofxCortexUI/utils/ParameterUtils.h"

namespace ofxCortex { namespace ui {

class GroupHeadingView : public LabelView {
protected:
  GroupHeadingView(const std::string & name)
  : LabelView(name)
  {}
  
  GroupHeadingView(ofAbstractParameter & param)
  : LabelView(param) {}
  
public:
  template<typename ... T>
  static std::shared_ptr<GroupHeadingView> create(T&& ... t) {
    struct EnableMakeShared : public GroupHeadingView { EnableMakeShared(T&&... arg) : GroupHeadingView(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual void viewDidLoad() override {
    float w = 12;
    float h = w * sqrt(3) / 2.0;
    
    arrowRotation.setStiffness(1.0);
    arrowRotation.setDamping(0.4);
    
    triangleCoords = ofxCortex::core::utils::Array::constructVector<glm::vec2>(3, [](int i) { return ofxCortex::core::utils::Vector::fromAngle((i / 3.0) * 360 + 30 + 180); });
  }
  
  virtual void onDraw() override;
  virtual void onMousePressed(const View::MouseEventArgs & e) override;
  
  std::vector<glm::vec2> triangleCoords;
  bool isOpen { false };
  ofxCortex::core::utils::InertialLerp<float> arrowRotation { 0.0f };
};

class GroupView : public ofxCortex::ui::View {
protected:
  GroupView(const ofParameterGroup & param) : ofxCortex::ui::View(param.getName()) {
    groupRef = param.newReference();
  };
  
public:
  template<typename ... T>
  static std::shared_ptr<GroupView> create(T&& ... t) {
    struct EnableMakeShared : public GroupView { EnableMakeShared(T&&... arg) : GroupView(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual void viewDidLoad() override;
  virtual void updateConstraints() override;
  
  virtual void onPreDraw() override;
  virtual void onPostDraw() override;
  virtual void onWindowResized(const ofResizeEventArgs & e) override { this->setNeedsUpdateConstraints(); }
  
  void onToggle(View::MouseEventArgs & e);
  bool isOpen { false };
  
  std::shared_ptr<GroupHeadingView> heading;
  ofEventListener onHeadingPressed;
  
  std::vector<std::shared_ptr<View>> parameterViews;
  std::shared_ptr<ofAbstractParameter> groupRef;
  
};

}}
