#pragma once

namespace ofxCortex { namespace ui {

class Constraint {
public:
  enum Attribute {
    LEFT,
    CENTER_X,
    RIGHT,
    TOP,
    CENTER_Y,
    BOTTOM,
    
    WIDTH,
    HEIGHT,
    
    NO_ATTRIBUTE
  };
  
  enum AttributeType {
    LOCATION_X,
    LOCATION_Y,
    SIZE_X,
    SIZE_Y,
    UNDEFINED
  };
  
  enum RelationOperator {
    EQUAL,
    LESS_THAN,
    GREATER_THAN
  };
  
  enum Priority {
    LOW = 250,
    MEDIUM = 500,
    HIGH = 750,
    VERY_HIGH = 999,
    REQUIRED = 1000
  };
  
  Constraint() = default;
  Constraint(shared_ptr<Component> trgt, Attribute targetAttr, shared_ptr<Component> src, Attribute sourceAttr, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : target(trgt), targetAttribute(targetAttr),
    source(src), sourceAttribute(sourceAttr),
    relation(relation),
    multiplier(multiplier), constant(constant) {};
  
  virtual void evaluate() = 0;
protected:
  float getSourceAttributeValue(Attribute attr) const
  {
    if (!source) return 0.0f;
    
    return 0.0f;
    
//    switch(attr) {
//      case LEFT: return source->left.value(); break;
//      case RIGHT: return source->right(); break;
//      case TOP: return source->top(); break;
//      case BOTTOM: return source->bottom(); break;
//      case CENTER_X: return source->centerX(); break;
//      case CENTER_Y: return source->centerY(); break;
//
//      case WIDTH: return source->getWidth(); break;
//      case HEIGHT: return source->getHeight(); break;
//      case NO_ATTRIBUTE: return 0.0f; break;
//    }
  }
  float getSourceAttributeValue() const { return getSourceAttributeValue(sourceAttribute); }
  
  string attributeToString(Attribute attr) const
  {
    switch(attr) {
      case LEFT: return "LEFT"; break;
      case RIGHT: return "RIGHT"; break;
      case TOP: return "TOP"; break;
      case BOTTOM: return "BOTTOM"; break;
      case CENTER_X: return "CENTER X"; break;
      case CENTER_Y: return "CENTER Y"; break;
        
      case WIDTH: return "WIDTH"; break;
      case HEIGHT: return "HEIGHT"; break;
        
      case NO_ATTRIBUTE: return "NO ATTRIBUTE"; break;
    }
  }
  
  bool isLocationAttribute(Attribute attr) const { return (getAttributeType(attr) == LOCATION_X || getAttributeType(attr) == LOCATION_Y); }
  bool isSizeAttribute(Attribute attr) const { return (attr == WIDTH || attr == HEIGHT); }
  bool isConstantAttribute(Attribute attr) const { return (attr == NO_ATTRIBUTE); }
  bool isOnSameAxis(Attribute attr1, Attribute attr2) const { return getAttributeType(attr1) == getAttributeType(attr2); }
  AttributeType getAttributeType(Attribute attr) const
  {
    switch(attr) {
      case LEFT: case CENTER_X: case RIGHT: return LOCATION_X; break;
      case TOP: case CENTER_Y: case BOTTOM: return LOCATION_Y; break;
      case WIDTH: return SIZE_X; break;
      case HEIGHT: return SIZE_Y; break;
      case NO_ATTRIBUTE: default: return UNDEFINED; break;
    }
  }
  
  
  friend ostream& operator<<(ostream& os, const Constraint& constraint)
  {
    os << "Target('" << ((constraint.target) ? constraint.target->getName() : "NULL") << "')." << constraint.attributeToString(constraint.targetAttribute);
    if (constraint.relation == EQUAL) os << " == ";
    else if (constraint.relation == LESS_THAN) os << " <= ";
    else if (constraint.relation == GREATER_THAN) os << " >= ";
    os << constraint.multiplier << " * ";
    os << "Source('" << ((constraint.source) ? constraint.source->getName() : "NULL") << "')." << constraint.attributeToString(constraint.sourceAttribute) << " (=" << constraint.getSourceAttributeValue(constraint.sourceAttribute) << ")";
    os << " + " << constraint.constant;
    return os;
  }
  
  float multiplier;
  float constant;
  
  int priority { Priority::MEDIUM };
  
protected:
  shared_ptr<Component> target;
  Attribute targetAttribute;
  
  shared_ptr<Component> source;
  Attribute sourceAttribute;
        
  RelationOperator relation;
  
};

class LocationConstraint : public Constraint {
public:
  LocationConstraint() = default;
  LocationConstraint(shared_ptr<Component> target, Attribute targetAttribute, shared_ptr<Component> source, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : Constraint(target, targetAttribute, source, sourceAttribute, relation, multiplier, constant) {};
  
  virtual void evaluate() = 0;
  
protected:
  bool constraintCheck(string type)
  {
    if (!target) { ofLogWarning(type) << "Target is not set. Returning.."; return false; }
    if (!source) { ofLogWarning(type) << "Source has to be defined. Cannot pull coordinates from The Ether. Returning.."; return false; }
    if (!isLocationAttribute(sourceAttribute)) { ofLogWarning(type) << "Source attribute [" << attributeToString(sourceAttribute) << "] has to be a location-attribute. Returning.."; return false; }
    if (!isOnSameAxis(targetAttribute, sourceAttribute)) { ofLogWarning(type) << "Source attribute [" << attributeToString(sourceAttribute) << "] has to be on the same axis as target [" << attributeToString(targetAttribute) << "]."; return false; }
    return true;
  }
};

class TopConstraint : public LocationConstraint {
public:
  TopConstraint() = default;
  
  TopConstraint(shared_ptr<Component> target, shared_ptr<Component> source, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::TOP, source, sourceAttribute, relation, multiplier, constant) {};
  
  TopConstraint(shared_ptr<Component> target, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::TOP, NULL, sourceAttribute, relation, multiplier, constant) {
    if (target && target->hasParent()) source = target->getParent<Component>();
  };
  
  virtual void evaluate() override
  {
    if (!constraintCheck("Top Constraint")) return;
    
    float value = multiplier * getSourceAttributeValue() + constant;
//    target->top(value);
  }
};

class LeftConstraint : public LocationConstraint {
public:
  LeftConstraint() = default;
  
  LeftConstraint(shared_ptr<Component> target, shared_ptr<Component> source, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::LEFT, source, sourceAttribute, relation, multiplier, constant) {};
  
  LeftConstraint(shared_ptr<Component> target, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::LEFT, NULL, sourceAttribute, relation, multiplier, constant) {
    if (target && target->hasParent()) source = target->getParent<Component>();
  };
  
  virtual void evaluate() override
  {
    if (!constraintCheck("Left Constraint")) return;
    
    float value = multiplier * getSourceAttributeValue() + constant;
//    target->left(value);
  }
};

class RightConstraint : public LocationConstraint {
public:
  RightConstraint() = default;
  
  RightConstraint(shared_ptr<Component> target, shared_ptr<Component> source, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::RIGHT, source, sourceAttribute, relation, multiplier, constant * -1.0) {};
  
  RightConstraint(shared_ptr<Component> target, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::RIGHT, NULL, sourceAttribute, relation, multiplier, constant * -1.0) {
    if (target && target->hasParent()) source = target->getParent<Component>();
  };
  
  
  virtual void evaluate() override
  {
    if (!constraintCheck("Right Constraint")) return;
    
    float value = multiplier * getSourceAttributeValue() + constant;
//    ofLogNotice("Left Constraint") << "Value: " << value;
//    target->right(value);
  }
};

class BottomConstraint : public LocationConstraint {
public:
  BottomConstraint() = default;
  
  BottomConstraint(shared_ptr<Component> target, shared_ptr<Component> source, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::BOTTOM, source, sourceAttribute, relation, multiplier, constant * -1.0) {};
  
  BottomConstraint(shared_ptr<Component> target, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : LocationConstraint(target, Attribute::BOTTOM, NULL, sourceAttribute, relation, multiplier, constant * -1.0) {
    if (target && target->hasParent()) source = target->getParent<Component>();
  };
  
  
  virtual void evaluate() override
  {
    if (!constraintCheck("Bottom Constraint")) return;
    
    float value = multiplier * getSourceAttributeValue() + constant;
//    ofLogNotice("Left Constraint") << "Value: " << value;
//    target->bottom(value);
  }
};

class HeightConstraint : public Constraint {
public:
  HeightConstraint() = default;
  
  HeightConstraint(shared_ptr<Component> target, shared_ptr<Component> source, Attribute sourceAttribute, RelationOperator relation = EQUAL, float multiplier = 1.0f, float constant = 0.0f)
  : Constraint(target, Attribute::HEIGHT, source, sourceAttribute, relation, multiplier, constant) {};
  
  virtual void evaluate() override
  {
    if (!target) { ofLogWarning("Height Constraint") << "Target is not set. Returning.."; return; }
    if (!isSizeAttribute(sourceAttribute) && !isConstantAttribute(sourceAttribute)) { ofLogWarning("Height Constraint") << "Source attribute [" << attributeToString(sourceAttribute) << "] has to be a size-attribute. Returning.."; return; }
    if (!isOnSameAxis(targetAttribute, sourceAttribute) && !isConstantAttribute(sourceAttribute)) { ofLogWarning("Height Constraint") << "Source attribute [" << attributeToString(sourceAttribute) << "] has to be on the same axis as target [SIZE_Y]. Returning.."; return; }
    
    float value = multiplier * getSourceAttributeValue() + constant;
//    ofLogNotice("Left Constraint") << "Value: " << value;
//    target->setHeight(value);
  }
};

}}
