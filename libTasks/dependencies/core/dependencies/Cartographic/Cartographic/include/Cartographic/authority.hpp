#ifndef CARTOGRAPHIC_AUTHORITY_HPP
#define CARTOGRAPHIC_AUTHORITY_HPP
//==============================================================================
typedef char const AuthorityElement;
typedef AuthorityElement *AuthorityReference;
typedef AuthorityElement AuthorityObject[];
//==============================================================================
#define CARTOGRAPHIC_DECLARE_AUTHORITY(NAME)                                   \
/*===========================================================================*/\
namespace Cartographic {                                                       \
namespace Authority {                                                          \
/*===========================================================================*/\
extern AuthorityObject NAME;                                                   \
/*===========================================================================*/\
} }
//==============================================================================
#define CARTOGRAPHIC_DEFINE_AUTHORITY(NAME)                                    \
/*===========================================================================*/\
namespace Cartographic {                                                       \
namespace Authority {                                                          \
/*===========================================================================*/\
AuthorityObject NAME = #NAME;                                                  \
/*===========================================================================*/\
} }
//==============================================================================
#define CARTOGRAPHIC_ADD_AUTHORITY(NAME)                                       \
        CARTOGRAPHIC_DECLARE_AUTHORITY(NAME)                                   \
        CARTOGRAPHIC_DEFINE_AUTHORITY(NAME)
//==============================================================================
#endif //! CARTOGRAPHIC_AUTHORITY_HPP
