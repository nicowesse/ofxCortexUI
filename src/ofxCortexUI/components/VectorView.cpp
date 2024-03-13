#include "VectorView.h"

namespace ofxCortex { namespace ui {

//template<typename T>
//void VectorView<T>::viewDidLoad() 
//{
//  using namespace ofxCortex::ui;
//  
//  label = LabelView::create(ParameterView<T>::getParameter());
//  label->disableBackground();
//  
//  x.set("X", ParameterView<T>::getParameterValue().x, ParameterView<T>::getParameterMin().x, ParameterView<T>::getParameterMax().x);
//  sliderListeners.push(x.newListener([this](float & value){
//    T v = ParameterView<T>::getParameterValue();
//    v.x = value;
//    ParameterView<T>::setParameter(v);
//  }));
//  sliderX = SliderView<float>::create(x);
//  
//  y.set("Y", ParameterView<T>::getParameterValue().y, ParameterView<T>::getParameterMin().y, ParameterView<T>::getParameterMax().y);
//  sliderListeners.push(y.newListener([this](float & value){
//    T v = ParameterView<T>::getParameterValue();
//    v.y = value;
//    ParameterView<T>::setParameter(v);
//  }));
//  sliderY = SliderView<float>::create(y);
//  
//  z.set("Z", ParameterView<T>::getParameterValue().z, ParameterView<T>::getParameterMin().z, ParameterView<T>::getParameterMax().z);
//  sliderListeners.push(z.newListener([this](float & value){
//    T v = ParameterView<T>::getParameterValue();
//    v.z = value;
//    ParameterView<T>::setParameter(v);
//  }));
//  sliderZ = SliderView<float>::create(z);
//  
//  sliderListeners.push(ParameterView<T>::getParameter().newListener([this](T & value) {
//    x.setWithoutEventNotifications(value.x);
//    y.setWithoutEventNotifications(value.y);
//    z.setWithoutEventNotifications(value.z);
//  }));
//  
//  
//  this->addSubviews({ label, sliderX, sliderY, sliderZ });
//}
//
//template<typename T>
//void VectorView<T>::onPreDraw()
//{
//  using namespace ofxCortex::ui;
//  
//  ofPushStyle();
//  {
//    Styling::drawContainerBackground(this->getFrame(), Styling::getContainerColor(), Styling::getBorderColor(Styling::State::IDLE), this->level);
//  }
//  ofPopStyle();
//}

}}
