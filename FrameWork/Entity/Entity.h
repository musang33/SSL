#pragma once

#include <memory>
#include <atomic>

namespace SSL
{		
	class Action;
	class Entity
	{
	private:
		UINT32													m_ID;
		
		Action**												m_actions;
		EN_ENTITY_TYPE											m_type;

	public:
		Entity( UINT32 id, INT32 actionSize, EN_ENTITY_TYPE type )
			: m_ID(id), 			
			currentProcessingThreadID(0), 
			m_type( type )
		{			
			m_actions = new Action*[actionSize];
			for ( int i = 0; i < actionSize; i++ )
			{
				m_actions[i] = nullptr;
			}			
		}

		virtual ~Entity() {};

	public:
		std::atomic<UINT32>										currentProcessingThreadID;
		typedef std::shared_ptr<Entity> BaseEntityPtr;		

	public:	
		
		// Get function
		INT32 ID() const { return m_ID; };
		EN_ENTITY_TYPE Type() const { return m_type; };
				
		void AddAction( Action* action ) const;
		Action* GetAction( UINT32 id ) const;
		
	};
}
