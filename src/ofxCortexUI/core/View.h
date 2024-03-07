#pragma once

#include <stack>

#include "kiwi.h"
#include "ofEvents.h"
#include "ofLog.h"
#include "ofRectangle.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofParameter.h"
#include "LayoutEngine.h"
#include "Style.h"
#include "Stencil.h"
#include "ofxCortex/types/Parameter.h"

namespace ofxCortex { namespace ui {

class View : public std::enable_shared_from_this<View> {
protected:
  View(const std::string & name);
  
public:
  virtual ~View();
  
  template<typename ... F>
  static std::shared_ptr<View> create(F&& ... f) {
    struct EnableMakeShared : public View { EnableMakeShared(F&&... arg) : View(std::forward<F>(arg)...) {} };
    
    auto p = std::make_shared<EnableMakeShared>(std::forward<F>(f)...);
    
    ofLogVerbose(p->toString(__FUNCTION__));
    
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
  virtual void setName(std::string name) { _name = name; }
  std::string getName() const { return _name; }
  
  void update() { this->internalUpdate(ofGetElapsedTimef(), ofGetLastFrameTime()); }
  void update(float time, float delta) { this->internalUpdate(time, delta); }
  
  void draw() { this->internalDraw(); }
  void enableRendering() { this->shouldDraw = true; }
  void disableRendering() { this->shouldDraw = false; }
  void enableSubviewRendering() { this->shouldDrawSubviews = true; }
  void disableSubviewRendering() { this->shouldDrawSubviews = false; }
  
  void enableMask() { isMaskEnabled = true; }
  void disableMask() { isMaskEnabled = false; }
  
  void enableDebug() { isDebugEnabled = true; }
  void disableDebug() { isDebugEnabled = false; }
  
  
#pragma mark - OBJECT: Overrideables
protected:
  struct MouseEventArgs;
  struct DeltaMouseEventArgs;
  
  virtual void viewDidLoad() {
    ofLogVerbose(toString(__FUNCTION__));
//    this->setNeedsUpdateConstraints();
//    this->setNeedsLayout();
  };
  
  virtual void onUpdate(float time, float delta) {};
  
  virtual void onPreDraw() {}
  virtual void onDraw();
  virtual void onPostDraw();
  
  virtual void onDrawMask();
  virtual void onDebug();
  virtual void onDrawSubviews();
  
  virtual void onMousePressed(const MouseEventArgs & e);
  virtual void onMouseReleased(const MouseEventArgs & e);
  virtual void onMouseReleasedOutside(const MouseEventArgs & e);
  virtual void onMouseEnter(const MouseEventArgs & e);
  virtual void onMouseExit(const MouseEventArgs & e);
  virtual void onMouseMoved(const MouseEventArgs & e);
  virtual void onMouseDragged(const MouseEventArgs & e);
  virtual void onMouseScrolled(const MouseEventArgs & e);
  
  virtual void onKeyPressed(const ofKeyEventArgs & e);
  virtual void onKeyReleased(const ofKeyEventArgs & e);
  virtual void onCharTyped(const uint32_t & c);
  
  virtual void onWindowResized(const ofResizeEventArgs & e);
  
  
  
#pragma mark - OBJECT: Identification
protected:
  std::string _name;
  
  virtual std::string _getComponentName() const { return "View"; }
  virtual std::string _getLogModule(const std::string & func = "", bool includeStructure = true) const;
  std::string toString(const std::string & functionName = "", bool includeStructure = true) const { return _getLogModule(functionName, includeStructure); }
  friend std::ostream& operator<<(std::ostream& os, const View& view)
  {
    os << view.toString("", false);
    return os;
  }
  friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<View>& view)
  {
    os << view->toString("", false);
    return os;
  }
  
  std::string _getLevel() const {
    if (!hasParent()) return "";
    
    std::stringstream level;
    level << "┣";
    
    std::shared_ptr<View> current = getParent();
    while (current->hasParent())
    {
      level << " ━";
      current = current->getParent();
    }
    level << " ";
    
    return level.str();
  }
  
  
  
#pragma mark - LAYOUT: Positioning and Sizing
public:
  
  void setIntrinsicSize(float width, float height);
  glm::vec2 getIntrinsicSize() const { return glm::vec2(getIntrinsicWidth(), getIntrinsicHeight()); }
  void setIntrinsicWidth(float width);
  float getIntrinsicWidth() const { return this->intrinsic_width.value(); }
  void setIntrinsicHeight(float height);
  float getIntrinsicHeight() const { return this->intrinsic_height.value(); }
  
  void  setLeft(float value);
  float getLeft() const { return this->left.value(); }
  float getContentLeft() const { return this->content_left.value(); }
  void  setTop(float value);
  float getTop() const { return this->top.value(); }
  float getContentTop() const { return this->content_top.value(); }
  void  setRight(float value);
  float getRight() const { return this->right.value(); }
  float getContentRight() const { return this->content_right.value(); }
  void  setBottom(float value);
  float getBottom() const { return this->bottom.value(); }
  float getContentBottom() const { return this->content_bottom.value(); }
  
  void setPosition(float x, float y);
  void setPosition(const glm::vec2 & position);
  glm::vec2 getPosition() const { return glm::vec2(this->left.value(), this->top.value()); }
  void setWidth(float value);
  float getWidth() const { return this->width.value(); }
  float getContentWidth() const { return this->content_width.value(); }
  void setHeight(float value);
  float getHeight() const { return this->height.value(); }
  float getContentHeight() const { return this->content_height.value(); }
  void setSize(float w, float h);
  void setSize(glm::vec2 size);
  
  void setFrame(const ofRectangle & rect);
  ofRectangle getFrame() const;
  ofRectangle getContentFrame() const;
  
  void setCenter(float x, float y);
  void setCenter(glm::vec2 center);
  
  void translate(float dX, float dY);
  void translate(const glm::vec2 & delta);
  
  void setZ(int value) { layerZ = value; }
  int getZ() const { return layerZ; }
  
  bool inside(const glm::vec2 & point);
  bool isOverlapped(const glm::vec2 & point);
  
  void enableOverlap() { includeInOverlap = true; }
  void disableOverlap() { includeInOverlap = false; }
  
  
#pragma mark - LAYOUT: Variables
public:
  void addConstraint(kiwi::Constraint & constraints);
  void addConstraints(const std::vector<kiwi::Constraint> & constraints);
  
  void removeConstraint(kiwi::Constraint & constraints);
  void removeConstraints(const std::vector<kiwi::Constraint> constraints);
  void clearConstraints();
  
  kiwi::Variable left { "left" };
  kiwi::Variable top { "top" };
  
  kiwi::Variable width { "width" };
  kiwi::Variable height { "height" };
  kiwi::Variable right { "right" };
  kiwi::Variable bottom { "bottom" };
  
  kiwi::Variable centerX { "centerX" };
  kiwi::Variable centerY { "centerY" };
  
  
  kiwi::Variable content_left { "content_left" };
  kiwi::Variable content_top { "content_top" };
  
  kiwi::Variable content_width { "content_width" };
  kiwi::Variable content_height { "content_height" };
  kiwi::Variable content_right { "content_right" };
  kiwi::Variable content_bottom { "content_bottom" };
  
  kiwi::Variable content_centerX { "content_centerX" };
  kiwi::Variable content_centerY { "content_centerY" };
  
  kiwi::Variable intrinsic_width { "intrinsic_width" };
  kiwi::Variable intrinsic_height { "intrinsic_height" };
  
#pragma mark - LAYOUT: Helpers
protected:  
  void updateFrames()
  {
    ofLogVerbose(toString(__FUNCTION__));
    
    this->frame = getFrame();
    this->contentFrame = getContentFrame();
  }
  
  virtual void layoutSubviews();
  void layoutIfNeeded() {
    if (this->needsLayout) {
      this->updateFrames();
      this->layoutSubviews();
      
      // Subviews
      for (auto & subview : subviews)
      {
        subview->setNeedsLayout();
//        subview->layoutIfNeeded();
      }
      
      this->needsLayout = false;
    }
  }
  void setNeedsLayout() {
    ofLogVerbose(toString(__FUNCTION__));
    this->needsLayout = true;
  };
  void forceLayout() {
    this->updateFrames();
    this->layoutSubviews();
    
    // Subviews
    for (auto & subview : subviews)
    {
      subview->layoutSubviews();
    }
  }
  bool needsLayout { true };
  
  
  virtual void updateConstraints();
  void setNeedsUpdateConstraints() { this->needsUpdateConstraints = true; }
  void updateConstraintsIfNeeded() {
    if (this->needsUpdateConstraints) {
      this->updateConstraints();
      this->needsUpdateConstraints = false;
    }
  }
  bool needsUpdateConstraints { true };
  
  std::vector<kiwi::Constraint> baseConstraints;
  std::vector<kiwi::Constraint> layoutConstraints;
  
  
  
#pragma mark - LAYOUT: Rendering
protected:
  void internalUpdate(double time, double delta);
  void internalDraw();
  
  bool isMaskEnabled { true };
  bool isDebugEnabled { false };
  bool shouldDraw { true };
  bool shouldDrawSubviews { true };
  
  ofRectangle frame;
  ofRectangle contentFrame;
  
  ofRectangle getLocalFrame() const;
  ofRectangle getLocalContentFrame() const;
  
  int layerZ { 0 };
  bool includeInOverlap { true };
  
#pragma mark - OBJECT: Family Tree
public:
  virtual void addSubviewAt(const std::shared_ptr<View> & subview, size_t index);
  virtual void addSubview(const std::shared_ptr<View> & subview);
  virtual void addSubviews(std::vector<std::shared_ptr<View>> views);
  virtual void removeSubview(std::shared_ptr<View> subview);
  
  bool hasParent() const { return superview != nullptr; }
  std::shared_ptr<View> getParent() const { return superview; }
  void setParent(std::shared_ptr<View> parent) { this->superview = parent; }
  
  std::shared_ptr<View> getRoot();
  static std::vector<std::shared_ptr<View>> flatten(const std::shared_ptr<View> & node);
  void setLevel(int level) { this->level = level; }
  
protected:
  std::shared_ptr<View> superview;
  std::vector<std::shared_ptr<View>> subviews;
  
  std::shared_ptr<View> getSelf() { return shared_from_this(); }
  
  inline static std::shared_ptr<View> focused = nullptr;
  bool isFocused() const { return View::focused != nullptr && View::focused.get() == this; }
  
  inline static std::set<std::shared_ptr<View>> everyView;
  
  int level { 0 };
  int getDepth() const;
  int getActualZ() const { return getDepth() + layerZ; }
  
  
  
#pragma mark - EVENTS
public:
  void enableInteraction();
  void disableInteraction();
  
  void disableEvents() { this->unbindEvents(); }
  void enableUpdateEvent() { this->bindUpdateListener(); }
  void disableUpdateEvent() { this->unbindUpdateListener(); }
  void enableDrawEvent() { this->bindDrawListener(); }
  void disableDrawEvent() { this->bindUpdateListener(); }
  void enableMouseEvents() { this->bindMouseListeners(); }
  void disableMouseEvents() { this->unbindMouseListeners(); }
  void enableKeyEvents() { this->bindKeyListeners(); }
  void disableKeyEvents() { this->unbindKeyListeners(); }
  void enableWindowResizeEvents() { this->bindWindowResizedListener(); }
  void disableWindowResizeEvents() { this->unbindWindowResizedListener(); }
  
  void enableInteractionOutsideParent() { this->shouldEnableInteractionOutsideParent = true; }
  void disableInteractionOutsideParent() { this->shouldEnableInteractionOutsideParent = false; }
  
protected:
  inline static ofEventListeners eventListeners;
  
  bool isInteractionEnabled { true };
  // All Events
  void bindEvents();
  void unbindEvents();
  
  // Update
  void bindUpdateListener() { ofAddListener(ofEvents().update, this, &View::updateHandler); }
  void unbindUpdateListener() { ofRemoveListener(ofEvents().update, this, &View::updateHandler); }
  void updateHandler(ofEventArgs & e) { this->internalUpdate(ofGetElapsedTimef(), ofGetLastFrameTime()); }
  
  // Draw
  void bindDrawListener() { ofAddListener(ofEvents().draw, this, &View::drawHandler); }
  void unbindDrawListener() { ofRemoveListener(ofEvents().draw, this, &View::drawHandler); }
  void drawHandler(ofEventArgs & e) { this->internalDraw(); }
  
  // Window Resized
  void bindWindowResizedListener() { ofAddListener(ofEvents().windowResized, this, &View::windowResizedHandler); }
  void unbindWindowResizedListener() { ofRemoveListener(ofEvents().windowResized, this, &View::windowResizedHandler); }
  void windowResizedHandler(ofResizeEventArgs & e);
  
  // Mouse Handling
  struct MouseEventArgs : public ofMouseEventArgs {
    MouseEventArgs() : ofMouseEventArgs() {};
    MouseEventArgs(const ofMouseEventArgs & e) : ofMouseEventArgs(e) {};
    MouseEventArgs& operator= (const MouseEventArgs &other)
    {
      ofMouseEventArgs::operator=(other);
      
      isInside = other.isInside;
      isOverlapped = other.isOverlapped;
      delta = other.delta;
      
      return *this;
    }
    
    bool isInside { false };
    bool isOverlapped { false };
    glm::vec2 delta { 0, 0 };
  };
  
  void bindMouseListeners();
  void unbindMouseListeners();
  void mousePressedHandler(ofMouseEventArgs & e);
  void mouseReleasedHandler(ofMouseEventArgs & e);
  void mouseMovedHandler(ofMouseEventArgs & e);
  void mouseDraggedHandler(ofMouseEventArgs & e);
  void mouseScrolledHandler(ofMouseEventArgs & e);
  
  ofxCortex::ui::Styling::State getMouseState() const {
    if (!this->isInteractionEnabled) return ofxCortex::ui::Styling::State::IDLE;
    
    if (this->isMouseInside && this->isMousePressed) return ofxCortex::ui::Styling::State::ACTIVE;
    else if (this->isFocused()) return ofxCortex::ui::Styling::State::FOCUS;
    else if (this->isMouseInside) return ofxCortex::ui::Styling::State::HOVER;
    
    return ofxCortex::ui::Styling::State::IDLE;
  }
  
  std::string getMouseStateString(const ofxCortex::ui::Styling::State & state)
  {
    switch(state) {
      case ofxCortex::ui::Styling::State::ACTIVE: return "ACTIVE";
      case ofxCortex::ui::Styling::State::FOCUS: return "FOCUS";
      case ofxCortex::ui::Styling::State::HOVER: return "HOVER";
      default: case ofxCortex::ui::Styling::State::IDLE: return "IDLE";
    }
  }
  
  float getHoverIntensity() const { return mouseHoverIntensity; }
  float getActiveIntensity() const { return mouseActiveIntensity; }
  
  // Mouse Flags
  bool isMouseInside { false };
  bool isMousePressed { false };
  bool wasMousePressedInside { false };
  
  float mouseActiveIntensity { 0.0f };
  float mouseHoverIntensity { 0.0f };
  
  int lastMousePressTime { 0 };
  int doublePressInterval { 500 };
  glm::vec2 lastMousePosition { 0, 0 };
  
  bool shouldEnableInteractionOutsideParent { true };
  
  // Key Handling
  void bindKeyListeners();
  void unbindKeyListeners();
  void keyPressedHandler(ofKeyEventArgs & e);
  void keyReleasedHandler(ofKeyEventArgs & e);
  void charTypedHandler(uint32_t & c);
};

template<typename T>
class ParameterView : public View {
protected:
  ParameterView(const ofAbstractParameter & parameter) : View(parameter.getName())
  {
    this->parameterRef = parameter.newReference();
  }
  
protected:
  std::shared_ptr<ofAbstractParameter> parameterRef;
  
  ofParameter<T> & getParameter() {
    auto unit = std::dynamic_pointer_cast<ofxCortex::UnitParameter<T>>(parameterRef);
    
    if (unit) return unit->getParameter();
    else return parameterRef->cast<T>();
  }
  
  std::shared_ptr<ofAbstractParameter> getParameterReference() { return parameterRef->newReference(); }
  
  // Aliases
  std::string getParameterName() { return parameterRef->getName(); }
  T getParameterMin() { return getParameter().getMin(); }
  T getParameterMax() { return getParameter().getMax(); }
  std::string getParameterType() { return typeid(T).name(); }
  
  void setParameter(const T & value) { getParameter().set(value); }
  T getParameterValue() { return getParameter().get(); }
  
  std::string getUnit() {
    auto p = std::dynamic_pointer_cast<ofxCortex::UnitParameter<T>>(parameterRef);
    
    return (p) ? p->getUnit() : "";
  }
  
};

}}
