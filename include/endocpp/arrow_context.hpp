#pragma once

#include <endocpp/detail/detection.hpp>

namespace endo {

struct empty_context_t
{
	constexpr explicit empty_context_t() = default;
};
inline constexpr empty_context_t empty_context{};

template<class T>
struct is_arrow_context : std::false_type
{
};

template<>
struct is_arrow_context<empty_context_t> : std::true_type
{
};

template<class T>
inline constexpr bool is_arrow_context_v = is_arrow_context<T>::value;

} // namespace endo
