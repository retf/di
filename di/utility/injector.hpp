//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_utILITY_INJECTOR_HPP
    #define DI_utILITY_INJECTOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/set.hpp>
    #include <boost/mpl/insert.hpp>
    #include <boost/mpl/remove_if.hpp>
    #include <boost/mpl/filter_view.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/if.hpp>
    #include "di/back/aux/pool.hpp"
    #include "di/back/aux/utility.hpp"
    #include "di/back/module.hpp"
    #include "di/back/factory.hpp"
    #include "di/back/policy.hpp"
    #include "di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "di/utility/injector.hpp"))

    namespace di
    {
    namespace utility
    {

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    class injector
    {
        typedef boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> seq;

        struct modules : boost::mpl::remove_if<seq, boost::is_base_of<back::detail::policy, boost::mpl::_> >::type { };
        struct policies : boost::mpl::joint_view
            <
                boost::mpl::filter_view<seq, boost::is_base_of<back::detail::policy, boost::mpl::_> >,
                boost::mpl::vector1<typename defaults<back::detail::policy, specialized>::type>
            >::type
        { };

        template<typename TSeq, typename TResult = boost::mpl::set0<> >
        struct DependenciesImpl : boost::mpl::fold
            <
                TSeq,
                TResult,
                boost::mpl::if_
                <
                    boost::is_base_of<back::module, boost::mpl::_2>,
                    DependenciesImpl<back::aux::get_dependencies<boost::mpl::_2>, boost::mpl::_1>,
                    boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
                >
            >
        { };

        struct Externals : boost::mpl::fold
            <
                typename boost::mpl::fold
                <
                    modules,
                    boost::mpl::set<>,
                    boost::mpl::insert< boost::mpl::_1, back::aux::get_pool<boost::mpl::_2> >
                >::type,
                boost::mpl::vector0<>,
                boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
            >::type
        { };

        struct Dependencies : boost::mpl::fold
            <
                typename DependenciesImpl<modules>::type,
                boost::mpl::vector0<>,
                boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
            >::type
        { };

        typedef back::aux::pool<typename Externals::type> pool;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<policies>::type>::type policy;
        typedef back::factory<typename Dependencies::type, pool, policy> factory;

    public:
        injector()
            : m_pool(), m_factory(m_pool)
        { }

        #include BOOST_PP_ITERATE()

        template<typename T> T create()
        {
            return m_factory.create<T>();
        }

        template<typename T, typename visitor> void visit(const visitor& p_visitor)
        {
            return m_factory.visit<T>(p_visitor);
        }

    private:
        pool m_pool;
        factory m_factory;
    };

    } // namespace utility
    } // namespace di

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename M)>
    injector(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const M, &p_module))
        : m_pool(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), p_module, .get_pool() BOOST_PP_INTERCEPT)),
          m_factory(m_pool)
    { }

    #define DI_MODULE_ARG(_, n, module) BOOST_PP_COMMA_IF(n) const module##n& p_module##n = module##n()

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename M)>
    injector<typename boost::mpl::joint_view<modules, boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), M)> >::type> install(BOOST_PP_REPEAT(BOOST_PP_ITERATION(), DI_MODULE_ARG, M))
    {
        typedef injector<typename boost::mpl::joint_view<modules, boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), M)> >::type> injectorType;
        return injectorType(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_module));
    }

    #undef DI_MODULE_ARG

#endif
