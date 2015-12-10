#include "stdafx.h"

#include "ScriptBase.hpp"


ScriptBase::~ScriptBase()
{
  if (_L != nullptr) lua_close( _L );
    _L = NULL;
}

bool ScriptBase::defaultInti()
{
    _L = luaL_newstate();
    if (_L == NULL)
      return false;
    luaL_openlibs( _L );

    lua_tinker::init(_L); //support 64
   
    return true;
}


