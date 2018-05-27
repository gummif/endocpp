#pragma once

#include <endocpp/detail/detection.hpp>

namespace endo {

/// Pointed specialization point
template<class T>
struct pointed_traits;

namespace detail {
template<class T>
using pointed_impl = typename pointed_traits<std::decay_t<T>>::implementation;
} // namespace detail

/// Pointed T value type
template<class T>
using pointed_value_t = typename pointed_traits<std::decay_t<T>>::value_type;

/// Detect if T is a Pointed
template<class T>
using is_pointed =
	std::conjunction<is_higher_order_type<T>, detail::is_detected<detail::pointed_impl, T>>;

/// Detect if T is a Pointed
template<class T>
inline constexpr bool is_pointed_v = is_pointed<T>::value;

/// Pointed: point
template<class Tag, class ArrowContext, class T>
decltype(auto) point(ArrowContext&& ctx, T&& arg)
{
	using Pointed = compose_type<Tag, std::decay_t<T>>;
	static_assert(is_pointed_v<Pointed>, "point requires a Pointed");
	return detail::pointed_impl<Pointed>::point(std::forward<ArrowContext>(ctx),
												std::forward<T>(arg));
};

/// Pointed: point
template<class Tag, class T>
decltype(auto) point(T&& arg)
{
	using Pointed = compose_type<Tag, std::decay_t<T>>;
	static_assert(is_pointed_v<Pointed>, "point requires a Pointed");
	return detail::pointed_impl<Pointed>::point(empty_context, std::forward<T>(arg));
};

/// Pointed: point_eval
template<class Tag, class ArrowContext, class Function>
decltype(auto) point_eval(ArrowContext&& ctx, Function&& f)
{
	using Pointed = compose_type<Tag, detail::decay_result_t<Function>>;
	static_assert(is_pointed_v<Pointed>, "point_eval requires a Pointed");
	return detail::pointed_impl<Pointed>::point_eval(std::forward<ArrowContext>(ctx),
													 std::forward<Function>(f));
};

/// Pointed: point_eval
template<class Tag, class Function>
decltype(auto) point_eval(Function&& f)
{
	using Pointed = compose_type<Tag, detail::decay_result_t<Function>>;
	static_assert(is_pointed_v<Pointed>, "point_eval requires a Pointed");
	return detail::pointed_impl<Pointed>::point_eval(empty_context, std::forward<Function>(f));
};

// TODO point from function? point_invoke

} // namespace endo
