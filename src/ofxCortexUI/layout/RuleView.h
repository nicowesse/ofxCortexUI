#pragma once

#include "ofxCortexUI/layout/LayoutHelpers.h"
#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class RuleView : public ofxCortex::ui::View {
public:
  enum RuleType
  {
    AUTO = 0,
    STATIC,
    RATIO,
    FIT_CONTENT
  };
  
  struct Rule {
    Rule(RuleType type = RuleType::AUTO, float value = 0.0f) : type(type), value(value) {};
    
    RuleType type;
    float value;
    
    Layout::Alignment align;
  };
  
  static const Rule Auto;
  static const Rule FillRemaining;
  static const Rule FitContent;
  
  struct StaticRule : public Rule {
    StaticRule(float size) : Rule(RuleType::STATIC, size) {};
  };
  
  struct RatioRule : public Rule {
    RatioRule(float ratio) : Rule(RuleType::RATIO, CLAMP(ratio, 0, 1)) {};
  };
  
public:
  RuleView(Layout::Axis axis, ofRectangle rect = ofGetCurrentViewport(), string name = "RuleView")
  : View(name), _axis(axis)
  {
    setRect(rect, false);
    
    _adjustLayout();
  }
  
  static shared_ptr<RuleView> create(Layout::Axis axis, ofRectangle rect = ofGetCurrentViewport(), string name = "RuleView") {
    return make_shared<RuleView>(axis, rect, name);
  }
  
  void setAxis(Layout::Axis axis) { _axis = axis; }
  
  virtual void addView(shared_ptr<View> view, Rule rule, Layout::Alignment alignment = Layout::FILL);
  virtual void addViewAt(shared_ptr<View> view, unsigned int index, Rule rule, Layout::Alignment alignment = Layout::FILL);
  
  virtual void removeView(shared_ptr<View> view);
  virtual void removeViewAt(unsigned int index);
  virtual void removeAllViews();
  
protected:
  virtual void _adjustLayout() override;
  
  Layout::Axis _axis;
  
  vector<pair<shared_ptr<View>, Rule>> _rules;
  vector<pair<shared_ptr<View>, Rule>> _getItemsByRule(vector<RuleType> types = vector<RuleType>{});
  vector<shared_ptr<View>> _getViewsByRule(vector<RuleType> types = vector<RuleType>{});
};

}}
