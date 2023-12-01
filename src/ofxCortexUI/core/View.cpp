#include "View.h"

namespace ofxCortex { namespace ui {

shared_ptr<View> View::_focused;

View::View(string name)
{
  setName(name);
  style = Style::create();
  
  setWidth(200);
  setHeight(40);
  
  _adjustLayout();
}

void View::setStyle(shared_ptr<Style> style, bool affectChildren)
{
  this->style = style;
  
  if (affectChildren)
  {
    for (auto & child : children<View>())
    {
      child->setStyle(style, affectChildren);
    }
  }
}

View* View::setWidthWithEvent(float width, bool adjustLayout)
{
  DisplayObject::setWidth(width, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

View* View::setHeightWithEvent(float width, bool adjustLayout)
{
  DisplayObject::setHeight(width, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

View* View::setSizeWithEvent(glm::vec2 size, bool adjustLayout)
{
  DisplayObject::setSize(size, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

View* View::setSizeWithEvent(float width, float height, bool adjustLayout)
{
  DisplayObject::setSize(width, height, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

View* View::setRectWithEvent(const ofRectangle & rect, bool adjustLayout)
{
  DisplayObject::setRect(rect, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

void View::setFocused()
{
  if (_isFocusEnabled && !hasFocus()) // We are not currently focused
  {
    _focused->removeFocus();
    _focused = dynamic_pointer_cast<View>(shared_from_this());
  }
}

void View::removeFocus()
{
  if (hasFocus())
  {
    _focused.reset();
  }
}

void View::_draw()
{
  _drawFocusOutline();
}

void View::_drawFocusOutline()
{
  if (!hasFocus()) return;
  
  auto & rect = getRenderRect();
  ofPushStyle();
  {
    ofNoFill();
    ofSetColor(ofFloatColor(style->accentColor, 0.35));
    ofDrawRectRounded(rect, style->boxRadius.topLeft, style->boxRadius.topRight, style->boxRadius.bottomRight, style->boxRadius.bottomLeft);
  }
  ofPopStyle();
}

void View::_update(float time, float delta)
{
  DisplayObject::_update(time, delta);
}

void View::_adjustLayout()
{
  _recalculateRenderRect();
  
  DisplayObject::_adjustLayout();
}

void View::_debug()
{
  DisplayObject::_debug();
  
  ofSetColor(ofColor::springGreen, 128);
  ofDrawRectangle(_renderRect);
}

void View::_recalculateRenderRect()
{
  _renderRect.x = _localRect.getLeft() + style->padding.left;
  _renderRect.y = _localRect.getTop() + style->padding.top;
  _renderRect.width = _localRect.width - style->padding.left - style->padding.right;
  _renderRect.height = _localRect.height - style->padding.top - style->padding.bottom;
}

void View::_mousePressed(const ofMouseEventArgs & e)
{
  setFocused();
}

void View::_onResize(const ofRectangle &rect)
{
  ofLogVerbose(_getLogModule()) << "On Resize! " << rect;
}

}}
