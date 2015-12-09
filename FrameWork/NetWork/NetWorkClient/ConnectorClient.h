#pragma once
#include <Network/NetworkCommon/Actor.h>

namespace SSL
{

	class ConnectorClient : public Actor
	{
	public:
		ConnectorClient( Proactor* proactor );
		~ConnectorClient();

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};
}