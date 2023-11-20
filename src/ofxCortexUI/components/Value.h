#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Label.h"
#include "ofxCortex/types/Range.h"
#include "ofxCortex/types/Image.h"

namespace ofxCortex { namespace ui {

template<typename T>
class Value : public ofxCortex::ui::View {
public:
  
  Value(string name, T value)
  {
    setName(name);
    
    parameter.setName(name);
    parameter.set(value);
    
    _init();
    _adjustLayout();
  }
  static shared_ptr<Value<T>> create(string name, T value) { return make_shared<Value<T>>(name, value); }
  
  Value(ofParameter<T> param)
  {
    setName(param.getName());
    parameter.makeReferenceTo(param);
    
    _init();
    _adjustLayout();
  }
  static shared_ptr<Value<T>> create(ofParameter<T> parameter) { return make_shared<Value<T>>(parameter); }
  
  
  void drawValue(ofFloatColor color)
  {
    const auto & rect = getRenderRect();
    
    if (!style->getValueFont()->isLoaded())
    {
      ofLogWarning(_getLogModule()) << "Value font is not loaded. Please make sure the paths are correct. Returning.."; return;
    }
    
    string value = _getFormattedString();
    float valueWidth = style->getValueFont()->stringWidth(value);
    float x = rect.getRight() - 12 - valueWidth;
    float y = rect.getCenter().y + (style->getFontXHeight() * 0.5);
    
    ofPushStyle();
    {
      ofSetColor(color);
      style->getValueFont()->drawString(value, x, y);
    }
    ofPopStyle();
  }
  
  void drawValue()
  {
    this->drawValue(style->valueFontColor);
  }
  
  virtual bool hasParameter() const override { return true; }
  virtual ofParameter<T> & getParameter() { return parameter; }
  
protected:
  virtual string _getModule() const override { return "Value"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("Value::Background");
    background->disableEvents();
    
    label = ui::Label::create(parameter);
    label->setName("Value::Label");
    label->disableEvents();
  }
  
  virtual void _draw() override
  {
    background->drawBackground();
    label->drawLabel();
    this->drawValue();
  }
  
  virtual void _adjustLayout() override
  {
    background->setRect(this->getRect());
    label->setRect(this->getRect());
    
    View::_adjustLayout();
  }
  
  string _getFormattedString()
  {
    stringstream ss;
    ss << parameter;
    return ofToString(ss.str(), 1);
  }
  
  shared_ptr<ui::Background> background;
  shared_ptr<ui::Label> label;
  ofParameter<T> parameter;

};

template<>
inline string Value<float>::_getFormattedString()
{
  int precision = 3;
  stringstream ss;
  ss << ofToString(parameter.get(), precision);
  return ss.str();
}

template<>
inline string Value<glm::vec2>::_getFormattedString()
{
  int precision = 3;
  stringstream ss;
  ss << ofToString(parameter->x, precision) << ", " << ofToString(parameter->y, precision);
  return ss.str();
}

template<>
inline string Value<glm::vec3>::_getFormattedString()
{
  int precision = 3;
  stringstream ss;
  ss << ofToString(parameter->x, precision) << ", " << ofToString(parameter->y, precision) << ", " << ofToString(parameter->z, precision);
  return ss.str();
}

template<>
inline string Value<ofxCortex::core::types::Range>::_getFormattedString()
{
  int precision = 2;
  stringstream ss;
  ss << ofToString(parameter->from, precision) << " ←→ " << ofToString(parameter->to, precision);
  return ss.str();
}

template<>
inline string Value<ofxCortex::core::types::Image>::_getFormattedString()
{
  stringstream ss;
  ss << parameter->path;
  string original = ss.str();
  
  int length = MIN(28, original.size());
  int index = CLAMP(original.size() - length - 0, 0, original.size() - length);
  string substring = original.substr(index, length);
  
  stringstream output;
  if (original.size() > 28) output << "...";
  output << substring;
  return output.str();
}

}}
