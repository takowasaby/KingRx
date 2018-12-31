#pragma once
#include <exception>

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
		virtual void OnNext(T value) = 0;
		virtual void OnError(std::exception error) = 0;
		virtual void OnComprated() = 0;
	};

	template<typename T>
	class IObservable
	{
	public:
		virtual ~IObservable() = 0;
		IDisposable Subscribe() = 0;
	};
}