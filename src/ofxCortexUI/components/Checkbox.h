#pragma once

namespace ofxCortex::ui::components {

inline bool Checkbox(const char* label, bool* value)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  bool didChange = false;
  
  ImVec2 cursorPosition = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  const ImVec2 & framePadding = ImGui::GetStyle().FramePadding;
  
  //  ImGui::PushID(ImGui::GetUniqueID(label));
  
  if (ImGui::InvisibleButton("##Button", ImVec2(availableWidth, frameHeight)))
  {
    *value = !*value;
  }
  
  core::DrawBackground(cursorPosition, availableWidth, frameHeight);
  
  ImVec2 statusPos = ImVec2(cursorPosition.x + availableWidth - framePadding.x - 2.5f, cursorPosition.y + frameHeight * 0.5);
  
  draw_list->AddCircle(statusPos, 5.0f, IM_COL32(255, 255, 255, 255));
  draw_list->AddCircleFilled(statusPos, 2.0f, IM_COL32(255, 255, 255, 255 * (*value)));
  
  core::DrawOverlay(label, NULL, cursorPosition, availableWidth, frameHeight);
  
  //  ImGui::PopID();
  
  return didChange;
}

inline bool ParameterCheckbox(ofParameter<bool> & parameter)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  
  bool didChange = false;
  bool ref = parameter.get();
  
  ImVec2 cursorPosition = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  const ImVec2 & framePadding = ImGui::GetStyle().FramePadding;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    if (ImGui::InvisibleButton("##Button", ImVec2(availableWidth, frameHeight)))
    {
      parameter.set(!ref);
      didChange = true;
    }
    
    core::DrawBackground(cursorPosition, availableWidth, frameHeight);
    
    ImVec2 statusPos = ImVec2(cursorPosition.x + availableWidth - framePadding.x - 2.5f, cursorPosition.y + frameHeight * 0.5);
    draw_list->AddCircle(statusPos, 5.0f, IM_COL32(255, 255, 255, 255));
    draw_list->AddCircleFilled(statusPos, 2.0f, IM_COL32(255, 255, 255, 255 * ref));
    
    
    core::DrawOverlay(parameter.getName().c_str(), NULL, cursorPosition, availableWidth, frameHeight);
  }
  ImGui::PopID();
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

}
