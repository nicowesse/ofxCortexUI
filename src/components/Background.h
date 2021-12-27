#pragma once

#include "core/Component.h"

namespace ofxCortex { namespace ui {

class Background : public ofxCortex::ui::Component {
public:
  Background() : Component() {};
  static shared_ptr<Background> create() { return make_shared<Background>(); }
  
  void drawBackground(ofFloatColor c)
  {
    const auto & rect = getRenderRect();
    
    ofPushStyle();
    {
      ofFill();
      ofSetColor(c);
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
