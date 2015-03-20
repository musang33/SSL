#include "LuaManager.h"

#include "..\ExternalLibrary\lua-5.3.0\lua_tinker\ScriptBase.hpp"

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
	return ScriptBase::defaultInti(str);
}

void LuaManager::registerStates()
{

}

}