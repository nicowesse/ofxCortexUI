#include "StackView.h"

namespace ofxCortex { namespace ui {

void StackView::addSubviewAt(const std::shared_ptr<View> & subview, size_t index)
{
  ofLogVerbose(toString(__FUNCTION__)) << "OVERRIDDEN: Add subview '" << subview->getName() << "'";
  View::addSubviewAt(subview, index);
  this->setNeedsUpdateConstraints();
  
//  this->setIntrinsicHeight(Styling::getRowHeight(subviews.size()) + Styling::getPaddingTop() + Styling::getPaddingBottom());
}

void StackView::removeSubview(std::shared_ptr<View> subview)
{
//    stackViews.erase(std::remove(std::begin(stackViews), std::end(stackViews), subview), std::end(stackViews));
  
  View::removeSubview(subview);
  this->setNeedsUpdateConstraints();
  
  this->setIntrinsicHeight(Styling::getRowHeight() * subviews.size());
}

void StackView::onUpdate(float time, float delta)
{
  View::onUpdate(time, delta);
  
  _scrollIntensity = ofLerp(_scrollIntensity, 0.0, 1.0 - pow(0.005, delta));
}

void StackView::updateConstraints()
{
  ofLogVerbose(getLogModule(__FUNCTION__)) << "START OF updateConstraints(): Subviews = " << ofToString(subviews) << " Size = " << subviews.size();
  ofLogVerbose(getLogModule(__FUNCTION__)) << "Axis = '" << LayoutHelpers::getAxisString(this->axis) << "' Alignment = '" << LayoutHelpers::getAlignmentString(this->alignment) << "' Distribution = '" << LayoutHelpers::getDistributionString(this->distribution) << "'";
  
  if (subviews.size() == 0) return;
  
  View::clearConstraints();
  
  auto alignmentConstraints = LayoutHelpers::alignment(getSelf(), subviews, this->axis, this->alignment);
  auto distributeConstraints = LayoutHelpers::distribute(getSelf(), subviews, this->axis, this->distribution);
//  auto stackConstraints = LayoutHelpers::stack(subviews, this->axis);
//  auto attachConstraints = LayoutHelpers::attachLeading(getSelf(), subviews, LayoutHelpers::Axis::VERTICAL);
  auto trailingEdgeConstraints = std::vector<kiwi::Constraint>{
    { this->content_bottom >= subviews.back()->bottom | kiwi::strength::strong }
  };
  
  auto scrollConstraints = std::vector<kiwi::Constraint>{
    { subviews.front()->top <= this->content_top | kiwi::strength::strong }
  };
  
  ofLogVerbose(toString(__FUNCTION__)) 
  << "\n— " << alignmentConstraints.size() << " x Alignment Constraints"
  << "\n— " << distributeConstraints.size() << " x Distribute Constraints";
//  << "\n— " << stackConstraints.size() << " x Stack Constraints"
//  << "\n— " << attachConstraints.size() << " x Attach Constraints";
  
  std::vector<kiwi::Constraint> constraints;
  ofxCortex::core::utils::Array::appendVector(constraints, alignmentConstraints);
  ofxCortex::core::utils::Array::appendVector(constraints, distributeConstraints);
//  ofxCortex::core::utils::Array::appendVector(constraints, stackConstraints);
//  ofxCortex::core::utils::Array::appendVector(constraints, attachConstraints);
  ofxCortex::core::utils::Array::appendVector(constraints, trailingEdgeConstraints);
//    ofxCortex::core::utils::Array::appendVector(constraints, scrollConstraints);
  
  ofLogVerbose(toString(__FUNCTION__)) << "Constraints = " << constraints.size();
  
  this->addConstraints(constraints);
//  for (auto & subview : subviews) subview->setNeedsLayout();
  
  ofLogVerbose(toString(__FUNCTION__)) << "END OF updateConstraints(): Current Layout Constraints = " << this->layoutConstraints.size();
}

void StackView::onPreDraw()
{
  View::onPreDraw();
  if (this->_enableBackground)
  {
    ofPushStyle();
    {
      Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(), this->level);
    }
    ofPopStyle();
  }
}

void StackView::onPostDraw()
{
  View::onPostDraw();

  ofPushStyle();
  
//    if (wrapper->getHeight() > this->getHeight())
//    {
//      float scrollbarAvailableHeight = this->getContentHeight();
//      float scrollbarHeight = scrollbarAvailableHeight / (wrapper->getHeight() / this->getHeight());
//      float scrollbarDifference = scrollbarAvailableHeight - scrollbarHeight;
//      float overflowDifference = wrapper->getHeight() - this->getHeight();
//      float offsetY = scrollbarDifference * (abs(wrapper->getTop()) / overflowDifference);
//
//      ofSetColor(ofColor(Styling::getAccentColor(), 255 * _scrollIntensity));
//      ofDrawRectRounded(this->getContentRight() + Styling::getPaddingRight() * 0.5 - 0.55, this->getContentTop() + offsetY, 0.5, scrollbarHeight, 4);
//    }
  ofPopStyle();
}

void StackView::onDrawMask()
{
  ofPushStyle();
  {
//    Styling::drawBackground(this->getContentFrame(), ofColor::white, ofColor(0, 0));
  }
  ofPopStyle();
}

void StackView::onMouseScrolled(const MouseEventArgs & e)
{
//  float heightDiff = this->getHeight() - wrapper->getHeight();
//  bool shouldScroll = heightDiff < 0.0f && scroll_y.value() + e.scrollY > heightDiff;
//    LayoutEngine::suggestValue(scroll_y, scroll_y.value() + e.scrollY * shouldScroll);
  _scrollIntensity = CLAMP(_scrollIntensity + abs(e.scrollY), 0, 1.0f);
}

}}
