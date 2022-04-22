#include "ParameterUtils.h"
#include "ofxCortexUI/components/Label.h"
#include "ofxCortexUI/components/Button.h"
#include "ofxCortexUI/components/Slider.h"
#include "ofxCortexUI/components/Checkbox.h"

#include "ofxCortex/types/Range.h"

namespace ofxCortex { namespace ui {

std::vector<std::shared_ptr<ofxCortex::ui::View> > ParameterUtils::createViewsForParameterGroup(ofParameterGroup & parameters, bool includeGroupName, int addSpaces)
{
  vector<shared_ptr<ofxCortex::ui::View>> views;
  
  if (parameters.getName().size() > 0 && includeGroupName) views.push_back(ui::Label::create(parameters.getName()));

  vector<shared_ptr<ofAbstractParameter> >::iterator param = parameters.begin();

  for (;param != parameters.end(); param++)
  {
    auto parameterViews = createViewsFromParameter(*(param->get()));
    if (parameterViews.size() > 0) views.insert(views.end(), parameterViews.begin(), parameterViews.end());
  }
  
//  if (addSpaces > 0) views.push_back(ofxCortex::ui::Spacer::create(addSpaces));
  
  return views;
}

std::vector<shared_ptr<ofxCortex::ui::View> > ParameterUtils::createViewsFromParameter(ofAbstractParameter & param)
{
  string type = param.type();
  vector<shared_ptr<ui::View> > views;
  
  if (type == typeid(ofParameterGroup).name()) // float
  {
//    ofParameterGroup &p = param.castGroup();
//    auto group = ui::Group::create(ofxVoid::utils::ParameterUtils::getKey(p));
//    group->addViews(ofxVoid::ui::createComponentsForParameterGroup(p));
//
//    views.push_back(group);
  }
  else if (type == typeid(ofParameter<float>).name())
  {
    ofParameter<float> & p = param.cast<float>();
    views.push_back(Slider<float>::create(p));
  }
  else if (type == typeid(ofParameter<int>).name())
  {
    ofParameter<int> & p = param.cast<int>();
    views.push_back(Slider<int>::create(p));
  }
  else if (type == typeid(ofParameter<string>).name())
  {
    ofParameter<string> & p = param.cast<string>();
    views.push_back(Label::create(p));
  }
  else if (type == typeid(ofParameter<bool>).name())
  {
    ofParameter<bool> & p = param.cast<bool>();
    views.push_back(Checkbox::create(p));
  }
  else if (type == typeid(ofParameter<void>).name())
  {
    ofParameter<void> & p = param.cast<void>();
    views.push_back(Button::create(p));
  }
  else if (type == typeid(ofParameter<ofxCortex::types::Range>).name())
  {
    ofParameter<ofxCortex::types::Range> & p = param.cast<ofxCortex::types::Range>();
    views.push_back(RangeSlider::create(p));
  }
  else
  {
    ofLogNotice("ParameterUtils::createViewsFromParameter") << "Implementation for type '" << type << "' is missing. No View created.";
  }
  
  return views;
}

}}
