#include "Umeyama.h"
#include <stdint.h>
#include "micros.h"

const float pi23 = 2.094395102393195;

void svd3_fast(Matrix3f& A, Matrix3f *UU, Matrix3f *VV)
{
	float A11, A12, A13, A21, A22, A23, A31, A32, A33;
	float B11, B12, B13, B22, B23, B33;
	float B12B23, B13B22, B13B12, B11B23, B12B12, B11B22;
	float a, b, c, a3, p, q, p2, T, theta3;
	float x0, x1, x2, x3, rs1, rs2, rs3, tmp;
	float d1, d2, d3;
	Vector3f u1, u2, u3;

    A11 = A(0, 0);            A12 = A(0, 1);                A13 = A(0, 2);
    A21 = A(1, 0);            A22 = A(1, 1);                A23 = A(1, 2);
    A31 = A(2, 0);            A32 = A(2, 1);                A33 = A(2, 2);
                              
    B11 = A11 * A11 + A21 * A21 + A31 * A31;  
    B12 = A11 * A12 + A21 * A22 + A31 * A32; 
    B13 = A11 * A13 + A21 * A23 + A31 * A33;
    B22 = A12 * A12 + A22 * A22 + A32 * A32; 
    B23 = A12 * A13 + A22 * A23 + A32 * A33;
    B33 = A13 * A13 + A23 * A23 + A33 * A33;
    
    B12B23 = B12 * B23;  
    B13B22 = B13 * B22; 
    B13B12 = B13 * B12; 
    B11B23 = B11 * B23; 
    B12B12 = B12 * B12;
    B11B22 = B11 * B22;
    
    a = -(B11 + B22 + B33);
    b =   B11B22 + B11 * B33 + B22 * B33 - B23 * B23 - B12B12 - B13 * B13;
    c = -(B11B22 * B33 - B11B23 * B23 - B12B12 * B33 + 2.0 * B12B23 * B13 - B13 * B13B22);
    a3 = a / 3.0;
    
    p = b - a * a3; 
    q = (2.0 * a * a * a - 9.0 * a * b + 27.0 * c) / 27.0;   
    p2 = p * p;

    T = -q * sqrt(-27.0 * p) / 2.0 / p2;
    theta3 = acos(T) / 3.0;
    
    x0 = 2.0 * sqrt(-p / 3.0); 
    x1 = x0 * cos(theta3); 
    x2 = x0 * cos(theta3 - pi23); 
    x3 = x0 * cos(theta3 + pi23);

    rs1 = x1 - a3; 
    rs2 = x2 - a3; 
    rs3 = x3 - a3;
    
    if(rs1 < rs2) 
   	{
    	tmp = rs2; rs2 = rs1; rs1 = tmp;
    }
    if(rs1 < rs3)
    {
    	tmp = rs3; rs3 = rs1; rs1 = tmp; 
    }
    if(rs2 < rs3)
    {
    	tmp = rs2; rs2 = rs3; rs3 = tmp;
    }
    
    
    Vector3f v1(B12B23 - B13B22 + rs1 * B13, 
    	        B13B12 - B11B23 + rs1 * B23, 
    	        (rs1 - B11) * (rs1 - B22) - B12B12);
    Vector3f v2(B12B23 - B13B22 + rs2 * B13, 
    	        B13B12 - B11B23 + rs2 * B23, 
    	        (rs2 - B11) * (rs2 - B22) - B12B12);
           
    v1 = v1 / v1.norm(); 
    v2 = v2 / v2.norm();
    
    Vector3f v3(v1(1) * v2(2) - v1(2) * v2(1), 
    	        v1(2) * v2(0) - v1(0) * v2(2), 
    	        v1(0) * v2(1) - v1(1) * v2(0));
       
    d1 = sqrt(rs1); 
    d2 = sqrt(rs2); 
    d3 = sqrt(rs3);
    
    u1 = A * v1 / d1; 
    u2 = A * v2 / d2; 
    u3 = A * v3 / d3;
    
    (*UU)(0, 0) = u1.x();		(*UU)(1, 0) = u1.y();		(*UU)(2, 0) = u1.z();
    (*UU)(0, 1) = u2.x();		(*UU)(1, 1) = u2.y();		(*UU)(2, 1) = u2.z();
    (*UU)(0, 2) = u3.x();		(*UU)(1, 2) = u3.y();		(*UU)(2, 2) = u3.z();

    (*VV)(0, 0) = v1.x();		(*VV)(1, 0) = v1.y();		(*VV)(2, 0) = v1.z();
    (*VV)(0, 1) = v2.x();		(*VV)(1, 1) = v2.y();		(*VV)(2, 1) = v2.z();
    (*VV)(0, 2) = v3.x();		(*VV)(1, 2) = v3.y();		(*VV)(2, 2) = v3.z();

}


void Umeyama(const vector<Vector3f>* P,
		     const vector<Vector3f>* Q,
             Matrix3f * sigma,
		  	 Matrix3f * rRes,
		     Vector3f * tRes,
		     bool isFastSVD,
             float &time)
{
    Matrix3f * sigma_ = sigma;
    Vector3f mean_X, mean_Y;

    if(P != nullptr && Q != nullptr && sigma == nullptr)
    {
       sigma_ = new Matrix3f();
    
       int n = P->size();
       mean_X.setZero();
       mean_Y.setZero();

       for (int i = 0; i < n; ++i)
       {
          mean_X = mean_X + (*P)[i];
          mean_Y = mean_Y + (*Q)[i];
       }
       mean_X = mean_X / n;
       mean_Y = mean_Y / n;

       sigma_->setZero();

       for (int i = 0; i < n; ++i)
       {
          *sigma_ = *sigma_ + ((*Q)[i] - mean_Y) * (((*P)[i] - mean_X).transpose());
       }
       *sigma_ = *sigma_ / n;
    }


    uint64_t time1, time2;
    time1 = micros();

    if(isFastSVD)
    {
        Matrix3f U, S, V;
        S.setIdentity();
    	svd3_fast(*sigma_, &U, &V);

        if(U.determinant() * V.determinant() < 0)
            S(2, 2) = -1.0;
        *rRes = U * S * V.transpose();
    }
    else
    {
		JacobiSVD<Eigen::Matrix3f> svd(*sigma_, ComputeFullU | ComputeFullV);  
        *rRes = svd.matrixU() * svd.matrixV().transpose();
        if ((*rRes).determinant() < 0.)
        {
            Matrix3f tmpV = svd.matrixV().transpose();
            tmpV.row(2) *= -1.;
            (*rRes) = svd.matrixU() * tmpV;
        }
	}

	*tRes = mean_X - (*rRes).transpose() * mean_Y;
    
    time2 = micros();
    time = (float)(time2 - time1);

    if(P != nullptr && Q != nullptr && sigma == nullptr)
        delete sigma_;
}

