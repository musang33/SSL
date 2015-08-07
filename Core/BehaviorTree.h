/******************************************************************************
* This file is part of the Behavior Tree Starter Kit.
*
* Copyright (c) 2012, AiGameDev.com.
*
* Credits:         Alex J. Champandard
*****************************************************************************/
#pragma once

#include <vector>
#include <iostream>

#include "ActionBT.h"

namespace SSL
{
	// ============================================================================

	struct Perception
	{
		bool isDead;
		bool isEngageState;
		bool visibleHostileEnemies;
		bool damageTaken;

		Perception()
			: isDead( false ),
			isEngageState( false ),
			visibleHostileEnemies( false ),
			damageTaken( false )
		{}
	};

	/*class Agent
	{
	public:
		virtual EN_BEHAVIOR_STATE FindEnemy(){ return BH_SUCCESS; }
		virtual EN_BEHAVIOR_STATE AttackEnemy(){ return BH_SUCCESS; }
		virtual EN_BEHAVIOR_STATE Move() = 0;
	};

	class Monster : public Agent
	{
	public:
		Perception preception;

	public:
		virtual EN_BEHAVIOR_STATE FindEnemy()
		{
			int randValue = rand() % 2;

			if ( 0 == randValue )
			{
				std::cout << "[Monster][FindEnemy][BH_SUCCESS]" << std::endl;
				return BH_SUCCESS;
			}
			else
			{
				std::cout << "[Monster][FindEnemy][BH_FAILURE]" << std::endl;
				return BH_FAILURE;
			}
		}

		virtual EN_BEHAVIOR_STATE Move()
		{
			int randValue = rand() % 3;

			if ( 0 == randValue )
			{
				std::cout << "[Monster][Move][BH_SUCCESS]" << std::endl;
				return BH_SUCCESS;
			}
			else if ( 1 == randValue )
			{
				std::cout << "[Monster][Move][BH_RUNNING]" << std::endl;
				return BH_RUNNING;
			}
			else
			{
				std::cout << "[Monster][Move][BH_FAILURE]" << std::endl;
				return BH_FAILURE;
			}
		}

		virtual EN_BEHAVIOR_STATE AttackEnemy()
		{
			int randValue = rand() % 3;

			if ( 0 == randValue )
			{
				std::cout << "[Monster][AttackEnemy][BH_SUCCESS]" << std::endl;
				return BH_SUCCESS;
			}
			else if ( 1 == randValue )
			{
				std::cout << "[Monster][AttackEnemy][BH_RUNNING]" << std::endl;
				return BH_RUNNING;
			}
			else
			{
				std::cout << "[Monster][AttackEnemy][BH_FAILURE]" << std::endl;
				return BH_FAILURE;
			}
		}
	};*/
	
	class Behavior
		/**
		* Base class for actions, conditions and composites.
		*/
	{
	public:
		virtual EN_BEHAVIOR_STATE update( Entity* agent ) = 0;

		virtual void onInitialize()			{}
		virtual void onTerminate( EN_BEHAVIOR_STATE )	{}

		Behavior()
			: m_eStatus( BH_INVALID )
		{
		}

		virtual ~Behavior()
		{
		}

		EN_BEHAVIOR_STATE tick( Entity* agent )
		{
			if ( m_eStatus == BH_INVALID )
			{
				onInitialize();
			}

			m_eStatus = update( agent );
			EN_BEHAVIOR_STATE tempStatus = m_eStatus;

			if ( m_eStatus != BH_RUNNING )
			{
				onTerminate( m_eStatus );
			}
			return tempStatus;
		}

		void reset()
		{
			m_eStatus = BH_INVALID;
		}

		void abort()
		{
			onTerminate( BH_ABORTED );
			m_eStatus = BH_ABORTED;
		}

		bool isTerminated() const
		{
			return m_eStatus == BH_SUCCESS || m_eStatus == BH_FAILURE;
		}

		bool isRunning() const
		{
			return m_eStatus == BH_RUNNING;
		}

		EN_BEHAVIOR_STATE getStatus() const
		{
			return m_eStatus;
		}

	private:
		EN_BEHAVIOR_STATE m_eStatus;
	};

	// ============================================================================
	
	class Decorator : public Behavior
	{
	protected:
		Behavior* m_pChild;

	public:
		Decorator( Behavior* child ) : m_pChild( child ) {}
	};

	
	class Repeat : public Decorator
	{
	public:
		Repeat( Behavior* child )
			: Decorator( child )
		{
		}

		void setCount( int count )
		{
			m_iLimit = count;
		}

		void onInitialize()
		{
			m_iCounter = 0;
		}

		EN_BEHAVIOR_STATE update( Entity* agent )
		{
			for ( ;; )
			{
				m_pChild->tick( agent );
				if ( m_pChild->getStatus() == BH_RUNNING ) break;
				if ( m_pChild->getStatus() == BH_FAILURE ) return BH_FAILURE;
				if ( ++m_iCounter == m_iLimit ) return BH_SUCCESS;
				m_pChild->reset();
			}
			return BH_INVALID;
		}

	protected:
		int m_iLimit;
		int m_iCounter;
	};

	// ============================================================================
	
	class Composite : public Behavior
	{
	public:
		void addChild( Behavior* child ) { m_Children.push_back( child ); }
		void removeChild( Behavior* );
		void clearChildren();
	protected:
		typedef std::vector<Behavior*> Behaviors;
		
		Behaviors m_Children;
	};

	
	class Sequence : public Composite
	{
	public:
		virtual ~Sequence()
		{
		}

		virtual void onInitialize()
		{
			m_CurrentChild = m_Children.begin();
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			// Keep going until a child behavior says it's running.
			for ( ;; )
			{
				EN_BEHAVIOR_STATE s = ( *m_CurrentChild )->tick( agent );

				// If the child fails, or keeps running, do the same.
				if ( s != BH_SUCCESS )
				{
					return s;
				}

				// Hit the end of the array, job done!
				if ( ++m_CurrentChild == m_Children.end() )
				{
					return BH_SUCCESS;
				}
			}
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			reset();
		}

		Behaviors::iterator m_CurrentChild;
		
	};

	// ============================================================================
	
	class Selector : public Composite
	{
	public:
		virtual ~Selector()
		{
		}

		virtual void onInitialize()
		{
			m_Current = m_Children.begin();
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			// Keep going until a child behavior says its running.
			for ( ;; )
			{
				EN_BEHAVIOR_STATE s = ( *m_Current )->tick( agent );

				// If the child succeeds, or keeps running, do the same.
				if ( s != BH_FAILURE )
				{
					return s;
				}

				// Hit the end of the array, it didn't end well...
				if ( ++m_Current == m_Children.end() )
				{
					return BH_FAILURE;
				}
			}
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			reset();
		}

		Behaviors::iterator m_Current;
	};

	
	class Parallel : public Composite
	{
	public:
		enum Policy
		{
			RequireOne,
			RequireAll,
		};

		Parallel( Policy forSuccess, Policy forFailure )
			: m_eSuccessPolicy( forSuccess )
			, m_eFailurePolicy( forFailure )
		{
		}

		virtual ~Parallel() {}

	public:
		Policy m_eSuccessPolicy;
		Policy m_eFailurePolicy;

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			size_t iSuccessCount = 0, iFailureCount = 0;

			for ( Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it )
			{
				Behavior& b = **it;
				if ( !b.isTerminated() )
				{
					b.tick( agent );
				}

				if ( b.getStatus() == BH_SUCCESS )
				{
					++iSuccessCount;
					if ( m_eSuccessPolicy == RequireOne )
					{
						return BH_SUCCESS;
					}
				}

				if ( b.getStatus() == BH_FAILURE )
				{
					++iFailureCount;
					if ( m_eFailurePolicy == RequireOne )
					{
						return BH_FAILURE;
					}
				}
			}

			if ( m_eFailurePolicy == RequireAll  &&  iFailureCount == m_Children.size() )
			{
				return BH_FAILURE;
			}

			if ( m_eSuccessPolicy == RequireAll  &&  iSuccessCount == m_Children.size() )
			{
				return BH_SUCCESS;
			}

			return BH_RUNNING;
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			for ( Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it )
			{
				Behavior& b = **it;
				if ( b.isRunning() )
				{
					b.abort();
				}
			}
		}
	};

	
	class Monitor : public Parallel
	{
	public:
		Monitor()
			: Parallel( Parallel::RequireOne, Parallel::RequireOne )
		{
		}

		void addCondition( Behavior* condition )
		{
			m_Children.insert( m_Children.begin(), condition );
		}

		void addAction( Behavior* action )
		{
			m_Children.push_back( action );
		}
	};


	// ============================================================================
	
	class ActiveSelector : public Selector
	{
	public:

		virtual void onInitialize()
		{
			m_Current = m_Children.end();
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			Behaviors::iterator previous = m_Current;

			Selector::onInitialize();
			EN_BEHAVIOR_STATE result = Selector::update( agent );

			if ( previous != m_Children.end() && m_Current != previous )
			{
				( *previous )->onTerminate( BH_ABORTED );
			}
			return result;
		}
	};

	// ============================================================================
	
	class ActionFreeWalk : public Behavior
	{
	public:
		virtual ~ActionFreeWalk() {}

		virtual void onInitialize()
		{
			std::cout << "[ActionFreeWalk][onInitialize]" << std::endl;
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			ActionBT* ab = GetEntityAction( agent );
			return ab->Patrol();
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			reset();
		}
	};

	
	class ActionFight : public Behavior
	{
	public:
		virtual ~ActionFight() {}

		virtual void onInitialize()
		{
			std::cout << "[ActionFight][onInitialize]" << std::endl;
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			ActionBT* ab = GetEntityAction( agent );
			return ab->AttackEnemy();
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			reset();
		}
	};

	
	class ConditionCheckHP : public Behavior
	{
	public:
		virtual ~ConditionCheckHP() {}

		virtual void onInitialize()
		{
			std::cout << "[ConditionCheckHP][onInitialize]" << std::endl;
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			ActionBT* ab = GetEntityAction( agent );
			return ab->CheckHP();
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			reset();
		}
	};

	
	class ConditionFindEnemy : public Behavior
	{
	public:
		virtual ~ConditionFindEnemy() {}

		virtual void onInitialize()
		{
			std::cout << "[ConditionFindEnemy][onInitialize]" << std::endl;
		}

		virtual EN_BEHAVIOR_STATE update( Entity* agent )
		{
			ActionBT* ab = GetEntityAction( agent );
			return ab->FindEnemy();
		}

		virtual void onTerminate( EN_BEHAVIOR_STATE )
		{
			reset();
		}
	};

} // namespace SSL
