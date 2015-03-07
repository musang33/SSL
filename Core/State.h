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

		virtual void Enter(EntityType*){};
		virtual void OnTick(EntityType*){};
		virtual void Exit(EntityType*){};

		virtual void OnMessage(EntityType*, const MessageInfo& messageInfo) const {};

		UINT32 GetID() { return m_id; }

	private:
		UINT32 m_id;
	};
}
