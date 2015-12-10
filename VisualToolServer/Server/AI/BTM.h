#pragma once

#include <AI/BehaviorTree.h>

#include <Action/ActionState.h>

namespace SSL
{

	class Entity;
	class BTM
	{
	private:
		Composite* root;
		Entity* m_owner;		

	public:
		BTM( Entity* owner )
			:root( nullptr ), m_owner( owner )
		{}

		~BTM()
		{
			if ( nullptr != root )
			{
				delete root;
			}
		}	

		EN_BEHAVIOR_STATE Update()
		{
			if ( nullptr == root )
			{
				return BH_INVALID;
			}

			root->onInitialize();
			return root->update( m_owner );
		}
	
		void SetBehaviorTreeNpc()
		{
			root = new Selector;
			Sequence* freeWalk = new Sequence;
			Behavior* conditionCheckHP = new ConditionCheckHP;
			Behavior* conditionFindEnemy = new ConditionFindEnemy;
			Behavior* actionFight = new ActionFight;
						
			freeWalk->addChild( conditionCheckHP );
			freeWalk->addChild( conditionFindEnemy );
			freeWalk->addChild( actionFight );

			Behavior* actionFreeWalk = new ActionFreeWalk;			

			root->addChild( freeWalk );
			root->addChild( actionFreeWalk );
		}

		void SetBehaviorTreePlayer()
		{
			root = new Selector;
			
			Behavior* conditionCheckHP = new ConditionCheckHP;		

			root->addChild( conditionCheckHP );						
		}

		void DealWithMessage( const ST_MESSAGE_INFO& messageInfo ) const
		{

			/*if ( m_eventQ.empty() )
			{
				Dispathc(msg);
			}
			else
			{
				m_eventQ.push)back( msg );
			}*/

			switch ( messageInfo.messageType )
			{
				case EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP:
				{
					ActionState* as = GetEntityAction( m_owner );
					as->AddHP( reinterpret_cast< int >( messageInfo.extraInfo ) );
					break;
				}
				default:
					break;
			}
		}
	};
}