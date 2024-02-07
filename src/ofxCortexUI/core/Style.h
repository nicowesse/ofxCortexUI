#pragma once

#include "kiwi.h"
#include "LayoutEngine.h"
#include "ofTrueTypeFont.h"
#include "ofAppGLFWWindow.h"

namespace ofxCortex { namespace ui {

class Styling {
public:
  enum State {
    IDLE = 0,
    HOVER,
    ACTIVE,
    FOCUS
  };
  
  struct Font : public ofTrueTypeFont {
    ofRectangle dimensions;
  };
  
  static const kiwi::Variable & defaultRowHeight() { return get().default_row_height; }
  static float getRowHeight(int rows = 1) { return getScaled(40.0 * rows); }
  
  static void  setMargins(float value) {
    setMarginTop(value);
    setMarginLeft(value);
    setMarginBottom(value);
    setMarginRight(value);
  }
  static const kiwi::Variable & marginTop() { return get().margin_top; }
  static double getMarginTop() { return get().margin_top.value(); }
  static void  setMarginTop(float value) { LayoutEngine::suggestValue(get().margin_top, value); }
  
  static const kiwi::Variable & marginLeft() { return get().margin_left; }
  static double getMarginLeft() { return get().margin_left.value(); }
  static void  setMarginLeft(float value) { LayoutEngine::suggestValue(get().margin_left, value); }
  
  static const kiwi::Variable & marginBottom() { return get().margin_bottom; }
  static double getMarginBottom() { return get().margin_bottom.value(); }
  static void  setMarginBottom(float value) { LayoutEngine::suggestValue(get().margin_bottom, value); }
  
  static const kiwi::Variable & marginRight() { return get().margin_right; }
  static double getMarginRight() { return get().margin_right.value(); }
  static void  setMarginRight(float value) { LayoutEngine::suggestValue(get().margin_right, value); }
  
  static void  setPadding(float value) {
    setPaddingTop(value);
    setPaddingLeft(value);
    setPaddingBottom(value);
    setPaddingRight(value);
  }
  static const kiwi::Variable & paddingTop() { return get().padding_top; }
  static double getPaddingTop() { return get().padding_top.value(); }
  static void  setPaddingTop(float value) { LayoutEngine::suggestValue(get().padding_top, value); }
  
  static const kiwi::Variable & paddingBottom() { return get().padding_bottom; }
  static double getPaddingBottom() { return get().padding_bottom.value(); }
  static void  setPaddingBottom(float value) { LayoutEngine::suggestValue(get().padding_bottom, value); }
  
  static double getVerticalPadding() { return get().getPaddingTop() + get().getPaddingBottom(); }
  
  static const kiwi::Variable & paddingLeft() { return get().padding_left; }
  static double getPaddingLeft() { return get().padding_left.value(); }
  static void  setPaddingLeft(float value) { LayoutEngine::suggestValue(get().padding_left, value); }
  
  static const kiwi::Variable & paddingRight() { return get().padding_right; }
  static double getPaddingRight() { return get().padding_right.value(); }
  static void  setPaddingRight(float value) { LayoutEngine::suggestValue(get().padding_right, value); }
  
  static double getHorizontalPadding() { return get().getPaddingLeft() + get().getPaddingRight(); }
  
  static const kiwi::Variable & spacingX() { return get().spacing_x; }
  static double getSpacingX() { return get().spacing_x.value(); }
  static const kiwi::Variable & spacingY() { return get().spacing_y; }
  static double getSpacingY() { return get().spacing_y.value(); }
  
  static const ofColor & getBackgroundColor(State state = IDLE) {
    switch (state) {
      case HOVER: return get().backgroundColorHovering; break;
      case ACTIVE: return get().backgroundColorActive; break;
      case IDLE:
      default: return get().backgroundColor; break;
    }
  }
  static const ofColor & getContainerColor() { return get().containerColor; }
  static const ofColor & getForegroundColor() { return get().foregroundColor; }
  static const ofColor & getAccentColor() { return get().accentColor; }
  static const ofColor & getBorderColor(State state = IDLE) {
    switch (state) {
      case ACTIVE: return getAccentColor(); break;
      case FOCUS: return get().borderColorFocused; break;
        //      case HOVER: get().borderColorHover; break;
      case IDLE: return get().borderColor; break;
      default: return get().borderColor; break;
    }
  }
  
  static float getDotSize() { return getScaled(3); }
  
  static void drawBackground(const ofRectangle & bounds, ofColor fillColor, ofColor borderColor) {
    ofPushStyle();
    {
      ofSetColor(fillColor);
      ofFill();
      ofDrawRectRounded(bounds, 6 * Styling::getScale());
    }
    ofPopStyle();
  }
  
  static void drawBackground(const ofRectangle & bounds, State mouseState = IDLE) {
    return drawBackground(bounds, getBackgroundColor(mouseState), getBorderColor(mouseState));
  }
  
  static void drawBorder(const ofRectangle & bounds, ofColor borderColor)
  {
    ofRectangle scaledBounds; scaledBounds.setFromCenter(bounds.getCenter(), bounds.width - 1, bounds.height - 1);
    ofPushStyle();
    {
      ofSetColor(borderColor);
      ofNoFill();
      ofDrawRectRounded(scaledBounds, Styling::getScaled(5));
    }
    ofPopStyle();
  }
  
  static void drawBorder(const ofRectangle & bounds, State mouseState = IDLE)
  {
    drawBorder(bounds, getBorderColor(mouseState));
  }
  
  static void drawFocusBorder(const ofRectangle & bounds)
  {
    drawBorder(bounds, getBorderColor(State::FOCUS));
  }
  
  static void drawContainerBackground(const ofRectangle & bounds, ofColor fillColor = Styling::getContainerColor(), ofColor borderColor = Styling::getBorderColor(), int level = 0) {
    fillColor += 8 * level;
    
    ofPushStyle();
    {
      ofSetColor(fillColor);
      ofFill();
      ofDrawRectRounded(bounds, Styling::getScaled(6));
      
      //      ofSetColor(borderColor);
      //      ofNoFill();
      //    //  ofDrawRectRounded(b.x, b.y, layerZ * 10 + 1, b.width, b.height, 4);
      //      ofDrawRectRounded(bounds, (6 + Styling::getPaddingTop()) * Styling::getScale());
    }
    ofPopStyle();
  }
  
  static void drawLabel(const std::string & text, const glm::vec2 & position, ofAlignHorz horzAlignment = OF_ALIGN_HORZ_LEFT, ofAlignVert verticalAlignment = OF_ALIGN_VERT_CENTER)
  {
    float textW = get().fonts.labelFont.stringWidth(text);
    
    glm::vec2 offset { 0, 0 };
    if (verticalAlignment == OF_ALIGN_VERT_TOP) offset.y += get().fonts.labelFont.dimensions.height;
    if (verticalAlignment == OF_ALIGN_VERT_CENTER) offset.y += get().fonts.labelFont.dimensions.height * 0.5;
    
    if (horzAlignment == OF_ALIGN_HORZ_CENTER) offset.x -= textW * 0.5;
    if (horzAlignment == OF_ALIGN_HORZ_RIGHT) offset.x -= textW;
    
    glm::vec2 renderPosition = position + offset;
    
    Styling::getLabelFont().drawString(text, renderPosition.x, renderPosition.y);
  }
  
  static void drawLabel(const std::string & text, const ofRectangle & BB, ofAlignHorz horzAlignment = OF_ALIGN_HORZ_LEFT, ofAlignVert verticalAlignment = OF_ALIGN_VERT_CENTER)
  {
    if (horzAlignment == OF_ALIGN_HORZ_LEFT)
    {
      float X = BB.getLeft() + Styling::getPaddingLeft();
      
      if (verticalAlignment == OF_ALIGN_VERT_TOP) drawLabel(text, glm::vec2(X, BB.getTop() + Styling::getPaddingTop()), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_CENTER) drawLabel(text, glm::vec2(X, BB.getCenter().y), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_BOTTOM) drawLabel(text, glm::vec2(X, BB.getBottom() - Styling::getPaddingBottom()), horzAlignment, verticalAlignment);
    }
    
    if (horzAlignment == OF_ALIGN_HORZ_CENTER)
    {
      float X = BB.getCenter().x;
      
      if (verticalAlignment == OF_ALIGN_VERT_TOP) drawLabel(text, glm::vec2(X, BB.getTop() + Styling::getPaddingTop()), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_CENTER) drawLabel(text, glm::vec2(X, BB.getCenter().y), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_BOTTOM) drawLabel(text, glm::vec2(X, BB.getBottom() - Styling::getPaddingBottom()), horzAlignment, verticalAlignment);
    }
    
    if (horzAlignment == OF_ALIGN_HORZ_RIGHT)
    {
      float X = BB.getRight() - getPaddingRight();
      
      if (verticalAlignment == OF_ALIGN_VERT_TOP) drawLabel(text, glm::vec2(X, BB.getTop() + Styling::getPaddingTop()), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_CENTER) drawLabel(text, glm::vec2(X, BB.getCenter().y), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_BOTTOM) drawLabel(text, glm::vec2(X, BB.getBottom() - Styling::getPaddingBottom()), horzAlignment, verticalAlignment);
    }
  }
  
  static void drawValue(const std::string & text, const glm::vec2 & position, ofAlignHorz horzAlignment = OF_ALIGN_HORZ_RIGHT, ofAlignVert verticalAlignment = OF_ALIGN_VERT_CENTER)
  {
    float textW = get().fonts.valueFont.stringWidth(text);
    
    glm::vec2 offset { 0, 0 };
    if (verticalAlignment == OF_ALIGN_VERT_TOP) offset.y += Styling::getValueFont().dimensions.height;
    if (verticalAlignment == OF_ALIGN_VERT_CENTER) offset.y += Styling::getValueFont().dimensions.height * 0.5;
    
    if (horzAlignment == OF_ALIGN_HORZ_CENTER) offset.x -= textW * 0.5;
    if (horzAlignment == OF_ALIGN_HORZ_RIGHT) offset.x -= textW;
    
    glm::vec2 renderPosition = position + offset;
    
    getValueFont().drawString(text, renderPosition.x, renderPosition.y);
  }
  
  static void drawValue(const std::string & text, const ofRectangle & BB, ofAlignHorz horzAlignment = OF_ALIGN_HORZ_RIGHT, ofAlignVert verticalAlignment = OF_ALIGN_VERT_CENTER)
  {
    if (horzAlignment == OF_ALIGN_HORZ_LEFT)
    {
      float X = BB.getLeft() + Styling::getPaddingLeft();
      
      if (verticalAlignment == OF_ALIGN_VERT_TOP) drawValue(text, glm::vec2(X, BB.getTop() + Styling::getPaddingTop()), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_CENTER) drawValue(text, glm::vec2(X, BB.getCenter().y), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_BOTTOM) drawValue(text, glm::vec2(X, BB.getBottom() - Styling::getPaddingBottom()), horzAlignment, verticalAlignment);
    }
    
    if (horzAlignment == OF_ALIGN_HORZ_CENTER)
    {
      float X = BB.getCenter().x;
      
      if (verticalAlignment == OF_ALIGN_VERT_TOP) drawValue(text, glm::vec2(X, BB.getTop() + Styling::getPaddingTop()), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_CENTER) drawValue(text, glm::vec2(X, BB.getCenter().y), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_BOTTOM) drawValue(text, glm::vec2(X, BB.getBottom() - Styling::getPaddingBottom()), horzAlignment, verticalAlignment);
    }
    
    if (horzAlignment == OF_ALIGN_HORZ_RIGHT)
    {
      float X = BB.getRight() - Styling::getPaddingRight();
      
      if (verticalAlignment == OF_ALIGN_VERT_TOP) drawValue(text, glm::vec2(X, BB.getTop() + Styling::getPaddingTop()), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_CENTER) drawValue(text, glm::vec2(X, BB.getCenter().y), horzAlignment, verticalAlignment);
      if (verticalAlignment == OF_ALIGN_VERT_BOTTOM) drawValue(text, glm::vec2(X, BB.getBottom() - Styling::getPaddingBottom()), horzAlignment, verticalAlignment);
    }
  }
  
  static Styling::Font getLabelFont() { return get().fonts.labelFont; }
  static Styling::Font getValueFont() { return get().fonts.valueFont; }
  
  static float getScale() { return get().scale; }
  static float getScaled(float x = 1.0) { return x * get().scale; }
  
private:
  
  Styling() {
    ofLogVerbose(__PRETTY_FUNCTION__);
  
    ofAddListener(ofEvents().windowResized, this, &Styling::onWindowResized, OF_EVENT_ORDER_BEFORE_APP);
    
    float DPI = ((ofAppGLFWWindow*) ofGetWindowPtr())->getPixelScreenCoordScale();
    this->scale = DPI;
    
    double initialMargin = 6 * DPI;
    LayoutEngine::addEditVariable(margin_top, kiwi::strength::strong);
    LayoutEngine::addEditVariable(margin_left, kiwi::strength::strong);
    LayoutEngine::addEditVariable(margin_bottom, kiwi::strength::strong);
    LayoutEngine::addEditVariable(margin_right, kiwi::strength::strong);
    
    LayoutEngine::suggestValue(margin_top, initialMargin);
    LayoutEngine::suggestValue(margin_left, initialMargin);
    LayoutEngine::suggestValue(margin_bottom, initialMargin);
    LayoutEngine::suggestValue(margin_right, initialMargin);
    
    double initialPadding = 6 * DPI;
    LayoutEngine::addEditVariable(padding_top, kiwi::strength::strong);
    LayoutEngine::addEditVariable(padding_left, kiwi::strength::strong);
    LayoutEngine::addEditVariable(padding_bottom, kiwi::strength::strong);
    LayoutEngine::addEditVariable(padding_right, kiwi::strength::strong);
    
    LayoutEngine::suggestValue(padding_top, initialPadding);
    LayoutEngine::suggestValue(padding_left, initialPadding);
    LayoutEngine::suggestValue(padding_bottom, initialPadding);
    LayoutEngine::suggestValue(padding_right, initialPadding);
    
    double initialSpacing = 6 * DPI;
    LayoutEngine::addEditVariable(spacing_x, kiwi::strength::strong);
    LayoutEngine::addEditVariable(spacing_y, kiwi::strength::strong);
    LayoutEngine::suggestValue(spacing_x, initialSpacing);
    LayoutEngine::suggestValue(spacing_y, initialSpacing);
    
    LayoutEngine::addEditVariable(default_row_height, kiwi::strength::strong);
    LayoutEngine::suggestValue(default_row_height, 40.0 * DPI);
    
    this->loadFonts(DPI);
  };
  
  ~Styling() {
    ofRemoveListener(ofEvents().windowResized, this, &Styling::onWindowResized);
  }
  Styling(const Styling&) = delete;
  Styling& operator=(const Styling&) = delete;
  
  static Styling& get()
  {
    static Styling instance;
    return instance;
  }
  
  void onWindowResized(ofResizeEventArgs &e)
  {
    this->scale = ((ofAppGLFWWindow*) ofGetWindowPtr())->getPixelScreenCoordScale();
    
    ofLogVerbose("Styling::" + std::string(__FUNCTION__) + "()") << "DPI = " << this->scale;
    
    double initialMargin = 6 * scale;
    LayoutEngine::suggestValue(margin_top, initialMargin);
    LayoutEngine::suggestValue(margin_left, initialMargin);
    LayoutEngine::suggestValue(margin_bottom, initialMargin);
    LayoutEngine::suggestValue(margin_right, initialMargin);
    
    double initialPadding = 6 * scale;
    LayoutEngine::suggestValue(padding_top, initialPadding);
    LayoutEngine::suggestValue(padding_left, initialPadding);
    LayoutEngine::suggestValue(padding_bottom, initialPadding);
    LayoutEngine::suggestValue(padding_right, initialPadding);
    
    double initialSpacing = 6 * scale;
    LayoutEngine::suggestValue(spacing_x, initialSpacing);
    LayoutEngine::suggestValue(spacing_y, initialSpacing);
    
    LayoutEngine::suggestValue(default_row_height, 40.0 * scale);
    
    this->loadFonts(scale);
    
    LayoutEngine::forceSolve();
  }
  
  float scale;
//  kiwi::Variable scale { "scale" };
  
  // Dimensions
  kiwi::Variable default_row_height { "default_row_height" };
  
  // Spacing
  kiwi::Variable margin_top { "margin_top" };
  kiwi::Variable scaled_margin_top { "scaled_margin_top" };
  kiwi::Variable margin_left { "margin_left" };
  kiwi::Variable scaled_margin_left { "scaled_margin_left" };
  kiwi::Variable margin_bottom { "margin_bottom" };
  kiwi::Variable scaled_margin_bottom { "scaled_margin_bottom" };
  kiwi::Variable margin_right { "margin_right" };
  kiwi::Variable scaled_margin_right { "scaled_margin_right" };
  
  kiwi::Variable padding_top { "padding_top" };
  kiwi::Variable padding_left { "padding_left" };
  kiwi::Variable padding_bottom { "padding_bottom" };
  kiwi::Variable padding_right { "padding_right" };
  
  kiwi::Variable spacing_x { "spacing_x" };
  kiwi::Variable spacing_y { "spacing_y" };
  
  // Colors
  ofColor backgroundColor { 16 };
  ofColor backgroundColorHovering { 20 };
  ofColor backgroundColorActive { 32 };
  
  ofColor containerColor { 20 };
  ofColor foregroundColor { 255 };
  
  ofColor accentColor { ofColor::fromHex(0xFFD953) };
  
  ofColor borderColor { 24 };
  ofColor borderColorHover { 255 };
  ofColor borderColorFocused { 128 };
  
  // Fonts
  struct Fonts {
    Font labelFont;
    Font valueFont;
  } fonts;
  
  void loadFonts(float scale)
  {
    std::string labelPath = "assets/fonts/SF-Pro-Text-Regular.otf";
    if (!ofFile::doesFileExist(labelPath)) labelPath = OF_TTF_MONO;
    
    ofTrueTypeFontSettings labelSettings(labelPath, 8 * scale);
    labelSettings.antialiased = true;
    labelSettings.contours = true;
    labelSettings.addRanges(ofAlphabet::Latin);
    labelSettings.addRanges({
      ofUnicode::Arrows
    });
    fonts.labelFont.load(labelSettings);
    fonts.labelFont.dimensions = ofRectangle(0, 0, fonts.labelFont.stringWidth("M"), fonts.labelFont.stringHeight("X"));
    
    ofTrueTypeFontSettings valueSettings("assets/fonts/SF-Mono-Medium.otf", 8 * scale);
    valueSettings.antialiased = true;
    valueSettings.contours = true;
    valueSettings.addRanges(ofAlphabet::Latin);
    valueSettings.addRanges({
      ofUnicode::Arrows
    });
    fonts.valueFont.load(valueSettings);
    fonts.valueFont.dimensions = ofRectangle(0, 0, fonts.valueFont.stringWidth("M"), fonts.valueFont.stringHeight("X"));
  }
  
};

}}
