#pragma once

#include "State.h"

namespace SSL
{
	class HouseWife;

	class Cooking final : public State<HouseWife>, public Singleton < Cooking >
	{
	public:
		Cooking() :State<HouseWife>(ID){};
		~Cooking(){};		

		virtual void Enter(HouseWife* houseWife);
		
		virtual void OnTick(HouseWife* houseWife);
		
		virtual void Exit(HouseWife* houseWife);		

		virtual void OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_HOUSEWIFE_COOKING;
	};
	
	class BathRoom final : public State<HouseWife>, public Singleton < BathRoom >
	{
	public:
		BathRoom() :State<HouseWife>(ID){};
		~BathRoom(){};

		virtual void Enter(HouseWife* houseWife);

		virtual void OnTick(HouseWife* houseWife);

		virtual void Exit(HouseWife* houseWife);

		virtual void OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_HOUSEWIFE_BATHROOM;
	};

	class LivingRoom final : public State<HouseWife>, public Singleton < LivingRoom >
	{
	public:
		LivingRoom() :State<HouseWife>(ID){};
		~LivingRoom(){};

		virtual void Enter(HouseWife* houseWife);

		virtual void OnTick(HouseWife* houseWife);

		virtual void Exit(HouseWife* houseWife);		

		virtual void OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const;

	private:
		static const UINT32 ID = STATE_HOUSEWIFE_LIVINGROOM;
	};

} // namespace SSL
