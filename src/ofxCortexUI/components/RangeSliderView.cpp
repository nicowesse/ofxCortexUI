#include "RangeSliderView.h"

namespace ofxCortex { namespace ui {

//std::shared_ptr<RangeSliderView> RangeSliderView::create(const std::string & name)
//{
//  auto instance = std::make_shared<RangeSliderView>();
//  ofParameter<ofxCortex::core::types::Range> param { name, ofxCortex::core::types::Range(0, 1, 0, 1) };
//  instance->_setup(param);
//  return instance;
//}

void RangeSliderView::Bar::onUpdate(float time, float delta)
{
  ResizeableView::onUpdate(time, delta);
  
  float targetIntensity = isMouseInside ? (isMousePressed ? 2.0f : 1.5f) : 1.0f;
  _intensity = ofLerp(_intensity, targetIntensity, 1.0 - pow(0.02, delta));
}

void RangeSliderView::Bar::onDraw()
{
  auto BB = this->getFrame();
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

void RangeSliderView::viewDidLoad()
{
  this->disableSubviewRendering();
  
  bar = Bar::create("RangeSliderView::Bar");
  bar->disableY();
  bar->enableDebug();
  bar->disableOverlap();
  bar->setIntrinsicSize(1, Styling::getRowHeight());
  bar->setWidth(this->getWidth());
  bar->setGrabAreaDimensions(glm::vec2(16 * Styling::getScale(), 16 * Styling::getScale()));
  bar->onResizedE([this](const ResizeableView::ResizeEvent & e) {
    glm::vec2 minSize = bar->getMinSize();
    const auto & CBB = this->getContentFrame();
    float normalizedFrom = ofMap(e.bounds.getLeft(), CBB.getLeft(), CBB.getRight() - minSize.x, 0, 1, true);
    float normalizedTo = ofMap(e.bounds.getRight(), CBB.getLeft() + minSize.x, CBB.getRight(), 0, 1, true);
    
    bool hitLeft = ofIsFloatEqual(bar->getLeft(), (this->getContentLeft() - 0.0001f));
    bool hitRight = ofIsFloatEqual(bar->getRight(), (this->getContentRight() + 0.0001f));
    
    if ((hitLeft | hitRight) == 0)
    {
      ofxCortex::core::types::Range range = getParameterValue();
      range.from = ofMap(normalizedFrom, 0, 1, getParameter()->min, getParameter()->max);
      range.to = ofMap(normalizedTo, 0, 1, getParameter()->min, getParameter()->max);
      
      setParameter(range);
      _parameterSetInternally = true;
    }
  });
  this->addSubview(bar);
  
  onParameterChanged = getParameter().newListener([this](const ofxCortex::core::types::Range & e) {
    if (!_parameterSetInternally)
    {
      float normalLeft = ofMap(getParameter()->from, getParameter()->min, getParameter()->max, 0, 1, true);
      float normalRight = ofMap(getParameter()->to, getParameter()->min, getParameter()->max, 0, 1, true);
      _setLeftFromNormalized(normalLeft);
      _setRightFromNormalized(normalRight);
    }
    _parameterSetInternally = false;
  });
  
  this->setNeedsUpdateConstraints();
  LayoutEngine::forceSolve();
  
  float normalLeft = ofMap(getParameter()->from, getParameter()->min, getParameter()->max, 0, 1, true);
  float normalRight = ofMap(getParameter()->to, getParameter()->min, getParameter()->max, 0, 1, true);
  _setLeftFromNormalized(normalLeft);
  _setRightFromNormalized(normalRight);
}

void RangeSliderView::onDraw()
{
  Styling::drawBackground(this->getFrame());
  
  bar->draw();
  
  ofPushStyle();
  ofSetColor(Styling::getForegroundColor(), 255 - 128 * getActiveIntensity());
  Styling::drawLabel(getName(), this->getContentFrame());
  
  ofSetColor(Styling::getAccentColor(), 255 - 128 * getActiveIntensity());
  Styling::drawValue(getParameter()->toString(), this->getContentFrame());
  ofPopStyle();
}

void RangeSliderView::onMouseScrolled(const MouseEventArgs & e)
{
  getParameter() += -e.scrollX;
}

void RangeSliderView::onKeyPressed(const ofKeyEventArgs & e)
{
  View::onKeyPressed(e);
  
  if (!isFocused()) return;
  
  float delta = (e.key == OF_KEY_LEFT) ? -1.0f : (e.key == OF_KEY_RIGHT) ? 1.0f : 0.0f;
  
  if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
  else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
  else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
  
  getParameter() += delta;
}

void RangeSliderView::onWindowResized(const ofResizeEventArgs & e)
{
  View::onWindowResized(e);
  
  bar->setGrabAreaDimensions(glm::vec2(16 * Styling::getScale(), 16 * Styling::getScale()));
}

void RangeSliderView::updateConstraints()
{
  View::updateConstraints();
  
  this->addConstraints({
    { bar->left >= this->content_left | kiwi::strength::strong },
    { bar->right <= this->content_right | kiwi::strength::strong },
    { bar->top == this->content_top | kiwi::strength::strong },
    { bar->bottom == this->content_bottom | kiwi::strength::strong }
  });
}

}}
