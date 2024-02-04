#pragma once

#include <stack>

#include "kiwi.h"
#include "ofEvents.h"
#include "ofLog.h"
#include "ofRectangle.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "LayoutEngine.h"
#include "Style.h"
#include "Stencil.h"

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
    p->viewDidLoad();
    
    View::everyView.insert(p);
    return p;
  }
  
  virtual void setName(std::string name) { _name = name; }
  std::string getName() const { return _name; }
  
  void update() { this->internalUpdate(ofGetElapsedTimef(), ofGetLastFrameTime()); }
  void update(float time, float delta) { this->internalUpdate(time, delta); }
  
  void draw() { this->internalDraw(); }
  void enableSubviewRendering() { shouldDrawSubviews = true; }
  void disableSubviewRendering() { shouldDrawSubviews = false; }
  
  void enableMask() { isMaskEnabled = true; }
  void disableMask() { isMaskEnabled = false; }
  
  void enableDebug() { isDebugEnabled = true; }
  void disableDebug() { isDebugEnabled = false; }
  
  
#pragma mark - OBJECT: Overrideables
protected:
  struct MouseEventArgs;
  struct DeltaMouseEventArgs;
  
  virtual void viewDidLoad() {};
  
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
  
  virtual void onWindowResized(const ofResizeEventArgs & e) {};
  
  
  
#pragma mark - OBJECT: Identification
protected:
  std::string _name;
  
  virtual std::string _getComponentName() const { return "View"; }
  virtual std::string _getLogModule(const std::string & func);
  
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
  void addConstraint(kiwi::Constraint & constraints);
  void addConstraints(std::vector<kiwi::Constraint> constraints);
  
  void removeConstraint(kiwi::Constraint & constraints);
  void removeConstraints(std::vector<kiwi::Constraint> constraints);
  void removeAllConstraints();
  
  void setIntrinsicSize(float width, float height);
  void setIntrinsicWidth(float width);
  void setIntrinsicHeight(float height);
  
  void setLeft(float value);
  float getLeft() const { return this->left.value(); }
  float getContentLeft() const { return this->content_left.value(); }
  void setTop(float value);
  float getTop() const { return this->top.value(); }
  float getContentTop() const { return this->content_top.value(); }
  void setRight(float value);
  float getRight() const { return this->right.value(); }
  float getContentRight() const { return this->content_right.value(); }
  void setBottom(float value);
  float getBottom() const { return this->bottom.value(); }
  float getContentBottom() const { return this->content_bottom.value(); }
  
  void setPosition(float x, float y);
  void setPosition(glm::vec2 position);
  glm::vec2 getPosition() const { return glm::vec2(this->left.value(), this->top.value()); }
  void setWidth(float value);
  float getWidth() const { return this->width.value(); }
  float getContentWidth() const { return this->content_width.value(); }
  void setHeight(float value);
  float getHeight() const { return this->height.value(); }
  float getContentHeight() const { return this->content_height.value(); }
  void setSize(float w, float h);
  void setSize(glm::vec2 size);
  
  void setRect(const ofRectangle & rect);
  ofRectangle getRect() const { return getBounds(); }
  
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
  
  ofRectangle getBounds() const;
  ofRectangle getContentBounds() const;
  
  
  
#pragma mark - LAYOUT: Helpers
protected:  
  virtual void layoutSubviews();
  void layoutIfNeeded() {
    if (this->needsLayout) {
      this->layoutSubviews();
      this->needsLayout = false;
    }
  }
  void setNeedsLayout() { this->needsLayout = true; };
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
  
  std::vector<kiwi::Constraint> constraints;
  
  
  
#pragma mark - LAYOUT: Rendering
protected:
  void internalUpdate(double time, double delta);
  void internalDraw();
  
  bool isMaskEnabled { false };
  bool isDebugEnabled { false };
  bool shouldDrawSubviews { true };
  
//  ofRectangle bounds;
//  ofRectangle _getBounds() const;
  ofRectangle getLocalBounds() const;
  
//  ofRectangle contentBounds;
//  ofRectangle _getContentBounds() const;
  ofRectangle getLocalContentBounds() const;
  
  int layerZ { 0 };
  bool includeInOverlap { true };
  void updateBounds();
  
  
  
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
  
  inline static std::shared_ptr<View> focused = nullptr;
  bool isFocused() const { return View::focused != nullptr && View::focused.get() == this; }
  
  inline static std::set<std::shared_ptr<View>> everyView;
  
  int level { 0 };
  int getDepth() const;
  int getActualZ() const { return getDepth() + layerZ; }
  
  
  
#pragma mark - EVENTS
public:
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
  void windowResizedHandler(ofResizeEventArgs & e) { this->onWindowResized(e); }
  
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
    if (isMouseInside && isMousePressed) return ofxCortex::ui::Styling::State::ACTIVE;
    else if (isFocused()) return ofxCortex::ui::Styling::State::FOCUS;
    else if (isMouseInside) return ofxCortex::ui::Styling::State::HOVER;
    
    return ofxCortex::ui::Styling::State::IDLE;
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
  
protected:
  
  
  
#pragma mark - EVENTS: Handlers

//  virtual void _windowResized(ofResizeEventArgs &e) {
////    ofLogNotice(_getLogModule(__FUNCTION__)) << "Scale = " << Styling::getScale() << " Row height: " << Styling::defaultRowHeight().value() * Styling::getScale();
//    this->setIntrinsicHeight(Styling::getRowHeight());
//    LayoutEngine::setNeedsSolve();
//  }
};

}}
