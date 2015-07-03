#pragma once

#include "BehaviorTree.h"

namespace SSL
{

	template <typename EntityType>
	class BehaviorTreeManager
	{
	private:
		Composite<EntityType>* root;
		EntityType* m_owner;
		EN_STATE_ID m_currentState;

	public:
		BehaviorTreeManager( EntityType* owner )
			:root( nullptr ), m_owner( owner ), m_currentState( EN_STATE_ID::STATE_NONE )
		{}

		~BehaviorTreeManager()
		{
			if ( nullptr != root )
			{
				delete root;
			}
		}

		EN_STATE_ID GetCurrentState() { return m_currentState; }
		void SetCurrentStateID( EN_STATE_ID stateId ) { m_currentState = stateId; }

		EN_BEHAVIOR_STATE Update()
		{
			if ( nullptr == root )
			{
				return BH_INVALID;
			}

			root->onInitialize();
			return root->update( m_owner );
		}
	
		void SetBehaviorTree()
		{
			root = new Selector<EntityType>;
			Sequence<EntityType>* freeWalk = new Sequence<EntityType>;
			Behavior<EntityType>* conditionCheckHP = new ConditionCheckHP<EntityType>;
			Behavior<EntityType>* conditionFindEnemy = new ConditionFindEnemy<EntityType>;
			Behavior<EntityType>* actionFight = new ActionFight<EntityType>;
						
			freeWalk->addChild( conditionCheckHP );
			freeWalk->addChild( conditionFindEnemy );
			freeWalk->addChild( actionFight );

			Behavior<EntityType>* actionFreeWalk = new ActionFreeWalk<EntityType>;			

			root->addChild( freeWalk );
			root->addChild( actionFreeWalk );
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
					m_owner->AddHP( reinterpret_cast< int >( messageInfo.extraInfo ) );
					break;
				default:
					break;
			}
		}
	};
}