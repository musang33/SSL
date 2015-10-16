#pragma once

#include "Actor.h"

namespace SSL
{
	class Acceptor : public Actor
	{
	public:
		Acceptor( Proactor* proactor );
		~Acceptor( );

	public:
		void Complete( Act* act, unsigned int transferedBytes ) override;
		void Error( Act* act, unsigned int transferedBytes ) override;
	};
}