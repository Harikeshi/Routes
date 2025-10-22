#ifndef CARTOGRAPHIC_DATUM_HPP
#define CARTOGRAPHIC_DATUM_HPP
//==============================================================================
typedef char const DatumElement;
typedef DatumElement *DatumReference;
typedef DatumElement DatumObject[];
//==============================================================================
#define CARTOGRAPHIC_DECLARE_DATUM(NAME)                                       \
/*===========================================================================*/\
namespace Cartographic {                                                       \
namespace Datum {                                                              \
/*===========================================================================*/\
extern DatumObject NAME;                                                       \
/*===========================================================================*/\
} }
//==============================================================================
#define CARTOGRAPHIC_DEFINE_DATUM(NAME)                                        \
/*===========================================================================*/\
namespace Cartographic {                                                       \
namespace Datum {                                                              \
/*===========================================================================*/\
DatumObject NAME = #NAME;                                                      \
/*===========================================================================*/\
} }
//==============================================================================
#define CARTOGRAPHIC_ADD_DATUM(NAME)                                           \
        CARTOGRAPHIC_DECLARE_DATUM(NAME)                                       \
        CARTOGRAPHIC_DEFINE_DATUM(NAME)
//==============================================================================
#endif //! CARTOGRAPHIC_DATUM_HPP