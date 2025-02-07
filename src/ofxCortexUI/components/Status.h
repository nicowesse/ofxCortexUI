#pragma once

#include "ofxCortex/types/Status.h"
#include "ofxCortexUI/core/BaseComponents.h"

namespace ofxCortex::ui::components {

inline void DrawStatus(const ImVec2 & position, float width, float height, const ofxCortex::types::Status & status)
{
  using Status = ofxCortex::types::Status;
  
  const ImGuiStyle& style = ImGui::GetStyle();
  auto * renderer = ImGui::GetWindowDrawList();
  
  const float inset = 5;
  
  ImVec4 color = ImVec4(ofColor::white, 0.75f);
  switch (status)
  {
    case Status::OFF:
    case Status::DISCONNECTED:
      color = ImVec4(ofColor::red, 1.00f);
      break;
    case Status::ON:
    case Status::CONNECTED:
      color = ImVec4(ofColor::springGreen, 1.00f);
      break;
    case Status::CONNECTING:
      color = ImVec4(ofColor::tomato, 1.00f);
      break;
    case Status::RECEIVING:
    case Status::SENDING:
      color = ImVec4(ofColor::green, 1.00f);
      break;
  }
  
  float time = ofGetElapsedTimef();
  
  float radiusMultiplier = 1.0f;
  const float frequency = 1.0f;
  
  switch (status)
  {
    case Status::RECEIVING:
    case Status::SENDING:
      radiusMultiplier = 1.0f + sin(time * frequency * TWO_PI) * 0.2;
      break;
  }
  
  float offsetX = status == Status::CONNECTING ? (sin(time * frequency * TWO_PI) * 0.5 + 0.5) * -12 : 0.0f;
  
  bool shouldDrawBase = status == Status::CONNECTING || status == Status::RECEIVING;
  if (shouldDrawBase)
  {
    ImVec4 baseColor = status == Status::RECEIVING ? ImVec4(ofColor::springGreen, 0.20f) : ImVec4(ofColor::white, 0.10f);
    float baseRadius = inset * 0.5 + 2;
    if (status == Status::RECEIVING) baseRadius += (sin(time * frequency * TWO_PI) * 0.5 + 0.5) * 2.0;
    
    renderer->AddCircleFilled(ImVec2(position.x + width - style.FramePadding.x - inset, position.y + height * 0.5), baseRadius, ImGui::ColorConvertFloat4ToU32(baseColor));
    
  }
  
  renderer->AddCircleFilled(ImVec2(position.x + width - style.FramePadding.x - inset + offsetX, position.y + height * 0.5), inset * 0.5 * radiusMultiplier, ImGui::ColorConvertFloat4ToU32(color));
}

inline void ParameterStatus(ofParameter<ofxCortex::types::Status> & parameter)
{
  const ofxCortex::types::Status & ref = parameter.get();
  
  ImVec2 pos = ImGui::GetCursorScreenPos();
  float availableWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0, 0.5));
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::Button(parameter.getName().c_str(), ImVec2(-FLT_MIN, 0));
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
    
//    core::DrawOverlay(parameter.getName().c_str(), NULL, pos, availableWidth, frameHeight);
    
    DrawStatus(pos, availableWidth, frameHeight, ref);
  }
  ImGui::PopID();
}

}
