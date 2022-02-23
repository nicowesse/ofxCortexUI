#pragma once

#include "ofVectorMath.h"
#include "ofColor.h"
#include "ofTrueTypeFont.h"
#include "ofRectangle.h"

namespace ofxCortex { namespace ui {

class Style {
public:
  Style() :
    margin(0.0),
    padding(4.0),
    boxRadius(3.0),
    backgroundColor(0.06),
    foregroundColor(0.12),
    supportingColor(0.4),
    containerColor(0.08),
    accentColor(ofFloatColor::white),
    borderColor(ofFloatColor::white, 0.25),
    labelFontColor(ofFloatColor::white),
    valueFontColor(ofFloatColor::white, 0.75)
  {
    
  };
  
  static std::shared_ptr<Style> create() { return std::make_shared<Style>(); }
  static std::shared_ptr<Style> copy(std::shared_ptr<Style> original) { return std::make_shared<Style>(*original); }
  
  ofFloatColor backgroundColor;
  ofFloatColor foregroundColor;
  ofFloatColor supportingColor;
  ofFloatColor containerColor;
  ofFloatColor accentColor;
  ofFloatColor borderColor;
  
  struct Spacing {
    float top{0.0};
    float right{0.0};
    float bottom{0.0};
    float left{0.0};
    
    Spacing(float s) : top(s), right(s), bottom(s), left(s) {};
    operator glm::vec2() const { return glm::vec2(left, top); }
    operator glm::vec4() const { return glm::vec4(left, top, right, bottom); }
    static Spacing zero() { return Spacing(0); }
    
    Spacing& operator += (const float& rhs)
    {
      top += rhs;
      right += rhs;
      bottom += rhs;
      left += rhs;
      
      return *this;
    }
    
    Spacing& operator *= (const float& rhs)
    {
      top *= rhs;
      right *= rhs;
      bottom *= rhs;
      left *= rhs;
      
      return *this;
    }
  } margin, padding;
  
  struct BoxRadius {
    float topLeft{0.0};
    float topRight{0.0};
    float bottomLeft{0.0};
    float bottomRight{0.0};
    
    BoxRadius(float s) : topLeft(s), topRight(s), bottomLeft(s), bottomRight(s) {};
    BoxRadius& operator += (const float& rhs)
    {
      topLeft += rhs;
      topRight += rhs;
      bottomLeft += rhs;
      bottomRight += rhs;
      
      return *this;
    }
    
    BoxRadius& operator *= (const float& rhs)
    {
      topLeft *= rhs;
      topRight *= rhs;
      bottomLeft *= rhs;
      bottomRight *= rhs;
      
      return *this;
    }
  } boxRadius;
  
  ofAlignHorz labelAlignment { OF_ALIGN_HORZ_LEFT };
  ofAlignHorz valueAlignment { OF_ALIGN_HORZ_LEFT };
  
  struct Font {
    std::shared_ptr<ofTrueTypeFont> ttf;
    float xHeight;
    
    Font() {
      ttf = std::make_shared<ofTrueTypeFont>();
    }
    
    void load(std::string path = "assets/fonts/SF-Pro-Text-Regular.otf");
  };
  
  struct FontLibrary {
    Font label;
    Font value;
    
    FontLibrary() {
      label.load("assets/fonts/SF-Pro-Text-Regular.otf");
      value.load("assets/fonts/SF-Pro-Text-Regular.otf");
    }
  } fonts;
  
  
  ofFloatColor labelFontColor;
  ofFloatColor valueFontColor;
  
  const std::shared_ptr<ofTrueTypeFont> getLabelFont() { return fonts.label.ttf; }
  const std::shared_ptr<ofTrueTypeFont> getValueFont() { return fonts.value.ttf; }
  float getFontXHeight() const { return fonts.label.xHeight;  }
  
protected:
};

}}
