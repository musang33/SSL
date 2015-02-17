#pragma once

#include "State.h"

namespace SSL
{
	class HouseWife;

	class Cooking final : public State<HouseWife>, public Singleton < Cooking >
	{
	public:
		Cooking() = default;
		~Cooking(){};		

		virtual void Enter(HouseWife* houseWife);
		
		virtual void OnTick(HouseWife* houseWife);
		
		virtual void Exit(HouseWife* houseWife);		

		virtual void OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const;
	};
	
	class BathRoom final : public State<HouseWife>, public Singleton < BathRoom >
	{
	public:
		BathRoom() = default;
		~BathRoom(){};

		virtual void Enter(HouseWife* houseWife);

		virtual void OnTick(HouseWife* houseWife);

		virtual void Exit(HouseWife* houseWife);

		virtual void OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const;
	};

	class LivingRoom final : public State<HouseWife>, public Singleton < LivingRoom >
	{
	public:
		LivingRoom() = default;
		~LivingRoom(){};

		virtual void Enter(HouseWife* houseWife);

		virtual void OnTick(HouseWife* houseWife);

		virtual void Exit(HouseWife* houseWife);		

		virtual void OnMessage(HouseWife* houseWife, const MessageInfo& messageInfo) const;
	};

} // namespace SSL
