#include "Triangulation/Objects/Structure/Link.hpp"
#include "Triangulation/Objects/Structure/Head.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
void Link::out()
{
  if (mNext)
  {
    std::shared_ptr<Linkage> linkage = mNext;
    linkage->mPrev = mPrev;
    linkage = mPrev;
    linkage->mNext = mNext;
    mNext = nullptr;
    mPrev = nullptr;
  }
}
//==============================================================================
void Link::follow(std::shared_ptr<Link> link)
{
  out();
  std::shared_ptr<Linkage> linkage(nullptr);
  if (link)
    linkage = link;
  if (linkage->mNext)
  {
    mPrev = link;
    mNext = linkage->mNext;
    linkage->mNext = shared_from_this();
    linkage = mNext;
    linkage->mPrev = shared_from_this();
  }
}
//==============================================================================
void Link::precede(std::shared_ptr<Link> link)
{
  out();
  std::shared_ptr<Linkage> linkage(nullptr);
  if (link)
    linkage = link;
  if (linkage->mNext)
  {
    mNext = link;
    mPrev = linkage->mPrev;
    linkage->mPrev = shared_from_this();
    linkage = mPrev;
    linkage->mNext = shared_from_this();
  }
}
//==============================================================================
void Link::into(std::shared_ptr<Head>& head)
{
  out();
  if (head)
  {
    mNext = head;
    std::shared_ptr<Linkage> linkage = head;
    mPrev = linkage->mPrev;
    linkage->mPrev = shared_from_this();
    linkage = mPrev;
    linkage->mNext = shared_from_this();
  }
}
//==============================================================================
Link::~Link()
{
	out();
}
//==============================================================================
Link::Link() 
  : Linkage()
{
}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
