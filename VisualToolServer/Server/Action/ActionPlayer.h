#pragma once

#include <../FrameWork/Core/Action.h>

namespace SSL
{

	class ActionPlayer : public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_PLAYER;

	public:
		ActionPlayer( Entity* entity );
		~ActionPlayer();

		void SetSocketIndex( UINT32 socketIndex )
		{
			m_socketIndex = socketIndex;
		}

		UINT32 GetSocketIndex( )
		{
			return m_socketIndex;
		}

	public:		
		UINT32	m_socketIndex;
	};

}