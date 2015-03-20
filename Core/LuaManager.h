#pragma once

#include "..\ExternalLibrary\lua-5.3.0\lua_tinker\ScriptBase.hpp"

namespace SSL
{
	class LuaManager : public ScriptBase
	{
	public:
		LuaManager();
		~LuaManager();

	public:
		bool init(const char *str) override;
		void registerStates();
	};

}
