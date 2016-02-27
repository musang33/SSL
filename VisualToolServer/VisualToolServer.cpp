// VisualTool.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <stdio.h>

#include <State/StateNPC.h>
#include <State/StatePlayer.h>
#include <Manager/LuaManager.h>

#include <../FrameWork/Entity/Entity.h>
#include <../FrameWork/Core/TimerWorker.h>
#include <../FrameWork/Event/Event.h>
#include <../FrameWork/Entity/EntityFactory.h>

#include <Action/ActionMove.h>
#include <Action/ActionAI.h>
#include <Action/ActionState.h>


#include <Factory/FactoryNpcImpl.h>
#include <Factory/FactoryPlayerImpl.h>

#include <Manager/MessageManager.h>
#include <Manager/EntityManager.h>

#include <Thread/ThreadEventManager.h>


#include "TestServer.h"

using namespace SSL;

int main( )
{
	TestServer testServer;
	testServer.Initialize();

	testServer.Update( );

}

