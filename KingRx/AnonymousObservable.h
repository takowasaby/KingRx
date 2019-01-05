#pragma once
#include "Interfaces.h"
#include <functional>

namespace rx
{
	template<typename T, typename TException>
	class AnonymousObservable : public IObservable<T, TException>
	{
	public:
		AnonymousObservable(std::function<std::shared_ptr<IDisposable(std::unique_ptr<IObserver<T, TException>>&&)>> subscribe);
		std::shared_ptr<IDisposable> Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer) override;
	private:
		std::function<std::shared_ptr<IDisposable(std::unique_ptr<IObserver<T, TException>>&&)>> _subscribe;
	};

	template<typename T, typename TException>
	inline AnonymousObservable<T, TException>::AnonymousObservable(std::function<std::shared_ptr<IDisposable(std::unique_ptr<IObserver<T, TException>>&&)>> subscribe) :
		_subscribe(subscribe)
	{}

	template<typename T, typename TException>
	inline std::shared_ptr<IDisposable> AnonymousObservable<T, TException>::Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer)
	{
		return _subscribe(observer);
	}
}