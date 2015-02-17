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
		std::list<MessageInfo>		m_delayedMessageList;

	public:
		bool Dispatch(MessageInfo& messageInfo);
		void DispatchDelayedMessage();
	};
} // SSL

