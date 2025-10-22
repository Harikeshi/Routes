#ifndef CARTOGRAPHIC_ELLIPSOID_HPP
#define CARTOGRAPHIC_ELLIPSOID_HPP
//==============================================================================
#include <Cartographic/iellipsoid.hpp>
//==============================================================================
namespace Cartographic
{
namespace Ellipsoid
{
//==============================================================================
class Unknown: public IEllipsoid
{
public:
  Unknown(Meters a, Units f);

  Meters A() const;
  Units E2() const;

private:
  Meters a_;
  Units e2_;
};
//==============================================================================
} // !Ellipsoid
//==============================================================================
class Sphere: public IEllipsoid
{
public:
  explicit Sphere(Meters r);

  Meters A() const;
  Units E2() const;

private:
  Meters r_;
};
//==============================================================================
} // !Cartographic
//==============================================================================
#define CARTOGRAPHIC_DECLARE_ELLIPSOID(NAME)                                   \
/*===========================================================================*/\
namespace Cartographic {                                                       \
namespace Ellipsoid {                                                          \
/*===========================================================================*/\
class NAME: public IEllipsoid                                                  \
{                                                                              \
public:                                                                        \
  static Meters const a;                                                       \
  static Units const f, e2;                                                    \
                                                                               \
  Meters A() const { return a;  }                                              \
  Units E2() const { return e2; }                                              \
  Units F() const  { return f;  }                                              \
};                                                                             \
/*===========================================================================*/\
} }
//==============================================================================
#define CARTOGRAPHIC_DEFINE_ELLIPSOID(NAME, EA, EF)                            \
/*===========================================================================*/\
namespace Cartographic {                                                       \
namespace Ellipsoid {                                                          \
/*===========================================================================*/\
Meters const NAME::a(EA);                                                      \
Units const NAME::f(EF), NAME::e2((2. - (EF)) * (EF));                         \
/*===========================================================================*/\
} }
//==============================================================================
#define CARTOGRAPHIC_ADD_ELLIPSOID(NAME, EA, EF)                               \
        CARTOGRAPHIC_DECLARE_ELLIPSOID(NAME)                                   \
        CARTOGRAPHIC_DEFINE_ELLIPSOID(NAME, EA, EF)
//==============================================================================
#endif //! CARTOGRAPHIC_ELLIPSOID_HPP
