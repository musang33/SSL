#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace SSL
{
	template<typename T>
	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		virtual ~NonCopyable() {};

	private:
		NonCopyable(const NonCopyable<T>& src) = delete;
		NonCopyable<T>& operator=(const NonCopyable<T>& rhs) = delete;
	};
}

#endif // NONCOPYABLE_H

