#pragma once

namespace rx
{
	namespace property
	{
		template<typename T>
		struct SimplePropertyGetter
		{
			static const T& Get(const T& value) { return value; }
		};

		template<typename T>
		struct SimplePropertySetter
		{
			static const T& Set(const T& value) { return value; }
		};
	}
}