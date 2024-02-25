
#ifndef I2C_MATH_MATRIX_BASE_HPP__
#define I2C_MATH_MATRIX_BASE_HPP__

#include "Math.hpp"
#include "Linear.hpp"

// C++
#include <array>
#include <cstdint>
#include <optional>
#include <algorithm>
#include <type_traits>

namespace PBL::Math
{

template < typename T, std::size_t Rows, std::size_t Columns >
	requires std::is_arithmetic_v< T >
class MatrixBase
{
	static constexpr std::size_t Size = Rows * Columns;
	static constexpr std::size_t Alignment = AlignmentSelectorV< T >;

public:
	using ValueType = T;

	constexpr MatrixBase() noexcept = default;

	template < typename... Args >
		requires( sizeof...( Args ) == Size )
	constexpr MatrixBase( Args&&... args ) noexcept
		: m_data{ std::forward< Args >( args )... }
	{ }

	[[nodiscard]] constexpr std::optional< T > at( std::size_t idx ) const noexcept
	{
		if( idx < Size )
		{
			return m_data[ idx ];
		}

		return {};
	}

	[[nodiscard]] constexpr std::optional< T > at( std::size_t column, std::size_t row ) const noexcept { return at( column * row ); }

	[[nodiscard]] constexpr auto& data() noexcept { return m_data; }
	[[nodiscard]] constexpr auto& data() const noexcept { return m_data; }

	[[nodiscard]] static constexpr std::size_t size() noexcept { return Size; }
	[[nodiscard]] static constexpr std::size_t rows() noexcept { return Rows; }
	[[nodiscard]] static constexpr std::size_t columns() noexcept { return Columns; }

	// Compile-time constant to check if the data is aligned
	// static constexpr bool IsDataAligned = ( sizeof( std::array< T, Size > ) % Alignment == 0 );

protected:
	[[nodiscard]] constexpr bool isAligned() const noexcept
	{
		auto ptr = reinterpret_cast< std::uintptr_t >( m_data.data() );
		return ptr % Alignment == 0;
	}

protected:
	alignas( Alignment ) std::array< T, Size > m_data;
};

} // namespace PBL::Math
#endif //I2C_MATH_MATRIX_BASE_HPP__
