#pragma once

#include "ofxCortexUI/core/View.h"

#include "ofxCortex/types/Range.h"
#include "ofxCortex/types/Image.h"
#include "ofxCortex/types/File.h"

namespace ofxCortex { namespace ui {

template<typename T>
class ValueView : public ofxCortex::ui::ParameterView<T> {
protected:
//  ValueView(std::string name, T value) : View(name)
//  {
//    parameter.setName(name);
//    parameter.set(value);
//  }
  
  ValueView(const ofAbstractParameter & param) : ParameterView<T>(param)
  {
//    parameter.makeReferenceTo(param);
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
  virtual std::string getComponentName() const override { return "ValueView"; };
  
  virtual void onDraw() override
  {
    const auto & b = this->getContentFrame();
    
    Styling::drawBackground(this->getFrame());
    
    ofSetColor(Styling::getForegroundColor());
    ofxCortex::ui::Styling::drawLabel(ParameterView<T>::getParameterName(), b);
    
    ofSetColor(Styling::getAccentColor());
    ofxCortex::ui::Styling::drawValue(getFormattedString(), b);
  }
  
  std::string getFormattedString()
  {
    std::stringstream ss;
    ss << ParameterView<T>::getParameterToString();
    return ofToString(ss.str(), 1);
  }
  
  ofParameter<T> parameter;

};

template<>
inline std::string ValueView<float>::getFormattedString()
{
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(getParameterValue(), precision);
  return ss.str();
}

template<>
inline std::string ValueView<glm::vec2>::getFormattedString()
{
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(getParameter()->x, precision) << ", " << ofToString(getParameter()->y, precision);
  return ss.str();
}

template<>
inline std::string ValueView<glm::vec3>::getFormattedString()
{
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(getParameter()->x, precision) << ", " << ofToString(getParameter()->y, precision) << ", " << ofToString(getParameter()->z, precision);
  return ss.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::Range>::getFormattedString()
{
  int precision = 2;
  std::stringstream ss;
  ss << ofToString(getParameter()->from, precision) << " ←→ " << ofToString(getParameter()->to, precision);
  return ss.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::Image>::getFormattedString()
{
  std::stringstream ss;
  ss << getParameter()->path;
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
inline std::string ValueView<ofxCortex::core::types::File>::getFormattedString()
{
  std::stringstream ss;
  ss << getParameter()->path;
  std::string original = ss.str();
  
  int leftover = ceil(this->getWidth() / Styling::getValueFont().dimensions.width) - getParameterName().size() + 8;
  
  int length = MIN(leftover, original.size());
  int index = CLAMP(original.size() - length - 0, 0, original.size() - length);
  std::string substring = original.substr(index, length);
  
  std::stringstream output;
  if (original.size() > leftover) output << "...";
  output << substring;
  return output.str();
}

}}
