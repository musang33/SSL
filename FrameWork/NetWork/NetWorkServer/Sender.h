#pragma once
#include "Actor.h"

namespace SSL
{
	class Sender : public Actor
	{
	public:
		Sender( Proactor* proactor );
		~Sender();

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};
}