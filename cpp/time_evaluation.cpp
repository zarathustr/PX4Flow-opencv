#include "eig3D.h"
#include "FA3R.h"
#include "Umeyama.h"
#include <fstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

using namespace std;
using namespace Eigen;

std::ofstream output("time_consumption.txt");

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

	int loopNum, FA3R_num;
	std::cout << "Please enter the maximum number of loops: ";
	std::cin >> loopNum;
	std::cout << "Please enter the noise magnitude of reference vectors: ";
	std::cin >> reference_scale;
	std::cout << "Please enter the magnitude of the translation vector: ";
	std::cin >> T_scale;
	std::cout << "Please enter the SNR: ";
	std::cin >> snr;
    std::cout << "Please enter the iteration number of FA3R: ";
    std::cin >> FA3R_num;

	Matrix3d R_umeyama_fast, R_umeyama_eigen, R_FA3R_int, R_eig3D_eig, R_eig3D_symbolic, R_FA3R_double;
	Vector3d T_umeyama_fast, T_umeyama_eigen, T_FA3R_int, T_eig3D_eig, T_eig3D_symbolic, T_FA3R_double;

	output.setf(std::ios::showpoint); 
	output.precision(20); 

    
    double time;
    VectorXd time_umeyama_fast(loopNum + 1),
             time_umeyama_eigen(loopNum + 1),
             time_FA3R_int(loopNum + 1),
             time_FA3R_double(loopNum + 1),
             time_eig3D_eig(loopNum + 1), 
             time_eig3D_symbolic(loopNum + 1);


	for (int i = 1; i <= loopNum; ++i) {

		vector<Vector3d> pointsOriginal;
		vector<Vector3d> pointsTransformed;

		for(int j = 0 ; j < i * 10000 ; ++j)
		{
			Vector3d v = getRandomVector();
			pointsOriginal.push_back(v * reference_scale);
		}

		Vector3d axis = getRandomVector();
		Vector3d T = getRandomVector() * T_scale;
		double angle = 2 * M_PI * getRandom();
		Quaterniond Q;
		Q = AngleAxisd(angle, axis);

		for(int j = 0 ; j < i * 10000 ; ++j)
		{
			Vector3d tmp = Q._transformVector(pointsOriginal[j]) + (1.0 / snr) * (reference_scale + T_scale) * getRandomVector() + T;
			pointsTransformed.push_back(tmp);
		}

		Matrix3d M;
		M.setZero();
		Vector3d mean_X, mean_Y;

		int nn = pointsTransformed.size();
       	mean_X.setZero();
       	mean_Y.setZero();

       	for (int i = 0; i < nn; ++i)
       	{
          	mean_X = mean_X + pointsTransformed[i];
          	mean_Y = mean_Y + pointsOriginal[i];
       	}
       	mean_X = mean_X / nn;
       	mean_Y = mean_Y / nn;

       	for (int i = 0; i < nn; ++i)
       	{
          	M = M + (pointsOriginal[i] - mean_Y) * ((pointsTransformed[i] - mean_X).transpose());
       	}
       	M = M / nn;

		for(int j = 0; j < i * 10000; ++j)
		{

			Umeyama(nullptr, nullptr, &M, &R_umeyama_fast, &T_umeyama_fast, true, time);
			time_umeyama_fast(i) += time;

			Umeyama(nullptr, nullptr, &M, &R_umeyama_eigen, &T_umeyama_eigen, false, time);
			time_umeyama_eigen(i) += time;

			FA3R_int(nullptr, nullptr, &M, FA3R_num, &R_FA3R_int, &T_FA3R_int, time);
			time_FA3R_int(i) += time;

			FA3R_double(nullptr, nullptr, &M, FA3R_num, &R_FA3R_double, &T_FA3R_double, time);
			time_FA3R_double(i) += time;

			eig3D_eig(nullptr, nullptr, &M, &R_eig3D_eig, &T_eig3D_eig, time);
			time_eig3D_eig(i) += time;

			eig3D_symbolic(nullptr, nullptr, &M, &R_eig3D_symbolic, &T_eig3D_symbolic, time);
			time_eig3D_symbolic(i) += time;
    	}
    	/*
		std::cout << "Exec time Umeyama_fast: "<< time_umeyama_fast(i) / double(CLOCKS_PER_SEC) << " sec." << endl;
		std::cout << "Exec time Umeyama_eigen: "<< time_umeyama_eigen(i) / double(CLOCKS_PER_SEC) << " sec." << endl;
		std::cout << "Exec time eig3D_eig: " << time_eig3D_eig(i) / double(CLOCKS_PER_SEC) << " sec." << endl;
		std::cout << "Exec time eig3D_symbolic: " << time_eig3D_symbolic(i) / double(CLOCKS_PER_SEC) << " sec." << endl;
    	std::cout << "Exec time FA3R_int: "<< time_FA3R_int(i) / double(CLOCKS_PER_SEC) << " sec." << endl;
    	std::cout << "Exec time FA3R_double: "<< time_FA3R_double(i) / double(CLOCKS_PER_SEC) << " sec." << endl;
    	std::cout << std::endl;
    	*/

		output << i << " ";
    	output << time_umeyama_fast(i) / double(CLOCKS_PER_SEC) << " ";
		output << time_umeyama_eigen(i) / double(CLOCKS_PER_SEC) << " ";
		output << time_eig3D_eig(i) / double(CLOCKS_PER_SEC) << " ";
		output << time_eig3D_symbolic(i) / double(CLOCKS_PER_SEC) << " ";
    	output << time_FA3R_int(i) / double(CLOCKS_PER_SEC) << " ";
    	output << time_FA3R_double(i) / double(CLOCKS_PER_SEC) << " ";
    	output << std::endl;
	}
	return 0;
}
