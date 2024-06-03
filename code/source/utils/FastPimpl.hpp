#ifndef PBL_UTILS_FAST_PIMPL_HPP__
#define PBL_UTILS_FAST_PIMPL_HPP__

// C++
#include <memory>
#include <cstddef>
#include <utility>

namespace pbl::utils
{

/**
 * @brief FastPimpl is a "pointer to implementation" or Pimpl idiom variation designed specifically for 
 * environments where memory allocation performance is critical or dynamic memory allocation is not desired, 
 * such as in embedded systems or microcontrollers. 
 * 
 * Traditional Pimpl idioms often use a pointer to a dynamically allocated memory block for the implementation, 
 * which abstracts the implementation details from the interface. However, this induces a mandatory heap allocation, 
 * which can negatively impact performance and memory usage in resource-constrained environments.
 * 
 * FastPimpl circumvents this by storing the implementation object on the stack, eliminating the need for dynamic 
 * memory allocation and improving cache locality, potentially leading to faster access times. The template parameters 
 * require the user to specify the size and alignment of the type T, ensuring that the object is properly stored within 
 * FastPimpl without the need for a pointer dereference.
 * 
 * @tparam T The type of the object which you want to hide behind the implementation.
 * @tparam Size The size in bytes of the type T. It is crucial to specify this size exactly, as too small a size will 
 *              lead to stack corruption, and a larger size will waste stack space.
 * @tparam Alignment The alignment requirement of type T. Proper alignment is necessary to comply with the hardware 
 *                   and language rules about the correct memory addresses for objects of type T.
 * 
 * Usage of this class requires careful balance and understanding of the trade-offs between hiding the implementation 
 * details and the restrictions of stack memory in constrained environments.
 */
template < class T, std::size_t Size, std::size_t Alignment >
class FastPimpl final
{
public:
	/**
     * @brief Constructs the object within the internal storage of the class.
     *        This is where the 'fast' in FastPimpl comes from, as it avoids a heap allocation.
     * 
     * @param args The arguments to forward to the constructor of T.
     */
	template < typename... Args >
	FastPimpl( Args&&... args )
	{
		validate< sizeof( T ), alignof( T ) >();
		std::construct_at( reinterpret_cast< T* >( &m_storage ), std::forward< Args >( args )... );
	}

	/// Destroys the internal object. Ensures proper destruction is called for the encapsulated object.
	~FastPimpl() noexcept
	{
		validate< sizeof( T ), alignof( T ) >();
		std::destroy_at( reinterpret_cast< T* >( &m_storage ) );
	}

	/// Gets the pointer to the internal object.
	T* get() { return reinterpret_cast< T* >( &m_storage ); }

	/// Gets the pointer to the internal object (const version)
	const T* get() const { return reinterpret_cast< const T* >( &m_storage ); }

	/// Overloads the arrow operator to allow direct access to the internal object's methods.
	T* operator->() { return reinterpret_cast< T* >( &m_storage ); }

	/// Overloads the arrow operator to allow direct access to the internal object's methods (const version).
	const T* operator->() const { return reinterpret_cast< const T* >( &m_storage ); }

private:
	/**
     * @brief Validates the size and alignment at compile time. This is where the safety in FastPimpl comes from.
     *        It ensures that the storage is correctly sized and aligned for the type T.
     * 
     * @tparam ActualSize The actual size needed by the type T.
     * @tparam ActualAlignment The actual alignment needed by the type T.
     */
	template < std::size_t ActualSize, std::size_t ActualAlignment >
	static void validate() noexcept
	{
		static_assert( Size == ActualSize, "invalid size" );
		static_assert( Alignment == ActualAlignment, "invalid alignment" );
	}

private:
	alignas( Alignment ) std::byte m_storage[ Size ]; //!< Aligned storage for the type T.
};

} // namespace pbl::utils
#endif // PBL_UTILS_FAST_PIMPL_HPP__
