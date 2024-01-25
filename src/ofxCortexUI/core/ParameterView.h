#pragma once

#include "View.h"
#include "ofxCortex/types/Parameter.h"
#include "ofxCortex/types/Range.h"

namespace ofxCortex { namespace ui {

template<typename T>
class ParameterView : public View {
public:
  ParameterView(const std::string & name) : View(name) {
    setName(name);
  };
  ParameterView(const ofAbstractParameter & param)
  : View(param.getName())
  {
    parameter = param.newReference();
    View::setName(param.getName());
  }
  
  ofParameter<T> getParameter() {
    auto p = std::dynamic_pointer_cast<ofParameter<T>>(parameter);
    if (p) return *p;
    else {
      ofLogWarning(_getLogModule()) << "There is no set parameter. Returning a nullptr.";
      return ofParameter<T>();
    }
  }
  bool hasParameter() const { return parameter != nullptr; }
  
  std::string getParameterName() { return getParameter().getName(); }
  virtual void setName(const std::string & name) {
    DisplayObject::setName(name);
    getParameter().setName(name);
  };
  T getValue() { return getParameter().get(); }
  void setValue(T value) { getParameter().set(value); }
  
  T getMin() { return getParameter().getMin(); }
  T getMax() { return getParameter().getMax(); }
  
  std::string toString() { return getParameter().toString(); }
  void fromString(const std::string & str) { getParameter().fromString(str); }
  
  std::string getUnit() {
    auto p = std::dynamic_pointer_cast<UnitParameter<T>>(parameter);
    return (p) ? p->getUnit() : "";
  }
  
protected:
  std::shared_ptr<ofAbstractParameter> parameter;
};

//template<> 
//inline float ParameterView<ofxCortex::core::types::Range>::getMin() { return getParameter()->min; }
//
//template<>
//inline float ParameterView<ofxCortex::core::types::Range>::getMax() { return getParameter()->max; }

template<>
class ParameterView<void> : public View {
public:
  ParameterView() : View("ParameterView") {};
  ParameterView(const ofAbstractParameter & param)
  : View(param.getName())
  {
    parameter = param.newReference();
    View::setName(param.getName());
  }
  
protected:
  std::shared_ptr<ofAbstractParameter> parameter;
  
  ofParameter<void> getParameter() {
    auto p = std::dynamic_pointer_cast<ofParameter<void>>(parameter);
    if (p) return *p;
    else {
      ofLogWarning(_getLogModule()) << "There is no set parameter. Returning a nullptr.";
      return ofParameter<void>();
    }
  }
  bool hasParameter() const { return parameter != nullptr; }
  
  std::string getParameterName() { return getParameter().getName(); }
  void getValue() { }
  void setValue(void) { }
  
  void getMin() {}
  void getMax() {}
  
  std::string getUnit() {
    auto p = std::dynamic_pointer_cast<UnitParameter<void>>(parameter);
    return (p) ? p->getUnit() : "";
  }
};

}}
