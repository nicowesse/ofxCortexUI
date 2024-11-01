#pragma once

#include "ofxCortex/utils/ParameterUtils.h"
#include "ofxCortex/utils/ShapingUtils.h"
#include "ofxCortex/types/AllTypes.h"

namespace ofxCortex::ui::utils {

inline bool Group(ofParameterGroup & group);

template<typename T>
inline bool ShapingFunction(ofParameter<T> & parameter);

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
  else if (type == typeid(ofParameter<string>).name()) { didChange |= components::ParameterTextInput(parameter.cast<string>()); }
  else if (type == typeid(ofParameter<bool>).name()) { didChange |= components::ParameterCheckbox(parameter.cast<bool>()); }
  
  else if (type == typeid(ofParameter<ofxCortex::core::types::Select<int>>).name()) { didChange |= components::ParameterDropdown(parameter.cast<ofxCortex::core::types::Select<int>>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::types::BeatDivision>).name()) { didChange |= components::ParameterDropdown(parameter.cast<ofxCortex::core::types::BeatDivision>()); }
  
  else if (type == typeid(ofParameter<ofFloatColor>).name()) { didChange |= components::ParameterColor(parameter.cast<ofFloatColor>()); }
  else if (type == typeid(ofParameter<void>).name()) { didChange |= components::ParameterButton(parameter.cast<void>()); }
  else if (type == typeid(ofParameter<ofxCortex::core::types::Separator>).name()) { ImGui::SeparatorText(parameter.cast<ofxCortex::core::types::Separator>()->heading.c_str()); }
  
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
  
  return didChange;
}

inline bool Group(ofParameterGroup & group)
{
  bool didChange = false;
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(group).c_str());
  {
    int level = ofxCortex::core::utils::Parameters::getLevel(group);
    int flags = (level == 0) ? ImGuiTreeNodeFlags_DefaultOpen : 0;
    
    if (ImGui::CollapsingHeader(group.getName().c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | flags))
    {
      ImGui::Indent();
      for (auto & param : group)
      {
        didChange |= DrawParameter(*param);
      }
      ImGui::Unindent();
    }
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
    
//    std::function<float(int)> func = [&](int i) {
//        return ref(static_cast<float>(i) / (resolution - 1));
//    };
//    
//    auto funcWrapper = [](void* data, int i) -> float {
//        auto& func = *static_cast<std::function<float(int)>*>(data);
//        return func(i);
//    };
    
    ImGui::PushItemWidth(-1.0f);

    // Pass funcWrapper as the function pointer, and pass &func as the data parameter
    ImGui::PlotLines("##Graph", funcWrapper, &plotFunc, resolution, 0, parameter.getName().c_str(), ref.min(), ref.max(), ImVec2(0.0f, 80.0f));
    
    ImGui::PopItemWidth();
    
    for (auto & param : ref.parameters) didChange |= DrawParameter(*param);
  }
  
  ImGui::PopID();
  
  return didChange;
}

}
