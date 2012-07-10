//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_FIXED_HPP
#define BOOST_DI_SCOPES_FIXED_HPP

#include <string>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/non_type.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/string.hpp>

#include "boost/di/scopes/external.hpp"

namespace boost {
namespace di {
namespace scopes {

namespace aux {

template<typename TDerived, typename = void>
class has_value
{
    struct helper { static int value; };
    struct base
        : helper
        , mpl::if_<
              is_arithmetic<TDerived>
            , mpl::void_
            , TDerived
          >::type
    { };

    template<typename T>
    static mpl::aux::no_tag deduce(non_type<const int*, &T::value>*);

    template<typename>
    static mpl::aux::yes_tag deduce(...);

public:
    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(deduce<base>(0)) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename, typename = void>
class fixed_impl
    : public mpl::false_
{ };

template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
class fixed_impl<
    mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>
>
    : public mpl::true_
{
public:
    inline static std::string create() {
        return mpl::c_str<
            mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>
        >::value;
    }
};

template<
    typename T
>
class fixed_impl<
    T
  , typename enable_if<
        has_value<T>
    >::type
>
    : public mpl::true_
{
public:
    inline static BOOST_TYPEOF_TPL(T::value) create() {
        return T::value;
    }
};

} // namespace aux

class fixed
{
public:
    template<
        typename TExpected
      , typename TGiven = TExpected
    >
    class scope
    {
    public:
        typedef variant<TExpected> result_type;

        result_type create() {
            return aux::fixed_impl<TGiven>::create();
        }
    };
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

