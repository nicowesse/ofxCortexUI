#pragma once

#include "imgui_internal.h" // for RenderArrow()
#include "ofxCortex/utils/ParameterUtils.h"
#include "ofxCortex/utils/ShapingUtils.h"
#include "ofxCortex/types/AllTypes.h"
#include "BaseComponents.h"

namespace ofxCortex::ui::utils {

inline std::vector<const char*> toArray(const std::vector<std::string>& input) {
    std::vector<const char*> output;
    output.reserve(input.size());
  
    for (const auto& str : input) {
      output.push_back(str.c_str());
    }

    return output;
}

inline bool Group(ofParameterGroup & group);

template<typename T>
inline bool ShapingFunction(ofParameter<T> & parameter);

inline bool PlotFunction(ofParameter<ofxCortex::core::types::Plot> & parameter);

inline void DrawOutputParameter(const ofParameter<ofxCortex::types::DisplayValue> & parameter)
{
  const auto & ref = parameter.get();
  
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
    
    core::DrawOverlay(NULL, ((std::string)ref).c_str(), pos, availableWidth, frameHeight);
  }
  ImGui::PopID();
}

inline bool DrawParameter(ofAbstractParameter & parameter)
{
  bool didChange = false;
  
  auto type = parameter.type();
  auto valueType = parameter.valueType();
  
  if      (type == typeid(ofParameterGroup).name()) { didChange |= Group(parameter.castGroup()); }
  else if (type == typeid(ofParameter<float>).name()) { didChange |= components::ParameterSlider(parameter.cast<float>()); }
  else if (type == typeid(ofParameter<int>).name()) { didChange |= components::ParameterSlider(parameter.cast<int>()); }
  else if (type == typeid(ofParameter<glm::vec3>).name()) { didChange |= components::ParameterSlider(parameter.cast<glm::vec3>()); }
  else if (type == typeid(ofParameter<glm::vec2>).name()) { didChange |= components::ParameterSlider(parameter.cast<glm::vec2>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::types::Range>).name()) { didChange |= components::ParameterSlider(parameter.cast<ofxCortex::core::types::Range>()); }
//  else if (type == typeid(ofParameter<ofxCortex::core::types::Range>).name()) { didChange |= components::ParameterSlider(parameter.cast<ofxCortex::core::types::Range>()); }
  else if (type == typeid(ofParameter<string>).name()) { didChange |= components::ParameterTextInput(parameter.cast<string>()); }
  else if (type == typeid(ofParameter<bool>).name()) { didChange |= components::ParameterCheckbox(parameter.cast<bool>()); }
  else if (type == typeid(ofParameter<ofxCortex::types::Status>).name()) { components::ParameterStatus(parameter.cast<ofxCortex::types::Status>()); }
  else if (type == typeid(ofParameter<std::vector<std::string>>).name()) { didChange |= components::ParameterList(parameter.cast<std::vector<std::string>>()); }
  
  else if (type == typeid(ofParameter<ofxCortex::core::types::Select<int>>).name()) { didChange |= components::ParameterDropdown(parameter.cast<ofxCortex::core::types::Select<int>>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::types::BeatDivision>).name()) { didChange |= components::ParameterDropdown(parameter.cast<ofxCortex::core::types::BeatDivision>()); }
  
  else if (type == typeid(ofParameter<ofFloatColor>).name()) { didChange |= components::ParameterColor(parameter.cast<ofFloatColor>()); }
  else if (type == typeid(ofParameter<void>).name()) { didChange |= components::ParameterButton(parameter.cast<void>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::types::Separator>).name()) { ImGui::SeparatorText(parameter.cast<ofxCortex::core::types::Separator>()->heading.c_str()); }
  else if (type == typeid(ofParameter<ofxCortex::types::DisplayValue>).name()) { DrawOutputParameter(parameter.cast<ofxCortex::types::DisplayValue>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::types::File>).name()) { components::ParameterFilePicker(parameter.cast<ofxCortex::core::types::File>()); }
  
  else if (type == typeid(ofParameter<ofxCortex::core::utils::LinearFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::LinearFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::InvertFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::InvertFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::GainFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::GainFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::PulseWidthFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::PulseWidthFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::ExponentialFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::ExponentialFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::BiasedGainFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::BiasedGainFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::ParabolaFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::ParabolaFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::ExpBlendFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::ExpBlendFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::ExpStepFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::ExpStepFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::SmoothstepFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::SmoothstepFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::SineFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::SineFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::SignedSineFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::SignedSineFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::PeriodicSineFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::PeriodicSineFunction>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::utils::TriangleFunction>).name()) { ShapingFunction(parameter.cast<ofxCortex::core::utils::TriangleFunction>()); }
  
  else if (type == typeid(ofParameter<ofxCortex::core::types::Plot>).name()) { PlotFunction(parameter.cast<ofxCortex::core::types::Plot>()); }
  
  return didChange;
}


inline bool Header(const char* label, bool defaultOpen = false)
{
  using namespace ImGui;
  ImGuiStyle& style = ImGui::GetStyle();
  auto* renderer = ImGui::GetWindowDrawList();
  
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  float frameWidth = ImGui::GetContentRegionAvail().x;
  float frameHeight = ImGui::GetFrameHeight();
  
  bool * isOpen = GetStateStorage()->GetBoolRef(GetID(label), defaultOpen);
  
  PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
  if (Button(label, ImVec2(-FLT_MIN, 0.0f))) *isOpen ^= 1;
  PopStyleVar();
  
  renderer->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + 2.0f, cursorPos.y + frameHeight), GetColorU32(ImGuiCol_Text, 0.5f + 0.35 * *isOpen), style.FrameRounding);
  
  //    ImVec2 arrow_pos = ImVec2(GetItemRectMax().x - style.FramePadding.x - GetFontSize(), GetItemRectMin().y + style.FramePadding.y);
  //    RenderArrow(GetWindowDrawList(), arrow_pos, GetColorU32(ImGuiCol_Text), *p_open ? ImGuiDir_Down : ImGuiDir_Right, 0.75);
  
  return *isOpen;
}

inline bool Group(ofParameterGroup & group)
{
  bool didChange = false;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(group).c_str());
  {
    int level = ofxCortex::core::utils::Parameters::getLevel(group);
    int flags = (level == 0) ? ImGuiTreeNodeFlags_DefaultOpen : 0;
    
    if (Header(group.getName().c_str()))
    {
      ImGui::Indent();
      for (auto & param : group)
      {
        didChange |= DrawParameter(*param);
      }
      ImGui::Unindent();
    }
    
    if (ImGui::BeginPopupContextItem("ParameterGroupPopup"))
    {
      if (ImGui::SmallButton("Delete"))
      {
        std::cout << "Delete group " << group.getName() << std::endl;
      }
      
      ImGui::EndPopup();
    }
    
    ImGui::OpenPopupOnItemClick("ParameterGroupPopup", ImGuiPopupFlags_MouseButtonRight);
  }
  ImGui::PopID();
  
  return didChange;
}

template<typename T>
inline bool ShapingFunction(ofParameter<T> & parameter)
{
  const T& ref = parameter.get();
  bool didChange = false;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    int resolution = ImGui::GetContentRegionAvail().x / 2.0;
    
    struct PlotContext {
      const T& func;
      const ofParameterGroup& params;
    };
    
    PlotContext context{ ref, ref.parameters};
    
    // Updated function to use the context
    std::function<float(int)> plotFunc = [&context, resolution](int i) {
      float x = static_cast<float>(i) / (resolution - 1);
      return context.func(x);
    };
    
    auto funcWrapper = [](void* data, int i) -> float {
      auto& func = *static_cast<std::function<float(int)>*>(data);
      return func(i);
    };
    
    ImGui::PushItemWidth(-FLT_MIN);

    // Pass funcWrapper as the function pointer, and pass &func as the data parameter
    ImGui::PlotLines("##Graph", funcWrapper, &plotFunc, resolution, 0, parameter.getName().c_str(), ref.min(), ref.max(), ImVec2(0.0f, 80.0f));
    
    ImGui::PopItemWidth();
    
    for (auto & param : ref.parameters) didChange |= DrawParameter(*param);
  }
  
  ImGui::PopID();
  
  return didChange;
}

inline bool PlotFunction(ofParameter<ofxCortex::core::types::Plot> & parameter)
{
  const ofxCortex::core::types::Plot & ref = parameter.get();
  bool didChange = false;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float availableWidth = ImGui::GetContentRegionAvail().x;
    float frameHeight = ImGui::GetFrameHeight();
    
    int resolution = ImGui::GetContentRegionAvail().x / 2.0;
    
    struct PlotContext {
      const ofxCortex::core::types::Plot & function;
//      const ofParameterGroup& params;
    };
    
    PlotContext context{ ref };
    
    // Updated function to use the context
    std::function<float(int)> plotFunc = [&context, resolution](int i) {
      float x = static_cast<float>(i) / (resolution - 1);
      return context.function(x);
    };
    
    auto funcWrapper = [](void* data, int i) -> float {
      auto& func = *static_cast<std::function<float(int)>*>(data);
      return func(i);
    };
    
    ImGui::PushItemWidth(-FLT_MIN);

    // Pass funcWrapper as the function pointer, and pass &func as the data parameter
    ImGui::PlotLines("##Graph", funcWrapper, &plotFunc, resolution, 0, NULL, ref.getMin(), ref.getMax(), ImVec2(0.0f, 80.0f));
    
    ImGui::PopItemWidth();
    
    core::DrawOverlay(parameter.getName().c_str(), NULL, pos, availableWidth, frameHeight);
  }
  
  ImGui::PopID();
  
  return didChange;
}

}
