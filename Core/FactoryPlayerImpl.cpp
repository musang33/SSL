#include "FactoryPlayerImpl.h"
#include "Entity.h"
#include "ActionAI.h"
#include "ActionState.h"
#include "ActionMove.h"
#include "ActionPlayerFight.h"
#include "ActionBT.h"

namespace SSL
{

	FactoryPlayerImpl::FactoryPlayerImpl()
	{
	}


	FactoryPlayerImpl::~FactoryPlayerImpl()
	{
	}	

	Entity* FactoryPlayerImpl::Create( UINT32 entityId )
	{
		Entity* entity = new Entity( entityId, EN_ACTION_TYPE::MAX_ACTION_COUNT, GetFactoryType() );

		entity->AddAction( new ActionAI( entity ) );
		entity->AddAction( new ActionState( entity ) );
		entity->AddAction( new ActionBT( entity ) );
		entity->AddAction( new ActionMove( entity ) );
		entity->AddAction( new ActionPlayerFight( entity ) );

		return entity;
	}
}