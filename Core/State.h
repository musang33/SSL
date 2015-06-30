#pragma once

#include "Singleton.h"
#include "CommonData.h"

namespace SSL
{	
	template <typename EntityType>
	class State 
	{
	public:
		explicit State(UINT32 ID) : m_id(ID) {}
		virtual ~State(){};

		// ��ũ��Ʈ���� ȣ��
		// NPCBaseState �� ���� �� State�� �θ� State������ ȣ��
		virtual void OnEnter( EntityType* entity ){ onEnter( entity ); };
		virtual void OnTick( EntityType* entity ){ onTick( entity ); };
		virtual void OnExit( EntityType* entity ){ onExit( entity ); };

		virtual void OnMessage( EntityType* entity, const ST_MESSAGE_INFO& messageInfo ) { onMessage( entity, messageInfo ); };

		UINT32 GetID() { return m_id; }

	protected:
		
		virtual void onEnter( EntityType* ){ };
		virtual void onTick( EntityType* ){ };
		virtual void onExit( EntityType* ){ };

		virtual void onMessage( EntityType* , const ST_MESSAGE_INFO&  ) const { };

	private:
		UINT32 m_id;
	};
}
