#pragma once

#include "ofxCortex/types/Select.h"

namespace ofxCortex::ui::components {

template<typename T>
inline bool ParameterDropdown(ofParameter<T> & parameter)
{
  auto ref = parameter.get();
  auto itemStrings = ref.getItemStrings();
  
  // Get the current selected index
  int selectedIndex = ref.getSelectedIndex();
  bool didChange = false;
  
  ImVec2 pos = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    if (ImGui::Button(parameter.getName().c_str()))
    {
      ref.setSelectedIndex(0);
      parameter.set(ref);
    }; ImGui::SameLine();
    
    float width = ImGui::GetContentRegionAvail().x;
    ImGui::PushItemWidth(-1);
    
    if (ImGui::Combo("##Dropdown", &selectedIndex, itemStrings.data(), (int)ref.size())) {
      ref.setSelectedIndex(selectedIndex);
      parameter.set(ref);
      
      didChange = true;
    }
    ImGui::PopItemWidth();
  }
  ImGui::PopID();
  
  if (ofxCortex::ui::linkedParameters.find(ofxCortex::core::utils::Parameters::hash(parameter)) != ofxCortex::ui::linkedParameters.end())
  {
    core::DrawLinkBorder(pos, availableWidth, frameHeight);
  }
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

}
