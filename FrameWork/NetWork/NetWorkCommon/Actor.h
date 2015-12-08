#pragma once

namespace SSL
{
	struct Act;
	class Proactor;
	class Actor
	{
	public:
		Actor(Proactor* proactor)
			: m_proactor( proactor )
		{};

		~Actor(){};

		virtual void Complete( Act* act, unsigned int transferedBytes ) = 0;
		virtual void Error( Act* act, unsigned int transferedBytes ) = 0;

	protected:
		Proactor* m_proactor;
	};
}

