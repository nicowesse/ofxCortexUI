#pragma once

#include "ofxCortexUI/core/View.h"

namespace ofxCortex { namespace ui {

struct Layout {
  enum Axis {
    VERTICAL,
    HORIZONTAL
  };
  
  enum Alignment {
    FILL,
    ALIGN_START,
    ALIGN_CENTER,
    ALIGN_END
  };
};

class LayoutHelpers {
public:
  // Metrics
  static float getViewsHeight(const vector<shared_ptr<View>> & views)
  {
    float h = 0.0;
    
    for (const auto & view : views) h += view->getHeight();
    
    return h;
  };
  
  static float getSpacedViewsHeight(const vector<shared_ptr<View>> & views, ofxCortex::ui::Style::Spacing padding)
  {
    float h = padding.top;
    
    for (const auto & view : views) h += view->getHeight() + view->getStyle()->margin.bottom;
    h += padding.bottom;
    
    return h;
  };
  
  static float getViewsWidth(const vector<shared_ptr<View>> & views)
  {
    float w = 0.0;
    
    for (const auto & view : views) w += view->getWidth();
    
    return w;
  };
  
  static float getSpacedViewsWidth(const vector<shared_ptr<View>> & views, const ofxCortex::ui::Style::Spacing & padding)
  {
    float w = padding.left;
    
    for (const auto & view : views) w += view->getWidth() + view->getStyle()->margin.right;
    w += padding.right;
    
    return w;
  };
};

}}
