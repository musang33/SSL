#pragma once

#include "CommonData.h"
#include "..\ExternalLibrary\lua-5.3.0\lua_tinker\ScriptBase.hpp"
#include "Singleton.h"

namespace SSL
{
	class LuaManager : public ScriptBase, public Singleton<LuaManager>
	{
	public:
		LuaManager();
		~LuaManager();

	public:
		bool init(const char *str) override;
		bool CallLuaFunction(UINT32 stateId, const char* aiIndex, const char* instanceName, const char* function);

	private:
		void registerStates();	
		void registerNPCFunctions();
	};

}
