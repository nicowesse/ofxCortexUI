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
  }
  
public:
  static std::shared_ptr<StackView> create(std::vector<std::shared_ptr<View>> views, LayoutHelpers::Axis axis = LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Distribution distribution = LayoutHelpers::Distribution::STACK, LayoutHelpers::Alignment align = LayoutHelpers::Alignment::FILL_SPACE) 
  {
    struct EnableMakeShared : public StackView { EnableMakeShared(LayoutHelpers::Axis axis, LayoutHelpers::Distribution distribution, LayoutHelpers::Alignment align) : StackView(axis, distribution, align) {} };
    
    auto instance = std::make_shared<EnableMakeShared>(axis, distribution, align);
    instance->viewDidLoad();
    instance->setup(views);
    
    View::everyView.insert(instance);
    
    return instance;
  }
  
  // Bypassing the
  virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index) override
  {
    ofLogNotice(_getLogModule(__FUNCTION__));
    
//    wrapper->addSubviewAt(subview, index);
    stackViews.push_back(subview);
    
    View::addSubviewAt(subview, index);
    this->setNeedsUpdateConstraints();
  }
  
  virtual void removeSubview(std::shared_ptr<View> subview) override
  {
    stackViews.erase(std::remove(std::begin(stackViews), std::end(stackViews), subview), std::end(stackViews));
    this->removeSubview(subview);
    this->setNeedsUpdateConstraints();
  }
  
  void enableBackground() { this->_enableBackground = true; }
  void disableBackground() { this->_enableBackground = false; }
  
protected:
  virtual std::string _getComponentName() const override { return "StackView"; };
  
  void setup(const std::vector<std::shared_ptr<View>> & views = {})
  {
    ofLogNotice(_getLogModule(__FUNCTION__)) << "Axis = '" << LayoutHelpers::getAxisString(this->axis) << "' | Distribution = '" << LayoutHelpers::getDistributionString(this->distribution) << "' | Alignment = '" << LayoutHelpers::getAlignmentString(this->alignment) << "'";
    
    this->enableSubviewRendering();
    this->disableMask();
    
    wrapper = Wrapper::create();
//    wrapper->enableDebug();
    wrapper->enableSubviewRendering();
    View::addSubviewAt(wrapper, subviews.size()); // Cannot use the addSubview, since it uses the modified addSubviewAt
    
    this->addConstraints({
      { wrapper->left == this->left | kiwi::strength::strong },
      { wrapper->right == this->right | kiwi::strength::strong },
      { wrapper->top <= this->top | kiwi::strength::strong },
      { wrapper->top == this->scroll_y + Styling::marginTop() | kiwi::strength::strong },
    });
    
    this->addSubviews(views);
    
    this->setIntrinsicHeight(Styling::getRowHeight() * 5);
  }
  
  virtual void onUpdate(float time, float delta) override
  {
    View::onUpdate(time, delta);
    
    _scrollIntensity = ofLerp(_scrollIntensity, 0.0, 1.0 - pow(0.005, delta));
  }
  
  virtual void updateConstraints() override
  {
    if (subviews.size() == 0) return;
    
    ofLogNotice(_getLogModule(__FUNCTION__)) << "StackViews Size = " << stackViews.size();
    
    wrapper->removeAllConstraints();
    stackConstraints.clear();
    
//    auto alignmentConstraints = LayoutHelpers::alignment(wrapper, stackViews, this->axis, this->alignment);
//    auto distributeConstraints = LayoutHelpers::distribute(wrapper, stackViews, this->axis, this->distribution);
    
    std::vector<kiwi::Constraint> constraints;
//    ofxCortex::core::utils::Array::appendVector(constraints, alignmentConstraints);
//    ofxCortex::core::utils::Array::appendVector(constraints, distributeConstraints);
    
    this->addConstraints({
      { stackViews.front()->top == this->content_top | kiwi::strength::strong },
//      { stackViews.front()->left == wrapper->content_left | kiwi::strength::strong },
//      { stackViews.front()->right == wrapper->content_left | kiwi::strength::strong },
    });
    
//    this->addConstraints(constraints);
    stackConstraints = constraints;
  }
  
  virtual void onPreDraw() override
  {
    View::onPreDraw();
    
    ofPushStyle();
    {
      if (this->_enableBackground) Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(), this->level);
    }
    ofPopStyle();
  }
  
  virtual void onPostDraw() override {
    View::onPostDraw();

    ofPushStyle();
    
    if (wrapper->getHeight() > this->getHeight())
    {
      float scrollbarAvailableHeight = this->getContentHeight();
      float scrollbarHeight = scrollbarAvailableHeight / (wrapper->getHeight() / this->getHeight());
      float scrollbarDifference = scrollbarAvailableHeight - scrollbarHeight;
      float overflowDifference = wrapper->getHeight() - this->getHeight();
      float offsetY = scrollbarDifference * (abs(wrapper->getTop()) / overflowDifference);
      
      ofSetColor(ofColor(Styling::getAccentColor(), 255 * _scrollIntensity));
      ofDrawRectRounded(this->getContentRight() + Styling::getPaddingRight() * 0.5 - 0.55, this->getContentTop() + offsetY, 0.5, scrollbarHeight, 4);
    }
    ofPopStyle();
  }
  
  virtual void onDrawMask() override
  {
    ofPushStyle();
    {
      Styling::drawBackground(this->getContentFrame(), ofColor::white, ofColor(0, 0));
    }
    ofPopStyle();
  }
  
  virtual void onMouseScrolled(const MouseEventArgs & e) override
  {
    float heightDiff = this->getHeight() - wrapper->getHeight();
    bool shouldScroll = heightDiff < 0.0f && scroll_y.value() + e.scrollY > heightDiff;
    LayoutEngine::suggestValue(scroll_y, scroll_y.value() + e.scrollY * shouldScroll);
    _scrollIntensity = CLAMP(_scrollIntensity + abs(e.scrollY), 0, 1.0f);
  }
  
  std::vector<std::shared_ptr<View>> stackViews;
  std::vector<kiwi::Constraint> stackConstraints;
  
  class Wrapper : public ofxCortex::ui::View {
  protected:
    Wrapper() : View("Wrapper") {}
    
  public:
    
    
    template<typename ... F>
    static std::shared_ptr<Wrapper> create(F&& ... f) {
      struct EnableMakeShared : public Wrapper { EnableMakeShared(F&&... arg) : Wrapper(std::forward<F>(arg)...) {} };
      
      auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
      p->viewDidLoad();
      
      View::everyView.insert(p);
      return p;
    }
    
    virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index) override
    {
      ofLogNotice(_getLogModule(__FUNCTION__)) << "Index = " << index << " Name = '" << subview->getName() << "'";
      
      subview->setParent(this->getParent());
      subview->setLevel(this->level + 1);
      subview->disableInteractionOutsideParent();
      
      this->subviews.push_back(subview);
    }
    
  protected:
    virtual std::string _getComponentName() const override { return "StackView::Wrapper"; };
  };
  std::shared_ptr<Wrapper> wrapper;
  
  LayoutHelpers::Axis axis;
  LayoutHelpers::Distribution distribution;
  LayoutHelpers::Alignment alignment;
  kiwi::Variable scroll_y;
  
  float _scrollIntensity { 0.0f };
  bool _enableBackground { true };
};

}}
