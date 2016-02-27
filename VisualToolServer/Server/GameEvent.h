#pragma once

#include <..\FrameWork\Event\Event.h>

namespace SSL
{

	struct GameProtocol
	{
		enum
		{
			EREQ_ENTER_SERVER = 10000,
			ERES_ENTER_SERVER,

			EREQ_PC_MOVE,
			ERES_PC_MOVE,
			EREQ_PC_ATACK,
			ERES_PC_ATACK,
			EREQ_PC_DAMAGED,
			ERES_PC_DAMAGED,

			EREQ_NPC_MOVE,
			ERES_NPC_MOVE,
			EREQ_NPC_ATACK,
			ERES_NPC_ATACK,
			EREQ_NPC_DAMAGED,
			ERES_NPC_DAMAGED
		};
	};


	EVENT_DECL_BEGIN( EReqEnterServer, Event, GameProtocol::EREQ_ENTER_SERVER )
		socketIndex;
	UINT32	result;

	void Reset( )
	{
		result = 0;
	}

	virtual bool Pack( PacketStream& bs )
	{
		super::Pack( bs );
		bs.Write( socketIndex );
		bs.Write( result );

		return bs.IsValid( );
	}

	virtual bool Unpack( PacketStream& bs )
	{
		super::Unpack( bs );
		bs.Read( socketIndex );
		bs.Read( result );

		return bs.IsValid( );
	}

	EVENT_DECL_END

}