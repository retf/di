//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_HPP
    #define BOOST_DI_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/has_xxx.hpp>

    #include "boost/di/detail/module.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/scopes/deduce.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/module.hpp"           \
        )                                   \
    )

    namespace boost {
    namespace di {

    template<typename T>
    struct default_scope
        : scope<scopes::deduce>::bind<T>
    { };

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class module
        : public detail::module<
              typename mpl::fold<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          mpl::is_sequence<mpl::_2>
                        , mpl::_2
                        , default_scope<mpl::_2>
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
          >
    { };

    template<typename TSeq>
    struct concepts2
        : mpl::fold<
            TSeq
          , mpl::vector0<>
          , mpl::copy<
                mpl::if_<
                    mpl::is_sequence<mpl::_2>
                  , mpl::_2
                  , mpl::vector1<mpl::_2>
                >
              , mpl::back_inserter<mpl::_1>
            >
          >::type
    { };

    template<
        typename TDeps = mpl::vector0<>
      , template<typename, typename = void> class TPool = detail::pool
    >
    class module2
        : public detail::module<TDeps>
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(context)

        template<
            typename
          , template<typename, typename = void> class
        > friend class module2;

        template<typename TSeq>
        struct externals
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::if_<
                      has_context<mpl::_2>
                    , mpl::push_back<mpl::_1, mpl::_2>
                    , mpl::_1
                  >
              >::type
        { };


        template<typename TSeq, typename TExternals, typename T>
        module2<TSeq, TPool> create_module2(
            const T&
          , typename enable_if_c<mpl::size<TExternals>::value == 0>::type* = 0) const {
            return module2<TSeq, TPool>();
        }

    public:
        module2() { }

        #include BOOST_PP_ITERATE()

        module2<mpl::vector0<>, TPool> operator()() const {
            return module2<mpl::vector0<>, TPool>();
        }
    };

    } // namespace di
    } // namespace boost

    #endif

#else

private:
    template<BOOST_DI_TYPES(Args)>
    explicit module2(BOOST_DI_ARGS(Args, args))
        : detail::module<TDeps>
        (BOOST_DI_ARGS_FORWARD(args))
    { }

    template<typename TSeq, typename TExternals, typename T>
    module2<TSeq, TPool> create_module2(
        const T& pool
      , typename enable_if_c<
            mpl::size<TExternals>::value == BOOST_PP_ITERATION()
        >::type* = 0) const {

        #define BOOST_DI_GET(z, n, _)                                   \
            BOOST_PP_COMMA_IF(n)                                        \
            pool.template get<typename mpl::at_c<TExternals, n>::type>()

        return module2<TSeq, TPool>(
            BOOST_PP_REPEAT(
                BOOST_PP_ITERATION()
              , BOOST_DI_GET
              , ~
            )
        );

        #undef BOOST_DI_GET
    }

public:
    template<BOOST_DI_TYPES(Args)>
    module2<typename concepts2<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type, TPool>
    operator()(BOOST_DI_ARGS(Args, args)) const {
        TPool<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > pool(BOOST_DI_ARGS_FORWARD(args));
        return create_module2<
            typename concepts2<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type
          , typename externals<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type
        >(pool);
    }

#endif
