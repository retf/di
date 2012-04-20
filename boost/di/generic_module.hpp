//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_generic_module_HPP
    #define BOOST_DI_MODULE_generic_module_HPP

    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include "boost/di/aux/pool.hpp"
    #include "boost/di/aux/utility.hpp"
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/scopes/singleton.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/concepts.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "boost/di/generic_module.hpp"))

    namespace boost {
    namespace di {

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    class generic_module : public aux::module
    {
        template<typename TInstance, typename T>
        struct is_same_instance
            : mpl::or_<
                  is_same<typename TInstance::name, T>
                , is_same<typename TInstance::value_type, T>
              >
        { };

        template<typename TSequence, typename T>
        struct find_instance_type
            : mpl::find_if<TSequence, is_same_instance<mpl::_1, T> >::type
        { };

        template<typename T, typename Enable = void>
        struct make_annotation
        {
            typedef typename annotate<aux::instance<T> >::template with<> type;
        };

        template<typename T>
        struct make_annotation<T, typename enable_if<is_base_of<aux::internal, T> >::type>
        {
            typedef typename T::template rebind<scopes::singleton>::type dependency;
            typedef aux::instance<typename dependency::expected, typename dependency::context> instance;
            typedef typename annotate<instance>::template with<typename T::name> type;
        };

        struct externals
            : mpl::transform<
                  typename mpl::fold<
                      mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
                    , mpl::vector0<>
                    , mpl::copy<
                          mpl::if_<
                              is_base_of<concepts::detail::externals, mpl::_2>
                            , mpl::_2
                            , mpl::vector0<>
                          >
                        , mpl::back_inserter<mpl::_1>
                      >
                  >::type
                , make_annotation<mpl::_1>
              >::type
        { };

        struct instances
            : mpl::transform<
                  externals
                , aux::get_derived<mpl::_1>
              >::type
        { };

    public:
        typedef aux::pool<typename instances::type> pool;

        struct dependencies
            : mpl::fold<
                mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
              , mpl::vector0<>
              , mpl::copy<
                    mpl::if_<
                        is_base_of<concepts::detail::externals, mpl::_2>
                      , mpl::vector0<>
                      , mpl::if_<
                            mpl::is_sequence<mpl::_2>
                          , mpl::_2
                          , per_request<mpl::_2>
                        >
                    >
                  , mpl::back_inserter<mpl::_1>
                >
            >::type
        { };

        generic_module() { }

        #include BOOST_PP_ITERATE()

        template<typename T, typename Tvalue> inline static
            typename disable_if<
                is_same<
                    find_instance_type<externals, T>
                  , mpl::end<externals>
                >
              , typename find_instance_type<externals, T>::type::derived
            >::type
        set(Tvalue value)
        {
            typedef typename find_instance_type<externals, T>::type annotation;
            return typename annotation::derived(value);
        }

        const pool& get_pool() const { return pool_; }

    private:
        pool pool_;
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    generic_module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg))
        : pool_(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg))
    { }

#endif
