#pragma once
#include "SubjectDisposable.h"
#include "SubjectBase.h"
#include "../AnonymousObserver.h"
#include <map>
#include <chrono>
#include <queue>

namespace rx
{
	namespace subject
	{
		using DefaultClock = std::chrono::system_clock;

		template<typename T, typename TException = std::exception, typename TClock = DefaultClock>
		class ReplaySubject : public SubjectBase<T, TException>
		{
		public:
			ReplaySubject() :
				_replaySubjectImpl(ReplaySubjectImplBuffer<T, TException>(SIZE_MAX))
			{}
			ReplaySubject(size_t bufferSize) :
				_replaySubjectImpl(ReplaySubjectImplBuffer<T, TException>(bufferSize))
			{}
			template<class Rep, class Period>
			ReplaySubject(std::chrono::duration<Rep, Period> window) :
				_replaySubjectImpl(ReplaySubjectImplTime<T, TException, TClock>(std::chrono::duration_cast<TClock::duration>(window)))
			{}
			template<class Rep, class Period>
			ReplaySubject(size_t bufferSize, std::chrono::duration<Rep, Period> window) :
				_replaySubjectImpl(ReplaySubjectImplTime<T, TException, TClock>(bufferSize, std::chrono::duration_cast<TClock::duration>(window)))
			{}

			bool HasObservers() const noexcept override
			{
				return _replaySubjectImpl.HasObservers();
			}
			bool IsDisposed() const noexcept override
			{
				return _replaySubjectImpl.IsDisposed();
			}

			void OnNext(const T& value) override
			{
				_replaySubjectImpl.OnNext(value);
			}
			void OnError(const TException& error) override
			{
				_replaySubjectImpl.OnError(error);
			}
			void OnCompleted() noexcept override
			{
				_replaySubjectImpl.OnCompleted();
			}

		protected:
			std::shared_ptr<IDisposable> SubscribeCore(std::unique_ptr<IObserver<T, TException>>&& observer) override
			{
				if (!observer)
					throw std::exception();
				_replaySubjectImpl.Subscribe(std::move(observer));
			}

		private:
			template<typename T, typename TException = std::exception>
			class ReplaySubjectImplBase : public SubjectBase<T, TException>
			{
			public:
				bool HasObservers() const noexcept override
				{
					return _hasObservers;
				}
				bool IsDisposed() const noexcept override
				{
					return _isDisposed;
				}

				void OnNext(const T& value) override
				{
					if (_hasObservers == true && _isDisposed == false)
					{
						cacheValue(value);
						for (auto obs : _observers)
						{
							obs.second->OnNext(value);
						}
					}
				}
				void OnError(const TException& error) override
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
				void OnCompleted() noexcept override
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
			protected:
				std::shared_ptr<IDisposable> SubscribeCore(std::unique_ptr<IObserver<T, TException>>&& observer) override
				{
					std::unique_ptr<IObserver<T, TException>> upObserver = std::move(observer);
					trimingValues();
					replay(upObserver);
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
					_observers.insert(std::make_pair(_observerCount, spSubjectDisposable));

					_observerCount++;
					if (!_hasObservers)
						_hasObservers = true;

					return spSubjectDisposable;
				}
				
				virtual void cacheValue(const T& value) = 0;
				virtual size_t replay(std::unique_ptr<IObserver<T, TException>>& observer) = 0;
			private:
				std::map<unsigned, std::shared_ptr<SubjectDisposable<T, TException>>> _observers;
				std::unique_ptr<TException> _exception;
				unsigned _observerCount;
				bool _hasObservers;
				bool _isDisposed;

				void disposeProcess() noexcept
				{
					_isDisposed = true;
					_hasObservers = false;
					for (auto obs : _observers)
					{
						obs.second->nullification();
					}
					std::map<unsigned, std::shared_ptr<SubjectDisposable<T, TException>>>().swap(_observers);
				}
				std::function<void()> makeDisposableFunc()
				{
					return [&hasObservers = _hasObservers, &observers = _observers, observerNum = _observerCount]()
					{
						auto itr = observers.find(observerNum);
						if (itr != observers.end())
							if ((*itr).second)
								observers.erase(itr);
						if (observers.size() == 0)
							hasObservers = false;
					};
				}
			};

			template<typename T, typename TException = std::exception, typename TClock>
			class ReplaySubjectImplTime : ReplaySubjectImplBase<T, TException>
			{
			public:
				ReplaySubjectImplTime(std::chrono::duration<Rep, Period> window);
				ReplaySubjectImplTime(size_t bufferSize, std::chrono::duration<Rep, Period> window);
			protected:
				void cacheValue(const T& value) override
				{
					auto now = TClock::now();
					auto duration = now - _startTime;
					auto value_pair = std::make_pair(duration, value);
					_valueQueue.push(value_pair);
				}
				size_t replay(std::unique_ptr<IObserver<T, TException>>& observer) override
				{
					while (_valueQueue.size() > _bufferSize)
					{
						_valueQueue.pop();
					}
					while (_valueQueue.front().first > _window)
					{
						_valueQueue.pop();
					}
					for (const auto& value_pair : _valueQueue)
					{
						observer->OnNext(value_pair.second);
					}
					return _valueQueue.size();
				}
			private:
				size_t _bufferSize;
				TClock::duration _window;
				std::queue<std::pair<TClock::duration, T>> _valueQueue;
				std::chrono::time_point<TClock> _startTime;
			};

			template<typename T, typename TException = std::exception>
			class ReplaySubjectImplBuffer : ReplaySubjectImplBase<T, TException>
			{
			public:
				ReplaySubjectImplBuffer(size_t bufferSize) :
					_bufferSize(bufferSize)
				{}
			protected:
				void cacheValue(const T& value) override
				{
					_valueQueue.push(value);
				}
				size_t replay(std::unique_ptr<IObserver<T, TException>>& observer) override
				{
					while (_valueQueue.size() > _bufferSize)
					{
						_valueQueue.pop();
					}
					for (const auto& value : _valueQueue)
					{
						observer->OnNext(value);
					}
					return _valueQueue.size();
				}
			private:
				size_t _bufferSize;
				std::queue<T> _valueQueue;
			};

			ReplaySubjectImplBase<T, TException> _replaySubjectImpl;
		};
	}
}