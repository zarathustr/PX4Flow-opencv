#include "eig3D.h"
#include "FA3R.h"
#include "Umeyama.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

using namespace std;
using namespace Eigen;

Vector3d getRandomVector()
{
	double e0 = (double)rand() / (double)RAND_MAX;
	double e1 = (double)rand() / (double)RAND_MAX;
	double e2 = (double)rand() / (double)RAND_MAX;

	Vector3d v(e0, e1, e2);
	v.normalize();
	return v;
}

double getRandom()
{
	return (double)rand() / (double)RAND_MAX;
}

double EstimationError(const vector<Vector3d>& P, const vector<Vector3d>& Q, const Matrix3d& R, Vector3d& T)
{
	int N = P.size();
	double error = 0;
	for (int i = 0; i < N; ++i)
	{
		error += (R.transpose() * Q[i] + T - P[i]).norm() * (R.transpose() * Q[i] + T - P[i]).norm();
	}
	return error / N;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	double snr, reference_scale, T_scale;

	int numVec, loopNum, FA3R_num;
	std::cout << "Please enter the number of vectors: ";
	std::cin >> numVec;
	std::cout << "Please enter the number of loops: ";
	std::cin >> loopNum;
	std::cout << "Please enter the noise magnitude of reference vectors: ";
	std::cin >> reference_scale;
	std::cout << "Please enter the magnitude of the translation vector: ";
	std::cin >> T_scale;
	std::cout << "Please enter the SNR: ";
	std::cin >> snr;
    std::cout << "Please enter the iteration number of FA3R: ";
    std::cin >> FA3R_num;

	vector<Vector3d> pointsOriginal;
	vector<Vector3d> pointsTransformed;

	for(int i = 0 ; i < numVec ; ++i)
	{
		Vector3d v = getRandomVector();
		pointsOriginal.push_back(v * reference_scale);
	}

	Vector3d axis = getRandomVector();
	Vector3d T = getRandomVector() * T_scale;
	double angle = 2 * M_PI * getRandom();
	Quaterniond Q;
	Q = AngleAxisd(angle, axis);

	for(int i = 0 ; i < numVec ; ++i)
	{
		Vector3d tmp = Q._transformVector(pointsOriginal[i]) + (1.0 / snr) * (reference_scale + T_scale) * getRandomVector() + T;
		pointsTransformed.push_back(tmp);
	}

	Matrix3d R_umeyama_fast, R_umeyama_eigen, R_FA3R_int, R_eig3D_eig, R_eig3D_symbolic, R_FA3R_double;
	Vector3d T_umeyama_fast, T_umeyama_eigen, T_FA3R_int, T_eig3D_eig, T_eig3D_symbolic, T_FA3R_double;

    double tmp = 0.0;
	Umeyama(&pointsTransformed, &pointsOriginal, nullptr, &R_umeyama_fast, &T_umeyama_fast, true, tmp);
	Umeyama(&pointsTransformed, &pointsOriginal, nullptr, &R_umeyama_eigen, &T_umeyama_eigen, false, tmp);
	FA3R_int(&pointsTransformed, &pointsOriginal, nullptr, FA3R_num, &R_FA3R_int, &T_FA3R_int, tmp);
	FA3R_double(&pointsTransformed, &pointsOriginal, nullptr, FA3R_num, &R_FA3R_double, &T_FA3R_double, tmp);
	eig3D_eig(&pointsTransformed, &pointsOriginal, nullptr, &R_eig3D_eig, &T_eig3D_eig, tmp);
	eig3D_symbolic(&pointsTransformed, &pointsOriginal, nullptr, &R_eig3D_symbolic, &T_eig3D_symbolic, tmp);

	double errorUmeyama_fast = EstimationError(pointsTransformed, pointsOriginal, R_umeyama_fast, T_umeyama_fast);
	double errorUmeyama_eigen = EstimationError(pointsTransformed, pointsOriginal, R_umeyama_eigen, T_umeyama_eigen);
	double errorFA3R_int = EstimationError(pointsTransformed, pointsOriginal, R_FA3R_int, T_FA3R_int);
	double errorFA3R_double = EstimationError(pointsTransformed, pointsOriginal, R_FA3R_double, T_FA3R_double);
	double errorEig3D_eig = EstimationError(pointsTransformed, pointsOriginal, R_eig3D_eig, T_eig3D_eig);
	double errorEig3D_symbolic = EstimationError(pointsTransformed, pointsOriginal, R_eig3D_symbolic, T_eig3D_symbolic);

	std::cout.setf(std::ios::showpoint); 
	std::cout.precision(20); 

	std::cout << "Umeyama_fast error " << errorUmeyama_fast << endl;
	std::cout << "Umeyama_eigen error " << errorUmeyama_eigen << endl;
	std::cout << "FA3R_int error " << errorFA3R_int << endl;
	std::cout << "FA3R_double error " << errorFA3R_double << endl;
	std::cout << "eig3D_eig error " << errorEig3D_eig << endl;
	std::cout << "eig3D_symbolic error " << errorEig3D_symbolic << endl;
    std::cout << std::endl;

    std::cout << "Umeyama_fast R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << R_umeyama_fast(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Umeyama_fast T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T_umeyama_fast(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "Umeyama_eigen R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << R_umeyama_eigen(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Umeyama_eigen T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T_umeyama_eigen(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	
	std::cout << "FA3R_int R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << R_FA3R_int(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "FA3R_int T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T_FA3R_int(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "FA3R_double R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << R_FA3R_double(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "FA3R_double T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T_FA3R_double(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	
	std::cout << "eig3D_eig R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << R_eig3D_eig(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "eig3D_eig T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T_eig3D_eig(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "eig3D_symbolic R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << R_eig3D_symbolic(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "eig3D_symbolic T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T_eig3D_symbolic(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	
	std::cout << "Standard R:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << Q.toRotationMatrix()(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Standard T:" << std::endl;
	for(int i = 0; i < 3; ++i)
	{
		std::cout << T(i) << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
    
    double time_umeyama_fast = 0.0,
            time_umeyama_eigen = 0.0,
            time_FA3R_int = 0.0,
            time_FA3R_double = 0.0,
            time_eig3D_eig = 0.0, 
            time_eig3D_symbolic = 0.0,
            time;

	for (int i = 0; i < loopNum; ++i) {

		Umeyama(&pointsTransformed, &pointsOriginal, nullptr, &R_umeyama_fast, &T_umeyama_fast, true, time);
		time_umeyama_fast += time;

		Umeyama(&pointsTransformed, &pointsOriginal, nullptr, &R_umeyama_eigen, &T_umeyama_eigen, false, time);
		time_umeyama_eigen += time;

		FA3R_int(&pointsTransformed, &pointsOriginal, nullptr, FA3R_num, &R_FA3R_int, &T_FA3R_int, time);
		time_FA3R_int += time;

		FA3R_double(&pointsTransformed, &pointsOriginal, nullptr, FA3R_num, &R_FA3R_double, &T_FA3R_double, time);
		time_FA3R_double += time;

		eig3D_eig(&pointsTransformed, &pointsOriginal, nullptr, &R_eig3D_eig, &T_eig3D_eig, time);
		time_eig3D_eig += time;

		eig3D_symbolic(&pointsTransformed, &pointsOriginal, nullptr, &R_eig3D_symbolic, &T_eig3D_symbolic, time);
		time_eig3D_symbolic += time;
	}
	std::cout << "Exec time Umeyama_fast: "<< time_umeyama_fast / double(CLOCKS_PER_SEC) << " sec." << endl;
	std::cout << "Exec time Umeyama_eigen: "<< time_umeyama_eigen / double(CLOCKS_PER_SEC) << " sec." << endl;
	std::cout << "Exec time eig3D_eig: " << time_eig3D_eig / double(CLOCKS_PER_SEC) << " sec." << endl;
	std::cout << "Exec time eig3D_symbolic: " << time_eig3D_symbolic / double(CLOCKS_PER_SEC) << " sec." << endl;
    std::cout << "Exec time FA3R_int: "<< time_FA3R_int / double(CLOCKS_PER_SEC) << " sec." << endl;
    std::cout << "Exec time FA3R_double: "<< time_FA3R_double / double(CLOCKS_PER_SEC) << " sec." << endl;
	return 0;
}
