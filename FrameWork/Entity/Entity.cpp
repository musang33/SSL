#include "stdafx.h"

#include <Entity/Entity.h>
#include <Core/Action.h>

namespace SSL
{

	void Entity::AddAction( Action* action ) const
	{
		m_actions[action->GetId()] = action;
	}

	Action* Entity::GetAction( UINT32 id ) const
	{
		if ( id > EN_ACTION_TYPE::MAX_ACTION_COUNT )
		{
			return nullptr;
		}

		return m_actions[id];
	}

}