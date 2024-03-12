#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/core/LayoutHelpers.h"
#include "ofParameter.h"

namespace ofxCortex { namespace ui {

class LabelView : public ofxCortex::ui::View {
protected:
  LabelView(const std::string & name)
  : View(name)
  {
    parameter.setName(name);
  }
  
  LabelView(ofAbstractParameter & param)
  : View(param.getName())
  {
    parameter.makeReferenceTo(param.cast<std::string>());
  }
  
public:
  template<typename ... T>
  static std::shared_ptr<LabelView> create(T&& ... t) {
    struct EnableMakeShared : public LabelView { EnableMakeShared(T&&... arg) : LabelView(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string getComponentName() const override { return "LabelView"; };
  
  virtual void viewDidLoad() override 
  {
    this->disableInteraction();
  }
  
  virtual void onDraw() override
  {
    ofPushStyle();
    {
      Styling::drawBackground(this->getFrame());
      
      ofSetColor(Styling::getForegroundColor());
      Styling::drawLabel(parameter.getName(), this->getContentFrame());
    }
    ofPopStyle();
  }
  
  ofParameter<std::string> parameter;
};

}}
