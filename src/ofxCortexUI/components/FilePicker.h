#pragma once

#include "ofxCortex/types/File.h"

namespace ofxCortex::ui::components {

inline bool ParameterFilePicker(ofParameter<ofxCortex::core::types::File> & parameter)
{
  auto ref = parameter.get();
  bool didChange = false;
  
  ImVec2 pos = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
    ImGui::Button(parameter.getName().c_str());
    ImGui::PopStyleVar();
    
    ImGui::SameLine();
    
//    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(1.0f, 0.5f));
    if (ImGui::Button(ref.path.c_str(), ImVec2(-FLT_MIN, 0.0f)))
    {
      auto result = ofSystemLoadDialog();
      if (result.bSuccess)
      {
        ref = result.filePath;
        parameter.set(ref);
      }
    }
//    ImGui::PopStyleVar();
  }
  ImGui::PopID();
  
//  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

}
