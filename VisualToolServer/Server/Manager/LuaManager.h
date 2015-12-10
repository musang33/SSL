#pragma once

#include <..\ExternalLibrary\lua_tinker\ScriptBase.hpp>
#include <../FrameWork/Core/Singleton.h>

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
