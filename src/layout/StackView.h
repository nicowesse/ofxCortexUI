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
  enum Axis {
    VERTICAL,
    HORIZONTAL
  };
  
  enum Distribution {
    STACK,
    STACK_CENTER,
    STACK_END,
    FILL_EQUALLY,
    SPACE_EQUALLY
  };
  
  enum Alignment {
    FILL,
    ALIGN_START,
    ALIGN_CENTER,
    ALIGN_END
  };
  
  StackView(ofRectangle rect = ofRectangle(0, 0, 200, 400), Axis axis = VERTICAL, Distribution distribute = STACK, Alignment align = FILL) : View("Layout"), _axis(axis), _distribution(distribute), _alignment(align) {
    this->setRect(rect, false);
    this->disableChildRendering();
  }
  
  static shared_ptr<StackView> create(ofRectangle rect = ofRectangle(0, 0, 200, 400), Axis axis = VERTICAL, Distribution distribute = STACK, Alignment align = FILL) {
    auto ptr = make_shared<StackView>(rect, axis, distribute, align);
    ptr->_init();
    return ptr;
  }
  
#pragma mark - Family Releations
  virtual void addView(shared_ptr<View> view, bool adjustLayout = true);
  virtual void addViewAt(shared_ptr<View> view, unsigned int index, bool adjustLayout = true);
  virtual void addViews(vector<shared_ptr<View>> views, bool adjustLayout = true);
  
  virtual void removeView(shared_ptr<View> view);
  virtual void removeAllViews();
  
  
protected:
  virtual string _getModule() const override { return "StackView"; };
  
  void _init();
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
  
  float _getViewsHeight(bool includeSpacing = true);
  float _getViewsWidth(bool includeSpacing = true);
  float _getInnerWidth();
  float _getInnerHeight();
  void _alignVertical(shared_ptr<View> view, const glm::vec2 & pos);
  void _alignHorizontal(shared_ptr<View> view, const glm::vec2 & pos);
  
  vector<shared_ptr<View>> _subviews;
  
  Axis _axis;
  Distribution _distribution;
  Alignment _alignment;
  
  shared_ptr<Wrapper> wrapper;
  shared_ptr<Background> background;
  
};

}}
