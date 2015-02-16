#ifndef STATE_MINER_H
#define STATE_MINER_H

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

} // namespace SSL

#endif // STATEMINER_H