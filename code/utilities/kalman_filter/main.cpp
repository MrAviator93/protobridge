
#include <math/KalmanFilter.hpp>

// C++
#include <array>
#include <format>
#include <iostream>

namespace
{

template < typename T >
struct NonInvocable
{
	using ValueType = T;

	// Not implementing this compiler should trigger compile time error
	// T operator()() { return 0; }
};

} // namespace

int main()
{
	const double initialValue{};
	const double processNoise{ 1e-5 };
	const double measurementNoise{ 0.1 };
	const double error{ 1 };
	const std::array measurements = { 0.39, 0.50, 0.48, 0.29, 0.25, 0.32, 0.34, 0.48, 0.41, 0.45 };

	// Constant system, prediction model omitted
	{
		// Create a Kalman filter instance
		PBL::Math::KalmanFilter1Dd kf{ initialValue, processNoise, measurementNoise, error };

		// Simulated measurements

		std::cout << "Results - Constant System Model" << std::endl;
		for( const auto measurement : measurements )
		{
			auto result = kf( measurement );
			std::cout << std::format( "Measured value: {:.4f} Estimated value: {:.4f}", measurement, result )
					  << std::endl;
		}
	}

	{
		double dt{ 0.1 }; // Time step between measurements
		double rate{ 0.1 };
		PBL::Math::KalmanFilterAdvanced1D< double, PBL::Math::LinearMotionModel > kf{
			initialValue, processNoise, measurementNoise, error };

		std::cout << "Results - Linear Motion Model" << std::endl;
		for( const auto& measurement : measurements )
		{
			auto result = kf( measurement, dt, rate );
			std::cout << std::format( "Measured value: {:.4f} Estimated value: {:.4f}", measurement, result )
					  << std::endl;
		}
	}

	{
		double dt{ 0.1 }; // Time step between measurements
		double circularSpeed{ 1.0 }; // Speed of the circular motion
		double circularRadius{ 5.0 }; // Radius of the circular path

		PBL::Math::KalmanFilterAdvanced1D< double, PBL::Math::CircularMotionModel > kfCircular{
			initialValue, processNoise, measurementNoise, error };

		std::cout << "Results - Circular Motion Model:" << std::endl;
		for( const auto& measurement : measurements )
		{
			// And potentially other args for circular motion
			auto estimatedState = kfCircular( measurement, dt, circularSpeed, circularRadius );
			std::cout << std::format( "Measured value: {:.4f} Estimated value: {:.4f}", measurement, estimatedState )
					  << std::endl;
		}
	}

	{
		double dt{ 0.1 }; // Time step between measurements
		double rate = 0.0; // Initial rate of change of the state (e.g., initial velocity in a physical system)
		double externalForce = 10.0; // The external force applied to the system
		double mass = 2.0; // Mass of the object subjected to the external force

		PBL::Math::KalmanFilterAdvanced1D< double, PBL::Math::ExternalForceModel > kfExternal{
			initialValue, processNoise, measurementNoise, error };

		std::cout << "Results - External Force Model:" << std::endl;
		for( const auto& measurement : measurements )
		{
			// And potentially other args for circular motion
			auto estimatedState = kfExternal( measurement, dt, rate, externalForce, mass );
			std::cout << std::format( "Measured value: {:.4f} Estimated value: {:.4f}", measurement, estimatedState )
					  << std::endl;
		}
	}

	{
		PBL::Math::KalmanFilterAdvanced1D< double, PBL::Math::RandomWalkModel > kfRandom{
			initialValue, processNoise, measurementNoise, error, 0.0, processNoise };

		std::cout << "Results - Random Walk Model:" << std::endl;
		for( const auto& measurement : measurements )
		{
			// And potentially other args for circular motion
			auto estimatedState = kfRandom( measurement, measurement, processNoise );
			std::cout << std::format( "Measured value: {:.4f} Estimated value: {:.4f}", measurement, estimatedState )
					  << std::endl;
		}
	}

	// Won't compile
	// {
	// 	PBL::Utils::KalmanFilterAdvanced1D< double, NonInvocable > kfNonInvocable{
	// 		initialValue, processNoise, measurementNoise, error };
	// }

	return 0;
}
