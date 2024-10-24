#pragma once

namespace ofxCortex::ui::components {

inline bool Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0), float rounding = -1.0f, ImDrawFlags rounding_flags = ImDrawFlags_RoundCornersAll)
{
  // Get the current window draw list
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImGuiStyle& style = ImGui::GetStyle();
  
  // Calculate button size based on provided size argument
  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImVec2 size = size_arg;
  if (size.x == 0.0f) size.x = ImGui::CalcItemWidth();  // Use available width if none specified
  if (size.y == 0.0f) size.y = ImGui::GetFrameHeight(); // Use default button height if none specified
  rounding = (rounding == -1.0f) ? style.FrameRounding : rounding;
  
  // Button behavior (hover/active states)
  ImGui::InvisibleButton(label, size);
  bool hovered = ImGui::IsItemHovered();
  bool active = ImGui::IsItemActive();
  bool pressed = ImGui::IsItemClicked();
  
  // Determine colors from the current style
  ImU32 bg_color;
  ImU32 border_color = ImGui::GetColorU32(style.Colors[ImGuiCol_Border]);
  
  if (active) { bg_color = ImGui::GetColorU32(style.Colors[ImGuiCol_ButtonActive]); }
  else if (hovered) { bg_color = ImGui::GetColorU32(style.Colors[ImGuiCol_ButtonHovered]); }
  else { bg_color = ImGui::GetColorU32(style.Colors[ImGuiCol_Button]); }
  
  // Draw the button's background
  draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color, rounding, rounding_flags);
  
  // Draw the button's border
  if (style.FrameBorderSize > 0.0f) draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), border_color, rounding, rounding_flags, style.FrameBorderSize);
  
  // Draw button label
  ImVec2 text_size = ImGui::CalcTextSize(label);
  ImVec2 text_pos = ImVec2(pos.x + (size.x - text_size.x) * 0.5f, pos.y + (size.y - text_size.y) * 0.5f); // Center text
  draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), label);
  
  return pressed;
}

inline bool ParameterButton(ofParameter<void> & parameter)
{
  bool didChange = false;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImGui::Button(parameter.getName().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight()));
    if (ImGui::IsItemClicked())
    {
      parameter.trigger();
      didChange = true;
    }
  }
  ImGui::PopID();
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

}
