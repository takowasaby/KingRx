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
		virtual void OnError(std::unique_ptr<TException> error) = 0;
		virtual void OnCompleted() = 0;
		virtual bool operator==(const IObserver<T, TException>& other) = 0;
	};

	template<typename T>
	class IObservable
	{
	public:
		virtual ~IObservable() {};
		virtual std::unique_ptr<IDisposable> Subscribe(std::shared_ptr<IObserver<T>> observer) = 0;
	};
}