// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_DUSELTRONIK_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_DUSELTRONIK_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

#include "dusel_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   dusel_mode = dusel_mode::NOTHING >
         struct duseltronik;

         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control >
         struct duseltronik< Rule, A, M, Action, Control, dusel_mode::NOTHING >
         {
            template< typename Input, typename... States >
            static auto match( Input& in, States&&... st ) -> decltype( Rule::template match< A, M, Action, Control >( in, st... ), true )
            {
               return Rule::template match< A, M, Action, Control >( in, st... );
            }

            // NOTE: The additional "int = 0" is a work-around for missing expression SFINAE in VS2015.

            template< typename Input, typename... States, int = 0 >
            static auto match( Input& in, States&&... ) -> decltype( Rule::match( in ), true )
            {
               return Rule::match( in );
            }
         };

         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control >
         struct duseltronik< Rule, A, M, Action, Control, dusel_mode::CONTROL >
         {
            template< typename Input, typename... States >
            static bool match( Input& in, States&&... st )
            {
               Control< Rule >::start( const_cast< const Input& >( in ), st... );

               if( duseltronik< Rule, A, M, Action, Control, dusel_mode::NOTHING >::match( in, st... ) ) {
                  Control< Rule >::success( const_cast< const Input& >( in ), st... );
                  return true;
               }
               Control< Rule >::failure( const_cast< const Input& >( in ), st... );
               return false;
            }
         };

         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control >
         struct duseltronik< Rule, A, M, Action, Control, dusel_mode::CONTROL_AND_APPLY >
         {
            template< typename Input, typename... States >
            static bool match( Input& in, States&&... st )
            {
               auto m = in.template mark< rewind_mode::REQUIRED >();

               if( duseltronik< Rule, A, rewind_mode::ACTIVE, Action, Control, dusel_mode::CONTROL >::match( in, st... ) ) {
                  Control< Rule >::template apply< typename Input::action_t, Action >( m.iterator(), in.iterator(), in.source(), st... );
                  return m( true );
               }
               return false;
            }
         };

         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control >
         struct duseltronik< Rule, A, M, Action, Control, dusel_mode::CONTROL_AND_APPLY0 >
         {
            template< typename Input, typename... States >
            static bool match( Input& in, States&&... st )
            {
               if( duseltronik< Rule, A, M, Action, Control, dusel_mode::CONTROL >::match( in, st... ) ) {
                  Control< Rule >::template apply0< Action >( st... );
                  return true;
               }
               return false;
            }
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
