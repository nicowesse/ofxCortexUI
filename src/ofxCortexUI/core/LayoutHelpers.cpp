#include "LayoutHelpers.h"

namespace ofxCortex { namespace ui {

std::string LayoutHelpers::getAxisString(const Axis & axis)
{
  switch (axis) {
    case Axis::HORIZONTAL: return "HORIZONTAL";
    case Axis::VERTICAL: return "VERTICAL";
  }
}

std::string LayoutHelpers::getDistributionString(const Distribution & distribution)
{
  switch (distribution) {
    case Distribution::STACK: return "STACK";
    case Distribution::FILL: return "FILL";
    case Distribution::FILL_EQUALLY: return "FILL_EQUALLY";
    case Distribution::FILL_PROPORTIONAL: return "FILL_PROPORTIONAL";
    case Distribution::EQUAL_SPACING: return "EQUAL_SPACING";
    case Distribution::EQUAL_CENTERING: return "EQUAL_CENTERING";
  }
}

std::string LayoutHelpers::getAlignmentString(const Alignment & alignment)
{
  switch (alignment) {
    case Alignment::FILL_SPACE: return "FILL_SPACE";
    case Alignment::LEADING: return "LEADING";
    case Alignment::CENTER: return "CENTER";
    case Alignment::TRAILING: return "TRAILING";
  }
}

std::vector<kiwi::Constraint> LayoutHelpers::fillWindow(std::shared_ptr<View> view)
{
  return std::vector<kiwi::Constraint>{
    kiwi::Constraint { view->left == 0 | kiwi::strength::strong },
    kiwi::Constraint { view->right == LayoutEngine::getScreenWidth() | kiwi::strength::strong },
    kiwi::Constraint { view->top == 0 | kiwi::strength::strong },
    kiwi::Constraint { view->bottom == LayoutEngine::getScreenHeight()  | kiwi::strength::strong },
  };
}

std::vector<kiwi::Constraint> LayoutHelpers::fillInner(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views)
{
  std::vector<kiwi::Constraint> constraints;
  std::for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
    std::vector<kiwi::Constraint> fittings = {
      { current->left == outer->content_left | kiwi::strength::strong },
      { current->right == outer->content_right | kiwi::strength::strong },
      { current->top == outer->content_top | kiwi::strength::strong },
      { current->bottom == outer->content_bottom | kiwi::strength::strong },
    };
    
    constraints.insert(constraints.end(), fittings.begin(), fittings.end());
  });
  return constraints;
}

std::vector<kiwi::Constraint> LayoutHelpers::fillOuter(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views)
{
  std::vector<kiwi::Constraint> constraints;
  std::for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
    std::vector<kiwi::Constraint> fittings = {
      { current->left == outer->left | kiwi::strength::strong },
      { current->right == outer->right | kiwi::strength::strong },
      { current->top == outer->top | kiwi::strength::strong },
      { current->bottom == outer->bottom | kiwi::strength::strong },
    };
    
    constraints.insert(constraints.end(), fittings.begin(), fittings.end());
  });
  return constraints;
}

std::vector<kiwi::Constraint> LayoutHelpers::distribute(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis, Distribution distribution)
{
  std::vector<kiwi::Constraint> constraints;
  if (axis == Axis::VERTICAL)
  {
    if (distribution == Distribution::FILL_EQUALLY)
    {
      auto stackConstraints = stack(views, ofxCortex::ui::LayoutHelpers::Axis::VERTICAL);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto equalHeightConstraints = equalHeight(views);
      constraints.insert(std::end(constraints), std::begin(equalHeightConstraints), std::end(equalHeightConstraints));

      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::FILL_PROPORTIONAL)
    {
      float totalHeight = std::accumulate(std::begin(views), std::end(views), 0.0f, [](float carry, std::shared_ptr<View> current) { return carry + current->getHeight(); });
      
      std::for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
        float proportion = current->getHeight() / totalHeight;
//        constraints.push_back({ current->height == outer->height * proportion | kiwi::strength::strong });
      });
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::FILL)
    {
      auto stackConstraints = stack(views, axis);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::STACK)
    {
      auto stackConstraints = stack(views, axis);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto attachmentConstraints = attachLeading(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::EQUAL_SPACING)
    {
      auto stackConstraints = equalSpacing(views, axis);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
  }
  else if (axis == Axis::HORIZONTAL)
  {
    if (distribution == Distribution::FILL_EQUALLY)
    {
      auto equalWidthConstraints = equalWidth(views);
      constraints.insert(std::end(constraints), std::begin(equalWidthConstraints), std::end(equalWidthConstraints));
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::FILL_PROPORTIONAL)
    {
      float totalWidth = std::accumulate(std::begin(views), std::end(views), 0.0f, [](float carry, std::shared_ptr<View> current) { return carry + current->getWidth(); });
      std::for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
        float proportion = current->getWidth() / totalWidth;
        constraints.push_back({ current->width == outer->width * proportion | kiwi::strength::strong });
      });
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::FILL)
    {
      auto stackConstraints = stack(views, axis);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::STACK)
    {
      auto stackConstraints = stack(views, axis);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
    else if (distribution == Distribution::EQUAL_SPACING)
    {
      auto stackConstraints = equalSpacing(views, axis);
      constraints.insert(std::end(constraints), std::begin(stackConstraints), std::end(stackConstraints));
      
      auto attachmentConstraints = attachEnds(outer, views, axis);
      constraints.insert(std::end(constraints), std::begin(attachmentConstraints), std::end(attachmentConstraints));
    }
  }
  
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::stack(std::vector<std::shared_ptr<View>> views, Axis axis)
{
  std::vector<kiwi::Constraint> constraints;
  if (axis == Axis::HORIZONTAL)
  {
    LayoutHelpers::for_each_pair(std::begin(views), std::end(views), [&](std::shared_ptr<View> current, std::shared_ptr<View> next) {
      constraints.push_back({ next->left == current->right + Styling::spacingX() | kiwi::strength::strong });
    });
  }
  else if (axis == Axis::VERTICAL)
  {
    LayoutHelpers::for_each_pair(std::begin(views), std::end(views), [&](std::shared_ptr<View> current, std::shared_ptr<View> next) {
      constraints.push_back({ next->top == current->bottom + Styling::spacingY() | kiwi::strength::strong });
    });
  }
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::equalSpacing(std::vector<std::shared_ptr<View>> views, Axis axis)
{
  std::vector<kiwi::Constraint> constraints;
  std::vector<std::shared_ptr<Spacer>> spacers;
  if (axis == Axis::HORIZONTAL)
  {
    for (int i = 0; i < views.size() - 1; i++)
    {
      auto spacer = Spacer::create();
      const auto & current = views[i];
      const auto & next = views[i + 1];
      
      constraints.push_back({ current->right == spacer->left | kiwi::strength::strong });
      constraints.push_back({ next->left == spacer->right | kiwi::strength::strong });
//        constraints.push_back({ current->height == spacer->height | kiwi::strength::strong });
      
      spacers.push_back(spacer);
      allSpacers.push_back(spacer);
    }
    
    LayoutHelpers::for_each_pair(std::begin(spacers), std::end(spacers), [&](std::shared_ptr<Spacer> current, std::shared_ptr<Spacer> next) {
      constraints.push_back({ next->height == current->height | kiwi::strength::strong });
    });
  }
  else if (axis == Axis::VERTICAL)
  {
    for (int i = 0; i < views.size() - 1; i++)
    {
      auto spacer = Spacer::create();
      const auto & current = views[i];
      const auto & next = views[i + 1];
      
      constraints.push_back({ current->bottom == spacer->top | kiwi::strength::strong });
      constraints.push_back({ next->top == spacer->bottom | kiwi::strength::strong });
//        constraints.push_back({ current->width == spacer->width | kiwi::strength::strong });
      
      spacers.push_back(spacer);
      allSpacers.push_back(spacer);
    }
    
    LayoutHelpers::for_each_pair(std::begin(spacers), std::end(spacers), [&](std::shared_ptr<Spacer> current, std::shared_ptr<Spacer> next) {
      constraints.push_back({ current->height == next->height | kiwi::strength::strong });
    });
  }
  
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::fillEqually(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis)
{
  std::vector<kiwi::Constraint> constraints;
  if (axis == Axis::HORIZONTAL)
  {
    auto equalSizeConstraints = equalWidth(views);
    constraints.insert(std::end(constraints), std::begin(equalSizeConstraints), std::end(equalSizeConstraints));
    
    auto attachEndsConstraints = attachEnds(outer, views, axis);
    constraints.insert(std::end(constraints), std::begin(attachEndsConstraints), std::end(attachEndsConstraints));
  }
  else if (axis == Axis::VERTICAL)
  {
    auto equalSizeConstraints = equalHeight(views);
    constraints.insert(std::end(constraints), std::begin(equalSizeConstraints), std::end(equalSizeConstraints));
    
    auto attachEndsConstraints = attachEnds(outer, views, axis);
    constraints.insert(std::end(constraints), std::begin(attachEndsConstraints), std::end(attachEndsConstraints));
  }
  
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::equalHeight(std::vector<std::shared_ptr<View>> views)
{
  std::vector<kiwi::Constraint> constraints;
  LayoutHelpers::for_each_pair(std::begin(views), std::end(views), [&](std::shared_ptr<View> current, std::shared_ptr<View> next) {
    constraints.push_back({ current->height == next->height | kiwi::strength::strong });
  });
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::equalWidth(std::vector<std::shared_ptr<View>> views)
{
  std::vector<kiwi::Constraint> constraints;
  LayoutHelpers::for_each_pair(std::begin(views), std::end(views), [&](std::shared_ptr<View> current, std::shared_ptr<View> next) {
    constraints.push_back({ current->width == next->width | kiwi::strength::strong });
  });
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::fitWidth(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views)
{
  std::vector<kiwi::Constraint> constraints;
  for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
    std::vector<kiwi::Constraint> fittings = {
      { current->width == outer->content_width | kiwi::strength::strong },
      { current->left == outer->content_left | kiwi::strength::strong },
      { current->right == outer->content_right | kiwi::strength::strong }
    };
    
    constraints.insert(constraints.end(), fittings.begin(), fittings.end());
  });
  return constraints;
}

std::vector<kiwi::Constraint> LayoutHelpers::fitWidth(std::shared_ptr<View> outer, std::shared_ptr<View> inner)
{
  return fitWidth(outer, std::vector<std::shared_ptr<View>>{ inner });
}


std::vector<kiwi::Constraint> LayoutHelpers::fitHeight(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views)
{
  std::vector<kiwi::Constraint> constraints;
  for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
    std::vector<kiwi::Constraint> fittings = {
      { current->height == outer->content_height | kiwi::strength::strong },
      { current->top == outer->content_top | kiwi::strength::strong },
      { current->bottom == outer->content_bottom | kiwi::strength::strong }
    };
    
    constraints.insert(constraints.end(), fittings.begin(), fittings.end());
  });
  return constraints;
}

std::vector<kiwi::Constraint> LayoutHelpers::fitHeight(std::shared_ptr<View> outer, std::shared_ptr<View> inner)
{
  return fitHeight(outer, std::vector<std::shared_ptr<View>>{ inner });
}


std::vector<kiwi::Constraint> LayoutHelpers::attachEnds(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis)
{
  if (views.size() == 0) return std::vector<kiwi::Constraint>();
  
  if (axis == Axis::HORIZONTAL)
  {
    return std::vector<kiwi::Constraint>{
      { outer->content_left == views.front()->left | kiwi::strength::strong },
      { outer->content_right == views.back()->right | kiwi::strength::strong }
    };
  }
  else if (axis == Axis::VERTICAL)
  {
    return std::vector<kiwi::Constraint>{
     { outer->content_top == views.front()->top | kiwi::strength::strong },
     { outer->content_bottom == views.back()->bottom | kiwi::strength::strong }
   };
  }
  
  return std::vector<kiwi::Constraint>();
}


std::vector<kiwi::Constraint> LayoutHelpers::attachLeading(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis)
{
  if (views.size() == 0) return std::vector<kiwi::Constraint>();
  
  if (axis == Axis::HORIZONTAL)
  {
    return std::vector<kiwi::Constraint>{
      { outer->content_left == views.front()->left | kiwi::strength::strong }
    };
  }
  else if (axis == Axis::VERTICAL)
  {
    return std::vector<kiwi::Constraint>{
     { outer->content_top == views.front()->top | kiwi::strength::strong }
   };
  }
  
  return std::vector<kiwi::Constraint>();
}


std::vector<kiwi::Constraint> LayoutHelpers::attachTrailing(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis)
{
  if (views.size() == 0) return std::vector<kiwi::Constraint>();
  
  if (axis == Axis::HORIZONTAL)
  {
    return std::vector<kiwi::Constraint>{
      { outer->content_right == views.back()->right | kiwi::strength::strong }
    };
  }
  else if (axis == Axis::VERTICAL)
  {
    return std::vector<kiwi::Constraint>{
     { outer->content_bottom == views.back()->bottom | kiwi::strength::strong }
   };
  }
  
  return std::vector<kiwi::Constraint>();
}

#pragma mark - Alignment
std::vector<kiwi::Constraint> LayoutHelpers::alignment(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Axis axis, Alignment alignment)
{
  if (axis == Axis::HORIZONTAL) return alignHorizontal(outer, views, alignment);
  else if (axis == Axis::VERTICAL) return alignVertical(outer, views, alignment);
  
  return std::vector<kiwi::Constraint>();
}


std::vector<kiwi::Constraint> LayoutHelpers::alignHorizontal(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Alignment alignment)
{
  std::vector<kiwi::Constraint> constraints;
  for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
    if (alignment == Alignment::FILL_SPACE) {
      constraints.push_back({ current->top == outer->content_top | kiwi::strength::strong });
      constraints.push_back({ current->bottom == outer->content_bottom | kiwi::strength::strong });
    }
    else if (alignment == Alignment::LEADING) constraints.push_back({ current->top == outer->content_top | kiwi::strength::strong });
    else if (alignment == Alignment::CENTER) constraints.push_back({ current->centerY == outer->content_centerY | kiwi::strength::strong });
    else if (alignment == Alignment::TRAILING) constraints.push_back({ current->bottom == outer->content_bottom | kiwi::strength::strong });
    else constraints.push_back({ current->top == outer->content_top | kiwi::strength::strong });
  });
  
  return constraints;
}


std::vector<kiwi::Constraint> LayoutHelpers::alignVertical(std::shared_ptr<View> outer, std::vector<std::shared_ptr<View>> views, Alignment alignment)
{
  std::vector<kiwi::Constraint> constraints;
  std::for_each(std::begin(views), std::end(views), [&](std::shared_ptr<View> current) {
    if (alignment == Alignment::FILL_SPACE) {
      constraints.push_back({ current->left == outer->content_left | kiwi::strength::strong });
      constraints.push_back({ current->right == outer->content_right | kiwi::strength::strong });
    }
    else if (alignment == Alignment::LEADING) constraints.push_back({ current->left == outer->content_left | kiwi::strength::strong });
    else if (alignment == Alignment::CENTER) constraints.push_back({ current->centerX == outer->content_centerX | kiwi::strength::strong });
    else if (alignment == Alignment::TRAILING) constraints.push_back({ current->right == outer->content_right | kiwi::strength::strong });
    else constraints.push_back({ current->left == outer->content_left | kiwi::strength::strong });
  });
  
  return constraints;
}

}}
