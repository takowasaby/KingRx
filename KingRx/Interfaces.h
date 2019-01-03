#pragma once
#include <exception>
#include <memory>

namespace rx
{
	class IDisposable
	{
	public:
		virtual void Dispose() = 0;
	};

	template<typename T>
	class IObserver
	{
	public:
		virtual ~IObserver() = 0;
		virtual void OnNext(const T& value) = 0;
		virtual void OnError(std::unique_ptr<std::exception> error) = 0;
		virtual void OnComprated() = 0;
	};

	template<typename T>
	class IObservable
	{
	public:
		virtual ~IObservable() = 0;
		std::shared_ptr<IDisposable> Subscribe(std::unique_ptr<IObserver> observer) = 0;
	};
}