#pragma once
#include <exception>
#include <string>
#include <xmemory>
#include <iostream>
#include <fstream>
#include <shush-format.hpp>

namespace shush {
namespace dump {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - MACROS- - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Message Assert.
#ifdef DEBUG
#define MASSERT(x, msg) \
  !(x) && ({throw shush::dump::Dump(#x, __FILE__, __LINE__, msg);})
#else
#define MASSERT(x, msg) ;
#endif
#ifdef DEBUG
#define ASSERTED \
   && ({throw shush::dump::Dump(#x, __FILE__, __LINE__, "");})
#else
#define ASSERTED ;
#endif

template <class T>
class OkOnConstructOnDestructClass {
  OkOnConstructOnDestructClass(T* this_ptr)
      : this_ptr(this_ptr) {
    this_ptr->Ok();
  }

  ~OkOnConstructOnDestructClass() {
    this_ptr->Ok();
  }

  T* this_ptr;
};

// Verify method.
#ifdef DEBUG
#define VERIFY \
  shush::dump::OkOnConstructOnDestructClass<decltype(*this)> v(this);
#else
#define VERIFY ;
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - -EXCEPTION CLASS- - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

inline const uint8_t  EXC_SIZE_EXPRESSION  = 128;
inline const uint8_t  EXC_SIZE_FILE_NAME   = 255;
inline const uint16_t EXC_SIZE_MESSAGE     = 2048;
inline const uint8_t  DUMP_EXCEPTION_COUNT = 16;

inline const char    DEFAULT_DUMP_NAME[26] = "crash-report-{date}.dump";
inline const uint8_t FILE_NAME_SIZE        = 50;

/**
 * @brief A class used in Dump as an element of a dump stack.
 */
class DumpElement {
public:
  /**
   * @note Mind the size limit for strings.
   */
  DumpElement(const char* const expression, const char* const file_name,
              uint32_t line_number, const char* const msg);
  DumpElement() = default;
  ~DumpElement() = default;

  DumpElement(DumpElement&);
  DumpElement& operator=(const DumpElement&);
  DumpElement(DumpElement&&) noexcept;
  DumpElement& operator=(DumpElement&&) noexcept;

  void LoadVars(const char* const expression, const char* const file_name,
                uint32_t line_number, const char* const msg);

  char     expression [EXC_SIZE_EXPRESSION]{};
  char     file_name  [EXC_SIZE_FILE_NAME ]{};
  uint32_t line_number                     {};
  char     msg        [EXC_SIZE_MESSAGE   ]{};
};

class Dump {
public:
  Dump(const char* const expression, const char* const file_name,
       uint32_t line_number, const char* const msg);
  Dump();
  ~Dump() = default;

  Dump(Dump&) = delete;
  Dump(Dump&&) = delete;
  Dump& operator=(const Dump&) = delete;
  Dump& operator=(Dump&&) = delete;

  void PushToStack(DumpElement&& exc);
  void EmplaceToStack(const char* const expression, const char* const file_name,
                      uint32_t line_number, const char* const msg);
  DumpElement PopFromStack();
  bool Empty();
  uint8_t Size() const;

  DumpElement dump_stack[DUMP_EXCEPTION_COUNT];

private:
  uint8_t cur_size_;
};

extern void HandleFinalDump(Dump& dump);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - -DUMPING- - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}
}