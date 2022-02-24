#pragma once


#include "ofParameterGroup.h"
#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

static ofEventListeners listeners;

class ParameterUtils {
public:
  
  template<typename T>
  static ofParameter<T> addParameter(const std::string & name, T value, T min, T max, ofParameterGroup & parameters)
  {
    ofParameter<T> param(name, value, min, max);
    parameters.add(param);
    return param;
  }
  
  template<typename T, typename F>
  static ofParameter<T> addParameterWithEvent(const std::string & name, T value, T min, T max, ofParameterGroup & parameters, F&& lambda)
  {
    ofParameter<T> param(name, value, min, max);
    parameters.add(param);
    
    ofxCortex::ui::listeners.push(param.newListener(lambda));
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
