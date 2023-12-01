#include "RangeSliderView.h"

namespace ofxCortex { namespace ui {

//std::shared_ptr<RangeSliderView> RangeSliderView::create(const std::string & name)
//{
//  auto instance = std::make_shared<RangeSliderView>();
//  ofParameter<ofxCortex::core::types::Range> param { name, ofxCortex::core::types::Range(0, 1, 0, 1) };
//  instance->_setup(param);
//  return instance;
//}

void RangeSliderView::Bar::_update(double time, double delta)
{
  ResizeableView::_update(time, delta);
  
  float targetIntensity = isMouseOver() ? (isMousePressed() ? 2.0f : 1.5f) : 1.0f;
  _intensity = ofLerp(_intensity, targetIntensity, 1.0 - pow(0.02, delta));
}

void RangeSliderView::Bar::_draw()
{
  auto BB = this->getBounds();
  BB.setFromCenter(BB.getCenter(), BB.width, MIN(Styling::getDotSize() * 2 * 4, BB.height));
  const float & scale = Styling::getScale();
  float borderRadius = BB.height * 0.5;
  ofPushStyle();
  
  ofFill();
  ofSetColor(ofColor::white, 20 + 32 * getActiveIntensity());
  ofDrawRectRounded(BB, borderRadius);
  
  float verticalHintHeight = MIN(BB.height - 12 * scale, 12 * scale);
  float horizontalHintWidth = MIN(BB.width - 12 * scale, 12 * scale);
  
  ofSetColor(ofxCortex::ui::Styling::getForegroundColor(), ofMap(getHoverIntensity() + getActiveIntensity(), 0, 2, 128, 255));
  if (_enableX)
  {
    float circleRadius = MIN(borderRadius * 0.5 * 0.75, Styling::getDotSize());
    ofDrawCircle(BB.getLeft() + borderRadius, BB.getCenter().y, circleRadius);
    ofDrawCircle(BB.getRight() - borderRadius, BB.getCenter().y, circleRadius);
  }
//  if (_enableX)
//  {
//    ofDrawRectRounded(BB.getLeft() + 4 * scale, BB.getCenter().y - (verticalHintHeight / 2.0), 2 * scale, verticalHintHeight, 2 * scale); // Left Hint
//    ofDrawRectRounded(BB.getRight() - 4 * scale, BB.getCenter().y - (verticalHintHeight / 2.0), -2 * scale, verticalHintHeight, 2 * scale); // Right Hint
//  }
//
//  if (_enableY)
//  {
//    ofDrawRectRounded(BB.getCenter().x - (horizontalHintWidth / 2.0f), BB.getTop() + 4 * scale, horizontalHintWidth, 2 * scale, 2 * scale); // Top Hint
//    ofDrawRectRounded(BB.getCenter().x - (horizontalHintWidth / 2.0f), BB.getBottom() - 4 * scale, horizontalHintWidth, -2 * scale, 2 * scale);
//  }
  
  ofPopStyle();
}

std::shared_ptr<RangeSliderView> RangeSliderView::create(ofParameter<ofxCortex::core::types::Range> & param)
{
  auto instance = std::make_shared<RangeSliderView>(param);
  instance->_setup();
  return instance;
}

void RangeSliderView::_setup()
{
  this->disableSubviewRendering();
  
  bar = Bar::create("RangeSliderView::Bar");
  bar->disableY();
  bar->disableOverlap();
  bar->setIntrinsicSize(1, 1);
  bar->setWidth(this->getWidth());
  bar->setGrabAreaDimensions(glm::vec2(16 * Styling::getScale(), 16 * Styling::getScale()));
  bar->onResized([this](const ResizeableView::ResizeEvent & e) {
    glm::vec2 minSize = bar->getMinSize();
    const auto & CBB = this->getContentBounds();
    float normalizedFrom = ofMap(e.bounds.getLeft(), CBB.getLeft(), CBB.getRight() - minSize.x, 0, 1, true);
    float normalizedTo = ofMap(e.bounds.getRight(), CBB.getLeft() + minSize.x, CBB.getRight(), 0, 1, true);
    
    bool hitLeft = ofIsFloatEqual(bar->getLeft(), (this->getContentLeft() - 0.0001f));
    bool hitRight = ofIsFloatEqual(bar->getRight(), (this->getContentRight() + 0.0001f));
    
    if ((hitLeft | hitRight) == 0)
    {
      ofxCortex::core::types::Range range = parameter.get();
      range.from = ofMap(normalizedFrom, 0, 1, parameter->min, parameter->max);
      range.to = ofMap(normalizedTo, 0, 1, parameter->min, parameter->max);
      
      parameter.set(range);
      _parameterSetInternally = true;
    }
  });
  this->addSubview(bar);
  
  onParameterChanged = parameter.newListener([this](const ofxCortex::core::types::Range & e) {
    if (!_parameterSetInternally)
    {
      float normalLeft = ofMap(parameter->from, parameter->min, parameter->max, 0, 1, true);
      float normalRight = ofMap(parameter->to, parameter->min, parameter->max, 0, 1, true);
      _setLeftFromNormalized(normalLeft);
      _setRightFromNormalized(normalRight);
    }
    _parameterSetInternally = false;
  });
  
  this->_updateConstraintsImmediately();
  LayoutEngine::forceSolve();
  
  float normalLeft = ofMap(parameter->from, parameter->min, parameter->max, 0, 1, true);
  float normalRight = ofMap(parameter->to, parameter->min, parameter->max, 0, 1, true);
  _setLeftFromNormalized(normalLeft);
  _setRightFromNormalized(normalRight);
}

void RangeSliderView::_draw()
{
  Styling::drawBackground(this->getBounds());
  
  bar->draw();
  
  ofPushStyle();
  ofSetColor(Styling::getForegroundColor(), 255 - 128 * getActiveIntensity());
  Styling::drawLabel(getName(), this->getContentBounds());
  
  ofSetColor(Styling::getAccentColor(), 255 - 128 * getActiveIntensity());
  Styling::drawValue(parameter->toString(), this->getContentBounds());
  ofPopStyle();
}

void RangeSliderView::_mouseScrolled(const MouseEventArgs & e)
{
  parameter += -e.scrollX;
}

void RangeSliderView::_keyPressed(const ofKeyEventArgs & e)
{
  View::_keyPressed(e);
  
  if (!_isFocused()) return;
  
  float delta = (e.key == OF_KEY_LEFT) ? -1.0f : (e.key == OF_KEY_RIGHT) ? 1.0f : 0.0f;
  
  if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
  else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
  else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
  
  parameter += delta;
}

void RangeSliderView::_windowResized(ofResizeEventArgs & e)
{
  View::_windowResized(e);
  
  bar->setGrabAreaDimensions(glm::vec2(16 * Styling::getScale(), 16 * Styling::getScale()));
}

void RangeSliderView::_updateConstraints()
{
  View::_updateConstraints();
  
  this->addConstraints({
    { bar->left >= this->content_left | kiwi::strength::strong },
    { bar->right <= this->content_right | kiwi::strength::strong },
    { bar->top == this->content_top | kiwi::strength::strong },
    { bar->bottom == this->content_bottom | kiwi::strength::strong }
  });
}

}}
