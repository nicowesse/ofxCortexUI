#include "StackView.h"

namespace ofxCortex { namespace ui {

void StackView::Wrapper::_drawChildren()
{
  if (auto parent = getParent())
  {
    for (auto & child : children())
    {
      if (parent->isInsideRectangle(child->getGlobalRect(), true)) child->draw();
    }
  }
}

void StackView::addView(shared_ptr<View> view, unsigned int index)
{
  index = ofClamp(index, 0, _subviews.size());
//  component->setStyle(this->style);
  _subviews.insert(_subviews.begin() + index, view);
  wrapper->addChild(view);
  
  _adjustLayout();
}

void StackView::addView(shared_ptr<View> view)
{
  addView(view, _subviews.size());
}

void StackView::addViews(vector<shared_ptr<View>> views)
{
  for (auto & view : views) addView(view);
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
  backgroundStyle->boxRadius += this->style->margin.top;
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
  background->drawBackground(style->containerColor);
  wrapper->draw();
}

void StackView::_adjustLayout()
{
  glm::vec2 pos = style->margin;
  float innerWidth = getWidth() - style->margin.left - style->margin.right;
  
  for (auto & component : _subviews)
  {
    component->setWidth(innerWidth)->setPosition(pos);
    
    pos.y += component->getHeight() + style->margin.bottom;
  }
  
  background->setRect(this->getRect());
  wrapper->setSize({ this->getWidth(), _getComponentsHeight() });
}

void StackView::_mouseScrolled(const ofMouseEventArgs & e)
{
  View::_mouseScrolled(e);
  
  if (!(wrapper->getHeight() > this->getHeight())) return;
  float diff = wrapper->getHeight() - this->getHeight();
  float potentialY = wrapper->getY() + e.scrollY;
  
//  cout << "Mouse scrolled: " << e.scrollY << " Diff: " << diff << " Current Y: " <<  wrapper->getY() << " Potential Y: " << potentialY << endl;
  
  if (potentialY > -diff && potentialY <= 0.0f) wrapper->translate({ 0.0f, e.scrollY });
}

float StackView::_getComponentsHeight()
{
  float h = style->margin.top;
  
  if (_subviews.size() > 0)
  {
    h += _subviews.back()->getBottom() + style->margin.bottom;
  }
  
  return h;
}


}}
