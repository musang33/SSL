#pragma once

#include <vector>
#include <assert.h>
#include "CommonData.h"

namespace SSL
{
	class BaseEntity;

	class Behavior
	{
	public:
		Behavior();

		BEHAVIOR_STATE Tick( BaseEntity* baseEntity )
		{
			if ( BEHAVIOR_STATE::BH_INVALID == m_state )
			{
				onInitialize( baseEntity );
			}

			m_state = update( baseEntity );

			if ( BEHAVIOR_STATE::BH_RUNNING != m_state )
			{
				onTerminate( baseEntity, m_state );
			}

			return m_state;
		}
	protected:
		virtual BEHAVIOR_STATE update( BaseEntity* baseEntity ) = 0;
		virtual void onInitialize( BaseEntity* baseEntity ) {}
		virtual void onTerminate( BaseEntity* baseEntity, BEHAVIOR_STATE ) {}

	private:
		BEHAVIOR_STATE	m_state;
	};

	class CompositBehavior : public Behavior
	{
	protected:
		typedef std::vector<Behavior*> Behaviors;
		Behaviors		m_children;
	};

	class Sequence : public CompositBehavior
	{
	protected:
		virtual void onInitialize( BaseEntity* baseEntity ) override
		{
			m_currentChild = m_children.begin();
		}

		virtual BEHAVIOR_STATE update( BaseEntity* baseEntity ) override
		{
			while ( true )
			{
				BEHAVIOR_STATE state = ( *m_currentChild )->Tick( baseEntity );

				if ( BEHAVIOR_STATE::BH_SUCCESS != state )
				{
					return state;
				}

				if ( ++m_currentChild == m_children.end() )
				{
					return BEHAVIOR_STATE::BH_SUCCESS;
				}
			}
			
			assert( false );
			return BEHAVIOR_STATE::BH_INVALID;
		}

		Behaviors::iterator m_currentChild;
	};

	class Select : public CompositBehavior
	{
	protected:
		virtual void onInitialize( BaseEntity* baseEntity ) override
		{
			m_currentChild = m_children.begin();
		}

		virtual BEHAVIOR_STATE update( BaseEntity* baseEntity ) override
		{
			while ( true )
			{
				BEHAVIOR_STATE state = ( *m_currentChild )->Tick( baseEntity );

				if ( BEHAVIOR_STATE::BH_FAILURE == state )
				{
					return state;
				}

				if ( ++m_currentChild == m_children.end() )
				{
					return BEHAVIOR_STATE::BH_FAILURE;
				}
			}

			assert( false );
			return BEHAVIOR_STATE::BH_INVALID;
		}

		Behaviors::iterator m_currentChild;
	};

	class ActionNPCAttack : public Behavior
	{
	protected:
		virtual BEHAVIOR_STATE update( BaseEntity* baseEntity ) override
		{

		}
	};

}

