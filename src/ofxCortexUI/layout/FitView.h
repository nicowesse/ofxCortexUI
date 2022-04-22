#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Background.h"

namespace ofxCortex { namespace ui {

class FitView : public ofxCortex::ui::View {
public:
  FitView(ofRectangle rect = ofRectangle(0, 0, 200, 400), string name = "FitView")
  : View(name)
  {
    setRect(rect, false);
  }
  
  static shared_ptr<FitView> create(ofRectangle rect = ofRectangle(0, 0, 200, 400), string name = "FitView") {
    auto ptr = make_shared<FitView>(rect, name);
    ptr->_init();
    return ptr;
  }
  
  virtual void addView(shared_ptr<View> view, bool adjustLayout = true);
  virtual void addViewAt(shared_ptr<View> view, unsigned int index, bool adjustLayout = true);
  virtual void addViews(vector<shared_ptr<View>> views, bool adjustLayout = true);
  
  virtual void removeView(shared_ptr<View> view);
  virtual void removeAllViews();
  
protected:
  virtual string _getModule() const override { return "FitView"; };
  
  void _init();
  virtual void _draw() override;
  virtual void _adjustLayout() override;
  
  float _getViewsHeight(bool includeSpacing = true);
  
  vector<shared_ptr<View>> _subviews;
  shared_ptr<Background> background;
};

}}
