#pragma once

#include "ofEvents.h"
#include "ofxCortex/graphics/Typography.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/ValueView.h"
#include "ofxCortexUI/components/LabelView.h"
#include "ofxCortex/types/Image.h"
#include "ofxTweenzor.h"

namespace ofxCortex { namespace ui {

class ImageViewer : public ofxCortex::ui::View {
public:
  ImageViewer() : View("Image Viewer")
  {
      ofAddListener(ofEvents().fileDragEvent, this, &ImageViewer::_dragHandler, OF_EVENT_ORDER_AFTER_APP);
  }
  
  ImageViewer(ofParameter<ofxCortex::core::types::Image> param)
  : ImageViewer()
  {
    parameter.makeReferenceTo(param);
  }
  
  static shared_ptr<ImageViewer> create(ofParameter<ofxCortex::core::types::Image> param) {
    auto ptr = make_shared<ImageViewer>(param);
    ptr->_setup();
    return ptr;
  }
  
  ~ImageViewer()
  {
    ofRemoveListener(ofEvents().fileDragEvent, this, &ImageViewer::_dragHandler, OF_EVENT_ORDER_AFTER_APP);
  }
  
  void drawImage()
  {
    if (!parameter->image.isAllocated()) {
      return;
    }
    
    auto RR = this->getContentBounds();
    auto imageRect = ofRectangle(0, 0, parameter->image.getWidth(), parameter->image.getHeight());
    RR.setFromCenter(RR.getCenter(), RR.width - (Styling::getHorizontalPadding()), RR.height - (Styling::getVerticalPadding()));
    
    imageRect.scaleTo(RR, OF_ASPECT_RATIO_KEEP);
    
    parameter->image.draw(imageRect);
  }
  
protected:
  virtual string _getComponentName() const override { return "Image Viewer"; };
  
  void _setup()
  {
    
  }
  
  void _drawOverlay(float alpha)
  {
    auto RR = this->getContentBounds();
    RR.setFromCenter(RR.getCenter(), RR.width - (Styling::getHorizontalPadding()), RR.height - (Styling::getVerticalPadding()));
    RR.scaleFromCenter(1.0 - (0.04 * alpha));
    
//    if (!parameter->image.isAllocated()) alpha = 1.0f;
    
    ofPushStyle();
    ofSetColor(0, 192.0 * alpha);
    ofDrawRectRounded(RR, 4);
    
    
    ofSetColor(255, 255.0 * alpha);
    Styling::drawLabel("Drop or Click to Upload Image..", this->getContentBounds(), OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
//    ofxCortex::core::graphics::Typography::draw(*(this->style->getLabelFont()), "Drop or Click to Upload Image..", RR.getCenter(), 8, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
    ofPopStyle();
  }
  
  virtual void onDraw() override
  {
//    background->drawBackground();
    Styling::drawBackground(this->getBounds(), View::getMouseState());
    this->drawImage();
    this->_drawOverlay(_overlayOpacity);
  }
  
  virtual void onMouseEnter(const MouseEventArgs & e) override
  {
    Tweenzor::add(&_overlayOpacity, _overlayOpacity, 1.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
  }
  
  virtual void onMousePressed(const MouseEventArgs & e) override
  {
    auto result = ofSystemLoadDialog("Image Path");
    if (result.bSuccess) {
      ofxCortex::core::types::Image nextImage(result.filePath);
      parameter = nextImage;
      
      
      Tweenzor::add(&_overlayOpacity, _overlayOpacity, 0.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
    }
  }
  
  virtual void onMouseExit(const MouseEventArgs & e) override
  {
    Tweenzor::add(&_overlayOpacity, _overlayOpacity, 0.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
  }
  
  void _dragHandler(ofDragInfo & e)
  {
      _dragOver(e);
  }
  
  virtual void _dragOver(const ofDragInfo & e)
  {
    ofxCortex::core::types::Image nextImage(e.files[0]);
    parameter = nextImage;
    
    Tweenzor::add(&_overlayOpacity, _overlayOpacity, 0.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
  }
  
  // Members
  ofParameter<ofxCortex::core::types::Image> parameter;
  float _overlayOpacity { 0.0f };
};

class Image : public ofxCortex::ui::View {
protected:
  Image(ofParameter<ofxCortex::core::types::Image> param) : View(param.getName())
  {
    parameter.makeReferenceTo(param);
  }
  
public:
  template<typename ... T>
  static std::shared_ptr<Image> create(T&& ... t) {
    struct EnableMakeShared : public Image { EnableMakeShared(T&&... arg) : Image(std::forward<T>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<T>(t)...);
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
protected:
  virtual std::string _getComponentName() const override { return "Image"; };
  
  virtual void viewDidLoad() override
  {
    heading = ui::ValueView<ofxCortex::core::types::Image>::create(parameter);
    heading->setName(getName() + "::Value");
    heading->disableEvents();
    this->addSubview(heading);
    
    imageViewer = ImageViewer::create(parameter);
    imageViewer->setName(getName() + "::Image Viewer");
    this->addSubview(imageViewer);
    
    this->disableSubviewRendering();
  }
  
  virtual void onDraw() override
  {
    heading->draw();
    imageViewer->draw();
  }
  
  // Members
  ofParameter<ofxCortex::core::types::Image> parameter;
  ofEventListener onParameterTrigger;
  
  std::shared_ptr<ui::ValueView<ofxCortex::core::types::Image>> heading;
  std::shared_ptr<ui::ImageViewer> imageViewer;
};

}}
