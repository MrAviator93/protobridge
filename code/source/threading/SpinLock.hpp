#ifndef PBL_THREADING_SPIN_LOCK_HPP__
#define PBL_THREADING_SPIN_LOCK_HPP__

// C++
#include <atomic>

namespace pbl::threading
{

/**
 * @class SpinLock
 * @brief Advanced spin-lock implementation based on lock free std::atomic_flag.
 * 
 * @note Based on std::atomic_flag
 */
class SpinLock final
{
public:
	SpinLock() = default;
	~SpinLock() = default;

	/// Locks the the spinlock
	inline void lock() noexcept
	{
		// Quick locking attempt
		if( !m_locked.test_and_set( std::memory_order::acquire ) ) [[likely]]
		{
			return;
		}

		deepLock();
	}

	/// Unlocks the spinlock
	inline void unlock() noexcept { m_locked.clear( std::memory_order::release ); }

	/// Returns wether the spinlock is locked
	[[nodiscard]] inline bool isLocked() const noexcept { return m_locked.test( std::memory_order::consume ); }

	/// Attempts to lock the spinlock, returns the result of the attempt
	[[nodiscard]] inline bool tryLock() noexcept { return !m_locked.test_and_set( std::memory_order::acquire ); }

private:
	void deepLock() noexcept;

private:
	SpinLock( const SpinLock& ) = delete;
	SpinLock& operator=( const SpinLock& ) = delete;
	SpinLock( SpinLock&& ) = delete;
	SpinLock& operator=( SpinLock&& ) = delete;

private:
	mutable std::atomic_flag m_locked{ ATOMIC_FLAG_INIT };
};

} // namespace pbl::threading
#endif // PBL_THREADING_SPIN_LOCK_HPP__
