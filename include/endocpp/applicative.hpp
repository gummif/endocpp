#pragma once

#include <endocpp/detail/detection.hpp>
#include <endocpp/functor.hpp>
#include <endocpp/pointed.hpp>

namespace endo {

/// Applicative specialization point
template<class T>
struct applicative_traits;

namespace detail {
template<class T>
using applicative_impl = typename applicative_traits<std::decay_t<T>>::implementation;
} // namespace detail

/// Applicative T value type
template<class T>
using applicative_value_t = typename applicative_traits<std::decay_t<T>>::value_type;

/// Detect if T is an Applicative
template<class T>
using is_applicative = std::conjunction<is_pointed<T>, is_functor<T>,
										detail::is_detected<detail::applicative_impl, T>>;

/// Detect if T is an Applicative
template<class T>
inline constexpr bool is_applicative_v = is_applicative<T>::value;

/// Applicative: ap
template<class ArrowContext, class ApplicativeFunction, class... ApplicativeArgs,
		 std::enable_if_t<is_arrow_context_v<ArrowContext>, int> = 0>
decltype(auto) ap(ArrowContext&& ctx, ApplicativeFunction&& f, ApplicativeArgs&&... args)
{
	static_assert(is_applicative_v<ApplicativeFunction>, "ap requires an Applicative");
	return detail::applicative_impl<ApplicativeFunction>::ap(
		std::forward<ArrowContext>(ctx), std::forward<ApplicativeFunction>(f),
		std::forward<ApplicativeArgs>(args)...);
};

/// Applicative: ap
template<class ApplicativeFunction, class... ApplicativeArgs,
		 std::enable_if_t<!is_arrow_context_v<ApplicativeFunction>, int> = 0>
decltype(auto) ap(ApplicativeFunction&& f, ApplicativeArgs&&... args)
{
	static_assert(is_applicative_v<ApplicativeFunction>, "ap requires an Applicative");
	return detail::applicative_impl<ApplicativeFunction>::ap(
		empty_context, std::forward<ApplicativeFunction>(f),
		std::forward<ApplicativeArgs>(args)...);
};

/// Applicative: lift
template<class ArrowContext, class Function, class Applicative, class... ApplicativeArgs,
		 std::enable_if_t<is_arrow_context_v<ArrowContext>, int> = 0>
decltype(auto) lift(ArrowContext&& ctx, Function&& f, Applicative&& a,
					ApplicativeArgs&&... args)
{
	static_assert(is_applicative_v<Applicative>, "lift requires an Applicative");
	return detail::applicative_impl<Applicative>::lift(
		std::forward<ArrowContext>(ctx), std::forward<Function>(f),
		std::forward<Applicative>(a), std::forward<ApplicativeArgs>(args)...);
};

/// Applicative: lift
template<class Function, class Applicative, class... ApplicativeArgs,
		 std::enable_if_t<!is_arrow_context_v<Function>, int> = 0>
decltype(auto) lift(Function&& f, Applicative&& a, ApplicativeArgs&&... args)
{
	static_assert(is_applicative_v<Applicative>, "lift requires an Applicative");
	return detail::applicative_impl<Applicative>::lift(
		empty_context, std::forward<Function>(f), std::forward<Applicative>(a),
		std::forward<ApplicativeArgs>(args)...);
};

/*/// Applicative: lift_value
template<class Tag, class ArrowContext, class T>
decltype(auto) lift_value(ArrowContext&& ctx, T&& arg)
{
	using Applicative = compose_type<Tag, std::decay_t<T>>;
	static_assert(is_applicative_v<Applicative>, "lift_value requires an Applicative");
	return detail::applicative_impl<Applicative>::lift_value(std::forward<ArrowContext>(ctx),
															 std::forward<T>(arg));
};

/// Applicative: lift_value
template<class Tag, class T>
decltype(auto) lift_value(T&& arg)
{
	using Applicative = compose_type<Tag, std::decay_t<T>>;
	static_assert(is_applicative_v<Applicative>, "lift_value requires an Applicative");
	return ::endo::lift_value<Tag>(empty_context, std::forward<T>(arg));
};*/

} // namespace endo
