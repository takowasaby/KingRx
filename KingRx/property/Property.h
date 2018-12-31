#pragma once
#include "PropertyBase.h"
#include "SimplePropertyPolicies.h"
#include "PropertyConcept.h"

namespace rx
{
	namespace property
	{
		template<typename T, class Getter = SimplePropertyGetter<T>, class Setter = SimplePropertySetter<T>>
		class Property : public PropertyBase<T>, private Getter, private Setter
		{
		public:
			Property(T& value) : PropertyBase<T>(value) {
				static_assert(is_gettable<Getter, T>::value, "Property has impertinent Getter");
				static_assert(is_settable<Setter, T>::value, "Property has impertinent Setter");
			}
			Property(const Property& ref) : PropertyBase<T>(ref) {}
			virtual ~Property() {}
		public:
			operator const T& () const { return this->Get(this->_value); }
			const T& operator -> () const { return this->Get(this->_value); }

			Property<T, Getter, Setter>& operator = (const T& value) { this->_value = this->Set(value); return *this; }
		};
	}
}