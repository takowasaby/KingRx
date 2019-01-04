#pragma once
#include <exception>
#include <memory>

namespace rx
{
	class IDisposable
	{
	public:
		virtual ~IDisposable() {};
		virtual void Dispose() = 0;
	};

	template<typename T, typename TException = std::exception>
	class IObserver
	{
	public:
		virtual ~IObserver() {};
		virtual void OnNext(const T& value) = 0;
		virtual void OnError(const TException& error) = 0;
		virtual void OnCompleted() noexcept = 0;
	};

	template<typename T, typename TException = std::exception>
	class IObservable
	{
	public:
		virtual ~IObservable() {};
		virtual std::shared_ptr<IDisposable> Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer) = 0;
	};
}