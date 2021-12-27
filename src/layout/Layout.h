#pragma once

#include "core/Component.h"
#include "components/Background.h"

namespace ofxCortex { namespace ui {

class Layout : public ofxCortex::ui::Component {
  class Wrapper : public DisplayObject {
  protected:
//    virtual void _drawChildren() override {};
  };
  
public:
  Layout(ofRectangle rect = ofRectangle(0, 0, 200, 400)) : Component("Layout") {
    this->setRect(rect, false);
  }
  
  static shared_ptr<Layout> create(ofRectangle rect = ofRectangle(0, 0, 200, 400)) {
    auto ptr = make_shared<Layout>(rect);
    ptr->_init();
    return ptr;
  }
  
#pragma mark - Family Releations
  virtual void addComponent(shared_ptr<Component> component);
  virtual void addComponentAt(shared_ptr<Component> component, unsigned int index);
  virtual void addComponents(vector<shared_ptr<Component>> components);
  
  virtual void removeComponent(shared_ptr<Component> component);
  virtual void removeAllComponents();
  
  
protected:
  void _init();
  virtual void _draw() override;
  virtual void _adjustLayout() override;
  
  virtual void _mouseScrolled(const ofMouseEventArgs & e) override;
  
  float _getComponentsHeight();
  
  vector<shared_ptr<Component>> _components;
  shared_ptr<Wrapper> wrapper;
  shared_ptr<Background> background;
  
};

}}
