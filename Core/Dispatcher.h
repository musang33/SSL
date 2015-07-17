#pragma once

#include <map>
#include <functional>

#include "CommonData.h"
#include "Event.h"

namespace SSL
{
	template< typename T >
	class Dispatcher
	{
	public:
		Dispatcher( T* t )
			: funcInstance(t)
		{}
		~Dispatcher() {}

	public:
		typedef void ( T::*func )( EVENTPtr& e );

	public:
		void RegisterFunction( UINT32 eventIndex, func f );
		bool Dispatch( EVENTPtr& e );

	private:
		T* funcInstance;
		std::map<UINT32, func> mapFunction;
	};

	template< typename T >
	void Dispatcher<T>::RegisterFunction( UINT32 eventIndex, func f )
	{
		if ( mapFunction.find( eventIndex ) != mapFunction.end() )
		{
			return;
		}

		mapFunction.insert( std::map<UINT32, func>::value_type( eventIndex, f ) );
	}

	template< typename T >
	bool Dispatcher<T>::Dispatch( EVENTPtr& e )
	{
		auto it = mapFunction.find( e->eventIndex );
		if ( it == mapFunction.end() )
		{
			return false;
		}

		func f = it->second;
		(funcInstance->*f)( e );

		return true;
	}
}