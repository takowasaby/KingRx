#pragma once
#include "ISubject.h"
#include <functional>

namespace rx
{
	namespace subject
	{
		template<typename T, typename TException>
		class SubjectBase : public ISubject<T, TException>
		{
		public:
			std::shared_ptr<IDisposable> Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer) override;
			std::shared_ptr<IDisposable> Subscribe(std::function<void(const T& value)> onNext, std::function<void(const TException& error)> onError, std::function<void()> onCompleted);
			std::shared_ptr<IDisposable> Subscribe(std::function<void(const T& value)> onNext, std::function<void(const TException& error)> onError);
			std::shared_ptr<IDisposable> Subscribe(std::function<void(const T& value)> onNext, std::function<void()> onCompleted);
			std::shared_ptr<IDisposable> Subscribe(std::function<void(const T& value)> onNext);
		protected:
			virtual std::shared_ptr<IDisposable> SubscribeCore(std::unique_ptr<IObserver<T, TException>>&& observer) = 0;
		};

		template<typename T, typename TException>
		inline std::shared_ptr<IDisposable> SubjectBase<T, TException>::Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer)
		{
			return SubscribeCore(std::move(observer));
		}

		template<typename T, typename TException>
		inline std::shared_ptr<IDisposable> SubjectBase<T, TException>::Subscribe(std::function<void(const T&value)> onNext, std::function<void(const TException&error)> onError, std::function<void()> onCompleted)
		{
			return SubscribeCore(std::make_unique<AnonymousObserver<T, TException>>(onNext, onError, onCompleted));
		}

		template<typename T, typename TException>
		inline std::shared_ptr<IDisposable> SubjectBase<T, TException>::Subscribe(std::function<void(const T&value)> onNext, std::function<void(const TException&error)> onError)
		{
			return SubscribeCore(std::make_unique<AnonymousObserver<T, TException>>(onNext, onError));
		}

		template<typename T, typename TException>
		inline std::shared_ptr<IDisposable> SubjectBase<T, TException>::Subscribe(std::function<void(const T&value)> onNext, std::function<void()> onCompleted)
		{
			return SubscribeCore(std::make_unique<AnonymousObserver<T, TException>>(onNext, onCompleted));
		}

		template<typename T, typename TException>
		inline std::shared_ptr<IDisposable> SubjectBase<T, TException>::Subscribe(std::function<void(const T&value)> onNext)
		{
			return SubscribeCore(std::make_unique<AnonymousObserver<T, TException>>(onNext));
		}
	}
}