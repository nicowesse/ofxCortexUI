#pragma once

#include "ofxCortex/utils/ParameterUtils.h"

namespace ofxCortex::ui::utils {

inline bool Group(ofParameterGroup & group);

inline bool DrawParameter(ofAbstractParameter & parameter)
{
  bool didChange = false;
  
  auto type = parameter.type();
  
  if      (type == typeid(ofParameterGroup).name()) { didChange |= Group(parameter.castGroup()); }
  else if (type == typeid(ofParameter<float>).name()) { didChange |= components::ParameterSlider(parameter.cast<float>()); }
  else if (type == typeid(ofParameter<int>).name()) { didChange |= components::ParameterSlider(parameter.cast<int>()); }
  else if (type == typeid(ofParameter<glm::vec3>).name()) { didChange |= components::ParameterSlider(parameter.cast<glm::vec3>()); }
  else if (type == typeid(ofParameter<glm::vec2>).name()) { didChange |= components::ParameterSlider(parameter.cast<glm::vec2>()); }
  else if (type == typeid(ofParameter<string>).name()) { didChange |= components::ParameterTextInput(parameter.cast<string>()); }
  else if (type == typeid(ofParameter<bool>).name()) { didChange |= components::ParameterCheckbox(parameter.cast<bool>()); }
  else if (type == typeid(ofParameter<Select<int>>).name()) { didChange |= components::ParameterDropdown(parameter.cast<Select<int>>()); }
  else if (type == typeid(ofParameter<ofFloatColor>).name()) { didChange |= components::ParameterColor(parameter.cast<ofFloatColor>()); }
  else if (type == typeid(ofParameter<void>).name()) { didChange |= components::ParameterButton(parameter.cast<void>()); }
  
  return didChange;
}

inline bool Group(ofParameterGroup & group)
{
  bool didChange = false;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(group).c_str());
  {
    int level = ofxCortex::core::utils::Parameters::getLevel(group);
    int flags = (level == 0) ? ImGuiTreeNodeFlags_DefaultOpen : 0;
    
    if (ImGui::CollapsingHeader(group.getName().c_str(), ImGuiTreeNodeFlags_SpanFullWidth | flags))
    {
      for (auto & param : group)
      {
        didChange |= DrawParameter(*param);
      }
    }
  }
  ImGui::PopID();
  
  return didChange;
}

}
