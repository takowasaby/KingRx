#pragma once
#include "PropertyBase.h"
#include "SimplePropertyPolicies.h"

namespace rx
{
	namespace property
	{
		template<typename T, class Getter = SimplePropertyGetter<T>>
		class ReadOnlyProperty : public PropertyBase<T>, private Getter
		{
		public:
			ReadOnlyProperty(T& value) : PropertyBase<T>(value) {
				static_assert(is_gettable<Getter, T>::value, "Property has impertinent Getter");
			}
			ReadOnlyProperty(const ReadOnlyProperty& ref) : PropertyBase<T>(ref) {}
			virtual ~ReadOnlyProperty() {}
		public:
			operator const T& () const { return this->Get(this->_value); }
			const T& operator -> () const { return this->Get(this->_value); }
		};
	}
}