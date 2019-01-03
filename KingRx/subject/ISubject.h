#pragma once
#include "../Interfaces.h"

namespace rx
{
	namespace subject
	{
		template<typename TSource, typename TResult, typename TException>
		class ISubject : public IObservable<TSource, TException>, public IObserver<TResult, TException>{};

		template<typename T, typename TException>
		class ISubject : public ISubject<T, T, TException>{};
	}
}