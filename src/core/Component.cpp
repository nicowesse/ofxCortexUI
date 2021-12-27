#include "Component.h"

namespace ofxCortex { namespace ui {

shared_ptr<Component> Component::_focused;

Component::Component(string name)
{
  setName(name);
  style = Style::create();
  
  setWidth(288);
  setHeight(40);
  
  _adjustLayout();
}

void Component::setStyle(shared_ptr<Style> style, bool affectChildren)
{
  this->style = style;
  
  if (affectChildren)
  {
    for (auto & child : children<Component>())
    {
      child->setStyle(style, affectChildren);
    }
  }
}

Component* Component::setWidthWithEvent(float width, bool adjustLayout)
{
  DisplayObject::setWidth(width, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

Component* Component::setHeightWithEvent(float width, bool adjustLayout)
{
  DisplayObject::setHeight(width, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

Component* Component::setSizeWithEvent(glm::vec2 size, bool adjustLayout)
{
  DisplayObject::setSize(size, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

Component* Component::setSizeWithEvent(float width, float height, bool adjustLayout)
{
  DisplayObject::setSize(width, height, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

Component* Component::setRectWithEvent(const ofRectangle & rect, bool adjustLayout)
{
  DisplayObject::setRect(rect, adjustLayout);
  
  ofRectangle r = getRect();
  _onResize(r);
  onResizedE.notify(this, r);
}

void Component::setFocused()
{
  if (_isFocusEnabled && !hasFocus()) // We are not currently focused
  {
    _focused->removeFocus();
    _focused = dynamic_pointer_cast<Component>(shared_from_this());
  }
}

void Component::removeFocus()
{
  if (hasFocus())
  {
    _focused.reset();
  }
}

void Component::_draw()
{
  _drawFocusOutline();
}

void Component::_drawFocusOutline()
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

void Component::_update()
{
  
}

void Component::_adjustLayout()
{
  _recalculateRenderRect();
  
  DisplayObject::_adjustLayout();
}

void Component::_debug()
{
  DisplayObject::_debug();
  
  ofSetColor(ofColor::springGreen, 128);
  ofDrawRectangle(_renderRect);
}

void Component::_recalculateRenderRect()
{
  _renderRect.x = _localRect.getLeft() + style->padding.left;
  _renderRect.y = _localRect.getTop() + style->padding.top;
  _renderRect.width = _localRect.width - style->padding.left - style->padding.right;
  _renderRect.height = _localRect.height - style->padding.top - style->padding.bottom;
}

void Component::_mousePressed(const ofMouseEventArgs & e)
{
  setFocused();
}

void Component::_onResize(const ofRectangle &rect)
{
  ofLogVerbose(_getLogModule()) << "On Resize! " << rect;
}

}}
