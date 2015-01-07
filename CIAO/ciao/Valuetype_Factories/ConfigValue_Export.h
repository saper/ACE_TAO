
// -*- C++ -*-
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl ConfigValue_Factory
// ------------------------------
#ifndef CONFIGVALUE_FACTORY_EXPORT_H
#define CONFIGVALUE_FACTORY_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CONFIGVALUE_FACTORY_HAS_DLL)
#  define CONFIGVALUE_FACTORY_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CONFIGVALUE_FACTORY_HAS_DLL */

#if !defined (CONFIGVALUE_FACTORY_HAS_DLL)
#  define CONFIGVALUE_FACTORY_HAS_DLL 1
#endif /* ! CONFIGVALUE_FACTORY_HAS_DLL */

#if defined (CONFIGVALUE_FACTORY_HAS_DLL) && (CONFIGVALUE_FACTORY_HAS_DLL == 1)
#  if defined (CONFIGVALUE_FACTORY_BUILD_DLL)
#    define ConfigValue_Factory_Export ACE_Proper_Export_Flag
#    define CONFIGVALUE_FACTORY_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CONFIGVALUE_FACTORY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CONFIGVALUE_FACTORY_BUILD_DLL */
#    define ConfigValue_Factory_Export ACE_Proper_Import_Flag
#    define CONFIGVALUE_FACTORY_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CONFIGVALUE_FACTORY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CONFIGVALUE_FACTORY_BUILD_DLL */
#else /* CONFIGVALUE_FACTORY_HAS_DLL == 1 */
#  define ConfigValue_Factory_Export
#  define CONFIGVALUE_FACTORY_SINGLETON_DECLARATION(T)
#  define CONFIGVALUE_FACTORY_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CONFIGVALUE_FACTORY_HAS_DLL == 1 */

// Set CONFIGVALUE_FACTORY_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CONFIGVALUE_FACTORY_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CONFIGVALUE_FACTORY_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CONFIGVALUE_FACTORY_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CONFIGVALUE_FACTORY_NTRACE */

#if (CONFIGVALUE_FACTORY_NTRACE == 1)
#  define CONFIGVALUE_FACTORY_TRACE(X)
#else /* (CONFIGVALUE_FACTORY_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CONFIGVALUE_FACTORY_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CONFIGVALUE_FACTORY_NTRACE == 1) */

#endif /* CONFIGVALUE_FACTORY_EXPORT_H */

// End of auto generated file.
