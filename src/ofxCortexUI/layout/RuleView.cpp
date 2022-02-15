#include "RuleView.h"

namespace ofxCortex { namespace ui {

const RuleView::Rule RuleView::Auto = Rule { RuleType::AUTO, 0.0 };
const RuleView::Rule RuleView::FillRemaining = RuleView::Auto;
const RuleView::Rule RuleView::FitContent = Rule { RuleType::FIT_CONTENT, 0.0 };

void RuleView::addView(shared_ptr<View> view, Rule rule, Layout::Alignment alignment)
{
  addViewAt(view, this->numChildren(false), rule, alignment);
}

void RuleView::addViewAt(shared_ptr<View> view, unsigned int index, Rule rule, Layout::Alignment alignment)
{
  if (view == nullptr) ofLogWarning(_getLogModule()) << "addViewAt(): View is NULL. Returning..";
  
  rule.align = alignment;
  
  View::addChildAt(view, index);
//  _rules[view] = rule;
  _rules.push_back({ view, rule });
  
  _adjustLayout();
}

void RuleView::removeView(shared_ptr<View> view)
{
  View::removeChild(view);
//  _rules.erase(view);
  _rules.erase(find_if(_rules.begin(), _rules.end(), [&](const pair<shared_ptr<View>, Rule> & pair) { return pair.first == view; }), _rules.end());
  
  _adjustLayout();
}

void RuleView::removeViewAt(unsigned int index)
{
  auto view = dynamic_pointer_cast<View>(View::childAt(index));
  if (view) { removeView(view); }
}

void RuleView::removeAllViews()
{
  View::removeAllChildren();
  _rules.clear();
  
  _adjustLayout();
}

void RuleView::_adjustLayout()
{
  float majorAxisSpace = (_axis == Layout::VERTICAL) ? getHeight() : getWidth();
  majorAxisSpace -= (_axis == Layout::VERTICAL) ? (style->padding.top + style->padding.bottom) : (style->padding.left + style->padding.right);
  
  float minorAxisSpace = (_axis == Layout::VERTICAL) ? getWidth() : getHeight();
  minorAxisSpace -= (_axis == Layout::VERTICAL) ? (style->padding.left + style->padding.right) : (style->padding.top + style->padding.bottom);
  
  auto views = children<View>();
  float availableSpace = majorAxisSpace - accumulate(views.begin(), views.end(), 0.0, [this](double acc, const shared_ptr<View> & view) {
    return (_axis == Layout::VERTICAL) ? (acc + view->getStyle()->margin.bottom) : (acc + view->getStyle()->margin.right);
  });
  
  // Calculate all items sizes and space allocation
  
  // Static Items
  for (auto & staticItem : _getItemsByRule({ RuleType::STATIC }))
  {
    auto & view = staticItem.first;
    const auto & rule = staticItem.second;
    
    if (_axis == Layout::HORIZONTAL) view->setWidth(rule.value);
    else view->setHeight(rule.value);
    
    availableSpace -= rule.value;
  }
  
  // Fit-Content Items
  for (auto & fitContentItem : _getItemsByRule({ RuleType::FIT_CONTENT }))
  {
    availableSpace -= (_axis == Layout::VERTICAL) ? fitContentItem.first->getHeight() : fitContentItem.first->getWidth();
  }
  
  // Ratio Items
  auto ratioItems = _getItemsByRule({ RuleType::RATIO });
  float totalRatio = accumulate(ratioItems.begin(), ratioItems.end(), 0.0, [this](double acc, pair<shared_ptr<View>, Rule> item) { return acc + item.second.value; });
  float ratioMultiplier = 1.0 / MAX(1.0, totalRatio);
  float availableSpaceForRatio = availableSpace;
  
  for (auto & ratioItem : ratioItems)
  {
    auto & view = ratioItem.first;
    const auto & rule = ratioItem.second;
    
    float size = (rule.value * ratioMultiplier) * availableSpaceForRatio;
    if (_axis == Layout::HORIZONTAL) view->setWidth(size);
    else view->setHeight(size);
    
    availableSpace -= size;
  }
  
  // Auto Items
  auto autoItems = _getItemsByRule({ RuleType::AUTO });
  size_t autoItemsCount = autoItems.size();
  float autoRatio = 1.0 / autoItemsCount;
  
  for (auto & autoItem : autoItems)
  {
    auto & view = autoItem.first;
    const auto & rule = autoItem.second;
    
    float size = availableSpace * autoRatio;
    
    if (_axis == Layout::HORIZONTAL) view->setWidth(size);
    else view->setHeight(size);
    
    availableSpace -= size;
  }
  
  // Layout all objects
  glm::vec2 pos = this->getStyle()->padding;
  for (auto & item : _getItemsByRule())
  {
    auto & view = item.first;
    const auto & rule = item.second;
    
    if (_axis == Layout::VERTICAL)
    {
      float minorAxisDifference = minorAxisSpace - view->getWidth();
      
      if (rule.align == Layout::FILL) view->setWidth(minorAxisSpace)->setPosition(pos);
      else if (rule.align == Layout::ALIGN_START) view->setPosition(pos);
      else if (rule.align == Layout::ALIGN_CENTER) view->setPosition(pos + glm::vec2(minorAxisDifference * 0.5f, 0));
      else if (rule.align == Layout::ALIGN_END) view->setPosition(pos + glm::vec2(minorAxisDifference, 0));
      
      pos.y = view->getHeight() + view->getStyle()->margin.bottom;
    }
    else if (_axis == Layout::HORIZONTAL)
    {
      float minorAxisDifference = minorAxisSpace - view->getHeight();
      
      if (rule.align == Layout::FILL) view->setHeight(minorAxisSpace)->setPosition(pos);
      else if (rule.align == Layout::ALIGN_START) view->setPosition(pos);
      else if (rule.align == Layout::ALIGN_CENTER) view->setPosition(pos + glm::vec2(0, minorAxisDifference * 0.5f));
      else if (rule.align == Layout::ALIGN_END) view->setPosition(pos + glm::vec2(0, minorAxisDifference));
      
      pos.x += view->getWidth() + view->getStyle()->margin.right;
    }
  }
  
  
}

vector<pair<shared_ptr<View>, RuleView::Rule>> RuleView::_getItemsByRule(vector<RuleType> types)
{
  vector<pair<shared_ptr<View>, Rule>> output;
  
  if (types.size() == 0) return _rules;
  
  for (const auto & entry : _rules)
  {
    if (find(types.begin(), types.end(), entry.second.type) != types.end()) output.push_back(entry);
  }
  
  return output;
}


vector<shared_ptr<View>> RuleView::_getViewsByRule(vector<RuleType> types)
{
  vector<shared_ptr<View>> output;
  
  if (types.size() == 0)
  {
    std::for_each(_rules.begin(), _rules.end(), [&output] (const std::pair<shared_ptr<View>, Rule> &entry) { output.push_back(entry.first); });
    return output;
  }
  
  for (const auto & entry : _rules)
  {
    if (find(types.begin(), types.end(), entry.second.type) != types.end()) output.push_back(entry.first);
  }
  
  return output;
}

}}
