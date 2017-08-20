#ifndef ICSA_PRIVATE_MEMORY_ARENA
#define ICSA_PRIVATE_MEMORY_ARENA

#include "memory_arena.hpp"

#include <cstddef>
// using std::size_t

#include <new>
// using std::bad_alloc

#include <memory>
// using std::align

#include <sys/mman.h>
// using mmap
// using munmap

namespace icsa {

template <std::size_t N>
struct private_memory_arena : public memory_arena<N> {
  private_memory_arena() {
    m_base = mmap(NULL, N, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
                  -1, 0);

    if (!m_base) throw std::bad_alloc();
  }

  private_memory_arena(const private_memory_arena &) = delete;
  private_memory_arena &operator=(const private_memory_arena &) = delete;

  ~private_memory_arena() { munmap(m_base, m_size); }

  char *allocate(std::size_t n, std::size_t a) {
    if (m_offset + n >= m_size) return nullptr;

    auto *ptr = m_base + offset;
    auto space = m_size - m_offset - 1;
    auto modified_space = space;

    auto *rptr = std::align(a, n, ptr, modified_space);

    if (rptr) {
      auto added = space - modified_space + n;
      m_offset += added;
    }

    return rptr;
  }

  void deallocate(char *ptr, std::size_t) {}
};

}  // namespace icsa end

#endif  // header