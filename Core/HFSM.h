#pragma once

#include <Windows.h>
#include <list>
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
		State<EntityType>*	parent;
		State<EntityType>*	current;		
		std::list<State<EntityType>*> children;

		StateRelationInfo(UINT32 TparentId, 
			UINT32 TcurrentId,
			State<EntityType>*	Tparent,
			State<EntityType>*	Tcurrent)
			:parentStateID(TparentId), currentStateID(TcurrentId),
			parent(Tparent), current(Tcurrent)
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
		HFSM(EntityType* owner) :m_owner(owner), m_currentState(nullptr){};
		~HFSM(){};

	private:
		EntityType* m_owner;
		std::unordered_map<UINT32, StateRelationInfo<EntityType>> m_stateTree;	// < currentStateId, StateRelationInfo>
		State<EntityType>*	m_currentState;
		typedef std::list<Node<EntityType>> PATH;

	private:
		void makePath(PATH& path, State<EntityType>* currentState, State<EntityType>* targetState)
		{
			// ���� ���迡 ���� ������� ���¸� �Է��Ѵ�. 
			// EXIT, ENTER, TARGET ���� �� ������ Action�� �����Ѵ�. 

			if ( nullptr == currentState )
			{
				return;
			}	

			auto itCurrent = m_stateTree.find(currentState->GetID());
			
			State<EntityType>* matchState = getMatchStateFromMeToChildren(path, currentState, targetState);
			if ( nullptr == matchState )
			{
				path.emplace_back(Node<EntityType>::Action::EXIT, itCurrent->second.current);
				return makePath(path, itCurrent->second.parent, targetState);
			}
			else
			{
				PATH temp;

				if ( matchState->GetID() == currentState->GetID() )
				{
					path.emplace_back(Node<EntityType>::Action::TARGET, itCurrent->second.current);
				}
				else
				{
					// Enter�ϴ� ù State�� temp�� �� �ִ´�.
					// �ֳ��ϸ� target�̸� ���� ���¸� ��� ù State�� ������ �ֱ� �����̴�.
					temp.emplace_back(Node<EntityType>::Action::TARGET, matchState);

					do
					{	
						auto itMatch = m_stateTree.find(matchState->GetID());
						matchState = itMatch->second.parent;

						if ( matchState->GetID() == currentState->GetID() )
						{
							break;
						}

						temp.emplace_back(Node<EntityType>::Action::ENTER, matchState);												

					} while ( true );				
				}				
				
				path.insert(path.end(), temp.rbegin(), temp.rend());
			}		
		}

		State<EntityType>* getMatchStateFromMeToRoot( State<EntityType>* checkState, State<EntityType>* currentState )
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

		State<EntityType>* getMatchStateFromMeToChildren( PATH& path, State<EntityType>* currentState, State<EntityType>* targetState )
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

				State<EntityType>* matchState = getMatchStateFromMeToChildren( path, itChild, targetState );
				if ( nullptr != matchState )
				{
					return matchState;
				}
			}

			return nullptr;
		};

	public:
		STATE_ID GetCurrentState() { return static_cast<STATE_ID>(m_currentState->GetID()); }
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

		void DealWithMessage(const MessageInfo& messageInfo) const
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

		BOOL RegisterState(State<EntityType>* parent, State<EntityType>* child)
		{
			if ( nullptr == child )
			{
				return false;
			}

			if ( nullptr == parent )
			{
				m_stateTree.emplace(std::piecewise_construct, 
					std::make_tuple(child->GetID()),
					std::make_tuple(STATE_ID::STATE_NONE, child->GetID(), parent, child));

				return true;
			}
			
			// ���� ���� ���°� �ִ��� Ȯ��
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
				std::make_tuple(STATE_ID::STATE_NONE, child->GetID(), parent, child));

			itParent->second.children.push_back(child);
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
				case Node<EntityType>::Action::ENTER:
					itNode.state->OnEnter(m_owner);
					break;

				case Node<EntityType>::Action::TARGET:
					m_currentState = itNode.state;		
					m_currentState->OnEnter(m_owner);
					break;

				case Node<EntityType>::Action::EXIT:
					itNode.state->OnExit(m_owner);
					break;

				default:
					return false;
				}
			}

			return true;
		};

		void SetCurrentState(State<EntityType>* state)
		{
			m_currentState = state;
		}		
	};

} // namespace SSL

