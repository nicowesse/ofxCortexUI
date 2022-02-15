#include "RangeSlider.h"

namespace ofxCortex { namespace ui {

void RangeSlider::Bar::drawBar(ofFloatColor acc, float horizontalPadding)
{
  ofRectangle rect = getRect();
  rect.setFromCenter(rect.getCenter(), rect.width + horizontalPadding, 16);
  ofPushStyle();
  {
    ofSetColor(acc);
    ofDrawRectRounded(rect, rect.height / 2.0f);
    
  }
  ofPopStyle();
}

DisplayObject* RangeSlider::Bar::setLeft(float x, bool adjustLayout)
{
  x = ofClamp(x, _dragBounds.getLeft(), _dragBounds.getRight());
  return DisplayObject::setLeft(x, false);
}

DisplayObject* RangeSlider::Bar::setRight(float x, bool adjustLayout)
{
  x = ofClamp(x, _dragBounds.getLeft(), _dragBounds.getRight());
  
  return DisplayObject::setRight(x, false);
}

void RangeSlider::drawSlider()
{
  const auto & rect = getRenderRect();
  float centerY = rect.getCenter().y;
  float centerBulge = (parameter->min < 0.0) ? ofMap(0, parameter->min, parameter->max, 0, 1, true) : 0.5;
  
  ofPushStyle();
  {
    bar->drawBar(ofFloatColor(style->foregroundColor, 1.0), fromHandle->getWidth() + toHandle->getWidth() + 2);
    
    Slider<float>::_getShader().begin();
    Slider<float>::_getShader().setUniform1f("u_center", centerBulge);
    ofDrawPlane(rect.getCenter(), rect.width, 1);
    Slider<float>::_getShader().end();
    
    if (parameter->min < 0.0) this->drawZero();
    fromHandle->drawHandle(style->accentColor, ofFloatColor(0, 0), 8);
    toHandle->drawHandle(style->accentColor, ofFloatColor(0, 0), 8);
  }
  ofPopStyle();
}

void RangeSlider::drawZero()
{
  const auto & rect = getRenderRect();
  float zeroPoint = ofMap(0, parameter->min, parameter->max, rect.getLeft(), rect.getRight());
  
  float r = 4;
  float alpha = 0.6;
  
  ofPushStyle();
  {
    ofSetColor(style->backgroundColor);
    ofDrawCircle(zeroPoint, rect.getCenter().y, (r + 2) * 0.5);
    
    ofSetColor(style->accentColor, 255 * alpha);
    ofDrawCircle(zeroPoint, rect.getCenter().y, r * 0.5);
  }
  ofPopStyle();
}

void RangeSlider::_init()
{
  background = ofxCortex::ui::Background::create();
  background->setName("RangeSlider::Background");
  background->disableEvents();
  this->addChild(background);
  
  label = ofxCortex::ui::Label<ofxCortex::types::Range>::create(parameter);
  label->setName("RangeSlider::Label");
  label->disableEvents();
  this->addChild(label);
  
  value = ofxCortex::ui::Value<ofxCortex::types::Range>::create(parameter);
  value->setName("RangeSlider::Value");
  value->disableEvents();
  this->addChild(value);
  
  fromHandle = make_shared<Handle>(getRenderRect());
  fromHandle->setName("RangeSlider::From Handle");
  fromHandle->setWidth(16);
  this->addChild(fromHandle);
  
  toHandle = make_shared<Handle>(getRenderRect());
  toHandle->setName("RangeSlider::To Handle");
  toHandle->setWidth(16);
  this->addChild(toHandle);
  
  auto onHandleDragged = [this](Draggable::DraggableEventArgs e) {
    auto minHandle = _getMinHandle();
    auto maxHandle = _getMaxHandle();
    
    ofxCortex::types::Range range = parameter.get();
    range.from = ofMap(minHandle->getNormalizedPosition().x, 0, 1, parameter->min, parameter->max);
    range.to = ofMap(maxHandle->getNormalizedPosition().x, 0, 1, parameter->min, parameter->max);
    parameter.set(range);
    
    bar->setLeft(minHandle->getRect().getRight());
    bar->setRight(maxHandle->getRect().getLeft());
  };
  
  _eventListeners.push(fromHandle->onDragE.newListener(onHandleDragged));
  _eventListeners.push(toHandle->onDragE.newListener(onHandleDragged));
  
  bar = make_shared<Bar>(getRenderRect());
  bar->setName("RangeSlider::Bar");
  bar->setWidth(2);
  this->addChild(bar);
  
  _eventListeners.push(bar->onDragE.newListener([this](Draggable::DraggableEventArgs & e) {
    fromHandle->setX(bar->getRect().getLeft() - fromHandle->getWidth());
    toHandle->setX(bar->getRect().getRight());
    
    auto minHandle = _getMinHandle();
    auto maxHandle = _getMaxHandle();
    
    ofxCortex::types::Range range = parameter.get();
    range.from = ofMap(minHandle->getNormalizedPosition().x, 0, 1, parameter->min, parameter->max);
    range.to = ofMap(maxHandle->getNormalizedPosition().x, 0, 1, parameter->min, parameter->max);
    parameter.set(range);
  }));
  
  _eventListeners.push(parameter.newListener([this](const ofxCortex::types::Range & value) {
    fromHandle->setFromNormalizedX(ofMap(value.from, value.min, value.max, 0, 1, true));
    toHandle->setFromNormalizedX(ofMap(value.to, value.min, value.max, 0, 1, true));
    
    bar->setLeft(fromHandle->getRect().getRight());
    bar->setRight(toHandle->getRect().getLeft());
  }));
  
  _adjustLayout();
}

void RangeSlider::_update()
{
  DisplayObject::_update();
}

void RangeSlider::_draw()
{
  background->drawBackground();
  this->drawSlider();
  
  label->drawLabel(ofFloatColor(style->labelFontColor, 0.6));
  value->drawValue(ofFloatColor(style->valueFontColor, 0.6));
  
  _drawFocusOutline();
}

void RangeSlider::_debug()
{
  View::_debug();
  
  fromHandle->debug();
  toHandle->debug();
}

void RangeSlider::_adjustLayout()
{
  View::_recalculateRenderRect();
  
  background->setRect(this->getRect());
  label->setRect(this->getRect());
  value->setRect(this->getRect());
  
  ofRectangle dragBounds = getRenderRect();
  dragBounds.setFromCenter(dragBounds.getCenter(), dragBounds.width - 12, dragBounds.height);
  
  fromHandle->setDragBounds(dragBounds);
  fromHandle->setHeight(dragBounds.height);
  fromHandle->setY(dragBounds.y);
  fromHandle->setFromNormalizedX(parameter->getNormalizedFrom());
  
  toHandle->setDragBounds(dragBounds);
  toHandle->setHeight(dragBounds.height);
  toHandle->setY(dragBounds.y);
  toHandle->setFromNormalizedX(parameter->getNormalizedTo());
  
//  dragBounds.setFromCenter(dragBounds.getCenter(), dragBounds.width - (fromHandle->getWidth() + toHandle->getWidth()), dragBounds.height);
  
  bar->setDragBounds(dragBounds);
  bar->setHeight(dragBounds.height);
  bar->setY(dragBounds.y);
  bar->setLeft(_getMinHandle()->getRect().getRight());
  bar->setRight(_getMaxHandle()->getRect().getLeft());
  
  DisplayObject::_adjustLayout();
}

void RangeSlider::_mousePressed(const ofMouseEventArgs & e)
{
  View::_mousePressed(e);
  
  glm::vec2 localCoord = globalToLocal(e);
}

void RangeSlider::_mouseScrolled(const ofMouseEventArgs & e)
{
  if (!hasFocus()) return;
  
  float delta = 0.0166f;
  
  if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
  else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
  else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
  
  parameter += e.scrollY * delta;
}

void RangeSlider::_keyPressed(const ofKeyEventArgs & e)
{
  if (!hasFocus()) return;
  
  switch(e.key)
  {
    case OF_KEY_LEFT:
    {
      float delta = 1.0f;
      
      if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
      else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
      else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
      
      parameter -= delta;
    }
    break;
    case OF_KEY_RIGHT:
    {
      float delta = 1.0f;
      
      if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
      else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
      else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
      
      parameter += delta;
    }
    break;
  }
}


}}
