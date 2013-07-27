//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/units/detail/utility.hpp>
#include <boost/di.hpp>

namespace mpl   = boost::mpl;
namespace utils = boost::units::detail;
namespace di    = boost::di;

namespace {

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<i0>) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, int, double, char) { }
};

struct c3
{
    BOOST_DI_CTOR(c3, boost::shared_ptr<c1>, boost::shared_ptr<c2>) { }
};

} // namespace

class text_visitor
{
public:
    template<typename T>
    void operator()() const {
        int size = mpl::size<typename T::context>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << utils::demangle(typeid(typename T::type).name()) << std::endl;
    }
};

int main()
{
    typedef di::generic_module<
        di::per_requests<
            mpl::int_<42>
        >
      , di::singletons<
            c0
        >
    > module;

    di::injector<module> injector;
    injector.visit<c3>(text_visitor());

    return 0;
}

// (anonymous namespace)::c3
// 	boost::shared_ptr<(anonymous namespace)::c1>
// 		boost::shared_ptr<(anonymous namespace)::i0>
// 	boost::shared_ptr<(anonymous namespace)::c2>
// 		int
// 		double
// 		char
