#pragma once

#include "ofUtils.h"
#include "ofxCortexUI/core/BaseComponents.h"
#include "ofxCortex/types/Status.h"

namespace ofxCortex::ui::components {

template<typename T>
inline void DrawListItem(const T & item)
{
  ImGui::Button(ofToString(item).c_str(), ImVec2(-FLT_MIN, 0.0f));
}

template<typename T>
inline bool ParameterList(ofParameter<std::vector<T>> & parameter)
{
  bool didChange = false;
  auto ref = parameter.get();
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    float width = ImGui::GetContentRegionAvail().x;
    ImGui::PushItemWidth(width);
    if (!ref.empty() && ImGui::CollapsingHeader(parameter.getName().c_str()))
    {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      for (auto & item : ref)
      {
        DrawListItem(item);
      }
      ImGui::PopItemFlag();
    }
    ImGui::PopItemWidth();
  }
  ImGui::PopID();
  
  return didChange;
}

}
