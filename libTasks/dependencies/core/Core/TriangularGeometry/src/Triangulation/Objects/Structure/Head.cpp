#include "Triangulation/Objects/Structure/Head.hpp"
#include "Triangulation/Objects/Structure/Link.hpp"
//==============================================================================
namespace TriangularGeometry
{
//==============================================================================
std::shared_ptr<Head> Head::create()
{
  std::shared_ptr<Head> head(new Head());
  head->mNext = head;
  head->mPrev = head;
  return head;
}
//==============================================================================
bool Head::isEmpty()
{
  return (mNext == shared_from_this());
}
//==============================================================================
std::shared_ptr<Link> Head::first()
{
  if (isEmpty())
    return nullptr;
  return std::dynamic_pointer_cast<Link>(mNext);
}
//==============================================================================
std::shared_ptr<Link> Head::last()
{
  if (isEmpty())
    return nullptr;
  return std::dynamic_pointer_cast<Link>(mPrev);
}
//==============================================================================
int Head::cardinal()
{
  int result(0);
  if (!isEmpty())
  {
    std::shared_ptr<Linkage> linkage = first();
    while (linkage != shared_from_this())
    {
      result++;
      linkage = linkage->mNext;
    }
  }
  return result;
}
//==============================================================================
void Head::clear()
{
  std::shared_ptr<Link> link = first();
  while (link != nullptr)
  {
    link->out();
    link = first();
  }
}
//==============================================================================
void Head::unlock()
{
  clear();
  mNext = nullptr;
  mPrev = nullptr;
}
//==============================================================================
void Head::lock()
{
  if ((!mPrev) && (!mNext))
  {
    mNext = shared_from_this();
    mPrev = shared_from_this();
  }
}
//==============================================================================
Head::Head()
	: Linkage()
{}
//==============================================================================
} // !TriangularGeometry
//==============================================================================
