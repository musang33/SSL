#pragma once

#include "State.h"

namespace SSL
{
	class Miner;

	class EnterMineAndDigForNugget final : public State<Miner>, public Singleton < EnterMineAndDigForNugget >
	{
	public:
		EnterMineAndDigForNugget() :State<Miner>(ID){};
		~EnterMineAndDigForNugget(){};

		virtual void Enter(Miner* miner);
		
		virtual void OnTick(Miner* miner);
		
		virtual void Exit(Miner* miner);

		virtual void OnMessage(Miner* miner, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_MINER_ENTER_MINE_AND_DIG_FOR_NUGGET;
	};
	
	class GotoBankAndDeposit final : public State<Miner>, public Singleton < GotoBankAndDeposit >
	{
	public:
		GotoBankAndDeposit() :State<Miner>(ID){};
		~GotoBankAndDeposit(){};

		virtual void Enter(Miner* miner);

		virtual void OnTick(Miner* miner);

		virtual void Exit(Miner* miner);

	private:
		static const UINT32 ID = STATE_MINER_GOTO_BAND_AND_DEPOSIT;
	};

	class Hungry final : public State<Miner>, public Singleton < Hungry >
	{
	public:
		Hungry() :State<Miner>(ID){};
		~Hungry(){};

		virtual void Enter(Miner* miner);

		virtual void OnTick(Miner* miner);

		virtual void Exit(Miner* miner);

		void OnMessage(Miner* miner, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_MINER_HUNGRY;
	};

} // namespace SSL
