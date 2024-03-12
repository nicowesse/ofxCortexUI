#pragma once

#include "kiwi.h"
#include "ofEvents.h"
#include "ofLog.h"

namespace ofxCortex { namespace ui {

class LayoutEngine {
public:
  static void addConstraint(const kiwi::Constraint& constraint) { get().solver.addConstraint(constraint); get().setNeedsSolve(); }
  static void addConstraints(const std::vector<kiwi::Constraint>& constraints) { for (auto & constraint : constraints) get().addConstraint(constraint); get().setNeedsSolve(); }
  
  static void removeConstraint(const kiwi::Constraint& constraint) { if (get().solver.hasConstraint(constraint)) get().solver.removeConstraint(constraint); get().setNeedsSolve(); }
  static void removeConstraints(const std::vector<kiwi::Constraint> & constraints) { for (auto & constraint : constraints) get().removeConstraint(constraint); get().setNeedsSolve(); }
  
  static void suggestValue(const kiwi::Variable& variable, double value)
  {
    if (!get().solver.hasEditVariable(variable))
    {
      ofLogWarning(__PRETTY_FUNCTION__) << "No edit variable exists for '" << variable.name() << "'";
    }
    
    get().solver.suggestValue(variable, value);
    get().setNeedsSolve();
  }
  
  static void addEditVariable(const kiwi::Variable& variable, double strength) { get().solver.addEditVariable(variable, strength); }
  static void removeEditVariable(const kiwi::Variable& variable) { get().solver.removeEditVariable(variable); }
  
  static const kiwi::Variable & getScreenWidth() { return get().screen_w; }
  static const kiwi::Variable & getScreenHeight() { return get().screen_h; }
  
  static void setNeedsSolve() { get().needSolve = true; }
  static void forceSolve()
  {
    get().setNeedsSolve();
    ofEventArgs e;
    get().updateHandler(e);
  }
  
  static ofEvent<void> & onSolveE() { return get().onSolve; }
  
  
private:
  LayoutEngine() {
    ofLogVerbose("🚒 LayoutEngine");
    
    ofAddListener(ofEvents().update, this, &LayoutEngine::updateHandler, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().windowResized, this, &LayoutEngine::onWindowResized, OF_EVENT_ORDER_APP);
    
    solver.addEditVariable(screen_w, kiwi::strength::strong);
    solver.suggestValue(screen_w, ofGetWidth());
    solver.addEditVariable(screen_h, kiwi::strength::strong);
    solver.suggestValue(screen_h, ofGetHeight());
    
    needSolve = true;
  };
  
  ~LayoutEngine() {
    ofRemoveListener(ofEvents().update, this, &LayoutEngine::updateHandler);
    ofRemoveListener(ofEvents().windowResized, this, &LayoutEngine::onWindowResized);
  }
  
  LayoutEngine(const LayoutEngine&) = delete;
  LayoutEngine& operator=(const LayoutEngine&) = delete;
  
  static LayoutEngine& get()
  {
    static LayoutEngine instance;
    return instance;
  }
  
  void updateHandler(ofEventArgs &e)
  {
    if (this->needSolve)
    {
      ofLogVerbose("🚒 LayoutEngine") << "updateHandler()";
      
      solver.updateVariables();
      onSolve.notify(this);
      
      this->needSolve = false;
    }
  }
  
  void onWindowResized(ofResizeEventArgs &e)
  {
    ofLogVerbose("🚒 LayoutEngine::" + std::string(__FUNCTION__) + "()");
    
    solver.suggestValue(screen_w, e.width);
    solver.suggestValue(screen_h, e.height);
    needSolve = true;
  }
  
  kiwi::Solver solver;
  kiwi::Variable screen_w { "screen_w" };
  kiwi::Variable screen_h { "screen_h" };
  ofEvent<void> onSolve;
  
  bool needSolve { true };
};

}}
