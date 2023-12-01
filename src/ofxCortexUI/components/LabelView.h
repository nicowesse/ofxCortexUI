#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/core/LayoutHelpers.h"
#include "ofParameter.h"

namespace ofxCortex { namespace ui {

class LabelView : public ofxCortex::ui::View {
public:
  
  LabelView(std::string name)
  : View(name)
  {
    setName(name);
    parameter.setName(name);
  }
  static std::shared_ptr<LabelView> create(std::string name) {
    auto instance = std::make_shared<LabelView>(name);
    instance->_setup();
    return instance;
  }
  
  LabelView(ofAbstractParameter & param)
  : View(param.getName())
  {
    parameter.makeReferenceTo(param.cast<std::string>());
  }
  
  static std::shared_ptr<LabelView> create(ofAbstractParameter & parameter) {
    auto instance = std::make_shared<LabelView>(parameter);
    instance->_setup();
    return instance;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "LabelView"; };
  
  void _setup()
  {
  }
  
  virtual void _draw() override
  {
    ofPushStyle();
    {
      Styling::drawBackground(this->getBounds());
      
      ofSetColor(Styling::getForegroundColor());
      Styling::drawLabel(parameter.getName(), this->getContentBounds());
    }
    ofPopStyle();
  }
  
  virtual void _postDraw() override
  {
    if (_isFocused()) Styling::drawFocusBorder(this->getBounds());
  }
  
  ofParameter<std::string> parameter;
};

}}
