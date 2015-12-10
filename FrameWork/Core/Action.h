#pragma once


#include <Entity/Entity.h>

namespace SSL
{
	class Entity;
	class Action
	{
	public:
		explicit Action( Entity* _owner, UINT32 _id)
			: m_owner(_owner), m_id(_id)
		{}
		virtual ~Action(){}

	public:
		Entity* GetOwner() { return m_owner; };
		UINT32 GetId() const { return m_id; };

	private:
		UINT32		m_id;
		Entity*		m_owner;
	};


	struct GetEntityAction
	{
		GetEntityAction( const Entity* _owner ) 
			: m_owner( _owner )
		{}

		template< typename ActionType >
		operator ActionType* ( )
		{
			if ( nullptr == m_owner )
			{
				return nullptr;
			}

			Action* elem = m_owner->GetAction( ActionType::ID );
			if ( nullptr == elem )
			{
				return nullptr;
			}

			return static_cast< ActionType* >( elem );
		}
	private:
		const Entity* m_owner;
	};

}