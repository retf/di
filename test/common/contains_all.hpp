//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONTAINS_ALL_HPP
#define BOOST_DI_CONTAINS_ALL_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/push_back.hpp>

namespace boost {
namespace di {

template<
    typename TSrc
  , typename TDst
  , typename = void
>
struct contains_all
    : mpl::empty<
          typename mpl::fold<
              TSrc
            , mpl::vector0<>
            , mpl::if_<
                  mpl::contains<TDst, mpl::_2>
                , mpl::_1
                , mpl::push_back<mpl::_1, mpl::_2>
              >
          >::type
      >
{ };

template<typename TSrc, typename TDst>
struct contains_all<
    TSrc
  , TDst
  , typename enable_if<
        mpl::not_equal_to<
            mpl::size<TSrc>
          , mpl::size<TDst>
        >
    >::type
>
    : mpl::false_
{ };

} // namespace di
} // namespace boost

#endif

