#pragma mark

#include "ofEvents.h"
#include "ofxCortexUI/core/View.h"
#include "ofxCortexUI/components/Value.h"
#include "ofxCortexUI/components/Label.h"
#include "ofxCortex/types/Image.h"
#include "ofxTweenzor.h"

namespace ofxCortex { namespace ui {

class ImagewViewer;

//class ImageViewer : public ofxCortex::ui::View {
//public:
//  ImageViewer()
//  {
//      ofAddListener(ofEvents().fileDragEvent, this, &ImageViewer::_dragHandler, OF_EVENT_ORDER_AFTER_APP);
//  }
//  
//  ImageViewer(ofParameter<ofxCortex::core::types::Image> param)
//  : ImageViewer()
//  {
//    parameter.makeReferenceTo(param);
//  }
//  
//  static shared_ptr<ImageViewer> create(ofParameter<ofxCortex::core::types::Image> param) {
//    auto ptr = make_shared<ImageViewer>(param);
//    ptr->_init();
//    return ptr;
//  }
//  
//  ~ImageViewer()
//  {
//    ofRemoveListener(ofEvents().fileDragEvent, this, &ImageViewer::_dragHandler, OF_EVENT_ORDER_AFTER_APP);
//  }
//  
//  void drawImage()
//  {
//    auto RR = this->getRenderRect();
//    RR.setFromCenter(RR.getCenter(), RR.width - (style->padding.left + style->padding.right), RR.height - (style->padding.top + style->padding.bottom));
//    parameter->image.draw(RR);
//  }
//  
//protected:
//  virtual string _getModule() const override { return "Image Viewer"; };
//  
//  void _init()
//  {
//    background = ofxCortex::ui::Background::create("ImageContainer::Background");
//    background->disableEvents();
//  }
//  
//  void _drawOverlay(float alpha)
//  {
//    auto RR = this->getRenderRect();
//    RR.setFromCenter(RR.getCenter(), RR.width - (style->padding.left + style->padding.right), RR.height - (style->padding.top + style->padding.bottom));
//    
//    ofPushStyle();
//    ofSetColor(255, 64.0 * alpha);
//    ofDrawRectangle(RR);
//    ofPopStyle();
//  }
//  
//  virtual void _draw() override
//  {
//    background->drawBackground();
//    this->drawImage();
//    this->_drawOverlay(_overlayOpacity);
//  }
//  
//  virtual void _adjustLayout() override
//  {
//    float aspectRatio = parameter->image.getWidth() / parameter->image.getHeight();
//    this->setHeight(this->getWidth() / aspectRatio, false);
//    
//    background->setRect(this->getRect());
//    
//    View::_adjustLayout();
//  }
//  
//  virtual void _mouseEnter(const ofMouseEventArgs & e) override
//  {
//    Tweenzor::add(&_overlayOpacity, _overlayOpacity, 1.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
//  }
//  
//  virtual void _mouseExit(const ofMouseEventArgs & e) override
//  {
//    Tweenzor::add(&_overlayOpacity, _overlayOpacity, 0.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
//  }
//  
//  void _dragHandler(ofDragInfo & e)
//  {
//    if (ofGetCurrentViewport().inside(e.position))
//    {
//      _isMouseDown = true;
//      
//      bool isInsideRect = isInsideRectangle(e.position, true);
//      bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e.position, true) : false;
//      bool isInteractionOutsideParentAllowed = true;
//      bool isCovered = this->_isCovered(e.position);
//      
//      if (isInsideRect)
//      {
//        if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
//        else _dragOver(e);
//      }
//    }
//  }
//  
//  virtual void _dragOver(const ofDragInfo & e)
//  {
//    ofxCortex::core::types::Image nextImage(e.files[0]);
//    parameter = nextImage;
//    
//    _adjustLayout();
//  }
//  
//  // Members
//  shared_ptr<ofxCortex::ui::Background> background;
//  
//  ofParameter<ofxCortex::core::types::Image> parameter;
//  float _overlayOpacity { 0.0f };
//};

class Image;

//class Image : public ofxCortex::ui::View {
//public:
//  Image(ofParameter<ofxCortex::core::types::Image> param)
//  {
//    setName(param.getName());
//    parameter.makeReferenceTo(param);
//    
//    this->disableChildRendering();
//  }
//  
//  static shared_ptr<Image> create(ofParameter<ofxCortex::core::types::Image> param) {
//    auto ptr = make_shared<Image>(param);
//    ptr->_init();
//    return ptr;
//  };
//  
//protected:
//  virtual string _getModule() const override { return "Image"; };
//  
//  void _init()
//  {
//    heading = ui::Value<ofxCortex::core::types::Image>::create(parameter);
//    heading->setName("Image::Label");
//    heading->disableEvents();
//    this->addChild(heading);
//    
//    imageContainer = ImageViewer::create(parameter);
//    imageContainer->setName("Image::Container");
//    this->addChild(imageContainer);
//  }
//  
//  virtual void _draw() override
//  {
//    heading->draw();
//    imageContainer->draw();
//  }
//  
//  virtual void _adjustLayout() override
//  {
//    heading->setWidth(this->getWidth());
//    imageContainer->setWidth(this->getWidth());
//    imageContainer->setTop(heading->getBottom());
//    
//    View::_adjustLayout();
//  }
//  
//  // Members
//  ofParameter<ofxCortex::core::types::Image> parameter;
//  ofEventListener onParameterTrigger;
//  
//  shared_ptr<ui::Value<ofxCortex::core::types::Image>> heading;
//  shared_ptr<ui::ImageViewer> imageContainer;
//};

}}
