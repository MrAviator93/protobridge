
#ifndef I2C_UTILS_COMPLEMENTARY_FILTER_HPP__
#define I2C_UTILS_COMPLEMENTARY_FILTER_HPP__

#include <concepts>

namespace PBL::Utils
{

/**
 * @brief Implements a complementary filter often used in sensor fusion.
 *        This filter combines the advantages of two different sensors, typically
 *        an accelerometer and a gyroscope, to produce a more accurate output. It
 *        is particularly useful in the context of angle estimation.
 * 
 * @note It's important to create a separate instance of ComplementaryFilter for
 *       each angle you are measuring (e.g., pitch, roll, yaw) as each instance
 *       maintains its internal state.
 * 
 * @tparam T Numeric type (usually float or double) representing the measurement 
 *           values. This type should support arithmetic operations.
 */
template < std::floating_point T >
class ComplementaryFilter
{
public:
	/**
     * @brief Constructs a new Complementary Filter object with a single weight factor.
     *        This constructor automatically balances the weight between the gyroscope 
     *        and the accelerometer based on the provided alpha for the gyroscope.
     * 
     * @param alphaGyro The weight factor for the gyroscope measurements. It's a value 
     *                  between 0 and 1, where values closer to 1 give more weight to the 
     *                  gyroscope, and values closer to 0 imply more reliance on the 
     *                  accelerometer data (by deducting the gyroscope's alpha value 
     *                  from 1).
     */
	constexpr ComplementaryFilter( T alphaGyro ) noexcept
		: m_alphaGyro{ alphaGyro }
		, m_alphaAccel{ T{ 1 } - alphaGyro }
	{ }

	/**
     * @brief Constructs a new Complementary Filter object with distinct weight factors.
     *        This constructor allows for independent setting of the weight factors
     *        (alpha values) for the gyroscope and accelerometer, enabling finer control
     *        over the balance of sensor data reliance.
     * 
     * @param alphaGyro The weight factor for the gyroscope measurements. It's a value
     *                  between 0 and 1, representing the reliance on the gyroscope data.
     * @param alphaAccel The weight factor for the accelerometer measurements. Similar
     *                   to alphaGyro, it's a value between 0 and 1, indicating the
     *                   reliance on the accelerometer data.
     *
     *  @note For stability and effective sensor fusion, it's generally recommended that
     * alphaGyro, alphaAccel collectively sum to 1.0. Adjustments may be necessary based
     * on specific application dynamics and testing.
     */
	constexpr ComplementaryFilter( T alphaGyro, T alphaAccel ) noexcept
		: m_alphaGyro{ alphaGyro }
		, m_alphaAccel{ alphaAccel }
	{ }

	/**
     * @brief Applies the complementary filter algorithm on the input data to estimate the angle.
     * 
     * @param dt Time interval since the last update (time step).
     * @param gyroRate Rate of change of the angle as measured by the gyroscope (angular velocity).
     * @param accelAngle Current angle as measured by the accelerometer.
     * 
     * @return T The estimated angle after applying the complementary filter.
     */
	[[nodiscard]] constexpr T operator()( T dt, T gyroRate, T accelAngle ) noexcept
	{
		m_angle = m_alphaGyro * ( m_angle + gyroRate * dt ) + m_alphaAccel * accelAngle;
		return m_angle;
	}

	/// Returns the current angle estimation
	[[nodiscard]] T angle() const { return m_angle; }

	/// Retrieves the alpha value used as the weight factor in the filter
	[[nodiscard]] T alpha() const { return m_alphaGyro; }

	/// Updates the alpha value, adjusting the balance in sensor readings
	void setAlpha( T alpha ) { m_alphaGyro = alpha; }

private:
	T m_angle{}; //!< Current angle estimation maintained by this filter
	T m_alphaGyro{}; //!< Weight factor for gyroscope
	T m_alphaAccel{}; //!< Weight factor for acceleration
};

/**
 * @brief A specialized version of the complementary filter that integrates velocity data 
 *        into the sensor fusion algorithm, suitable for scenarios where precise motion tracking 
 *        is required. It harmonizes data from gyroscopes, accelerometers, and a velocity metric 
 *        to produce a highly accurate, real-time estimation, often used in robotics and motion analysis.
 * 
 * @tparam T A floating-point type that represents the measurement values and parameters of the filter. 
 *           This type should support arithmetic operations and is typically either 'float' or 'double'.
 */
template < std::is_floating_point T >
class ComplementaryFilterWithVelocity
{
public:
	/**
     * @brief Constructs a new Complementary Filter object that incorporates velocity.
     * 
     * Initializes the filter with weight factors for gyroscope, accelerometer, and velocity data.
     *
     * @param alphaGyro Weight factor for the gyroscope data.
     * @param alphaAccel Weight factor for the accelerometer data.
     * @param alphaVelocity Weight factor for the velocity data.
     *
     * @note For stability and effective sensor fusion, it's generally recommended that 
     * alphaGyro, alphaAccel, and alphaVelocity collectively sum to 1.0. Adjustments may be 
     * necessary based on specific application dynamics and testing.
     */
	constexpr ComplementaryFilterWithVelocity( T alphaGyro, T alphaAccel, T alphaVelocity ) noexcept
		: m_filter{ alphaGyro, alphaAccel }
		, m_alphaVelocity{ alphaVelocity }
	{ }

	/**
     * @brief Calculates the filtered output considering the gyroscope, accelerometer, and velocity data.
     *
     * @param dt Delta time - time step since the last update.
     * @param gyroRate Rate of change of the angle measured by the gyroscope.
     * @param accelAngle Angle as measured by the accelerometer.
     * @param velocity Velocity measurement to be considered in the filtering process.
     * @return Filtered value (typically angle) after processing the inputs with their respective weight factors.
     */
	[[nodiscard]] constexpr T()( T dt, T gyroRate, T accelAngle, T velocity ) noexcept
	{
		return m_filter( dt, gyroRate, accelAngle ) + ( m_alphaVelocity * velocity * dt );
	}

private:
	ComplementaryFilter< T > m_filter; //!< Instance of the basic complementary filter for gyro and accel data
	T m_alphaVelocity{}; //!< Weight factor for the velocity data in the filter calculation
};

} // namespace PBL::Utils
#endif // I2C_UTILS_COMPLEMENTARY_FILTER_HPP__
