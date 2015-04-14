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
		virtual void OnEnter(EntityType*){};
		virtual void OnTick(EntityType*){};
		virtual void OnExit(EntityType*){};

		virtual void OnMessage(EntityType*, const MessageInfo& messageInfo) const {};

		UINT32 GetID() { return m_id; }

	protected:
		
		virtual void onEnter( EntityType* entity ){ onEnter( entity ); };
		virtual void onTick( EntityType* entity ){ onTick( entity ); };
		virtual void onExit( EntityType* entity ){ onExit( entity ); };

	private:
		UINT32 m_id;
	};
}
