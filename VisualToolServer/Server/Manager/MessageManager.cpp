#include "stdafx.h"

#include <Manager/MessageManager.h>
#include <../FrameWork/Entity/Entity.h>
#include <Manager/EntityManager.h>
#include <Action/ActionAI.h>

namespace SSL
{
	bool MessageManager::Dispatch(ST_MESSAGE_INFO& messageInfo)
	{		
		const Entity* sender = EntityManager::GetInstance()->GetEntity( messageInfo.senderID );
		const Entity* receiver = EntityManager::GetInstance()->GetEntity( messageInfo.receiverID );

		if ( nullptr == sender || nullptr == receiver )
		{
			return false;
		}

		if ( messageInfo.delayTime )
		{
			messageInfo.delayTime += ::GetTickCount();
			m_delayedMessageList.emplace_back(messageInfo);
		}
		else
		{
			ActionAI* aa = GetEntityAction( receiver );
			aa->DealWithMessage( messageInfo );
		}	

		return true;
	}

	void MessageManager::DispatchDelayedMessage()
	{
		if ( m_delayedMessageList.empty() )
		{
			return;
		}

		double currentTickCount = GetTickCount();

		auto it = m_delayedMessageList.begin();
		auto it_end = m_delayedMessageList.end();
		for ( ; it != it_end;  )
		{
			if ( it->delayTime < currentTickCount )
			{
				const Entity* receiver = EntityManager::GetInstance()->GetEntity( it->receiverID );
				ActionAI* aa = GetEntityAction( receiver );
				aa->DealWithMessage( *it );
				it = m_delayedMessageList.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

} // namespace SSL