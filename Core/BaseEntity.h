#pragma once

#include "CommonData.h"
#include <concurrent_queue.h>
#include "EventQueue.h"

namespace SSL
{		
	class BaseEntity : public EventQueue
	{
	private:
		INT32													m_ID;
		ST_COORDINATE											m_currentLocation;
		EN_ENTITY_DIRECTION										m_direction;
		EN_ENTITY_STATE											m_entityState;
		static INT32											m_iNextValidID;			

	public:
		BaseEntity(INT32 id)
			: m_ID(id), 
			m_entityState( STATE_ALIVE )
		{			
		}

		virtual ~BaseEntity() {};
		
	public:
		// virtual function
		virtual void Update() = 0;
		virtual void DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const = 0;
		virtual EN_STATE_ID GetCurrentStateID() = 0;
		virtual void SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId ) = 0;
	
		// ===========================================================================================
		
		// Get function
		INT32 ID() const { return m_ID; };

		// Get/Set function
		ST_COORDINATE GetCurLocation() const { return m_currentLocation; };
		void SetCurLocation( INT32 _x, INT32 _y ) 
		{ 
			m_currentLocation.x = _x;
			m_currentLocation.y = _y;
		}

		EN_ENTITY_DIRECTION GetDirection() { return m_direction; }
		void SetDirection( EN_ENTITY_DIRECTION direction )
		{
			m_direction = direction;
		}		
		
		EN_ENTITY_STATE GetEntityState() { return m_entityState; }
		void SetEntityState( EN_ENTITY_STATE  entityState )
		{
			m_entityState = entityState;
		}	
	};
}
