#include <mutex>

namespace SSL
{
	template< typename T >
	struct PoolAlloc
	{
		typedef T* pointer;
		typedef T element_type;

		static pointer allocate( size_t n )
		{
			return new ( std::nothrow ) T[n];
		}

		static void deallocate(pointer p)
		{
			delete[] p;
		}
	};

	template < typename T >
	class MemoryPool
	{
	public:
		explicit MemoryPool( size_t arraySize, size_t initialCapacity = 0 );
		~MemoryPool();

		T* Acquire();
		void Release( T* p );
		void Reserve( size_t n );
		void Clear();
		size_t GetArraySize() const { return m_arraySize; };
		
	private:
		typedef PoolAlloc<T> AllocType;
		typedef typename AllocType::pointer pointer;

		struct Node
		{
			pointer block;
			Node* next;
		};

		struct Band
		{
			Node* nodes;
			pointer begin;
			pointer end;
		};

		static const size_t MAX_BANDS = sizeof( size_t ) * 8;

		bool stretch();

	private:
		Node* m_free;
		Band m_bands[MAX_BANDS];
		size_t m_numBands;
		size_t m_lastBandIndex;

		size_t m_arraySize;
		size_t m_capacity;

		std::mutex g_i_mutex;

		MemoryPool( const MemoryPool& );
		MemoryPool& operator=( const MemoryPool& );
	};

	template < typename T >
	class MemoryPoolPtr
	{
	public:
		MemoryPoolPtr( MemoryPool<T>* pool, T* obj )
			: m_pool( pool )
			, m_obj( obj )
		{
			assert( m_pool != nullptr );
			assert( m_obj != nullptr );
		}

		T& operator*( ) const
		{
			return *m_obj;
		}

		~MemoryPoolPtr()
		{
			m_pool->Release( m_obj );
		}

	private:
		T*				m_obj;
		MemoryPool<T>*	m_pool;
	};

	template< typename T>
	MemoryPool<T>::MemoryPool( size_t arraySize, size_t initialCapacity )
		: m_free(nullptr)
		, m_numBands(0)
		, m_lastBandIndex(0)
		, m_arraySize( arraySize )
		, m_capacity(0)
	{
		if ( initialCapacity != 0 )
		{
			Reserve( initialCapacity );
		}
	}

	template< typename T>
	MemoryPool<T>::~MemoryPool()
	{
		Clear();
	}

	template< typename T>
	T* MemoryPool<T>::Acquire()
	{
		std::lock_guard<std::mutex> lock( g_i_mutex );
		if ( m_free == nullptr )
		{
			if ( stretch() == false )
			{
				return nullptr;
			}			
		}

		T* p = m_free->block;
		m_free = m_free->next;

		return p;
	}

	template< typename T>
	void MemoryPool<T>::Release( T* p )
	{
		if ( p == nullptr )
		{
			return;
		}

		pointer ptr = static_cast< pointer >( p );
		size_t i, index;
		Band* band = nullptr;

		std::lock_guard<std::mutex> lock( g_i_mutex );
		for ( i = 0, index = m_lastBandIndex; i < m_numBands; ++i, ++index )
		{
			if ( index >= m_numBands )
			{
				index = 0;
			}
			band = m_bands + index;
			if ( band->begin <= ptr && ptr < band->end )
			{
				break;
			}
		}

		if ( i >= m_numBands )
		{
			return;
		}

		size_t nodeIndex = ptr - band->begin;
		if ( m_arraySize > 1 )
		{
			if ( nodeIndex % m_arraySize != 0 )
			{
				return;
			}

			nodeIndex /= m_arraySize;
		}

		Node* node = band->nodes + nodeIndex;
		if ( node->block != p )
		{
			return;
		}

		node->next = m_free;
		m_free = node;
		m_lastBandIndex = index;
	}

	template< typename T>
	void MemoryPool<T>::Reserve( size_t n )
	{
		std::lock_guard<std::mutex> lock( g_i_mutex );
		while ( m_capacity < n )
		{
			if ( stretch() == false )
			{
				break;
			}
		}
	}

	template< typename T>
	void MemoryPool<T>::Clear()
	{
		std::lock_guard<std::mutex> lock( g_i_mutex );
		if ( m_numBands == 0 )
		{
			return;
		}

		while ( m_numBands != 0 )
		{
			Band& band = m_bands[--m_numBands];
			AllocType::deallocate( band.begin );
			delete[] band.nodes;
		}

		m_capacity = 0;
		m_free = nullptr;
	}

	template< typename T>
	bool MemoryPool<T>::stretch()
	{
		if ( m_numBands >= MAX_BANDS )
		{
			return false;
		}

		size_t allockSize = 1 << ( m_numBands != 0 ? m_numBands : 1 );
		pointer p = nullptr;		
		do
		{
			p = static_cast< pointer >( AllocType::allocate( allockSize * m_arraySize ) );

		} while ( p != nullptr && ( ( allockSize = allockSize >> 1 ) > 1 );

		if ( p == nullptr )
		{
			return false;
		}

		Node* node = new Node[allockSize];
		if ( nullptr == node )
		{
			AllocType::deallocate( p );
			return false;
		}

		Band &band = m_bands[m_numBands++];
		band.nodes = node;
		band.begin = p;
		band.end = p + ( allockSize * m_arraySize );

		Node* tail = m_free;
		Node** link = &m_free;

		for ( size_t i = 0; i < allockSize; i++, ++node, p += m_arraySize )
		{
			node->block = p;
			*link = node;
			link = &( node->next );
		}

		*link = tail;

		m_capacity += allockSize;
		
		return true;
	}
}