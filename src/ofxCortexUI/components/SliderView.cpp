#include "SliderView.h"

namespace ofxCortex { namespace ui {

template<typename T>
void SliderView<T>::onDraw()
{
  Styling::drawBackground(this->getFrame());
  this->drawSlider();
  
  ofSetColor(Styling::getForegroundColor(), 255);
  this->drawDot();
  
  ofSetColor(Styling::getForegroundColor(), 255 - 128 * View::getActiveIntensity());
  Styling::drawLabel(ParameterView<T>::getParameterName(), this->getContentFrame());
  
  ofSetColor(Styling::getAccentColor(), 255 - 128 * View::getActiveIntensity());
  Styling::drawValue(ofToString(ParameterView<T>::getParameterValue(), 3) + ParameterView<T>::getUnit(), this->getContentFrame());
}

template<typename T>
void SliderView<T>::drawSlider()
{
  const auto & rect = this->getContentFrame();
  float centerY = rect.getCenter().y;
  float centerBulge = (ParameterView<T>::getParameterMin() < 0.0) ? ofMap(0, ParameterView<T>::getParameterMin(), ParameterView<T>::getParameterMax(), 0, 1, true) : 0.5;
  
  ofFloatColor sliderColor = Styling::getForegroundColor();
  sliderColor.a = ofMap(interpolation, 0, 1, 0.6, 1.0);
  
  ofPushStyle();
  {
    SliderView<T>::getShader().begin();
    SliderView<T>::getShader().setUniform1f("u_center", centerBulge);
    SliderView<T>::getShader().setUniform4f("u_color", sliderColor);
    ofDrawPlane(rect.getCenter(), rect.width, 0.5);
    SliderView<T>::getShader().end();
    
    ofFill();
    if (ParameterView<T>::getParameterMin() < 0.0) this->drawZero();
  }
  ofPopStyle();
}

template<typename T>
void SliderView<T>::drawZero()
{
  const auto & rect = this->getContentFrame();
  float zeroPoint = ofMap(0, ParameterView<T>::getParameterMin(), ParameterView<T>::getParameterMax(), rect.getLeft() + Styling::getPaddingLeft(), rect.getRight() - Styling::getPaddingRight());
  
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
void SliderView<T>::drawDot()
{
  const auto & BB = this->getContentFrame();
  
  float x = ofMap(ParameterView<T>::getParameterValue(), ParameterView<T>::getParameterMin(), ParameterView<T>::getParameterMax(), this->content_left.value() + Styling::getPaddingLeft(), this->content_right.value() - Styling::getPaddingRight(), true);
  
  ofPushStyle();
  ofSetColor(Styling::getForegroundColor());
  ofDrawCircle(x, BB.getCenter().y, Styling::getDotSize());
  
  ofSetColor(Styling::getAccentColor());
  ofDrawCircle(x, BB.getCenter().y, Styling::getDotSize() * 0.5 * View::getHoverIntensity() + 1 * View::getActiveIntensity() * Styling::getScale());
  ofPopStyle();
}

template<typename T>
void SliderView<T>::onMousePressed(const View::MouseEventArgs & e)
{
  View::onMousePressed(e);
  
  if (e.isOverlapped) return;
  
  float normalizedX = ofMap(e.x - 2, this->content_left.value() + Styling::getPaddingLeft(), this->content_right.value() - Styling::getPaddingRight(), 0, 1, true);
  float parameterValue = ofMap(normalizedX, 0, 1, ParameterView<T>::getParameterMin(), ParameterView<T>::getParameterMax());
  ParameterView<T>::setParameter(parameterValue);
}

template<typename T>
void SliderView<T>::onMouseDragged(const View::MouseEventArgs & e)
{
  View::onMouseDragged(e);
  
  if (e.isOverlapped) return;
  
  float normalizedX = ofMap(e.x - 2, this->content_left.value() + Styling::getPaddingLeft(), this->content_right.value() - Styling::getPaddingRight(), 0, 1, true);
  float parameterValue = ofMap(normalizedX, 0, 1, ParameterView<T>::getParameterMin(), ParameterView<T>::getParameterMax());
  ParameterView<T>::setParameter(parameterValue);
}

template<typename T>
void SliderView<T>::onMouseScrolled(const View::MouseEventArgs & e)
{
  if (!View::isFocused()) return;
  
  if (e.modifiers == OF_KEY_SHIFT)
  {
    ParameterView<T>::getParameter() = (e.scrollX > 0.0f) ? ceil(ParameterView<T>::getParameterValue() - 0.00001f) + 1.0 : floor(ParameterView<T>::getParameterValue() + 0.00001f) - 1.0;
  }
  else
  {
    float delta = 0.0166f;
    
    if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) delta *= 0.001f;
    else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) delta *= 0.01f;
    else if (e.modifiers == OF_KEY_COMMAND) delta *= 0.1f;
    
    ParameterView<T>::getParameter() += e.scrollX * delta;
  }
}

template<typename T>
void SliderView<T>::onKeyPressed(const ofKeyEventArgs & e)
{
  View::onKeyPressed(e);
  
  if (!View::isFocused()) return;
  
  float multiplier = 1.0;
  if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT + OF_KEY_SHIFT)) multiplier = 1000.0;
  else if (e.modifiers == (OF_KEY_COMMAND + OF_KEY_ALT)) multiplier = 100.0;
  else if (e.modifiers == OF_KEY_COMMAND) multiplier = 10.0;
  
  switch(e.key)
  {
    case OF_KEY_LEFT:
    {
      if (e.modifiers == OF_KEY_SHIFT) { ParameterView<T>::getParameter() = floor(ParameterView<T>::getParameterValue() - 0.000001f) + 1; }
      
      ParameterView<T>::getParameter() += -1.0 / multiplier;
    }
    break;
    case OF_KEY_DOWN:
    {
      if (e.modifiers == OF_KEY_SHIFT) { ParameterView<T>::getParameter() = floor(ParameterView<T>::getParameterValue() - 0.000001f) + 1; }
      
      ParameterView<T>::getParameter() += -10.0 * multiplier;
    }
    break;
    case OF_KEY_RIGHT:
    {
      if (e.modifiers == OF_KEY_SHIFT) { ParameterView<T>::getParameter() = ceil(ParameterView<T>::getParameterValue() + 0.000001f) - 1; }
      
      ParameterView<T>::getParameter() += 1.0 / multiplier;
    }
    break;
    case OF_KEY_UP:
    {
      if (e.modifiers == OF_KEY_SHIFT) { ParameterView<T>::getParameter() = ceil(ParameterView<T>::getParameterValue() + 0.000001f) - 1; }
      
      ParameterView<T>::getParameter() += 10.0 * multiplier;
    }
    break;
  }
}

template<typename T>
ofShader & SliderView<T>::getShader()
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
void SliderView<int>::onDraw()
{
  Styling::drawBackground(this->getFrame());
  
  this->drawSlider();
  
  int parameterRange = ParameterView<int>::getParameterMax() - ParameterView<int>::getParameterMin();
  float stepWidth = this->getWidth() / parameterRange;
  int stepSize = parameterRange / 10; //(stepWidth < 5.0f) ? 5 : 1;
  
  ofSetColor(Styling::getForegroundColor(), 64 + (64 * View::getHoverIntensity() * View::getActiveIntensity()));
  
  for (int i = ParameterView<int>::getParameterMin() + 1; i <= ParameterView<int>::getParameterMax() - 1; i++)
  {
//    if (i == parameter.getMin())
    
    float x = ofMap(i, getParameter().getMin(), getParameter().getMax(), this->content_left.value() + Styling::getPaddingLeft(), this->content_right.value() - Styling::getPaddingRight(), true);
    
    float scaleY = 1.0 + 2.0 * getActiveIntensity() + 1.0 * getHoverIntensity();
    float baseOffset = 2.5 * (stepWidth > 3.0f);
    baseOffset += 2.5 * (i % 5 == 0) * (stepWidth >= 1.0f);
    baseOffset += 2.5 * (i % 10 == 0);
    baseOffset += 2.5 * (i % 100 == 0);
    baseOffset *= Styling::getScale();
    
//      ofDrawCircle(x, this->contentFrame.getCenter().y, 1.5 + interpolation);
    if (baseOffset > 0.0f ) ofDrawLine(x, this->getContentFrame().getCenter().y - baseOffset - scaleY, x, this->getContentFrame().getCenter().y + baseOffset + scaleY);
  }
  
  
  ofSetColor(Styling::getForegroundColor(), 255);
  this->drawDot();
  
  ofSetColor(Styling::getForegroundColor(), 255 - 128 * getActiveIntensity());
  Styling::drawLabel(getParameter().getName(), this->getContentFrame());
  
  ofSetColor(Styling::getAccentColor(), 255 - 128 * getActiveIntensity());
  Styling::drawValue(ofToString(getParameter().get(), 3) + ParameterView<int>::getUnit(), this->getContentFrame());
}

template<>
void SliderView<int>::onMousePressed(const MouseEventArgs & e)
{
  View::onMousePressed(e);
  
  float parameterRange = ParameterView<int>::getParameterMax() - ParameterView<int>::getParameterMin();
  float stepWidth = this->getContentFrame().width / parameterRange;
  
  float parameterValue = ofMap(ofxCortex::core::utils::roundToNearest(e.x, stepWidth), this->content_left.value() + Styling::getPaddingLeft(), this->content_right.value() - Styling::getPaddingRight(), getParameter().getMin(), getParameter().getMax(), true);
  ParameterView<int>::setParameter(parameterValue);
}

template<>
void SliderView<int>::onMouseScrolled(const View::MouseEventArgs & e)
{
  if (!View::isFocused()) return;
  
  ParameterView<int>::getParameter() += ofSign(e.scrollX) * ceil(abs(e.scrollX));
}

template<>
void SliderView<int>::onMouseDragged(const MouseEventArgs & e)
{
  View::onMouseDragged(e);
  
  float parameterRange = ParameterView<int>::getParameterMax() - ParameterView<int>::getParameterMin();
  float stepWidth = this->getContentFrame().width / parameterRange;
  
  float parameterValue = ofMap(ofxCortex::core::utils::roundToNearest(e.x, stepWidth), this->content_left.value() + Styling::getPaddingLeft(), this->content_right.value() - Styling::getPaddingRight(), getParameter().getMin(), getParameter().getMax(), true);
  ParameterView<int>::setParameter(parameterValue);
}


template class SliderView<float>;
template class SliderView<int>;
template class SliderView<double>;

}}
