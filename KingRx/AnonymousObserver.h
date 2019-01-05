#pragma once
#include "Interfaces.h"
#include <functional>

namespace rx
{
	template<typename T, typename TException>
	class AnonymousObserver : public IObserver<T, TException>
	{
	public:
		AnonymousObserver(
			std::function<void(const T& value)> onNext,
			std::function<void(const TException& error)> onError,
			std::function<void()> onCompleted
		);
		AnonymousObserver(
			std::function<void(const T& value)> onNext,
			std::function<void(const TException& error)> onError
		);
		AnonymousObserver(
			std::function<void(const T& value)> onNext,
			std::function<void()> onCompleted
		);
		AnonymousObserver(
			std::function<void(const T& value)> onNext
		);
		void OnNext(const T& value) override;
		void OnError(const TException& error) override;
		void OnCompleted() noexcept override;
	private:
		std::function<void(const T& value)> _onNext;
		std::function<void(const TException& error)> _onError;
		std::function<void()> _onCompleted;
	};

	template<typename T, typename TException>
	inline AnonymousObserver<T, TException>::AnonymousObserver(std::function<void(const T&value)> onNext, std::function<void(const TException&error)> onError, std::function<void()> onCompleted) :
		_onNext(onNext), _onError(onError), _onCompleted(onCompleted)
	{}

	template<typename T, typename TException>
	inline AnonymousObserver<T, TException>::AnonymousObserver(std::function<void(const T&value)> onNext, std::function<void(const TException&error)> onError) :
		_onNext(onNext), _onError(onError), _onCompleted(std::function<void()>([]() {}))
	{}

	template<typename T, typename TException>
	inline AnonymousObserver<T, TException>::AnonymousObserver(std::function<void(const T&value)> onNext, std::function<void()> onCompleted) :
		_onNext(onNext), _onError(std::function<void(const TException& error)>([](const TException&) {})), _onCompleted(onCompleted)
	{}

	template<typename T, typename TException>
	inline AnonymousObserver<T, TException>::AnonymousObserver(std::function<void(const T&value)> onNext) :
		_onNext(onNext), _onError(std::function<void(const TException& error)>([](const TException&) {})), _onCompleted(std::function<void()>([]() {}))
	{}

	template<typename T, typename TException>
	inline void AnonymousObserver<T, TException>::OnNext(const T & value)
	{
		_onNext(value);
	}

	template<typename T, typename TException>
	inline void AnonymousObserver<T, TException>::OnError(const TException & error)
	{
		_onError(error);
	}

	template<typename T, typename TException>
	inline void AnonymousObserver<T, TException>::OnCompleted() noexcept
	{
		_onCompleted();
	}
}