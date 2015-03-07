#pragma once

#include <Windows.h>
#include <vector>
#include <unordered_map>

namespace SSL
{
	template <typename EntityType>
	class State;

	template <typename EntityType>
	struct StateRelationInfo
	{		
		UINT32	parentStateID;
		UINT32	currentStateID;
		State<EntityType>*	current;
		State<EntityType>*	parent;
		std::vector<State<EntityType>*> children;

		StateRelationInfo(UINT32 TparentId, 
			UINT32 TcurrentId,
			State<EntityType>*	Tcurrent,
			State<EntityType>*	Tparent)
			:parentStateID(Tid), current(Tcurrent), parent(Tparent)
		{};
	};

	template <typename EntityType>
	struct Node
	{
		enum Action
		{
			NONE, 
			ENTER,
			EXIT,
			TARGET
		};

		Action action;
		State<EntityType>*	state;
		Node() :action(NONE), state(nullptr) {}
		Node(Action Taction, State<EntityType>*	Tstate)
			: action(Taction), state(Tstate) {}
	};	

	template <typename EntityType>
	class HFSM
	{
	public:
		HFSM(EntityType* owner) :m_owner(owner){};
		~HFSM(){};

	private:
		EntityType* m_owner;
		std::unordered_map<UINT32, StateRelationInfo<EntityType>> m_stateTree;	// < currentStateId, StateRelationInfo>
		State<EntityType>*	m_currentState;
		typedef std::vector<Node<EntityType>> PATH;

	private:
		bool makePath(PATH& path, State<EntityType>* currentState, State<EntityType>* targetState)
		{
			// 현재 상태의 자식중에 target이나 target의 부모가 있는지 체크
			// 없으면 exit 후 상위로 올라간다
			// *>있으면 enter
			// 현재 상태의 자식중에 target이나 target의 부모가 있는지 체크
			// 없으면 exit 후 상위로 올라간다
			// *>있으면 enter

			if ( nullptr == currentState )
			{
				return false;
			}

			auto itCurrent = m_stateTree.find(currentState->GetID());
			
			State<EntityType>* matchState = GetMatchStateFromMetoChildren(itCurrent->second.parent, targetState);
			if ( nullptr == matchState )
			{
				path.emplace_back(Node::Action::EXIT, itCurrent->second.current);
				makePath(path, itCurrent->second.parent, targetState);
			}
			else
			{
				if ( matchState->GetID() == targetState->GetID() )
				{
					path.emplace_back(Node::Action::TARGET, itCurrent->second.current);
				}
				else
				{
					makePath(path, matchState, targetState);
					path.emplace_back(Node::Action::ENTER, itCurrent->second.current);
				}
			}

			return true;
		}

	public:
		BOOL RegisterState(State<EntityType>* parent, State<EntityType>* child)
		{
			if ( nullptr == child )
			{
				return false;
			}

			if ( nullptr == parent )
			{
				m_stateTree.emplace(child->GetId(),
					0, child->GetId(), parent, child);

				return true;
			}

			// 먼저 상위 상태가 있는지 확인
			auto itParent = m_stateTree.find(parent->GetID());
			if ( itParent != m_stateTree.end() )
			{
				for ( auto &itChildren : itParent->children )
				{
					if ( itChildren->GetID() == child->GetID() )
					{
						return false;
					}					
				}

				itParent->children.push_back(child);
				return true;
			}			

			return true;
		}

		bool IsHavingState(State<EntityType>* targetState)
		{
			auto it = m_stateTree.find(targetState->GetID());
			if ( it != m_stateTree.end() )
			{
				return true;
			}

			return false;
		};

		State<EntityType>* GetMatchStateFromMetoChildren(State<EntityType>* parentState, State<EntityType>* childState)
		{
			auto it = m_stateTree.find(targetState->GetID());
			for ( auto & childIt : it->second.children )
			{
				if ( childIt->GetId() == childState->GetID() )
				{
					return true;
				}

				if ( IsHavingStateInChildren(childIt, childState) )
				{
					return true;
				}
			}		

			return false;
		};

		bool ChangeState(State<EntityType>* targetState)
		{
			if ( m_stateTree.empty() )
			{
				return false;
			}

			if ( false == IsHavingState(targetState) )
			{
				return false;
			}

			if ( currentState->GetID() == targetState->GetID() )
			{
				return false;
			}

			PATH path;
			if ( false == makePath->(path, m_currentState, targetState) )
			{
				return false;
			}

			for ( auto& itNode : path )
			{
				switch ( itNode.action )
				{
				case Node::Action::ENTER:
					itNode.state->Exit(m_owner);
					break;

				case Node::Action::TARGET:
					m_currentState = itNode.state;
					itNode.state->Exit(m_owner);
					break;

				case Node::Action::EXIT:
					itNode.state->Exit(m_owner);
					break;

				default:
					return false;
				}
			}

			return true;
		};
	};

} // namespace SSL

