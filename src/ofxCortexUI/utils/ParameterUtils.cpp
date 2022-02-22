//
//  ParameterUtils.cpp
//  cortex
//
//  Created by Nicolay Wesseltoft on 2/20/22.
//

#include "ParameterUtils.h"
#include "ofxCortexUI/components/Label.h"

namespace ofxCortex { namespace utils {

std::vector<std::shared_ptr<ofxCortex::ui::View> > ParameterUtils::createComponentsForParameterGroup(ofParameterGroup & parameters, int addSpaces)
{
  vector<shared_ptr<ofxCortex::ui::View>> views;
  
  if (parameters.getName().size() > 0) views.push_back(ui::Label<string>::create(parameters.getName()));

  vector<shared_ptr<ofAbstractParameter> >::iterator param = parameters.begin();

  for (;param != parameters.end(); param++)
  {
    auto view = createViewsFromParameter(*(param->get()));
    if (view.size() > 0) views.insert(views.end(), view.begin(), view.end());
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
}

}}
