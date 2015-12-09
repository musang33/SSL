#include "LuaManager.h"
#include "CommonData.h"

#include "..\ExternalLibrary\lua-5.3.0\lua_tinker\ScriptBase.hpp"

#include "ActionLuaScript.h"

namespace SSL
{
	LuaManager::LuaManager()
	{
	}


	LuaManager::~LuaManager()
	{
	}

	bool LuaManager::init(const char *str)
	{
		// Lua 초기화
		if ( false == ScriptBase::defaultInti() )
		{
			return false;
		}

		// class, global value 등 설정
		registerStates();

		// Lua에서 호출할 함수 인터페이스 정의
		registerNPCFunctions();

		// Lua 스크립트 실행
		ScriptBase::dofile(str);

		return true;
	}

	void LuaManager::registerStates()
	{
		// Player
		ScriptBase::setglobal("PlayerRoot", STATE_NPC_ROOT);

		ScriptBase::setglobal("PlayerAlive", STATE_PLAYER_ALIVE);
		ScriptBase::setglobal("PlayerDead", STATE_PLAYER_DEAD);

		ScriptBase::setglobal("PlayerPeace", STATE_PLAYER_PEACE);
		ScriptBase::setglobal("PlayerEngage", STATE_PLAYER_ENGAGE);

		ScriptBase::setglobal("PlayerPatrol", STATE_PLAYER_PATROL);
		ScriptBase::setglobal("PlayerGuard", STATE_PLAYER_GUARD);

		ScriptBase::setglobal("PlayerThink", STATE_PLAYER_THINK);
		ScriptBase::setglobal("PlayerAttack", STATE_PLAYER_ATTACK);
		ScriptBase::setglobal("PlayerFlee", STATE_PLAYER_FLEE);

		// NPC
		ScriptBase::setglobal("NPCRoot", STATE_NPC_ROOT);

		ScriptBase::setglobal("NPCAlive", STATE_NPC_ALIVE);
		ScriptBase::setglobal("NPCDead", STATE_NPC_DEAD);

		ScriptBase::setglobal("NPCPeace", STATE_NPC_PEACE);
		ScriptBase::setglobal("NPCEngage", STATE_NPC_ENGAGE);

		ScriptBase::setglobal("NPCPatrol", STATE_NPC_PATROL);
		ScriptBase::setglobal("NPCGuard", STATE_NPC_GUARD);

		ScriptBase::setglobal("NPCThink", STATE_NPC_THINK);
		ScriptBase::setglobal("NPCAttack", STATE_NPC_ATTACK);
		ScriptBase::setglobal("NPCFlee", STATE_NPC_FLEE);
	}

	void LuaManager::registerNPCFunctions()
	{		
		ScriptBase::regClass<ActionLuaScript>("NPC");
		
		ScriptBase::regClassFunc<ActionLuaScript>("AddHPByRate", &ActionLuaScript::AddHPByRate);
		ScriptBase::regClassFunc<ActionLuaScript>("GetCurrentHPRate", &ActionLuaScript::GetCurrentHPRate);
		ScriptBase::regClassFunc<ActionLuaScript>("IsDead", &ActionLuaScript::IsDead);
		ScriptBase::regClassFunc<ActionLuaScript>("HasFoundEnemy", &ActionLuaScript::HasFoundEnemy);
		ScriptBase::regClassFunc<ActionLuaScript>("IsTargetInSkillDistance", &ActionLuaScript::IsTargetInSkillDistance);
		ScriptBase::regClassFunc<ActionLuaScript>("HasEnemyInAggroList", &ActionLuaScript::HasEnemyInAggroList);
		ScriptBase::regClassFunc<ActionLuaScript>("PrintLog", &ActionLuaScript::PrintLog);
	}

	bool LuaManager::CallLuaFunction(UINT32 stateId, const char* aiIndex, const char* instanceName, const char* function)
	{
		bool result = false;

		if ( nullptr == function )
		{
			return false;
		}

		lua_getglobal(_L, aiIndex);

		if ( lua_istable(_L, -1) )
		{
			// get state table
			lua_pushnumber(_L, stateId);
			lua_gettable(_L, -2);

			if ( lua_istable(_L, -1) )
			{
				lua_pushcclosure(_L, lua_tinker::on_error, 0);
				int errFunc = lua_gettop(_L);

				// get function
				lua_pushfstring(_L, function);
				lua_gettable(_L, -3); // function / on_error / state

				if ( lua_isfunction(_L, -1) )
				{
					lua_getglobal(_L, instanceName);
					if ( lua_isnil(_L, -1) )
					{
						result = false;
					}
					else
					{
						lua_pcall(_L, 1, 0, errFunc);
						result = true;
					}				
				}
				else
				{
					lua_pop(_L, 1);	// pop function
				}

				lua_remove(_L, errFunc);
			}			

			lua_pop(_L, 1); // pop state table
		}

		lua_pop(_L, 1); // pop npc table

		return result;
	}
} // namespace SSL