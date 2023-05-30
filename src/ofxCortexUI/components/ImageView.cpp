#include "ImageView.h"

namespace ofxCortex { namespace ui {

#pragma mark - Image Viewer

//ImageViewer::ImageViewer()
//{
//    ofAddListener(ofEvents().fileDragEvent, this, &ImageViewer::_dragHandler, OF_EVENT_ORDER_AFTER_APP);
//}
//
//ImageViewer::ImageViewer(ofParameter<ofxCortex::core::types::Image> param) : ImageViewer()
//{
//  parameter.makeReferenceTo(param);
//}

//void ImageViewer::drawImage()
//{
//  auto RR = this->getRenderRect();
//  RR.setFromCenter(RR.getCenter(), RR.width - (style->padding.left + style->padding.right), RR.height - (style->padding.top + style->padding.bottom));
//  parameter->image.draw(RR);
//}

//void ImageViewer::_init()
//{
//  background = ofxCortex::ui::Background::create("ImageContainer::Background");
//  background->disableEvents();
//}

//void ImageViewer::_drawOverlay(float alpha)
//{
//  auto RR = this->getRenderRect();
//  RR.setFromCenter(RR.getCenter(), RR.width - (style->padding.left + style->padding.right), RR.height - (style->padding.top + style->padding.bottom));
//
//  ofPushStyle();
//  ofSetColor(255, 64.0 * alpha);
//  ofDrawRectangle(RR);
//  ofPopStyle();
//}

//void ImageViewer::_draw()
//{
//  background->drawBackground();
//  this->drawImage();
//  this->_drawOverlay(_overlayOpacity);
//}

//void ImageViewer::_adjustLayout()
//{
//  float aspectRatio = parameter->image.getWidth() / parameter->image.getHeight();
//  this->setHeight(this->getWidth() / aspectRatio, false);
//
//  background->setRect(this->getRect());
//
//  View::_adjustLayout();
//}

//void ImageViewer::_mouseEnter(const ofMouseEventArgs & e)
//{
//  Tweenzor::add(&_overlayOpacity, _overlayOpacity, 1.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
//}

//void ImageViewer::_mouseExit(const ofMouseEventArgs & e)
//{
//  Tweenzor::add(&_overlayOpacity, _overlayOpacity, 0.0f, 0.0f, 0.25f, EASE_IN_OUT_QUART);
//}

//void ImageViewer::_dragHandler(ofDragInfo & e)
//{
//  if (ofGetCurrentViewport().inside(e.position))
//  {
//    _isMouseDown = true;
//
//    bool isInsideRect = isInsideRectangle(e.position, true);
//    bool isInsideParent = (hasParent()) ? getParent()->isInsideRectangle(e.position, true) : false;
//    bool isInteractionOutsideParentAllowed = true;
//    bool isCovered = this->_isCovered(e.position);
//
//    if (isInsideRect)
//    {
//      if (!isInsideParent && !isInteractionOutsideParentAllowed) {}
//      else _dragOver(e);
//    }
//  }
//}

//void ImageViewer::_dragOver(const ofDragInfo & e)
//{
//  ofxCortex::core::types::Image nextImage(e.files[0]);
//  parameter = nextImage;
//
//  _adjustLayout();
//}


#pragma mark - Image

//void Image::_init()
//{
//  heading = ui::Value<ofxCortex::core::types::Image>::create(parameter);
//  heading->setName("Image::Label");
//  heading->disableEvents();
//  this->addChild(heading);
//
//  imageContainer = ImageViewer::create(parameter);
//  imageContainer->setName("Image::Container");
//  this->addChild(imageContainer);
//}

//void Image::_draw() override
//{
//  heading->draw();
//  imageContainer->draw();
//}

//void Image::_adjustLayout() override
//{
//  heading->setWidth(this->getWidth());
//  imageContainer->setWidth(this->getWidth());
//  imageContainer->setTop(heading->getBottom());
//  
//  View::_adjustLayout();
//}


}}
