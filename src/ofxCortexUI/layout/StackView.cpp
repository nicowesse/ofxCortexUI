#include "StackView.h"

namespace ofxCortex { namespace ui {

void StackView::addSubviewAt(const std::shared_ptr<View> & subview, size_t index)
{
  ofLogVerbose(toString(__FUNCTION__)) << "Add subview '" << subview->getName() << "'";
  View::addSubviewAt(subview, index);
  this->setNeedsUpdateConstraints();
  
  this->setIntrinsicHeight(Styling::getRowHeight(subviews.size()));
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
  ofLogNotice(_getLogModule(__FUNCTION__)) << "Subviews Size = " << subviews.size();
  
  if (subviews.size() == 0) return;
  
  this->clearConstraints();
  
  auto alignmentConstraints = LayoutHelpers::alignment(getSelf(), subviews, this->axis, this->alignment);
//    auto distributeConstraints = LayoutHelpers::distribute(getSelf(), subviews, this->axis, this->distribution);
  auto stackConstraints = LayoutHelpers::stack(subviews, this->axis);
  auto attachConstraints = LayoutHelpers::attachLeading(getSelf(), subviews, LayoutHelpers::Axis::VERTICAL);
  
  auto scrollConstraints = std::vector<kiwi::Constraint>{
    { subviews.front()->top <= this->content_top | kiwi::strength::strong }
  };
  
  ofLogVerbose(toString(__FUNCTION__)) << "Alignment Constraints = " << alignmentConstraints.size() << " Stack Constraints = " << stackConstraints.size() << " Attach Constraints = " << attachConstraints.size();
  
  std::vector<kiwi::Constraint> constraints;
  ofxCortex::core::utils::Array::appendVector(constraints, alignmentConstraints);
  ofxCortex::core::utils::Array::appendVector(constraints, stackConstraints);
  ofxCortex::core::utils::Array::appendVector(constraints, attachConstraints);
//    ofxCortex::core::utils::Array::appendVector(constraints, scrollConstraints);
  
  this->addConstraints(constraints);
  
  ofLogVerbose(toString(__FUNCTION__)) << "Current Constraints = " << constraints.size();
}

void StackView::onPreDraw()
{
  View::onPreDraw();
  
  ofPushStyle();
  {
      if (this->_enableBackground) Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(), this->level);
  }
  ofPopStyle();
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
    Styling::drawBackground(this->getContentFrame(), ofColor::white, ofColor(0, 0));
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
