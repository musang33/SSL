#pragma once

#include "State.h"

namespace SSL
{
	class Miner;

	class EnterMineAndDigForNugget final : public State<Miner>, public Singleton < EnterMineAndDigForNugget >
	{
	public:
		EnterMineAndDigForNugget() = default;
		~EnterMineAndDigForNugget(){};

		virtual void Enter(Miner* miner);
		
		virtual void OnTick(Miner* miner);
		
		virtual void Exit(Miner* miner);

		virtual void OnMessage(Miner* miner, const MessageInfo& messageInfo) const;
	};
	
	class GotoBankAndDeposit final : public State<Miner>, public Singleton < GotoBankAndDeposit >
	{
	public:
		GotoBankAndDeposit() = default;
		~GotoBankAndDeposit(){};

		virtual void Enter(Miner* miner);

		virtual void OnTick(Miner* miner);

		virtual void Exit(Miner* miner);
	};

	class Hungry final : public State<Miner>, public Singleton < Hungry >
	{
	public:
		Hungry() = default;
		~Hungry(){};

		virtual void Enter(Miner* miner);

		virtual void OnTick(Miner* miner);

		virtual void Exit(Miner* miner);

		void OnMessage(Miner* miner, const MessageInfo& messageInfo) const;
	};

} // namespace SSL
