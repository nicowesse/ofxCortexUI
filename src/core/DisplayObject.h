#pragma once

#include "ofMain.h"
#include "core/UUID.h"
#include "core/Stencil.h"

namespace ofxCortex { namespace ui {
  
  class DisplayObject : public enable_shared_from_this<DisplayObject> {
#pragma mark - Public
  public:
    
    DisplayObject();
    ~DisplayObject();
    
    static shared_ptr<DisplayObject> create() { return make_shared<DisplayObject>(); }
    template<typename T> shared_ptr<T> create() { return make_shared<T>(); }
    
    virtual void setName(string name) { _name = name; };
    const string & getName() const { return _name; };
    
    bool operator== (const DisplayObject& other) const { return this->_uuid == other._uuid; }
    bool compare(shared_ptr<DisplayObject> other) { return _uuid == other->_uuid; }
    
    const string getUUID() const { return _uuid.toString(); }
    const string getPrettyID() const { return (_name != "") ? _name : _uuid; }
    
  protected:
    
    string _name;
    ofxCortex::core::UUID _uuid;
    
    virtual string _getModule() const { return "DisplayObject"; }
    string _getLogModule() const { return _getModule() + "(\"" + getPrettyID() + "\")"; }
    
    
    
#pragma mark - Positioning and Sizing
  public:
    
    const glm::vec2 getPosition() const { return _rect.getPosition(); }
    virtual DisplayObject* setPosition(const glm::vec2 & position);
    virtual DisplayObject* setPosition(float x, float y) { return this->setPosition(glm::vec2(x, y)); }
    virtual DisplayObject* translate(const glm::vec2 & delta) { _rect.translate(delta); return this; }
    
    float getX() const { return _rect.x; }
    float getLeft() const { return _rect.getLeft(); }
    virtual DisplayObject* setX(float x) { _rect.x = x; return this; }
    virtual DisplayObject* setLeft(float left, bool adjustLayout = true);
    
    float getY() const { return _rect.y; }
    float getTop() const { return _rect.getTop(); }
    virtual DisplayObject* setY(float y) { _rect.y = y; return this; }
    virtual DisplayObject* setTop(float top, bool adjustLayout = true);
    
    float centerX() const { return _rect.getCenter().x; }
    virtual DisplayObject* centerX(float x, bool adjustLayout = true);
    float centerY() const { return _rect.getCenter().x; }
    virtual DisplayObject* centerY(float y, bool adjustLayout = true);
    glm::vec2 center() const { return _rect.getCenter(); }
    virtual DisplayObject* center(glm::vec2 pos, bool adjustLayout = true);
    
    float getWidth() const { return _rect.width; }
    float getRight() const { return _rect.getRight(); }
    virtual DisplayObject* setWidth(float width, bool adjustLayout = true);
    virtual DisplayObject* setRight(float right, bool adjustLayout = true);
    virtual DisplayObject* alignRight(float right);
//    virtual DisplayObject* right(float x, bool adjustLayout = true) { setRight(x, adjustLayout); }
    
    float getHeight() const { return _rect.height; }
    float getBottom() const { return _rect.getBottom(); }
    virtual DisplayObject* setHeight(float height, bool adjustLayout = true);
    virtual DisplayObject* setBottom(float bottom, bool adjustLayout = true);
    virtual DisplayObject* alignBottom(float bottom);
    
    const glm::vec2 getSize() const { return { _rect.width, _rect.height }; };
    virtual DisplayObject* setSize(glm::vec2 size, bool adjustLayout = true);
    virtual DisplayObject* setSize(float x, float y, bool adjustLayout = true) { return this->setSize(glm::vec2(x, y), adjustLayout); }
    
    const ofRectangle & getRect() const { return _rect; }
    virtual DisplayObject* setRect(const ofRectangle & rect, bool adjustLayout = true);
    
    ofRectangle getGlobalRect();
    const ofRectangle & getLocalRect() { return _localRect; };
    
    const float getRotation() const { return _rotation; }
    DisplayObject* setRotation(float rotation) { _rotation = rotation; return this; }
    
    const glm::vec2 getScale() const { return _scale; }
    DisplayObject* setScale(glm::vec2 scale) { _scale = scale; return this; }
    
    glm::vec2 localToGlobal(glm::vec2 pos) const;
    glm::vec2 globalToLocal(glm::vec2 pos, bool includeSelf = true) const;
    
    bool isInsideRectangle(glm::vec2 pos, bool useGlobalRect = false);
    bool isInsideRectangle(const ofRectangle & rect, bool useGlobalRect = false);
    
    
  protected:
    ofRectangle _rect;
    ofRectangle _localRect {0, 0, 1, 1};
    glm::vec2 _scale {1.0, 1.0};
    float _rotation {0.0};
    
    void _updateRect();
    
    ofMatrix4x4 _getTransformationMatrix() const;
    ofMatrix4x4 _getInverseTransformationMatrix() const;
    
    glm::vec2 _getNormalizedCoordinates(const glm::vec2 & pos) const;
    
    
  
#pragma mark - Rendering and Layout
  public:
    
    virtual void draw();
    
    virtual void update();
    virtual void update(float time, float delta);
    
    void debug(bool debugChildren = false);
    void debugLocal(bool debugChildren = false);
    
    void enableMask() { _useMask = true; };
    void disableMask() { _useMask = false; };
    
    void enableChildRendering() { _doDrawChildren = true; }
    void disableChildRendering() { _doDrawChildren = false; }
    
  protected:
    
    float _time;
    float _deltaTime;
    bool _doDrawChildren { true };
    
    virtual void _preDraw() {};
    virtual void _draw();
    virtual void _postDraw() {};
    
    virtual void _drawChildren();
    virtual void _drawMask();
    
    virtual void _update();
    virtual void _update(float time, float deltaTime);
    
    virtual void _adjustLayout();
    
    virtual void _debug();
    
    // Stencil Mask
    bool _useMask { true };
    
    
    
    
    
#pragma mark - The Family
  public:
    
    void makeBaseObject();
    shared_ptr<DisplayObject> getBaseObject() { return (_isBaseObject) ? shared_from_this() : this->getParent()->getBaseObject(); }
    
    // Parents
    shared_ptr<DisplayObject> getParent() const { return _parent; }
    
    template<typename T> shared_ptr<T> getParent()
    {
      if (!hasParent()) return nullptr;
      
      shared_ptr<T> candidate = dynamic_pointer_cast<T>(getParent());
      
      if (candidate) return candidate;
      else return getParent()->getParent<T>();
    }
    
    void setParent(shared_ptr<DisplayObject> parent) { _parent = parent; }
    bool hasParent() const { return _parent != nullptr; }
    
    // Children
    vector<shared_ptr<DisplayObject>> children() const { return _children; }
    
    template<typename T> vector<shared_ptr<T>> children()
    {
      vector<shared_ptr<T>> candidates;
      
      for (shared_ptr<DisplayObject> child : _children)
      {
        auto childWithType = dynamic_pointer_cast<T>(child);
        if (childWithType) candidates.push_back(childWithType);
        
        vector<shared_ptr<T>> childCandidates = child->children<T>();
        candidates.insert(candidates.end(), childCandidates.begin(), childCandidates.end());
      }
      
      return candidates;
    }
    
    template<typename N> bool isOfType() { return dynamic_pointer_cast<N>(shared_from_this()); }
    
    size_t numChildren(bool recursive = false);
    unsigned int indexOfChild(shared_ptr<DisplayObject> child) { return find(_children.begin(), _children.end(), child) - _children.begin(); }
    shared_ptr<DisplayObject> childAt(unsigned int index)
    {
      if (index < 0 || index > numChildren(false)) ofLogWarning("Display Object::childAt()") << "Index '" << index << "' is outside range 0-" << numChildren(false) << ". Returning null."; return nullptr;
      
      return _children[index];
    }
    
    virtual void addChild(shared_ptr<DisplayObject> child);
    virtual void addChildren(vector<shared_ptr<DisplayObject>> children);
    virtual void addChildAt(shared_ptr<DisplayObject> child, unsigned int index);
    
    virtual void removeChild(shared_ptr<DisplayObject> child);
    virtual void removeChildAt(unsigned int index);
    virtual void removeAllChildren();
    

  protected:
    
    bool _isBaseObject;
    
    shared_ptr<DisplayObject> _parent;
    void _setParent(shared_ptr<DisplayObject> parent) { _parent = parent; }
    
    vector<shared_ptr<DisplayObject>> _children;
    
#pragma mark - Events and Interaction
    
  public:
    
    void enableMouseEvents();
    void disableMouseEvents();
    
    void enableKeyEvents();
    void disableKeyEvents();
    
    void enableEvents();
    void disableEvents();
    
    ofEvent<ofMouseEventArgs> onMousePressedE;
    void onMousePressed(function<void(ofMouseEventArgs)> callback);
    bool isMousePressed() { return _isMouseDown; }
    
    ofEvent<ofMouseEventArgs> onMouseDoublePressedE;
    void onMouseDoublePressed(function<void(ofMouseEventArgs)> callback);
    
    ofEvent<ofMouseEventArgs> onMouseReleasedE;
    void onMouseReleased(function<void(ofMouseEventArgs)> callback);
    
    ofEvent<ofMouseEventArgs> onMouseReleasedOutsideE;
    void onMouseReleasedOutside(function<void(ofMouseEventArgs)> callback);
    
    ofEvent<ofMouseEventArgs> onMouseMovedE;
    void onMouseMoved(function<void(ofMouseEventArgs)> callback);
    
    ofEvent<ofMouseEventArgs> onMouseDraggedE;
    void onMouseDragged(function<void(ofMouseEventArgs)> callback);
    
    ofEvent<ofMouseEventArgs> onMouseEnterE;
    void onMouseEnter(function<void(ofMouseEventArgs)> callback);
    bool isMouseOver(){ return _isMouseOver; };
    
    ofEvent<ofMouseEventArgs> onMouseExitE;
    void onMouseExit(function<void(ofMouseEventArgs)> callback);
    
    ofEvent<ofMouseEventArgs> onMouseScrolledE;
    void onMouseScrolled(function<void(ofMouseEventArgs)> callback);
    
    
    ofEvent<ofKeyEventArgs> onKeyPressedE;
    void onKeyPressed(function<void(ofKeyEventArgs)> callback);
    static bool isKeyPressed(int key) { return ofGetKeyPressed(key); };
    
    ofEvent<ofKeyEventArgs> onKeyReleasedE;
    void onKeyReleased(function<void(ofKeyEventArgs)> callback);
    
    ofEvent<uint32_t> onCharTypedE;
    void onCharTyped(function<void(uint32_t)> callback);
    
    void enableChildInteractionOutsideRect() { _isChildInteractionOutsideRectEnabled = true; }
    void disableChildInteractionOutsideRect() { _isChildInteractionOutsideRectEnabled = false; }
    
    
  protected:
    
    bool _isMouseDown { false };
    bool _isMousePressedInside { false };
    bool _isMouseOver { false };
    bool _hasEnteredWhileDragged { false };
    uint64_t _lastMousePressTime { 0 };
    int _doublePressInterval { 200 }; // ms
    
    bool _isChildInteractionOutsideRectEnabled { true };
    
    // Bulk-add listeners
    void _addUpdateEventListener();
    void _removeUpdateEventListener();
    
    void _addDrawEventListener();
    void _removeDrawEventListener();
    
    void _addMouseEventListeners();
    void _removeMouseEventListeners();
    
    void _addKeyEventListeners();
    void _removeKeyEventListeners();
    
//    void _addTouchEventListeners();
//    void _removeTouchEventListeners();
    
    ofEventListeners _eventListeners;
    
    // Handlers
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
    
    // Hooks
    virtual void _mousePressed(const ofMouseEventArgs & args);
    virtual void _mouseDoublePressed(const ofMouseEventArgs & args);
    virtual void _mouseReleased(const ofMouseEventArgs & args);
    virtual void _mouseReleasedOutside(const ofMouseEventArgs & args);
    virtual void _mouseMoved(const ofMouseEventArgs & e);
    virtual void _mouseDragged(const ofMouseEventArgs & e);
    virtual void _mouseScrolled(const ofMouseEventArgs & e);
    virtual void _mouseEnter(const ofMouseEventArgs & e);
    virtual void _mouseExit(const ofMouseEventArgs & e);
    
    virtual void _keyPressed(const ofKeyEventArgs & e);
    virtual void _keyReleased(const ofKeyEventArgs & e);
    virtual void _charTyped(const uint32_t &c);
    
    bool _isInsideLocalRect(glm::vec2 pos);
    bool _isHovering(glm::vec2 pos, bool isCovered = false);
    bool _isCovered(const glm::vec2 & pos);
  };
  
}}
