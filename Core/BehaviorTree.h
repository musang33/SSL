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
		virtual BEHAVIOR_STATE FindEnemy(){ return BH_SUCCESS; }
		virtual BEHAVIOR_STATE AttackEnemy(){ return BH_SUCCESS; }
		virtual BEHAVIOR_STATE Move() = 0;
	};

	class Monster : public Agent
	{
	public:
		Perception preception;

	public:
		virtual BEHAVIOR_STATE FindEnemy()
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

		virtual BEHAVIOR_STATE Move()
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

		virtual BEHAVIOR_STATE AttackEnemy()
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
	template <typename EntityType>
	class Behavior
		/**
		* Base class for actions, conditions and composites.
		*/
	{
	public:
		virtual BEHAVIOR_STATE update( EntityType* agent ) = 0;

		virtual void onInitialize()			{}
		virtual void onTerminate( BEHAVIOR_STATE )	{}

		Behavior()
			: m_eStatus( BH_INVALID )
		{
		}

		virtual ~Behavior()
		{
		}

		BEHAVIOR_STATE tick( EntityType* agent )
		{
			if ( m_eStatus == BH_INVALID )
			{
				onInitialize();
			}

			m_eStatus = update( agent );
			BEHAVIOR_STATE tempStatus = m_eStatus;

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

		BEHAVIOR_STATE getStatus() const
		{
			return m_eStatus;
		}

	private:
		BEHAVIOR_STATE m_eStatus;
	};

	// ============================================================================
	template <typename EntityType>
	class Decorator : public Behavior<EntityType>
	{
	protected:
		Behavior<EntityType>* m_pChild;

	public:
		Decorator( Behavior<EntityType>* child ) : m_pChild( child ) {}
	};

	template <typename EntityType>
	class Repeat : public Decorator<EntityType>
	{
	public:
		Repeat( Behavior<EntityType>* child )
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

		BEHAVIOR_STATE update( EntityType* agent )
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
	template <typename EntityType>
	class Composite : public Behavior<EntityType>
	{
	public:
		void addChild( Behavior<EntityType>* child ) { m_Children.push_back( child ); }
		void removeChild( Behavior<EntityType>* );
		void clearChildren();
	protected:
		typedef std::vector<Behavior<EntityType>*> Behaviors;
		
		Behaviors m_Children;
	};

	template <typename EntityType>
	class Sequence : public Composite<EntityType>
	{
	public:
		virtual ~Sequence()
		{
		}

		virtual void onInitialize()
		{
			m_CurrentChild = m_Children.begin();
		}

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			// Keep going until a child behavior says it's running.
			for ( ;; )
			{
				BEHAVIOR_STATE s = ( *m_CurrentChild )->tick( agent );

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

		virtual void onTerminate( BEHAVIOR_STATE )
		{
			reset();
		}

		typename Behaviors::iterator m_CurrentChild;
		
	};

	// ============================================================================
	template <typename EntityType>
	class Selector : public Composite<EntityType>
	{
	public:
		virtual ~Selector()
		{
		}

		virtual void onInitialize()
		{
			m_Current = m_Children.begin();
		}

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			// Keep going until a child behavior says its running.
			for ( ;; )
			{
				BEHAVIOR_STATE s = ( *m_Current )->tick( agent );

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

		virtual void onTerminate( BEHAVIOR_STATE )
		{
			reset();
		}

		typename Behaviors::iterator m_Current;
	};

	template <typename EntityType>
	class Parallel : public Composite<EntityType>
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

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			size_t iSuccessCount = 0, iFailureCount = 0;

			for ( Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it )
			{
				Behavior<EntityType>& b = **it;
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

		virtual void onTerminate( BEHAVIOR_STATE )
		{
			for ( Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it )
			{
				Behavior<EntityType>& b = **it;
				if ( b.isRunning() )
				{
					b.abort();
				}
			}
		}
	};

	template <typename EntityType>
	class Monitor : public Parallel<EntityType>
	{
	public:
		Monitor()
			: Parallel( Parallel::RequireOne, Parallel::RequireOne )
		{
		}

		void addCondition( Behavior<EntityType>* condition )
		{
			m_Children.insert( m_Children.begin(), condition );
		}

		void addAction( Behavior<EntityType>* action )
		{
			m_Children.push_back( action );
		}
	};


	// ============================================================================
	template <typename EntityType>
	class ActiveSelector : public Selector<EntityType>
	{
	public:

		virtual void onInitialize()
		{
			m_Current = m_Children.end();
		}

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			Behaviors::iterator previous = m_Current;

			Selector::onInitialize();
			BEHAVIOR_STATE result = Selector::update( agent );

			if ( previous != m_Children.end() && m_Current != previous )
			{
				( *previous )->onTerminate( BH_ABORTED );
			}
			return result;
		}
	};

	// ============================================================================
	template <typename EntityType>
	class ActionFreeWalk : public Behavior<EntityType>
	{
	public:
		virtual ~ActionFreeWalk() {}

		virtual void onInitialize()
		{
			std::cout << "[ActionFreeWalk][onInitialize]" << std::endl;
		}

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			return agent->Move();
		}

		virtual void onTerminate( BEHAVIOR_STATE )
		{
			reset();
		}
	};

	template <typename EntityType>
	class ActionFight : public Behavior<EntityType>
	{
	public:
		virtual ~ActionFight() {}

		virtual void onInitialize()
		{
			std::cout << "[ActionFight][onInitialize]" << std::endl;
		}

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			return agent->AttackEnemy();
		}

		virtual void onTerminate( BEHAVIOR_STATE )
		{
			reset();
		}
	};

	template <typename EntityType>
	class ConditionFindEnemy : public Behavior<EntityType>
	{
	public:
		virtual ~ConditionFindEnemy() {}

		virtual void onInitialize()
		{
			std::cout << "[ConditionFindEnemy][onInitialize]" << std::endl;
		}

		virtual BEHAVIOR_STATE update( EntityType* agent )
		{
			return agent->FindEnemy();
		}

		virtual void onTerminate( BEHAVIOR_STATE )
		{
			reset();
		}
	};

} // namespace SSL
