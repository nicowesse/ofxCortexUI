#include "StackView.h"

namespace ofxCortex { namespace ui {

void StackView::Wrapper::_drawChildren()
{
  if (auto parent = getParent())
  {
    for (auto & child : children())
    {
//      if (parent->isInsideRectangle(child->getGlobalRect(), true))
      if (child->getGlobalRect().getTop() <= parent->getGlobalRect().getBottom() && child->getGlobalRect().getBottom() >= parent->getGlobalRect().getTop()) child->draw();
    }
  }
}

void StackView::addViewAt(shared_ptr<View> view, unsigned int index, bool adjustLayout)
{
  index = ofClamp(index, 0, _subviews.size());
  view->setStyle(this->style);
  _subviews.insert(_subviews.begin() + index, view);
  wrapper->addChild(view);
  
  if (adjustLayout) _adjustLayout();
}

void StackView::addView(shared_ptr<View> view, bool adjustLayout)
{
  addViewAt(view, _subviews.size(), adjustLayout);
}

void StackView::addViews(vector<shared_ptr<View>> views, bool adjustLayout)
{
  for (auto & view : views) addView(view, false);
  
  if (adjustLayout) _adjustLayout();
}

void StackView::removeView(shared_ptr<View> view)
{
  _subviews.erase(std::remove(begin(_subviews), end(_subviews), view), end(_subviews));
  
  _adjustLayout();
}

void StackView::removeAllViews()
{
  while (_subviews.size()) removeView(_subviews.front());
}

void StackView::_init()
{
  background = ofxCortex::ui::Background::create();
  auto backgroundStyle = ofxCortex::ui::Style::copy(this->style);
  backgroundStyle->boxRadius += this->style->padding.top;
  background->setStyle(backgroundStyle);
  background->setName(getName() + "::Background");
  background->disableEvents();
  addChild(background);
  
  wrapper = make_shared<Wrapper>();
  wrapper->disableEvents();
  addChild(wrapper);
}

void StackView::_draw()
{
  background->drawBackground(style->containerColor, ofFloatColor(1.0f, 0.1f));
  wrapper->draw();
}

void StackView::_drawMask()
{
  background->drawBackground(ofFloatColor::white);
}

void StackView::_debug()
{
  _drawMask();
  
  View::_debug();
}

void StackView::_adjustLayout()
{
  View::_recalculateRenderRect();
  
  if (_axis == VERTICAL)
  {
    if (_getViewsHeight() > this->getHeight() || _distribution == STACK || _distribution == STACK_CENTER || _distribution == STACK_END) _layoutVerticalStack();
    else if (_distribution == FILL_EQUALLY) _layoutVerticalFillEqually();
    else if (_distribution == SPACE_EQUALLY) _layoutVerticalEqualSpacing();
  }
  else if (_axis == HORIZONTAL)
  {
    if (_getViewsWidth() > this->getWidth() || _distribution == STACK || _distribution == STACK_CENTER || _distribution == STACK_END) _layoutHorizontalStack();
    else if (_distribution == FILL_EQUALLY) _layoutHorizontalFillEqually();
    else if (_distribution == SPACE_EQUALLY) _layoutHorizontalEqualSpacing();
  }
  
  background->setRect(this->getRect());
  
  DisplayObject::_adjustLayout();
}

void StackView::_layoutVerticalStack()
{
  glm::vec2 pos = getRenderRect().getPosition();
  
  float offsetY = 0.0f;
  if (_getViewsHeight() < _getInnerHeight())
  {
    if (_distribution == STACK_CENTER) offsetY = (_getInnerHeight() - _getViewsHeight()) * 0.5f;
    else if (_distribution == STACK_END) offsetY = _getInnerHeight() - _getViewsHeight(false);
  }
  
  pos.y += offsetY;
  
  for (auto & view : _subviews)
  {
    _alignVertical(view, pos);
    
    pos.y += view->getHeight() + view->getStyle()->margin.bottom;
  }
  
  wrapper->setSize({ this->getWidth(), _getViewsHeight() });
}

void StackView::_layoutHorizontalStack()
{
  glm::vec2 pos = getRenderRect().getPosition();
  
  float offsetX = 0.0f;
  if (_getViewsWidth() < _getInnerWidth())
  {
    if (_distribution == STACK_CENTER) offsetX = (_getInnerWidth() - _getViewsWidth()) * 0.5f;
    else if (_distribution == STACK_END) offsetX = _getInnerWidth() - _getViewsWidth(false);
  }
  
  pos.x += offsetX;
  
  for (auto & view : _subviews)
  {
    _alignHorizontal(view, pos);
    
    pos.x += view->getWidth() + view->getStyle()->margin.right;
  }
  
  wrapper->setSize({ _getViewsWidth(), this->getHeight() });
}

void StackView::_layoutVerticalFillEqually()
{
  glm::vec2 pos = style->padding;
  float viewHeight = _getInnerHeight() / _subviews.size();
  
  for (auto & view : _subviews)
  {
    view->setHeight(viewHeight);
    _alignVertical(view, pos);
    
    pos.y += view->getHeight();
  }
  
  wrapper->setSize({ this->getWidth(), pos.y });
}

void StackView::_layoutHorizontalFillEqually()
{
  glm::vec2 pos = style->padding;
  float viewWidth = _getInnerWidth() / _subviews.size();
  
  for (auto & view : _subviews)
  {
    view->setWidth(viewWidth);
    _alignHorizontal(view, pos);
    
    pos.x += view->getWidth() + view->getStyle()->margin.right;
  }
  
  wrapper->setSize({ this->getWidth(), pos.y });
}

void StackView::_layoutVerticalEqualSpacing()
{
  glm::vec2 pos = style->padding;
  float innerWidth = _getInnerWidth();
  float innerHeight = _getInnerHeight();
  
  float spacing = (innerHeight - _getViewsHeight(false)) / (_subviews.size() - 1);
  
  for (auto & view : _subviews)
  {
    _alignVertical(view, pos);
    
    pos.y += view->getHeight() + spacing;
  }
  
  float height = pos.y - (_subviews.back()->getHeight() + spacing);
  
  wrapper->setSize({ this->getWidth(), height });
}

void StackView::_layoutHorizontalEqualSpacing()
{
  glm::vec2 pos = style->padding;
  float innerHeight = _getInnerHeight();
  
  float spacing = (_getInnerWidth() - _getViewsWidth(false)) / (_subviews.size() - 1);
  
  for (auto & view : _subviews)
  {
    _alignHorizontal(view, pos);
    
    pos.x += view->getWidth() + spacing;
  }
  
  float width = pos.x - (_subviews.back()->getWidth() + spacing);
  
  wrapper->setSize({ width, this->getHeight() });
}

void StackView::_mouseScrolled(const ofMouseEventArgs & e)
{
  View::_mouseScrolled(e);
  
  if (wrapper->getHeight() <= this->getHeight() && wrapper->getWidth() <= this->getWidth()) return;
  
  // Scroll Y
  float diffY = wrapper->getHeight() - this->getHeight();
  float potentialY = wrapper->getY() + e.scrollY;
  
  if (potentialY > -diffY && potentialY <= 0.0f) wrapper->translate({ 0.0f, e.scrollY });
  
  // Scroll X
  float diffX = wrapper->getWidth() - this->getWidth();
  float potentialX = wrapper->getX() + e.scrollX;
  
  if (potentialX > -diffX && potentialX <= 0.0f) wrapper->translate({ e.scrollX, 0.0f });
}

float StackView::_getViewsHeight(bool includeSpacing)
{
  float h = style->padding.top * includeSpacing;
  
  for (const auto & view : _subviews) h += view->getHeight() + (style->margin.bottom * includeSpacing);
  h += style->padding.bottom * includeSpacing;
  
  return h;
}

float StackView::_getViewsWidth(bool includeSpacing)
{
  float w = style->padding.left * includeSpacing;
  
  for (const auto & view : _subviews) w += view->getWidth() + (style->margin.right * includeSpacing);
  w += style->padding.right * includeSpacing;
  
  return w;
}

float StackView::_getInnerWidth()
{
  return getRenderRect().width;
//  return getWidth() - style->padding.left - style->padding.right;
}

float StackView::_getInnerHeight()
{
  return getRenderRect().height;
//  return getHeight() - style->padding.top - style->padding.bottom;
}

void StackView::_alignVertical(shared_ptr<View> view, const glm::vec2 & pos)
{
  if (_alignment == FILL) { view->setPosition(pos)->setWidth(_getInnerWidth()); }
  else if (_alignment == ALIGN_START) { view->setPosition(pos); }
  else if (_alignment == ALIGN_END) { view->setY(pos.y)->alignRight(getRenderRect().getRight()); }
  else if (_alignment == ALIGN_CENTER) { view->setY(pos.y)->centerX(getRenderRect().getCenter().x); }
}

void StackView::_alignHorizontal(shared_ptr<View> view, const glm::vec2 & pos)
{
  if (_alignment == FILL) { view->setPosition(pos)->setHeight(_getInnerHeight()); }
  else if (_alignment == ALIGN_START) { view->setPosition(pos); }
  else if (_alignment == ALIGN_END) { view->setX(pos.x)->alignBottom(getRenderRect().getBottom()); }
  else if (_alignment == ALIGN_CENTER) { view->setX(pos.x)->centerY(getRenderRect().getCenter().y); }
}

}}
