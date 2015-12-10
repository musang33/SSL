#include "stdafx.h"

#include <State/StateNPC.h>
#include <State/StatePlayer.h>
#include <Manager/EntityManager.h>
#include <Manager/MessageManager.h>
#include <Manager/LuaManager.h>

using namespace SSL;

//int main()
//{			
//	LuaManager::GetInstance()->init(".\\lua_script\\script_list.lua");
//
//	NPC* npc = new NPC(ID_RANGE_NPC, NPCPatrol::GetInstance());
//	Player* player = new Player(ID_RANGE_PLAYER, PlayerPatrol::GetInstance());
//
//	EntityManager::GetInstance()->RegisterEntity(npc);
//	EntityManager::GetInstance()->RegisterEntity(player);
//	
//	while ( 1 )
//	{
//		npc->Update();
//		player->Update();
//
//		MessageManager::GetInstance()->DispatchDelayedMessage();
//
//		Sleep(1000);
//	}
//
//	return 0;
//}

