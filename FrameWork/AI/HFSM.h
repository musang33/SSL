#pragma once

#include <Windows.h>
#include <list>
#include <unordered_map>

namespace SSL
{	
	class State;

	class Entity;
	struct StateRelationInfo
	{		
		UINT32	parentStateID;
		UINT32	currentStateID;
		State*	parent;
		State*	current;		
		std::list<State*> children;

		StateRelationInfo(UINT32 TparentId, 
			UINT32 TcurrentId,
			State*	Tparent,
			State*	Tcurrent)
			:parentStateID(TparentId), currentStateID(TcurrentId),
			parent(Tparent), current(Tcurrent)
		{};
	};
		
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
		State*	state;
		Node() :action(NONE), state(nullptr) {}
		Node(Action Taction, State*	Tstate)
			: action(Taction), state(Tstate) {}
	};	

	class HFSM
	{
	public:
		HFSM(const Entity* owner) :m_owner(owner), m_currentState(nullptr)  {};
		~HFSM(){};

	private:
		const Entity* m_owner;
		std::unordered_map<UINT32, StateRelationInfo> m_stateTree;	// < currentStateId, StateRelationInfo>
		State*	m_currentState;
		typedef std::list<Node> PATH;

	private:
		void makePath(PATH& path, State* currentState, State* targetState)
		{
			// 계층 관계에 따라 순서대로 상태를 입력한다. 
			// EXIT, ENTER, TARGET 으로 각 상태의 Action을 설정한다. 

			if ( nullptr == currentState )
			{
				return;
			}	

			auto itCurrent = m_stateTree.find(currentState->GetID());
			
			State* matchState = getMatchStateFromMeToChildren(path, currentState, targetState);
			if ( nullptr == matchState )
			{
				path.emplace_back(Node::Action::EXIT, itCurrent->second.current);
				return makePath(path, itCurrent->second.parent, targetState);
			}
			else
			{
				PATH temp;

				if ( matchState->GetID() == currentState->GetID() )
				{
					path.emplace_back(Node::Action::TARGET, itCurrent->second.current);
				}
				else
				{
					// Enter하는 첫 State는 temp에 안 넣는다.
					// 왜냐하면 target이며 현재 상태면 모두 첫 State의 하위에 있기 때문이다.
					temp.emplace_back(Node::Action::TARGET, matchState);

					do
					{	
						auto itMatch = m_stateTree.find(matchState->GetID());
						matchState = itMatch->second.parent;

						if ( matchState->GetID() == currentState->GetID() )
						{
							break;
						}

						temp.emplace_back(Node::Action::ENTER, matchState);												

					} while ( true );				
				}				
				
				path.insert(path.end(), temp.rbegin(), temp.rend());
			}		
		}

		State* getMatchStateFromMeToRoot( State* checkState, State* currentState )
		{
			auto itCheck = m_stateTree.find( checkState->GetID() );
			auto itCurrent = m_stateTree.find( currentState->GetID() );

			while ( itCheck != m_stateTree.end() && itCheck->second.current != nullptr )
			{
				auto tempCurrent = itCurrent;

				while ( tempCurrent != m_stateTree.end() && tempCurrent->second.current != nullptr )
				{
					if ( tempCurrent->second.current->GetID() == itCheck->second.current->GetID() )
					{
						return tempCurrent->second.current;
					}

					if ( nullptr == tempCurrent->second.parent )
					{
						return nullptr;
					}

					tempCurrent = m_stateTree.find( tempCurrent->second.parent->GetID() );
				}

				if ( nullptr == itCheck->second.parent )
				{
					return nullptr;
				}
				itCheck = m_stateTree.find( itCheck->second.parent->GetID() );
			}

			return nullptr;
		};

		State* getMatchStateFromMeToChildren( PATH& path, State* currentState, State* targetState )
		{
			auto it = m_stateTree.find( currentState->GetID() );
			for ( auto & itChild : it->second.children )
			{
				for ( auto &itPath : path )
				{
					if ( itPath.state->GetID() == itChild->GetID() )
					{
						continue;
					}
				}

				if ( itChild->GetID() == targetState->GetID() )
				{
					return itChild;
				}

				State* matchState = getMatchStateFromMeToChildren( path, itChild, targetState );
				if ( nullptr != matchState )
				{
					return matchState;
				}
			}

			return nullptr;
		};

	public:
		EN_STATE_ID GetCurrentState() { return static_cast<EN_STATE_ID>(m_currentState->GetID()); }
		void Update()
		{
			auto itPrevious = m_stateTree.find(m_currentState->GetID());
			if ( itPrevious == m_stateTree.end() )
			{
				return;
			}

			UINT32 previousParentID = itPrevious->second.parent->GetID();
			m_currentState->OnTick(m_owner);	
			
			auto itCurrent = m_stateTree.find(m_currentState->GetID());
			if ( itCurrent == m_stateTree.end() )
			{
				return;
			}

			if ( previousParentID != itCurrent->second.parent->GetID() )
			{
				auto processState = getMatchStateFromMeToRoot(itPrevious->second.current, m_currentState);
				if ( processState == nullptr )
				{
					return;
				}

				itCurrent = m_stateTree.find(processState->GetID());
			}

			while ( itCurrent->second.parent )
			{
				itCurrent->second.parent->OnTick(m_owner);
				itCurrent = m_stateTree.find(itCurrent->second.parent->GetID());
			}
		}

		void DealWithMessage(const ST_MESSAGE_INFO& messageInfo) const
		{			
			if ( m_currentState )
			{
				auto itProcessState = m_stateTree.find(m_currentState->GetID());
				while ( itProcessState != m_stateTree.end() &&
					nullptr != itProcessState->second.current )
				{
					itProcessState->second.current->OnMessage(m_owner, messageInfo);

					if ( nullptr == itProcessState->second.parent )
					{
						return;
					}

					itProcessState = m_stateTree.find(itProcessState->second.parent->GetID());
				}				
			}
		}

		BOOL RegisterState(State* parent, State* child)
		{
			if ( nullptr == child )
			{
				return false;
			}

			if ( nullptr == parent )
			{
				m_stateTree.emplace(std::piecewise_construct, 
					std::make_tuple(child->GetID()),
					std::make_tuple(EN_STATE_ID::STATE_NONE, child->GetID(), parent, child));

				return true;
			}
			
			// 먼저 상위 상태가 있는지 확인
			auto itParent = m_stateTree.find(parent->GetID());
			if ( itParent == m_stateTree.end() )
			{
				return false;
			}

			auto itChild = m_stateTree.find(child->GetID());
			if ( itChild != m_stateTree.end() )
			{
				return false;
			}				

			for ( auto &itChildren : itParent->second.children )
			{
				if ( itChildren->GetID() == child->GetID() )
				{
					return false;
				}					
			}

			m_stateTree.emplace(std::piecewise_construct,
				std::make_tuple(child->GetID()),
				std::make_tuple(EN_STATE_ID::STATE_NONE, child->GetID(), parent, child));

			itParent->second.children.push_back(child);
			return true;		
		}

		bool IsHavingState(State* targetState)
		{
			auto it = m_stateTree.find(targetState->GetID());
			if ( it != m_stateTree.end() )
			{
				return true;
			}

			return false;
		};		

		bool ChangeState(State* targetState)
		{
			if ( m_stateTree.empty() )
			{
				return false;
			}

			if ( false == IsHavingState(targetState) )
			{
				return false;
			}

			if ( m_currentState->GetID() == targetState->GetID() )
			{
				return false;
			}

			PATH path;
			makePath(path, m_currentState, targetState);
			if ( path.empty() )
			{
				return false;
			}

			for ( auto& itNode : path )
			{
				switch ( itNode.action )
				{
				case Node::Action::ENTER:
					itNode.state->OnEnter(m_owner);
					break;

				case Node::Action::TARGET:
					m_currentState = itNode.state;		
					m_currentState->OnEnter(m_owner);
					break;

				case Node::Action::EXIT:
					itNode.state->OnExit(m_owner);
					break;

				default:
					return false;
				}
			}

			return true;
		};

		void SetCurrentState(State* state)
		{
			m_currentState = state;
		}		
	};

} // namespace SSL

