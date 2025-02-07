#pragma once

namespace ofxCortex::ui::core {

inline void DrawBackground(const ImVec2 & position, float width, float height)
{
  const ImGuiStyle& style = ImGui::GetStyle();
  auto * renderer = ImGui::GetWindowDrawList();
  
  renderer->AddRectFilled(position, ImVec2(position.x + width, position.y + height), ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBg]), style.FrameRounding);
  if (style.FrameBorderSize > 0.0f) renderer->AddRect(position, ImVec2(position.x + width, position.y + height), ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Border]), style.FrameRounding, ImDrawFlags_RoundCornersAll, style.FrameBorderSize);  // Gray tracking
}

inline void DrawOverlay(const char* label, const char* value, const ImVec2 & position, float width, float height)
{
  const ImGuiStyle& style = ImGui::GetStyle();
  auto * renderer = ImGui::GetWindowDrawList();
  
  if (label != nullptr)
  {
    ImVec2 labelTextSize = ImGui::CalcTextSize(label);
    renderer->AddText(ImVec2(position.x + style.FramePadding.x, position.y + (height - labelTextSize.y) * 0.5f), ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]), label);
  }
  
  if (value != nullptr)
  {
    ImVec2 valueTextSize = ImGui::CalcTextSize(value);
    renderer->AddText(ImVec2(position.x + width - style.FramePadding.x - valueTextSize.x, position.y + (height - valueTextSize.y) * 0.5f), ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]), value);
  }
}

inline void DrawLinkBorder(const ImVec2 & position, float width, float height)
{
  const ImGuiStyle& style = ImGui::GetStyle();
  auto * renderer = ImGui::GetWindowDrawList();
  
  const float inset = 5;
  
//  renderer->AddRect(position, ImVec2(position.x + width, position.y + height), ImGui::ColorConvertFloat4ToU32(ImVec4(ofColor::springGreen, 0.50f)), style.FrameRounding, ImDrawFlags_RoundCornersAll, style.FrameBorderSize);  // Gray tracking
  
  renderer->AddCircleFilled(ImVec2(position.x + width - inset, position.y + inset), inset * 0.5, ImGui::ColorConvertFloat4ToU32(ImVec4(ofColor::springGreen, 0.75f)));
}

}
