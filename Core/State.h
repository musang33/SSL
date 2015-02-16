#ifndef STATE_H
#define STATE_H

#include "Singleton.h"

namespace SSL
{	
	template <typename EntityType>
	class State 
	{
	public:
		virtual ~State(){};

		virtual void Enter(EntityType* ) = 0;
		virtual void OnTick(EntityType* ) = 0;
		virtual void Exit(EntityType* ) = 0;
	};
}

#endif