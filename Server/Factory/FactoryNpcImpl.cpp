#include "FactoryNpcImpl.h"
#include "Entity.h"
#include "ActionAI.h"
#include "ActionState.h"
#include "ActionMove.h"
#include "ActionNpcFight.h"
#include "ActionBT.h"

namespace SSL
{

	FactoryNpcImpl::FactoryNpcImpl()
	{
	}


	FactoryNpcImpl::~FactoryNpcImpl()
	{
	}	

	Entity* FactoryNpcImpl::Create( UINT32 entityId )
	{
		Entity* entity = new Entity( entityId, EN_ACTION_TYPE::MAX_ACTION_COUNT, GetFactoryType() );

		entity->AddAction( new ActionAI( entity ) );
		entity->AddAction( new ActionState( entity ) );
		entity->AddAction( new ActionBT( entity ) );
		entity->AddAction( new ActionMove( entity ) );
		entity->AddAction( new ActionNpcFight( entity ) );		

		return entity;
	}
}