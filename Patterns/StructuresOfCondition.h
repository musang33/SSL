#pragma once
#include <Windows.h>
#include <assert.h>
#include <memory>
#include <vector>


struct ConditionType
{
	enum Enum
	{
		TYPE_NONE = 0,
		GET_MONEY = TYPE_NONE,
		GET_EXP,
		FIRST_DIED,
		LEVEL_UP,
		TYPE_END
	};

	static bool IsValidCondition( INT32 condition )
	{
		return (condition < TYPE_NONE || condition >= TYPE_END);
	}
};


struct Condition
{
public:
	~Condition( )
	{
	};

public:
	INT32 GetConditionType( ) const
	{
		return m_type;
	}

private:
	INT32 m_type = ConditionType::TYPE_NONE;

protected:
	explicit Condition( INT32 type )
		: m_type( type )
	{		
		assert( ConditionType::IsValidCondition( m_type ) );
	}
};

struct ConditionGetMoney : public Condition
{
	INT32	money = 0;

	ConditionGetMoney( )
		: Condition( ConditionType::GET_MONEY )
	{
	}
};

struct ConditionLevelUp : public Condition
{
	BYTE	level = 0;

	ConditionLevelUp( )
		: Condition( ConditionType::LEVEL_UP )
	{
	}
};


using ConditionSmartPointer = std::shared_ptr<Condition>;
using ConditionList = std::vector<ConditionSmartPointer>;


struct ConditionUnit
{
	UINT32						unitIndex;
	ConditionList				conditions;
	ConditionSmartPointer		countCondition;
};

using ConditionUnitList = std::vector<ConditionUnit>;
