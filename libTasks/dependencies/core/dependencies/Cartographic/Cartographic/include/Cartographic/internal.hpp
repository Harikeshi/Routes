#ifndef CARTOGRAPHIC_INTERNAL_HPP
#define CARTOGRAPHIC_INTERNAL_HPP
//==============================================================================
namespace Cartographic
{
//==============================================================================
template <typename T>
class InternalPtr
{
public:
  explicit InternalPtr(T *ptr): ptr_(ptr) {}
  ~InternalPtr();

  T *operator->()
  {
    return ptr_;
  }
  T const *operator->() const
  {
    return ptr_;
  }

  T &operator*()
  {
    return *ptr_;
  }
  T const &operator*() const
  {
    return *ptr_;
  }

private:
  T *const ptr_;

  InternalPtr(InternalPtr const &);
  InternalPtr &operator=(InternalPtr const &);
};
//==============================================================================
} // !Cartographic
//==============================================================================
#define CARTOGRAPHIC_INTERNAL(CLASS)                                           \
public:                                                                        \
  class Internal;                                                              \
protected:                                                                     \
  InternalPtr< CLASS::Internal> internal_;
//==============================================================================
#define CARTOGRAPHIC_INTERNAL_DESTRUCTOR(CLASS)                                \
/*===========================================================================*/\
template <>                                                                    \
InternalPtr< CLASS::Internal>::~InternalPtr() { delete ptr_; }
//==============================================================================
#endif //! CARTOGRAPHIC_INTERNAL_HPP