#pragma once

#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

class Background : public ofxCortex::ui::View {
public:
  Background(string name = "Background") : View(name) {};
  static shared_ptr<Background> create(string name = "Background") { return make_shared<Background>(name); }
  
  void drawBackground(ofFloatColor background, ofFloatColor border = ofFloatColor(1.0f, 0.0f))
  {
    const auto & rect = getRenderRect();
    
    ofPushStyle();
    {
      ofFill();
      ofSetColor(background);
      ofDrawRectRounded(rect, style->boxRadius.topLeft, style->boxRadius.topRight, style->boxRadius.bottomRight, style->boxRadius.bottomLeft);
      
      ofNoFill();
      ofSetColor(border);
      ofDrawRectRounded(rect, style->boxRadius.topLeft, style->boxRadius.topRight, style->boxRadius.bottomRight, style->boxRadius.bottomLeft);
    }
    ofPopStyle();
  }
  
  void drawBackground() { this->drawBackground(style->backgroundColor); }
  
protected:
  virtual string _getModule() const override { return "Background"; };
  
  virtual void _draw() override
  {
    drawBackground();
  }
  
  
};

}}
