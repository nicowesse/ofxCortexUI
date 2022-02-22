#include "Slider.h"

namespace ofxCortex { namespace ui {

void Handle::drawHandle(ofFloatColor acc, ofFloatColor bg, float diameter)
{
  ofPushStyle();
  {
    ofSetColor(bg);
    ofDrawCircle(getRect().getCenter(), (diameter + 4) * 0.5);
    
    ofSetColor(acc);
    ofDrawCircle(getRect().getCenter(), diameter * 0.5);
  }
  ofPopStyle();
}

template<typename T>
void Slider<T>::drawSlider()
{
  const auto & rect = getRenderRect();
  float centerY = rect.getCenter().y;
  float centerBulge = (parameter.getMin() < 0.0) ? ofMap(0, parameter.getMin(), parameter.getMax(), 0, 1, true) : 0.5;
  
  ofPushStyle();
  {
    _getShader().begin();
    _getShader().setUniform1f("u_center", centerBulge);
    ofDrawPlane(rect.getCenter(), rect.width - handle->getWidth(), 1);
    _getShader().end();
    
    // Rounded Rect BG
//      ofRectangle rect = getRenderRect();
//      rect.setFromCenter(rect.getCenter(), rect.width - 8, 20);
//
//      ofSetColor(style->foregroundColor);
//      ofDrawRectRounded(rect, rect.height / 2);
    
    
    ofFill();
    if (parameter.getMin() < 0.0) this->drawZero();
    handle->drawHandle(ofFloatColor(style->accentColor * _handleOpacity), ofFloatColor(style->backgroundColor * _handleOpacity), 6.0 + (2.0 * _handleOpacity));
  }
  ofPopStyle();
}

template<typename T>
void Slider<T>::drawZero()
{
  const auto & rect = getRenderRect();
  float zeroPoint = ofMap(0, parameter.getMin(), parameter.getMax(), rect.getLeft(), rect.getRight());
  
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

template<typename T>
void Slider<T>::_init()
{
  background = ofxCortex::ui::Background::create();
  background->setName("Slider::Background");
  background->disableEvents();
  this->addChild(background);
  
  label = ofxCortex::ui::Label<T>::create(parameter);
  label->setName("Slider::Label");
  label->disableEvents();
  this->addChild(label);
  
  value = ofxCortex::ui::Value<T>::create(parameter);
  value->setName("Slider::Value");
  value->disableEvents();
  this->addChild(value);
  
  handle = make_shared<Handle>(getRenderRect());
  handle->setName("Slider::Handle");
  handle->setWidth(16);
  this->addChild(handle);
  
  _eventListeners.push(handle->onDragE.newListener([this](Draggable::DraggableEventArgs e) {
    parameter.set(ofMap(handle->getNormalizedPosition().x, 0, 1, parameter.getMin(), parameter.getMax()));
    _handleOpacity = ofClamp(_handleOpacity + abs(e.delta.x), 0.7, 1.0);
  }));
  
  _eventListeners.push(parameter.newListener([this](const T & value) {
    handle->setFromNormalizedX(ofMap(value, parameter.getMin(), parameter.getMax(), 0, 1, true));
  }));
}

template<typename T>
void Slider<T>::_update()
{
  DisplayObject::_update();
  
  if (!_isMousePressedInside) _handleOpacity = ofClamp(_handleOpacity - _deltaTime, 0.7, 1.0);
}

template<typename T>
void Slider<T>::_draw()
{
  background->drawBackground();
  this->drawSlider();
  
  label->drawLabel(ofFloatColor(style->labelFontColor, _textOpacity));
  value->drawValue(ofFloatColor(style->valueFontColor, _textOpacity));
  
  View::_drawFocusOutline();
}

template<typename T>
void Slider<T>::_debug()
{
  View::_debug();
  
  handle->debug();
}

template<typename T>
void Slider<T>::_adjustLayout()
{
  View::_recalculateRenderRect();
  
  background->setRect(this->getRect());
  label->setRect(this->getRect());
  value->setRect(this->getRect());
  
  ofRectangle dragBounds = getRenderRect();
  dragBounds.setFromCenter(dragBounds.getCenter(), dragBounds.width - 12, dragBounds.height);
  
  handle->setDragBounds(dragBounds);
  handle->setHeight(dragBounds.height);
  handle->setY(dragBounds.y);
  handle->setFromNormalizedX(getNormalizedParameter());
  
  DisplayObject::_adjustLayout();
}

template<typename T>
void Slider<T>::_mousePressed(const ofMouseEventArgs & e)
{
  View::_mousePressed(e);
  
  glm::vec2 localCoord = globalToLocal(e);
  
  if (hasFocus() && !handle->isInsideRectangle(e, true))
  {
    ofRectangle sliderRect = handle->getDragBounds();
    sliderRect.setFromCenter(sliderRect.getCenter(), sliderRect.width - handle->getWidth(), sliderRect.height);
    float normalizedFromX = ofMap(localCoord.x, sliderRect.getLeft(), sliderRect.getRight(), 0, 1, true);
    parameter.set(ofMap(normalizedFromX, 0, 1, parameter.getMin(), parameter.getMax()));
  }
  
  _handleOpacity = 1.0f;
}

template<typename T>
void Slider<T>::_mouseScrolled(const ofMouseEventArgs & e)
{
  if (!hasFocus()) return;
  
  if (e.modifiers == OF_KEY_SHIFT)
  {
    parameter = (e.scrollY < 0.0f) ? floor(parameter.get() - 0.00001f) : ceil(parameter.get() + 0.00001f);
  }
  else
  {
    float delta = 0.0166f;
    
    if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
    else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
    else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
    
    parameter += e.scrollY * delta;
  }
  
  _handleOpacity = ofClamp(_handleOpacity + abs(e.scrollY), 0.7, 1.0);
}

template<typename T>
void Slider<T>::_keyPressed(const ofKeyEventArgs & e)
{
  if (!hasFocus()) return;
  
  switch(e.key)
  {
    case OF_KEY_LEFT:
    {
      if (e.modifiers == OF_KEY_SHIFT)
      {
        parameter = floor(parameter.get() - 0.000001f) + 1;
      }
      
      float delta = -1.0f;
      
      if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
      else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
      else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
      
      parameter += delta;
      
      _handleOpacity = 1.0f;
    }
    break;
    case OF_KEY_RIGHT:
    {
      if (e.modifiers == OF_KEY_SHIFT)
      {
        parameter = ceil(parameter.get() + 0.000001f) - 1;
      }
      
      float delta = 1.0f;
      
      if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
      else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
      else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
      
      parameter += delta;
      
      _handleOpacity = 1.0f;
    }
    break;
  }
}

template<typename T>
ofShader & Slider<T>::_getShader()
{
  static ofShader shader;
  
  if (!shader.isLoaded())
  {
    string vertexShader;
    string fragShader;
    
    if (ofIsGLProgrammableRenderer())
    {
      vertexShader = R"(
      #version 150
      
      uniform mat4 modelViewProjectionMatrix;
      
      in vec4 position;
      in vec4 color;
      in vec4 normal;
      in vec2 texcoord;
      
      out vec2 texCoordVarying;
      
      void main() {
        texCoordVarying = texcoord;
        gl_Position = modelViewProjectionMatrix * position;
      }
      )";
      
      fragShader =
      R"(
      #version 150
      
      const float PI = 3.1415926535897932384626433832795;
      
      float parabola( float x, float k )
      {
          return pow( 4.0 * x * (1.0 - x), k );
      }
      
      float pcurve( float x, float c, float s )
      {
          float a = c * s;
          float b = (1.0 - c) * s;
          float k = pow(a + b, a + b) / (pow(a, a) * pow(b, b));
      
          return k * pow(x, a) * pow(1.0 - x, b);
      }
      
      in vec2 texCoordVarying;
      out vec4 outputColor;
      
      uniform float u_center = 0.5;
      
      void main() {
        float alpha = pcurve(texCoordVarying.x, u_center, 6.0);
        outputColor = vec4(1.0, 1.0, 1.0, alpha);
      }
      )";
    }
    
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.bindDefaults();
    shader.linkProgram();
  }
  
  return shader;
}

template class Slider<float>;

}}
