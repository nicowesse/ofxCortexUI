#pragma once

#include "core/View.h"
#include "components/Background.h"

namespace ofxCortex { namespace ui {

template<typename T>
class Label : public ofxCortex::ui::View {
public:
  
  Label(string name)
  : ofxCortex::ui::View(name)
  {
    parameter.setName(name);
    
    _init();
  }
  static shared_ptr<Label<T>> create(string name) { return make_shared<Label<T>>(name); }
  
  Label(ofParameter<T> & param)
  : ofxCortex::ui::View()
  {
    setName(parameter.getName());
    parameter.makeReferenceTo(param);
    
    _init();
  }
  static shared_ptr<Label<T>> create(ofParameter<T> & parameter) { return make_shared<Label<T>>(parameter); }
  
  
  void drawLabel(ofFloatColor color, ofAlignHorz labelAlignment = OF_ALIGN_HORZ_LEFT)
  {
    const auto & rect = getRenderRect();
    
    float labelInset = 12.0f;
    
    string label = parameter.getName();
    float labelWidth = style->getLabelFont()->stringWidth(label);
    
    float x = rect.getLeft() + 12;
    if (labelAlignment == OF_ALIGN_HORZ_CENTER) x = rect.getCenter().x - (labelWidth * 0.5f);
    else if (labelAlignment == OF_ALIGN_HORZ_RIGHT) x = rect.width - 12 - labelWidth;
    float y = rect.getCenter().y + (style->getFontXHeight() * 0.5);
    
    ofPushStyle();
    {
      ofSetColor(color);
      style->getLabelFont()->drawString(label, x, y);
    }
    ofPopStyle();
  }
  
  void drawLabel()
  {
    this->drawLabel(style->labelFontColor, style->labelAlignment);
  }
  
protected:
  virtual string _getModule() const override { return "Label"; };
  
  void _init()
  {
    background = ofxCortex::ui::Background::create();
    background->setName("Label::Background");
    background->disableEvents();
  }
  
  virtual void _draw() override
  {
    background->drawBackground();
    this->drawLabel();
  }
  
  virtual void _adjustLayout() override
  {
    background->setRect(this->getRect());
    
    float labelWidth = style->getLabelFont()->stringWidth(parameter.getName());
    if (this->getWidth() < labelWidth) { this->setWidth(labelWidth); }
    
    View::_adjustLayout();
  }
  
  shared_ptr<ofxCortex::ui::Background> background;
  ofParameter<T> parameter;
};

}}
