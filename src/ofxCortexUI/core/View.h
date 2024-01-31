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
public:
//  View();
  View(std::string name);
  virtual ~View();
  
  static std::shared_ptr<View> create(const std::string & name);
  
  void update();
  void update(double time, double delta);
  void enableAutoUpdate();
  void disableAutoUpdate();
  
  void draw();
  void enableSubviewRendering() { _shouldDrawSubviews = true; }
  void disableSubviewRendering() { _shouldDrawSubviews = false; }
  
  void enableMask() { _enableMask = true; }
  void disableMask() { _enableMask = false; }
  
  void enableDebug() { _enableDebug = true; }
  void disableDebug() { _enableDebug = false; }
  
  
#pragma mark - OBJECT: Initialisation and Loops
protected:
  void _setup();
  
  virtual void _update(double time, double delta);
  
  virtual void _preDraw();
  virtual void _draw();
  virtual void _postDraw();
  
  virtual void _drawMask();
  bool _enableMask { false };
  
  virtual void _debug();
  bool _enableDebug { false };
  
  virtual void _drawSubviews();
  bool _shouldDrawSubviews { true };
  
  
#pragma mark - OBJECT: Identification
public:
  virtual void setName(std::string name) { _name = name; }
  std::string getName() const { return _name; }
  
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
  
  bool isInside(const glm::vec2 & point);
  bool isOverlapped(const glm::vec2 & point);
  void enableOverlap() { _includeInOverlap = true; }
  void disableOverlap() { _includeInOverlap = false; }
  
  
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
  void setNeedsLayout(bool affectSubviews = false);
  
protected:
  std::vector<kiwi::Constraint> constraints;
  virtual void _updateConstraints();
  void _updateConstraintsIfNeeded();
  void _updateConstraintsImmediately();
  void _setNeedsUpdateConstraints();
  bool _needsUpdateConstraints { false };
  
  
  virtual void _updateLayout();
  void _updateLayoutImmediately();
  void _layoutIfNeeded(bool layoutSubviews = true);
  bool _needsLayout { false };
  
  
#pragma mark - LAYOUT: Rendering
//  ofRectangle bounds;
//  ofRectangle _getBounds() const;
  ofRectangle _getLocalBounds() const;
  
//  ofRectangle contentBounds;
//  ofRectangle _getContentBounds() const;
  ofRectangle _getLocalContentBounds() const;
  
  int layerZ { 0 };
  bool _includeInOverlap { true };
  void _updateBounds();
  
  ofMatrix4x4 _getTransformationMatrix() const;
  ofMatrix4x4 _getInverseTransformationMatrix() const;
  
  
  
#pragma mark - Family Tree
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
  bool _isFocused() const { return View::focused != nullptr && View::focused.get() == this; }
  static std::vector<std::shared_ptr<View>> everyView;
  int level { 0 };
  int _getDepth() const;
  int _getActualZ() const { return _getDepth() + layerZ; }
  
  
  
#pragma mark - EVENTS
public:
  void enableEvents() {
    _addMouseEventListeners();
    _addKeyEventListeners();
  }
  void disableEvents() {
    _removeMouseEventListeners();
    _removeKeyEventListeners();
  }
  void enableMouseEvents() { _addMouseEventListeners(); }
  void disableMouseEvents() { _removeMouseEventListeners(); }
  
  void enableInteractionOutsideParent() { this->_enableInteractionOutsideParent = true; }
  void disableInteractionOutsideParent() { this->_enableInteractionOutsideParent = false; }
  
  struct MouseEventArgs : public ofMouseEventArgs {
    MouseEventArgs() : ofMouseEventArgs() {};
    MouseEventArgs(const ofMouseEventArgs & e) : ofMouseEventArgs(e) {};
    MouseEventArgs& operator= (const MouseEventArgs &other)
    {
      ofMouseEventArgs::operator=(other);
      isOverlapped = other.isOverlapped;
      return *this;
    }
    
    bool isOverlapped { false };
  };
  
  struct DeltaMouseEvent : public MouseEventArgs {
    DeltaMouseEvent() : MouseEventArgs() {};
    DeltaMouseEvent(const ofMouseEventArgs & e) : MouseEventArgs(e) {};
    DeltaMouseEvent(const DeltaMouseEvent & e) : MouseEventArgs(e), delta(e.delta) {};
    DeltaMouseEvent& operator= (const DeltaMouseEvent &other)
    {
      MouseEventArgs::operator=(other);
      delta = other.delta;
      return *this;
    }
    
    glm::vec2 delta;
  };
  
  ofxCortex::ui::Styling::State getMouseState() const {
    if (_isMouseOver && _isMousePressed) return ofxCortex::ui::Styling::State::ACTIVE;
    else if (_isFocused()) return ofxCortex::ui::Styling::State::FOCUS;
    else if (_isMouseOver) return ofxCortex::ui::Styling::State::HOVER;
    
    return ofxCortex::ui::Styling::State::IDLE;
  }
  
  float getHoverIntensity() const { return _mouseHoverIntensity; }
  float getActiveIntensity() const { return _mouseActiveIntensity; }
  
  
  
  ofEvent<MouseEventArgs> onMousePressedE;
  void onMousePressed(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMousePressedE.newListener(callback, 0)); }
  bool isMousePressed() { return _isMousePressed; }
  
  ofEvent<MouseEventArgs> onMouseDoublePressedE;
  void onMouseDoublePressed(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMouseDoublePressedE.newListener(callback, 0)); }
  
  ofEvent<MouseEventArgs> onMouseReleasedE;
  void onMouseReleased(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMouseReleasedE.newListener(callback, 0)); }
  
  ofEvent<MouseEventArgs> onMouseReleasedOutsideE;
  void onMouseReleasedOutside(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMouseReleasedOutsideE.newListener(callback, 0)); }
  
  ofEvent<DeltaMouseEvent> onMouseMovedE;
  void onMouseMoved(std::function<void(DeltaMouseEvent)> callback) { _eventListeners.push(onMouseMovedE.newListener(callback, 0)); }
  
  ofEvent<DeltaMouseEvent> onMouseDraggedE;
  void onMouseDragged(std::function<void(DeltaMouseEvent)> callback) { _eventListeners.push(onMouseDraggedE.newListener(callback, 0)); }
  
  ofEvent<MouseEventArgs> onMouseEnterE;
  void onMouseEnter(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMouseDraggedE.newListener(callback, 0)); }
  bool isMouseOver(){ return _isMouseOver; };
  
  ofEvent<MouseEventArgs> onMouseExitE;
  void onMouseExit(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMouseExitE.newListener(callback, 0)); }
  
  ofEvent<MouseEventArgs> onMouseScrolledE;
  void onMouseScrolled(std::function<void(MouseEventArgs)> callback) { _eventListeners.push(onMouseScrolledE.newListener(callback, 0)); }
  
  
  ofEvent<ofKeyEventArgs> onKeyPressedE;
  void onKeyPressed(std::function<void(ofKeyEventArgs)> callback) { _eventListeners.push(onKeyPressedE.newListener(callback, 0)); }
  static bool isKeyPressed(int key) { return ofGetKeyPressed(key); };
  
  ofEvent<ofKeyEventArgs> onKeyReleasedE;
  void onKeyReleased(std::function<void(ofKeyEventArgs)> callback) { _eventListeners.push(onKeyReleasedE.newListener(callback, 0)); }
  
  ofEvent<uint32_t> onCharTypedE;
  void onCharTyped(std::function<void(uint32_t)> callback) { _eventListeners.push(onCharTypedE.newListener(callback, 0)); }
  
protected:
  void _addUpdateEventListener();
  void _removeUpdateEventListener();
  
  void _addDrawEventListener();
  void _removeDrawEventListener();
  
  void _addWindowResizeEventListener();
  void _removeWindowResizeEventListener();
  
  void _addMouseEventListeners();
  void _removeMouseEventListeners();
  
  void _addKeyEventListeners();
  void _removeKeyEventListeners();
  
  //    void _addTouchEventListeners();
  //    void _removeTouchEventListeners();
  
  bool _isMousePressed { false };
  float _mouseActiveIntensity { 0.0f };
  
  bool _isMouseOver { false };
  float _mouseHoverIntensity { 0.0f };
  
  bool _wasMousePressedInside { false };
  bool _wasMouseReleasedInside { false };
  int _lastMousePressTime { 0 };
  int _doublePressInterval { 500 };
  glm::vec2 _lastMousePosition { 0, 0 };
  
  bool _enableInteractionOutsideParent { true };
  
  ofEventListeners _eventListeners;
  
#pragma mark - EVENTS: Handlers
  void _updateHandler(ofEventArgs &e);
  void _drawHandler(ofEventArgs &e);
  void _mousePressedHandler(ofMouseEventArgs &e);
  void _mouseReleasedHandler(ofMouseEventArgs &e);
  void _mouseMovedHandler(ofMouseEventArgs &e);
  void _mouseDraggedHandler(ofMouseEventArgs &e);
  void _mouseScrolledHandler(ofMouseEventArgs &e);
  void _keyPressedHandler(ofKeyEventArgs &e);
  void _keyReleasedHandler(ofKeyEventArgs &e);
  void _charHandler(uint32_t &c);
  
  // Trigger Helpers
  void _triggerMousePressed(MouseEventArgs & args) {
    onMousePressedE.notify(args);
    _mousePressed(args);
  }
  
  void _triggerMouseDoublePressed(MouseEventArgs & args) {
    onMouseDoublePressedE.notify(args);
    _mouseDoublePressed(args);
  }
  
  void _triggerMouseReleased(MouseEventArgs & args) {
    onMouseReleasedE.notify(args);
    _mouseReleased(args);
  }
  
  void _triggerMouseReleasedOutside(MouseEventArgs & args) {
    onMouseReleasedOutsideE.notify(args);
    _mouseReleasedOutside(args);
  }
  
  void _triggerMouseMoved(DeltaMouseEvent & args) {
    onMouseMovedE.notify(args);
    _mouseMoved(args);
  }
  
  void _triggerMouseDragged(DeltaMouseEvent & args) {
    onMouseDraggedE.notify(args);
    _mouseDragged(args);
  }
  
  void _triggerMouseScrolled(MouseEventArgs & args) {
    onMouseScrolledE.notify(args);
    _mouseScrolled(args);
  }
  
  void _triggerMouseEnter(MouseEventArgs & args) {
    onMouseEnterE.notify(args);
    _mouseEnter(args);
  }
  
  void _triggerMouseExit(MouseEventArgs & args) {
    onMouseExitE.notify(args);
    _mouseExit(args);
  }
  
  void _triggerKeyPressed(ofKeyEventArgs & args) {
    onKeyPressedE.notify(args);
    _keyPressed(args);
  }
  
  void _triggerKeyReleased(ofKeyEventArgs & args) {
    onKeyReleasedE.notify(args);
    _keyReleased(args);
  }
  
  void _triggerCharTyped(uint32_t & args) {
    onCharTypedE.notify(args);
    _charTyped(args);
  }
  
  // Hooks
  virtual void _mousePressed(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y << " Is overlapped = " << e.isOverlapped;
  }
  virtual void _mouseDoublePressed(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y << " Is overlapped = " << e.isOverlapped;
  }
  virtual void _mouseReleased(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y << " Is overlapped = " << e.isOverlapped;
  }
  virtual void _mouseReleasedOutside(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y << " Is overlapped = " << e.isOverlapped;
  }
  virtual void _mouseMoved(const DeltaMouseEvent & e) {
    //    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y << " delta: " << e.delta.x << "," << e.delta.y;
  }
  virtual void _mouseDragged(const DeltaMouseEvent & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y << " delta: " << e.delta.x << "," << e.delta.y;
  }
  virtual void _mouseScrolled(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.scrollX << "," << e.scrollY;
  }
  virtual void _mouseEnter(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y;
  }
  virtual void _mouseExit(const MouseEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.x << "," << e.y;
  }
  
  virtual void _keyPressed(const ofKeyEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.key;
  }
  virtual void _keyReleased(const ofKeyEventArgs & e) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << e.key;
  }
  virtual void _charTyped(const uint32_t &c) {
    ofLogVerbose(_getLogModule(__FUNCTION__)) << c;
  }
  
  virtual void _windowResized(ofResizeEventArgs &e) {
//    ofLogNotice(_getLogModule(__FUNCTION__)) << "Scale = " << Styling::getScale() << " Row height: " << Styling::defaultRowHeight().value() * Styling::getScale();
    this->setIntrinsicHeight(Styling::getRowHeight());
    LayoutEngine::setNeedsSolve();
  }
};

}}
