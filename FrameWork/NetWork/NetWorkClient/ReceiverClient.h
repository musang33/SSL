#pragma once
#include <Actor.h>

namespace SSL
{

	class ReceiverClient :
		public Actor
	{
	public:
		ReceiverClient( Proactor* proactor );
		~ReceiverClient();

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};

}