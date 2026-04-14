/****************************************************************************
 *
 * config/mac-support.h
 *
 *   Mac/OS X support configuration header.
 *
 * Copyright (C) 1996-2025 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */
#ifndef FREETYPE_CONFIG_MAC_SUPPORT_H_
#define FREETYPE_CONFIG_MAC_SUPPORT_H_

  /**************************************************************************
   *
   * Mac support
   *
   *   This is the only necessary change, so it is defined here instead
   *   providing a new configuration file.
   */
#if defined( __APPLE__ ) || ( defined( __MWERKS__ ) && defined( macintosh ) )
  
  
  
#include <errno.h>
#ifdef ECANCELED 
#include "AvailabilityMacros.h"
#endif
#if defined( __LP64__ ) && \
    ( MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_4 )
#undef FT_MACINTOSH
#endif

#elif defined( __SC__ ) || defined( __MRC__ )
  
#include "ConditionalMacros.h"
#if TARGET_OS_MAC
#define FT_MACINTOSH 1
#endif

#endif  

#endif  
