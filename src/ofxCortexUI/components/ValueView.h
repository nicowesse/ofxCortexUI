#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortex/types/Range.h"
#include "ofxCortex/types/Image.h"
#include "ofxCortex/types/File.h"

namespace ofxCortex { namespace ui {

template<typename T>
class ValueView : public ofxCortex::ui::View {
protected:
  ValueView(std::string name, T value) : View(name)
  {
    parameter.setName(name);
    parameter.set(value);
  }
  
  ValueView(ofParameter<T> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  }
  
public:
  
  template<typename ... F>
  static std::shared_ptr<ValueView<T>> create(F&& ... f) {
    struct EnableMakeShared : public ValueView<T> { EnableMakeShared(F&&... arg) : ValueView<T>(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "ValueView"; };
  
  virtual void onDraw() override
  {
    const auto & b = this->getContentFrame();
    
    Styling::drawBackground(this->getFrame());
    
    ofSetColor(Styling::getForegroundColor());
    ofxCortex::ui::Styling::drawLabel(parameter.getName(), b);
    
    ofSetColor(Styling::getAccentColor());
    ofxCortex::ui::Styling::drawValue(_getFormattedString(), b);
  }
  
  std::string _getFormattedString()
  {
    std::stringstream ss;
    ss << parameter;
    return ofToString(ss.str(), 1);
  }
  
  ofParameter<T> parameter;

};

template<>
inline std::string ValueView<float>::_getFormattedString()
{
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(parameter.get(), precision);
  return ss.str();
}

template<>
inline std::string ValueView<glm::vec2>::_getFormattedString()
{
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(parameter->x, precision) << ", " << ofToString(parameter->y, precision);
  return ss.str();
}

template<>
inline std::string ValueView<glm::vec3>::_getFormattedString()
{
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(parameter->x, precision) << ", " << ofToString(parameter->y, precision) << ", " << ofToString(parameter->z, precision);
  return ss.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::Range>::_getFormattedString()
{
  int precision = 2;
  std::stringstream ss;
  ss << ofToString(parameter->from, precision) << " ←→ " << ofToString(parameter->to, precision);
  return ss.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::Image>::_getFormattedString()
{
  std::stringstream ss;
  ss << parameter->path;
  std::string original = ss.str();
  
  int length = MIN(28, original.size());
  int index = CLAMP(original.size() - length - 0, 0, original.size() - length);
  std::string substring = original.substr(index, length);
  
  std::stringstream output;
  if (original.size() > 28) output << "...";
  output << substring;
  return output.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::File>::_getFormattedString()
{
  std::stringstream ss;
  ss << parameter->path;
  std::string original = ss.str();
  
  int leftover = ceil(this->getWidth() / Styling::getValueFont().dimensions.width) - parameter.getName().size() + 8;
  
  int length = MIN(leftover, original.size());
  int index = CLAMP(original.size() - length - 0, 0, original.size() - length);
  std::string substring = original.substr(index, length);
  
  std::stringstream output;
  if (original.size() > leftover) output << "...";
  output << substring;
  return output.str();
}

}}
