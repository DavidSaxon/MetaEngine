/**
  * \file
  *
  * \brief A collection of `typedefs` for primitive types.
  *
  * \author David Saxon
  */
#ifndef CHAOSCORE_BASE_TYPES_HPP_
#define CHAOSCORE_BASE_TYPES_HPP_

#include "chaoscore/base/Preproc.hpp"

// If we are running documentation we will not include the platform directives
// and pretend this running under Unix based platform in order to pick up the
// c++ 11 style types for documentation
#ifdef IN_DOXYGEN
  #define CHAOS_OS_UNIX
#endif

// include for Unix
#ifdef CHAOS_OS_UNIX
    #include <cinttypes>
#endif

namespace chaos
{

#ifdef CHAOS_OS_UNIX


    // typedefs for the built-in C++ types

    /**
      * \brief a 8-bit signed integer type
      *
      * \note This may not necessarily `typedef` a `int8_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `signed char`.
      */
    typedef std::int8_t int8;

    /**
      * \brief a 8-bit unsigned integer type
      *
      * \note This may not necessarily `typedef` a `uint8_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `unsigned char`.
      */
    typedef std::uint8_t uint8;

    /**
      * \brief a 16-bit signed integer type
      *
      * \note This may not necessarily `typedef` a `int16_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `signed short`.
      */
    typedef std::int16_t int16;

    /**
      * \brief a 16-bit unsigned integer type
      *
      * \note This may not necessarily `typedef` a `uint16_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `unsigned short`.
      */
    typedef std::uint16_t uint16;

    /**
      * \brief a 32-bit signed integer type
      *
      * \note This may not necessarily `typedef` a `int32_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `signed int`.
      */
    typedef std::int32_t int32;

    /**
      * \brief a 32-bit unsigned integer type
      *
      * \note This may not necessarily `typedef` a `uint32_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `unsigned int`.
      */
    typedef std::uint32_t uint32;

    /**
      * \brief a 64-bit signed integer type
      *
      * \note This may not necessarily `typedef` a `int64_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` a `signed long`.
      */
    typedef std::int64_t int64;

    /**
      * \brief a 64-bit unsigned integer type
      *
      * \note This may not necessarily `typedef` a `uint64_t` as it is platform
      *       specific. If the platform cannot be resolved this will naively
      *       `typedef` an `unsigned long`.
      */
    typedef std::uint64_t uint64;

#elif defined( CHAOS_OS_WINDOWS )

    // typedef the windows types
    typedef signed   __int8  int8;
    typedef unsigned __int8  uint8;
    typedef signed   __int16 int16;
    typedef unsigned __int16 uint16;
    typedef signed   __int32 int32;
    typedef unsigned __int32 uint32;
    typedef signed   __int64 int64;
    typedef unsigned __int64 uint64;

#else

    // the platform has not been recognised, naive underlying types will have to
    // be used, and we will hope for the best.
    typedef signed   char  int8;
    typedef unsigned char  uint8;
    typedef signed   short int16;
    typedef unsigned short uint16;
    typedef signed   int   int32;
    typedef unsigned int   uint32;
    typedef signed   long int64;
    typedef unsigned long uint64;

#endif

} // namespace chaos

#endif
