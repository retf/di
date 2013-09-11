//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_CONVERTIBLE_ANY_HPP
#define BOOST_DI_CONVERTIBLES_CONVERTIBLE_ANY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/variant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/has_xxx.hpp>

#include "boost/di/type_traits/value_type.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(name)
BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

template<typename T, typename = void>
struct plain_type
{
    typedef T type;
};

template<typename T>
struct plain_type<
    T
  , typename enable_if<
        mpl::and_<
            has_name<T>
          , has_value_type<T>
        >
    >::type
>
{
    typedef typename T::value_type type;
};


template<typename>
class convertible_any_impl;

template<typename TDest>
class convertible_any_impl<TDest&>
    : public static_visitor<TDest&>
{
public:
    template<typename TSrc>
    TDest& operator()(const TSrc& object) const {
        return const_cast<TDest&>(object);
    }

    template<typename TSrc>
    TDest& operator()(TSrc& object) const {
        return object;
    }

    template<typename TSrc>
    TDest& operator()(const shared_ptr<TSrc>& object) const {
        return *object;
    }
};

template<typename TDest>
class convertible_any_impl<TDest*>
    : public static_visitor<TDest*>
{
public:
    template<typename TSrc>
    TDest* operator()(const TSrc& object) const {
        return new TSrc(object);
    }

    template<typename TSrc>
    TDest* operator()(TSrc& object) const {
        return new TSrc(object);
    }

    template<typename TSrc>
    TDest* operator()(const shared_ptr<TSrc>& object) const {
        return new TSrc(*object);
    }
};

template<typename TDest>
class convertible_any_impl<shared_ptr<TDest> >
    : public static_visitor<shared_ptr<TDest> >
{
public:
    template<typename TSrc>
    shared_ptr<TDest> operator()(const TSrc& object) const {
        //return shared_ptr<TSrc>(new TSrc(object));
        return shared_ptr<TDest>();
    }

    template<typename TSrc>
    shared_ptr<TDest> operator()(TSrc& object) const {
        //return shared_ptr<TSrc>(new TSrc(&object));
        return shared_ptr<TDest>();
    }

    template<typename TSrc>
    shared_ptr<TDest> operator()(const shared_ptr<TSrc>& object) const {
        return object;
    }
};

template<
    typename T
  , typename = void
>
class convertible_any
{
    typedef typename plain_type<T>::type object_type;

    typedef variant<
        object_type&
      , const object_type&
      , shared_ptr<object_type>
    > value_type;

public:
    template<typename TValue>
    explicit convertible_any(const TValue& value)
        : object_(value)
    { }

    operator object_type&() const {
        return apply_visitor(convertible_any_impl<object_type&>(), object_);
    }

    operator object_type*() const {
        return apply_visitor(convertible_any_impl<object_type*>(), object_);
    }

    template<typename I>
    operator shared_ptr<I>() const {
        return apply_visitor(convertible_any_impl<shared_ptr<object_type> >(), object_);
    }

    template<typename I, typename TName>
    operator named<shared_ptr<I>, TName>() const {
        return apply_visitor(convertible_any_impl<shared_ptr<object_type> >(), object_);
    }

private:
    value_type object_;
};

template<typename T>
class convertible_any<
    T
  , typename enable_if<
        mpl::or_<
            is_same<typename type_traits::value_type<T>::type, std::string>
          , is_arithmetic<typename type_traits::value_type<T>::type>
          , has_result_type<typename type_traits::value_type<T>::type>
        >
    >::type
>
{
    typedef typename type_traits::value_type<T>::type value_type;
    typedef typename plain_type<value_type>::type object_type;

public:
    template<typename TValue>
    explicit convertible_any(const TValue& value)
        : object_(value)
    { }

    operator object_type() const {
        return object_;
    }

    operator object_type*() const {
        return new object_type(object_);
    }

    operator shared_ptr<object_type>() const {
        return make_shared<object_type>(object_);
    }

    template<typename TName>
    operator named<shared_ptr<object_type>, TName>() const {
        return make_shared<object_type>(object_);
    }

    template<typename TName>
    operator named<object_type, TName>() const {
        return object_;
    }

private:
    value_type object_;
};

} // namespace detail

template<typename T>
class convertible_any : public detail::convertible_any<T>
{
public:
    template<typename TValue>
    explicit convertible_any(const TValue& value)
        : detail::convertible_any<T>(value)
    { }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif
