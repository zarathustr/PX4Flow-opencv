#include "FA3R.h"



void FA3R(const vector<Vector3d>& P, 
	      const vector<Vector3d>& Q,
		  Matrix3d * rRes,
		  Vector3d * tRes)
{
	double NDr = 0.0, eig;
	double G11, G12, G13, G14;
	double G22, G23, G24;
	double G33, G34;
	Vector3d Db_, Dr_;
	Matrix3d MM;

	int n = P.size();
	MM.setZero();
	Db_.setZero();
	Dr_.setZero();

	for (int i = 0; i < n; ++i)
	{
		MM = MM + Q[i] * (P[i].transpose());
		NDr = NDr + Q[i].x() * Q[i].x() + Q[i].y() * Q[i].y() + Q[i].z() * Q[i].z();
		Db_ = Db_ + P[i];
		Dr_ = Dr_ + Q[i];
	}
	MM = MM / n;
	Db_ = Db_ / n;
	Dr_ = Dr_ / n;
	NDr = NDr / n;

	eig = NDr - (Dr_(0) * Dr_(0) + Dr_(1) * Dr_(1) + Dr_(2) * Dr_(2));


	G11 = MM(0, 0) + MM(1, 1) + MM(2, 2) - (Db_(0) * Dr_(0) + Db_(1) * Dr_(1) + Db_(2) * Dr_(2)) - eig;		
	G12 = - MM(1, 2) + MM(2, 1) - (Db_(1) * Dr_(2) - Db_(2) * Dr_(1));		
	G13 = - MM(2, 0) + MM(0, 2) - (Db_(2) * Dr_(0) - Db_(0) * Dr_(2));		
	G14 = - MM(0, 1) + MM(1, 0) - (Db_(0) * Dr_(1) - Db_(1) * Dr_(0));
	
	G22 = MM(0, 0) - MM(1, 1) - MM(2, 2) - (Db_(0) * Dr_(0) - Db_(1) * Dr_(1) - Db_(2) * Dr_(2)) - eig;		
	G23 = MM(0, 1) + MM(1, 0) - (Db_(0) * Dr_(1) + Db_(1) * Dr_(0));		
	G24 = MM(0, 2) + MM(2, 0) - (Db_(0) * Dr_(2) + Db_(2) * Dr_(0));

	G33 = MM(1, 1) - MM(0, 0) - MM(2, 2) - (Db_(1) * Dr_(1) - Db_(0) * Dr_(0) - Db_(2) * Dr_(2)) - eig;		
	G34 = MM(1, 2) + MM(2, 1) - (Db_(1) * Dr_(2) + Db_(2) * Dr_(1));

	Quaterniond qRes = Quaterniond(
		       G14 * G23 * G23 - G13 * G23 * G24 - G14 * G22 * G33 + G12 * G24 * G33 + G13 * G22 * G34 - G12 * G23 * G34,
			   G13 * G13 * G24 + G12 * G14 * G33 - G11 * G24 * G33 + G11 * G23 * G34 - G13 * G14 * G23 - G13 * G12 * G34,
	           G13 * G14 * G22 - G12 * G14 * G23 - G12 * G13 * G24 + G11 * G23 * G24 + G12 * G12 * G34 - G11 * G22 * G34, 
	           - (G13 * G13 * G22 - 2 * G12 * G13 * G23 + G11 * G23 * G23 + G12 * G12 * G33 - G11 * G22 * G33));
	qRes.normalize();
	
	*rRes = qRes.toRotationMatrix();
	*tRes = Dr_ - (*rRes) * Db_;
}