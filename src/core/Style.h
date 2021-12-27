#pragma once

namespace ofxCortex { namespace ui {

class Style {
public:
  Style() :
    margin(2.0),
    padding(2.0),
    boxRadius(3.0),
    backgroundColor(0.06),
    foregroundColor(0.12),
    supportingColor(0.4),
    accentColor(ofFloatColor::white),
    borderColor(ofFloatColor::white, 0.25),
    labelFontColor(ofFloatColor::white),
    valueFontColor(ofFloatColor::white, 0.75)
  {
    fonts.load();
  };
  
  static shared_ptr<Style> create() { return make_shared<Style>(); }
  
  struct Spacing {
    float top{0.0};
    float right{0.0};
    float bottom{0.0};
    float left{0.0};
    
    Spacing(float s) : top(s), right(s), bottom(s), left(s) {};
    operator glm::vec2() const { return glm::vec2(left, top); }
    operator glm::vec4() const { return glm::vec4(left, top, right, bottom); }
  } margin, padding;
  
  struct BoxRadius {
    float topLeft{0.0};
    float topRight{0.0};
    float bottomLeft{0.0};
    float bottomRight{0.0};
    
    BoxRadius(float s) : topLeft(s), topRight(s), bottomLeft(s), bottomRight(s) {};
  } boxRadius;
  
  ofAlignHorz labelAlignment { OF_ALIGN_HORZ_LEFT };
  ofAlignHorz valueAlignment { OF_ALIGN_HORZ_LEFT };
  
  struct Font {
    shared_ptr<ofTrueTypeFont> regular;
    shared_ptr<ofTrueTypeFont> medium;
    float xHeight;
    
    Font() {
      regular = make_shared<ofTrueTypeFont>();
      medium = make_shared<ofTrueTypeFont>();
    }
    
    void load(string regularFontPath = "fonts/SF-Mono-Medium.otf", string mediumFontPath = "fonts/SF-Mono-Regular.otf")
    {
      std::initializer_list<ofUnicode::range> fontRanges = {
        ofUnicode::Latin1Supplement,
        ofUnicode::Arrows
      };
      
      ofTrueTypeFontSettings regularFontSettings(ofToDataPath(regularFontPath), 8.0f);
      regularFontSettings.antialiased = true;
      regularFontSettings.contours = true;
      regularFontSettings.addRanges(fontRanges);
      regular->load(regularFontSettings);
      
      xHeight = regular->stringHeight("X");
      
      ofTrueTypeFontSettings mediumFontSettings(ofToDataPath(mediumFontPath), 8.0f);
      mediumFontSettings.antialiased = true;
      mediumFontSettings.contours = true;
      mediumFontSettings.addRanges(fontRanges);
      medium->load(mediumFontSettings);
    }
  } fonts;
  
  ofFloatColor backgroundColor;
  ofFloatColor foregroundColor;
  ofFloatColor supportingColor;
  ofFloatColor accentColor;
  ofFloatColor borderColor;
  ofFloatColor labelFontColor;
  ofFloatColor valueFontColor;
  
  const shared_ptr<ofTrueTypeFont> getLabelFont() { return fonts.regular; }
  const shared_ptr<ofTrueTypeFont> getValueFont() { return fonts.regular; }
  float getFontXHeight() const { return fonts.xHeight;  }
  
protected:
};

}}
