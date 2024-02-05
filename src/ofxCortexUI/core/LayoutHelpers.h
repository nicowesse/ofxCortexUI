#pragma once

#include "kiwi.h"
#include "View.h"
#include "Style.h"
#include "ofxCortex/utils/Helpers.h"
#include <numeric>

namespace ofxCortex { namespace ui {

class LayoutHelpers {
public:
  enum class Axis {
    HORIZONTAL, VERTICAL
  };
  static std::string getAxisString(const Axis & axis);
  
  enum class Distribution {
    STACK,
    FILL,
    FILL_EQUALLY,
    FILL_PROPORTIONAL,
    EQUAL_SPACING,
    EQUAL_CENTERING
  };
  static std::string getDistributionString(const Distribution & distribution);
  
  enum class Alignment {
    FILL_SPACE,
    LEADING,
    CENTER,
    TRAILING,
  };
  
  static std::string getAlignmentString(const Alignment & alignment);
  
  static std::vector<kiwi::Constraint> fillWindow(std::shared_ptr<View> view);
  static std::vector<kiwi::Constraint> fillInner(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views);
  static std::vector<kiwi::Constraint> fillInner(std::shared_ptr<View> outer, std::shared_ptr<View> inner) { return fillInner(outer, std::vector<std::shared_ptr<View>>{ inner }); }
  static std::vector<kiwi::Constraint> fillOuter(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views);
  static std::vector<kiwi::Constraint> fillOuter(std::shared_ptr<View> outer, std::shared_ptr<View> inner) { return fillOuter(outer, std::vector<std::shared_ptr<View>>{ inner }); }
  
  
  static std::vector<kiwi::Constraint> distribute(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis, Distribution distribution);
  
  static std::vector<kiwi::Constraint> stack(std::vector<std::shared_ptr<View>> views, Axis axis);
  
  static std::vector<kiwi::Constraint> equalSpacing(std::vector<std::shared_ptr<View>> views, Axis axis);
  
  
  static std::vector<kiwi::Constraint> fillEqually(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis);
  
  static std::vector<kiwi::Constraint> equalHeight(std::vector<std::shared_ptr<View>> views);
  
  static std::vector<kiwi::Constraint> equalWidth(std::vector<std::shared_ptr<View>> views);
  
  static std::vector<kiwi::Constraint> fitWidth(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views);
  
  static std::vector<kiwi::Constraint> fitWidth(std::shared_ptr<View> outer, std::shared_ptr<View> inner);
  
  static std::vector<kiwi::Constraint> fitHeight(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views);
  
  static std::vector<kiwi::Constraint> fitHeight(std::shared_ptr<View> outer, std::shared_ptr<View> inner);
  
  static std::vector<kiwi::Constraint> attachEnds(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis);
  
  static std::vector<kiwi::Constraint> attachLeading(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis);
  
  static std::vector<kiwi::Constraint> attachTrailing(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis);
  
  
#pragma mark - Alignment
  static std::vector<kiwi::Constraint> alignment(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis, Alignment alignment);
  
  static std::vector<kiwi::Constraint> alignHorizontal(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Alignment alignment);
  
  static std::vector<kiwi::Constraint> alignVertical(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Alignment alignment);
  
protected:
  class Spacer {
  public:
    Spacer() {
      LayoutEngine::addConstraints({
        kiwi::Constraint { right >= left | kiwi::strength::required },
        kiwi::Constraint { bottom >= top | kiwi::strength::required },
        
        kiwi::Constraint { width >= 0 | kiwi::strength::required },
        kiwi::Constraint { height >= 0 | kiwi::strength::required },
        kiwi::Constraint { right == left + width | kiwi::strength::required },
        kiwi::Constraint { bottom == top + height | kiwi::strength::required },
      });
      
      LayoutEngine::addEditVariable(left, kiwi::strength::weak);
      LayoutEngine::addEditVariable(top, kiwi::strength::weak);
      LayoutEngine::addEditVariable(width, kiwi::strength::weak);
      LayoutEngine::addEditVariable(height, kiwi::strength::weak);
    }
    
    static std::shared_ptr<Spacer> create() { return std::make_shared<Spacer>(); }
    
    kiwi::Variable left { "left" };
    kiwi::Variable top { "top" };
    
    kiwi::Variable width { "width" };
    kiwi::Variable height { "height" };
    kiwi::Variable right { "right" };
    kiwi::Variable bottom { "bottom" };
  };
  
  inline static std::vector<std::shared_ptr<Spacer>> allSpacers;
  
#pragma mark - Private Helpers
private:
  LayoutHelpers() = default;
  LayoutHelpers(LayoutHelpers&) = delete;
  LayoutHelpers& operator=(const LayoutHelpers&) = delete;
  
  template< typename FwdIter, typename Func >
  static Func for_each_pair( FwdIter iterStart, FwdIter iterEnd, Func func )
  {
    if( iterStart == iterEnd )
      return func;
    
    FwdIter iterNext = iterStart;
    ++iterNext;
    
    for( ; iterNext != iterEnd; ++iterStart, ++iterNext )
    {
      func( *iterStart, *iterNext );
    }
    return func;
  }
};

}}
