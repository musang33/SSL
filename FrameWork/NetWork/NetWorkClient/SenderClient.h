#pragma once
#include <Network/NetworkCommon/Actor.h>

namespace SSL
{
	class SenderClient : public Actor
	{
	public:
		SenderClient( Proactor* proactor );
		~SenderClient();

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};
}