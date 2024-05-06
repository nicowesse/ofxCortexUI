#include "GroupView.h"

namespace ofxCortex { namespace ui {

void GroupHeadingView::onDraw()
{
  LabelView::onDraw();
  
  const auto & frame = this->contentFrame;
  float x = frame.getRight() - Styling::getScaled(12);
  float y = frame.getCenter().y;
  
//  ofDrawCircle(x, y, 4);
  
  ofPushMatrix();
  ofTranslate(x, y);
  ofTranslate(0, Styling::getScaled(-1));
  ofScale(Styling::getScaled(4));
  ofRotateDeg(arrowRotation);
  
  ofPushStyle();
  ofSetColor(Styling::getAccentColor());
  ofDrawTriangle(triangleCoords[0], triangleCoords[1], triangleCoords[2]);
  ofPopStyle();
  ofPopMatrix();
}

void GroupHeadingView::onMousePressed(const View::MouseEventArgs & e)
{
  isOpen = !isOpen;
  arrowRotation = isOpen * 180.0f;
}


void GroupView::viewDidLoad()
{
  heading = GroupHeadingView::create(groupRef->getName());
  onHeadingPressed = heading->onMousePressedE.newListener(this, &GroupView::onToggle);
  this->addSubview(heading);
  
  parameterViews = ofxCortex::ui::ParameterUtils::createViewsForParameterGroup(groupRef->castGroup(), false);
  
  for (auto & view : parameterViews) {
    for (auto & node : View::flatten(view)) node->disableInteraction();
  }
}

void GroupView::updateConstraints()
{
  using namespace ofxCortex::ui;
  
  if (View::subviews.size() == 0) return;
  
  View::clearConstraints();
  
  auto alignmentConstraints = LayoutHelpers::alignment(View::getSelf(), { heading }, LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Alignment::FILL_SPACE);
  this->addConstraints(alignmentConstraints);
  
  auto stackConstraints = LayoutHelpers::stack(View::subviews, LayoutHelpers::Axis::VERTICAL);
  this->addConstraints(stackConstraints);
  
  auto attachConstraints = LayoutHelpers::attachEnds(View::getSelf(), View::subviews, LayoutHelpers::Axis::VERTICAL);
  this->addConstraints(attachConstraints);
  
  auto parameterAlignmentConstraints = LayoutHelpers::alignment(View::getSelf(), parameterViews, LayoutHelpers::Axis::VERTICAL, LayoutHelpers::Alignment::TRAILING);
  this->addConstraints(parameterAlignmentConstraints);
  
  auto insetConstraint = LayoutHelpers::insetFromEdge(View::getSelf(), parameterViews, LayoutHelpers::Edge::CONTENT_LEFT, Styling::getPaddingLeft() * 1.5);
  this->addConstraints(insetConstraint);
}

void GroupView::onPreDraw()
{
  using namespace ofxCortex::ui;
  
  ofPushStyle();
  {
    Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(Styling::State::IDLE), this->level);
  }
  ofPopStyle();
}

void GroupView::onPostDraw()
{
  using namespace ofxCortex::ui;
  
  if (!parameterViews.empty() && isOpen)
  {
    float x = this->getContentLeft();
    float y = parameterViews.front()->getTop();
    float h = parameterViews.back()->getBottom() - parameterViews.front()->getTop();
    float w = Styling::getPaddingLeft() * 0.5;
    
    ofPushStyle();
    ofSetColor(Styling::getAccentColor(), 192);
    ofDrawRectRounded(x + 1, y, w, h, Styling::getScaled(w * 0.5));
    ofPopStyle();
  }
  
  View::onPostDraw();
}


void GroupView::onToggle(View::MouseEventArgs & e)
{
  this->isOpen = !this->isOpen;
  
  if (isOpen)
  {
    for (auto & view : parameterViews) {
      for (auto & node : View::flatten(view)) { node->enableInteraction(); }
    }
    this->addSubviews(parameterViews);
  }
  else {
    for (auto & view : parameterViews) {
      for (auto & node : View::flatten(view)) { node->disableInteraction(); }
    }
    this->removeSubviews(parameterViews);
  }
}

}}
