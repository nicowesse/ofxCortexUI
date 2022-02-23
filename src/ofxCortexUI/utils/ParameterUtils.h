#pragma once


#include "ofParameterGroup.h"
#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class ParameterUtils {
public:
  
  template<typename T>
  static ofParameter<T> addParameter(const std::string & name, T value, T min, T max, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name, value, min, max);
    parameters.add(param);
    return param;
  }
  
  template<typename T>

  static ofParameter<T> addParameter(const std::string & name, T value, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name, value);
    parameters.add(param);
    return param;
  }
  
  template<typename T>
  static ofParameter<T> addParameter(const std::string & name, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name);
    parameters.add(param);
    return param;
  }
  
  static std::vector<std::shared_ptr<ofxCortex::ui::View> > createViewsForParameterGroup(ofParameterGroup & params, int addSpaces = 0);
  static std::vector<shared_ptr<ofxCortex::ui::View> > createViewsFromParameter(ofAbstractParameter & param);
  
private:
  ParameterUtils() = default;
  
};

}}
