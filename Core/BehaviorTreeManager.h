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

	public:
		BehaviorTreeManager( EntityType* owner )
			:root(nullptr), m_owner(owner)
		{}

		~BehaviorTreeManager()
		{
			if ( nullptr != root )
			{
				delete root;
			}
		}

		BEHAVIOR_STATE Update()
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

			Behavior<EntityType>* conditionFindEnemy = new ConditionFindEnemy<EntityType>;
			Behavior<EntityType>* actionFight = new ActionFight<EntityType>;
						
			freeWalk->addChild( conditionFindEnemy );
			freeWalk->addChild( actionFight );

			Behavior<EntityType>* actionFreeWalk = new ActionFreeWalk<EntityType>;			

			root->addChild( freeWalk );
			root->addChild( actionFreeWalk );
		}
	};
}