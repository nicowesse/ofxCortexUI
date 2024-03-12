#pragma once


#include "ofParameterGroup.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/SliderView.h"
#include "ofxCortexUI/components/ButtonView.h"
#include "ofxCortexUI/components/CheckboxView.h"
#include "ofxCortexUI/components/ValueView.h"

namespace ofxCortex { namespace ui {

class ParameterUtils {
public:
  
  static std::vector<std::shared_ptr<ofxCortex::ui::View> > createViewsForParameterGroup(ofParameterGroup & params, bool includeGroupName = true, int addSpaces = 0);
  static std::vector<std::shared_ptr<ofxCortex::ui::View> > createViewsFromParameter(ofAbstractParameter & param);
  
  
private:
  ParameterUtils() = default;
  
};

}}
