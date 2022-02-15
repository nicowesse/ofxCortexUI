#include "FitView.h"

namespace ofxCortex { namespace ui {

void FitView::addViewAt(shared_ptr<View> view, unsigned int index, bool adjustLayout)
{
  index = ofClamp(index, 0, _subviews.size());
  view->setStyle(this->style);
  _subviews.insert(_subviews.begin() + index, view);
  this->addChild(view);
  
  if (adjustLayout) _adjustLayout();
}

void FitView::addView(shared_ptr<View> view, bool adjustLayout)
{
  addViewAt(view, _subviews.size(), adjustLayout);
}

void FitView::addViews(vector<shared_ptr<View>> views, bool adjustLayout)
{
  for (auto & view : views) addView(view, false);
  
  if (adjustLayout) _adjustLayout();
}

void FitView::removeView(shared_ptr<View> view)
{
  _subviews.erase(std::remove(begin(_subviews), end(_subviews), view), end(_subviews));
  
  _adjustLayout();
}

void FitView::removeAllViews()
{
  while (_subviews.size()) removeView(_subviews.front());
}

void FitView::_init()
{
  background = ofxCortex::ui::Background::create();
  auto backgroundStyle = ofxCortex::ui::Style::copy(this->style);
  backgroundStyle->boxRadius += this->style->padding.top;
  background->setStyle(backgroundStyle);
  background->setName(getName() + "::Background");
  background->disableEvents();
  addChild(background);
}

void FitView::_draw()
{
  background->drawBackground(style->containerColor, ofFloatColor(1.0f, 0.1f));
}

void FitView::_adjustLayout()
{
  View::_recalculateRenderRect();
  
  setHeight(_getViewsHeight(), false);
  
  glm::vec2 pos = getRenderRect().getPosition();
  
  for (auto & view : _subviews)
  {
    view->setPosition(pos)->setWidth(getRenderRect().width);
    
    pos.y += view->getHeight() + view->getStyle()->margin.bottom;
  }
  
  DisplayObject::_adjustLayout();
}

float FitView::_getViewsHeight(bool includeSpacing)
{
  float h = style->padding.top * includeSpacing;
  
  for (const auto & view : _subviews) h += view->getHeight() + (style->margin.bottom * includeSpacing);
  h += style->padding.bottom * includeSpacing;
  
  return h;
}

}}
