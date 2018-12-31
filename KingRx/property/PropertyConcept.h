#pragma once
#include <utility>

namespace rx
{
	namespace property
	{
		struct is_gettable_impl
		{
			template<class Getter, typename T>
			static auto check(Getter*, T*) -> decltype(
				std::declval<Getter>().Get(std::declval<T>()),
				std::true_type()) {}
			template<class Getter, typename T>
			static auto check(...)->std::false_type {};
		};

		template<class Getter, typename T>
		struct is_gettable
			: decltype(is_gettable_impl::check<Getter, T>(
				nullptr, nullptr
				)) {};

		struct is_settable_impl
		{
			template<class Setter, typename T>
			static auto check(Setter*, T*) -> decltype(
				std::declval<Setter>().Set(std::declval<T>()),
				std::true_type()) {}
			template<class Setter, typename T>
			static auto check(...)->std::false_type {};
		};

		template<class Setter, typename T>
		struct is_settable
			: decltype(is_settable_impl::check<Setter, T>(
				nullptr, nullptr
				)) {};
	}
}