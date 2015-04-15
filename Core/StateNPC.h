#pragma once

#include "State.h"

namespace SSL
{
	class NPC;	

	class NPCBaseState : public State<NPC>
	{
	public:
		NPCBaseState(UINT32 ID) :State<NPC>(ID){};
		~NPCBaseState(){};

		virtual void OnEnter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void OnExit(NPC* npc);

		virtual void OnMessage( NPC* npc, const MessageInfo& messageInfo );
	};

	class NPCRoot final : public NPCBaseState, public Singleton < NPCRoot >
	{
	public:
		NPCRoot() :NPCBaseState(ID){};
		~NPCRoot(){};

		virtual void onEnter(NPC* npc){};

		virtual void onTick(NPC* npc){};

		virtual void onExit(NPC* npc){};

	private:
		static const UINT32 ID = STATE_NPC_ROOT;
	};

	class NPCAlive final : public NPCBaseState, public Singleton < NPCAlive >
	{
	public:
		NPCAlive() :NPCBaseState(ID){};
		~NPCAlive(){};

		virtual void onEnter(NPC* npc);
		
		virtual void onTick(NPC* npc);
		
		virtual void onExit(NPC* npc);

		virtual void onMessage(NPC* npc, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_NPC_ALIVE;
	};

	class NPCDead final : public NPCBaseState, public Singleton < NPCDead >
	{
	public:
		NPCDead() :NPCBaseState(ID){};
		~NPCDead(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);		

	private:
		static const UINT32 ID = STATE_NPC_DEAD;
	};
	
	class NPCPeace final : public NPCBaseState, public Singleton < NPCPeace >
	{
	public:
		NPCPeace() :NPCBaseState(ID){};
		~NPCPeace(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

		virtual void onMessage(NPC* npc, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_NPC_PEACE;
	};

	class NPCEngage final : public NPCBaseState, public Singleton < NPCEngage >
	{
	public:
		NPCEngage() :NPCBaseState(ID){};
		~NPCEngage(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_ENGAGE;
	};

	class NPCPatrol final : public NPCBaseState, public Singleton < NPCPatrol >
	{
	public:
		NPCPatrol() :NPCBaseState(ID){};
		~NPCPatrol(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_PATROL;
	};

	class NPCGuard final : public NPCBaseState, public Singleton < NPCGuard >
	{
	public:
		NPCGuard() :NPCBaseState(ID){};
		~NPCGuard(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_GUARD;
	};

	class NPCThink final : public NPCBaseState, public Singleton < NPCThink >
	{
	public:
		NPCThink() :NPCBaseState(ID){};
		~NPCThink(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_THINK;
	};

	class NPCAttack final : public NPCBaseState, public Singleton < NPCAttack >
	{
	public:
		NPCAttack() :NPCBaseState(ID){};
		~NPCAttack(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_ATTACK;
	};

	class NPCFlee final : public NPCBaseState, public Singleton < NPCFlee >
	{
	public:
		NPCFlee() :NPCBaseState(ID){};
		~NPCFlee(){};

		virtual void onEnter(NPC* npc);

		virtual void onTick(NPC* npc);

		virtual void onExit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_FLEE;
	};

} // namespace SSL
