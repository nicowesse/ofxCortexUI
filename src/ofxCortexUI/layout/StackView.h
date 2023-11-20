#pragma once

#include "ofxCortexUI/layout/LayoutHelpers.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Background.h"

namespace ofxCortex { namespace ui {

class StackView : public ofxCortex::ui::View {
  class Wrapper : public DisplayObject {
  protected:
    virtual string _getModule() const override { return "Wrapper"; };
    virtual void _drawChildren() override;
  };
  
public:
  enum Distribution {
    STACK,
    STACK_CENTER,
    STACK_END,
    FILL_EQUALLY,
    SPACE_EQUALLY
  };
  
  
  StackView(Layout::Axis axis = Layout::VERTICAL, Distribution distribute = STACK, Layout::Alignment align = Layout::FILL, ofRectangle rect = ofRectangle(0, 0, 200, 400), string name = "StackView") : View(name), _axis(axis), _distribution(distribute), _alignment(align) {
    this->setRect(rect, false);
    this->disableChildRendering();
  }
  
  /**
   *  Helper for creating a StackView
   *
   *  @param  axis What axis you want the stack to distribute along. Takes VERTICAL or HORIZONTAL. Defaults to VERTICAL.
   *  @param  distribute How to distribute the views along the axis. Takes STACK, STACK_CENTER, STACK_END, FILL_EQUALLY, SPACE_EQUALLY. Defaults to STACK
   *  @param  align How to align the views on the opposite of the axis. Takes FILL, ALIGN_START, ALIGN_CENTER, ALIGN_END. Defaults to FILL.
   */
  static shared_ptr<StackView> create(Layout::Axis axis = Layout::VERTICAL, Distribution distribute = STACK, Layout::Alignment align = Layout::FILL, ofRectangle rect = ofRectangle(0, 0, 200, 400), string name = "StackView") {
    auto ptr = make_shared<StackView>(axis, distribute, align, rect, name);
    ptr->_init();
    return ptr;
  }
  
#pragma mark - Family Releations
  virtual void addView(shared_ptr<View> view, bool adjustLayout = true);
  virtual void addViewAt(shared_ptr<View> view, unsigned int index, bool adjustLayout = true);
  virtual void addViews(vector<shared_ptr<View>> views, bool adjustLayout = true);
  
  virtual void removeView(shared_ptr<View> view);
  virtual void removeAllViews();
    
  virtual void enableBackground() { _drawBackground = true; }
  virtual void disableBackground() { _drawBackground = false; }
  
  
protected:
  virtual string _getModule() const override { return "StackView"; };
  
  void _init();
  virtual void _update(float time, float delta) override;
  virtual void _draw() override;
  virtual void _adjustLayout() override;
  virtual void _debug() override;
  
  virtual void _drawMask() override;
  
  void _layoutVerticalStack();
  void _layoutHorizontalStack();
  void _layoutVerticalFillEqually();
  void _layoutHorizontalFillEqually();
  void _layoutVerticalEqualSpacing();
  void _layoutHorizontalEqualSpacing();
  
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  
//  float _getViewsHeight(bool includeSpacing = true);
//  float _getViewsWidth(bool includeSpacing = true);
  void _alignVertical(shared_ptr<View> view, const glm::vec2 & pos);
  void _alignHorizontal(shared_ptr<View> view, const glm::vec2 & pos);
  
  vector<shared_ptr<View>> _subviews;
  
  Layout::Axis _axis;
  Distribution _distribution;
  Layout::Alignment _alignment;
  
  shared_ptr<Wrapper> wrapper;
  shared_ptr<Background> background;
  bool _drawBackground { true };
  
  float scrollOpacity { 0.0f };
  
};

}}
