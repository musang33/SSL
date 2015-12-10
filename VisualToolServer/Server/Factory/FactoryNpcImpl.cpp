#include "stdafx.h"

#include <../FrameWork/Entity/Entity.h>

#include <Action/ActionAI.h>
#include <Action/ActionState.h>
#include <Action/ActionMove.h>
#include <Action/ActionNpcFight.h>
#include <Action/ActionBT.h>

#include <Factory/FactoryNpcImpl.h>

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