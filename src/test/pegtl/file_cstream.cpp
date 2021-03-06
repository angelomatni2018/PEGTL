// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <clocale>
#include <cstdio>

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      struct file_content : seq< TAOCPP_PEGTL_STRING( "dummy content" ), eol, discard >
      {
      };

      struct file_grammar : seq< rep_min_max< 11, 11, file_content >, eof >
      {
      };

      void unit_test()
      {
         const char* const filename = "src/test/pegtl/file_data.txt";
#if defined( _WIN32 )
         std::FILE* stream;
         ::fopen_s( &stream, filename, "rb" );
#else
         std::FILE* stream = std::fopen( filename, "rb" );
#endif
         TAOCPP_PEGTL_TEST_ASSERT( stream != nullptr );
         TAOCPP_PEGTL_TEST_ASSERT( parse_cstream< file_grammar >( stream, filename, 16 ) );
         std::fclose( stream );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
