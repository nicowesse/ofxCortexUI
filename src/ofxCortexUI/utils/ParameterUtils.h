#pragma once


#include "ofParameterGroup.h"
#include "ofxCortex/types/Parameter.h"
#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class ParameterUtils {
public:
  
  static std::vector<std::shared_ptr<ofxCortex::ui::View> > createViewsForParameterGroup(ofParameterGroup & params, bool includeGroupName = true, int addSpaces = 0);
  static std::vector<shared_ptr<ofxCortex::ui::View> > createViewsFromParameter(ofAbstractParameter & param);
  
  static std::vector<std::shared_ptr<ofxCortex::ui::View> > createViewsForParameterGroup(Group & params, bool includeGroupName = true, int addSpaces = 0);
  static std::vector<shared_ptr<ofxCortex::ui::View> > createViewsFromParameter(AbstractParameter & param);
  
  
private:
  ParameterUtils() = default;
  
};

}}
