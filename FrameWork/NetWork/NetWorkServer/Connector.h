#pragma once
#include <Network/NetworkCommon/Actor.h>

namespace SSL
{

	class Connector : public Actor
	{
	public:
		Connector( Proactor* proactor );
		~Connector();

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};
}