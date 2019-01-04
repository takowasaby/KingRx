#pragma once
#include "../Interfaces.h"

namespace rx
{
	namespace subject
	{
		template<typename T, typename TException>
		class ISubject : public IObservable<T, TException>, public IObserver<T, TException>{};
	}
}