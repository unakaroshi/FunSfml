#pragma once
constexpr float PI = 3.1415926535897932;
constexpr float RAD2DEG = 180.0f / PI;
constexpr float DEG2RAD = PI / 180.0f;

class Degrees;

class Radians {
  float value;
public:
  constexpr explicit Radians(float radiansValue) :value(radiansValue) {}
  Radians(const Degrees& degreesValue); // defined below

  constexpr float getValue() const { return value; }


  constexpr Radians operator -() const { return Radians(-value); }
  constexpr Radians operator +(const Radians& rhs) const { return Radians(value + rhs.value); }
  constexpr Radians operator -(const Radians& rhs) const { return Radians(value - rhs.value); }
  constexpr Radians operator *(float rhs) const { return Radians(value * rhs); }
  constexpr Radians operator /(float rhs) const { return Radians(value / rhs); }
  constexpr float   operator /(const Radians& rhs) const { return (value / rhs.value); }
  constexpr friend Radians operator*(float f, const Radians& d) { return Radians(d.value * f); }
};

class Degrees {
  float value;
public:
  constexpr explicit Degrees(float degreesValue) :value(degreesValue) {}
  Degrees(const Radians& radiansValue) :value(radiansValue.getValue()* RAD2DEG) {}

  constexpr float getValue() const { return value; }
};

inline Radians::Radians(const Degrees& degreesValue) :value(degreesValue.getValue()* DEG2RAD) {}

inline Radians operator"" _rad(long double r) { return Radians(r); }
inline Radians operator"" _rad(unsigned long long r) { return Radians(r); }
inline Degrees operator"" _deg(long double d) { return Degrees(d); }
inline Degrees operator"" _deg(unsigned long long d) { return Degrees(d); }
