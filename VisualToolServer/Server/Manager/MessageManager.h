#pragma once

#include <../FrameWork/Core/Singleton.h>

#include <list>

namespace SSL
{
	class Entity;

	class MessageManager : public Singleton<MessageManager>
	{
	private:
		std::list<ST_MESSAGE_INFO>		m_delayedMessageList;

	public:
		bool Dispatch(ST_MESSAGE_INFO& messageInfo);
		void DispatchDelayedMessage();
	};
} // SSL

