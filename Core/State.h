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

		// 스크립트에서 호출
		// NPCBaseState 과 같은 각 State의 부모 State에서만 호출
		virtual void OnEnter( EntityType* entity ){ onEnter( entity ); };
		virtual void OnTick( EntityType* entity ){ onTick( entity ); };
		virtual void OnExit( EntityType* entity ){ onExit( entity ); };

		virtual void OnMessage( EntityType* entity, const MessageInfo& messageInfo ) { onMessage( entity, messageInfo ); };

		UINT32 GetID() { return m_id; }

	protected:
		
		virtual void onEnter( EntityType* ){ };
		virtual void onTick( EntityType* ){ };
		virtual void onExit( EntityType* ){ };

		virtual void onMessage( EntityType* , const MessageInfo&  ) const { };

	private:
		UINT32 m_id;
	};
}
