// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_FILE_PARSER_HPP
#define TAOCPP_PEGTL_INCLUDE_FILE_PARSER_HPP

#include "config.hpp"
#include "read_parser.hpp"

#if defined( __unix__ ) || ( defined( __APPLE__ ) && defined( __MACH__ ) )
#include <unistd.h>  // Required for _POSIX_MAPPED_FILES
#endif

#if defined( _POSIX_MAPPED_FILES )
#include "mmap_parser.hpp"
#endif

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
#if defined( _POSIX_MAPPED_FILES )
      using file_parser = mmap_parser;
      template< typename Eol >
      using basic_file_parser = basic_mmap_parser< Eol >;
#else
      using file_parser = read_parser;
      template< typename Eol >
      using basic_file_parser = basic_read_parser< Eol >;
#endif

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_file( const std::string& filename, States&&... st )
      {
         return file_parser( filename ).parse< Rule, Action, Control, A, M >( st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_file_nested( const Outer& oi, const std::string& filename, States&&... st )
      {
         return basic_file_parser< typename Outer::eol_t >( filename ).template parse_nested< Rule, Action, Control, A, M >( oi, st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
