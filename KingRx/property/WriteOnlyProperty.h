#pragma once
#include "PropertyBase.h"
#include "SimplePropertyPolicies.h"

namespace rx
{
	namespace property
	{
		template<typename T, class Setter = SimplePropertySetter<T>>
		class WriteOnlyProperty : public PropertyBase<T>, private Setter
		{
		public:
			WriteOnlyProperty(T& value) : PropertyBase<T>(value) {
				static_assert(is_settable<Setter, T>::value, "Property has impertinent Setter");
			}
			WriteOnlyProperty(const WriteOnlyProperty& ref) : PropertyBase<T>(ref) {}
			virtual ~WriteOnlyProperty() {}
		public:
			Property<T, Getter, Setter>& operator = (const T& value) { this->_value = this->Set(value); return *this; }
		};
	}
}