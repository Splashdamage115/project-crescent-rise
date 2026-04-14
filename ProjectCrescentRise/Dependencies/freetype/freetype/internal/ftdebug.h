/****************************************************************************
 *
 * ftdebug.h
 *
 *   Debugging and logging component (specification).
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
 *
 * IMPORTANT: A description of FreeType's debugging support can be
 *             found in 'docs/DEBUG.TXT'.  Read it if you need to use or
 *             understand this code.
 *
 */


#ifndef FTDEBUG_H_
#define FTDEBUG_H_


#include <ft2build.h>
#include FT_CONFIG_CONFIG_H
#include <freetype/freetype.h>

#include "compiler-macros.h"

#ifdef FT_DEBUG_LOGGING
#define DLG_STATIC
#include <dlg/output.h>
#include <dlg/dlg.h>

#include <freetype/ftlogging.h>
#endif 


FT_BEGIN_HEADER

  
  
  
#ifdef FT_DEBUG_LOGGING
#undef  FT_DEBUG_LEVEL_TRACE
#define FT_DEBUG_LEVEL_TRACE
#endif

  
  
  
#ifdef FT_DEBUG_LEVEL_TRACE
#undef  FT_DEBUG_LEVEL_ERROR
#define FT_DEBUG_LEVEL_ERROR
#endif


  /**************************************************************************
   *
   * Define the trace enums as well as the trace levels array when they are
   * needed.
   *
   */

#ifdef FT_DEBUG_LEVEL_TRACE

#define FT_TRACE_DEF( x )  trace_ ## x ,

  
  typedef enum  FT_Trace_
  {
#include <freetype/internal/fttrace.h>
    trace_count

  } FT_Trace;


  
  
  extern int*  ft_trace_levels;

#undef FT_TRACE_DEF

#endif 


  /**************************************************************************
   *
   * Define the FT_TRACE macro
   *
   * IMPORTANT!
   *
   * Each component must define the macro FT_COMPONENT to a valid FT_Trace
   * value before using any TRACE macro.
   *
   * To get consistent logging output, there should be no newline character
   * (i.e., '\n') or a single trailing one in the message string of
   * `FT_TRACEx` and `FT_ERROR`.
   */


  /*************************************************************************
   *
   * If FT_DEBUG_LOGGING is enabled, tracing messages are sent to dlg's API.
   * If FT_DEBUG_LOGGING is disabled, tracing messages are sent to
   * `FT_Message` (defined in ftdebug.c).
   */
#ifdef FT_DEBUG_LOGGING

  
#define FT_LOGGING_TAG( x )   FT_LOGGING_TAG_( x )
#define FT_LOGGING_TAG_( x )  #x

  
  
#define FT_LOGGING_TAGX( x, y )   FT_LOGGING_TAGX_( x, y )
#define FT_LOGGING_TAGX_( x, y )  #x ":" #y


#define FT_LOG( level, varformat )                                         \
          do                                                               \
          {                                                                \
            const char*  dlg_tag = FT_LOGGING_TAGX( FT_COMPONENT, level ); \
                                                                           \
                                                                           \
            ft_add_tag( dlg_tag );                                         \
            if ( ft_trace_levels[FT_TRACE_COMP( FT_COMPONENT )] >= level ) \
            {                                                              \
              if ( custom_output_handler != NULL )                         \
                FT_Logging_Callback varformat;                             \
              else                                                         \
                dlg_trace varformat;                                       \
            }                                                              \
            ft_remove_tag( dlg_tag );                                      \
          } while( 0 )

#else 

#define FT_LOG( level, varformat )                                         \
          do                                                               \
          {                                                                \
            if ( ft_trace_levels[FT_TRACE_COMP( FT_COMPONENT )] >= level ) \
              FT_Message varformat;                                        \
          } while ( 0 )

#endif 


#ifdef FT_DEBUG_LEVEL_TRACE

  
#define FT_TRACE_COMP( x )   FT_TRACE_COMP_( x )
#define FT_TRACE_COMP_( x )  trace_ ## x

#define FT_TRACE( level, varformat )  FT_LOG( level, varformat )

#else 

#define FT_TRACE( level, varformat )  do { } while ( 0 )      

#endif 


  /**************************************************************************
   *
   * @function:
   *   FT_Trace_Get_Count
   *
   * @description:
   *   Return the number of available trace components.
   *
   * @return:
   *   The number of trace components.  0 if FreeType 2 is not built with
   *   FT_DEBUG_LEVEL_TRACE definition.
   *
   * @note:
   *   This function may be useful if you want to access elements of the
   *   internal trace levels array by an index.
   */
  FT_BASE( FT_Int )
  FT_Trace_Get_Count( void );


  /**************************************************************************
   *
   * @function:
   *   FT_Trace_Get_Name
   *
   * @description:
   *   Return the name of a trace component.
   *
   * @input:
   *   The index of the trace component.
   *
   * @return:
   *   The name of the trace component.  This is a statically allocated
   *   C~string, so do not free it after use.  `NULL` if FreeType is not
   *   built with FT_DEBUG_LEVEL_TRACE definition.
   *
   * @note:
   *   Use @FT_Trace_Get_Count to get the number of available trace
   *   components.
   */
  FT_BASE( const char* )
  FT_Trace_Get_Name( FT_Int  idx );


  /**************************************************************************
   *
   * @function:
   *   FT_Trace_Disable
   *
   * @description:
   *   Switch off tracing temporarily.  It can be activated again with
   *   @FT_Trace_Enable.
   */
  FT_BASE( void )
  FT_Trace_Disable( void );


  /**************************************************************************
   *
   * @function:
   *   FT_Trace_Enable
   *
   * @description:
   *   Activate tracing.  Use it after tracing has been switched off with
   *   @FT_Trace_Disable.
   */
  FT_BASE( void )
  FT_Trace_Enable( void );


  /**************************************************************************
   *
   * You need two opening and closing parentheses!
   *
   * Example: FT_TRACE0(( "Value is %i", foo ))
   *
   * Output of the FT_TRACEX macros is sent to stderr.
   *
   */

#define FT_TRACE0( varformat )  FT_TRACE( 0, varformat )
#define FT_TRACE1( varformat )  FT_TRACE( 1, varformat )
#define FT_TRACE2( varformat )  FT_TRACE( 2, varformat )
#define FT_TRACE3( varformat )  FT_TRACE( 3, varformat )
#define FT_TRACE4( varformat )  FT_TRACE( 4, varformat )
#define FT_TRACE5( varformat )  FT_TRACE( 5, varformat )
#define FT_TRACE6( varformat )  FT_TRACE( 6, varformat )
#define FT_TRACE7( varformat )  FT_TRACE( 7, varformat )


  /**************************************************************************
   *
   * Define the FT_ERROR macro.
   *
   * Output of this macro is sent to stderr.
   *
   */

#ifdef FT_DEBUG_LEVEL_ERROR

  /**************************************************************************
   *
   * If FT_DEBUG_LOGGING is enabled, error messages are sent to dlg's API.
   * If FT_DEBUG_LOGGING is disabled, error messages are sent to `FT_Message`
   * (defined in ftdebug.c).
   *
   */
#ifdef FT_DEBUG_LOGGING

#define FT_ERROR( varformat )                                      \
          do                                                       \
          {                                                        \
            const char*  dlg_tag = FT_LOGGING_TAG( FT_COMPONENT ); \
                                                                   \
                                                                   \
            ft_add_tag( dlg_tag );                                 \
            dlg_trace varformat;                                   \
            ft_remove_tag( dlg_tag );                              \
          } while ( 0 )

#else 

#define FT_ERROR( varformat )  FT_Message varformat

#endif 


#else  

#define FT_ERROR( varformat )  do { } while ( 0 )      

#endif 


  /**************************************************************************
   *
   * Define the FT_ASSERT and FT_THROW macros.  The call to `FT_Throw` makes
   * it possible to easily set a breakpoint at this function.
   *
   */

#ifdef FT_DEBUG_LEVEL_ERROR

#define FT_ASSERT( condition )                                      \
          do                                                        \
          {                                                         \
            if ( !( condition ) )                                   \
              FT_Panic( "assertion failed on line %d of file %s\n", \
                        __LINE__, __FILE__ );                       \
          } while ( 0 )

#define FT_THROW( e )                                   \
          ( FT_Throw( FT_ERR_CAT( FT_ERR_PREFIX, e ),   \
                      __LINE__,                         \
                      __FILE__ )                      | \
            FT_ERR_CAT( FT_ERR_PREFIX, e )            )

#else 

#define FT_ASSERT( condition )  do { } while ( 0 )

#define FT_THROW( e )  FT_ERR_CAT( FT_ERR_PREFIX, e )

#endif 


  /**************************************************************************
   *
   * Define `FT_Message` and `FT_Panic` when needed.
   *
   */

#ifdef FT_DEBUG_LEVEL_ERROR

#include "stdio.h"  

  
  FT_BASE( void )
  FT_Message( const char*  fmt,
              ... );

  
  FT_BASE( void )
  FT_Panic( const char*  fmt,
            ... );

  
  FT_BASE( int )
  FT_Throw( FT_Error     error,
            int          line,
            const char*  file );

#endif 


  FT_BASE( void )
  ft_debug_init( void );


#ifdef FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * 'dlg' uses output handlers to control how and where log messages are
   * printed.  Therefore we need to define a default output handler for
   * FreeType.
   */
  FT_BASE( void )
  ft_log_handler( const struct dlg_origin*  origin,
                  const char*               string,
                  void*                     data );


  /**************************************************************************
   *
   * 1. `ft_default_log_handler` stores the function pointer that is used
   *    internally by FreeType to print logs to a file.
   *
   * 2. `custom_output_handler` stores the function pointer to the callback
   *    function provided by the user.
   *
   * It is defined in `ftdebug.c`.
   */
  extern dlg_handler            ft_default_log_handler;
  extern FT_Custom_Log_Handler  custom_output_handler;


  /**************************************************************************
   *
   * If FT_DEBUG_LOGGING macro is enabled, FreeType needs to initialize and
   * un-initialize `FILE*`.
   *
   * These functions are defined in `ftdebug.c`.
   */
  FT_BASE( void )
  ft_logging_init( void );

  FT_BASE( void )
  ft_logging_deinit( void );


  /**************************************************************************
   *
   * For printing the name of `FT_COMPONENT` along with the actual log we
   * need to add a tag with the name of `FT_COMPONENT`.
   *
   * These functions are defined in `ftdebug.c`.
   */
  FT_BASE( void )
  ft_add_tag( const char*  tag );

  FT_BASE( void )
  ft_remove_tag( const char*  tag );


  /**************************************************************************
   *
   * A function to print log data using a custom callback logging function
   * (which is set using `FT_Set_Log_Handler`).
   *
   * This function is defined in `ftdebug.c`.
   */
  FT_BASE( void )
  FT_Logging_Callback( const char*  fmt,
                       ... );

#endif 


FT_END_HEADER

#endif 



