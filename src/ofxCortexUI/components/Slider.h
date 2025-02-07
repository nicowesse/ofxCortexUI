#pragma once

#include "imgui_internal.h"
#include "ofxCortex/types/Range.h"
#include "ofxCortexUI/components/Button.h"
#include "ofxCortexUI/core/RangeSlider.h"

namespace ofxCortex::ui::components {

inline bool IsAnyKeyPressed()
{
  for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
    if (ImGui::IsKeyPressed((ImGuiKey) key)) { return true; }
  }
  return false;
}

template<typename T>
inline bool Slider(const char* label, T * value, T v_min, T v_max, float step = 1.0f, bool showLabel = true, bool showValue = true)
{
  const ImGuiStyle& style = ImGui::GetStyle();
  auto* renderer = ImGui::GetWindowDrawList();
  
  // Save cursor position for the slider
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  float frameWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  step = std::max(0.001f, step);
  
  bool isActive = false;
  // Use a unique ID for this slider
  ImGui::PushID(label);
  {
    // Draw the slider
    ImGui::SetNextItemWidth(frameWidth);
    
    std::string buttonID = "##invisible_slider_" + std::string(label);
    ImGui::InvisibleButton(buttonID.c_str(), ImVec2(std::max(frameWidth, 1.0f), frameHeight));
    
    isActive = ImGui::IsItemActive();
    
    // Check if the slider is active (being dragged)
    if (isActive)
    {
      float mouseX = ImGui::GetIO().MousePos.x;
      
      *value = std::round(ofMap(mouseX, cursorPos.x + style.FramePadding.x, cursorPos.x + frameWidth - style.FramePadding.x, v_min, v_max, true) / step) * step;;
    }
    
    // Arrow key control
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsItemFocused() && IsAnyKeyPressed())
    {
      bool shiftPressed = (io.KeyMods & ImGuiMod_Shift);
      bool commandPressed = (io.KeyMods & ImGuiMod_Super);
      bool controlPressed = (io.KeyMods & ImGuiMod_Ctrl);
      bool altPressed = (io.KeyMods & ImGuiMod_Alt);
      
      float delta = 1.0f * (commandPressed ? 0.1f : 1.0f) * (controlPressed ? 0.1f : 1.0f) * (altPressed ? 0.1f : 1.0f);
      
      isActive = true;
      
      if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && !shiftPressed) { *value -= delta; }
      if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && shiftPressed) { *value = std::ceil(*value - 1.00001f); }
      else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) && !shiftPressed) { *value += delta; }
      else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) && shiftPressed) { *value = std::floor(*value + 1.00001f); }
      else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) { *value -= 1.0f / delta; }
      else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) { *value += 1.0f / delta; }
    }
    
    float handleX = ofMap(*value, v_min, v_max, cursorPos.x + style.FramePadding.x, cursorPos.x + frameWidth - style.FramePadding.x, true);
    ImVec2 handlePos = ImVec2(handleX, cursorPos.y + frameHeight * 0.5f);
    
    core::DrawBackground(cursorPos, frameWidth, frameHeight);
    
    ImU32 grabColor = isActive ? ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_SliderGrabActive]) : ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_SliderGrab]);
    
    // Slider Line
    renderer->AddRectFilled(ImVec2(cursorPos.x + style.FramePadding.x, cursorPos.y + frameHeight * 0.5f - 1.0f),
                            ImVec2(cursorPos.x + frameWidth - style.FramePadding.x, cursorPos.y + frameHeight * 0.5f + 1.0f),
                            IM_COL32(255, 255, 255, 32),
                            style.FrameRounding);
    
    renderer->AddCircleFilled(handlePos, 3.5f, grabColor);  // Red dot
    
    if (v_min < 0.0f)
    {
      float middleX = ImLerp(cursorPos.x + style.FramePadding.x, cursorPos.x + frameWidth - style.FramePadding.x, 0.5f);
      ImVec2 middlePos = ImVec2(middleX, cursorPos.y + frameHeight * 0.5f);
      renderer->AddCircleFilled(middlePos, 2.5f, IM_COL32(255, 255, 255, 64));
    }
    
    core::DrawOverlay(showLabel ? label : nullptr, showValue ? ofToString(*value, 3).c_str() : nullptr, cursorPos, frameWidth, frameHeight);
  }
  ImGui::PopID();
  
  return isActive;
}

template<typename T>
inline bool ParameterSlider(ofParameter<T> & parameter, float step = 0.001f)
{
  bool didChange = false;
  T ref = parameter.get();
  
  ImVec2 pos = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImGui::PushItemWidth(availableWidth);
    if (Slider<T>(parameter.getName().c_str(), &ref, parameter.getMin(), parameter.getMax(), step)) {
      parameter.set(ref);
      didChange |= true;
    }
    ImGui::PopItemWidth();
  }
  ImGui::PopID();
  
  if (ofxCortex::ui::linkedParameters.find(ofxCortex::core::utils::Parameters::hash(parameter)) != ofxCortex::ui::linkedParameters.end())
  {
    core::DrawLinkBorder(pos, availableWidth, frameHeight);
  }
  
  if (ImGui::IsItemActive() || didChange)
  {
    ofxCortex::ui::focusedParameter = parameter.newReference();
  }
  
  return didChange;
}

inline bool ParameterSlider(ofParameter<float> & parameter)
{
  return ParameterSlider<float>(parameter);
}

inline bool ParameterSlider(ofParameter<int> & parameter)
{
  return ParameterSlider<int>(parameter, 1.0f);
}

inline bool ParameterSlider(ofParameter<glm::vec2> & parameter)
{
  bool didChange = false;
  auto ref = parameter.get();
  
  float aW = ImGui::GetContentRegionAvail().x;
  float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImVec2 btnSize = { 4.0f, frameHeight };
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    core::DrawBackground(ImGui::GetCursorScreenPos(), aW, frameHeight);
    core::DrawOverlay(parameter.getName().c_str(), ofToString(ref, 1).c_str(), ImGui::GetCursorScreenPos(), aW, frameHeight);
    ImGui::Dummy(ImVec2(aW, frameHeight));
    
    float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;
    
    float indent = 0.0f;
    
    // X
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.15f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
      if (components::Button("", btnSize, -1.0f, ImDrawFlags_RoundCornersAll)) {
        ref.x = 0.0f;
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopStyleColor(3);
      
      ImGui::SameLine();
      
      ImGui::PushItemWidth(aW - btnSize.x - spacingX - indent);
      //    if (ImGui::SliderFloat("##X", &ref.x, parameter.getMin().x, parameter.getMax().x, "%.3f")) {
      //      parameter.set(ref);
      //      didChange |= true;
      //    }
      if (Slider("X", &ref.x, parameter.getMin().x, parameter.getMax().x))
      {
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopItemWidth();
    }
    
    // Y
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
      if (components::Button("", btnSize, -1.0f, ImDrawFlags_RoundCornersAll)) {
        ref.y = 0.0f;
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopStyleColor(3);
      
      ImGui::SameLine();
      
      ImGui::PushItemWidth(aW - btnSize.x - spacingX - indent);
      //    if (ImGui::SliderFloat("##Y", &ref.y, parameter.getMin().y, parameter.getMax().y)) {
      //      parameter.set(ref);
      //      didChange |= true;
      //    }
      if (Slider("Y", &ref.y, parameter.getMin().y, parameter.getMax().y))
      {
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopItemWidth();
    }
  }
  ImGui::PopID();
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

inline bool ParameterSlider(ofParameter<glm::vec3> & parameter)
{
  bool didChange = false;
  auto ref = parameter.get();
  
  float aW = ImGui::GetContentRegionAvail().x;
  float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImVec2 btnSize = { 4.0f, frameHeight };
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    core::DrawBackground(ImGui::GetCursorScreenPos(), aW, frameHeight);
    core::DrawOverlay(parameter.getName().c_str(), ofToString(ref, 1).c_str(), ImGui::GetCursorScreenPos(), aW, frameHeight);
    ImGui::Dummy(ImVec2(aW, frameHeight));
    
    float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;
    
    float indent = 0.0f;
    
    // X
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.15f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
      if (components::Button("", btnSize, -1.0f, ImDrawFlags_RoundCornersAll)) {
        ref.x = 0.0f;
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopStyleColor(3);
      
      ImGui::SameLine();
      
      ImGui::PushItemWidth(aW - btnSize.x - spacingX - indent);
      //    if (ImGui::SliderFloat("##X", &ref.x, parameter.getMin().x, parameter.getMax().x, "%.3f")) {
      //      parameter.set(ref);
      //      didChange |= true;
      //    }
      if (Slider("X", &ref.x, parameter.getMin().x, parameter.getMax().x, 0.001f))
      {
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopItemWidth();
    }
    
    // Y
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
      if (components::Button("", btnSize, -1.0f, ImDrawFlags_RoundCornersAll)) {
        ref.y = 0.0f;
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopStyleColor(3);
      
      ImGui::SameLine();
      
      ImGui::PushItemWidth(aW - btnSize.x - spacingX - indent);
      //    if (ImGui::SliderFloat("##Y", &ref.y, parameter.getMin().y, parameter.getMax().y)) {
      //      parameter.set(ref);
      //      didChange |= true;
      //    }
      if (Slider("Y", &ref.y, parameter.getMin().y, parameter.getMax().y, 0.001f))
      {
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopItemWidth();
    }
    // Z
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
      if (components::Button("", btnSize, -1.0f, ImDrawFlags_RoundCornersAll)) {
        ref.z = parameter.getInit().z;
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopStyleColor(3);
      
      ImGui::SameLine();
      
      ImGui::PushItemWidth(aW - btnSize.x - spacingX - indent);
      //    if (ImGui::SliderFloat("##Z", &ref.z, parameter.getMin().z, parameter.getMax().z)) {
      //      parameter.set(ref);
      //      didChange = true;
      //    }
      if (Slider("Z", &ref.z, parameter.getMin().z, parameter.getMax().z, 0.001f))
      {
        parameter.set(ref);
        didChange |= true;
      }
      ImGui::PopItemWidth();
      
      ImVec2 currentCursor = ImGui::GetCursorScreenPos();
      core::DrawOverlay("X", ofToString(ref.z).c_str(), currentCursor, aW - btnSize.x - spacingX, frameHeight);
    }
  }
  ImGui::PopID();
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

inline bool ParameterSlider(ofParameter<ofxCortex::core::types::Range> & parameter)
{
  bool didChange = false;
  ofxCortex::core::types::Range ref = parameter.get();
  
  ImVec2 pos = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImGui::PushItemWidth(availableWidth);
    if (core::RangeSlider(parameter.getName().c_str(), &ref.from, &ref.to, ref.min, ref.max, "%.3f <-> %.3f")) {
      parameter.set(ref);
      didChange |= true;
    }
    ImGui::PopItemWidth();
  }
  ImGui::PopID();
  
  if (ofxCortex::ui::linkedParameters.find(ofxCortex::core::utils::Parameters::hash(parameter)) != ofxCortex::ui::linkedParameters.end())
  {
    core::DrawLinkBorder(pos, availableWidth, frameHeight);
  }
  
  if (ImGui::IsItemActive() || didChange)
  {
    ofxCortex::ui::focusedParameter = parameter.newReference();
  }
  
  return didChange;
}

}
