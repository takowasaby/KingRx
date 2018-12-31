#pragma once
#include "Interfaces.h"

namespace rx
{
	namespace subject
	{
		template<typename TSource, typename TResult>
		class ISubject : IObservable<TSource>, IObserver<TResult>{};

		template<typename T>
		class ISubject : ISubject<T, T>{};
	}
}