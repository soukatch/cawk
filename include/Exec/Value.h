#pragma once

#include "Support/Sequence.h"
#include "Support/Support.h"
#include <cassert>
#include <unordered_map>

namespace cawk {

class Value {
public:
  enum ValueKind { VK_Number, VK_String, VK_Array, VK_Vector };

private:
  const ValueKind Kind;

protected:
  Value(ValueKind Kind) : Kind(Kind) {}

  static Value Create(ValueKind Kind) { return Value(Kind); }

public:
  ValueKind GetKind() const { return Kind; }
};

class Primitive : public Value {
protected:
  Primitive(ValueKind Kind) : Value(Kind) {}
};

class Composite : public Value {
protected:
  Composite(ValueKind Kind) : Value(Kind) {}
};

template <typename T, typename P, Value::ValueKind V>
class ValueType : public P {
private:
  T Data;

protected:
  ValueType(T Data) : P(V), Data(Data) {}
  ValueType() : P(V), Data({}) {}

public:
  static bool classof(auto &&X) { return X->GetKind() == V; }

  static ValueType Create(T Data) { return ValueType(Data); }
  static ValueType CreateEmpty() { return ValueType(); }

  T GetData() const { return Data; }
  void SetData(T D) { Data = D; }
};

using Number = ValueType<double, Primitive, Value::VK_Number>;
using String = ValueType<std::string, Primitive, Value::VK_Number>;
using Array = ValueType<std::unordered_map<Primitive *, Primitive *>, Composite,
                        Value::VK_Array>;
using Vector = ValueType<Sequence<Primitive *>, Composite, Value::VK_Vector>;

template <typename T> T *value_cast(Value *V) {
  if (isa<T>(V))
    return static_cast<T *>(V);

  if (!isa<Number>(V) && !isa<String>(V)) {
    assert(false && "invalid conversion source");
    return nullptr;
  }

  if constexpr (std::is_same_v<T, Number>)
    return Number::Create(
        ToFloat(static_cast<String *>(V)->GetData()).value_or(0));
  else if (std::is_same_v<T, String>)
    return String::Create(std::to_string(static_cast<Number *>(V)->GetData()));
  else
    static_assert(false && "invalid conversion target");
}

} // namespace cawk