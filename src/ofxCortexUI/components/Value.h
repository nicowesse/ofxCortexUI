#pragma once

#include "ofxCortexUI/core/ParameterView.h"
#include "ofxCortexUI/components/Label.h"
#include "ofxCortex/types/Range.h"
#include "ofxCortex/types/Image.h"
#include "ofxCortex/types/Parameter.h"

namespace ofxCortex { namespace ui {

template<typename T>
class Value : public ofxCortex::ui::ParameterView<T> {
public:
  
  Value(const std::string & name, T value)
  : ParameterView<T>()
  {
//    setName(name);
    
    ofParameter<T> param { name, value };
    ParameterView<T>::parameter = param.newReference();
    
    _init();
    _adjustLayout();
  }
  static shared_ptr<Value<T>> create(string name, T value) { return make_shared<Value<T>>(name, value); }
  
  Value(ofAbstractParameter && param)
  : ParameterView<T>(std::move(param))
  {
    _init();
    _adjustLayout();
  }
  static shared_ptr<Value<T>> create(ofAbstractParameter && param) { return make_shared<Value<T>>(std::move(param)); }
  
  Value(ofParameter<T> & param)
  : ParameterView<T>(param)
  {
    _init();
    _adjustLayout();
  }
  static shared_ptr<Value<T>> create(ofParameter<T> & param) { return make_shared<Value<T>>(param); }
  
  
  void drawValue(ofFloatColor color)
  {
    const auto & rect = View::getRenderRect();
    
    if (!View::style->getValueFont()->isLoaded())
    {
      ofLogWarning(DisplayObject::_getLogModule()) << "Value font is not loaded. Please make sure the paths are correct. Returning.."; return;
    }
    
    string value = _getFormattedString();
    float valueWidth = View::style->getValueFont()->stringWidth(value);
    float x = rect.getRight() - 12 - valueWidth;
    float y = rect.getCenter().y + (View::style->getFontXHeight() * 0.5);
    
    ofPushStyle();
    {
      ofSetColor(color);
      View::style->getValueFont()->drawString(value, x, y);
    }
    ofPopStyle();
  }
  
  void drawValue()
  {
    this->drawValue(View::style->valueFontColor);
  }
  
protected:
  virtual string _getModule() const override { return "Value"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("Value::Background");
    background->disableEvents();
    
    label = ui::Label::create("Value");
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
  
  std::string _getFormattedString()
  {
    stringstream ss;
    ss << ParameterView<T>::getParameter().toString();
    return ofToString(ss.str(), 1);
  }
  
  shared_ptr<ui::Background> background;
  shared_ptr<ui::Label> label;
};

template<>
inline string Value<float>::_getFormattedString()
{
  int precision = 3;
  stringstream ss;
  ss << ofToString(getValue(), precision);
  return ss.str();
}

template<>
inline string Value<glm::vec2>::_getFormattedString()
{
  int precision = 3;
  stringstream ss;
  ss << ofToString(getParameter()->x, precision) << ", " << ofToString(getParameter()->y, precision);
  return ss.str();
}

template<>
inline string Value<glm::vec3>::_getFormattedString()
{
  int precision = 3;
  stringstream ss;
  ss << ofToString(getParameter()->x, precision) << ", " << ofToString(getParameter()->y, precision) << ", " << ofToString(getParameter()->z, precision);
  return ss.str();
}

template<>
inline string Value<ofxCortex::core::types::Range>::_getFormattedString()
{
  int precision = 2;
  stringstream ss;
  ss << ofToString(getParameter()->from, precision) << " ←→ " << ofToString(getParameter()->to, precision);
  return ss.str();
}

template<>
inline string Value<ofxCortex::core::types::Image>::_getFormattedString()
{
  stringstream ss;
  ss << getParameter()->path;
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
