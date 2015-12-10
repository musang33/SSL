#pragma once

#include <../FrameWork/AI/State.h>

namespace SSL
{
	class PlayerRoot final : public State, public Singleton < PlayerRoot >
	{
	public:
		PlayerRoot() :State(ID){};
		~PlayerRoot(){};

		virtual void OnEnter( const Entity* player ){ State::OnEnter( player ); };

		virtual void OnTick( const Entity* player ){ State::OnTick( player ); };

		virtual void OnExit( const Entity* player ){ State::OnExit( player ); };

	private:
		static const UINT32 ID = STATE_NPC_ROOT;
	};

	class PlayerAlive final : public State, public Singleton < PlayerAlive >
	{
	public:
		PlayerAlive() :State(ID){};
		~PlayerAlive(){};

		virtual void onEnter(const Entity* player);

		virtual void OnTick(const Entity* player);

		virtual void onExit(const Entity* player);

		virtual void OnMessage(const Entity* player, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_PLAYER_ALIVE;
	};

	class PlayerDead final : public State, public Singleton < PlayerDead >
	{
	public:
		PlayerDead() :State(ID){};
		~PlayerDead(){};

		virtual void onEnter(const Entity* player);

		virtual void OnTick(const Entity* player);

		virtual void onExit(const Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_DEAD;
	};

	class PlayerPeace final : public State, public Singleton < PlayerPeace >
	{
	public:
		PlayerPeace() :State(ID){};
		~PlayerPeace(){};

		virtual void onEnter(const Entity* player);

		virtual void OnTick(const Entity* player);

		virtual void onExit(const Entity* player);

		virtual void OnMessage(const Entity* player, const ST_MESSAGE_INFO& messageInfo) const;

	private:
		static const UINT32 ID = STATE_PLAYER_PEACE;
	};

	class PlayerEngage final : public State, public Singleton < PlayerEngage >
	{
	public:
		PlayerEngage() :State(ID){};
		~PlayerEngage(){};

		virtual void onEnter(const Entity* player);

		virtual void OnTick(const Entity* player);

		virtual void onExit(const Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_ENGAGE;
	};
	
	class PlayerAttack final : public State, public Singleton < PlayerAttack >
	{
	public:
		PlayerAttack() :State(ID){};
		~PlayerAttack(){};

		virtual void onEnter(const Entity* player);

		virtual void OnTick(const Entity* player);

		virtual void onExit(const Entity* player);

	private:
		static const UINT32 ID = STATE_PLAYER_ATTACK;
	};
	
} // namespace SSL
