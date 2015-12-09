#pragma once
#include <Network/NetworkCommon/Actor.h>

namespace SSL
{

	class Receiver :
		public Actor
	{
	public:
		Receiver( Proactor* proactor );
		~Receiver();

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};

}