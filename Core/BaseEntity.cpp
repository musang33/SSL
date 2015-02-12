#include "BaseEntity.h"

namespace SSL
{
	int BaseEntity::m_iNextValidID = 0;

	void BaseEntity::SetID(int val)
	{
		if (val > MAX_VALID_ENTITY_ID || val <= m_iNextValidID)
		{
			std::cout << "[ERROR][Entity][SetID][$$:Not valid EntityID]" << std::endl;
			exit(1);
		}

		m_iNextValidID = val;
		m_ID = val;
	}

}