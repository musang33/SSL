#pragma once

#include <map>
#include <functional>

#include <Event/Event.h>

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
		typedef void ( T::*func )( EventPtr& e );

	public:
		void RegisterFunction( UINT32 eventIndex, func f );
		bool Dispatch( EventPtr& e );

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
	bool Dispatcher<T>::Dispatch( EventPtr& e )
	{
		auto it = mapFunction.find( e->GetEvent() );
		if ( it == mapFunction.end() )
		{
			return false;
		}

		func f = it->second;
		(funcInstance->*f)( e );

		return true;
	}
}