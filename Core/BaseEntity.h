#pragma once

#include "CommonData.h"

namespace SSL
{	
	class BaseEntity
	{
	private:
		int m_ID;
		Location m_currentLocation;
		ENTITY_DIRECTION m_direction;

		static int m_iNextValidID;

		void SetID(int val);			

	public:
		BaseEntity(int id)
		{
			SetID(id);
		}

		virtual ~BaseEntity() {};

		virtual void Update() = 0;

		virtual void DealWithMessage(const MessageInfo& messageInfo) const = 0;

		int ID() const { return m_ID; };

		Location GetCurLocation() { return m_currentLocation; };
		void SetCurLocation( int _x, int _y ) 
		{ 
			m_currentLocation.x = _x;
			m_currentLocation.y = _y;
		}

		void SetDirection( ENTITY_DIRECTION direction )
		{
			m_direction = direction;
		}
		ENTITY_DIRECTION GetDirection() { return m_direction; }

		virtual STATE_ID GetCurrentStateID() = 0;		
		virtual void SetCurrentStateIDInBehaviorTree( STATE_ID stateId ) = 0;
	};
}
