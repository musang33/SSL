#include "stdafx.h"

#include <Action/ActionAI.h>
#include <Action/ActionState.h>
#include <Action/ActionMove.h>
#include <Action/ActionPlayerFight.h>
#include <Action/ActionBT.h>
#include <Action/ActionPlayer.h>

#include <Factory/FactoryPlayerImpl.h>

#include <../FrameWork/Entity/Entity.h>

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
		
		entity->AddAction( new ActionState( entity ) );
		entity->AddAction( new ActionBT( entity ) );
		entity->AddAction( new ActionMove( entity ) );
		entity->AddAction( new ActionPlayer( entity ) );
		entity->AddAction( new ActionPlayerFight( entity ) );

		return entity;
	}
}