#pragma once

#include "core/View.h"
#include "components/Background.h"

namespace ofxCortex { namespace ui {

class StackView : public ofxCortex::ui::View {
  class Wrapper : public DisplayObject {
  protected:
    virtual void _drawChildren() override;
  };
  
public:
  StackView(ofRectangle rect = ofRectangle(0, 0, 200, 400)) : View("Layout") {
    this->setRect(rect, false);
    this->disableChildRendering();
  }
  
  static shared_ptr<StackView> create(ofRectangle rect = ofRectangle(0, 0, 200, 400)) {
    auto ptr = make_shared<StackView>(rect);
    ptr->_init();
    return ptr;
  }
  
#pragma mark - Family Releations
  virtual void addView(shared_ptr<View> view);
  virtual void addView(shared_ptr<View> view, unsigned int index);
  virtual void addViews(vector<shared_ptr<View>> views);
  
  virtual void removeView(shared_ptr<View> view);
  virtual void removeAllViews();
  
  
protected:
  void _init();
  virtual void _draw() override;
  virtual void _adjustLayout() override;
  
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  
  float _getComponentsHeight();
  
  vector<shared_ptr<View>> _subviews;
  
  shared_ptr<Wrapper> wrapper;
  shared_ptr<Background> background;
  
};

}}
