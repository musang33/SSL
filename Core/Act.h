#pragma once

namespace SSL
{
	class TcpSocket;
	class Actor;
	struct Act : public OVERLAPPED		
	{
		Act( Actor* actor )
			:m_actor( actor )
			, m_tcpSocket( nullptr )
		{
			clear();
		}

		void clear()
		{
			hEvent = 0;
			Internal = 0;
			InternalHigh = 0;
			Offset = 0;
			OffsetHigh = 0;			
		}

		Actor*		m_actor;
		TcpSocket*	m_tcpSocket;
	};

	struct SendAct : public Act
	{
		SendAct()
			: Act( nullptr )
			, bytesRequested( 0 )
			, sentTransferred( 0 )
		{
			clear();

			wsabuf.buf = nullptr;
			wsabuf.len = 0;
		}

		~SendAct() {};

		PacketStream	buffer;		// 전송용 버퍼
		WSABUF			wsabuf;

		DWORD			bytesRequested;
		DWORD			sentTransferred;
	};
}