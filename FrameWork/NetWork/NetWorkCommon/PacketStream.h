#pragma once

#include <Common/CommonData.h>

namespace SSL
{
	class PacketStream
	{
		enum
		{
			EN_DEFAULT_BUFFER_SIZE = 1024,
			EN_DEFAULT_INCREASE_SIZE = 1024
		};

	public:
		PacketStream();
		PacketStream( UINT32 bufferSize );
		~PacketStream();

	public:
		void ResetIncreaseSize( UINT32 increaseSize );
		void ResetBufferSize( UINT32 bufferSize );
		void Reset();
		UINT32 Write( const BYTE* data, UINT32 size );
		UINT32 Read( BYTE* data, UINT32 size );
		
		// inline function
	public:
		CHAR* GetDataPtr()	{ return reinterpret_cast<CHAR*>(m_data);  }
		UINT32 Size() { return static_cast< UINT32 >( m_pos - m_data ); }
		BYTE* GetPtr() { return m_data; }
		UINT32 Write( const CHAR* data, UINT32 size ) { return Write( (BYTE*)( data ), size ); };
		void Pop( UINT32 size ) { m_pos -= size; }
		bool IsValid( )
		{
			return m_isValid;
		}
	public:
		template < class T >
		UINT32 Write( const T& v )
		{
			return Write( ( BYTE* ) &v, sizeof( T ) );
		}

		template < class T >
		UINT32 Read( T& v )
		{
			return Read( ( BYTE* ) &v, sizeof( T ) );
		}

	private:
		BYTE*		m_data;			// buffer
		BYTE*		m_pos;			// current m_data position
		BYTE*		m_end;			// m_data's end position
		UINT32		m_dataLength;	
		UINT32		m_increaseSize;	

		bool		m_isValid;
	};
}