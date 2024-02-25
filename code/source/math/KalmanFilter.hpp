#ifndef PBL_MATH_KALMAN_FILTER_HPP__
#define PBL_MATH_KALMAN_FILTER_HPP__

#include "PredictionModels.hpp"
#include <utils/RandomGenerator.hpp>

namespace PBL::Math
{

/**
 * @class KalmanFilter1D
 * @brief Implements a one-dimensional Kalman filter.
 *
 * This class represents a Kalman filter designed for one-dimensional data points. It is capable of 
 * smoothing out noise from an estimated variable by predicting and correcting estimates based on 
 * incoming measurements, which is commonly used in signal processing, statistical estimation, and 
 * control systems.
 *
 * @tparam T A floating-point type that represents the measurement values and parameters of the filter. 
 *           This type should support arithmetic operations and is typically either 'float' or 'double'.
 */
template < std::floating_point T >
class KalmanFilter1D
{
public:
	/**
     * @brief Constructs a new KalmanFilter1D object with initial conditions.
     * 
     * @param initialValue The initial estimated value.
     * @param processNoise The process noise covariance (Q), representing the estimated error in the process model.
     * @param measurementNoise The measurement noise covariance (R), representing the estimated error in measurements.
     * @param error The initial estimation error covariance (P), representing the accuracy of the initial estimate.
     */
	constexpr KalmanFilter1D( T initialValue, T processNoise, T measurementNoise, T error ) noexcept
		: m_initial{ initialValue }
		, m_q{ processNoise }
		, m_r{ measurementNoise }
		, m_p{ error }
	{ }

	/**
     * @brief Operator overload to allow the object to behave as a functor. 
     *        Applies the Kalman filter update step on a given measurement.
     *
     * This method represents the "predict and correct" cycle of a Kalman filter update. It doesn't explicitly
     * handle the prediction step, assuming that the system model remains constant, and directly applies the
     * correction step using the provided measurement.
     * 
     * @param measurement The new measurement value to be taken into account.
     * @return T The corrected estimate after applying the Kalman filter.
     */
	[[nodiscard]] constexpr T operator()( T measurement ) noexcept
	{
		// Prediction update omitted for simplicity: assumes constant system.

		// Measurement update (correction)
		m_k = m_p / ( m_p + m_r ); // Calculate Kalman gain

		// Estimate the current state based on the measurement and our prediction
		m_initial = m_initial + m_k * ( measurement - m_initial );

		// Update error estimate
		m_p = ( 1 - m_k ) * m_p + m_q;

		return m_initial;
	}

protected:
	T m_initial; //!< Initial value - the starting state estimate.
	T m_q; //!< Process noise covariance - represents the process uncertainty.
	T m_r; //!< Measurement noise covariance - represents the uncertainty in each measurement.
	T m_p; //!< Estimation error covariance - the uncertainty of the state estimate.
	T m_k{}; //!< Kalman gain - balances the estimate and measurement uncertainties during the update.
};

/**
 * @class KalmanFilterAdvanced1D
 * @brief Implements a one-dimensional Kalman filter with customizable prediction models.
 *
 * This advanced version of the Kalman filter maintains the foundational principles of the standard
 * Kalman filter but adds a layer of complexity and adaptability by incorporating various prediction
 * models. This feature allows the filter to be tailored to more specific applications or scenarios 
 * where different dynamics are in play, significantly extending its utility beyond simple linear contexts.
 *
 * @tparam T A floating-point type that encapsulates the measurement values and the parameters of the filter. 
 *           This type should support arithmetic operations and is typically either 'float' or 'double'.
 * @tparam PredictionModel A model type that is used to predict the next state based on the current state and 
 *                         possibly external influences. This type must be a callable entity providing the signature 
 *                         equivalent to 'T operator()(Args...)' where 'Args' represents the model-specific parameters.
 */
template < typename T, template < typename > typename PredictionModel, typename ModelValueType = T >
	requires PredictionModelType< PredictionModel< ModelValueType > >
class KalmanFilterAdvanced1D : public KalmanFilter1D< T >
{
public:
	using ModelType = PredictionModel< ModelValueType >;

	static_assert( std::is_same_v< T, typename ModelType::ValueType >,
				   "KalmanFilterAdvanced1D: Type mismatch between T and PredictionModel::ValueType" );

	/**
     * @brief Constructs a new KalmanFilterAdvanced1D object with initial conditions.
     * 
     * This constructor initializes the advanced Kalman filter with the given parameters, 
     * setting up the environment for subsequent predictions and corrections based on a 
     * customizable model.
     *
     * @param initialValue The initial estimated state value.
     * @param processNoise The process noise covariance (Q), indicating the estimated error in the process model.
     * @param measurementNoise The measurement noise covariance (R), indicating the estimated error in measurements.
     * @param error The initial estimation error covariance (P), representing the accuracy of the initial estimate.
     */
	template < typename... PredictionModelArgs >
	constexpr KalmanFilterAdvanced1D(
		T initialValue, T processNoise, T measurementNoise, T error, PredictionModelArgs&&... args ) noexcept
		: KalmanFilter1D< T >{ initialValue, processNoise, measurementNoise, error }
		, m_predictionModel{ std::forward< PredictionModelArgs >( args )... }
	{ }

	/**
     * @brief Operator overload to facilitate the filter's operation as a functor. 
     *        It applies the Kalman filter's prediction and update steps based on the provided measurement and model.
     *
     * This method integrates the prediction model into the Kalman filter's routine, allowing for 
     * an advanced, dynamic prediction in the 'predict' phase before proceeding with the standard 
     * correction step. This adaptability makes the filter suitable for a range of applications, 
     * including those requiring non-linear prediction logic.
     * 
     * @param measurement The new measurement value to be considered.
     * @param args The additional arguments required by the prediction model, passed variadically.
     * 
     * @return T The updated estimate after considering the incoming measurement and applying the prediction model.
     */
	template < typename... PredictionModelArgs >
	[[nodiscard]] T operator()( T measurement, PredictionModelArgs&&... args )
	{
		auto& self = *this;

		// Prediction step: Apply the user-defined prediction model to estimate the new state.
		// This process may involve various real-world factors, encapsulated within the prediction model.
		T predictedState = self.m_initial + m_predictionModel( std::forward< PredictionModelArgs >( args )... );

		// Update estimation error covariance for the prediction
		self.m_p += self.m_q;

		// Measurement update (correction)
		self.m_k = self.m_p / ( self.m_p + self.m_r ); // Calculate Kalman gain

		// Correct the state estimate using the new measurement.
		self.m_initial = predictedState + self.m_k * ( measurement - predictedState );

		// Update error estimate
		self.m_p = ( 1 - self.m_k ) * self.m_p;

		return self.m_initial;
	}

private:
	ModelType m_predictionModel;
};

using KalmanFilter1Df = KalmanFilter1D< float >;
using KalmanFilter1Dd = KalmanFilter1D< double >;

} // namespace PBL::Math
#endif // PBL_MATH_KALMAN_FILTER_HPP__
