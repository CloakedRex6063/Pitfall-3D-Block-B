#pragma once

#define SCRWIDTH 1280.f
#define SCRHEIGHT 720.f

#if defined(_MSC_VER)
#define WARNING_DISABLE \
  __pragma(warning(push, 0)) \
  __pragma(warning(disable: 4701)) \
  __pragma(warning(disable: 4702))
#define WARNING_ENABLE \
  __pragma(warning(default: 4701)) \
  __pragma(warning(default: 4702)) \
  __pragma(warning(pop ))
#elif defined(__GNUC__) || defined(__clang__)
#define WARNING_DISABLE \
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wunused-result\"") \
  _Pragma("GCC diagnostic ignored \"-Wreorder\"") \
  _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"") \
  _Pragma("GCC diagnostic ignored \"-Wformat\"") \
  _Pragma("GCC diagnostic ignored \"-Wswitch\"") \
  _Pragma("GCC diagnostic ignored \"-Wmisleading-indentation\"") \
  _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"") \
  _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"") \
  _Pragma("GCC diagnostic ignored \"-Wint-in-bool-context\"") \
  _Pragma("GCC diagnostic ignored \"-Wunused-variable\"") \
  _Pragma("GCC diagnostic ignored \"-Wpointer-arith\"")
#define WARNING_ENABLE _Pragma("GCC diagnostic pop")
#else
#define WARNING_DISABLE
#define WARNING_ENABLE
#endif