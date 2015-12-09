#include "stdafx.h"

#include <Network/NetWorkCommon/PacketStream.h>


namespace SSL
{
	PacketStream::PacketStream()
	{
		m_data = new BYTE[EN_DEFAULT_BUFFER_SIZE];
		m_pos = m_data;
		m_end = m_data + EN_DEFAULT_BUFFER_SIZE;
		m_dataLength = EN_DEFAULT_BUFFER_SIZE;
		m_increaseSize = EN_DEFAULT_INCREASE_SIZE;
		m_isValid = true;

		::ZeroMemory( m_data, sizeof( BYTE ) * EN_DEFAULT_BUFFER_SIZE );
	}

	PacketStream::PacketStream(UINT32 bufferSize)
	{
		if ( bufferSize < EN_DEFAULT_BUFFER_SIZE )
		{
			bufferSize = EN_DEFAULT_BUFFER_SIZE;
		}

		m_data = new BYTE[bufferSize];
		m_pos = m_data;
		m_end = m_data + bufferSize;
		m_dataLength = bufferSize;
		m_increaseSize = EN_DEFAULT_INCREASE_SIZE;
		m_isValid = true;

		::ZeroMemory( m_data, sizeof( BYTE ) * bufferSize );
	}


	PacketStream::~PacketStream()
	{
		if ( nullptr != m_data )
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

	void PacketStream::ResetIncreaseSize( UINT32 increaseSize )
	{
		if ( increaseSize < EN_DEFAULT_INCREASE_SIZE )
		{
			return;
		}

		m_increaseSize = increaseSize;
	}	

	void PacketStream::ResetBufferSize( UINT32 bufferSize )
	{
		if ( bufferSize < EN_DEFAULT_BUFFER_SIZE )
		{
			return;
		}

		UINT32 newSize = EN_DEFAULT_BUFFER_SIZE;

		while ( newSize < bufferSize )
		{
			newSize += m_increaseSize;
		}

		BYTE* tempBuffer = new BYTE[newSize];

		ZeroMemory( tempBuffer, sizeof( BYTE ) * newSize );
		memcpy( tempBuffer, m_data, m_dataLength );

		m_pos = tempBuffer + ( m_pos - m_data );
		m_end = tempBuffer + newSize;
		m_dataLength = newSize;

		delete[] m_data;
		m_data = tempBuffer;
	}

	void PacketStream::Reset()
	{
		m_pos = m_data;
		m_isValid = true;
	}

	UINT32 PacketStream::Write( const BYTE* data, UINT32 size )
	{
		if ( m_pos + size > m_end )
		{
			UINT32 resize = size - ( m_end - m_pos );
			ResetBufferSize( m_dataLength + resize );
		}

		memcpy( m_data, data, size );
		m_pos += size;

		return size;
	}

	UINT32 PacketStream::Read( BYTE* data, UINT32 size )
	{
		if ( nullptr == data || m_isValid == false )
		{
			return 0;
		}

		UINT32 copyLength = size;
		if ( m_pos + size > m_end )
		{
			m_isValid = false;		
			copyLength = m_end - m_pos;
		}

		if ( copyLength == 0 )
		{
			return 0;
		}

		assert( copyLength >= size );

		memcpy( data, m_pos, copyLength );
		m_pos += copyLength;		

		return copyLength;
	}


	
}