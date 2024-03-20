#pragma once

#include "ofxCortexUI/core/View.h"

#include "ofxCortex/types/Range.h"
#include "ofxCortex/types/Image.h"
#include "ofxCortex/types/File.h"

#include "ofxCortex/utils/Helpers.h"

namespace ofxCortex { namespace ui {

template<typename T>
class ValueView : public ofxCortex::ui::ParameterView {
protected:
  ValueView(const ofAbstractParameter & param) : ParameterView(param) {}
  
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
    
    if (this->shouldDrawBackground) Styling::drawBackground(this->getFrame());
    
    ofSetColor(Styling::getForegroundColor());
    ofxCortex::ui::Styling::drawLabel(ParameterView::getParameterName(), b);
    
    ofSetColor(Styling::getAccentColor());
    ofxCortex::ui::Styling::drawValue(getFormattedString(), b);
  }
  
  std::string getFormattedString()
  {
    std::stringstream ss;
    ss << ParameterView::getParameterToString();
    return ofToString(ss.str(), 1);
  }
};

template<>
inline std::string ValueView<float>::getFormattedString()
{
  const auto & value = getParameterValue<float>();
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(value, precision);
  return ss.str();
}

template<>
inline std::string ValueView<glm::vec2>::getFormattedString()
{
  const auto & value = getParameterValue<glm::vec2>();
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(value.x, precision) << ", " << ofToString(value.y, precision);
  return ss.str();
}

template<>
inline std::string ValueView<glm::vec3>::getFormattedString()
{
  const auto & value = getParameterValue<glm::vec3>();
  int precision = 3;
  std::stringstream ss;
  ss << ofToString(value.x, precision) << ", " << ofToString(value.y, precision) << ", " << ofToString(value.z, precision);
  return ss.str();
}

template<>
inline std::string ValueView<ofFloatColor>::getFormattedString()
{
  const auto & value = getParameterValue<ofColor>();
  std::stringstream ss;
  ss << ofxCortex::core::utils::Color::colorToHex(value);
  return ss.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::Range>::getFormattedString()
{
  const auto & value = getParameterValue<ofxCortex::core::types::Range>();
  int precision = 2;
  std::stringstream ss;
  ss << ofToString(value.from, precision) << " ←→ " << ofToString(value.to, precision);
  return ss.str();
}

template<>
inline std::string ValueView<ofxCortex::core::types::Image>::getFormattedString()
{
  const auto & value = getParameterValue<ofxCortex::core::types::Image>();
  std::stringstream ss;
  ss << value.path;
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
  const auto & value = getParameterValue<ofxCortex::core::types::File>();
  std::stringstream ss;
  ss << value.path;
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
