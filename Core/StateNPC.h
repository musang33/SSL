#pragma once

#include "State.h"

namespace SSL
{
	class NPC;	

	class NPCRoot final : public State<NPC>, public Singleton < NPCRoot >
	{
	public:
		NPCRoot() :State<NPC>(ID){};
		~NPCRoot(){};

		virtual void Enter(NPC* npc){};

		virtual void OnTick(NPC* npc){};

		virtual void Exit(NPC* npc){};

	private:
		static const UINT32 ID = STATE_NPC_ROOT;
	};

	class NPCAlive final : public State<NPC>, public Singleton < NPCAlive >
	{
	public:
		NPCAlive() :State<NPC>(ID){};
		~NPCAlive(){};

		virtual void Enter(NPC* npc);
		
		virtual void OnTick(NPC* npc);
		
		virtual void Exit(NPC* npc);

		virtual void OnMessage(NPC* npc, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_NPC_ALIVE;
	};

	class NPCDead final : public State<NPC>, public Singleton < NPCDead >
	{
	public:
		NPCDead() :State<NPC>(ID){};
		~NPCDead(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);		

	private:
		static const UINT32 ID = STATE_NPC_DEAD;
	};
	
	class NPCPeace final : public State<NPC>, public Singleton < NPCPeace >
	{
	public:
		NPCPeace() :State<NPC>(ID){};
		~NPCPeace(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

		virtual void OnMessage(NPC* npc, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_NPC_PEACE;
	};

	class NPCEngage final : public State<NPC>, public Singleton < NPCEngage >
	{
	public:
		NPCEngage() :State<NPC>(ID){};
		~NPCEngage(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_ENGAGE;
	};

	class NPCPatrol final : public State<NPC>, public Singleton < NPCPatrol >
	{
	public:
		NPCPatrol() :State<NPC>(ID){};
		~NPCPatrol(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_PATROL;
	};

	class NPCGuard final : public State<NPC>, public Singleton < NPCGuard >
	{
	public:
		NPCGuard() :State<NPC>(ID){};
		~NPCGuard(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_GUARD;
	};

	class NPCThink final : public State<NPC>, public Singleton < NPCThink >
	{
	public:
		NPCThink() :State<NPC>(ID){};
		~NPCThink(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_THINK;
	};

	class NPCAttack final : public State<NPC>, public Singleton < NPCAttack >
	{
	public:
		NPCAttack() :State<NPC>(ID){};
		~NPCAttack(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_ATTACK;
	};

	class NPCFlee final : public State<NPC>, public Singleton < NPCFlee >
	{
	public:
		NPCFlee() :State<NPC>(ID){};
		~NPCFlee(){};

		virtual void Enter(NPC* npc);

		virtual void OnTick(NPC* npc);

		virtual void Exit(NPC* npc);

	private:
		static const UINT32 ID = STATE_NPC_FLEE;
	};

} // namespace SSL
