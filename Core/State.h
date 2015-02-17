#pragma once

#include "Singleton.h"
#include "CommonData.h"

namespace SSL
{	
	template <typename EntityType>
	class State 
	{
	public:
		virtual ~State(){};

		virtual void Enter(EntityType*) = 0;
		virtual void OnTick(EntityType*) = 0;
		virtual void Exit(EntityType*) = 0;

		virtual void OnMessage(EntityType*, const MessageInfo& messageInfo) const {};
	};
}
