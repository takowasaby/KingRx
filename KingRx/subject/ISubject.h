#pragma once
#include "../Interfaces.h"

namespace rx
{
	namespace subject
	{
		template<typename T, typename TException>
		class ISubject : public IObservable<T, TException>, public IObserver<T, TException>
		{
		public:
			virtual ~ISubject() {};
			virtual bool HasObservers() const noexcept = 0;
			virtual bool IsDisposed() const noexcept = 0;
		};
	}
}