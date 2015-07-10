#pragma once

namespace SSL
{

	class EventQueue;

	__declspec( thread ) EventQueue* eventQueue = nullptr;

	class ThreadWorker
	{
	public:
		ThreadWorker();
		~ThreadWorker();

	private:
		void initTLS();

	public:
		void Run();
	};

}