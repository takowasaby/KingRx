#pragma once
#include "../Interfaces.h"
#include <memory>
#include <functional>

namespace rx
{
	namespace subject
	{
		template<typename T, typename TException>
		class SubjectDisposable : public IDisposable, public IObserver<T, TException>
		{
		public:
			SubjectDisposable(std::unique_ptr<IObserver<T, TException>>&& observer, std::function<void()> disposeFunc) :
				_observer(move(observer)),
				_disposeFunc(disposeFunc)
			{}
			void OnNext(const T& value) override
			{
				_observer->OnNext(value);
			}
			void OnError(const TException& error) override
			{
				_observer->OnError(error);
			}
			void OnCompleted() noexcept override
			{
				_observer->OnCompleted();
			}
			void Dispose() override
			{
				_disposeFunc();
				nullification();
			}
			void nullification() noexcept
			{
				std::unique_ptr<IObserver<T, TException>>().swap(_observer);
				std::function<void()>().swap(_disposeFunc);
			}
		private:
			std::unique_ptr<IObserver<T, TException>> _observer;
			std::function<void()> _disposeFunc;
		};
	}
}