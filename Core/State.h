#pragma once

#include "Singleton.h"
#include "CommonData.h"

namespace SSL
{	
	class Entity;
	class State 
	{
	public:
		explicit State(UINT32 ID) : m_id(ID) {}
		virtual ~State(){};

		// ��ũ��Ʈ���� ȣ��
		// NPCBaseState �� ���� �� State�� �θ� State������ ȣ��
		virtual void OnEnter( const Entity* entity ){ onEnter( entity ); };
		virtual void OnTick( const Entity* entity ){ onTick( entity ); };
		virtual void OnExit( const Entity* entity ){ onExit( entity ); };

		virtual void OnMessage( const Entity* entity, const ST_MESSAGE_INFO& messageInfo ) { onMessage( entity, messageInfo ); };

		UINT32 GetID() { return m_id; }

	protected:
		
		virtual void onEnter( const Entity* ){ };
		virtual void onTick( const Entity* ){ };
		virtual void onExit( const Entity* ){ };

		virtual void onMessage( const Entity*, const ST_MESSAGE_INFO& ) const { };

	private:
		UINT32 m_id;
	};
}
