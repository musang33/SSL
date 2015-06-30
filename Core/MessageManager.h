#pragma once

#include "CommonData.h"
#include "Singleton.h"

#include <list>

namespace SSL
{
	class BaseEntity;

	class MessageManager : public Singleton<MessageManager>
	{
	private:
		std::list<ST_MESSAGE_INFO>		m_delayedMessageList;

	public:
		bool Dispatch(ST_MESSAGE_INFO& messageInfo);
		void DispatchDelayedMessage();
	};
} // SSL

