#pragma once

#include "State.h"

namespace SSL
{
	class Entity;

	class PlayerRoot final : public State, public Singleton < PlayerRoot >
	{
	public:
		PlayerRoot() :State(ID){};
		~PlayerRoot(){};

		virtual void OnEnter( Entity* player ){ State::OnEnter( player ); };

		virtual void OnTick( Entity* player ){ State::OnTick( player ); };

		virtual void OnExit( Entity* player ){ State::OnExit( player ); };

	private:
		static const UINT32 ID = STATE_NPC_ROOT;
	};

	class PlayerAlive final : public State, public Singleton < PlayerAlive >
	{
	public:
		PlayerAlive() :State(ID){};
		~PlayerAlive(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

		virtual void OnMessage(Entity* player, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_PLAYER_ALIVE;
	};

	class PlayerDead final : public State, public Singleton < PlayerDead >
	{
	public:
		PlayerDead() :State(ID){};
		~PlayerDead(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_DEAD;
	};

	class PlayerPeace final : public State, public Singleton < PlayerPeace >
	{
	public:
		PlayerPeace() :State(ID){};
		~PlayerPeace(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

		virtual void OnMessage(Entity* player, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_PLAYER_PEACE;
	};

	class PlayerEngage final : public State, public Singleton < PlayerEngage >
	{
	public:
		PlayerEngage() :State(ID){};
		~PlayerEngage(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_ENGAGE;
	};

	class PlayerPatrol final : public State, public Singleton < PlayerPatrol >
	{
	public:
		PlayerPatrol() :State(ID){};
		~PlayerPatrol(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_PATROL;
	};

	class PlayerGuard final : public State, public Singleton < PlayerGuard >
	{
	public:
		PlayerGuard() :State(ID){};
		~PlayerGuard(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_GUARD;
	};

	class PlayerThink final : public State, public Singleton < PlayerThink >
	{
	public:
		PlayerThink() :State(ID){};
		~PlayerThink(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_THINK;
	};

	class PlayerAttack final : public State, public Singleton < PlayerAttack >
	{
	public:
		PlayerAttack() :State(ID){};
		~PlayerAttack(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_ATTACK;
	};

	class PlayerFlee final : public State, public Singleton < PlayerFlee >
	{
	public:
		PlayerFlee() :State(ID){};
		~PlayerFlee(){};

		virtual void onEnter(Entity* player);

		virtual void OnTick(Entity* player);

		virtual void onExit(Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_FLEE;
	};

} // namespace SSL
