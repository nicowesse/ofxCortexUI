#pragma once

#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Label.h"

namespace ofxCortex { namespace ui {

class GroupLabel : public ofxCortex::ui::View {
public:
  
  GroupLabel(ofParameterGroup & group)
  {
    groupRef = group.newReference();
    
    _init();
    _adjustLayout();
  };
  
  static shared_ptr<GroupLabel> create(ofParameterGroup & group) { return make_shared<GroupLabel>(group); }
  
protected:
  virtual string _getModule() const override { return "GroupLabel"; };
  
  void _init()
  {
    background = ui::Background::create();
    background->setName("GroupLabel::Background");
    background->disableEvents();
    
    label = ui::Label::create(groupRef->getName());
    label->setName("GroupLabel::Label");
    label->disableEvents();
  };
  
  virtual void _draw() override
  {
    int levels = groupRef->getGroupHierarchyNames().size() - 1;
    
    auto & RR = this->getRenderRect();
    background->drawBackground();
    label->drawLabel(style->labelFontColor, OF_ALIGN_HORZ_LEFT);
    
    ofPushStyle();
    ofSetColor(style->accentColor, 160 - 48 * levels);
    ofDrawRectRounded(RR.getLeft(), RR.getTop(), 3, RR.height, 8);
    ofPopStyle();
  }
  
  virtual void _adjustLayout() override
  {
    background->setRect(this->getRect());
    label->setRect(this->getRect());
    
    View::_adjustLayout();
  }
  
  // Members
  std::shared_ptr<ofAbstractParameter> groupRef;
  
  shared_ptr<ui::Background> background;
  shared_ptr<ui::Label> label;
};

}}
