#include "stdafx.h"

#include <Action/ActionLuaScript.h>

#include <Action/ActionState.h>
#include <Action/ActionNpcFight.h>

namespace SSL
{

	ActionLuaScript::ActionLuaScript( Entity* entity )
		:Action( entity, EN_ACTION_TYPE::ACTION_LUA_SCRIPT )
	{
	}


	ActionLuaScript::~ActionLuaScript()
	{
	}

	void ActionLuaScript::PrintLog( const char* msg )
	{
		if ( nullptr == msg || 0 == strlen( msg ) )
		{
			return;
		}

		std::cout << msg << std::endl;
	}

	UINT32 ActionLuaScript::GetCurrentHPRate()
	{
		ActionState* as = GetEntityAction( GetOwner() );
		return as->GetCurrentHPRate();
	}

	bool ActionLuaScript::IsDead()
	{
		ActionState* as = GetEntityAction( GetOwner() );
		return as->IsDead();
	}

	void ActionLuaScript::AddHPByRate( INT32 addHPRate )
	{
		ActionState* as = GetEntityAction( GetOwner() );
		return as->AddHPByRate( addHPRate );
	}

	bool ActionLuaScript::IsTargetInSkillDistance()
	{
		ActionNpcFight* anf = GetEntityAction( GetOwner() );
		return anf->IsTargetInSkillDistance();
	}

	bool ActionLuaScript::HasFoundEnemy()
	{
		ActionNpcFight* anf = GetEntityAction( GetOwner() );
		return anf->HasFoundEnemy();
	}

	bool ActionLuaScript::HasEnemyInAggroList()
	{
		ActionNpcFight* anf = GetEntityAction( GetOwner() );
		return anf->HasEnemyInAggroList();
	}
}