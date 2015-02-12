#include "CommonData.h"
#include "GameEntity.h"
#include "State.h"

namespace SSL
{
	Miner::Miner(int id, LOCATION location, State* state)
		:BaseEntity(id)
		, m_currentLocation(location)
		, m_currentState(state)
		, m_nuggetSize(0)
		, m_gold(0)
	{
		std::cout << "[INFO][Miner][$$:create new Miner]" << std::endl;
	}

	void Miner::Update()
	{
		if ( nullptr != m_currentState )
		{
			m_currentState->OnTick(this);
		}
	}

	void Miner::ChangeState(State* newState)
	{
		if ( m_currentState == newState )
		{
			std::cout << "[ERROR][Miner][ChangeState][$$:newState is same with currentState]" << std::endl;
			return;
		}

		if ( nullptr == newState )
		{
			std::cout << "[ERROR][Miner][ChangeState][$$:newState is nullptr]" << std::endl;
			return;
		}

		if ( nullptr == m_currentState )
		{
			m_currentState = newState;
			m_currentState->Enter(this);
		}

		m_currentState->Exit(this);
		m_currentState = newState;
		m_currentState->Enter(this);
	}

	void Miner::GotoLocation(LOCATION location)
	{
		m_currentLocation = location;
		std::cout << "[INFO][Miner][$$:Goto : " << location << "]" << std::endl;
	}

	void Miner::MineGold()
	{
		std::cout << "[INFO][Miner][MineGold][$$Mining Gold]" << std::endl;
		m_nuggetSize += (rand() % 30);
	}

	bool Miner::IsCurrentLocation(LOCATION location)
	{
		if ( m_currentLocation == location )
		{
			return true;
		}

		return false;
	}

	bool Miner::IsFullNugget()
	{
		if ( m_nuggetSize >= MAX_NUGGET_SIZE )
		{
			return true;
		}

		return false;
	}

	void Miner::ChangeNuggetToGoldAndSave()
	{
		std::cout << "[INFO][Miner][SaveNugget][$$Cange Nugget to gold and save]" << std::endl;
		m_gold += (m_nuggetSize / 10);
	}

}