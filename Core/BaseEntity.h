#pragma once

#include "CommonData.h"

namespace SSL
{	
	class BaseEntity
	{
	private:
		int m_ID;
		Location currentLocation;

		static int m_iNextValidID;

		void SetID(int val);			

	public:
		BaseEntity(int id)
		{
			SetID(id);
		}

		virtual ~BaseEntity() {};

		virtual void Update() = 0;

		virtual void DealWithMessage(const MessageInfo& messageInfo) const = 0;

		int ID() const { return m_ID; };

		Location GetCurLocation() { return currentLocation; };
		void SetCurLocation( int _x, int _y ) 
		{ 
			currentLocation.x = _x; 
			currentLocation.y = _y;
		}
	};
}
