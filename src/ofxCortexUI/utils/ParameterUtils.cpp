#include "ParameterUtils.h"


#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortexUI/components/ValueView.h"
#include "ofxCortexUI/components/ButtonView.h"
#include "ofxCortexUI/components/SliderView.h"
#include "ofxCortexUI/components/RangeSliderView.h"
#include "ofxCortexUI/components/CheckboxView.h"
#include "ofxCortexUI/components/ImageView.h"
#include "ofxCortexUI/components/FileView.h"

#include "ofxCortex/types/Parameter.h"
#include "ofxCortex/types/Range.h"
#include "ofxCortex/types/Image.h"
#include "ofxCortex/types/Spacer.h"

namespace ofxCortex { namespace ui {

std::vector<std::shared_ptr<ofxCortex::ui::View> > ParameterUtils::createViewsForParameterGroup(ofParameterGroup & parameters, bool includeGroupName, int addSpaces)
{
  vector<shared_ptr<ofxCortex::ui::View>> views;
  
  if (parameters.getName().size() > 0 && includeGroupName) views.push_back(ui::LabelView::create(parameters.getName()));

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
  string valueType = param.valueType();
  vector<shared_ptr<ui::View> > views;
  
  if (type == typeid(ofParameterGroup).name()) // float
  {
    auto groupParams = createViewsForParameterGroup(param.castGroup());
    views.insert(views.end(), groupParams.begin(), groupParams.end());
  }
  else if (valueType == typeid(float).name() || type == typeid(ofParameter<float>).name() || type == typeid(ofxCortex::UnitParameter<float>).name())
  {
    views.push_back(SliderView<float>::create(param));
  }
  else if (valueType == typeid(int).name() || type == typeid(ofParameter<int>).name() || type == typeid(ofxCortex::UnitParameter<int>).name())
  {
    views.push_back(SliderView<int>::create(param));
  }
  else if (valueType == typeid(std::string).name() || type == typeid(ofParameter<string>).name())
  {
    ofParameter<string> & p = param.cast<string>();
    views.push_back(ValueView<string>::create(p));
  }
  else if (type == typeid(ofParameter<bool>).name())
  {
    ofParameter<bool> & p = param.cast<bool>();
    views.push_back(CheckboxView::create(p));
  }
  else if (type == typeid(ofParameter<void>).name())
  {
    ofParameter<void> & p = param.cast<void>();
    views.push_back(ButtonView::create(p));
  }
  else if (type == typeid(ofParameter<ofxCortex::core::types::Spacer>).name())
  {
    ofParameter<ofxCortex::core::types::Spacer> & p = param.cast<ofxCortex::core::types::Spacer>();
    
    auto spacer = View::create("Spacer");
    spacer->setHeight(Styling::getRowHeight(p->rows));
    views.push_back(spacer);
  }
  else if (type == typeid(ofParameter<ofxCortex::core::types::Range>).name())
  {
//    ofParameter<ofxCortex::core::types::Range> & p = param.cast<ofxCortex::core::types::Range>();
    views.push_back(RangeSliderView::create(param));
  }
//  else if (type == typeid(ofParameter<ofxCortex::core::types::Image>).name())
//  {
//    ofParameter<ofxCortex::core::types::Image> & p = param.cast<ofxCortex::core::types::Image>();
//    views.push_back(Image::create(p));
//  }
//  else if (type == typeid(ofParameter<ofxCortex::core::types::File>).name())
//  {
//    ofParameter<ofxCortex::core::types::File> & p = param.cast<ofxCortex::core::types::File>();
//    views.push_back(FileView::create(p));
//  }
  else
  {
    string prettyType = type;
    ofStringReplace(prettyType, "11", "");
    ofStringReplace(prettyType, "IN9", "<");
    ofStringReplace(prettyType, "4", "::");
    ofStringReplace(prettyType, "5", "::");
    ofStringReplace(prettyType, "EE", ">");
    ofStringReplace(prettyType, "I", "<");
    ofStringReplace(prettyType, "E", ">");
    ofLogNotice("⚠️ ParameterUtils::createViewsFromParameter") << "Implementation for type '" << prettyType << "' is missing. No View created.";
  }
  
  return views;
}

}}
