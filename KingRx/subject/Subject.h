#pragma once
#include "SubjectBase.h"
#include <map>

namespace rx
{
	namespace subject
	{
		template<typename T, typename TException = std::exception>
		class Subject : public SubjectBase<T, TException>
		{
		public:
			Subject();
			~Subject();
			void OnNext(const T& value) override;
			void OnError(const TException& error) override;
			void OnCompleted() noexcept override;
			std::shared_ptr<IDisposable> Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer) override;
			void Dispose() noexcept;
		private:
			std::map<unsigned, std::shared_ptr<SubjectDisposable<T, TException>>> _observers;
			std::unique_ptr<TException> _exception;
			unsigned _observerCount;

			void disposeProcess() noexcept;
			std::function<void()> makeDisposableFunc();
		};

		template<typename T, typename TException>
		inline Subject<T, TException>::Subject() :
			_observerCount(0),
			_exception(nullptr)
		{
		}

		template<typename T, typename TException>
		inline Subject<T, TException>::~Subject()
		{
			if (_hasObservers == true && _isDisposed == false)
			{
				OnCompleted();
			}
		}

		template<typename T, typename TException>
		inline void Subject<T, TException>::OnNext(const T & value)
		{
			if (_hasObservers == true && _isDisposed == false)
			{
				for (auto obs : _observers)
				{
					obs.second->OnNext(value);
				}
			}
		}

		template<typename T, typename TException>
		inline void Subject<T, TException>::OnError(const TException& error)
		{
			if (_hasObservers == true && _isDisposed == false)
			{
				_exception = std::make_unique<TException>(error);
				for (auto obs : _observers)
				{
					obs.second->OnError(error);
				}
			}
			disposeProcess();
		}

		template<typename T, typename TException>
		inline void Subject<T, TException>::OnCompleted() noexcept
		{
			if (_hasObservers == true && _isDisposed == false)
			{
				for (auto obs : _observers)
				{
					obs.second->OnCompleted();
				}
			}
			disposeProcess();
		}

		template<typename T, typename TException>
		inline std::shared_ptr<IDisposable> Subject<T, TException>::Subscribe(std::unique_ptr<IObserver<T, TException>>&& observer)
		{
			if (!observer)
				throw std::exception();
			if (_isDisposed)
			{
				if (_exception)
				{
					observer->OnError(*_exception);
				}
				else
				{
					observer->OnCompleted();
				}
				return std::shared_ptr<IDisposable>();
			}

			auto disposeFunc = makeDisposableFunc();
			auto spSubjectDisposable = std::make_shared<SubjectDisposable<T, TException>>(std::move(observer), disposeFunc);

			_observerCount++;
			if (!_hasObservers)
				_hasObservers = true;

			return spSubjectDisposable;
		}

		template<typename T, typename TException>
		inline void Subject<T, TException>::Dispose() noexcept
		{
			std::unique_ptr<TException>().swap(_exception);
			disposeProcess();
		}

		template<typename T, typename TException>
		inline void Subject<T, TException>::disposeProcess() noexcept
		{
			_isDisposed = true;
			_hasObservers = false;
			for (auto obs : _observers)
			{
				obs.second->nullifiacation();
			}
			std::map<unsigned, std::shared_ptr<SubjectDisposable<T, TException>>>().swap(_observers);
		}

		template<typename T, typename TException>
		inline std::function<void()> Subject<T, TException>::makeDisposableFunc()
		{
			return [&hasObservers = _hasObservers, &observers = _observers, observerNum = _observerCount]() 
			{
				auto itr = observers.find(observerNum);
				if (itr != observers.end())
					if (*itr)
						observers.erase(itr);
				if (observers.size() == 0)
					hasObservers = false;
			};
		}

	}
} 