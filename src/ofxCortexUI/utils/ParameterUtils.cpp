#include "ParameterUtils.h"
#include "ofxCortexUI/components/Label.h"
#include "ofxCortexUI/components/Value.h"
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
    auto groupParams = createViewsForParameterGroup(param.castGroup());
    views.insert(views.end(), groupParams.begin(), groupParams.end());
  }
  else if (type == typeid(ofParameter<float>).name())
  {
    views.push_back(Slider<float>::create(param.cast<float>()));
  }
  else if (type == typeid(ofParameter<int>).name())
  {
    views.push_back(Slider<int>::create(param.cast<int>()));
  }
  else if (type == typeid(ofParameter<string>).name())
  {
    views.push_back(Value<string>::create(param.cast<string>()));
  }
  else if (type == typeid(ofParameter<bool>).name())
  {
    views.push_back(Checkbox::create(param.cast<bool>()));
  }
  else if (type == typeid(ofParameter<void>).name())
  {
    views.push_back(Button::create(param.cast<void>()));
  }
  else if (type == typeid(ofParameter<ofxCortex::core::types::Range>).name())
  {
    views.push_back(RangeSlider::create(param.cast<ofxCortex::core::types::Range>()));
  }
  else
  {
    ofLogNotice("ParameterUtils::createViewsFromParameter") << "Implementation for type '" << type << "' is missing. No View created.";
  }
  
  return views;
}

}}
