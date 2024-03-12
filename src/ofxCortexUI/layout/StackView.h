#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortexUI/core/LayoutHelpers.h"

namespace ofxCortex { namespace ui {

class StackView : public ofxCortex::ui::View {
protected:
  StackView(LayoutHelpers::Axis axis = LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Distribution distribution = LayoutHelpers::Distribution::STACK, LayoutHelpers::Alignment align = LayoutHelpers::Alignment::FILL_SPACE)
  : View("StackView"), axis(axis), distribution(distribution), alignment(alignment)
  {
    LayoutEngine::addEditVariable(scroll_y, kiwi::strength::strong);
    LayoutEngine::suggestValue(scroll_y, 0.0);
    
    this->enableSubviewRendering();
    this->enableMask();
    this->disableIntrinsicSize();
  }
  
public:
  template<typename ... F>
  static std::shared_ptr<StackView> create(F&& ... f) {
    struct EnableMakeShared : public StackView { EnableMakeShared(F&&... arg) : StackView(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    ofLogVerbose(p->toString(__FUNCTION__));
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
  // Bypassing the
  virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index) override;
  virtual void removeSubview(std::shared_ptr<View> subview) override;
  
  void enableBackground() { this->_enableBackground = true; }
  void disableBackground() { this->_enableBackground = false; }
  
protected:
  virtual std::string getComponentName() const override { return "StackView"; };
  
  virtual void onUpdate(float time, float delta) override;
  virtual void updateConstraints() override;
  
  virtual void onPreDraw() override;
  virtual void onPostDraw() override;
  virtual void onDrawMask() override;
  
  virtual void onMouseScrolled(const MouseEventArgs & e) override;
  
//  class Wrapper : public ofxCortex::ui::View {
//  protected:
//    Wrapper() : View("Wrapper") {}
//    
//  public:
//    
//    
//    template<typename ... F>
//    static std::shared_ptr<Wrapper> create(F&& ... f) {
//      struct EnableMakeShared : public Wrapper { EnableMakeShared(F&&... arg) : Wrapper(std::forward<F>(arg)...) {} };
//      
//      auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
//      p->viewDidLoad();
//      
//      View::everyView.insert(p);
//      return p;
//    }
//    
//    virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index) override
//    {
//      ofLogNotice(_getLogModule(__FUNCTION__)) << "Index = " << index << " Name = '" << subview->getName() << "'";
//      
//      subview->setParent(this->getParent());
//      subview->setLevel(this->level + 1);
//      subview->disableInteractionOutsideParent();
//      
//      this->subviews.push_back(subview);
//    }
//    
//  protected:
//    virtual std::string _getComponentName() const override { return "StackView::Wrapper"; };
//  };
//  std::shared_ptr<Wrapper> wrapper;
  
  LayoutHelpers::Axis axis;
  LayoutHelpers::Distribution distribution;
  LayoutHelpers::Alignment alignment;
  kiwi::Variable scroll_y;
  
  float _scrollIntensity { 0.0f };
  bool _enableBackground { true };
};

}}
