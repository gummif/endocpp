#pragma once

#include <endocpp/detail/detection.hpp>

namespace endo {

/// Concept TypeTag

/// Helper alias
template<class Tag, class... Args>
using compose_type = typename Tag::template compose<Args...>;

/// Detect if T is a TypeTag
template<class T, class... Args>
using is_type_tag = detail::is_detected<compose_type, T, Args...>;

/// Detect if T is a TypeTag
template<class T, class... Args>
inline constexpr bool is_type_tag_v = is_type_tag<T, Args...>::value;

/// A pack of types that can be returned from type aliases
template<class... Args>
struct parameter_pack
{
	template<class Tag>
	using compose_type = typename Tag::template compose<Args...>;
};

/// Concept HigherOrderType
/// Allows decomposition and composition of higher order types

/// HigherOrderType specialization point
/// Provides type aliases `tag` and `parameters`.
/// `tag` must be a TypeTag and `parameters` must be a `parameter_pack`.
template<class T>
struct higher_order_type_traits;

/// Helper alias
template<class T>
using type_tag = typename higher_order_type_traits<std::decay_t<T>>::tag;

/// Helper alias
template<class T>
using type_parameters = typename higher_order_type_traits<std::decay_t<T>>::parameters;

/// Detect if T is a HigherOrderType
template<class T>
using is_higher_order_type = std::conjunction<detail::is_detected<type_tag, T>,
											  detail::is_detected<type_parameters, T>>;

/// Detect if T is a HigherOrderType
template<class T>
inline constexpr bool is_higher_order_type_v = is_higher_order_type<T>::value;

} // namespace endo
