//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_ASSERT_HPP
#define BOOST_DI_FAKE_ASSERT_HPP

#include <typeinfo>
#include <stdexcept>
#include <boost/config.hpp>

#define BOOST_DI_ASSERT_MSG(c, msg, type)                           \
    if (!c) {                                                       \
        throw ::boost::di::assert_exception(#msg, &typeid(type));   \
    }

namespace boost {
namespace di {

class assert_exception : public std::exception
{
public:
    assert_exception(const char* msg, const std::type_info* type)
        : msg_(msg), type_(type)
    { }

    const char* what() const BOOST_NOEXCEPT_OR_NOTHROW override { return msg_; }
    const std::type_info& get_type() const { return *type_; }

private:
    const char* msg_;
    const std::type_info* type_;
};

} // namespace di
} // namespace boost

#endif

