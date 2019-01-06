#pragma once
#include "SubjectDisposable.h"
#include "SubjectBase.h"
#include "../AnonymousObserver.h"
#include <map>
#include <chrono>

namespace rx
{
	namespace subject
	{
		template<typename T, typename TException = std::exception>
		class ReplaySubject : public SubjectBase<T, TException>
		{
		public:
			ReplaySubject();
			ReplaySubject(size_t bufferSize);
			template<class Rep, class Period>
			ReplaySubject(std::chrono::duration<Rep, Period> window);
			template<class Rep, class Period>
			ReplaySubject(size_t bufferSize, std::chrono::duration<Rep, Period> window);

			~ReplaySubject();

			bool HasObservers() const noexcept override;
			bool IsDisposed() const noexcept override;

			void OnNext(const T& value) override;
			void OnError(const TException& error) override;
			void OnCompleted() noexcept override;

		protected:
			std::shared_ptr<IDisposable> SubscribeCore(std::unique_ptr<IObserver<T, TException>>&& observer) override;

		private:
			template<typename T, typename TException = std::exception>
			class ReplaySubjectImplBase : public SubjectBase<T, TException>
			{
			public:
				bool HasObservers() const noexcept override;
				bool IsDisposed() const noexcept override;

				void OnNext(const T& value) override;
				void OnError(const TException& error) override;
				void OnCompleted() noexcept override;
			protected:
				std::shared_ptr<IDisposable> SubscribeCore(std::unique_ptr<IObserver<T, TException>>&& observer) override;
				
				virtual void cacheValue(const T& value) = 0;
				virtual size_t replay(std::unique_ptr<IObserver<T, TException>>& observer) = 0;
				virtual void trimingValues() = 0;
			private:
				std::map<unsigned, std::shared_ptr<SubjectDisposable<T, TException>>> _observers;
				std::unique_ptr<TException> _exception;
				unsigned _observerCount;
				bool _hasObservers;
				bool _isDisposed;

				void disposeProcess() noexcept;
				std::function<void()> makeDisposableFunc();
			};

			template<typename T, typename TException = std::exception>
			class ReplaySubjectImplTime : ReplaySubjectImplBase<T, TException>
			{
			public:
				ReplaySubjectImplTime(std::chrono::duration<Rep, Period> window);
				ReplaySubjectImplTime(size_t bufferSize, std::chrono::duration<Rep, Period> window);
			protected:
				void cacheValue(const T& value) override;
				size_t replay(std::unique_ptr<IObserver<T, TException>>& observer) override;
				void trimingValues() override;
			};

			template<typename T, typename TException = std::exception>
			class ReplaySubjectImplBuffer : ReplaySubjectImplBase<T, TException>
			{
			public:
				ReplaySubjectImplBuffer(size_t bufferSize);
			protected:
				void cacheValue(const T& value) override;
				size_t replay(std::unique_ptr<IObserver<T, TException>>& observer) override;
				void trimingValues() override;
			};

			ReplaySubjectImplBase<T, TException> _replaySubjectImplBase;
		};
	}
}