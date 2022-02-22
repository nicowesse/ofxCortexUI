#pragma once

#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class ParameterUtils {
public:
  static shared_ptr<View> createViewFromParameter(ofAbstractParameter & param){};
  
  template<typename T>
  static ofParameter<T> addParameter(const string & name, T value, T min, T max, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name, value, min, max);
    parameters.add(param);
    return param;
  }
  
  template<typename T>
  static ofParameter<T> addParameter(const string & name, T value, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name, value);
    parameters.add(param);
    return param;
  }
  
  template<typename T>
  static ofParameter<T> addParameter(const string & name, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name);
    parameters.add(param);
    return param;
  }
    
protected:
};

}}
