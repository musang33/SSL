#include "BehaviorTree.h"

namespace SSL
{

	template <typename EntityType>
	class BehaviorTreeManager
	{
	private:
		Behavior<EntityType>* root;
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
			root->onInitialize();
			return root->update( owner );
		}
	
		void SetBehaviorTree()
		{
			root = new Selector<EntityType>;
			Sequence<EntityType> freeWalk;

			ConditionFindEnemy<EntityType> conditionFindEnemy;
			ActionFight<EntityType> actionFight;

			freeWalk.addChild( &conditionFindEnemy );
			freeWalk.addChild( &actionFight );

			ActionFreeWalk<EntityType> actionFreeWalk;

			root.addChild( &freeWalk );
			root.addChild( &actionFreeWalk );
		}
	};

}