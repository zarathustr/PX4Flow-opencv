#include "eig3D.h"
#include <complex>
#include <stdint.h>

void eig3D_eig(const vector<Vector3f>* P,
	const vector<Vector3f>* Q,
	Matrix3f * sigma,
	Matrix3f * rRes,
	Vector3f * tRes,
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
	
	Matrix3f A = (*sigma_) - sigma_->transpose();
	Matrix3f tmp;
	Vector3f D(A(1, 2), A(2, 0), A(0, 1));
	Matrix4f QQ;
	QQ(0, 0) = (*sigma_)(0, 0) + (*sigma_)(1, 1) + (*sigma_)(2, 2);
	tmp = (*sigma_) + sigma_->transpose();
	tmp(0, 0) -= QQ(0, 0);    tmp(1, 1) -= QQ(0, 0);    tmp(2, 2) -= QQ(0, 0);
	QQ(0, 1) = D.x();     QQ(0, 2) = D.y();     QQ(0, 3) = D.z();
	QQ(1, 0) = D.x();     QQ(2, 0) = D.y();     QQ(3, 0) = D.z();

	QQ(1, 1) = tmp(0, 0); QQ(1, 2) = tmp(0, 1); QQ(1, 3) = tmp(0, 2);
	QQ(2, 1) = tmp(1, 0); QQ(2, 2) = tmp(1, 1); QQ(2, 3) = tmp(1, 2);
	QQ(3, 1) = tmp(2, 0); QQ(3, 2) = tmp(2, 1); QQ(3, 3) = tmp(2, 2);

	SelfAdjointEigenSolver<Matrix4f> es(QQ);
	
	Quaternionf q(es.eigenvectors().col(3));
	q.normalize();

	*rRes = q.toRotationMatrix();
	*tRes = mean_X - (*rRes).transpose() * mean_Y;
    
    time2 = micros();
    time = (float)(time2 - time1);

    if(P != nullptr && Q != nullptr && sigma == nullptr)
        delete sigma_;
}


void eig3D_symbolic(const vector<Vector3f>* P,
	const vector<Vector3f>* Q,
	Matrix3f * sigma,
	Matrix3f * rRes,
	Vector3f * tRes,
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
	
	Matrix3f A = (*sigma_) - sigma_->transpose();
	Matrix3f tmp;
	Vector3f D(A(1, 2), A(2, 0), A(0, 1));
	Matrix4f QQ;
	QQ(0, 0) = (*sigma_)(0, 0) + (*sigma_)(1, 1) + (*sigma_)(2, 2);
	tmp = (*sigma_) + sigma_->transpose();
	tmp(0, 0) -= QQ(0, 0);    tmp(1, 1) -= QQ(0, 0);    tmp(2, 2) -= QQ(0, 0);
	QQ(0, 1) = D.x();     QQ(0, 2) = D.y();     QQ(0, 3) = D.z();
	QQ(1, 0) = D.x();     QQ(2, 0) = D.y();     QQ(3, 0) = D.z();

	QQ(1, 1) = tmp(0, 0); QQ(1, 2) = tmp(0, 1); QQ(1, 3) = tmp(0, 2);
	QQ(2, 1) = tmp(1, 0); QQ(2, 2) = tmp(1, 1); QQ(2, 3) = tmp(1, 2);
	QQ(3, 1) = tmp(2, 0); QQ(3, 2) = tmp(2, 1); QQ(3, 3) = tmp(2, 2);

	float c = QQ.determinant();
        float b = - 8.0f * (sigma_->transpose()).determinant();
        float a = - 2.0f * ((*sigma_) * (sigma_->transpose())).trace();

    std::complex<float> T0(2.0f * a * a * a + 27.0f * b * b - 72.0f * a * c);
    std::complex<float> T1(pow((T0 + sqrt(- 4.0f * (a * a + 12.0f * c) * (a * a + 12.0f * c) * (a * a + 12.0f * c) + T0 * T0)), 1.0f / 3.0f));
    std::complex<float> T2(sqrt(- 4.0f * a + 2.519842099789746f * (a * a + 12.0f * c) / T1 + 1.587401051968199f * T1));

    float lambda = (0.20412414523193150818310700622549f * (T2 + sqrt(-T2 * T2 - 12.0f * a - 12.0f * 2.4494897427831780981972840747059f * b / T2))).real();
	
	float G11 = QQ(0, 0) - lambda, G12 = QQ(0, 1), G13 = QQ(0, 2), G14 = QQ(0, 3);		
	float G22 = QQ(1, 1) - lambda, G23 = QQ(1, 2), G24 = QQ(1, 3);
	float G33 = QQ(2, 2) - lambda, G34 = QQ(2, 3);
	float G44 = QQ(3, 3);

	Quaternionf qRes = Quaternionf(
		       G14 * G23 * G23 - G13 * G23 * G24 - G14 * G22 * G33 + G12 * G24 * G33 + G13 * G22 * G34 - G12 * G23 * G34,
			   G13 * G13 * G24 + G12 * G14 * G33 - G11 * G24 * G33 + G11 * G23 * G34 - G13 * G14 * G23 - G13 * G12 * G34,
	           G13 * G14 * G22 - G12 * G14 * G23 - G12 * G13 * G24 + G11 * G23 * G24 + G12 * G12 * G34 - G11 * G22 * G34, 
	           - (G13 * G13 * G22 - 2 * G12 * G13 * G23 + G11 * G23 * G23 + G12 * G12 * G33 - G11 * G22 * G33));
	qRes.normalize();

	*rRes = qRes.toRotationMatrix();
	*tRes = mean_X - (*rRes).transpose() * mean_Y;
    
    time2 = micros();
    time = (float)(time2 - time1);

    if(P != nullptr && Q != nullptr && sigma == nullptr)
        delete sigma_;
}

