//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPE_HPP
#define BOOST_DI_CONCEPTS_SCOPE_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/push_back.hpp>
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/utility.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace concepts {

template<typename TScope>
class scope
{
    template<typename TRebind, typename T>
    struct rebind
        : TRebind::template rebind<T>
    { };

public:
    template<
        typename TExpected
      , typename TGiven
      , typename TContext = mpl::vector0<>
    >
    struct dependency
        : aux::dependency<
              mpl::_1
             , TExpected
             , TGiven
             , TContext
             , mpl::or_<
                  is_base_of<mpl::_1, TExpected>,
                  is_same<mpl::_1, TExpected>
               >
          >
        , aux::internal
    { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct bind
        : mpl::fold<
              mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
            , mpl::vector0<>
            , mpl::if_<
                  is_base_of<aux::internal, mpl::_2>
                , mpl::push_back<mpl::_1, rebind<mpl::_2, TScope> >
                , mpl::push_back<mpl::_1, rebind<dependency<mpl::_2, mpl::_2>, TScope> >
              >
          >::type
        , aux::internal
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(BOOST_MPL_LIMIT_VECTOR_SIZE, 1), mpl_::na BOOST_PP_INTERCEPT)>
        : mpl::fold<
              mpl::vector1<TExpected>
            , mpl::vector0<>
            , mpl::if_<
                  is_base_of<aux::internal, mpl::_2>
                , mpl::push_back<mpl::_1, rebind<mpl::_2, TScope> >
                , mpl::push_back<mpl::_1, rebind< dependency<mpl::_2, mpl::_2>, TScope> >
              >
          >::type
        , aux::internal
    {
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
        struct in_call_stack
            : mpl::fold<
                  mpl::vector1<TExpected>
                , mpl::vector0<>
                , mpl::if_<
                      is_base_of<aux::internal, mpl::_2>
                    , mpl::push_back<mpl::_1, rebind<mpl::_2, TScope> >
                    , mpl::push_back<mpl::_1, rebind< dependency<mpl::_2, mpl::_2, mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                  >
              >::type
            , aux::internal
        {
            template<typename TName>
            struct in_name
                : mpl::fold<
                      mpl::vector1<TExpected>
                    , mpl::vector0<>
                    , mpl::if_<
                          is_base_of<aux::internal, mpl::_2>
                        , mpl::push_back<mpl::_1, rebind<mpl::_2, TScope> >
                        , mpl::push_back<mpl::_1, rebind< dependency<named<mpl::_2, TName>, mpl::_2, mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                    >
                  >::type
                , aux::internal
            { };
        };

        template<typename TName>
        struct in_name
            : mpl::fold<
                  mpl::vector1<TExpected>
                , mpl::vector0<>
                , mpl::if_<
                      is_base_of<aux::internal, mpl::_2>
                    , mpl::push_back<mpl::_1, rebind<mpl::_2, TScope> >
                    , mpl::push_back<mpl::_1, rebind< dependency<named<mpl::_2, TName>, mpl::_2>, TScope> >
                  >
              >::type
            , aux::internal
        {
            template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
            struct in_call_stack
                : mpl::fold<
                    mpl::vector1<TExpected>
                  , mpl::vector0<>
                  , mpl::if_<
                        is_base_of<aux::internal, mpl::_2>
                      , mpl::push_back<mpl::_1, rebind<mpl::_2, TScope> >
                      , mpl::push_back<mpl::_1, rebind< dependency<named<mpl::_2, TName>, mpl::_2, mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> >, TScope> >
                    >
                  >::type
                , aux::internal
            { };
        };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif
