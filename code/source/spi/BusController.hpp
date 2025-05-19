#ifndef PBL_SPI_BUS_CONTROLLER_HPP__
#define PBL_SPI_BUS_CONTROLLER_HPP__

#include <utils/Result.hpp>

// C++
#include <mutex>
#include <chrono>
#include <atomic>
#include <string>
#include <cstdint>
#include <utility>
#include <shared_mutex>

namespace pbl::spi
{

inline namespace v1
{

class BusController
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	using ByteSpan = std::span< std::uint8_t >;
	using ConstByteSpan = std::span< const std::uint8_t >;

	/// SPI mode
	enum class Mode : std::uint8_t
	{
		MODE_0,
		MODE_1,
		MODE_2,
		MODE_3
	};

	/// SPI speed
	enum class Speed : std::uint32_t
	{
		SPEED_500KHZ = 500000,
		SPEED_1MHZ = 1000000,
		SPEED_5MHZ = 5000000,
		SPEED_10MHZ = 10000000
	};

	/// Bits per word
	enum class BitsPerWord : std::uint8_t
	{
		BITS_8 = 8,
		BITS_16 = 16
	};

	[[nodiscard]] static Result< BusController > open( const std::string& device,
													   Mode mode = Mode::MODE_0,
													   Speed speed = Speed::SPEED_1MHZ,
													   BitsPerWord bits = BitsPerWord::BITS_8 );

	BusController( BusController&& other ) noexcept
		: m_busName( std::move( other.m_busName ) )
		, m_open( other.m_open.load() )
		, m_fd( std::exchange( other.m_fd, -1 ) )
	{ }

	BusController& operator=( BusController&& other ) noexcept
	{
		if( this != &other )
		{
			m_fd = std::exchange( other.m_fd, -1 );
			m_open = other.m_open.load();
		}
		return *this;
	}

	/// Default dtor, closes file m_fd file.
	virtual ~BusController();

	/// Returns the OS name of the physical bus name
	[[nodiscard]] auto& bus() const { return m_busName; }

	/// Puts asleep calling thread for specified sleep time in milliseconds
	void sleep( const std::chrono::milliseconds sleepTimeMs );

	/// Puts asleep calling thread for specified sleep time in microseconds
	void sleep( const std::chrono::microseconds sleepTimeUs );

private:
	explicit BusController( const std::string& busName );

	// This class is non-copyable
	BusController( const BusController& ) = delete;
	BusController operator=( const BusController& ) = delete;

	/// Configures the SPI bus
	[[nodiscard]] Result< void > configure( Mode mode, Speed speed, BitsPerWord bitsPerWord );

	/// TBW
	[[nodiscard]] static std::string getError();

private:
	const std::string m_busName; //!< I2C Bus name, i.e. "/dev/i2c-1"
	std::atomic_bool m_open{ false }; //!< Indicates whether the I2C bus is open

	mutable std::mutex m_fdMtx; //!< Locks the read write operations
	int m_fd{ -1 }; //!< File/device descriptor
};

} // namespace v1
} // namespace pbl::spi
#endif // PBL_SPI_BUS_CONTROLLER_HPP__
