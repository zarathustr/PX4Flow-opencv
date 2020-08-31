#ifndef __UMEYAMA_H__
#define __UMEYAMA_H__

#pragma once

#include <time.h>
#include <iostream>
#include <vector>
#include <map>
#include "Eigen30.h"
#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include "Eigen/SVD"

using namespace std;
using namespace Eigen;

void Umeyama(const vector<Vector3f>* P, 
             const vector<Vector3f>* Q, 
             Matrix3f * sigma, 
             Matrix3f * rRes, 
             Vector3f * tRes, 
             bool isFastSVD, 
             float &time);


#endif
