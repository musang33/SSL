#include "Player.h"
#include "StatePlayer.h"

#include <random>
#include <chrono>
#include <functional>

namespace SSL {

	Player::Player(int id, State<Player>* state)
		:BaseEntity(id)		
		, m_maxHP(1000)
		, m_currentHP(1000)		
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm = new HFSM<Player>(this);
			initHFSMState();
			m_hfsm->SetCurrentState(state);
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree = new BehaviorTreeManager<Player>( this );
			if ( nullptr != m_behaviorTree )
			{
				m_behaviorTree->SetBehaviorTree();
			}
			else
			{
				exit( 0 );
			}
		}
		else
		{
			m_fsm = new FSM < Player >( this );
			m_fsm->SetCurrentState( state );
		}	

		std::cout << "[INFO][Player][$$:create new Player]" << std::endl;
	}

	void Player::initHFSMState()
	{
		m_hfsm->RegisterState(nullptr, PlayerRoot::GetInstance());

		m_hfsm->RegisterState(PlayerRoot::GetInstance(), PlayerAlive::GetInstance());
		m_hfsm->RegisterState(PlayerRoot::GetInstance(), PlayerDead::GetInstance());

		m_hfsm->RegisterState(PlayerAlive::GetInstance(), PlayerPeace::GetInstance());
		m_hfsm->RegisterState(PlayerAlive::GetInstance(), PlayerEngage::GetInstance());

		m_hfsm->RegisterState(PlayerPeace::GetInstance(), PlayerPatrol::GetInstance());
		m_hfsm->RegisterState(PlayerPeace::GetInstance(), PlayerGuard::GetInstance());

		m_hfsm->RegisterState(PlayerEngage::GetInstance(), PlayerThink::GetInstance());
		m_hfsm->RegisterState(PlayerEngage::GetInstance(), PlayerAttack::GetInstance());
		m_hfsm->RegisterState(PlayerEngage::GetInstance(), PlayerFlee::GetInstance());
	}

	void Player::Update()
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm->Update();
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			m_behaviorTree->Update();
		}
		else
		{
			m_fsm->Update();
		}		
	}

	void Player::DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			m_hfsm->DealWithMessage(messageInfo);
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{

		}
		else
		{
			m_fsm->DealWithMessage(messageInfo);
		}
	}

	EN_STATE_ID Player::GetCurrentStateID()
	{
		if ( EN_AI_TYPE::AI_TYPE_HFSM == AIType )
		{
			return m_hfsm->GetCurrentState();
		}
		else if ( EN_AI_TYPE::AI_TYPE_BT == AIType )
		{
			return m_behaviorTree->GetCurrentState();
		}
		else
		{
			return m_fsm->GetCurrentState();
		}

		return EN_STATE_ID::STATE_NONE;
	}

	void Player::SetCurrentStateIDInBehaviorTree( EN_STATE_ID stateId )
	{
		m_behaviorTree->SetCurrentStateID( stateId );
	}

	void Player::AddHPByRate(INT32 addHPRate)
	{
		std::cout << "[INFO][Player][AddHP]" << std::endl;		

		INT32 addHP = m_maxHP * addHPRate / 10000;
		m_currentHP += addHP;

		if ( m_currentHP >= m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
	}

	UINT32 Player::GetCurrentHPRate()
	{
		if ( m_maxHP == 0 )
		{
			return 0;
		}

		return m_currentHP * 10000 / m_maxHP;
	}

	bool Player::IsDead()
	{
		if ( 0 > m_currentHP )
		{
			return true;
		}

		return false;
	}

	bool Player::IsTargetInSkillDistance()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, 100),
			std::mt19937_64(seed));

		if ( dice_rand() < 50 )
		{
			return false;
		}

		return true;
	}

	bool Player::HasEnemyInAggroList()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, 100),
			std::mt19937_64(seed));

		if ( dice_rand() < 50 )
		{
			return false;
		}

		return true;
	}

	EN_BEHAVIOR_STATE Player::CheckHP()
	{
		if ( GetCurrentHP() < 1 )
		{
			SetEntityState( EN_ENTITY_STATE::STATE_DEAD );
			return BH_INVALID;
		}

		return BH_SUCCESS;
	}

	void Player::AddHP( INT32 addHP )
	{
		m_currentHP += addHP;
		if ( m_currentHP > m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
		else if ( m_currentHP < 0 )
		{
			m_currentHP = 0;
		}
	}

}