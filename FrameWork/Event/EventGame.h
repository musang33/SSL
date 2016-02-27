#pragma once

#include <Event\Event.h>

namespace SSL
{

	struct EventGame : public Event
	{
		enum
		{
			eEventGameBegin = eEventBaseMax + 1			
			
			, eReqAddPlayer
			, eResAddPlayer

			, eReqMoveEntity
			, eNtfMoveEntity

			, eReqSkillFire
			, eResSkillFire			
		};
	};

EVENT_DECL_BEGIN( ReqAddPlayer, Event, EventGame::eReqAddPlayer )	
	
	virtual bool Pack( PacketStream& bs )
	{
		super::Pack( bs );
		return bs.IsValid( );
	}

	virtual bool Unpack( PacketStream& bs )
	{
		super::Unpack( bs );	
		return bs.IsValid( );
	}
EVENT_DECL_END

EVENT_DECL_BEGIN( ResAddPlayer, Event, EventGame::eResAddPlayer )
	INT32		entityId;
	UINT32		result;

	void Reset( )
	{
		entityId = 0;
		result = 0;		// ¼º°ø
	}

	virtual bool Pack( PacketStream& bs )
	{
		super::Pack( bs );
		bs.Write( entityId );
		bs.Write( result );
		return bs.IsValid( );
	}

	virtual bool Unpack( PacketStream& bs )
	{
		super::Unpack( bs );	
		bs.Read( entityId );
		bs.Read( result );
		return bs.IsValid( );
	}
EVENT_DECL_END

EVENT_DECL_BEGIN( ReqMoveEntity, Event, EventGame::eReqMoveEntity )
	INT32		entityId;
	UINT32		x;
	UINT32		y;

	void Reset( )
	{
		entityId = 0;
		x = 0;
		y = 0;
	}

	virtual bool Pack( PacketStream& bs )
	{
		super::Pack( bs );
		bs.Write( entityId );
		bs.Write( x );
		bs.Write( y );
		return bs.IsValid( );
	}

	virtual bool Unpack( PacketStream& bs )
	{
		super::Unpack( bs );
		bs.Read( entityId );
		bs.Read( x );
		bs.Read( y );
		return bs.IsValid( );
	}
EVENT_DECL_END

EVENT_DECL_BEGIN( NtfMoveEntity, Event, EventGame::eNtfMoveEntity )
	INT32		entityId;
	UINT32		x;
	UINT32		y;

	void Reset( )
	{
		entityId = 0;
		x = 0;
		y = 0;
	}

	virtual bool Pack( PacketStream& bs )
	{
		super::Pack( bs );
		bs.Write( entityId );
		bs.Write( x );
		bs.Write( y );
		return bs.IsValid( );
	}

	virtual bool Unpack( PacketStream& bs )
	{
		super::Unpack( bs );
		bs.Read( entityId );
		bs.Read( x );
		bs.Read( y );
		return bs.IsValid( );
	}
EVENT_DECL_END

}