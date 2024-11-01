#pragma once

#include "ofxImGui.h"
#include "ofParameter.h"
#include <unordered_set>

namespace ofxCortex::ui {

inline static std::shared_ptr<ofAbstractParameter> focusedParameter = nullptr;
inline static std::unordered_set<std::string> linkedParameters;

}

#include "ofxCortexUI/themes/DarkTheme.h"

#include "ofxCortexUI/core/BaseComponents.h"

#include "ofxCortexUI/components/Slider.h"
#include "ofxCortexUI/components/Button.h"
#include "ofxCortexUI/components/Checkbox.h"
#include "ofxCortexUI/components/Dropdown.h"
#include "ofxCortexUI/components/Color.h"
#include "ofxCortexUI/components/Text.h"

#include "ofxCortexUI/utils/Helpers.h"
