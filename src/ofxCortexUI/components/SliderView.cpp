#include "SliderView.h"

namespace ofxCortex { namespace ui {

template<typename T>
void SliderView<T>::_draw()
{
  Styling::drawBackground(this->getBounds());
  this->_drawSlider();
  
  ofSetColor(Styling::getForegroundColor(), 255);
  _drawDot();
  
  ofSetColor(Styling::getForegroundColor(), 255 - 128 * getActiveIntensity());
  Styling::drawLabel(parameter.getName(), this->getContentBounds());
  
  ofSetColor(Styling::getAccentColor(), 255 - 128 * getActiveIntensity());
  Styling::drawValue(ofToString(parameter.get(), 3), this->getContentBounds());
}

template<typename T>
void SliderView<T>::_drawSlider()
{
  const auto & rect = this->getContentBounds();
  float centerY = rect.getCenter().y;
  float centerBulge = (parameter.getMin() < 0.0) ? ofMap(0, parameter.getMin(), parameter.getMax(), 0, 1, true) : 0.5;
  
  ofFloatColor sliderColor = Styling::getForegroundColor();
  sliderColor.a = ofMap(interpolation, 0, 1, 0.6, 1.0);
  
  ofPushStyle();
  {
    _getShader().begin();
    _getShader().setUniform1f("u_center", centerBulge);
    _getShader().setUniform4f("u_color", sliderColor);
    ofDrawPlane(rect.getCenter(), rect.width, 0.5);
    _getShader().end();
    
    ofFill();
    if (parameter.getMin() < 0.0) this->_drawZero();
  }
  ofPopStyle();
}

template<typename T>
void SliderView<T>::_drawZero()
{
  const auto & rect = this->getContentBounds();
  float zeroPoint = ofMap(0, parameter.getMin(), parameter.getMax(), rect.getLeft() + _inset, rect.getRight() - _inset);
  
  float r = 4;
  float alpha = 0.6;
  
  ofPushStyle();
  {
    ofSetColor(Styling::getBackgroundColor());
    ofDrawCircle(zeroPoint, rect.getCenter().y, (r + 2) * 0.5);
    
    ofSetColor(Styling::getForegroundColor(), 255 * alpha);
    ofDrawCircle(zeroPoint, rect.getCenter().y, r * 0.5);
  }
  ofPopStyle();
}

template<typename T>
void SliderView<T>::_drawDot()
{
  const auto & BB = this->getContentBounds();
  
  float x = ofMap(parameter.get(), parameter.getMin(), parameter.getMax(), this->content_left.value() + _inset, this->content_right.value() - _inset, true);
  
  ofPushStyle();
  ofSetColor(Styling::getForegroundColor());
  ofDrawCircle(x, BB.getCenter().y, Styling::getDotSize());
  
  ofSetColor(Styling::getAccentColor());
  ofDrawCircle(x, BB.getCenter().y, Styling::getDotSize() * 0.5 * getHoverIntensity() + 1 * getActiveIntensity() * Styling::getScale());
  ofPopStyle();
}

template<typename T>
void SliderView<T>::_mousePressed(const MouseEventArgs & e)
{
  View::_mousePressed(e);
  
  float normalizedX = ofMap(e.x - 2, this->content_left.value() + _inset, this->content_right.value() - _inset, 0, 1, true);
  float parameterValue = ofMap(normalizedX, 0, 1, parameter.getMin(), parameter.getMax());
  parameter.set(parameterValue);
}

template<typename T>
void SliderView<T>::_mouseDragged(const DeltaMouseEvent & e)
{
  View::_mousePressed(e);
  
  float normalizedX = ofMap(e.x - 2, this->content_left.value() + _inset, this->content_right.value() - _inset, 0, 1, true);
  float parameterValue = ofMap(normalizedX, 0, 1, parameter.getMin(), parameter.getMax());
  parameter.set(parameterValue);
}

template<typename T>
void SliderView<T>::_mouseScrolled(const MouseEventArgs & e)
{
  if (!_isFocused()) return;
  
  if (e.modifiers == OF_KEY_SHIFT)
  {
    parameter = (e.scrollX > 0.0f) ? floor(parameter.get() - 0.00001f) : ceil(parameter.get() + 0.00001f);
  }
  else
  {
    float delta = 0.0166f;
    
    if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
    else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
    else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
    
    parameter += e.scrollX * delta;
  }
}

template<typename T>
void SliderView<T>::_keyPressed(const ofKeyEventArgs & e)
{
  View::_keyPressed(e);
  
  if (!_isFocused()) return;
  
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
      
//      _handleOpacity = 1.0f;
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
      
//      _handleOpacity = 1.0f;
    }
    break;
  }
}

template<typename T>
ofShader & SliderView<T>::_getShader()
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
      uniform vec4 u_color = vec4(1.0);
      
      void main() {
        float alpha = pcurve(texCoordVarying.x, u_center, 1.0);
        outputColor = vec4(u_color.r, u_color.g, u_color.b, u_color.a * alpha);
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

template<>
void SliderView<int>::_draw()
{
  Styling::drawBackground(this->getBounds());
  
  _drawSlider();
  
  
  float stepWidth = this->getWidth() / (parameter.getMax() - parameter.getMin());
  int stepSize = (parameter.getMax() - parameter.getMin()) / 10; //(stepWidth < 5.0f) ? 5 : 1;
  
  ofSetColor(Styling::getForegroundColor(), 64 + (64 * getHoverIntensity() * getActiveIntensity()));
  for (int i = parameter.getMin() + 1; i <= parameter.getMax() - 1; i++)
  {
//    if (i == parameter.getMin())
    
    float x = ofMap(i, parameter.getMin(), parameter.getMax(), this->content_left.value() + _inset, this->content_right.value() - _inset, true);
    
    float scaleY = 1.0 + 2.0 * getActiveIntensity() + 1.0 * getHoverIntensity();
    float baseOffset = 2.5 * (stepWidth > 3.0f);
    baseOffset += 2.5 * (i % 5 == 0) * (stepWidth >= 1.0f);
    baseOffset += 2.5 * (i % 10 == 0);
    baseOffset += 2.5 * (i % 100 == 0);
    baseOffset *= Styling::getScale();
    
//      ofDrawCircle(x, this->contentBounds.getCenter().y, 1.5 + interpolation);
    if (baseOffset > 0.0f ) ofDrawLine(x, this->getContentBounds().getCenter().y - baseOffset - scaleY, x, this->getContentBounds().getCenter().y + baseOffset + scaleY);
  }
  
  
  ofSetColor(Styling::getForegroundColor(), 255);
  _drawDot();
  
  ofSetColor(Styling::getForegroundColor(), 255 - 128 * getActiveIntensity());
  Styling::drawLabel(parameter.getName(), this->getContentBounds());
  
  ofSetColor(Styling::getAccentColor(), 255 - 128 * getActiveIntensity());
  Styling::drawValue(ofToString(parameter.get(), 3), this->getContentBounds());
}

template<>
void SliderView<int>::_mousePressed(const MouseEventArgs & e)
{
  View::_mousePressed(e);
  
  if (!_wasMousePressedInside) return;
  
  float parameterRange = parameter.getMax() - parameter.getMin();
  float halfStepWidth = this->getContentBounds().width / parameterRange * 0.5;
  
  float normalizedX = ofMap(e.x + halfStepWidth, this->content_left.value() + _inset, this->content_right.value() - _inset, 0, 1, true);
  float parameterValue = ofMap(normalizedX, 0, 1, parameter.getMin(), parameter.getMax());
  parameter.set(parameterValue);
}

template<>
void SliderView<int>::_mouseDragged(const DeltaMouseEvent & e)
{
  View::_mousePressed(e);
  
  if (!_wasMousePressedInside) return;
  
  float parameterRange = parameter.getMax() - parameter.getMin();
  float halfStepWidth = this->getContentBounds().width / parameterRange * 0.5;
  
  float normalizedX = ofMap(e.x + halfStepWidth, this->content_left.value() + _inset, this->content_right.value() - _inset, 0, 1, true);
  float parameterValue = ofMap(normalizedX, 0, 1, parameter.getMin(), parameter.getMax());
  parameter.set(parameterValue);
}


template class SliderView<float>;
template class SliderView<int>;
template class SliderView<double>;

}}
