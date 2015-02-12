#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "CommonData.h"

namespace SSL
{	
	class BaseEntity
	{
	private:
		int m_ID;

		static int m_iNextValidID;

		void SetID(int val);

	public:
		BaseEntity(int id)
		{
			SetID(id);
		}

		virtual ~BaseEntity() {};

		virtual void Update() = 0;

		int ID() const { return m_ID; };
	};
}

#endif