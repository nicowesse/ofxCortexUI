#pragma once

namespace ofxCortex::ui::components {

inline bool ParameterColor(ofParameter<ofFloatColor> & parameter)
{
  const auto & style = ImGui::GetStyle();
  auto ref = parameter.get();
  bool didChange = false;
  
  float availableWidth = ImGui::GetContentRegionAvail().x;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    float buttonWidth = ImGui::CalcTextSize(parameter.getName().c_str()).x + ImGui::GetStyle().FramePadding.x * 2;
    if (ImGui::Button(parameter.getName().c_str(), ImVec2(buttonWidth, 0))) {
      parameter.set(ofFloatColor::white);
        }
    
    ImGui::SameLine();
    
    ImVec4 color = ImVec4(ref.r, ref.g, ref.b, ref.a);
    if (ImGui::ColorButton("##colorButton", color, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoTooltip, ImVec2(availableWidth - buttonWidth - style.ItemSpacing.x, 0))) {
      ImGui::OpenPopup("ColorPicker");
    }
    
    ImGui::PushItemWidth(availableWidth);
    if (ImGui::BeginPopup("ColorPicker")) {
//      ImGui::PushItemWidth(availableWidth * 0.8);
      if (ImGui::ColorPicker4("##picker", (float*)&color, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview)) {
        parameter.set(ofFloatColor(color.x, color.y, color.z, color.w));
      }
      ImGui::EndPopup();
    }
    ImGui::PopItemWidth();
  }
  ImGui::PopID();
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

}
