#pragma once

#include "BaseTheme.h"
#include "ofMain.h"

namespace ofxCortex::ui::themes {

class DarkTheme: public ofxImGui::BaseTheme
{
public:
  void setup() override {
    ofColor BG = ofColor::black;
    ofColor componentBG = ofColor(16);
    ofColor headerBG = componentBG; //ofColor(24);
    ofColor accent = ofColor::white;
    
    ImGuiStyle* style = &ImGui::GetStyle();
    
    style->WindowMinSize = ImVec2(160, 65);
    
    style->WindowPadding = ImVec2(8, 8);
    style->FramePadding = ImVec2(12, 8);
    style->ItemSpacing = ImVec2(4, 4);
    style->ItemInnerSpacing = ImVec2(style->ItemSpacing.y, style->ItemSpacing.y);
    style->IndentSpacing = 12.0f;
    
    style->ScrollbarSize = 2.0f;
    style->GrabMinSize = 2.0f;
    
    style->WindowBorderSize = 0.0f;
    style->FrameBorderSize = 0.0f;
    style->PopupBorderSize = 1.0f;
    
    style->WindowRounding = 8.0f;
    style->ChildRounding = 4.0f;
    style->FrameRounding = 4.0f;
    style->PopupRounding = 4.0f;
    style->TabRounding = 4.0f;
    style->ScrollbarRounding = 2.0f;
    style->GrabRounding = 12.0f;
    
    style->CellPadding = ImVec2(4, 4);
    style->ColumnsMinSpacing = 50.0f;
    
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    //    style->WindowMenuButtonPosition = 2;
    
    style->Alpha = 1.0f;
    
    style->Colors[ImGuiCol_Text] = ImVec4(accent, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(ofColor::white, 0.60f);
    
    style->Colors[ImGuiCol_WindowBg] = ImVec4(BG, 1.00f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(componentBG + 12, 1.00f);
    
    style->Colors[ImGuiCol_Border] = ImVec4(ofColor::white, 0.20f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(componentBG, 1.00f);
    //    style->Colors[ImGuiCol_FrameBg] = ImVec4(ofColor::green, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(componentBG + 4, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(componentBG + 8, 1.00f);
    
    style->Colors[ImGuiCol_TitleBg] = ImVec4(ofColor::black, 0.00f);
    //    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(getRandomColor(), 0.75f);
    //    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(getRandomColor(), 1.00f);
    
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(ofColor::black, 0.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(componentBG + 16, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(ofColor::white, 0.50f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(ofColor::white, 0.75f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(ofColor::white, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(ofColor::white, 1.00f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(ofColor::white, 0.75f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(ofColor::white, 1.00f);
    
    style->Colors[ImGuiCol_Button] = style->Colors[ImGuiCol_FrameBg]; // ImVec4(ofColor::white, 0.05f);
    style->Colors[ImGuiCol_ButtonHovered] = style->Colors[ImGuiCol_FrameBgHovered]; //ImVec4(ofColor::white, 0.075f);
    style->Colors[ImGuiCol_ButtonActive] = style->Colors[ImGuiCol_FrameBgActive]; //ImVec4(ofColor::white, 0.1f);
    
    style->Colors[ImGuiCol_Tab] = ImVec4(headerBG, 1.00f);
//    style->Colors[ImGuiCol_TabSelected] = ImVec4(headerBG + 16, 1.00f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(headerBG + 8, 1.00f);
    
    style->Colors[ImGuiCol_Header] = ImVec4(componentBG, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(componentBG + 6, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(componentBG + 12, 1.00f);
    
    style->Colors[ImGuiCol_Separator] = ImVec4(ofColor::white, 0.25f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(ofColor::white, 0.50f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(ofColor::white, 1.00f);
    style->SeparatorTextAlign = ImVec2(0.5f, 0.5f);
    style->SeparatorTextBorderSize = 1.0f;
    
    //    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(getRandomColor(), 0.04f);
    //    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(getRandomColor(), 0.78f);
    //    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(getRandomColor(), 1.00f);
    
        style->Colors[ImGuiCol_PlotLines] = ImVec4(ofColor::tomato, 1.00f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(accent, 1.00f);
//        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(getRandomColor(), 0.63f);
//        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(getRandomColor(), 1.00f);
    
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(accent, 0.50f);
    
    //    style->Colors[ImGuiCol_PopupBg] = ImVec4(getRandomColor(), 0.92f);
    //    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(getRandomColor(), 0.73f);
  }
  
};

}
