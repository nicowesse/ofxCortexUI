#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortexUI/core/LayoutHelpers.h"

namespace ofxCortex { namespace ui {

class StackView : public ofxCortex::ui::View {
public:
  
  StackView(LayoutHelpers::Axis axis = LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Distribution distribution = LayoutHelpers::Distribution::STACK, LayoutHelpers::Alignment align = LayoutHelpers::Alignment::FILL_SPACE)
  : View("StackView"), axis(axis), distribution(distribution), alignment(alignment)
  {
    LayoutEngine::addEditVariable(scroll_y, kiwi::strength::strong);
    LayoutEngine::suggestValue(scroll_y, 0.0);
  }
  
  static std::shared_ptr<StackView> create(LayoutHelpers::Axis axis = LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Distribution distribution = LayoutHelpers::Distribution::STACK, LayoutHelpers::Alignment align = LayoutHelpers::Alignment::FILL_SPACE) {
    auto instance = std::make_shared<StackView>(axis, distribution, align);
    instance->_setup();
    return instance;
  }
  
  static std::shared_ptr<StackView> create(std::vector<std::shared_ptr<View>> views, LayoutHelpers::Axis axis = LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Distribution distribution = LayoutHelpers::Distribution::STACK, LayoutHelpers::Alignment align = LayoutHelpers::Alignment::FILL_SPACE) {
    auto instance = std::make_shared<StackView>(axis, distribution, align);
    instance->_setup(views);
    return instance;
  }
  
  virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index) override
  {
//    ofLogNotice(_getLogModule(__FUNCTION__));
    
    wrapper->addSubviewAt(subview, index);
    stackViews.push_back(subview);
    
    this->_setNeedsUpdateConstraints();
  }
  
  virtual void addSubviews(std::vector<std::shared_ptr<View>> views) override
  {
    View::addSubviews(views);
    
    this->_updateConstraintsImmediately();
  }
  
  virtual void removeSubview(std::shared_ptr<View> subview) override
  {
    stackViews.erase(std::remove(std::begin(stackViews), std::end(stackViews), subview), std::end(stackViews));
    wrapper->removeSubview(subview);
    this->_setNeedsUpdateConstraints();
  }
  
  void enableBackground() { this->_enableBackground = true; }
  void disableBackground() { this->_enableBackground = false; }
  
protected:
  virtual std::string _getComponentName() const override { return "StackView"; };
  
  void _setup(const std::vector<std::shared_ptr<View>> & views = {})
  {
    this->enableSubviewRendering();
    this->enableMask();
    
    wrapper = Wrapper::create();
    wrapper->enableSubviewRendering();
    View::addSubviewAt(wrapper, subviews.size());
    
//    ofLogNotice(_getLogModule(__FUNCTION__)) << "ADD SUBVIEWS";
    this->addSubviews(views);
    

    this->addConstraints({
      { wrapper->left == this->left | kiwi::strength::strong },
      { wrapper->right == this->right | kiwi::strength::strong },
      { wrapper->top <= this->top | kiwi::strength::strong },
      { wrapper->top == this->scroll_y | kiwi::strength::strong },
    });
    
    this->setIntrinsicHeight(Styling::getRowHeight() * 5);
    
    ofLogNotice(_getLogModule(__FUNCTION__)) << "Axis = '" << LayoutHelpers::getAxisString(this->axis) << "' | Distribution = '" << LayoutHelpers::getDistributionString(this->distribution) << "' | Alignment = '" << LayoutHelpers::getAlignmentString(this->alignment) << "'";
  }
  
  virtual void _update(double time, double delta) override
  {
    View::_update(time, delta);
    
    _scrollIntensity = ofLerp(_scrollIntensity, 0.0, 1.0 - pow(0.005, delta));
  }
  
  virtual void _updateConstraints() override
  {
    ofLogNotice(_getLogModule(__FUNCTION__)) << "StackViews Size = " << stackViews.size();
    
    if (stackViews.size() == 0) return;
    
    // Reset Constraints
    this->removeConstraints(stackConstraints);
    stackConstraints.clear();
    
    auto alignmentConstraints = LayoutHelpers::alignment(wrapper, stackViews, this->axis, this->alignment);
    auto distributeConstraints = LayoutHelpers::distribute(wrapper, stackViews, this->axis, this->distribution);
    
    std::vector<kiwi::Constraint> constraints;
    ofxCortex::core::utils::Array::appendVector(constraints, alignmentConstraints);
    ofxCortex::core::utils::Array::appendVector(constraints, distributeConstraints);
    
    ofLogNotice(_getLogModule(__FUNCTION__)) << "Constraints Size = " << constraints.size();
    
    this->addConstraints(constraints);
    stackConstraints = constraints;
  }
  
  virtual void _preDraw() override
  {
    ofPushStyle();
    {
      if (this->_enableBackground) Styling::drawContainerBackground(this->getBounds(), Styling::getContainerColor(), Styling::getBorderColor(), this->level);
    }
    ofPopStyle();
  }
  
  virtual void _postDraw() override {

    ofPushStyle();
    
    if (_isFocused()) Styling::drawFocusBorder(this->getBounds());
    
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
  
  virtual void _drawMask() override
  {
    ofPushStyle();
    {
      Styling::drawBackground(this->getContentBounds(), ofColor::white, ofColor(0, 0));
    }
    ofPopStyle();
  }
  
  virtual void _mouseScrolled(const MouseEventArgs & e) override
  {
    float heightDiff = this->getHeight() - wrapper->getHeight();
    bool shouldScroll = heightDiff < 0.0f && scroll_y.value() + e.scrollY > heightDiff;
    LayoutEngine::suggestValue(scroll_y, scroll_y.value() + e.scrollY * shouldScroll);
    _scrollIntensity = CLAMP(_scrollIntensity + abs(e.scrollY), 0, 1.0f);
  }
  
  std::vector<std::shared_ptr<View>> stackViews;
  std::vector<kiwi::Constraint> stackConstraints;
  
  class Wrapper : public ofxCortex::ui::View {
  public:
    Wrapper() : View("Wrapper") {}
    
    static std::shared_ptr<Wrapper> create() {
      return std::make_shared<Wrapper>();
    }
    
    virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index) override
    {
//      ofLogNotice(_getLogModule(__FUNCTION__));
      
      subview->setParent(this->getParent());
      subview->setLevel(this->level + 1);
      subview->disableInteractionOutsideParent();
      
      this->subviews.push_back(subview);
      this->everyView.push_back(subview);
    }
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
