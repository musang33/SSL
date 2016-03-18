#include "stdafx.h"
#include "Npc.h"

namespace SSL
{

	Npc::Npc( unsigned int id )
		: Entity( id, 0, SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE )
		, m_isDead( false )
		, m_state( 0 )
	{
	}

	Npc::~Npc( )
	{
	}
}