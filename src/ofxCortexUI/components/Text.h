#pragma once

namespace ofxCortex::ui::components {

inline bool ParameterTextInput(ofParameter<string> & parameter)
{
  bool didChange = false;
  auto ref = parameter.get();
  
  char inputBuffer[256];
  strcpy(inputBuffer, ref.c_str());
  
  ImGui::PushID(ofxCortex::core::utils::Parameters::hash(parameter).c_str());
  {
    ImGui::Button(parameter.getName().c_str()); ImGui::SameLine();
    
    float width = ImGui::GetContentRegionAvail().x;
    ImGui::PushItemWidth(width);
    if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer))) {
      parameter.set(inputBuffer);
      didChange |= true;
    }
    ImGui::PopItemWidth();
  }
  ImGui::PopID();
  
  if (didChange) ofxCortex::ui::focusedParameter = parameter.newReference();
  
  return didChange;
}

}
