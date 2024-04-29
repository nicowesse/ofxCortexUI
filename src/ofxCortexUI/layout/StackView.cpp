#include "StackView.h"

namespace ofxCortex { namespace ui {

void StackView::addSubviewAt(const std::shared_ptr<View> & subview, size_t index)
{
  ofLogVerbose(toString(__FUNCTION__)) << "OVERRIDDEN: Add subview '" << subview->getName() << "'";
  View::addSubviewAt(subview, index);
  this->setNeedsUpdateConstraints();
  
//  this->setIntrinsicHeight(Styling::getRowHeight(subviews.size()) + Styling::getPaddingTop() + Styling::getPaddingBottom());
}

void StackView::removeSubview(const std::shared_ptr<View> & subview)
{
//    stackViews.erase(std::remove(std::begin(stackViews), std::end(stackViews), subview), std::end(stackViews));
  
  View::removeSubview(subview);
  this->setNeedsUpdateConstraints();
  
  this->setIntrinsicHeight(Styling::getRowHeight() * subviews.size());
}

void StackView::onUpdate(float time, float delta)
{
  View::onUpdate(time, delta);
  
  scrollIntensity = ofLerp(scrollIntensity, 0.0, 1.0 - pow(0.005, delta));
}

void StackView::updateConstraints()
{
  ofLogVerbose(getLogModule(__FUNCTION__)) << "START OF updateConstraints(): Subviews = " << ofToString(subviews) << " Size = " << subviews.size();
  ofLogVerbose(getLogModule(__FUNCTION__)) << "Axis = '" << LayoutHelpers::getAxisString(this->axis) << "' Alignment = '" << LayoutHelpers::getAlignmentString(this->alignment) << "'";
  
  if (subviews.size() == 0) return;
  
  View::clearConstraints();
  
  this->addConstraints(LayoutHelpers::alignment(getSelf(), subviews, this->axis, this->alignment));
  this->addConstraints(LayoutHelpers::distribute(getSelf(), subviews, this->axis, LayoutHelpers::Distribution::STACK));
  
  auto scrollConstraints = std::vector<kiwi::Constraint>{
    { subviews.front()->top == this->content_top + scroll_y | kiwi::strength::strong },
    
  };
  this->addConstraints(scrollConstraints);

  ofLogVerbose(toString(__FUNCTION__)) << "END OF updateConstraints(): Current Layout Constraints = " << this->layoutConstraints.size();
}

void StackView::onPreDraw()
{
  View::onPreDraw();
  if (this->shouldDrawBackground)
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
    Styling::drawBackground(this->getContentFrame(), ofColor(255), ofColor(0, 0));
  }
  ofPopStyle();
}

void StackView::onMouseScrolled(const MouseEventArgs & e)
{
  float subviewHeight = subviews.back()->getBottom() - subviews.front()->getTop();
  float heightDiff = this->getContentHeight() - subviewHeight;
  
  if (subviewHeight >= this->getContentHeight()) LayoutEngine::suggestValue(scroll_y, ofClamp(scroll_y.value() + e.scrollY, heightDiff, 0));
  
  for (auto & view : subviews)
  {
    for (const auto & child : View::flatten(view))
    {
      if (child->getTop() > this->getContentBottom() || child->getBottom() < this->getContentTop()) { child->disableInteraction(); }
      else { child->enableInteraction(); }
    }
      
  }
}

}}
