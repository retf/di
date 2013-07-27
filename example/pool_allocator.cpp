//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/pool/pool_alloc.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct i
{
    virtual ~i() { }
};

struct c : i
{ };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<i>) { }
};

} // namespace

template<typename T>
struct pool_allocator
    : boost::pool_allocator<T>
{ };

int main()
{
    typedef di::generic_module<
        di::scope<di::scopes::per_request<pool_allocator>>::bind<
            c
        >
    > module;

    di::injector<module> injector;
    injector.create<c1>();
}
