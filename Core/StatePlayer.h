#pragma once

#include "State.h"

namespace SSL
{
	class Player;

	class PlayerRoot final : public State<Player>, public Singleton < PlayerRoot >
	{
	public:
		PlayerRoot() :State<Player>(ID){};
		~PlayerRoot(){};

		virtual void Enter(Player* npc){};

		virtual void OnTick(Player* npc){};

		virtual void Exit(Player* npc){};

	private:
		static const UINT32 ID = STATE_NPC_ROOT;
	};

	class PlayerAlive final : public State<Player>, public Singleton < PlayerAlive >
	{
	public:
		PlayerAlive() :State<Player>(ID){};
		~PlayerAlive(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

		virtual void OnMessage(Player* player, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_PLAYER_ALIVE;
	};

	class PlayerDead final : public State<Player>, public Singleton < PlayerDead >
	{
	public:
		PlayerDead() :State<Player>(ID){};
		~PlayerDead(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_DEAD;
	};

	class PlayerPeace final : public State<Player>, public Singleton < PlayerPeace >
	{
	public:
		PlayerPeace() :State<Player>(ID){};
		~PlayerPeace(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

		virtual void OnMessage(Player* player, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_PLAYER_PEACE;
	};

	class PlayerEngage final : public State<Player>, public Singleton < PlayerEngage >
	{
	public:
		PlayerEngage() :State<Player>(ID){};
		~PlayerEngage(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_ENGAGE;
	};

	class PlayerPatrol final : public State<Player>, public Singleton < PlayerPatrol >
	{
	public:
		PlayerPatrol() :State<Player>(ID){};
		~PlayerPatrol(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_PATROL;
	};

	class PlayerGuard final : public State<Player>, public Singleton < PlayerGuard >
	{
	public:
		PlayerGuard() :State<Player>(ID){};
		~PlayerGuard(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_GUARD;
	};

	class PlayerThink final : public State<Player>, public Singleton < PlayerThink >
	{
	public:
		PlayerThink() :State<Player>(ID){};
		~PlayerThink(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_THINK;
	};

	class PlayerAttack final : public State<Player>, public Singleton < PlayerAttack >
	{
	public:
		PlayerAttack() :State<Player>(ID){};
		~PlayerAttack(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_ATTACK;
	};

	class PlayerFlee final : public State<Player>, public Singleton < PlayerFlee >
	{
	public:
		PlayerFlee() :State<Player>(ID){};
		~PlayerFlee(){};

		virtual void Enter(Player* player);

		virtual void OnTick(Player* player);

		virtual void Exit(Player* player);

	private:
		static const UINT32 ID = STATE_PLAYER_FLEE;
	};

} // namespace SSL
