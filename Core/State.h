#ifndef STATE_H
#define STATE_H

#include "Singleton.h"

namespace SSL
{	
	class Miner;

	class State
	{
	public:
		virtual ~State(){};

		virtual void Enter(Miner* miner) = 0;
		virtual void OnTick(Miner* miner) = 0;
		virtual void Exit(Miner* miner) = 0;
	};

	class EnterMineAndDigForNugget final: public State, public Singleton<EnterMineAndDigForNugget>
	{	
	public:
		EnterMineAndDigForNugget() = default;
		~EnterMineAndDigForNugget(){};
		virtual void Enter(Miner* miner);
		virtual void OnTick(Miner* miner);
		virtual void Exit(Miner* miner);

	};

	class GotoBankAndDeposit final : public State, public Singleton<GotoBankAndDeposit>
	{	
	public:
		GotoBankAndDeposit() = default; 
		~GotoBankAndDeposit(){};	

		virtual void Enter(Miner* miner);
		virtual void OnTick(Miner* miner);
		virtual void Exit(Miner* miner);
	};
}

#endif