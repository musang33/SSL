#pragma once

#include <../FrameWork/Core/Action.h>

namespace SSL
{

	class ActionLuaScript :
		public Action
	{
	public:
		static const INT32 ID = EN_ACTION_TYPE::ACTION_LUA_SCRIPT;

	public:
		ActionLuaScript( Entity* entity );
		~ActionLuaScript();

	public:
		void AddHPByRate( INT32 addHPRate );
		UINT32 GetCurrentHPRate();
		bool IsDead();
		void PrintLog( const char* msg );		

		bool IsTargetInSkillDistance();
		bool HasFoundEnemy();
		bool HasEnemyInAggroList();
	};

}