#pragma once

#include <memory>
#include <mutex>

#include "NonCopyable.h"

namespace SSL
{	
	template<typename T> 
	class Singleton : public NonCopyable<T>
	{
	private:
		static std::once_flag m_onceFalg;
		static std::unique_ptr<T> m_instance;

	protected:
		Singleton() = default;
		virtual ~Singleton() {};

	public:
		static T* GetInstance()
		{
			std::call_once(m_onceFalg, []
					{
						m_instance.reset(new T);
			});

			return m_instance.get();
		}
	};

	template<typename T>
	std::once_flag Singleton<T>::m_onceFalg;

	template<typename T>
	std::unique_ptr<T> Singleton<T>::m_instance;
}

