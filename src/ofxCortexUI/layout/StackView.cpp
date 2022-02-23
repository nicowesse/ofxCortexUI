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

void StackView::_update(float time, float delta)
{
  scrollOpacity = ofClamp(scrollOpacity - delta * 2.0, 0, 1);
}

void StackView::_draw()
{
  if (_drawBackground) background->drawBackground(style->containerColor, ofFloatColor(1.0f, 0.1f));
  wrapper->draw();
  
  ofPushStyle();
  ofSetColor(255, scrollOpacity * 255.0f);
  if (wrapper->getHeight() > this->getHeight())
  {
    float scrollbarAvailableHeight = this->getRenderRect().height;
    float scrollbarHeight = scrollbarAvailableHeight / (wrapper->getHeight() / this->getHeight());
    float scrollbarDifference = scrollbarAvailableHeight - scrollbarHeight;
    float overflowDifference = wrapper->getHeight() - this->getHeight();
    float offsetY = scrollbarDifference * (abs(wrapper->getY()) / overflowDifference);
    ofDrawRectRounded(this->getRenderRect().getRight() - 2, this->getRenderRect().y + offsetY, 1, scrollbarHeight, 4);
  }
  ofPopStyle();
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
  
  
  
  if (_axis == Layout::VERTICAL)
  {
    if (LayoutHelpers::getSpacedViewsHeight(_subviews, style->padding) > this->getHeight() || _distribution == STACK || _distribution == STACK_CENTER || _distribution == STACK_END) _layoutVerticalStack();
    else if (_distribution == FILL_EQUALLY) _layoutVerticalFillEqually();
    else if (_distribution == SPACE_EQUALLY) _layoutVerticalEqualSpacing();
  }
  else if (_axis == Layout::HORIZONTAL)
  {
    if (LayoutHelpers::getSpacedViewsWidth(_subviews, style->padding) > this->getWidth() || _distribution == STACK || _distribution == STACK_CENTER || _distribution == STACK_END) _layoutHorizontalStack();
    else if (_distribution == FILL_EQUALLY) _layoutHorizontalFillEqually();
    else if (_distribution == SPACE_EQUALLY) _layoutHorizontalEqualSpacing();
  }
  
  background->setRect(this->getRect());
  
  DisplayObject::_adjustLayout();
}

void StackView::_layoutVerticalStack()
{
  glm::vec2 pos = getRenderRect().getPosition();
  float innerHeight = getRenderRect().height;
  
  float offsetY = 0.0f;
  if (LayoutHelpers::getSpacedViewsHeight(_subviews, style->padding) < innerHeight)
  {
    if (_distribution == STACK_CENTER) offsetY = (innerHeight - LayoutHelpers::getViewsHeight(_subviews)) * 0.5f;
    else if (_distribution == STACK_END) offsetY = innerHeight - LayoutHelpers::getViewsHeight(_subviews);
  }
  
  pos.y += offsetY;
  
  for (auto & view : _subviews)
  {
    _alignVertical(view, pos);
    
    pos.y += view->getHeight() + view->getStyle()->margin.bottom;
  }
  
  wrapper->setSize({ this->getWidth(), LayoutHelpers::getSpacedViewsHeight(_subviews, style->padding) });
}

void StackView::_layoutHorizontalStack()
{
  glm::vec2 pos = getRenderRect().getPosition();
  float innerWidth = getRenderRect().width;
  
  if (LayoutHelpers::getSpacedViewsWidth(_subviews, style->padding) < innerWidth)
  {
    if (_distribution == STACK_CENTER) pos.x += (innerWidth - LayoutHelpers::getSpacedViewsWidth(_subviews, style->padding)) * 0.5f;
    else if (_distribution == STACK_END) pos.x += innerWidth - LayoutHelpers::getViewsWidth(_subviews);
  }
  
  for (auto & view : _subviews)
  {
    _alignHorizontal(view, pos);
    
    pos.x += view->getWidth() + view->getStyle()->margin.right;
  }
  
  wrapper->setSize({ LayoutHelpers::getSpacedViewsWidth(_subviews, style->padding), this->getHeight() });
}

void StackView::_layoutVerticalFillEqually()
{
  glm::vec2 pos = style->padding;
  float viewHeight = getRenderRect().height / _subviews.size();
  
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
  float viewWidth = getRenderRect().width / _subviews.size();
  
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
  float innerWidth = getRenderRect().width;
  float innerHeight = getRenderRect().height;
  
  float spacing = (innerHeight - LayoutHelpers::getViewsHeight(_subviews)) / (_subviews.size() - 1);
  
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
  float innerHeight = getRenderRect().height;
  
  float spacing = (getRenderRect().width - LayoutHelpers::getViewsWidth(_subviews)) / (_subviews.size() - 1);
  
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
  scrollOpacity = ofClamp(scrollOpacity + abs(e.scrollY), 0, 1);
  
  // Scroll X
  float diffX = wrapper->getWidth() - this->getWidth();
  float potentialX = wrapper->getX() + e.scrollX;
  
  if (potentialX > -diffX && potentialX <= 0.0f) wrapper->translate({ e.scrollX, 0.0f });
}

void StackView::_alignVertical(shared_ptr<View> view, const glm::vec2 & pos)
{
  if (_alignment == Layout::FILL) { view->setPosition(pos)->setWidth(getRenderRect().width); }
  else if (_alignment == Layout::ALIGN_START) { view->setPosition(pos); }
  else if (_alignment == Layout::ALIGN_END) { view->setY(pos.y)->alignRight(getRenderRect().getRight()); }
  else if (_alignment == Layout::ALIGN_CENTER) { view->setY(pos.y)->centerX(getRenderRect().getCenter().x); }
}

void StackView::_alignHorizontal(shared_ptr<View> view, const glm::vec2 & pos)
{
  if (_alignment == Layout::FILL) { view->setPosition(pos)->setHeight(getRenderRect().height); }
  else if (_alignment == Layout::ALIGN_START) { view->setPosition(pos); }
  else if (_alignment == Layout::ALIGN_END) { view->setX(pos.x)->alignBottom(getRenderRect().getBottom()); }
  else if (_alignment == Layout::ALIGN_CENTER) { view->setX(pos.x)->centerY(getRenderRect().getCenter().y); }
}

}}
