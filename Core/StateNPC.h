#pragma once

#include "State.h"

namespace SSL
{
	class Entity;	

	class NPCBaseState : public State
	{
	public:
		NPCBaseState(UINT32 ID) :State(ID){};
		~NPCBaseState(){};

		virtual void OnEnter(Entity* npc);

		virtual void OnTick(Entity* npc);

		virtual void OnExit(Entity* npc);

		virtual void OnMessage( Entity* npc, const ST_MESSAGE_INFO& messageInfo );
	};

	class NPCRoot final : public NPCBaseState, public Singleton < NPCRoot >
	{
	public:
		NPCRoot() :NPCBaseState(ID){};
		~NPCRoot(){};

		virtual void onEnter(Entity* npc){};

		virtual void onTick(Entity* npc){};

		virtual void onExit(Entity* npc){};

	private:
		static const UINT32 ID = STATE_NPC_ROOT;
	};

	class NPCAlive final : public NPCBaseState, public Singleton < NPCAlive >
	{
	public:
		NPCAlive() :NPCBaseState(ID){};
		~NPCAlive(){};

		virtual void onEnter(Entity* npc);
		
		virtual void onTick(Entity* npc);
		
		virtual void onExit(Entity* npc);

		virtual void onMessage(Entity* npc, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_NPC_ALIVE;
	};

	class NPCDead final : public NPCBaseState, public Singleton < NPCDead >
	{
	public:
		NPCDead() :NPCBaseState(ID){};
		~NPCDead(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);		

	private:
		static const UINT32 ID = STATE_NPC_DEAD;
	};
	
	class NPCPeace final : public NPCBaseState, public Singleton < NPCPeace >
	{
	public:
		NPCPeace() :NPCBaseState(ID){};
		~NPCPeace(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

		virtual void onMessage(Entity* npc, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_NPC_PEACE;
	};

	class NPCEngage final : public NPCBaseState, public Singleton < NPCEngage >
	{
	public:
		NPCEngage() :NPCBaseState(ID){};
		~NPCEngage(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

	private:
		static const UINT32 ID = STATE_NPC_ENGAGE;
	};

	class NPCPatrol final : public NPCBaseState, public Singleton < NPCPatrol >
	{
	public:
		NPCPatrol() :NPCBaseState(ID){};
		~NPCPatrol(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

	private:
		static const UINT32 ID = STATE_NPC_PATROL;
	};

	class NPCGuard final : public NPCBaseState, public Singleton < NPCGuard >
	{
	public:
		NPCGuard() :NPCBaseState(ID){};
		~NPCGuard(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

	private:
		static const UINT32 ID = STATE_NPC_GUARD;
	};

	class NPCThink final : public NPCBaseState, public Singleton < NPCThink >
	{
	public:
		NPCThink() :NPCBaseState(ID){};
		~NPCThink(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

	private:
		static const UINT32 ID = STATE_NPC_THINK;
	};

	class NPCAttack final : public NPCBaseState, public Singleton < NPCAttack >
	{
	public:
		NPCAttack() :NPCBaseState(ID){};
		~NPCAttack(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

	private:
		static const UINT32 ID = STATE_NPC_ATTACK;
	};

	class NPCFlee final : public NPCBaseState, public Singleton < NPCFlee >
	{
	public:
		NPCFlee() :NPCBaseState(ID){};
		~NPCFlee(){};

		virtual void onEnter(Entity* npc);

		virtual void onTick(Entity* npc);

		virtual void onExit(Entity* npc);

	private:
		static const UINT32 ID = STATE_NPC_FLEE;
	};

} // namespace SSL
