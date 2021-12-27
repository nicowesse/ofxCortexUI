#include "Layout.h"

namespace ofxCortex { namespace ui {

void Layout::addComponentAt(shared_ptr<Component> component, unsigned int index)
{
  index = ofClamp(index, 0, _components.size());
//  component->setStyle(this->style);
  _components.insert(_components.begin() + index, component);
  wrapper->addChild(component);
  
  _adjustLayout();
}

void Layout::addComponent(shared_ptr<Component> component)
{
  addComponentAt(component, _components.size());
}

void Layout::addComponents(vector<shared_ptr<Component>> components)
{
  for (auto & component : components) addComponent(component);
}

void Layout::removeComponent(shared_ptr<Component> component)
{
  _components.erase(std::remove(begin(_components), end(_components), component), end(_components));
  
  _adjustLayout();
}

void Layout::removeAllComponents()
{
  while (_components.size()) removeComponent(_components[0]);
}

void Layout::_init()
{
  background = ofxCortex::ui::Background::create();
  background->setName(getName() + "::Background");
  background->disableEvents();
  addChild(background);
  
  wrapper = make_shared<Wrapper>();
  wrapper->disableEvents();
  addChild(wrapper);
}

void Layout::_draw()
{
  background->drawBackground(ofFloatColor::tomato);
}

void Layout::_adjustLayout()
{
  glm::vec2 pos = style->margin;
  float innerWidth = getWidth() - style->margin.left - style->margin.right;
  
  for (auto & component : _components)
  {
    component->setWidth(innerWidth)->setPosition(pos);
    
    pos.y += component->getHeight() + style->margin.bottom;
  }
  
  wrapper->setSize({ this->getWidth(), _getComponentsHeight() });
  background->setRect(this->getRect());
}

void Layout::_mouseScrolled(const ofMouseEventArgs & e)
{
  Component::_mouseScrolled(e);
  
  if (!(wrapper->getHeight() > this->getHeight())) return;
  float diff = wrapper->getHeight() - this->getHeight();
  float currentY = wrapper->localToGlobal(this->getPosition()).y;
  float potentialY = currentY + e.scrollY;
  
//  cout << "Mouse scrolled: " << e.scrollY << " Diff: " << diff << " Current Y: " <<  currentY << " Potential Y: " << potentialY << endl;
  
  if (potentialY > -diff && potentialY <= 0.0f) wrapper->translate({ 0.0f, e.scrollY });
}

float Layout::_getComponentsHeight()
{
  float h = style->margin.top;
  
  if (_components.size() > 0)
  {
    h += _components.back()->getBottom() + style->margin.bottom;
  }
  
  return h;
}


}}
