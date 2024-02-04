#pragma once

#include "ofxCortex/utils/Helpers.h"
#include "ofxCortex/types/File.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/ValueView.h"
#include "ofxCortexUI/components/ButtonView.h"

namespace ofxCortex { namespace ui {

class FileView : public ofxCortex::ui::View {
protected:
  FileView(ofParameter<ofxCortex::core::types::File> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  }
  
public:
  
  
  template<typename ... F>
  static std::shared_ptr<FileView> create(F&& ... f) {
    struct EnableMakeShared : public FileView { EnableMakeShared(F&&... arg) : FileView(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  
  virtual string _getComponentName() const override { return "FileView"; };
  
  virtual void viewDidLoad() override
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
  
  virtual void onDraw() override
  {
    fileValue->draw();
    button->draw();
  }
  
//  virtual void updateConstraints() override
//  {
//    View::updateConstraints();
//  }
  
  // Members
  ofParameter<ofxCortex::core::types::File> parameter;
  ofEventListener onParameterTrigger;
  
  std::shared_ptr<ui::ValueView<ofxCortex::core::types::File>> fileValue;
  std::shared_ptr<ui::ButtonView> button;
};

}}
