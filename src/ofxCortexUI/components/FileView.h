#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortex/types/File.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/ValueView.h"
#include "ofxCortexUI/components/ButtonView.h"

namespace ofxCortex { namespace ui {

class FileView : public ofxCortex::ui::View {
public:
  FileView(ofParameter<ofxCortex::core::types::File> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  }
  
  static std::shared_ptr<FileView> create(ofParameter<ofxCortex::core::types::File> param) {
    auto instance = make_shared<FileView>(param);
    instance->_setup();
    return instance;
  };
  
protected:
  
  virtual string _getComponentName() const override { return "FileView"; };
  
  void _setup()
  {
    fileValue = ui::ValueView<ofxCortex::core::types::File>::create(parameter);
    fileValue->setName(getName() + "::Value");
    fileValue->disableEvents();
    this->addSubview(fileValue);
    
    button = ButtonView::create("...", [this](){
      auto result = ofSystemLoadDialog("Select File...", false, parameter->path);
      if (result.bSuccess) parameter.set(ofxCortex::core::types::File(result.filePath));
    });
    button->setName(getName() + "::Button");
    this->addSubview(button);
    
    this->disableSubviewRendering();
  }
  
  virtual void _draw() override
  {
    fileValue->draw();
    button->draw();
  }
  
  virtual void _updateConstraints() override
  {
    View::_updateConstraints();
    
    this->addConstraints({
      { }
    });
  }
  
  // Members
  ofParameter<ofxCortex::core::types::File> parameter;
  ofEventListener onParameterTrigger;
  
  std::shared_ptr<ui::ValueView<ofxCortex::core::types::File>> fileValue;
  std::shared_ptr<ui::ButtonView> button;
};

}}
