//
//
//

#ifndef MNEME_ALLOCATOR_HPP
#define MNEME_ALLOCATOR_HPP

#include "mneme/memory_fwd.hpp"

#include "mneme/propagation_traits.hpp"
// using mneme::propagation_traits

#include "mneme/allocation_traits.hpp"
// using mneme::allocation_traits

#include <memory>
// using std::pointer_traits

#include <type_traits>
// using std::make_unsigned

#include <cstddef>
// using std::size_t

namespace mneme {

template <typename T, typename Storage>
struct allocator {
  template <typename U, typename S>
  friend struct allocator;

  using allocation_traits = allocation_traits<Storage>;
  using ptr_traits = std::pointer_traits<T *>;
  using prop_traits = propagation_traits<Storage>;

  using value_type = T;
  using size_type =
      typename std::make_unsigned<typename ptr_traits::difference_type>::type;
  using pointer = typename ptr_traits::pointer;
  using storage_type = Storage;

  using propagate_on_container_copy_assignment =
      typename prop_traits::propagate_on_container_copy_assignment;
  using propagate_on_container_move_assignment =
      typename prop_traits::propagate_on_container_move_assignment;
  using propagate_on_container_swap =
      typename prop_traits::propagate_on_container_swap;

  allocator(Storage &s) noexcept : storage(s) {}

  allocator(const allocator &other) noexcept : storage(other.storage) {}

  template <typename U>
  allocator(const allocator<U, Storage> &other) noexcept
      : storage(other.storage) {}

  // TODO maybe we can provide this using a reference wrapper
  allocator &operator=(const allocator &rhs) = delete;

  pointer allocate(size_type n, void * = 0) {
    return static_cast<pointer>(
        allocation_traits::allocate(storage, n * sizeof(T), alignof(T)));
  }

  void deallocate(pointer p, size_type n) noexcept {
    allocation_traits::deallocate(storage, p, n);
  }

 protected:
  storage_type &storage;
};

// comparison between any two allocators

template <typename T, typename StorageT, typename U, typename StorageU>
bool operator==(const allocator<T, StorageT> &lhs,
                const allocator<U, StorageU> &rhs) {
  return false;
}

template <typename T, typename StorageT, typename U, typename StorageU>
bool operator!=(const allocator<T, StorageT> &lhs,
                const allocator<U, StorageU> &rhs) {
  return !(lhs == rhs);
}

// comparison between an allocator and anything else

template <typename T, typename StorageT, typename Other>
bool operator==(const allocator<T, StorageT> &lhs, const Other &rhs) {
  return false;
}

template <typename T, typename StorageT, typename Other>
bool operator!=(const allocator<T, StorageT> &lhs, const Other &rhs) {
  return !(lhs == rhs);
}

}  // namespace mneme end

#endif  // header