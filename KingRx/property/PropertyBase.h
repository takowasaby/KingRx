#pragma once

namespace rx
{
	namespace property
	{
		template<typename T>
		class PropertyBase
		{
		public:
			PropertyBase(T& value) : _value(value) {}
			PropertyBase(const PropertyBase& ref) : _value(ref._value) {}
			virtual ~PropertyBase() {}
			PropertyBase<T>& operator = (const PropertyBase<T>& ref)
			{
				this->_value = T(ref._value);
				return *this;
			}
		protected:
			T& _value;
		};
	}
}