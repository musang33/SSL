#include "CommonData.h"
#include "NPC.h"
#include "StateNpc.h"

#include <random>
#include <chrono>
#include <functional>

namespace SSL
{
	NPC::NPC(int id, LOCATION location, State<NPC>* state)
		:BaseEntity(id)
		, m_currentLocation(location)		
		, m_maxHP(1000)
		, m_currentHP(1000)
		, m_strikingPower(0)
	{
		if ( USE_HSM )
		{
			m_hfsm = new HFSM<NPC>(this);
			initState();
			m_hfsm->SetCurrentState(state);
		}
		else
		{
			m_stateManager = new StateManager < NPC >(this);
			m_stateManager->SetCurrentState(state);
		}	

		std::cout << "[INFO][NPC][$$:create new NPC]" << std::endl;
	}

	void NPC::initState()
	{
		m_hfsm->RegisterState(nullptr, NPCRoot::GetInstance());

		m_hfsm->RegisterState(NPCRoot::GetInstance(), NPCAlive::GetInstance());
		m_hfsm->RegisterState(NPCRoot::GetInstance(), NPCDead::GetInstance());

		m_hfsm->RegisterState(NPCAlive::GetInstance(), NPCPeace::GetInstance());
		m_hfsm->RegisterState(NPCAlive::GetInstance(), NPCEngage::GetInstance());

		m_hfsm->RegisterState(NPCPeace::GetInstance(), NPCPatrol::GetInstance());
		m_hfsm->RegisterState(NPCPeace::GetInstance(), NPCGuard::GetInstance());

		m_hfsm->RegisterState(NPCEngage::GetInstance(), NPCThink::GetInstance());
		m_hfsm->RegisterState(NPCEngage::GetInstance(), NPCAttack::GetInstance());
		m_hfsm->RegisterState(NPCEngage::GetInstance(), NPCFlee::GetInstance());
	}

	void NPC::Update()
	{	
		if ( USE_HSM )
		{
			m_hfsm->Update();
		}
		else
		{
			m_stateManager->Update();
		}		
	}

	void NPC::DealWithMessage(const MessageInfo& messageInfo) const
	{
		if ( USE_HSM )
		{
			m_hfsm->DealWithMessage(messageInfo);
		}
		else
		{
			m_stateManager->DealWithMessage(messageInfo);
		}		
	}
	
	void NPC::GotoLocation(LOCATION location)
	{
		m_currentLocation = location;
		std::cout << "[INFO][NPC][$$:Goto : " << location << "]" << std::endl;
	}

	bool NPC::IsCurrentLocation(LOCATION location) const
	{
		if ( m_currentLocation == location )
		{
			return true;
		}

		return false;
	}

	void NPC::AddHPByRate(INT32 addHPRate)
	{
		std::cout << "[INFO][NPC][AddHP]" << std::endl;		

		INT32 addHP = m_maxHP * addHPRate / 10000;
		m_currentHP += addHP;

		if ( m_currentHP >= m_maxHP )
		{
			m_currentHP = m_maxHP;
		}
	}

	UINT32 NPC::GetCurrentHPRate()
	{
		if ( m_maxHP == 0 )
		{
			return 0;
		}
			 
		return m_currentHP * 10000 / m_maxHP;
	}

	bool NPC::IsDead()
	{
		if ( 0 > m_currentHP )
		{
			return true;
		}

		return false;
	}

	bool NPC::HasFoundEnemy()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		auto dice_rand = std::bind(std::uniform_int_distribution<int>(1, 100),
			std::mt19937_64(seed));

		if ( dice_rand() < 30 )
		{
			return false;
		}

		return true;
	}

	bool NPC::IsTargetInSkillDistance()
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

	bool NPC::HasEnemyInAggroList()
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

}