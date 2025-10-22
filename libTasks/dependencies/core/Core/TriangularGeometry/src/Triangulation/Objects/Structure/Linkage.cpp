#include "Triangulation/Objects/Structure/Linkage.hpp"
#include "Triangulation/Objects/Structure/Head.hpp"
//! std
#include <typeinfo>
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
std::shared_ptr<Linkage> Linkage::next()
{
  if (!(mNext))
    return nullptr;
  else if (typeid(*mNext) == typeid(Head))
    return nullptr;
  return mNext;
}
//==============================================================================
std::shared_ptr<Linkage> Linkage::previous()
{
  if (!(mPrev))
    return nullptr;
  else if (typeid(*mPrev) == typeid(Head))
    return nullptr;
  return mPrev;
}
//==============================================================================
Linkage::Linkage()
	: mNext(nullptr)
	, mPrev(nullptr)
{}
//==============================================================================
Linkage::~Linkage() {}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
