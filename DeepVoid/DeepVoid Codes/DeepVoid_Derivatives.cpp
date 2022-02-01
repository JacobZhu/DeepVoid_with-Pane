#include "stdafx.h"

// 20151107, ���� Huber Ȩ��
// Ĭ�ϲв� x �϶�����ֵ��������ͶӰ�в�����
double derivatives::weight_Huber(double x, double c)
{
	if (x<c)
	{
		return 1;
	}
	else
	{
		return c/x;
	}
}

// ���Գ���������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> derivatives::der_xy_fxfycxcys(const Matx33d & mR,	// input:	current rotation matrix
											   const Matx31d & mt,	// input:	current translation vector
											   const Matx31d & mX	// input:	current object coordinates
											   )
{
	Matx31d v = mR*mX+mt;

	double v3_1 = 1.0/v(2);

	double v1_v3 = v(0)*v3_1;
	double v2_v3 = v(1)*v3_1;

	Matx<double,2,5> ders;

	ders(0,0)=v1_v3;
	ders(0,2)=1;
	ders(0,4)=v2_v3;

	ders(1,1)=v2_v3;
	ders(1,3)=1;

	return ders;
}

// ���Գ���������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> derivatives::der_xy_fxfycxcys(double u, double v)
{
	Matx<double,2,5> ders;

	// [u 0 1 0 v]
	// [0 v 0 1 0]
	ders(0,0) = u;	ders(0,2) = 1;	ders(0,4) = v;
	ders(1,1) = v;	ders(1,3) = 1;

	return ders;
}

// ���Գ���������fx,fy,cx,cy��4���ڲ�����
Matx<double,2,4> derivatives::der_xy_fxfycxcy(double u, double v)
{
	Matx<double,2,4> ders;

	// [u 0 1 0]
	// [0 v 0 1]
	ders(0,0) = u;	ders(0,2) = 1;
	ders(1,1) = v;	ders(1,3) = 1;

	return ders;
}

// ���Գ���������f,cx,cy��3���ڲ�����
Matx<double,2,3> derivatives::der_xy_fcxcy(double u, double v)
{
	Matx<double,2,3> ders;

	// [u 1 0]
	// [v 0 1]
	ders(0,0) = u;	ders(0,1) = 1;
	ders(1,0) = v;	ders(1,2) = 1;

	return ders;
}

// ���Գ���������f��1���ڲ�����
Matx<double,2,1> derivatives::der_xy_f(double u, double v)
{
	Matx<double,2,1> ders;

	// [u]
	// [v]
	ders(0) = u;
	ders(1) = v;

	return ders;
}

// ���Գ����Ĺ�һ���������������Գ����������
Matx22d derivatives::der_uv_xy(double fx, double fy, double s)
{
	Matx22d ders;

	double fx_1 = 1.0/fx;
	double fy_1 = 1.0/fy;

	ders(0,0)=fx_1;	ders(0,1)=-s*fx_1*fy_1;
	ders(1,1)=fy_1;

	return ders;
}

// ���Գ������������һ�����������
Matx22d derivatives::der_xy_uv(double fx, double fy, double s)
{
	Matx22d ders;

	ders(0,0) = fx;	ders(0,1) = s;
	ders(1,1) = fy;

	return ders;
}

// ��һ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> derivatives::der_uv_fxfycxcys(double x, double y, double fx, double fy, double cx, double cy, double s)
{
	Matx<double,2,5> ders;

	double fx_1 = 1.0/fx; // 1/fx
	double fy_1 = 1.0/fy; // 1/fy

	double fx_2 = fx_1*fx_1; // 1/fx^2
	double fy_2 = fy_1*fy_1; // 1/fy^2
	double fxfy_1 = fx_1*fy_1; // 1/fxfy

	double x_cx = x-cx;
	double y_cy = y-cy;

	ders(0,0) = -(x_cx - s*y_cy*fy_1)*fx_2;
	ders(0,1) = s*y_cy*fx_1*fy_2;
	ders(0,2) = -fx_1;
	ders(0,3) = s*fxfy_1;
	ders(0,4) = -y_cy*fxfy_1;

	ders(1,1) = -y_cy*fy_2;
	ders(1,3) = -fy_1;

	return ders;
}

// ��һ�����������Թ�һ�����������
Matx22d derivatives::der_dudv_uv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx22d ders;

	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;

	ders(0,0) = 3*k1*u2 +   k1*v2 + 2*(k2+k4)*u + k5*v;
	ders(0,1) = 2*k1*uv + 2*k2*v + k5*u;
	ders(1,0) = 2*k1*uv + 2*k3*u + k4*v;
	ders(1,1) =   k1*u2 + 3*k1*v2 + 2*(k3+k5)*v + k4*u;

	return ders;
}

// ��һ�����������Թ�һ�����������
Matx22d derivatives::der_dudv_uv_brown(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx22d ders;

	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;

	double r2 = u2 + v2;
	double r4 = r2 * r2;
	double r6 = r4 * r2;

	double f1 = k1*r2 + k2*r4 + k5*r6;
	double f2 = 2*k1 + 4*k2*r2 + 6*k5*r4;

	ders(0,0) = f2*u2 + f1 + 6*k4*u + 2*k3*v;
	ders(0,1) = f2*uv + 2*k3*u + 2*k4*v;
	ders(1,0) = ders(0,1);
	ders(1,1) = f2*v2 + f1 + 6*k3*v + 2*k4*u;

	return ders;
}

// ���������Թ�һ������������
Matx22d derivatives::der_dxdy_dudv(double fx, double fy, double s)
{
	Matx22d ders;

	ders(0,0) = fx;
	ders(0,1) = s;
	ders(1,1) = fy;

	return ders;
}

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> derivatives::der_dxdy_fxfycxcys(double du, double dv)
{
	Matx<double,2,5> ders;

	// [du  0  0  0  dv]
	// [0  dv  0  0  0 ]
	ders(0,0) = du;
	ders(0,4) = dv;
	ders(1,1) = dv;

	return ders;
}

// ����������fx,fy,cx,cy,��4���ڲ�����
Matx<double,2,4> derivatives::der_dxdy_fxfycxcy(double du, double dv)
{
	Matx<double,2,4> ders;

	// [du  0  0  0]
	// [0  dv  0  0]
	ders(0,0) = du;
	ders(1,1) = dv;

	return ders;
}

// ����������f,cx,cy,��3���ڲ�����
Matx<double,2,3> derivatives::der_dxdy_fcxcy(double du, double dv)
{
	Matx<double,2,3> ders;

	// [du 0 0]
	// [dv 0 0]
	ders(0,0) = du;
	ders(1,0) = dv;

	return ders;
}

// ����������f��1���ڲ�����
Matx<double,2,1> derivatives::der_dxdy_f(double du, double dv)
{
	Matx<double,2,1> ders;

	// [du]
	// [dv]
	ders(0) = du;
	ders(1) = dv;

	return ders;
}

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> derivatives::der_dxdy_fxfycxcys_weng(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx<double,2,5> ders;

	double du,dv;

	distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);

	ders(0,0) = du;
	ders(0,4) = dv;
	ders(1,1) = dv;

	return ders;
}

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,4> derivatives::der_dxdy_fxfycxcy_weng(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx<double,2,4> ders;

	double du,dv;

	distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);

	ders(0,0) = du;
	ders(1,1) = dv;

	return ders;
}

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> derivatives::der_dxdy_fxfycxcys_brown(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx<double,2,5> ders;

	double du,dv;

	distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);

	ders(0,0) = du;
	ders(0,4) = dv;
	ders(1,1) = dv;

	return ders;
}

// ����������fx,fy,cx,cy,s��4���ڲ�����
Matx<double,2,4> derivatives::der_dxdy_fxfycxcy_brown(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx<double,2,4> ders;

	double du,dv;

	distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);

	ders(0,0) = du;
	ders(1,1) = dv;

	return ders;
}

// ���Գ�����������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
Matx<double,2,12> derivatives::der_xy_Rt(double fx, double fy, double s, const Matx33d & mR, const Matx31d & mt, const Matx31d & mX)
{
	Matx<double,2,12> ders;

	Matx31d v = mR*mX+mt;
	double v1 = v(0);
	double v2 = v(1);
	double v3 = v(2);

	double v3_1 = 1.0/v3;		// 1/v3
	double v32_1 = v3_1*v3_1;	// 1/v3^2

	double X = mX(0);
	double Y = mX(1);
	double Z = mX(2);

	double fxX = fx*X;	double fxXv1 = fxX*v1;
	double fxY = fx*Y;	double fxYv1 = fxY*v1;
	double fxZ = fx*Z;	double fxZv1 = fxZ*v1;
	double fxv1 = fx*v1;
	
	double fyX = fy*X;	double fyXv2 = fyX*v2;
	double fyY = fy*Y;	double fyYv2 = fyY*v2;
	double fyZ = fy*Z;	double fyZv2 = fyZ*v2;
	double fyv2 = fy*v2;

	double sX = s*X;	double sXv2 = sX*v2;
	double sY = s*Y;	double sYv2 = sY*v2;
	double sZ = s*Z;	double sZv2 = sZ*v2;
	double sv2 = s*v2;

	ders(0,0) = fxX*v3_1;
	ders(0,1) = fxY*v3_1;
	ders(0,2) = fxZ*v3_1;

	ders(0,3) = sX*v3_1;					ders(1,3) = fyX*v3_1;
	ders(0,4) = sY*v3_1;					ders(1,4) = fyY*v3_1;
	ders(0,5) = sZ*v3_1;					ders(1,5) = fyZ*v3_1;

	ders(0,6) = -fxXv1*v32_1-sXv2*v32_1;	ders(1,6) = -fyXv2*v32_1;
	ders(0,7) = -fxYv1*v32_1-sYv2*v32_1;	ders(1,7) = -fyYv2*v32_1;
	ders(0,8) = -fxZv1*v32_1-sZv2*v32_1;	ders(1,8) = -fyZv2*v32_1;

	ders(0,9) = fx*v3_1;
	ders(0,10)= s*v3_1;						ders(1,10)= fy*v3_1;
	ders(0,11)= -fxv1*v32_1-sv2*v32_1;		ders(1,11)= -fyv2*v32_1;

	return ders;
}

// ��һ������������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
Matx<double,2,12> derivatives::der_uv_Rt(const Matx33d & mR, const Matx31d & mt, const Matx31d & mX)
{
	Matx<double,2,12> ders;

	Matx31d v = mR*mX+mt;
	double v1 = v(0);
	double v2 = v(1);
	double v3 = v(2);

	double v3_1 = 1.0/v3;		// 1/v3
	double v32_1 = v3_1*v3_1;	// 1/v3^2

	double X = mX(0);
	double Y = mX(1);
	double Z = mX(2);

	double v1_v32_1 = v1*v32_1;
	double v2_v32_1 = v2*v32_1;

	ders(0,0) = X*v3_1;			ders(0,1) = Y*v3_1;			ders(0,2) = Z*v3_1;
	ders(0,6) = -X*v1_v32_1;	ders(0,7) = -Y*v1_v32_1;	ders(0,8) = -Z*v1_v32_1;
	ders(0,9) = v3_1;										ders(0,11)= -v1_v32_1;

	ders(1,3) = ders(0,0);		ders(1,4) = ders(0,1);		ders(1,5) = ders(0,2);
	ders(1,6) = -X*v2_v32_1;	ders(1,7) = -Y*v2_v32_1;	ders(1,8) = -Z*v2_v32_1;
								ders(1,10)= v3_1;			ders(1,11)= -v2_v32_1;

	return ders;
}

// ��һ������������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
Matx<double,2,12> derivatives::der_uv_Rt(const Matx33d & mR, const Matx31d & mt, double X, double Y, double Z, double W)
{
	Matx<double,2,12> ders;

	Matx31d mX;
	mX(0) = X;
	mX(1) = Y;
	mX(2) = Z;

	Matx31d v = mR*mX+W*mt; // [R|t][X;Y;Z;W]
	double v1 = v(0);
	double v2 = v(1);
	double v3 = v(2);

	double v3_1 = 1.0/v3;		// 1/v3
	double v32_1 = v3_1*v3_1;	// 1/v3^2

	double v1_v32_1 = v1*v32_1;
	double v2_v32_1 = v2*v32_1;

	ders(0,0) = X*v3_1;			ders(0,1) = Y*v3_1;			ders(0,2) = Z*v3_1;
	ders(0,6) = -X*v1_v32_1;	ders(0,7) = -Y*v1_v32_1;	ders(0,8) = -Z*v1_v32_1;
	ders(0,9) = W*v3_1;										ders(0,11)= -W*v1_v32_1;

	ders(1,3) = ders(0,0);		ders(1,4) = ders(0,1);		ders(1,5) = ders(0,2);
	ders(1,6) = -X*v2_v32_1;	ders(1,7) = -Y*v2_v32_1;	ders(1,8) = -Z*v2_v32_1;
	ders(1,10)= ders(0,9);		ders(1,11)= -W*v2_v32_1;

	return ders;
}

// ��һ������������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
// ���� v1 = r11 X + r12 Y + r13 Z + tx W
//		v2 = r21 X + r22 Y + r23 Z + ty W
//		v3 = r31 X + r32 Y + r33 Z + tz W
Matx<double,2,12> derivatives::der_uv_Rt(double v1, double v2, double v3, double X, double Y, double Z, double W)
{
	Matx<double,2,12> ders;

	double v3_1 = 1.0/v3;		// 1/v3
	double v32_1 = v3_1*v3_1;	// 1/v3^2

	double v1_v32_1 = v1*v32_1;
	double v2_v32_1 = v2*v32_1;

	ders(0,0) = X*v3_1;			ders(0,1) = Y*v3_1;			ders(0,2) = Z*v3_1;
	ders(0,6) = -X*v1_v32_1;	ders(0,7) = -Y*v1_v32_1;	ders(0,8) = -Z*v1_v32_1;
	ders(0,9) = W*v3_1;										ders(0,11)= -W*v1_v32_1;

	ders(1,3) = ders(0,0);		ders(1,4) = ders(0,1);		ders(1,5) = ders(0,2);
	ders(1,6) = -X*v2_v32_1;	ders(1,7) = -Y*v2_v32_1;	ders(1,8) = -Z*v2_v32_1;
	ders(1,10)= ders(0,9);		ders(1,11)= -W*v2_v32_1;

	return ders;
}

// ��һ������������ת���� R ��Ԫ���Լ��������� c ��
// ���� v1 = r11 (X-Cx) + r12 (Y-Cy) + r13 (Z-Cz)
//		v2 = r21 (X-Cx) + r22 (Y-Cy) + r23 (Z-Cz)
//		v3 = r31 (X-Cx) + r32 (Y-Cy) + r33 (Z-Cz)
// Matx<double,2,12> derivatives::der_uv_Rc(const Matx33d & R, double u, double v, double v1, double v2, double v3, double X, double Y, double Z, double Cx, double Cy, double Cz)
// {
// 	Matx<double,2,12> ders;
// 
// 	double v3_1 = 1.0/v3;		// 1/v3
// 	double v32_1 = v3_1*v3_1;	// 1/v3^2
// 
// 	// row 1
// 	ders(0,0) = (X-Cx)*v3_1;
// 	ders(0,1) = (Y-Cy)*v3_1;
// 	ders(0,2) = (Z-Cz)*v3_1;
// 
// 	ders(0,6) = -ders(0,0)*u;
// 	ders(0,7) = -ders(0,1)*u;
// 	ders(0,8) = -ders(0,2)*u;
// 
// 	ders(0,9) = (-R(0,0)*v3 + R(2,0)*v1)*v32_1;
// 	ders(0,10)= (-R(0,1)*v3 + R(2,1)*v1)*v32_1;
// 	ders(0,11)= (-R(0,2)*v3 + R(2,2)*v1)*v32_1;
// 
// 	// row 2
// 	ders(1,3) = ders(0,0);
// 	ders(1,4) = ders(0,1);
// 	ders(1,5) = ders(0,2);
// 
// 	ders(1,6) = -ders(0,0)*v;
// 	ders(1,7) = -ders(0,1)*v;
// 	ders(1,8) = -ders(0,2)*v;
// 
// 	ders(1,9) = (-R(1,0)*v3 + R(2,0)*v2)*v32_1;
// 	ders(1,10)= (-R(1,1)*v3 + R(2,1)*v2)*v32_1;
// 	ders(1,11)= (-R(1,2)*v3 + R(2,2)*v2)*v32_1;
// 	
// 	return ders;
// }

// ��һ������������ת���� R ��Ԫ���Լ��������� c ��
// ���� v1 = r11 (X-WCx) + r12 (Y-WCy) + r13 (Z-WCz)
//		v2 = r21 (X-WCx) + r22 (Y-WCy) + r23 (Z-WCz)
//		v3 = r31 (X-WCx) + r32 (Y-WCy) + r33 (Z-WCz)
Matx<double,2,12> derivatives::der_uv_Rc(const Matx33d & R, double u, double v, double v1, double v2, double v3, double X, double Y, double Z, double W, double Cx, double Cy, double Cz)
{
	Matx<double,2,12> ders;

	double v3_1 = 1.0/v3;		// 1/v3
	double v32_1 = v3_1*v3_1;	// 1/v3^2

	// row 1
	ders(0,0) = (X-W*Cx)*v3_1;
	ders(0,1) = (Y-W*Cy)*v3_1;
	ders(0,2) = (Z-W*Cz)*v3_1;

	ders(0,6) = -ders(0,0)*u;
	ders(0,7) = -ders(0,1)*u;
	ders(0,8) = -ders(0,2)*u;

	ders(0,9) = (-R(0,0)*v3 + R(2,0)*v1)*v32_1*W;
	ders(0,10)= (-R(0,1)*v3 + R(2,1)*v1)*v32_1*W;
	ders(0,11)= (-R(0,2)*v3 + R(2,2)*v1)*v32_1*W;

	// row 2
	ders(1,3) = ders(0,0);
	ders(1,4) = ders(0,1);
	ders(1,5) = ders(0,2);

	ders(1,6) = -ders(0,0)*v;
	ders(1,7) = -ders(0,1)*v;
	ders(1,8) = -ders(0,2)*v;

	ders(1,9) = (-R(1,0)*v3 + R(2,0)*v2)*v32_1*W;
	ders(1,10)= (-R(1,1)*v3 + R(2,1)*v2)*v32_1*W;
	ders(1,11)= (-R(1,2)*v3 + R(2,2)*v2)*v32_1*W;

	return ders;
}

// ��һ���������Կռ���������XYZW��
// ���� v1 = r11 X + r12 Y + r13 Z + tx W
//		v2 = r21 X + r22 Y + r23 Z + ty W
//		v3 = r31 X + r32 Y + r33 Z + tz W
Matx<double,2,4> derivatives::der_uv_XYZW(const Matx33d & R, const Matx31d & t, double v1, double v2, double v3)
{
	Matx<double,2,4> ders;

	double v32_1 = 1.0/(v3*v3);

	ders(0,0) = (R(0,0)*v3 - R(2,0)*v1)*v32_1;
	ders(0,1) = (R(0,1)*v3 - R(2,1)*v1)*v32_1;
	ders(0,2) = (R(0,2)*v3 - R(2,2)*v1)*v32_1;
	ders(0,3) = (t(0)*v3 - t(2)*v1)*v32_1;

	ders(1,0) = (R(1,0)*v3 - R(2,0)*v2)*v32_1;
	ders(1,1) = (R(1,1)*v3 - R(2,1)*v2)*v32_1;
	ders(1,2) = (R(1,2)*v3 - R(2,2)*v2)*v32_1;
	ders(1,3) = (t(1)*v3 - t(2)*v2)*v32_1;

	return ders;
}

// 20170808, for sparse LM of fundamental matrix
// ���Գ��������Կռ���������XYZW��
// ���� f1 = P11 X + P12 Y + P13 Z + P14 W
//      f2 = P21 X + P22 Y + P23 Z + P24 W
//      g  = P31 X + P32 Y + P33 Z + P34 W
Matx<double, 2, 4> derivatives::der_xy_XYZW(const Matx34d & P, double f1, double f2, double g)
{
	Matx<double, 2, 4> ders;

	double g2_1 = 1.0 / (g*g);

	ders(0, 0) = (P(0, 0)*g - P(2, 0)*f1)*g2_1;
	ders(0, 1) = (P(0, 1)*g - P(2, 1)*f1)*g2_1;
	ders(0, 2) = (P(0, 2)*g - P(2, 2)*f1)*g2_1;
	ders(0, 3) = (P(0, 3)*g - P(2, 3)*f1)*g2_1;

	ders(1, 0) = (P(1, 0)*g - P(2, 0)*f2)*g2_1;
	ders(1, 1) = (P(1, 1)*g - P(2, 1)*f2)*g2_1;
	ders(1, 2) = (P(1, 2)*g - P(2, 2)*f2)*g2_1;
	ders(1, 3) = (P(1, 3)*g - P(2, 3)*f2)*g2_1;

	return ders;
}

// 20170812, for sparse LM of fundamental matrix
// ���Գ��������Կռ���������XYW�󵼣�Z����̶�Ϊ1��������Ϊ 0
// ���� f1 = P11 X + P12 Y + P13 Z + P14 W
//      f2 = P21 X + P22 Y + P23 Z + P24 W
//      g  = P31 X + P32 Y + P33 Z + P34 W
Matx<double, 2, 3> derivatives::der_xy_XYW(const Matx34d & P, double f1, double f2, double g)
{
	Matx<double, 2, 3> ders;

	double g2_1 = 1.0 / (g*g);

	ders(0, 0) = (P(0, 0)*g - P(2, 0)*f1)*g2_1;
	ders(0, 1) = (P(0, 1)*g - P(2, 1)*f1)*g2_1;
	ders(0, 2) = (P(0, 3)*g - P(2, 3)*f1)*g2_1;

	ders(1, 0) = (P(1, 0)*g - P(2, 0)*f2)*g2_1;
	ders(1, 1) = (P(1, 1)*g - P(2, 1)*f2)*g2_1;
	ders(1, 2) = (P(1, 3)*g - P(2, 3)*f2)*g2_1;

	return ders;
}

// 20170808, for sparse LM of fundamental matrix
// ���Գ���������ͶӰ�����12��Ԫ����
// ���� f1 = P11 X + P12 Y + P13 Z + P14 W
//      f2 = P21 X + P22 Y + P23 Z + P24 W
//      g  = P31 X + P32 Y + P33 Z + P34 W
Matx<double, 2, 12> derivatives::der_xy_P(double X, double Y, double Z, double W, double f1, double f2, double g)
{
	Matx<double, 2, 12> ders;

	double g_1 = 1.0 / g;
	double x = f1*g_1;
	double y = f2*g_1;

	ders(0, 0) = ders(1, 4) = X*g_1;
	ders(0, 1) = ders(1, 5) = Y*g_1;
	ders(0, 2) = ders(1, 6) = Z*g_1;
	ders(0, 3) = ders(1, 7) = W*g_1;

	ders(0, 8)  = -ders(0, 0)*x;
	ders(0, 9)  = -ders(0, 1)*x;
	ders(0, 10) = -ders(0, 2)*x;
	ders(0, 11) = -ders(0, 3)*x;

	ders(1, 8)  = -ders(0, 0)*y;
	ders(1, 9)  = -ders(0, 1)*y;
	ders(1, 10) = -ders(0, 2)*y;
	ders(1, 11) = -ders(0, 3)*y;

	return ders;
}

// ��һ���������Կռ������XYZ��
// ���� v1 = r11 X + r12 Y + r13 Z + tx
//		v2 = r21 X + r22 Y + r23 Z + ty
//		v3 = r31 X + r32 Y + r33 Z + tz
Matx<double,2,3> derivatives::der_uv_XYZ(const Matx33d & R, double v1, double v2, double v3)
{
	Matx<double,2,3> ders;

	double v32_1 = 1.0/(v3*v3);

	ders(0,0) = (R(0,0)*v3 - R(2,0)*v1)*v32_1;
	ders(0,1) = (R(0,1)*v3 - R(2,1)*v1)*v32_1;
	ders(0,2) = (R(0,2)*v3 - R(2,2)*v1)*v32_1;

	ders(1,0) = (R(1,0)*v3 - R(2,0)*v2)*v32_1;
	ders(1,1) = (R(1,1)*v3 - R(2,1)*v2)*v32_1;
	ders(1,2) = (R(1,2)*v3 - R(2,2)*v2)*v32_1;

	return ders;
}

// ��ת����R��Ԫ�ض���ת����w��Ԫ���󵼣�����R=dR*R0��dR=I+[w]�ķ�ʽ��õ�
Matx<double,9,3> derivatives::der_R_w(const Matx33d & mR)
{
	Matx<double,9,3> ders;

	ders(0,1) = mR(2,0);	ders(0,2) = -mR(1,0);
	ders(1,1) = mR(2,1);	ders(1,2) = -mR(1,1);
	ders(2,1) = mR(2,2);	ders(2,2) = -mR(1,2);

	ders(3,0) = -mR(2,0);	ders(3,2) = mR(0,0);
	ders(4,0) = -mR(2,1);	ders(4,2) = mR(0,1);
	ders(5,0) = -mR(2,2);	ders(5,2) = mR(0,2);

	ders(6,0) = mR(1,0);	ders(6,1) = -mR(0,0);
	ders(7,0) = mR(1,1);	ders(7,1) = -mR(0,1);
	ders(8,0) = mR(1,2);	ders(8,1) = -mR(0,2);

	return ders;
}

// ����任����[R|t;0 1]��Ԫ�ض���ת����w��Ԫ���Լ�ƽ��������Ԫ���󵼣�����R=dR*R0��dR=I+[w]�ķ�ʽ��õ�
Matx<double,12,6> derivatives::der_Rt_wt(const Matx33d & mR)
{
	Matx<double,12,6> ders;

	ders(0,1) = mR(2,0);	ders(0,2) = -mR(1,0);
	ders(1,1) = mR(2,1);	ders(1,2) = -mR(1,1);
	ders(2,1) = mR(2,2);	ders(2,2) = -mR(1,2);

	ders(3,0) = -mR(2,0);	ders(3,2) = mR(0,0);
	ders(4,0) = -mR(2,1);	ders(4,2) = mR(0,1);
	ders(5,0) = -mR(2,2);	ders(5,2) = mR(0,2);

	ders(6,0) = mR(1,0);	ders(6,1) = -mR(0,0);
	ders(7,0) = mR(1,1);	ders(7,1) = -mR(0,1);
	ders(8,0) = mR(1,2);	ders(8,1) = -mR(0,2);

	ders(9,3) = ders(10,4) = ders(11,5) = 1;

	return ders;
}

// ��ת���� R �Լ��������� c ����ת���� w ��Ԫ���Լ��������� c ��Ԫ���󵼣�����R=dR*R0��dR=I+[w]�ķ�ʽ��õ�
Matx<double,12,6> derivatives::der_Rc_wc(const Matx33d & mR)
{
	Matx<double,12,6> ders;

	ders(0,1) = mR(2,0);	ders(0,2) = -mR(1,0);
	ders(1,1) = mR(2,1);	ders(1,2) = -mR(1,1);
	ders(2,1) = mR(2,2);	ders(2,2) = -mR(1,2);

	ders(3,0) = -mR(2,0);	ders(3,2) = mR(0,0);
	ders(4,0) = -mR(2,1);	ders(4,2) = mR(0,1);
	ders(5,0) = -mR(2,2);	ders(5,2) = mR(0,2);

	ders(6,0) = mR(1,0);	ders(6,1) = -mR(0,0);
	ders(7,0) = mR(1,1);	ders(7,1) = -mR(0,1);
	ders(8,0) = mR(1,2);	ders(8,1) = -mR(0,2);

	ders(9,3) = ders(10,4) = ders(11,5) = 1;

	return ders;
}

// ��һ������������weng's���ϵ����
Matx<double,2,5> derivatives::der_dudv_ki_weng(double u, double v)
{
	Matx<double,2,5> ders;

	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;
	double r2 = u2+v2;

	ders(0,0) = r2*u;	ders(0,1) = r2;	ders(0,3) = u2; ders(0,4) = uv;
	ders(1,0) = r2*v;	ders(1,2) = r2; ders(1,3) = uv; ders(1,4) = v2;

	return ders;
}

// ��һ������������brown's���ϵ����
Matx<double,2,5> derivatives::der_dudv_ki_brown(double u, double v)
{
	Matx<double,2,5> ders;

	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;
	double r2 = u2+v2;
	double r4 = r2*r2;
	double r6 = r4*r2;

	ders(0,0) = r2*u;	ders(0,1) = r4*u;	ders(0,2) = 2*uv;	ders(0,3) = r2+2*u2;	ders(0,4) = r6*u;	
	ders(1,0) = r2*v;	ders(1,1) = r4*v;	ders(1,2) = r2+2*v2;	ders(1,3) = 2*uv;	ders(1,4) = r6*v;	

	return ders;
}

// constraint: �̶���ǰͼ��ĳͼ֮��Ļ��߳�Ϊĳֵ: ||Rjck+tj||^2 - d^2 = 0
// ��Լ�����̶Ե�ǰͼ����ת�����Լ�ƽ����������
Matx<double,1,12> derivatives::der_c_fixedBaseline_Rt(double Xkw,double Ykw, double Zkw,	// ͼ k �Ĺ���������ϵ�е�����
													  double Xkj,double Ykj, double Zkj		// ͼ k �Ĺ����ڵ�ǰͼ��ϵ�е�����
													  )
{
	Matx<double,1,12> ders;

	double Xkj2 = 2*Xkj;
	double Ykj2 = 2*Ykj;
	double Zkj2 = 2*Zkj;

	ders(0) = Xkj2*Xkw;
	ders(1) = Xkj2*Ykw;
	ders(2) = Xkj2*Zkw;

	ders(3) = Ykj2*Xkw;
	ders(4) = Ykj2*Ykw;
	ders(5) = Ykj2*Zkw;

	ders(6) = Zkj2*Xkw;
	ders(7) = Zkj2*Ykw;
	ders(8) = Zkj2*Zkw;

	ders(9)  = Xkj2;
	ders(10) = Ykj2;
	ders(11) = Zkj2;

	return ders;
}

// Matx<double,2,16> derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(double X, double Y, double Z,	// �ÿ��Ƶ������
// 																 double x, double y,			// �ÿ��Ƶ��ʵ�ʹ۲��������
// 																 const Matx33d & mK,			// ��ǰ���ڲ�������
// 																 const Matx33d & mR,			// ��ǰ����̬����
// 																 const Matx31d & mt,			// ��ǰ��ƽ����������
// 																 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
// 																 double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
// 																 )
// {
// 	Matx31d mX;
// 	mX(0)=X; mX(1)=Y; mX(2)=Z;
// 
// 	Matx31d mx = mR*mX+mt;
// 	double mx2_1 = 1.0/mx(2);
// 
// 	// ���ȵõ���һ���������
// 	mx(0)*=mx2_1;
// 	mx(1)*=mx2_1;
// 	mx(2)=1;
// 
// 	double u = mx(0);
// 	double v = mx(1);
// 
// 	// ��ͨ���ڲ����õ����Գ��������
// 	mx = mK*mx;
// 	double x0 = mx(0);
// 	double y0 = mx(1);
// 
// 	double fx = mK(0,0);	double fy = mK(1,1);
// 	double cx = mK(0,2);	double cy = mK(1,2);
// 	double s  = mK(0,1);	
// 
// 	// ���������
// 	double ddx,ddy;
// 	distortions::dxdy_weng(fx,fy,s,u,v,k1,k2,k3,k4,k5,ddx,ddy);
// 
// 	dx = x0+ddx-x;
// 	dy = y0+ddy-y;
// 
// 
// 	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_xy_fxfycxcys = der_xy_fxfycxcys(mR,mt,mX);
// 	
// 	Matx<double,2,5> j_dxdy_fxfycxcys0 = der_dxdy_fxfycxcys_weng(u,v,k1,k2,k3,k4,k5);
// 
// 	Matx22d j_dxdy_dudv = der_dxdy_dudv(fx,fy,s);
// 	Matx22d j_dudv_uv = der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
// 	Matx22d j_dxdy_uv = j_dxdy_dudv*j_dudv_uv;
// 	Matx<double,2,5> j_uv_fxfycxcys = der_uv_fxfycxcys(x0,y0,fx,fy,cx,cy,s);
// 	Matx<double,2,5> j_dxdy_fxfycxcys1 = j_dxdy_uv*j_uv_fxfycxcys;
// 
// 	Matx22d j_uv_xy = der_uv_xy(fx,fy,s);
// 	Matx22d j_dxdy_xy = j_dxdy_uv*j_uv_xy;
// 	Matx<double,2,5> j_dxdy_fxfycxcys2 = j_dxdy_xy*j_xy_fxfycxcys;
// 
// 	Matx<double,2,5> j_f_fxfycxcys = j_xy_fxfycxcys + j_dxdy_fxfycxcys0 + j_dxdy_fxfycxcys1 + j_dxdy_fxfycxcys2;
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,12> j_xy_Rt = der_xy_Rt(fx,fy,s,mR,mt,mX);
// 	Matx<double,12,6> j_Rt_wt = der_Rt_wt(mR);
// 	Matx<double,2,6> j_xy_wt = j_xy_Rt*j_Rt_wt;
// 
// 	Matx<double,2,6> j_dxdy_wt = j_dxdy_xy*j_xy_wt;
// 
// 	Matx<double,2,6> j_f_wt = j_xy_wt + j_dxdy_wt;
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki = der_dudv_ki_weng(u,v);
// 	Matx<double,2,5> j_f_ki = j_dxdy_dudv*j_dudv_ki;
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 	Matx<double,2,16> jacob;
// 	for (int i=0;i<2;++i)
// 	{
// 		for (int j=0;j<5;++j)
// 		{
// 			jacob(i,j)=j_f_fxfycxcys(i,j);
// 		}
// 
// 		for (int j=0;j<6;++j)
// 		{
// 			jacob(i,j+5)=j_f_wt(i,j);
// 		}
// 
// 		for (int j=0;j<5;++j)
// 		{
// 			jacob(i,j+11)=j_f_ki(i,j);
// 		}
// 	}
// 
// 	return jacob;
// }

Matx<double,2,16> derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
															double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
															const Matx33d & mK,				// ��ǰ���ڲ�������
															const Matx33d & mR,				// ��ǰ����̬����
															const Matx31d & mt,				// ��ǰ��ƽ����������
															double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
															int distType,					// ������ͣ�0 weng's, 1 brown's
															double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
															)
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv
	
	// ������ͶӰ�в���
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_xy_fxfycxcys = der_xy_fxfycxcys(u,v); // dx/da

	Matx<double,2,5> j_dxdy_fxfycxcys = der_dxdy_fxfycxcys(du,dv);	

	Matx<double,2,5> j_f_fxfycxcys = j_xy_fxfycxcys + j_dxdy_fxfycxcys; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}
	
	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,16> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<5;++j)
		{
			jacob(i,j)=j_f_fxfycxcys(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+5)=j_f_wt(i,j);
		}

		for (int j=0;j<5;++j)
		{
			jacob(i,j+11)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,15> derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
														   double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
														   const Matx33d & mK,				// ��ǰ���ڲ�������
														   const Matx33d & mR,				// ��ǰ����̬����
														   const Matx31d & mt,				// ��ǰ��ƽ����������
														   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
														   int distType,					// ������ͣ�0 weng's, 1 brown's
														   double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
														   )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_xy_fxfycxcy = der_xy_fxfycxcy(u,v); // dx/da

	Matx<double,2,4> j_dxdy_fxfycxcy = der_dxdy_fxfycxcy(du,dv);
	
	Matx<double,2,4> j_f_fxfycxcy = j_xy_fxfycxcy + j_dxdy_fxfycxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}
	
	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}
	
	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,15> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<4;++j)
		{
			jacob(i,j)=j_f_fxfycxcy(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+4)=j_f_wt(i,j);
		}

		for (int j=0;j<5;++j)
		{
			jacob(i,j+10)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,14> derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
													    double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
													    const Matx33d & mK,				// ��ǰ���ڲ�������
													    const Matx33d & mR,				// ��ǰ����̬����
													    const Matx31d & mt,				// ��ǰ��ƽ����������
													    double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
													    int distType,					// ������ͣ�0 weng's, 1 brown's
													    double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
													    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da

	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);

	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}

	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,14> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<3;++j)
		{
			jacob(i,j)=j_f_fcxcy(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+3)=j_f_wt(i,j);
		}

		for (int j=0;j<5;++j)
		{
			jacob(i,j+9)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,12> derivatives::j_f_f_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
												    double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
												    const Matx33d & mK,				// ��ǰ���ڲ�������
												    const Matx33d & mR,				// ��ǰ����̬����
												    const Matx31d & mt,				// ��ǰ��ƽ����������
												    double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
												    int distType,					// ������ͣ�0 weng's, 1 brown's
												    double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
												    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,1> j_xy_f = der_xy_f(u,v); // dx/da

	Matx<double,2,1> j_dxdy_f = der_dxdy_f(du,dv);

	Matx<double,2,1> j_f_f = j_xy_f + j_dxdy_f; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}

	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,12> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<1;++j)
		{
			jacob(i,j)=j_f_f(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+1)=j_f_wt(i,j);
		}

		for (int j=0;j<5;++j)
		{
			jacob(i,j+7)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,11> derivatives::j_f_fxfycxcy_w_t_k1(double X, double Y, double Z,	// �ÿ��Ƶ������
												   double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
												   const Matx33d & mK,				// ��ǰ���ڲ�������
												   const Matx33d & mR,				// ��ǰ����̬����
												   const Matx31d & mt,				// ��ǰ��ƽ����������
												   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
												   int distType,					// ������ͣ�0 weng's, 1 brown's
												   double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
												   )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_xy_fxfycxcy = der_xy_fxfycxcy(u,v); // dx/da

	Matx<double,2,4> j_dxdy_fxfycxcy = der_dxdy_fxfycxcy(du,dv);

	Matx<double,2,4> j_f_fxfycxcy = j_xy_fxfycxcy + j_dxdy_fxfycxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}

	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,11> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<4;++j)
		{
			jacob(i,j)=j_f_fxfycxcy(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+4)=j_f_wt(i,j);
		}

		for (int j=0;j<1;++j)
		{
			jacob(i,j+10)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,10> derivatives::j_f_fcxcy_w_t_k1(double X, double Y, double Z,	// �ÿ��Ƶ������
											    double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
											    const Matx33d & mK,				// ��ǰ���ڲ�������
											    const Matx33d & mR,				// ��ǰ����̬����
											    const Matx31d & mt,				// ��ǰ��ƽ����������
											    double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
											    int distType,					// ������ͣ�0 weng's, 1 brown's
											    double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
											    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da

	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);

	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}

	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,10> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<3;++j)
		{
			jacob(i,j)=j_f_fcxcy(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+3)=j_f_wt(i,j);
		}

		for (int j=0;j<1;++j)
		{
			jacob(i,j+9)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,12> derivatives::j_f_fxfycxcy_w_t_k1k2(double X, double Y, double Z,	// �ÿ��Ƶ������
													 double x, double y,			// �ÿ��Ƶ��ʵ�ʹ۲��������
													 const Matx33d & mK,			// ��ǰ���ڲ�������
													 const Matx33d & mR,			// ��ǰ����̬����
													 const Matx31d & mt,			// ��ǰ��ƽ����������
													 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
													 int distType,					// ������ͣ�0 weng's, 1 brown's
													 double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
													 )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_xy_fxfycxcy = der_xy_fxfycxcy(u,v); // dx/da

	Matx<double,2,4> j_dxdy_fxfycxcy = der_dxdy_fxfycxcy(du,dv);

	Matx<double,2,4> j_f_fxfycxcy = j_xy_fxfycxcy + j_dxdy_fxfycxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}

	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,12> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<4;++j)
		{
			jacob(i,j)=j_f_fxfycxcy(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+4)=j_f_wt(i,j);
		}

		for (int j=0;j<2;++j)
		{
			jacob(i,j+10)=j_f_ki(i,j);
		}
	}

	return jacob;
}

Matx<double,2,11> derivatives::j_f_fcxcy_w_t_k1k2(double X, double Y, double Z,	// �ÿ��Ƶ������
												  double x, double y,			// �ÿ��Ƶ��ʵ�ʹ۲��������
												  const Matx33d & mK,			// ��ǰ���ڲ�������
												  const Matx33d & mR,			// ��ǰ����̬����
												  const Matx31d & mt,			// ��ǰ��ƽ����������
												  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
												  int distType,					// ������ͣ�0 weng's, 1 brown's
												  double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
												  )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// ���ȵõ���һ���������
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da

	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);

	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;

	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_dudv_ki;
	if (0==distType)
	{
		j_dudv_ki = der_dudv_ki_weng(u,v);
	} 
	else
	{
		j_dudv_ki = der_dudv_ki_brown(u,v);
	}

	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Matx<double,2,11> jacob;
	for (int i=0;i<2;++i)
	{
		for (int j=0;j<3;++j)
		{
			jacob(i,j)=j_f_fcxcy(i,j);
		}

		for (int j=0;j<6;++j)
		{
			jacob(i,j+3)=j_f_wt(i,j);
		}

		for (int j=0;j<2;++j)
		{
			jacob(i,j+9)=j_f_ki(i,j);
		}
	}

	return jacob;
}

void derivatives::j_f_w_t_XYZW(double X, double Y, double Z, double W,	// �ÿ��Ƶ������
							   double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
							   const Matx33d & mK,						// ��ǰ���ڲ�������
							   const Matx33d & mR,						// ��ǰ����̬����
							   const Matx31d & mt,						// ��ǰ��ƽ����������
							   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
							   int distType,							// ������ͣ�0 weng's, 1 brown's
							   Matx<double,2,6> & A,					// ��ͶӰ�����������������ĵ�
							   Matx<double,2,4> & B,					// ��ͶӰ�������Կռ������XYZW��ĵ�
							   double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
							   )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(V1,V2,V3,X,Y,Z,W);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rt = j_f_uv * j_uv_Rt;

	A = j_f_Rt * der_Rt_wt(mR); // A = j_f_wt
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Կռ�������� ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_uv_XYZW = der_uv_XYZW(mR,mt,V1,V2,V3);

	B = j_f_uv * j_uv_XYZW; // B = j_f_XYZW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_w_t_XYZ(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
							  double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
							  const Matx33d & mK,						// ��ǰ���ڲ�������
							  const Matx33d & mR,						// ��ǰ����̬����
							  const Matx31d & mt,						// ��ǰ��ƽ����������
							  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
							  int distType,								// ������ͣ�0 weng's, 1 brown's
							  Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
							  Matx<double,2,3> & B,						// ��ͶӰ�������Կռ������ XYZ ��ĵ�
							  double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
							  )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(V1,V2,V3,X,Y,Z,W);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rt = j_f_uv * j_uv_Rt;

	A = j_f_Rt * der_Rt_wt(mR); // A = j_f_wt
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Կռ�������� ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	B = j_f_uv * j_uv_XYZ; // B = j_f_XYZ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_w_c_XYZW(double X, double Y, double Z, double W,		// �ÿ��Ƶ���������
							   double x, double y,							// �ÿ��Ƶ��ʵ�ʹ۲��������
							   const Matx33d & mK,							// ��ǰ���ڲ�������
							   const Matx33d & mR,							// ��ǰ����̬����
							   const Matx31d & mc,							// ��ǰ�Ĺ����������
							   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
							   int distType,								// ������ͣ�0 weng's, 1 brown's
							   Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
							   Matx<double,2,4> & B,						// ��ͶӰ�������Կռ������ XYZW ��ĵ�
							   double & dx, double & dy						// ��ǰ�����µ���ͶӰ�в�
							   )
{
	Matx31d mt = -mR*mc; // ����ƽ������ t = -RC

	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*(mX-W*mc);

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rc = der_uv_Rc(mR,u,v,V1,V2,V3,X,Y,Z,W,mc(0),mc(1),mc(2));

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rc = j_f_uv * j_uv_Rc;

	A = j_f_Rc * der_Rc_wc(mR); // A = j_f_wc
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Կռ�������� ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_uv_XYZW = der_uv_XYZW(mR,mt,V1,V2,V3);

	B = j_f_uv * j_uv_XYZW; // B = j_f_XYZW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_w_c_XYZ(double X, double Y, double Z, double W,	// �ÿ��Ƶ��������꣬�˴� W ӦΪ 1
							  double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
							  const Matx33d & mK,						// ��ǰ���ڲ�������
							  const Matx33d & mR,						// ��ǰ����̬����
							  const Matx31d & mc,						// ��ǰ�Ĺ����������
							  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
							  int distType,								// ������ͣ�0 weng's, 1 brown's
							  Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
							  Matx<double,2,3> & B,						// ��ͶӰ�������Կռ������ XYZ ��ĵ�
							  double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
							  )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*(mX-W*mc);

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rc = der_uv_Rc(mR,u,v,V1,V2,V3,X,Y,Z,W,mc(0),mc(1),mc(2));

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rc = j_f_uv * j_uv_Rc;

	A = j_f_Rc * der_Rc_wc(mR); // A = j_f_wc
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Կռ�������� ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	B = j_f_uv * j_uv_XYZ; // B = j_f_XYZ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_f_w_t_XYZW(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
								 double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
								 const Matx33d & mK,						// ��ǰ���ڲ�������
								 const Matx33d & mR,						// ��ǰ����̬����
								 const Matx31d & mt,						// ��ǰ��ƽ����������
								 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
								 int distType,								// ������ͣ�0 weng's, 1 brown's
								 Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
								 Matx<double,2,4> & B,						// ��ͶӰ�������Կռ������XYZW��ĵ�
								 Matx<double,2,1> & C,						// ��ͶӰ��������һ����Ч���� f ��
								 double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
								 )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,1> j_xy_f = der_xy_f(u,v); // dx/da

	Matx<double,2,1> j_dxdy_f = der_dxdy_f(du,dv);

	C = j_xy_f + j_dxdy_f; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(V1,V2,V3,X,Y,Z,W);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rt = j_f_uv * j_uv_Rt;

	A = j_f_Rt * der_Rt_wt(mR); // A = j_f_wt
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Կռ�������� ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_uv_XYZW = der_uv_XYZW(mR,mt,V1,V2,V3);

	B = j_f_uv * j_uv_XYZW; // B = j_f_XYZW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_f_w_c_XYZ(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
							    double x, double y,							// �ÿ��Ƶ��ʵ�ʹ۲��������
							    const Matx33d & mK,							// ��ǰ���ڲ�������
							    const Matx33d & mR,							// ��ǰ����̬����
							    const Matx31d & mc,							// ��ǰ�Ĺ����������
							    double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
							    int distType,								// ������ͣ�0 weng's, 1 brown's
							    Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
							    Matx<double,2,3> & B,						// ��ͶӰ�������Կռ������XYZ��ĵ�
							    Matx<double,2,1> & C,						// ��ͶӰ��������һ����Ч���� f ��
							    double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
							    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*(mX-W*mc);

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,1> j_xy_f = der_xy_f(u,v); // dx/da

	Matx<double,2,1> j_dxdy_f = der_dxdy_f(du,dv);

	C = j_xy_f + j_dxdy_f; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rc = der_uv_Rc(mR,u,v,V1,V2,V3,X,Y,Z,W,mc(0),mc(1),mc(2));

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rc = j_f_uv * j_uv_Rc;

	A = j_f_Rc * der_Rc_wc(mR); // A = j_f_wc
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Կռ�������� ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	B = j_f_uv * j_uv_XYZ; // B = j_f_XYZ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// ������ȵ� BA �����У�һ���ɵ���������� xij �������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void derivatives::j_f_w_t_w0_t0_d0(double d0,				// ���룺��ǰ������������ο�ͼ�����ȹ���
								   double x, double y,		// ���룺������ڵ�ǰ����ͼ���еĹ۲��������
								   const Matx33d & mK,		// ���룺��ǰ����ͼ����ڲ�������
								   const Matx33d & mR,		// ���룺��ǰ����ͼ�����̬����
								   const Matx31d & mt,		// ���룺��ǰ����ͼ���ƽ����������
								   double k1,double k2,double k3, double k4, double k5,	// ���룺��ǰ����ͼ������ϵ������
								   int distType,			// ���룺��ǰ����ͼ���������ͣ�0 weng's, 1 brown's
								   const Matx31d & nx0,		// ���룺���������ο�ͼ��ϵ�еĹ�һ��������꣬��������������߷���
								   const Matx33d & mR0,		// ���룺��ǰ�ο�ͼ�����̬����
								   const Matx31d & mt0,		// ���룺��ǰ�ο�ͼ���ƽ����������
								   Matx<double,2,6> & A,	// �������ͶӰ�������Գ���ͼ�������ĵ�
								   Matx<double,2,1> & B,	// �������ͶӰ���������������ĵ�
								   Matx<double,2,6> & A0,	// �������ͶӰ�������Բο�ͼ�������ĵ�
								   double & dx, double & dy	// �������ǰ�����µ���ͶӰ�в�
								   )
{
	Matx31d mX;

	Matx33d mR0t = mR0.t();
	Matx31d mtmp31 = d0*nx0-mt0;

	// ���ڵ�ǰ����ڲο�ͼ����ȹ����Լ��ο�ͼ��ǰ�Ĳ����������õ�������������
	mX = mR0t*mtmp31;

	double X = mX(0);
	double Y = mX(1);
	double Z = mX(2);
	double W = 1;

	// �û���������ɵ���������ٷֱ�� R0, t0, d0 ��
	Matx<double,3,12> j_XYZ_R0t0;
	j_XYZ_R0t0(0,0) = j_XYZ_R0t0(1,1) = j_XYZ_R0t0(2,2) = mtmp31(0);
	j_XYZ_R0t0(0,3) = j_XYZ_R0t0(1,4) = j_XYZ_R0t0(2,5) = mtmp31(1);
	j_XYZ_R0t0(0,6) = j_XYZ_R0t0(1,7) = j_XYZ_R0t0(2,8) = mtmp31(2);

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			j_XYZ_R0t0(i,j+9) = -mR0t(i,j);
		}
	}
	
	Matx31d j_XYZ_d0 = mR0t*nx0;
	//////////////////////////////////////////////////////////////////////////

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// ���ȵõ���һ���������
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// ��ͨ���ڲ����õ����Գ��������
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// �ȼ����һ����������
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// ����������������
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// ������ͶӰ�в�
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,12> j_uv_Rt = der_uv_Rt(V1,V2,V3,X,Y,Z,W);

	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);

	Matx22d j_dxdy_uv;
	if (0==distType)
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_weng(u,v,k1,k2,k3,k4,k5);
	} 
	else
	{
		j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
	}

	Matx22d j_f_uv = j_xy_uv + j_dxdy_uv;

	Matx<double,2,12> j_f_Rt = j_f_uv * j_uv_Rt;

	A = j_f_Rt * der_Rt_wt(mR); // A = j_f_wt
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki;
// 	if (0==distType)
// 	{
// 		j_dudv_ki = der_dudv_ki_weng(u,v);
// 	} 
// 	else
// 	{
// 		j_dudv_ki = der_dudv_ki_brown(u,v);
// 	}
// 
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// �Բο�ͼ������Լ��������� /////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	Matx<double,2,3> j_f_XYZ = j_f_uv * j_uv_XYZ; // j_f_XYZ

	A0 = j_f_XYZ * j_XYZ_R0t0 * der_Rt_wt(mR0); // A0 = j_f_w0t0

	B = j_f_XYZ * j_XYZ_d0; // B = j_f_d0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// void derivatives::j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
// 							   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
// 							   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
// 							   const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
// 							   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
// 							   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
// 							   const vector<Point2d> & xys,				// ���룺����ͼ���ϵ�������꣬������Ϊ m*n ��
// 							   const vector<int> & j_fixed,				// ���룺��Щͼ��Ĳ����ǹ̶��ģ����ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
// 							   const vector<int> & i_fixed,				// ���룺��Щ�ռ�������ǹ̶��ģ������ i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
// 							   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
// 							   vector<Matx<double,2,6>> & A,			// ��������е�Aij���������ų�һ������
// 							   vector<Matx<double,2,4>> & B,			// ��������е�Bij���������ų�һ������
// 							   vector<Matx21d> & e						// ��������е� eij = xij - Q(aj,bi) ���������ų�һ������
// 							   )
// {
// 	int n = XYZWs.size(); // �ռ�����
// 	int m = Ks.size(); // ͼ�����
// 
// 	for (int i=0;i<n;++i)
// 	{
// 		Point4d XYZW = XYZWs[i];
// 
// 		for (int j=0;j<m;++j)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL==ptr) 
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx33d K = Ks[j];
// 			Matx33d R = Rs[j];
// 			Matx31d t = ts[j];
// 			Matx<double,5,1> dist = dists[j];
// 			int distType = distTypes[j];
// 
// 			Point2d xy = xys[*ptr];
// 
// 			Matx<double,2,6> Aij;
// 			Matx<double,2,4> Bij;
// 
// 			double dx, dy;
// 
// 			j_f_w_t_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
// 				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, dx, dy);
// 
// 			for (int ii=0;ii<2;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					A[*ptr](ii,jj) = Aij(ii,jj);
// 				}
// 
// 				for (int jj=0;jj<4;++jj)
// 				{
// 					B[*ptr](ii,jj) = Bij(ii,jj);
// 				}
// 			}
// 
// 			e[*ptr](0) = -dx;
// 			e[*ptr](1) = -dy;
// 		}
// 	}
// 
// 	// �����й̶�����ͼ��� Aij �� 0
// 	for (int k=0;k<j_fixed.size();++k)
// 	{
// 		int j = j_fixed[k];
// 
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			for (int ii=0;ii<2;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					A[*ptr](ii,jj) = 0;
// 				}
// 			}
// 		}
// 	}
// 
// 	// �����й̶�����Ŀռ��� Bij �� 0
// 	for (int k=0;k<i_fixed.size();++k)
// 	{
// 		int i = i_fixed[k];
// 
// 		for (int j=0;j<m;++j)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			for (int ii=0;ii<2;++ii)
// 			{
// 				for (int jj=0;jj<4;++jj)
// 				{
// 					B[*ptr](ii,jj) = 0;
// 				}
// 			}
// 		}
// 	}
// }

void derivatives::j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
							   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
							   const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
							   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
							   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							   vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
							   vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
							   vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							   vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							   vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
							   Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
							   Mat & g,									// �����6*m+4*nά�Ĳ����ݶ�
							   vector<double> & vds						// �����l��������ͶӰ�в���
							   )
{
	int n = XYZWs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,4>> B(l); // ���е� Bij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,4> Bij;
			Matx21d eij;

			double dx, dy;

			j_f_w_t_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}
			
			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// ����ɨ���������е� Uj��eaj �� Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,4> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];
			
			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;

			Matx<double,6,4> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;

		int j6 = j*6;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(j6+ii) = -eaj(ii);
		}
	}

	int m6 = m*6;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi;
		Matx<double,4,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,4> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,4,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
		}

		V[i] = Vi;
		eb[i] = ebi;

		int i4 = i*4;

		for (int ii=0;ii<4;++ii)
		{
			g.at<double>(m6+i4+ii) = -ebi(ii);
		}
	}
}

// 20151107, iteratively reweighted least square IRLS
// �����ؼ�Ȩģʽ��Ϊ��Ӧ�� outliers
// ���� Huber Ȩ��
void derivatives::j_f_w_t_XYZW_IRLS_Huber(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
										  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
										  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
										  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
										  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
										  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
										  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
										  vector<Matx22d> & covInvs,				// �����l�������������Э�������������Ҳ�����Խ���Ԫ�� (i)=wi*wi����ÿ���۲���Ȩ�ص�ƽ����Ȩ�����ɹ۲����������ͶӰ�в���������
										  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
										  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
										  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
										  vector<Matx<double,6,6>> & U,				// �����m��Uj���󣬽���ͼ������й�
										  vector<Matx<double,4,4>> & V,				// �����n��Vi���󣬽����ռ�������й�
										  vector<Matx<double,6,4>> & W,				// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
										  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
										  vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
										  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ��׼ȷ��˵Ӧ���� wi*fi����Ӧ�ó���Ȩ��ֵ
										  Mat & g,									// �����6*m+4*nά�Ĳ����ݶ�
										  vector<double> & vds,						// �����l��������ͶӰ�в���
										  double tc /*= 3.0*/						// ���룺������ͶӰ�в� d С�� tc �Ĺ۲���Ȩ��ȫ��Ϊ1������Ȩ��Ϊ tc/d
										  )
{
	int n = XYZWs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,4>> B(l); // ���е� Bij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,4> Bij;
			Matx21d eij;

			double dx, dy;

			j_f_w_t_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, dx, dy);

			double d = sqrt(dx*dx + dy*dy);

			// ������ͶӰ�в�������� Huber Ȩ��
			double weight = weight_Huber(d,tc);
			double weight2 = weight*weight;

			int idx2 = 2*(*ptr);

			// ��Ȩ��Ŀ�꺯��
			f.at<double>(idx2)   = weight*dx;
			f.at<double>(idx2+1) = weight*dy;

			// ��Ȩֵ����ֵ
			Matx22d & covInv = covInvs[*ptr];
			covInv(0,0) = covInv(1,1) = weight2;

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// ����ɨ���������е� Uj��eaj �� Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,4> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;

			Matx<double,6,4> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;

		int j6 = j*6;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(j6+ii) = -eaj(ii);
		}
	}

	int m6 = m*6;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi;
		Matx<double,4,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,4> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,4,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
		}

		V[i] = Vi;
		eb[i] = ebi;

		int i4 = i*4;

		for (int ii=0;ii<4;++ii)
		{
			g.at<double>(m6+i4+ii) = -ebi(ii);
		}
	}
}

// 20170811��һ��ͼ��ԣ���ͼ��ͶӰ����̶�Ϊ[I|0]����һ��ΪP�����Ż�������
// ������һϵ������Ӱ�ռ����ؽ������Ĵ��Ż���������꣬���ǵ� Z ����̶�Ϊ1����Ϊ����ͼ��������ע��Z���겻����Ϊ0�������4ά���������˻�Ϊ��XYW��ά�����ʾ
// �ú�����Ҫ���ڷ����Ե����Ż���ͼ��Ļ������� F
void derivatives::j_f_P_XYW(const Matx34d & P,				// ���룺��ǰ��ͼͶӰ���� P �Ĺ���
						    double X, double Y, double W,	// ���룺��ǰ������ X Y W �������
						    double x0, double y0,			// ���룺���������ο�ͼ�еĵ�ʵ�ʹ۲��������
						    double x1, double y1,			// ���룺���������ͼ�еĵ�ʵ�ʹ۲��������
						    Matx<double,2,12> & A1,			// ��������������ͼ�е���ͶӰ����������ͶӰ���� P ��Ԫ����ĵ���
						    Matx<double,2,3> & B1,			// ��������������ͼ�е���ͶӰ���������� X Y W ������ĵ���
						    double & dx0, double & dy0,		// �������ǰ�����£����������ο�ͼ�е���ͶӰ�в�
						    double & dx1, double & dy1		// �������ǰ�����£����������ͼ�е���ͶӰ�в�
						    )
{
	Matx41d XYZW;
	XYZW(0) = X; 
	XYZW(1) = Y;
	XYZW(2) = 1; 
	XYZW(3) = W;

	Matx31d xyw = P * XYZW;

	double f1 = xyw(0);
	double f2 = xyw(1);
	double g  = xyw(2);
	double g_1 = 1.0 / g;

	// �������ͼ�е���ͶӰ�������
	double rx1 = f1*g_1;
	double ry1 = f2*g_1;

	A1 = der_xy_P(X, Y, 1, W, f1, f2, g);
	B1 = der_xy_XYW(P, f1, f2, g);

	// [X Y 1 W] ����ο�ͼ [I|0] ����ͶӰ���������� [X Y]
	dx0 = X - x0;
	dy0 = Y - y0;

	dx1 = rx1 - x1;
	dy1 = ry1 - y1;
}

// 20170809��һ��ͼ��ԣ���ͼ��ͶӰ����̶�Ϊ[I|0]����һ��ΪP�����Ż�������
// ������һϵ������Ӱ�ռ����ؽ������Ĵ��Ż���������꣬���ǵ� Z ����̶�Ϊ1����Ϊ����ͼ��������ע��Z���겻����Ϊ0�������4ά���������˻�Ϊ��XYW��ά�����ʾ
// �ú�����Ҫ���ڷ����Ե����Ż���ͼ��Ļ������� F
void derivatives::j_f_P_XYW(const Matx34d & P,									// ���룺��ǰ���Ƶ���ͼ����Ӱ�ռ��е�ͶӰ����
						    const vector<Point3d> & XYWs,						// ���룺��ǰ���Ƶ� n ���ռ��� XYW ����
							const vector<Point2d> & xysL,						// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������
							const vector<Point2d> & xysR,						// ���룺n ���ռ������ͼ�й۲��������
						    const vector<Matx22d> & covInvsL,					// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������Э�������������
						    const vector<Matx22d> & covInvsR,					// ���룺n ���ռ������ͼ�й۲��������Э�������������
						    Matx<double, 12, 12> & U,							// �����1 �� U ���󣬽��� P �й�
						    vector<Matx<double, 3, 3>> & V,						// �����n �� Vi ���󣬽��Ϳռ�������й�
						    vector<Matx<double, 12, 3>> & W,					// �����n �� Wi ����ͬʱ�� P �Լ��ռ�������й�
						    Matx<double, 12, 1> & ea,							// �����1 �� ea �в�����
						    vector<Matx<double, 3, 1>> & eb,					// �����n �� ebi �в�����
						    double & F,											// �������ǰ��Ŀ�꺯��ֵ 0.5*ft*covinv*f
							double & x_norm,									// �������ǰ���Ż�����������ģ����2����L2��|x|2
						    Mat & g,											// �����12+3*nά�Ĳ����ݶ�
						    vector<Point2d> & vds								// �����n ���ռ������������ͼ�е���ͶӰ�в���
						    )
{
	int n = XYWs.size(); // ������

	Matx<double, 2, 12> A1;
	Matx23d B1;
	Matx<double, 12, 2> A1tc1;
	Matx<double, 3, 2> B1tc1;
	Matx33d B1tc1B1;
	Matx31d B1tc1e1;
	Matx21d e0, e1, c0e0;
	Matx<double, 1, 1> mF;

	double dx0, dy0, dx1, dy1;

	U = Matx<double, 12, 12>();
	ea = Matx<double, 12, 1>();

	x_norm = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			x_norm += (P(i, j)*P(i, j));
		}
	}

	for (int i = 0; i < n; ++i)
	{
		const Point3d & wrdpt = XYWs[i];
		const Point2d & imgpt0 = xysL[i];
		const Point2d & imgpt1 = xysR[i];
		const Matx22d & covInv0 = covInvsL[i];
		const Matx22d & covInv1 = covInvsR[i];

		j_f_P_XYW(P, wrdpt.x, wrdpt.y, wrdpt.z, imgpt0.x, imgpt0.y, imgpt1.x, imgpt1.y, A1, B1, dx0, dy0, dx1, dy1);

		e0(0) = -dx0;
		e0(1) = -dy0;
		double d0 = sqrt(dx0*dx0 + dy0*dy0);
		
		e1(0) = -dx1;
		e1(1) = -dy1;
		double d1 = sqrt(dx1*dx1 + dy1*dy1);

		Point2d ds;
		ds.x = d0;
		ds.y = d1;

		vds[i] = ds;

		A1tc1 = A1.t()*covInv1;
		B1tc1 = B1.t()*covInv1;

		U += A1tc1*A1;
		ea += A1tc1*e1;
		
		W[i] = A1tc1*B1;

		B1tc1B1 = B1tc1*B1;
		B1tc1e1 = B1tc1*e1;

		c0e0 = covInv0*e0;

		mF += (e0.t()*c0e0 + e1.t()*covInv1*e1);

		for (int ii = 0; ii < 2; ++ii)
		{
			for (int jj = 0; jj < 2; ++jj)
			{
				B1tc1B1(ii, jj) += covInv0(ii, jj);
			}

			B1tc1e1(ii) += c0e0(ii);
		}

		V[i] = B1tc1B1;
		eb[i] = B1tc1e1;

		g.at<double>(12 + i * 3)     = -B1tc1e1(0);
		g.at<double>(12 + i * 3 + 1) = -B1tc1e1(1);
		g.at<double>(12 + i * 3 + 2) = -B1tc1e1(2);

		x_norm += (wrdpt.x*wrdpt.x + wrdpt.y*wrdpt.y + wrdpt.z*wrdpt.z);
	}

	for (int i = 0; i < 12; ++i)
	{
		g.at<double>(i) = -ea(i);
	}

	F = 0.5*mF(0);

	x_norm = sqrt(x_norm);
}

// 20170816��ϡ��LM�Ż����������е�ϡ��������淽�̵ĺ���
void derivatives::solve_sparseLM_F(double u,								// ���룺����ϵ��
								 const Matx<double,12,12> & U,			// ���룺1��U���󣬽�����ͼ��ͶӰ����P�й�
								 const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
								 const vector<Matx<double,12,3>> & W,	// ���룺n��Wi����ͬʱ���ռ�㼰P�й�
								 const Matx<double,12,1> & ea,			// ���룺1��ea�в�����������P�й�
								 const vector<Matx<double,3,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
								 Matx<double,12,1> & da,				// �����1��P�ĸ�����
								 vector<Matx<double,3,1>> & db,			// �����n���ռ������ĸ�����
								 Mat & h								// �����12+3*nά�Ĳ���������
								 )
{
	int n = V.size(); // �ռ�����

	Matx<double, 12, 12> U_aug = U; // ��� U' ����
	vector<Matx<double, 3, 3>> V_aug(n); // ��� inv(Vi') ����
	vector<Matx<double, 12, 3>> Y(n); // ������е� Yi = Wi * inv(Vi') ����

	// �γ�������� U' = U + uI
	for (int k = 0; k < 12; ++k)
	{
		U_aug(k, k) += u;
	}	

	// �γ����������� inv(Vi') = (Vi + uI)^(-1)
	// �Լ����е� Yi = Wi * inv(Vi') ����
	for (int i = 0; i < n; ++i)
	{
		Matx<double, 3, 3> Vi_aug = V[i];

		for (int k = 0; k < 3; ++k)
		{
			Vi_aug(k, k) += u;
		}

		V_aug[i] = Vi_aug.inv(DECOMP_CHOLESKY);

		Y[i] = W[i] * V_aug[i];
	}
	
	Matx<double, 12, 12> S = U_aug;
	Matx<double, 12, 1> e = ea;

	// ��� S �� e
	for (int i = 0; i < n; ++i)
	{
		S -= Y[i] * W[i].t();
		e -= Y[i] * eb[i];
	}
	
	// �ⷽ�� S*da = e �õ� P �ĸ�����
	solve(S, e, da, DECOMP_CHOLESKY);

	for (int i = 0; i < 12; ++i)
	{
		h.at<double>(i) = da(i);
	}

	// �ٽ�һ�������ռ������ĸ�����
	for (int i = 0; i < n; ++i)
	{
		Matx<double, 3, 1> dbi = V_aug[i] * (eb[i] - W[i].t()*da);

		db[i] = dbi;

		int offset = 12 + i * 3;

		h.at<double>(offset)     = dbi(0);
		h.at<double>(offset + 1) = dbi(1);
		h.at<double>(offset + 2) = dbi(2);
	}
}

void derivatives::j_f_w_t_XYZ(const vector<Point3d> & XYZs,				// ���룺n���ռ��XYZ����
							  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
							  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
							  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
							  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							  vector<Matx<double,6,6>> & U,				// �����m��Uj���󣬽���ͼ������й�
							  vector<Matx<double,3,3>> & V,				// �����n��Vi���󣬽����ռ�������й�
							  vector<Matx<double,6,3>> & W,				// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							  vector<Matx<double,3,1>> & eb,			// �����n��ebi�в������������ռ�������й�
							  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
							  Mat & g,									// �����6*m+3*nά�Ĳ����ݶ�
							  vector<double> & vds						// �����l��������ͶӰ�в���
							  )
{
	int n = XYZs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,3>> B(l); // ���е� Bij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,3> Bij;
			Matx21d eij;

			double dx, dy;

			j_f_w_t_XYZ(XYZ.x, XYZ.y, XYZ.z, 1, xy.x, xy.y, 
				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// ����ɨ���������е� Uj��eaj �� Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,3> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;

			Matx<double,6,3> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;

		int j6 = j*6;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(j6+ii) = -eaj(ii);
		}
	}

	int m6 = m*6;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi;
		Matx<double,3,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,3> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,3,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
		}

		V[i] = Vi;
		eb[i] = ebi;

		int i3 = i*3;

		for (int ii=0;ii<3;++ii)
		{
			g.at<double>(m6+i3+ii) = -ebi(ii);
		}
	}
}

void derivatives::j_f_w_c_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
							   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
							   const vector<Matx31d> & cs,				// ���룺m��ͼ���������
							   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
							   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							   vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
							   vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
							   vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							   vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							   vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
							   Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
							   Mat & g,									// �����6*m+4*nά�Ĳ����ݶ�
							   vector<double> & vds						// �����l��������ͶӰ�в���
							   )
{
	int n = XYZWs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,4>> B(l); // ���е� Bij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d c = cs[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,4> Bij;
			Matx21d eij;

			double dx, dy;

			j_f_w_c_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
				K, R, c, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// ����ɨ���������е� Uj��eaj �� Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,4> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;

			Matx<double,6,4> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;

		int j6 = j*6;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(j6+ii) = -eaj(ii);
		}
	}

	int m6 = m*6;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi;
		Matx<double,4,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,4> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,4,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
		}

		V[i] = Vi;
		eb[i] = ebi;

		int i4 = i*4;

		for (int ii=0;ii<4;++ii)
		{
			g.at<double>(m6+i4+ii) = -ebi(ii);
		}
	}
}

void derivatives::j_f_w_c_XYZ(const vector<Point3d> & XYZs,				// ���룺n���ռ��XYZ����
							  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
							  const vector<Matx31d> & cs,				// ���룺m��ͼ���������
							  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
							  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							  vector<Matx<double,6,6>> & U,				// �����m��Uj���󣬽���ͼ������й�
							  vector<Matx<double,3,3>> & V,				// �����n��Vi���󣬽����ռ�������й�
							  vector<Matx<double,6,3>> & W,				// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							  vector<Matx<double,3,1>> & eb,			// �����n��ebi�в������������ռ�������й�
							  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
							  Mat & g,									// �����6*m+3*nά�Ĳ����ݶ�
							  vector<double> & vds						// �����l��������ͶӰ�в���
							  )
{
	int n = XYZs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,3>> B(l); // ���е� Bij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d c = cs[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,3> Bij;
			Matx21d eij;

			double dx, dy;

			j_f_w_c_XYZ(XYZ.x, XYZ.y, XYZ.z, 1, xy.x, xy.y, 
				K, R, c, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// ����ɨ���������е� Uj��eaj �� Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,3> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;

			Matx<double,6,3> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;

		int j6 = j*6;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(j6+ii) = -eaj(ii);
		}
	}

	int m6 = m*6;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi;
		Matx<double,3,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,3> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,3,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
		}

		V[i] = Vi;
		eb[i] = ebi;

		int i3 = i*3;

		for (int ii=0;ii<3;++ii)
		{
			g.at<double>(m6+i3+ii) = -ebi(ii);
		}
	}
}

void derivatives::j_f_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
								 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
								 const vector<Matx33d> & Rs,			// ���룺m��ͼ����ת����
								 const vector<Matx31d> & ts,			// ���룺m��ͼ��ƽ������
								 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
								 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
								 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								 vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
								 vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
								 vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
								 Matx<double,1,1> & Q,					// �����Ψһһ��Q����ֻ���������й�
								 vector<Matx<double,1,6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
								 vector<Matx<double,1,4>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
								 vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
								 vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
								 Matx<double,1,1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
								 Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
								 Mat & g,								// �����1+6*m+4*nά�Ĳ����ݶ�
								 vector<double> & vds					// �����l��������ͶӰ�в���
								 )
{
	int n = XYZWs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,4>> B(l); // ���е� Bij ����
	vector<Matx<double,2,1>> C(l); // ���е� Cij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij��Cij �� eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,4> Bij;
			Matx<double,2,1> Cij;
			Matx21d eij;

			double dx, dy;
			
			j_f_f_w_t_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, Cij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// ����ɨ���������е� Uj��eaj��Wij��Q=sum(Qij)��Gj �� ec
	for (int j=0;j<m;++j)
	{
		int idx_start = j*6+1;

		Matx<double,6,6> Uj;
		Matx<double,1,6> Gj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,4> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;
			Matx<double,1,2> Cijtcij = Cij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;
			Q_tmp += Cijtcij*Cij;
			Gj += Cijtcij*Aij;
			ec_tmp += Cijtcij*eij;

			Matx<double,6,4> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;
		G[j] = Gj;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(idx_start+ii) = -eaj(ii);
		}
	}

	Q = Q_tmp;
	ec = ec_tmp;

	g.at<double>(0) = -ec(0);

	int n_before = m*6+1;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		int idx_start = i*4+n_before;

		Matx<double,4,4> Vi;
		Matx<double,1,4> Hi;
		Matx<double,4,1> ebi;
		
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,4> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,4,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
			Hi += Cij.t()*cij*Bij;
		}

		V[i] = Vi;
		eb[i] = ebi;
		H[i] = Hi;

		for (int ii=0;ii<4;++ii)
		{
			g.at<double>(idx_start+ii) = -ebi(ii);
		}
	}
}

// 20200604, iteratively reweighted least square, IRLS
// �����ؼ�Ȩģʽ��Ϊ��Ӧ�� outliers
// ���� Huber Ȩ��
void derivatives::j_f_f_w_t_XYZW_IRLS_Huber(const vector<Point4d> & XYZWs,				// ���룺n���ռ��XYZW����
											const vector<Matx33d> & Ks,					// ���룺m��ͼ���ڲ�������
											const vector<Matx33d> & Rs,					// ���룺m��ͼ����ת����
											const vector<Matx31d> & ts,					// ���룺m��ͼ��ƽ������
											const vector<Matx<double, 5, 1>> & dists,	// ���룺m��ͼ�����ϵ��
											const vector<int> & distTypes,				// ���룺m��ͼ������ϵ������
											const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											vector<Matx22d> & covInvs,					// �����l�������������Э�������������Ҳ�����Խ���Ԫ�� (i)=wi*wi����ÿ���۲���Ȩ�ص�ƽ����Ȩ�����ɹ۲����������ͶӰ�в���������
											const vector<uchar> & j_fixed,				// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											const vector<uchar> & i_fixed,				// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											vector<Matx<double, 6, 6>> & U,				// �����m��Uj���󣬽���ͼ������й�
											vector<Matx<double, 4, 4>> & V,				// �����n��Vi���󣬽����ռ�������й�
											vector<Matx<double, 6, 4>> & W,				// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
											Matx<double, 1, 1> & Q,						// �����Ψһһ��Q����ֻ���������й�
											vector<Matx<double, 1, 6>> & G,				// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
											vector<Matx<double, 1, 4>> & H,				// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
											vector<Matx<double, 6, 1>> & ea,			// �����m��eaj�в�����������ͼ������й�
											vector<Matx<double, 4, 1>> & eb,			// �����n��ebi�в������������ռ�������й�
											Matx<double, 1, 1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
											Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
											Mat & g,									// �����1+6*m+4*nά�Ĳ����ݶ�
											vector<double> & vds,						// �����l��������ͶӰ�в���
											double tc /*= 3.0*/							// ���룺������ͶӰ�в� d С�� tc �Ĺ۲���Ȩ��ȫ��Ϊ1������Ȩ��Ϊ tc/d
											)
{
	int n = XYZWs.size();	// �ռ�����
	int m = Ks.size();		// ͼ�����
	int l = xys.size();		// ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,4>> B(l); // ���е� Bij ����
	vector<Matx<double,2,1>> C(l); // ���е� Cij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij��Cij �� eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,4> Bij;
			Matx<double,2,1> Cij;
			Matx21d eij;

			double dx, dy;
			
			j_f_f_w_t_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, Cij, dx, dy);

			double d = sqrt(dx*dx + dy*dy);

			// ������ͶӰ�в�������� Huber Ȩ��
			double weight = weight_Huber(d,tc);
			double weight2 = weight*weight;

			int idx2 = 2*(*ptr);

			// ��Ȩ��Ŀ�꺯��
			f.at<double>(idx2)	 = weight*dx;
			f.at<double>(idx2+1) = weight*dy;

			// ��Ȩֵ����ֵ
			Matx22d & covInv = covInvs[*ptr];
			covInv(0,0) = covInv(1,1) = weight2;

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// ����ɨ���������е� Uj��eaj��Wij��Q=sum(Qij)��Gj �� ec
	for (int j=0;j<m;++j)
	{
		int idx_start = j*6+1;

		Matx<double,6,6> Uj;
		Matx<double,1,6> Gj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,4> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;
			Matx<double,1,2> Cijtcij = Cij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;
			Q_tmp += Cijtcij*Cij;
			Gj += Cijtcij*Aij;
			ec_tmp += Cijtcij*eij;

			Matx<double,6,4> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;
		G[j] = Gj;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(idx_start+ii) = -eaj(ii);
		}
	}

	Q = Q_tmp;
	ec = ec_tmp;

	g.at<double>(0) = -ec(0);

	int n_before = m*6+1;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		int idx_start = i*4+n_before;

		Matx<double,4,4> Vi;
		Matx<double,1,4> Hi;
		Matx<double,4,1> ebi;
		
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,4> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,4,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
			Hi += Cij.t()*cij*Bij;
		}

		V[i] = Vi;
		eb[i] = ebi;
		H[i] = Hi;

		for (int ii=0;ii<4;++ii)
		{
			g.at<double>(idx_start+ii) = -ebi(ii);
		}
	}
}

void derivatives::j_f_f_w_c_XYZ(const vector<Point3d> & XYZs,			// ���룺n���ռ��XYZ����
							    const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							    const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
							    const vector<Matx31d> & cs,				// ���룺m��ͼ���������
							    const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							    const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							    const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							    const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
							    const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							    const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							    vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
							    vector<Matx<double,3,3>> & V,			// �����n��Vi���󣬽����ռ�������й�
							    vector<Matx<double,6,3>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							    Matx<double,1,1> & Q,					// �����Ψһһ��Q����ֻ���������й�
							    vector<Matx<double,1,6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
							    vector<Matx<double,1,3>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
							    vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							    vector<Matx<double,3,1>> & eb,			// �����n��ebi�в������������ռ�������й�
							    Matx<double,1,1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
							    Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
							    Mat & g,								// �����1+6*m+3*nά�Ĳ����ݶ�
							    vector<double> & vds					// �����l��������ͶӰ�в���
							    )
{
	int n = XYZs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,3>> B(l); // ���е� Bij ����
	vector<Matx<double,2,1>> C(l); // ���е� Cij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d c = cs[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,3> Bij;
			Matx<double,2,1> Cij;
			Matx21d eij;

			double dx, dy;

			j_f_f_w_c_XYZ(XYZ.x, XYZ.y, XYZ.z, 1, xy.x, xy.y, 
				K, R, c, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, Cij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// ����ɨ���������е� Uj��eaj��Wij��Q=sum(Qij)��Gj �� ec
	for (int j=0;j<m;++j)
	{
		int idx_start = j*6+1;

		Matx<double,6,6> Uj;
		Matx<double,1,6> Gj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,3> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;
			Matx<double,1,2> Cijtcij = Cij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;
			Q_tmp += Cijtcij*Cij;
			Gj += Cijtcij*Aij;
			ec_tmp += Cijtcij*eij;

			Matx<double,6,3> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;
		G[j] = Gj;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(idx_start+ii) = -eaj(ii);
		}
	}

	Q = Q_tmp;
	ec = ec_tmp;

	g.at<double>(0) = -ec(0);

	int n_before = m*6+1;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		int idx_start = i*3+n_before;

		Matx<double,3,3> Vi;
		Matx<double,1,3> Hi;
		Matx<double,3,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,3> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,3,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
			Hi += Cij.t()*cij*Bij;
		}

		V[i] = Vi;
		eb[i] = ebi;
		H[i] = Hi;

		for (int ii=0;ii<3;++ii)
		{
			g.at<double>(idx_start+ii) = -ebi(ii);
		}
	}
}

void derivatives::j_f_f_w_t_XYZW_c_fixedBaseline(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
												 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
												 const vector<Matx33d> & Rs,			// ���룺m��ͼ����ת����
												 const vector<Matx31d> & ts,			// ���룺m��ͼ��ƽ������
												 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
												 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
												 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
												 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
												 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
												 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
												 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
												 int k_fixedBaseLine,					// ���룺������ͼ��֮��Ļ��߹̶�������k �ǲ�������һ��
												 int j_fixedBaseLine,					// ���룺������ͼ��֮��Ļ��߹̶�������j �ǵ�����һ��
												 double d_fixedBaseLine,				// ���룺����ͼ��֮��Ĺ̶����߳���
												 vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
												 vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
												 vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
												 Matx<double,1,1> & Q,					// �����Ψһһ��Q����ֻ���������й�
												 vector<Matx<double,1,6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
												 vector<Matx<double,1,4>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
												 vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
												 vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
												 Matx<double,1,1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
												 Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
												 Mat & g,								// �����1+6*m+4*nά�Ĳ����ݶ�
												 vector<double> & vds,					// �����l��������ͶӰ�в���
												 Matx<double,1,6> & Daj,				// �����Լ�����̶Թ̶����ߵ� j ���������
												 double & h								// �����Լ�����̵�ǰ��ֵ
												 )
{
	int n = XYZWs.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ����
	vector<Matx<double,2,4>> B(l); // ���е� Bij ����
	vector<Matx<double,2,1>> C(l); // ���е� Cij ����
	vector<Matx21d> e(l); // ���е� eij �в�����

	// �������е� Aij��Bij �� eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,4> Bij;
			Matx<double,2,1> Cij;
			Matx21d eij;

			double dx, dy;

			j_f_f_w_t_XYZW(XYZW.x, XYZW.y, XYZW.z, XYZW.w, xy.x, xy.y, 
				K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType, Aij, Bij, Cij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// ����ɨ���������е� Uj��eaj��Wij��Q=sum(Qij)��Gj �� ec
	for (int j=0;j<m;++j)
	{
		int idx_start = j*6+1;

		Matx<double,6,6> Uj;
		Matx<double,1,6> Gj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,4> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;
			Matx<double,1,2> Cijtcij = Cij.t()*cij;

			Uj += Aijtcij*Aij;
			eaj += Aijtcij*eij;
			Q_tmp += Cijtcij*Cij;
			Gj += Cijtcij*Aij;
			ec_tmp += Cijtcij*eij;

			Matx<double,6,4> Wij = Aijtcij*Bij;

			W[*ptr] = Wij;
		}

		U[j] = Uj;
		ea[j] = eaj;
		G[j] = Gj;

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(idx_start+ii) = -eaj(ii);
		}
	}

	Q = Q_tmp;
	ec = ec_tmp;

	g.at<double>(0) = -ec(0);

	int n_before = m*6+1;

	// ����ɨ���������е� Vi �� ebi
	for (int i=0;i<n;++i)
	{
		int idx_start = i*4+n_before;

		Matx<double,4,4> Vi;
		Matx<double,1,4> Hi;
		Matx<double,4,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,4> Bij = B[*ptr];
			Matx<double,2,1> Cij = C[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx21d eij = e[*ptr];

			Matx<double,4,2> Bijtcij = Bij.t()*cij;

			Vi += Bijtcij*Bij;
			ebi += Bijtcij*eij;
			Hi += Cij.t()*cij*Bij;
		}

		V[i] = Vi;
		eb[i] = ebi;
		H[i] = Hi;

		for (int ii=0;ii<4;++ii)
		{
			g.at<double>(idx_start+ii) = -ebi(ii);
		}
	}

	// ����Լ������ h(x)
	Matx33d Rj = Rs[j_fixedBaseLine];
	Matx31d Ckw = -Rs[k_fixedBaseLine].t() * ts[k_fixedBaseLine];
	Matx31d Ckj = Rj * Ckw + ts[j_fixedBaseLine];
	double Cx_kj = Ckj(0);
	double Cy_kj = Ckj(1);
	double Cz_kj = Ckj(2);

	h = Cx_kj*Cx_kj + Cy_kj*Cy_kj + Cz_kj*Cz_kj - d_fixedBaseLine*d_fixedBaseLine;

	Daj = der_c_fixedBaseline_Rt(Ckw(0),Ckw(1),Ckw(2),Cx_kj,Cy_kj,Cz_kj) * der_Rt_wt(Rj);
}

// ������ȵ� BA �����У����пɵ���������� xij �������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void derivatives::j_f_w_t_w0_t0_d0(const vector<double> & ds,				// ���룺n���ռ����������ֵ
								   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
								   const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
								   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵĿɵ���������꣬������Ϊ m*n ��
								   const vector<Matx22d> & covInvs,			// ���룺l�����пɵ����������Э�������������
								   const vector<Matx31d> & nxys,			// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ����һ���������
								   const vector<int> & ri_j,				// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
								   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ������ǹ̶��ģ�i_fixed[i]=1��������� i ��ȹ̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								   vector<Matx<double,6,6>> & U,			// ������ܹ�(m+1)m/2��Ujk���󣬽���ͼ������й�
								   vector<double> & V,						// �����n��Vi���󣬽�������й�
								   vector<Matx<double,6,1>> & W,			// �����l��Wij����ͬʱ����ȼ���۲�ͼ���й�
								   vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
								   vector<double> & eb,						// �����n��ebi�в������������ռ�������й�
								   Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
								   Mat & g,									// �����6*m+nά�Ĳ����ݶ�
								   vector<double> & vds						// �����l��������ͶӰ�в���
								   )
{
	int n = ds.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ���󣬼���� xij ��ͼ�� j �Ĳ�����
	vector<Matx<double,2,1>> B(l); // ���е� Bij ���󣬼���� xij ����� i �������
	vector<Matx<double,2,6>> C(l); // ���е� Cij ���󣬼���� xij ����ο�ͼ�� k �Ĳ�����
	vector<Matx21d> e(l); // ���е� eij �в�����
	vector<vector<int>> rj_i(m); // ��¼������ͼ j Ϊ�ο�ͼ������ i��һ�� j ���Զ�Ӧ��� i

	// �������е� Aij��Bij��Cij �� eij
	for (int i=0;i<n;++i)
	{
		double d0 = ds[i];		// �� i �����
		int idx0 = ri_j[i];		// �� i �ο�ͼ�������
		Matx31d nxy0 = nxys[i];	// �� i �����Ĺ�һ���������
		Matx33d R0 = Rs[idx0];	// �� i �ο�ͼ�����ת����
		Matx31d t0 = ts[idx0];	// �� i �ο�ͼ���ƽ������

		rj_i[idx0].push_back(i);

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,1> Bij;
			Matx<double,2,6> Cij;
			Matx21d eij;

			double dx, dy;

			j_f_w_t_w0_t0_d0(d0, xy.x, xy.y, K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType,
				nxy0, R0, t0, Aij, Bij, Cij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			if (!j_fixed[idx0])
			{
				// ֻ�е��� i �Ĳο�ͼ���������Ż�ʱ���Ÿ��� Cij�����򲻶��������Ҫ�����е� Cij �� C ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				C[*ptr] = Cij;
			}

			e[*ptr] = eij;
		}
	}

	// �������е� Ujj
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Ujj;

		// �ȿ������в���ͼ j Ϊ�ο�ͼ������ͼ j �ϱ��۲⵽�����
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx22d cij = covInvs[*ptr];

			Ujj += Aij.t()*cij*Aij;
		}

		vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i

		// �ٿ���������ͼ j Ϊ�ο�ͼ����������ͼ��۲⵽�����
		for (int ii=0;ii<i_j.size();++ii)
		{
			int i = i_j[ii];

			for (int k=0;k<m;++k)
			{
				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xik ������
					continue;
				}

				Matx<double,2,6> Cik = C[*ptr];
				Matx22d cik = covInvs[*ptr];

				Ujj += Cik.t()*cik*Cik;
			}
		}

		int tmpidx = j*m+j-0.5*j*(j+1); // i*m+j-i(i+1)/2
		U[tmpidx] = Ujj;
	}

	// �������е� Ujk
	for (int j=0;j<m;++j)
	{
		for (int k=j+1;k<m;++k)
		{
			Matx<double,6,6> Ujk;

			// �ȿ������в���ͼ j ����ͼ k Ϊ�ο�ͼ����ͼ j �ϱ��۲⵽�����
			for (int i=0;i<n;++i)
			{
				// ����� i �Ĳο�ͼ����ͼ k ��ôֱ�� continue
				if (ri_j[i] != k)
				{
					continue;
				}

				const int * ptr = ptrMat.find<int>(i,j);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xij ������
					continue;
				}

				Matx<double,2,6> Aij = A[*ptr];
				Matx22d cij = covInvs[*ptr];
				Matx<double,2,6> Cij = C[*ptr];

				Ujk += Aij.t()*cij*Cij;
			}

			vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i

			// �ٿ���������ͼ j Ϊ�ο�ͼ����ͼ k �۲⵽�����
			for (int ii=0;ii<i_j.size();++ii)
			{
				int i = i_j[ii];

				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xik ������
					continue;
				}

				Matx<double,2,6> Aik = A[*ptr];
				Matx22d cik = covInvs[*ptr];
				Matx<double,2,6> Cik = C[*ptr];

				Ujk += Cik.t()*cik*Aik;
			}

			int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
			U[tmpidx] = Ujk;
		}
	}

	// ���������ɸ� Ujj ��ͬʱ���������е� Ujk
// 	U = vector<Matx<double,6,6>>(0.5*m*(m+1));
// 
// 	for (int j=0;j<m;++j)
// 	{
// 		Matx<double,6,6> Ujj;
// 
// 		// �ȿ������в���ͼ j Ϊ�ο�ͼ������ͼ j �ϱ��۲⵽�����
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx<double,2,6> Aij = A[*ptr];
// 			Matx22d cij = covInvs[*ptr];
// 			Matx<double,2,6> Cij = C[*ptr];
// 
// 			Matx<double,6,2> Aijtcij = Aij.t()*cij;
// 
// 			Ujj += Aijtcij*Aij;
// 
// 			// ѭ���У�ͬʱ�����Ծ����ͼ k Ϊ�ο�ͼ����ͼ j �ϱ��۲⵽�����
// 			int k = ri_j[i]; // ��� ij �Ĳο�ͼ����
// 
// 			if (k>j) // ֻ���������в��� Ujk
// 			{
// 				int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 				U[tmpidx] += Aijtcij*Cij;
// 			}
// 		}
// 
// 		vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i
// 
// 		// �ٿ���������ͼ j Ϊ�ο�ͼ����������ͼ��۲⵽�����
// 		for (int ii=0;ii<i_j.size();++ii)
// 		{
// 			int i = i_j[ii];
// 
// 			for (int k=0;k<m;++k)
// 			{
// 				const int * ptr = ptrMat.find<int>(i,k);
// 
// 				if (NULL == ptr)
// 				{
// 					// ��� ptr == NULL��˵����� xik ������
// 					continue;
// 				}
// 
// 				Matx<double,2,6> Aik = A[*ptr];
// 				Matx22d cik = covInvs[*ptr];
// 				Matx<double,2,6> Cik = C[*ptr];
// 
// 				Matx<double,6,2> Ciktcik = Cik.t()*cik;
// 
// 				Ujj += Ciktcik*Cik;
// 
// 				// ѭ���У�ͬʱ������ͼ j Ϊ�ο�ͼ���������ͼ k �۲⵽�����
// 				if (k>j) // ֻ���������в��� Ujk
// 				{
// 					int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 					U[tmpidx] += Ciktcik*Aik;
// 				}
// 			}
// 		}
// 
// 		int tmpidx = j*m+j-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 		U[tmpidx] = Ujj;
// 	}

	// ���� Wij��ͬʱ�͵� i �Լ�ͼ j ���
	for (int j=0;j<m;++j)
	{
		for (int i=0;i<n;++i)
		{
			if (ri_j[i]!=j)
			{
				// ����� i �Ĳο�ͼ���� j����ôͬʱ�͵� i ��ͼ j ��ص�����ֻ�� xij ��
				const int * ptr = ptrMat.find<int>(i,j);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xij ������
					continue;
				}

				Matx<double,2,6> Aik = A[*ptr];
				Matx22d cik = covInvs[*ptr];
				Matx<double,2,6> Cik = C[*ptr];
			}
			else
			{
				// ����� i �Ĳο�ͼ�� j����ôͬʱ�͵� i ��ͼ j ��ص����������е�{xik}��

			}
		}
	}


	// ����ɨ���������е� Uj��eaj �� Wij
// 	for (int j=0;j<m;++j)
// 	{
// 		Matx<double,6,6> Uj;
// 		Matx<double,6,1> eaj;
// 
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx<double,2,6> Aij = A[*ptr];
// 			Matx<double,2,3> Bij = B[*ptr];
// 			Matx22d cij = covInvs[*ptr];
// 			Matx21d eij = e[*ptr];
// 
// 			Matx<double,6,2> Aijtcij = Aij.t()*cij;
// 
// 			Uj += Aijtcij*Aij;
// 			eaj += Aijtcij*eij;
// 
// 			Matx<double,6,3> Wij = Aijtcij*Bij;
// 
// 			W[*ptr] = Wij;
// 		}
// 
// 		U[j] = Uj;
// 		ea[j] = eaj;
// 
// 		int j6 = j*6;
// 
// 		for (int ii=0;ii<6;++ii)
// 		{
// 			g.at<double>(j6+ii) = -eaj(ii);
// 		}
// 	}
// 
// 	int m6 = m*6;
// 
// 	// ����ɨ���������е� Vi �� ebi
// 	for (int i=0;i<n;++i)
// 	{
// 		Matx<double,3,3> Vi;
// 		Matx<double,3,1> ebi;
// 
// 		for (int j=0;j<m;++j)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx<double,2,3> Bij = B[*ptr];
// 			Matx22d cij = covInvs[*ptr];
// 			Matx21d eij = e[*ptr];
// 
// 			Matx<double,3,2> Bijtcij = Bij.t()*cij;
// 
// 			Vi += Bijtcij*Bij;
// 			ebi += Bijtcij*eij;
// 		}
// 
// 		V[i] = Vi;
// 		eb[i] = ebi;
// 
// 		int i3 = i*3;
// 
// 		for (int ii=0;ii<3;++ii)
// 		{
// 			g.at<double>(m6+i3+ii) = -ebi(ii);
// 		}
// 	}
}

// ������ȵ� BA �����У������������ xij �������ɵ����ķǲο�����Լ����ɵ����Ĳο���㣩�������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void derivatives::j_f_w_t_w0_t0_d0_new(const vector<double> & ds,			// ���룺n���ռ����������ֵ
								  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
								  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
								  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								  const vector<Point2d> & xys,				// ���룺l���۲�������꣬���Ϊ m*n ��
								  const vector<Matx22d> & covInvs,			// ���룺l���۲��������Э�������������
								  const vector<Matx31d> & nxys,				// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ����һ���������
								  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
								  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ������ǹ̶��ģ�i_fixed[i]=1��������� i ��ȹ̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								  vector<Matx<double,6,6>> & U,				// ������ܹ�(m+1)m/2��Ujk���󣬽���ͼ������й�
								  vector<double> & V,						// �����n��Vi���󣬽�������й�
								  vector<Matx<double,6,1>> & W,				// �����l��Wij����ͬʱ����ȼ���۲�ͼ���й�
								  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
								  vector<double> & eb,						// �����n��ebi�в������������ռ�������й�
								  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
								  Mat & g,									// �����6*m+nά�Ĳ����ݶ�
								  vector<double> & vds						// �����l��������ͶӰ�в���
								  )
{
	int n = ds.size(); // �ռ�����
	int m = Ks.size(); // ͼ�����
	int l = xys.size(); // ���й۲����ĸ���

	vector<Matx<double,2,6>> A(l); // ���е� Aij ���󣬼���� xij ��ͼ�� j �Ĳ�����
	vector<Matx<double,2,1>> B(l); // ���е� Bij ���󣬼���� xij ����� i �������
	vector<Matx<double,2,6>> C(l); // ���е� Cij ���󣬼���� xij ����ο�ͼ�� k �Ĳ�����
	vector<Matx21d> e(l); // ���е� eij �в�����
	vector<vector<int>> rj_i(m); // ��¼������ͼ j Ϊ�ο�ͼ������ i��һ�� j ���Զ�Ӧ��� i

	// �������е� Aij��Bij��Cij �� eij
	for (int i=0;i<n;++i)
	{
		double d0 = ds[i];		// �� i �����
		int idx0 = ri_j[i];		// �� i �ο�ͼ�������
		Matx31d nxy0 = nxys[i];	// �� i �����Ĺ�һ���������
		Matx33d R0 = Rs[idx0];	// �� i �ο�ͼ�����ת����
		Matx31d t0 = ts[idx0];	// �� i �ο�ͼ���ƽ������

		rj_i[idx0].push_back(i);

		for (int j=0;j<m;++j)
		{
			if (j==idx0)
			{
				// ��ʱ˵����� xij �ǿ϶����ڵģ���Ϊ�� i �Ĳο���㣬�ο���� xij �ǹ̶������ģ������κβ����ı�
				// �� Aij=Bij=Cij,j=0������в��� eij=0 ��ȻҲ����
				continue;
			}

			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[*ptr];

			Matx<double,2,6> Aij;
			Matx<double,2,1> Bij;
			Matx<double,2,6> Cij;
			Matx21d eij;

			double dx, dy;

			j_f_w_t_w0_t0_d0(d0, xy.x, xy.y, K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType,
				nxy0, R0, t0, Aij, Bij, Cij, dx, dy);

			int idx2 = 2*(*ptr);

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[*ptr] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (!j_fixed[j])
			{
				// ֻ�е� j ͼ���̶��������Ż�ʱ���Ÿ��� Aij�����򲻶��������Ҫ�����е� Aij �� A ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// ֻ�е� i �㲻�̶��������Ż�ʱ���Ÿ��� Bij�����򲻶��������Ҫ�����е� Bij �� B ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				B[*ptr] = Bij;
			}

			if (!j_fixed[idx0])
			{
				// ֻ�е��� i �Ĳο�ͼ���������Ż�ʱ���Ÿ��� Cij�����򲻶��������Ҫ�����е� Cij �� C ��һ��ʼ��ȫ����ʼ��Ϊ 0 ����
				C[*ptr] = Cij;
			}

			e[*ptr] = eij;
		}
	}

	// �������е� Ujj
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Ujj;

		// �ȿ������в���ͼ j Ϊ�ο�ͼ������ͼ j �ϱ��۲⵽�����
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx22d cij = covInvs[*ptr];

			Ujj += Aij.t()*cij*Aij;
		}

		vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i

		// �ٿ���������ͼ j Ϊ�ο�ͼ����������ͼ��۲⵽�����
		for (int ii=0;ii<i_j.size();++ii)
		{
			int i = i_j[ii];

			for (int k=0;k<m;++k)
			{
				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xik ������
					continue;
				}

				Matx<double,2,6> Cik = C[*ptr];
				Matx22d cik = covInvs[*ptr];

				Ujj += Cik.t()*cik*Cik;
			}
		}

		int tmpidx = j*m+j-0.5*j*(j+1); // i*m+j-i(i+1)/2
		U[tmpidx] = Ujj;
	}

	// �������е� Ujk
	for (int j=0;j<m;++j)
	{
		for (int k=j+1;k<m;++k)
		{
			Matx<double,6,6> Ujk;

			// �ȿ������в���ͼ j ����ͼ k Ϊ�ο�ͼ����ͼ j �ϱ��۲⵽�����
			for (int i=0;i<n;++i)
			{
				// ����� i �Ĳο�ͼ����ͼ k ��ôֱ�� continue
				if (ri_j[i] != k)
				{
					continue;
				}

				const int * ptr = ptrMat.find<int>(i,j);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xij ������
					continue;
				}

				Matx<double,2,6> Aij = A[*ptr];
				Matx22d cij = covInvs[*ptr];
				Matx<double,2,6> Cij = C[*ptr];

				Ujk += Aij.t()*cij*Cij;
			}

			vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i

			// �ٿ���������ͼ j Ϊ�ο�ͼ����ͼ k �۲⵽�����
			for (int ii=0;ii<i_j.size();++ii)
			{
				int i = i_j[ii];

				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xik ������
					continue;
				}

				Matx<double,2,6> Aik = A[*ptr];
				Matx22d cik = covInvs[*ptr];
				Matx<double,2,6> Cik = C[*ptr];

				Ujk += Cik.t()*cik*Aik;
			}

			int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
			U[tmpidx] = Ujk;
		}
	}

	// ���������ɸ� Ujj ��ͬʱ���������е� Ujk
// 	U = vector<Matx<double,6,6>>(0.5*m*(m+1));
// 
// 	for (int j=0;j<m;++j)
// 	{
// 		Matx<double,6,6> Ujj;
// 
// 		// �ȿ������в���ͼ j Ϊ�ο�ͼ������ͼ j �ϱ��۲⵽�����
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx<double,2,6> Aij = A[*ptr];
// 			Matx22d cij = covInvs[*ptr];
// 			Matx<double,2,6> Cij = C[*ptr];
// 
// 			Matx<double,6,2> Aijtcij = Aij.t()*cij;
// 
// 			Ujj += Aijtcij*Aij;
// 
// 			// ѭ���У�ͬʱ�����Ծ����ͼ k Ϊ�ο�ͼ����ͼ j �ϱ��۲⵽�����
// 			int k = ri_j[i]; // ��� ij �Ĳο�ͼ����
// 
// 			if (k>j) // ֻ���������в��� Ujk
// 			{
// 				int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 				U[tmpidx] += Aijtcij*Cij;
// 			}
// 		}
// 
// 		vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i
// 
// 		// �ٿ���������ͼ j Ϊ�ο�ͼ����������ͼ��۲⵽�����
// 		for (int ii=0;ii<i_j.size();++ii)
// 		{
// 			int i = i_j[ii];
// 
// 			for (int k=0;k<m;++k)
// 			{
// 				const int * ptr = ptrMat.find<int>(i,k);
// 
// 				if (NULL == ptr)
// 				{
// 					// ��� ptr == NULL��˵����� xik ������
// 					continue;
// 				}
// 
// 				Matx<double,2,6> Aik = A[*ptr];
// 				Matx22d cik = covInvs[*ptr];
// 				Matx<double,2,6> Cik = C[*ptr];
// 
// 				Matx<double,6,2> Ciktcik = Cik.t()*cik;
// 
// 				Ujj += Ciktcik*Cik;
// 
// 				// ѭ���У�ͬʱ������ͼ j Ϊ�ο�ͼ���������ͼ k �۲⵽�����
// 				if (k>j) // ֻ���������в��� Ujk
// 				{
// 					int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 					U[tmpidx] += Ciktcik*Aik;
// 				}
// 			}
// 		}
// 
// 		int tmpidx = j*m+j-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 		U[tmpidx] = Ujj;
// 	}

	// ���� Wij��ͬʱ�͵� i �Լ�ͼ j ���
	for (int j=0;j<m;++j)
	{
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij �����ڣ���ô Wij=0
				continue;
			}

			if (ri_j[i]!=j)
			{
				// ����� i �Ĳο�ͼ���� j����ôͬʱ�͵� i ��ͼ j ��ص�����ֻ�� xij ��
				Matx<double,2,6> Aij = A[*ptr];
				Matx22d cij = covInvs[*ptr];
				Matx<double,2,1> Bij = B[*ptr];

				W[*ptr] = Aij.t()*cij*Bij;
			}
			else
			{
				Matx<double,6,1> Wij;

				// ����� i �Ĳο�ͼ�� j����ôͬʱ�͵� i ��ͼ j ��ص����������е�{xik}��
				for (int k=0;k<m;++k)
				{
					const int * ptr = ptrMat.find<int>(i,k);

					if (NULL == ptr)
					{
						// ��� ptr == NULL��˵����� xik ������
						continue;
					}

					Matx<double,2,6> Cik = C[*ptr];
					Matx22d cik = covInvs[*ptr];
					Matx<double,2,1> Bik = B[*ptr];

					Wij += Cik.t()*cik*Bik;
				}

				W[*ptr] = Wij;
			}
		}
	}

	// ���������ɸ� Ujj ��ͬʱ���������е� Ujk���� Wij
	U = vector<Matx<double,6,6>>(0.5*m*(m+1));

	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Ujj;

		// �ȿ������в���ͼ j Ϊ�ο�ͼ������ͼ j �ϱ��۲⵽�����
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,1> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx<double,2,6> Cij = C[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Ujj += Aijtcij*Aij;

			// ѭ���У�ͬʱ�����Ծ����ͼ k Ϊ�ο�ͼ����ͼ j �ϱ��۲⵽�����
			int k = ri_j[i]; // ��� ij �Ĳο�ͼ����

			if (k>j) // ֻ���������в��� Ujk
			{
				int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
				U[tmpidx] += Aijtcij*Cij;
			}

			if (ri_j[i]!=j)
			{
				W[*ptr] = Aijtcij*Bij;
			}
		}

		vector<int> i_j = rj_i[j]; // ������ͼ j Ϊ�ο�ͼ������ i

		// �ٿ���������ͼ j Ϊ�ο�ͼ����������ͼ��۲⵽�����
		for (int ii=0;ii<i_j.size();++ii)
		{
			int i = i_j[ii];

			Matx<double,6,1> Wij;

			for (int k=0;k<m;++k)
			{
				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// ��� ptr == NULL��˵����� xik ������
					continue;
				}

				Matx<double,2,6> Aik = A[*ptr];
				Matx<double,2,1> Bik = B[*ptr];
				Matx22d cik = covInvs[*ptr];
				Matx<double,2,6> Cik = C[*ptr];

				Matx<double,6,2> Ciktcik = Cik.t()*cik;

				Ujj += Ciktcik*Cik;

				// ѭ���У�ͬʱ������ͼ j Ϊ�ο�ͼ���������ͼ k �۲⵽�����
				if (k>j) // ֻ���������в��� Ujk
				{
					int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
					U[tmpidx] += Ciktcik*Aik;
				}

				Wij += Ciktcik*Bik;
			}

			const int * ptr = ptrMat.find<int>(i,j);
			W[*ptr] = Wij;
		}

		int tmpidx = j*m+j-0.5*j*(j+1); // i*m+j-i(i+1)/2
		U[tmpidx] = Ujj;
	}


	// ����ɨ���������е� Uj��eaj �� Wij
// 	for (int j=0;j<m;++j)
// 	{
// 		Matx<double,6,6> Uj;
// 		Matx<double,6,1> eaj;
// 
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx<double,2,6> Aij = A[*ptr];
// 			Matx<double,2,3> Bij = B[*ptr];
// 			Matx22d cij = covInvs[*ptr];
// 			Matx21d eij = e[*ptr];
// 
// 			Matx<double,6,2> Aijtcij = Aij.t()*cij;
// 
// 			Uj += Aijtcij*Aij;
// 			eaj += Aijtcij*eij;
// 
// 			Matx<double,6,3> Wij = Aijtcij*Bij;
// 
// 			W[*ptr] = Wij;
// 		}
// 
// 		U[j] = Uj;
// 		ea[j] = eaj;
// 
// 		int j6 = j*6;
// 
// 		for (int ii=0;ii<6;++ii)
// 		{
// 			g.at<double>(j6+ii) = -eaj(ii);
// 		}
// 	}
// 
// 	int m6 = m*6;
// 
// 	// ����ɨ���������е� Vi �� ebi
// 	for (int i=0;i<n;++i)
// 	{
// 		Matx<double,3,3> Vi;
// 		Matx<double,3,1> ebi;
// 
// 		for (int j=0;j<m;++j)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Matx<double,2,3> Bij = B[*ptr];
// 			Matx22d cij = covInvs[*ptr];
// 			Matx21d eij = e[*ptr];
// 
// 			Matx<double,3,2> Bijtcij = Bij.t()*cij;
// 
// 			Vi += Bijtcij*Bij;
// 			ebi += Bijtcij*eij;
// 		}
// 
// 		V[i] = Vi;
// 		eb[i] = ebi;
// 
// 		int i3 = i*3;
// 
// 		for (int ii=0;ii<3;++ii)
// 		{
// 			g.at<double>(m6+i3+ii) = -ebi(ii);
// 		}
// 	}
}

// ������ȵ� BA �����У������������ xij �������ɵ����ķǲο�����Լ����ɵ����Ĳο���㣩�������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void derivatives::j_f_w_t_w0_t0_d0_new2(const vector<double> & ds,			// ���룺n���ռ����������ֵ
									  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
									  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
									  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
									  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
									  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
									  const vector<Point2d> & xys,				// ���룺l���۲�������꣬���Ϊ m*n ��
									  const vector<Matx22d> & covInvs,			// ���룺l���۲��������Э�������������
									  const vector<Matx31d> & nxys,				// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ����һ���������
									  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
									  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
									  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ������ǹ̶��ģ�i_fixed[i]=1��������� i ��ȹ̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
									  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									  vector<Matx<double,6,6>> & U,				// ������ܹ�(m+1)m/2��Ujk���󣬽���ͼ������й�
									  vector<Matx<double,1,1>> & V,				// �����n��Vi���󣬽�������й�
									  vector<Matx<double,6,1>> & W,				// �����l��Wij����ͬʱ����ȼ���۲�ͼ���й�
									  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
									  vector<Matx<double,1,1>> & eb,			// �����n��ebi�в������������ռ�������й�
									  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
									  Mat & g,									// �����6*m+nά�Ĳ����ݶ�
									  vector<double> & vds						// �����l��������ͶӰ�в���
									  )
{
	int n = ds.size();	 // �ռ�����
	int m = Ks.size();	 // ͼ�����
	int l = xys.size();	 // ���й۲����ĸ���

	// ��ʼ��ȫ�� 0
	U = vector<Matx<double,6,6>>((m+1)*m*0.5);
	V = vector<Matx<double,1,1>>(n);
	W = vector<Matx<double,6,1>>(l);
	ea = vector<Matx<double,6,1>>(m);
	eb = vector<Matx<double,1,1>>(n);

	// �������й۲���㣬�������е� Ujj,Ujk,Wij,Vi,eaj,ebi
	for (int i=0;i<n;++i)
	{
		int k = ri_j[i];		// �� i �Ĳο�ͼ��

		double d0 = ds[i];		// �� i �����
		Matx31d nxy0 = nxys[i];	// �� i �����Ĺ�һ���������
		Matx33d R0 = Rs[k];		// �� i �ο�ͼ�����ת����
		Matx31d t0 = ts[k];		// �� i �ο�ͼ���ƽ������

		for (int j=0;j<m;++j)
		{
			if (j==k)
			{
				// ��ʱ˵�� j �ǵ� i �Ĳο�ͼ����� xij �ǿ϶����ڵģ���Ϊ�� i �Ĳο����
				// �ο���� xij �����κβ����ı䣬Ҳ��������ͶӰ�в�� Aij=Bij=Cij=eij=0
				continue;
			}

			const int * ptr_ij = ptrMat.find<int>(i,j);

			if (NULL==ptr_ij)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			int idx_ij = (*ptr_ij);

			// �ȼ������� Aij,Bij,Cij,eij ////////////////////////////////////////////
			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[idx_ij];

			// �۲���� xij ͬʱ�����˵� i������ͼ j���Լ��ο�ͼ k ����Ϣ
			Matx<double,2,6> Aij;			// xij �Գ���ͼ j ��
			Matx<double,2,1> Bij;			// xij �Ե� i ��
			Matx<double,2,6> Cij;			// xij �Բο�ͼ k ��
			Matx22d cij = covInvs[idx_ij];	// xij ��Ȩ����
			Matx21d eij;					// xij ����ͶӰ�в�

			double dx, dy;

			j_f_w_t_w0_t0_d0(d0, xy.x, xy.y, K, R, t, dist(0), dist(1), dist(2), dist(3), dist(4), distType,
				nxy0, R0, t0, Aij, Bij, Cij, dx, dy);

			int idx2 = 2*idx_ij;

			f.at<double>(idx2)   = dx;
			f.at<double>(idx2+1) = dy;

			double d = sqrt(dx*dx + dy*dy);

			vds[idx_ij] = d;

			eij(0) = -dx;
			eij(1) = -dy;

			if (j_fixed[j])
			{
				// ���ͼ j �����̶�����ô Aij=0
				Aij = Matx<double,2,6>();
			}

			if (i_fixed[i])
			{
				// ����� i �̶�����ô Bij=0
				Bij = Matx<double,2,1>();
			}

			if (j_fixed[k])
			{
				// ����� i �ο�ͼ k �Ĳ����̶�����ô Cij=0
				Cij = Matx<double,2,6>();
			}
			//////////////////////////////////////////////////////////////////////////

			Matx<double,6,2> Aijtcij = Aij.t()*cij;
			Matx<double,1,2> Bijtcij = Bij.t()*cij;
			Matx<double,6,2> Cijtcij = Cij.t()*cij;

			U[j*m+j-0.5*j*(j+1)] += Aijtcij*Aij;		// ������ͼ j �й�
			U[k*m+k-0.5*k*(k+1)] += Cijtcij*Cij;		// ���ο�ͼ k �й�

			if (j<k)
			{
				U[j*m+k-0.5*j*(j+1)] += Aijtcij*Cij;	// Ujk ��һ�ͬʱ�ͳ���ͼ j �Լ��ο�ͼ k �й�
			}

			if (k<j)
			{
				U[k*m+j-0.5*k*(k+1)] += Cijtcij*Aij;	// Ukj ��һ�ͬʱ�ͳ���ͼ j �Լ��ο�ͼ k �й�
			}

			W[idx_ij] += Aijtcij*Bij;					// Wij��ͬʱ�͵� i �Լ�ͼ j �й�

			const int * ptr_ik = ptrMat.find<int>(i,k);	
			int idx_ik = (*ptr_ik);

			W[idx_ik] += Cijtcij*Bij;					// Wik��ͬʱ�͵� i �Լ��ο�ͼ k �й�

			ea[j] += Aijtcij*eij;
			ea[k] += Cijtcij*eij;

			V[i] += Bijtcij*Bij;

			eb[i] += Bijtcij*eij;
		}
	}

	for (int j=0;j<m;++j)
	{
		int ii_start = 6*j;

		Matx<double,6,1> eaj = ea[j];

		for (int ii=0;ii<6;++ii)
		{
			g.at<double>(ii_start+ii) = -eaj(ii);
		}
	}

	int ii_start = 6*m;

	for (int i=0;i<n;++i)
	{
		g.at<double>(ii_start+i) = -eb[i](0);
	}
}

void derivatives::solveSBA_0_6_4(double u,								// ���룺����ϵ��
								 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								 const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
								 const vector<Matx<double,4,4>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
								 const vector<Matx<double,6,4>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
								 const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
								 const vector<Matx<double,4,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
								 vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
								 vector<Matx<double,4,1>> & db,			// �����n���ռ������ĸ�����
								 Mat & h								// �����6*m+4*nά�Ĳ���������
								 )
{
	int m = U.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������

	int M = 6*m; // �ܵ�ͼ���������

	vector<Matx<double,6,6>> U_aug(m); // ��� Uj' ����
	vector<Matx<double,4,4>> V_aug(n); // ��� inv(Vi') ����
	vector<Matx<double,6,4>> Y(l); // ������е� Yij = Wij * inv(Vi') ����

	// �γ�������� Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	// �γ����������� inv(Vi') = (Vi + uI)^(-1)
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi_aug = V[i];

		for (int k=0;k<4;++k)
		{
			Vi_aug(k,k)+=u;
		}

		V_aug[i] = Vi_aug.inv(DECOMP_CHOLESKY);
	}
	
	// �γ����е� Yij = Wij * inv(Vi') ����
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Y[*ptr] = W[*ptr] * V_aug[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // ����ͼ������ĸ�����

	// �γ����е� Sjk ����
// 	for (int j=0;j<m;++j)
// 	{
// 		int j6 = j*6;
// 
// 		for (int k=0;k<m;++k)
// 		{
// 			int k6 = k*6;
// 
// 			Matx<double,6,6> Sjk;
// 
// 			for (int i=0;i<n;++i)
// 			{
// 				const int * ptr1 = ptrMat.find<int>(i,j);
// 				const int * ptr2 = ptrMat.find<int>(i,k);
// 
// 				if (NULL == ptr1 || NULL == ptr2)
// 				{
// 					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
// 					continue;
// 				}
// 
// 				Sjk -= Y[*ptr1] * W[*ptr2].t();
// 			}
// 
// 			if (j==k)
// 			{
// 				Sjk += U_aug[j];
// 			}
// 
// 			// ����� S ����
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		for (int k=j;k<m;++k)
		{
			int k6 = k*6;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += U_aug[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// �γ����е� ej �в���
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		Matx<double,6,1> ej = ea[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ej -= Y[*ptr] * eb[i];
		}

		for (int ii=0;ii<6;++ii)
		{
			e.at<double>(j6+ii) = ej(ii);
		}
	}

	// �ⷽ�� S*ha = e �õ�����ͼ������ĸ�����
	solve(S, e, ha, DECOMP_CHOLESKY);

	int m6 = m*6;

	for (int ii=0;ii<m6;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// ¼�뵽�����
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		Matx<double,6,1> haj;

		for (int ii=0;ii<6;++ii)
		{
			haj(ii) = ha.at<double>(j6+ii);
		}

		da[j] = haj;
	}

	// �ٽ�һ�������ռ������ĸ�����
	for (int i=0;i<n;++i)
	{
		Matx<double,4,1> ei = eb[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ei -= W[*ptr].t() * da[j];
		}

		Matx<double,4,1> hbi = V_aug[i] * ei;

		db[i] = hbi;

		int i4 = i*4;

		for (int ii=0;ii<4;++ii)
		{
			h.at<double>(m6+i4+ii) = hbi(ii);
		}
	}
}

void derivatives::solveSBA_0_6_3(double u,								// ���룺����ϵ��
							     const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							     const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
							     const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
							     const vector<Matx<double,6,3>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							     const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
							     const vector<Matx<double,3,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
							     vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
							     vector<Matx<double,3,1>> & db,			// �����n���ռ������ĸ�����
							     Mat & h								// �����6*m+3*nά�Ĳ���������
							     )
{
	int m = U.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������

	int M = 6*m; // �ܵ�ͼ���������

	vector<Matx<double,6,6>> U_aug(m); // ��� Uj' ����
	vector<Matx<double,3,3>> V_aug(n); // ��� inv(Vi') ����
	vector<Matx<double,6,3>> Y(l); // ������е� Yij = Wij * inv(Vi') ����

	// �γ�������� Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	// �γ����������� inv(Vi') = (Vi + uI)^(-1)
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi_aug = V[i];

		for (int k=0;k<3;++k)
		{
			Vi_aug(k,k)+=u;
		}

		V_aug[i] = Vi_aug.inv(DECOMP_CHOLESKY);
	}
	
	// �γ����е� Yij = Wij * inv(Vi') ����
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Y[*ptr] = W[*ptr] * V_aug[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // ����ͼ������ĸ�����

	// �γ����е� Sjk ����
// 	for (int j=0;j<m;++j)
// 	{
// 		int j6 = j*6;
// 
// 		for (int k=0;k<m;++k)
// 		{
// 			int k6 = k*6;
// 
// 			Matx<double,6,6> Sjk;
// 
// 			for (int i=0;i<n;++i)
// 			{
// 				const int * ptr1 = ptrMat.find<int>(i,j);
// 				const int * ptr2 = ptrMat.find<int>(i,k);
// 
// 				if (NULL == ptr1 || NULL == ptr2)
// 				{
// 					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
// 					continue;
// 				}
// 
// 				Sjk -= Y[*ptr1] * W[*ptr2].t();
// 			}
// 
// 			if (j==k)
// 			{
// 				Sjk += U_aug[j];
// 			}
// 
// 			// ����� S ����
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		for (int k=j;k<m;++k)
		{
			int k6 = k*6;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += U_aug[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// �γ����е� ej �в���
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		Matx<double,6,1> ej = ea[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ej -= Y[*ptr] * eb[i];
		}

		for (int ii=0;ii<6;++ii)
		{
			e.at<double>(j6+ii) = ej(ii);
		}
	}

	// �ⷽ�� S*ha = e �õ�����ͼ������ĸ�����
	solve(S, e, ha, DECOMP_CHOLESKY);

	int m6 = m*6;

	for (int ii=0;ii<m6;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// ¼�뵽�����
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		Matx<double,6,1> haj;

		for (int ii=0;ii<6;++ii)
		{
			haj(ii) = ha.at<double>(j6+ii);
		}

		da[j] = haj;
	}

	// �ٽ�һ�������ռ������ĸ�����
	for (int i=0;i<n;++i)
	{
		Matx<double,3,1> ei = eb[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ei -= W[*ptr].t() * da[j];
		}

		Matx<double,3,1> hbi = V_aug[i] * ei;

		db[i] = hbi;

		int i3 = i*3;

		for (int ii=0;ii<3;++ii)
		{
			h.at<double>(m6+i3+ii) = hbi(ii);
		}
	}
}

void derivatives::solveSBA_1_6_4(double u,								// ���룺����ϵ��
								 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								 const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
								 const vector<Matx<double,4,4>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
								 const vector<Matx<double,6,4>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
								 const Matx<double,1,1> & Q,			// ���룺Ψһһ��Q����ֻ���������й�
								 const vector<Matx<double,1,6>> & G,	// ���룺m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
								 const vector<Matx<double,1,4>> & H,	// ���룺n��Hi����ͬʱ�͹���ͼ������Լ�����й�
								 const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
								 const vector<Matx<double,4,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
								 const Matx<double,1,1> & ec,			// ���룺Ψһһ��ec�в���������������ͼ������й�
								 vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
							 	 vector<Matx<double,4,1>> & db,			// �����n���ռ������ĸ�����
								 Matx<double,1,1> & dc,					// ���������ͼ������ĸ�����
								 Mat & h								// �����1+6*m+4*nά�Ĳ���������
								 )
{
	int m = U.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������

	int M = 6*m+1; // �ܵ�ͼ����������������˸�ͼ����в����Լ����в���

	vector<Matx<double,6,6>> U_aug(m); // ��� Uj' ����
	vector<Matx<double,4,4>> V_aug(n); // ��� inv(Vi') ����
	vector<Matx<double,6,4>> Y(l); // ������е� Yij = Wij * inv(Vi') ����
	Matx<double,1,1> Q_aug = Q; // ��������ϵ������Ĺ����������� Q ����
	vector<Matx<double,1,4>> HV(n); // ��� Hi * inv(Vi') ����

	// �γ�������� Q' = Q + uI
	Q_aug(0)+=u;

	// �γ�������� Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	Matx<double,1,1> Q_HVH = Q_aug; // ���淽�̵����Ͻǲ��� Q' - H*inv(V')*Ht
	vector<Matx<double,1,6>> G_HVW(m); // ���淽�̵����Ͻǲ��� G - H*inv(V')*Wt
	Matx<double,1,1> ec_HVeb = ec; // ec - H*inv(V')*eb

	// �γ����������� inv(Vi') = (Vi + uI)^(-1)
	// �Լ����� Hi*inv(Vi')
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi_aug = V[i];

		for (int k=0;k<4;++k)
		{
			Vi_aug(k,k)+=u;
		}

		Matx<double,4,4> Vi_aug_inv = Vi_aug.inv(DECOMP_CHOLESKY);

		V_aug[i] = Vi_aug_inv;

		Matx<double,1,4> Hi = H[i];

		Matx<double,1,4> HiVi = Hi * Vi_aug_inv; // Hi * inv(Vi')

		HV[i] = HiVi;

		Q_HVH -= HiVi*Hi.t();
		ec_HVeb -= HiVi*eb[i];
	}
	
	// �γ����е� Yij = Wij * inv(Vi') ����
	for (int j=0;j<m;++j)
	{
		Matx<double,1,6> G_HVWj = G[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,6,4> Wij = W[*ptr];

			Y[*ptr] = Wij * V_aug[i];
			G_HVWj -= HV[i]*Wij.t();
		}

		G_HVW[j] = G_HVWj;
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // ����ͼ������ĸ�����

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	S.at<double>(0,0) = Q_HVH(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,1,6> G_HVWj = G_HVW[j];

		for (int ii=0;ii<1;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				S.at<double>(ii,j_start+jj) = S.at<double>(j_start+jj,ii) = G_HVWj(ii,jj);
			}
		}

		for (int k=j;k<m;++k)
		{
			int k_start = k*6+1;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += U_aug[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j_start+ii,k_start+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j_start+ii,k_start+jj) = S.at<double>(k_start+jj,j_start+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// �γ����е� ej �в���
	e.at<double>(0) = ec_HVeb(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,6,1> ej = ea[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ej -= Y[*ptr] * eb[i];
		}

		for (int ii=0;ii<6;++ii)
		{
			e.at<double>(j_start+ii) = ej(ii);
		}
	}

	// �ⷽ�� S*ha = e �õ�����ͼ������ĸ�����
	solve(S, e, ha, DECOMP_CHOLESKY);

	for (int ii=0;ii<M;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// ¼�뵽�����
	dc(0) = ha.at<double>(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,6,1> haj;

		for (int ii=0;ii<6;++ii)
		{
			haj(ii) = ha.at<double>(j_start+ii);
		}

		da[j] = haj;
	}

	// �ٽ�һ�������ռ������ĸ�����
	for (int i=0;i<n;++i)
	{
		int i_start = i*4+M;

		Matx<double,4,1> ei = eb[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ei -= W[*ptr].t() * da[j];
		}

		ei -= H[i].t()*dc;

		Matx<double,4,1> hbi = V_aug[i] * ei;

		db[i] = hbi;

		for (int ii=0;ii<4;++ii)
		{
			h.at<double>(i_start+ii) = hbi(ii);
		}
	}
}

void derivatives::solveSBA_1_6_4_Daj(double u,								// ���룺����ϵ��
									 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									 const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
									 const vector<Matx<double,4,4>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
									 const vector<Matx<double,6,4>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
									 const Matx<double,1,1> & Q,			// ���룺Ψһһ��Q����ֻ���������й�
									 const vector<Matx<double,1,6>> & G,	// ���룺m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
									 const vector<Matx<double,1,4>> & H,	// ���룺n��Hi����ͬʱ�͹���ͼ������Լ�����й�
									 const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
									 const vector<Matx<double,4,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
									 const Matx<double,1,1> & ec,			// ���룺Ψһһ��ec�в���������������ͼ������й�
									 int j_constrained,						// ���룺��Լ����ͼ�������
									 const vector<Matx<double,1,6>> & Daj,	// ���룺ÿ��Լ�����̶Ե� j ��ͼ����в����ĵ���
									 const vector<double> & hs,				// ���룺ÿ��Լ�������ڵ�ǰ�����µ�ֵ
									 vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
									 vector<Matx<double,4,1>> & db,			// �����n���ռ������ĸ�����
									 Matx<double,1,1> & dc,					// ���������ͼ������ĸ�����
									 Mat & h,								// �����1+6*m+4*nά�Ĳ���������
									 double & val							// �����0.5*dx'C'lamda
									 )
{
	int m = U.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������
	int nc = hs.size(); // Լ�����̵ĸ���

	int M = 6*m+1; // �ܵ�ͼ����������������˸�ͼ����в����Լ����в���

	vector<Matx<double,6,6>> U_aug(m); // ��� Uj' ����
	vector<Matx<double,4,4>> V_aug(n); // ��� inv(Vi') ����
	vector<Matx<double,6,4>> Y(l); // ������е� Yij = Wij * inv(Vi') ����
	Matx<double,1,1> Q_aug = Q; // ��������ϵ������Ĺ����������� Q ����
	vector<Matx<double,1,4>> HV(n); // ��� Hi * inv(Vi') ����

	// �γ�������� Q' = Q + uI
	Q_aug(0)+=u;

	// �γ�������� Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	Matx<double,1,1> Q_HVH = Q_aug; // ���淽�̵����Ͻǲ��� Q' - H*inv(V')*Ht
	vector<Matx<double,1,6>> G_HVW(m); // ���淽�̵����Ͻǲ��� G - H*inv(V')*Wt
	Matx<double,1,1> ec_HVeb = ec; // ec - H*inv(V')*eb

	// �γ����������� inv(Vi') = (Vi + uI)^(-1)
	// �Լ����� Hi*inv(Vi')
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi_aug = V[i];

		for (int k=0;k<4;++k)
		{
			Vi_aug(k,k)+=u;
		}

		Matx<double,4,4> Vi_aug_inv = Vi_aug.inv(DECOMP_CHOLESKY);

		V_aug[i] = Vi_aug_inv;

		Matx<double,1,4> Hi = H[i];

		Matx<double,1,4> HiVi = Hi * Vi_aug_inv; // Hi * inv(Vi')

		HV[i] = HiVi;

		Q_HVH -= HiVi*Hi.t();
		ec_HVeb -= HiVi*eb[i];
	}

	// �γ����е� Yij = Wij * inv(Vi') ����
	for (int j=0;j<m;++j)
	{
		Matx<double,1,6> G_HVWj = G[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,6,4> Wij = W[*ptr];

			Y[*ptr] = Wij * V_aug[i];
			G_HVWj -= HV[i]*Wij.t();
		}

		G_HVW[j] = G_HVWj;
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // ����ͼ������ĸ�����

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	S.at<double>(0,0) = Q_HVH(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,1,6> G_HVWj = G_HVW[j];

		for (int ii=0;ii<1;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				S.at<double>(ii,j_start+jj) = S.at<double>(j_start+jj,ii) = G_HVWj(ii,jj);
			}
		}

		for (int k=j;k<m;++k)
		{
			int k_start = k*6+1;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += U_aug[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j_start+ii,k_start+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j_start+ii,k_start+jj) = S.at<double>(k_start+jj,j_start+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// ����� lagrange multipliers ////////////////////////////////////////////////////////////////////////////
	int idx_tmp = 1+j_constrained*6;

	S = S.inv(DECOMP_CHOLESKY); // ��ʱ S = inv(S)

	Matx<double,6,1> Kajc;

	for (int i=0;i<6;++i)
	{
		Kajc(i) = S.at<double>(0,idx_tmp+i);
	}

	vector<Matx<double,6,6>> Kajaks(m);

	for (int j=0;j<m;++j)
	{
		int j_start = 1+j*6;

		Matx<double,6,6> Kajak;

		for (int ii=0;ii<6;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				Kajak(ii,jj) = S.at<double>(idx_tmp+ii,j_start+jj);
			}
		}

		Kajaks[j] = Kajak;
	}

	vector<Matx<double,6,4>> Kajbis(n);

	for (int i=0;i<n;++i)
	{
		Matx<double,6,4> Kajbi = -Kajc*HV[i];

		for (int k=0;k<m;++k)
		{
			const int * ptr = ptrMat.find<int>(i,k);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Kajbi -= Kajaks[k]*Y[*ptr];
		}

		Kajbis[i] = Kajbi;
	}

	// ������� lagrange multipliers �ķ������ϵ�����Լ�������
	Mat A(nc, nc, CV_64FC1, Scalar(0));
	Mat b(nc, 1, CV_64FC1, Scalar(0));
	Mat lmd; // ���� lagrange multipliers

	Matx<double,6,6> Kajaj = Kajaks[j_constrained];

	for (int ii=0;ii<nc;++ii)
	{
		Matx<double,1,6> Daj_ii = Daj[ii];

		for (int jj=0;jj<nc;++jj)
		{
			Matx<double,1,1> Aiijj = Daj_ii*Kajaj*Daj[jj].t(); 
			A.at<double>(ii,jj) = Aiijj(0);
		}
	}

	for (int ii=0;ii<nc;++ii)
	{
		Matx<double,1,6> Daj_ii = Daj[ii];

		Matx<double,1,1> bi = Daj_ii*Kajc*ec;

		for (int k=0;k<m;++k)
		{
			bi += Daj_ii*Kajaks[k]*ea[k];
		}

		for (int i=0;i<n;++i)
		{
			bi += Daj_ii*Kajbis[i]*eb[i];
		}

		b.at<double>(ii) = bi(0)+hs[ii];
	}
	
	// �ⷽ�� A*lmd = b �õ� lagrange multipliers
	solve(A, b, lmd, DECOMP_CHOLESKY);

	vector<double> lmds(nc);

	for (int i=0;i<nc;++i)
	{
		lmds[i] = lmd.at<double>(i);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// �γ����е� ej �в���
	e.at<double>(0) = ec_HVeb(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,6,1> ej = ea[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ej -= Y[*ptr] * eb[i];
		}

		// Լ������� ///////////////////////////
		if (j==j_constrained)
		{
			for (int k=0;k<nc;++k)
			{
				ej -= Daj[k].t()*lmds[k];
			}
		}
		/////////////////////////////////////////

		for (int ii=0;ii<6;++ii)
		{
			e.at<double>(j_start+ii) = ej(ii);
		}
	}

	// �ⷽ�� S*ha = e �õ�����ͼ������ĸ�����
	ha = S*e;

	for (int ii=0;ii<M;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// ¼�뵽�����
	dc(0) = ha.at<double>(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,6,1> haj;

		for (int ii=0;ii<6;++ii)
		{
			haj(ii) = ha.at<double>(j_start+ii);
		}

		da[j] = haj;
	}

	// �ٽ�һ�������ռ������ĸ�����
	for (int i=0;i<n;++i)
	{
		int i_start = i*4+M;

		Matx<double,4,1> ei = eb[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ei -= W[*ptr].t() * da[j];
		}

		ei -= H[i].t()*dc;

		Matx<double,4,1> hbi = V_aug[i] * ei;

		db[i] = hbi;

		for (int ii=0;ii<4;++ii)
		{
			h.at<double>(i_start+ii) = hbi(ii);
		}
	}

	// ���� 0.5*dx'C'lamda
	Matx<double,6,1> vTmp;
	for (int k=0;k<nc;++k)
	{
		vTmp += Daj[k].t()*lmds[k];
	}
	Matx<double,1,1> vVal = 0.5*da[j_constrained].t()*vTmp;

	val = vVal(0);
}

// 20150721,�������һά���������ʾ
void derivatives::solveSBA_0_6_1(double u,								// ���룺����ϵ��
								 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ������
								 const vector<Matx<double,6,6>> & U,	// ���룺�ܹ�(m+1)m/2�� Ujk ���󣬽���ͼ������й�
								 const vector<Matx<double,1,1>> & V,	// ���룺n��Vi���󣬽����ռ������й�
								 const vector<Matx<double,6,1>> & W,	// ���룺l��Wij����ͬʱ���ռ����ȼ���۲�ͼ���й�
								 const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
								 const vector<Matx<double,1,1>> & eb,	// ���룺n��ebi�в������������ռ������й�
								 vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
								 vector<Matx<double,1,1>> & db,			// �����n���ռ����ȵĸ�����
								 Mat & h								// �����6*m+nά�Ĳ���������
								 )
{
	int m = ea.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������

	int M = 6*m; // �ܵ�ͼ���������

	vector<Matx<double,6,6>> Ujj_aug(m);// ��� Ujj' ����
	vector<Matx<double,1,1>> V_aug(n);	// ��� inv(Vi') ����
	vector<Matx<double,6,1>> Y(l);		// ������е� Yij = Wij * inv(Vi') ����

	// �γ�������� Ujj' = Ujj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j*m+j-0.5*j*(j+1)];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		Ujj_aug[j] = Uj_aug;
	}

	// �γ����������� inv(Vi') = (Vi + uI)^(-1)
	for (int i=0;i<n;++i)
	{
		double Vi = V[i](0);

		V_aug[i](0) = 1/(Vi+u);
	}
	
	// �γ����е� Yij = Wij * inv(Vi') ����
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			int idx_ij = (*ptr);

			Y[idx_ij] = W[idx_ij] * V_aug[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // ����ͼ������ĸ�����

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		for (int k=j;k<m;++k)
		{
			int k6 = k*6;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += Ujj_aug[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			Sjk += U[j*m+k-0.5*j*(j+1)];

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// �γ����е� ej �в���
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		Matx<double,6,1> ej = ea[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ej -= Y[*ptr] * eb[i];
		}

		for (int ii=0;ii<6;++ii)
		{
			e.at<double>(j6+ii) = ej(ii);
		}
	}

	// �ⷽ�� S*ha = e �õ�����ͼ������ĸ�����
	solve(S, e, ha, DECOMP_CHOLESKY);

	int m6 = m*6;

	for (int ii=0;ii<m6;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// ¼�뵽�����
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		Matx<double,6,1> haj;

		for (int ii=0;ii<6;++ii)
		{
			haj(ii) = ha.at<double>(j6+ii);
		}

		da[j] = haj;
	}

	// �ٽ�һ�������ռ����ȵĸ�����
	for (int i=0;i<n;++i)
	{
		Matx<double,1,1> ei = eb[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				continue;
			}

			ei -= W[*ptr].t() * da[j];
		}

		Matx<double,1,1> hbi = V_aug[i] * ei;

		db[i] = hbi;

		h.at<double>(m6+i) = hbi(0);
	}
}

// void derivatives::covarianceSBA_0_6_4(const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
// 									  const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
// 									  const vector<Matx<double,4,4>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
// 									  const vector<Matx<double,6,4>> & W	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
// 									  )
// {
// 	int m = U.size(); // ͼ�����
// 	int n = V.size(); // �ռ�����
// 	int l = W.size(); // ����������
// 
// 	int M = 6*m; // �ܵ�ͼ���������
// 
// 	vector<Matx<double,4,4>> V_inv(n); // ��� inv(Vi) ����
// 	vector<Matx<double,6,4>> Y(l); // ������е� Yij = Wij * inv(Vi) ����
// 
// 	// �γ������ inv(Vi)
// 	for (int i=0;i<n;++i)
// 	{
// 		V_inv[i] = V[i].inv(DECOMP_CHOLESKY);
// 	}
// 
// 	// �γ����е� Yij = Wij * inv(Vi) ����
// 	for (int i=0;i<n;++i)
// 	{
// 		for (int j=0;j<m;++j)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// ��� ptr == NULL��˵����� xij ������
// 				continue;
// 			}
// 
// 			Y[*ptr] = W[*ptr] * V_inv[i];
// 		}
// 	}
// 
// 	Mat S(M, M, CV_64FC1, Scalar(0)); // U-W*inv(V)*W'
// 
// 	// �γ����е� Sjk ����
// // 	for (int j=0;j<m;++j)
// // 	{
// // 		int j6 = j*6;
// // 
// // 		for (int k=0;k<m;++k)
// // 		{
// // 			int k6 = k*6;
// // 
// // 			Matx<double,6,6> Sjk;
// // 
// // 			for (int i=0;i<n;++i)
// // 			{
// // 				const int * ptr1 = ptrMat.find<int>(i,j);
// // 				const int * ptr2 = ptrMat.find<int>(i,k);
// // 
// // 				if (NULL == ptr1 || NULL == ptr2)
// // 				{
// // 					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
// // 					continue;
// // 				}
// // 
// // 				Sjk -= Y[*ptr1] * W[*ptr2].t();
// // 			}
// // 
// // 			if (j==k)
// // 			{
// // 				Sjk += U[j];
// // 			}
// // 
// // 			// ����� S ����
// // 			for (int ii=0;ii<6;++ii)
// // 			{
// // 				for (int jj=0;jj<6;++jj)
// // 				{
// // 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// // 				}
// // 			}
// // 		}
// // 	}
// 
// 	// �γ����е� Sjk ����
// 	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
// 	// �����ٴ�� 35% ������
// 	for (int j=0;j<m;++j)
// 	{
// 		int j6 = j*6;
// 
// 		for (int k=j;k<m;++k)
// 		{
// 			int k6 = k*6;
// 
// 			Matx<double,6,6> Sjk;
// 
// 			for (int i=0;i<n;++i)
// 			{
// 				const int * ptr1 = ptrMat.find<int>(i,j);
// 				const int * ptr2 = ptrMat.find<int>(i,k);
// 
// 				if (NULL == ptr1 || NULL == ptr2)
// 				{
// 					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
// 					continue;
// 				}
// 
// 				Sjk -= Y[*ptr1] * W[*ptr2].t();
// 			}
// 
// 			if (j==k) // �Խ�����
// 			{
// 				Sjk += U[j];
// 
// 				// ����� S ����
// 				for (int ii=0;ii<6;++ii)
// 				{
// 					for (int jj=0;jj<6;++jj)
// 					{
// 						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 					}
// 				}
// 
// 				continue;
// 			}
// 
// 			// ����� S ����������Ԫ��
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}
// 
// 	Mat cov_a = S.inv(DECOMP_SVD); // ���������Э�������Ϊ���� U-W*inv(V)*W' ��α��
// }

void derivatives::covarianceSBA_0_6_3(const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									  const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
									  const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
									  const vector<Matx<double,6,3>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
									  vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э�������
									  vector<Matx<double,3,3>> & cov_b		// �����n���ռ��������Э�������
									  )
{
	cov_a.clear();
	cov_b.clear();

	int m = U.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������

	int M = 6*m; // �ܵ�ͼ���������

	vector<Matx<double,3,3>> V_inv(n); // ��� inv(Vi) ����
	vector<Matx<double,6,3>> Y(l); // ������е� Yij = Wij * inv(Vi) ����

	// �γ������ inv(Vi)
	for (int i=0;i<n;++i)
	{
		V_inv[i] = V[i].inv(DECOMP_CHOLESKY);
	}
	
	// �γ����е� Yij = Wij * inv(Vi) ����
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Y[*ptr] = W[*ptr] * V_inv[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0)); // U-W*inv(V)*W'

	// �γ����е� Sjk ����
// 	for (int j=0;j<m;++j)
// 	{
// 		int j6 = j*6;
// 
// 		for (int k=0;k<m;++k)
// 		{
// 			int k6 = k*6;
// 
// 			Matx<double,6,6> Sjk;
// 
// 			for (int i=0;i<n;++i)
// 			{
// 				const int * ptr1 = ptrMat.find<int>(i,j);
// 				const int * ptr2 = ptrMat.find<int>(i,k);
// 
// 				if (NULL == ptr1 || NULL == ptr2)
// 				{
// 					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
// 					continue;
// 				}
// 
// 				Sjk -= Y[*ptr1] * W[*ptr2].t();
// 			}
// 
// 			if (j==k)
// 			{
// 				Sjk += U[j];
// 			}
// 
// 			// ����� S ����
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	for (int j=0;j<m;++j)
	{
		int j6 = j*6;

		for (int k=j;k<m;++k)
		{
			int k6 = k*6;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += U[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	Mat mcov_a = S.inv(DECOMP_SVD); // ���������Э�������Ϊ���� U-W*inv(V)*W' ��α��

//	SaveMat2File_double("C:\\Users\\DeepVoid\\Desktop\\mcov_a.txt",mcov_a,WRITEMODE_E);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6;

		Matx<double,6,6> cov_aj;

		for (int ii=0;ii<6;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				cov_aj(ii,jj) = mcov_a.at<double>(j_start+ii,j_start+jj);
			}
		}

		cov_a.push_back(cov_aj);
	}

	for (int i=0;i<n;++i)
	{
		Mat Yi(M,3,CV_64FC1,Scalar(0)); // Yi = [Yi1; Yi2; ...; Yim]

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			int j_start = j*6;

			Matx<double,6,3> Yij = Y[*ptr];

			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<3;++jj)
				{
					Yi.at<double>(j_start+ii,jj) = Yij(ii,jj);
				}
			}
		}

		Matx33d Vi_inv = V_inv[i];
		Mat mVi_inv(3,3,CV_64FC1);
		for (int ii=0;ii<3;++ii)
		{
			for (int jj=0;jj<3;++jj)
			{
				mVi_inv.at<double>(ii,jj) = Vi_inv(ii,jj);
			}
		}

		Mat mcov_bi = Yi.t()*mcov_a*Yi + mVi_inv;

		Matx33d cov_bi;

		for (int ii=0;ii<3;++ii)
		{
			for (int jj=0;jj<3;++jj)
			{
				cov_bi(ii,jj) = mcov_bi.at<double>(ii,jj);
			}
		}

		cov_b.push_back(cov_bi);
	}
}

void derivatives::covarianceSBA_1_6_3(const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									  const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
									  const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
									  const vector<Matx<double,6,3>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
									  const Matx<double,1,1> & Q,			// ���룺Ψһһ��Q����ֻ���������й�
									  const vector<Matx<double,1,6>> & G,	// ���룺m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
									  const vector<Matx<double,1,3>> & H,	// ���룺n��Hi����ͬʱ�͹���ͼ������Լ�����й�
									  vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э������
									  vector<Matx<double,3,3>> & cov_b,		// �����n���ռ��������Э������
									  Matx<double,1,1> & cov_c,				// �������������Э������
									  vector<Matx<double,1,6>> & cov_ca		// �����ͼ�񹲲����͸�ͼ����в���֮���Э������
									  )
{
	cov_a.clear();
	cov_b.clear();
	cov_ca.clear();

	int m = U.size(); // ͼ�����
	int n = V.size(); // �ռ�����
	int l = W.size(); // ����������

	int M = 6*m+1; // �ܵ�ͼ����������������˸�ͼ����в����Լ����в���

	vector<Matx<double,3,3>> V_inv(n); // ��� inv(Vi) ����
	vector<Matx<double,6,3>> Y(l); // ������е� Yij = Wij * inv(Vi) ����
	vector<Matx<double,1,3>> HV(n); // ��� Hi * inv(Vi) ����

	Matx<double,1,1> Q_HVH = Q; // ���淽�̵����Ͻǲ��� Q - H*inv(V)*Ht
	vector<Matx<double,1,6>> G_HVW(m); // ���淽�̵����Ͻǲ��� G - H*inv(V)*Wt

	// �γ��� inv(Vi)
	// �Լ����� Hi*inv(Vi)
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi_inv = V[i].inv(DECOMP_CHOLESKY);

		V_inv[i] = Vi_inv;

		Matx<double,1,3> Hi = H[i];

		Matx<double,1,3> HiVi = Hi * Vi_inv; // Hi * inv(Vi)

		HV[i] = HiVi;

		Q_HVH -= HiVi*Hi.t();
	}

	// �γ����е� Yij = Wij * inv(Vi) ����
	for (int j=0;j<m;++j)
	{
		Matx<double,1,6> G_HVWj = G[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			Matx<double,6,3> Wij = W[*ptr];

			Y[*ptr] = Wij * V_inv[i];
			G_HVWj -= HV[i]*Wij.t();
		}

		G_HVW[j] = G_HVWj;
	}

	Mat S(M, M, CV_64FC1, Scalar(0));

	// �γ����е� Sjk ����
	// ���� S �����ǶԳ�������ֻ���������ǺͶԽ���Ԫ�ؼ��ɣ�������Ԫ��ֱ��ת�ù�ȥ���ɡ�
	// �����ٴ�� 35% ������
	S.at<double>(0,0) = Q_HVH(0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,1,6> G_HVWj = G_HVW[j];

		for (int ii=0;ii<1;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				S.at<double>(ii,j_start+jj) = S.at<double>(j_start+jj,ii) = G_HVWj(ii,jj);
			}
		}

		for (int k=j;k<m;++k)
		{
			int k_start = k*6+1;

			Matx<double,6,6> Sjk;

			for (int i=0;i<n;++i)
			{
				const int * ptr1 = ptrMat.find<int>(i,j);
				const int * ptr2 = ptrMat.find<int>(i,k);

				if (NULL == ptr1 || NULL == ptr2)
				{
					// ������ i ��ͬʱ��ͼ�� j �� k �۲⵽
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // �Խ�����
			{
				Sjk += U[j];

				// ����� S ����
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j_start+ii,k_start+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// ����� S ����������Ԫ��
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j_start+ii,k_start+jj) = S.at<double>(k_start+jj,j_start+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	Mat mcov_d = S.inv(DECOMP_SVD); // ������в����Լ����в�����Э������

//	SaveMat2File_double("C:\\Users\\DeepVoid\\Desktop\\mcov_d.txt",mcov_d,WRITEMODE_E);

	cov_c(0) = mcov_d.at<double>(0,0);

	for (int j=0;j<m;++j)
	{
		int j_start = j*6+1;

		Matx<double,6,6> cov_aj;
		Matx<double,1,6> cov_caj;

		for (int ii=0;ii<6;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				cov_aj(ii,jj) = mcov_d.at<double>(j_start+ii,j_start+jj);
			}
		}

		for (int ii=0;ii<1;++ii)
		{
			for (int jj=0;jj<6;++jj)
			{
				cov_caj(ii,jj) = mcov_d.at<double>(ii,j_start+jj);
			}
		}

		cov_a.push_back(cov_aj);
		cov_ca.push_back(cov_caj);
	}	

	for (int i=0;i<n;++i)
	{
		Mat HVYi(M,3,CV_64FC1,Scalar(0)); // HVYi = [Hi*inv(Vi); Yi1; Yi2; ...; Yim]

		Matx<double,1,3> HiVi = HV[i];

		for (int ii=0;ii<1;++ii)
		{
			for (int jj=0;jj<3;++jj)
			{
				HVYi.at<double>(ii,jj) = HiVi(ii,jj);
			}
		}

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			int j_start = j*6+1;

			Matx<double,6,3> Yij = Y[*ptr];

			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<3;++jj)
				{
					HVYi.at<double>(j_start+ii,jj) = Yij(ii,jj);
				}
			}
		}

		Matx33d Vi_inv = V_inv[i];
		Mat mVi_inv(3,3,CV_64FC1);
		for (int ii=0;ii<3;++ii)
		{
			for (int jj=0;jj<3;++jj)
			{
				mVi_inv.at<double>(ii,jj) = Vi_inv(ii,jj);
			}
		}

		Mat mcov_bi = HVYi.t()*mcov_d*HVYi + mVi_inv;

		Matx33d cov_bi;

		for (int ii=0;ii<3;++ii)
		{
			for (int jj=0;jj<3;++jj)
			{
				cov_bi(ii,jj) = mcov_bi.at<double>(ii,jj);
			}
		}

		cov_b.push_back(cov_bi);
	}
}

void derivatives::covarianceSBA_wj_cj_XiYiZi(const vector<Point3d> & XYZs,			// ���룺n���ռ������
											 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
											 const vector<Matx33d> & Rs,			// ���룺m��ͼ����ת����
											 const vector<Matx31d> & ts,			// ���룺m��ͼ��ƽ������
											 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
											 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
											 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
											 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											 vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э������
											 vector<Matx<double,3,3>> & cov_b		// �����n���ռ��������Э������
											 )
{
	int m = Ks.size();
	int n = XYZs.size();
	int l = xys.size();

	int N = 2*l;
	int M = 6*m+3*n;

	vector<Matx31d> cs(m);
	for (int j=0;j<m;++j)
	{
		cs[j] = -Rs[j].t()*ts[j]; // C=-R't    t=-RC
	}

	vector<Matx<double,6,6>> U(m);			
	vector<Matx<double,3,3>> V(n);
	vector<Matx<double,6,3>> W(l);
	vector<Matx<double,6,1>> ea(m);
	vector<Matx<double,3,1>> eb(n);
	Mat f(N,1,CV_64FC1,Scalar(0)), g(M,1,CV_64FC1,Scalar(0));
	vector<double> vds(l);

	j_f_w_c_XYZ(XYZs,Ks,Rs,cs,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	covarianceSBA_0_6_3(ptrMat,U,V,W,cov_a,cov_b);
}

void derivatives::covarianceSBA_f_wj_cj_XiYiZi(const vector<Point3d> & XYZs,			// ���룺n���ռ������
											   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
											   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
											   const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
											   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
											   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
											   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
											   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											   vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э������
											   vector<Matx<double,3,3>> & cov_b,		// �����n���ռ��������Э������
											   Matx<double,1,1> & cov_c,				// �������������Э������
											   vector<Matx<double,1,6>> & cov_ca		// �����ͼ�񹲲����͸�ͼ����в���֮���Э������
											   )
{
	int m = Ks.size();
	int n = XYZs.size();
	int l = xys.size();

	int N = 2*l;
	int M = 1+6*m+3*n;

	vector<Matx31d> cs(m);
	for (int j=0;j<m;++j)
	{
		cs[j] = -Rs[j].t()*ts[j]; // C=-R't    t=-RC
	}

	vector<Matx<double,6,6>> U(m);			
	vector<Matx<double,3,3>> V(n);
	vector<Matx<double,6,3>> W(l);
	Matx<double,1,1> Q;
	vector<Matx<double,1,6>> G(m);
	vector<Matx<double,1,3>> H(n);
	vector<Matx<double,6,1>> ea(m);
	vector<Matx<double,3,1>> eb(n);
	Matx<double,1,1> ec;
	Mat f(N,1,CV_64FC1,Scalar(0)), g(M,1,CV_64FC1,Scalar(0));
	vector<double> vds(l);

	j_f_f_w_c_XYZ(XYZs,Ks,Rs,cs,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,Q,G,H,ea,eb,ec,f,g,vds);

	covarianceSBA_1_6_3(ptrMat,U,V,W,Q,G,H,cov_a,cov_b,cov_c,cov_ca);
}

// Matx<double,2,16> derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(double X, double Y, double Z,	// �ÿ��Ƶ������
// 																  double x, double y,			// �ÿ��Ƶ��ʵ�ʹ۲��������
// 																  const Matx33d & mK,			// ��ǰ���ڲ�������
// 																  const Matx33d & mR,			// ��ǰ����̬����
// 																  const Matx31d & mt,			// ��ǰ��ƽ����������
// 																  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
// 																  double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
// 																  )
// {
// 	Matx31d mX;
// 	mX(0)=X; mX(1)=Y; mX(2)=Z;
// 
// 	Matx31d mx = mR*mX+mt;
// 	double mx2_1 = 1.0/mx(2);
// 
// 	// ���ȵõ���һ���������
// 	mx(0)*=mx2_1;
// 	mx(1)*=mx2_1;
// 	mx(2)=1;
// 
// 	double u = mx(0);
// 	double v = mx(1);
// 
// 	// ��ͨ���ڲ����õ����Գ��������
// 	mx = mK*mx;
// 	double x0 = mx(0);
// 	double y0 = mx(1);
// 
// 	double fx = mK(0,0);	double fy = mK(1,1);
// 	double cx = mK(0,2);	double cy = mK(1,2);
// 	double s  = mK(0,1);	
// 
// 	// ���������
// 	double ddx,ddy;
// 	distortions::dxdy_brown(fx,fy,s,u,v,k1,k2,k3,k4,k5,ddx,ddy);
// 
// 	dx = x0+ddx-x;
// 	dy = y0+ddy-y;
// 
// 
// 	// ���ڲ����� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_xy_fxfycxcys = der_xy_fxfycxcys(u,v); // dx/da
// 
// 	Matx<double,2,5> j_dxdy_fxfycxcys = der_dxdy_fxfycxcys_brown(u,v,k1,k2,k3,k4,k5); // ddx/da
// 
// 	Matx<double,2,5> j_f_fxfycxcys = j_xy_fxfycxcys + j_dxdy_fxfycxcys; // df/da = dx/da + ddx/da
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 	// ��������� ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,12> j_uv_Rt = der_uv_Rt(mR,mt,mX);
// 
// 	Matx22d j_xy_uv = der_xy_uv(fx,fy,s);
// 
// 	Matx22d j_dxdy_uv = j_xy_uv * der_dudv_uv_brown(u,v,k1,k2,k3,k4,k5);
// 
// 	Matx<double,2,12> j_f_Rt = (j_xy_uv + j_dxdy_uv) * j_uv_Rt;
// 
// 	Matx<double,2,6> j_f_wt = j_f_Rt * der_Rt_wt(mR);
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 	// �����ϵ���� /////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_dudv_ki = der_dudv_ki_brown(u,v);
// 	Matx<double,2,5> j_f_ki = j_xy_uv*j_dudv_ki;
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 	Matx<double,2,16> jacob;
// 	for (int i=0;i<2;++i)
// 	{
// 		for (int j=0;j<5;++j)
// 		{
// 			jacob(i,j)=j_f_fxfycxcys(i,j);
// 		}
// 
// 		for (int j=0;j<6;++j)
// 		{
// 			jacob(i,j+5)=j_f_wt(i,j);
// 		}
// 
// 		for (int j=0;j<5;++j)
// 		{
// 			jacob(i,j+11)=j_f_ki(i,j);
// 		}
// 	}
// 
// 	return jacob;
// }

void derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
											   const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
											   const Matx33d & mK,									// ��ǰ���ڲ�������
											   const Matx33d & mR,									// ��ǰ����̬����
											   const Matx31d & mt,									// ��ǰ��ƽ����������
											   double k1,double k2,double k3,double k4,double k5,	// ��ǰ�����ϵ������
											   int distType,										// ������ͣ�0 weng's, 1 brown's
											   Mat & J,												// �����	Jacobian����
											   Mat & f,												// �����	������Ŀ�꺯��ֵ
											   vector<double> & vds									// 
											   )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,16> jacob = j_f_fxfycxcys_w_t_k1k2k3k4k5(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<16;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
											  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
											  const Matx33d & mK,									// ��ǰ���ڲ�������
											  const Matx33d & mR,									// ��ǰ����̬����
											  const Matx31d & mt,									// ��ǰ��ƽ����������
											  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
											  int distType,											// ������ͣ�0 weng's, 1 brown's
											  Mat & J,												// �����	Jacobian����
											  Mat & f,												// �����	������Ŀ�꺯��ֵ
											  vector<double> & vds									// 
											  )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,15> jacob = j_f_fxfycxcy_w_t_k1k2k3k4k5(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<15;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
										   const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
										   const Matx33d & mK,									// ��ǰ���ڲ�������
										   const Matx33d & mR,									// ��ǰ����̬����
										   const Matx31d & mt,									// ��ǰ��ƽ����������
										   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
										   int distType,										// ������ͣ�0 weng's, 1 brown's
										   Mat & J,												// �����	Jacobian����
										   Mat & f,												// �����	������Ŀ�꺯��ֵ
										   vector<double> & vds									// 
										   )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,14> jacob = j_f_fcxcy_w_t_k1k2k3k4k5(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<14;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
									   const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
									   const Matx33d & mK,									// ��ǰ���ڲ�������
									   const Matx33d & mR,									// ��ǰ����̬����
									   const Matx31d & mt,									// ��ǰ��ƽ����������
									   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
									   int distType,										// ������ͣ�0 weng's, 1 brown's
									   Mat & J,												// �����	Jacobian����
									   Mat & f,												// �����	������Ŀ�꺯��ֵ
									   vector<double> & vds									// 
									   )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,12> jacob = j_f_f_w_t_k1k2k3k4k5(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<12;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
									  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
									  const Matx33d & mK,									// ��ǰ���ڲ�������
									  const Matx33d & mR,									// ��ǰ����̬����
									  const Matx31d & mt,									// ��ǰ��ƽ����������
									  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
									  int distType,											// ������ͣ�0 weng's, 1 brown's
									  Mat & J,												// �����	Jacobian����
									  Mat & f,												// �����	������Ŀ�꺯��ֵ
									  vector<double> & vds									// 
									  )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,11> jacob = j_f_fxfycxcy_w_t_k1(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<11;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
								   const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
								   const Matx33d & mK,									// ��ǰ���ڲ�������
								   const Matx33d & mR,									// ��ǰ����̬����
								   const Matx31d & mt,									// ��ǰ��ƽ����������
								   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
								   int distType,										// ������ͣ�0 weng's, 1 brown's
								   Mat & J,												// �����	Jacobian����
								   Mat & f,												// �����	������Ŀ�꺯��ֵ
								   vector<double> & vds									// 
								   )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,10> jacob = j_f_fcxcy_w_t_k1(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<10;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,					// ���룺		���Ƶ�����
									    const vector<Point2d> & vImgPts,					// ���룺		���Ƶ�۲��������
									    const Matx33d & mK,									// ��ǰ���ڲ�������
									    const Matx33d & mR,									// ��ǰ����̬����
									    const Matx31d & mt,									// ��ǰ��ƽ����������
									    double k1,double k2,double k3, double k4, double k5,// ��ǰ�����ϵ������
									    int distType,										// ������ͣ�0 weng's, 1 brown's
									    Mat & J,											// �����	Jacobian����
									    Mat & f,											// �����	������Ŀ�꺯��ֵ
									    vector<double> & vds								// 
									    )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,12> jacob = j_f_fxfycxcy_w_t_k1k2(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<12;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

void derivatives::j_f_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
									 const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
									 const Matx33d & mK,									// ��ǰ���ڲ�������
									 const Matx33d & mR,									// ��ǰ����̬����
									 const Matx31d & mt,									// ��ǰ��ƽ����������
									 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
									 int distType,											// ������ͣ�0 weng's, 1 brown's
									 Mat & J,												// �����	Jacobian����
									 Mat & f,												// �����	������Ŀ�꺯��ֵ
									 vector<double> & vds									// 
									 )
{
	int n = vWrdPts.size();

	for (int i=0;i<n;++i)
	{
		int idx_2i = 2*i;

		Point3d pt3d = vWrdPts[i];
		Point2d pt2d = vImgPts[i];

		double dx, dy;

		Matx<double,2,11> jacob = j_f_fcxcy_w_t_k1k2(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
			mK, mR, mt, k1, k2, k3, k4, k5, distType, dx, dy);

		double d = sqrt(dx*dx+dy*dy);
		vds[i] = d;

		f.at<double>(idx_2i)   = dx;
		f.at<double>(idx_2i+1) = dy;

		for (int jj=0;jj<11;++jj)
		{
			J.at<double>(idx_2i,jj)   = jacob(0,jj);
			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
		}
	}
}

// void derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
// 												   const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
// 												   const Matx33d & mK,									// ��ǰ���ڲ�������
// 												   const Matx33d & mR,									// ��ǰ����̬����
// 												   const Matx31d & mt,									// ��ǰ��ƽ����������
// 												   double k1,double k2,double k3,double k4,double k5,	// ��ǰ�����ϵ������
// 												   Mat & J,												// �����	Jacobian����
// 												   Mat & f,												// �����	������Ŀ�꺯��ֵ
// 												   vector<double> & vds									// 
// 												   )
// {
// 	int n = vWrdPts.size();
// 
// 	for (int i=0;i<n;++i)
// 	{
// 		int idx_2i = 2*i;
// 
// 		Point3d pt3d = vWrdPts[i];
// 		Point2d pt2d = vImgPts[i];
// 
// 		double dx, dy;
// 
// 		Matx<double,2,15> jacob = j_f_fxfycxcy_w_t_k1k2k3k4k5_weng(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
// 			mK, mR, mt, k1, k2, k3, k4, k5, dx, dy);
// 
// 		double d = sqrt(dx*dx+dy*dy);
// 		vds[i] = d;
// 
// 		f.at<double>(idx_2i)   = dx;
// 		f.at<double>(idx_2i+1) = dy;
// 
// 		for (int jj=0;jj<15;++jj)
// 		{
// 			J.at<double>(idx_2i,jj)   = jacob(0,jj);
// 			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
// 		}
// 	}
// }

// void derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
// 													 const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
// 													 const Matx33d & mK,									// ��ǰ���ڲ�������
// 													 const Matx33d & mR,									// ��ǰ����̬����
// 													 const Matx31d & mt,									// ��ǰ��ƽ����������
// 													 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
// 													 Mat & J,												// �����	Jacobian����
// 													 Mat & f,												// �����	������Ŀ�꺯��ֵ
// 													 vector<double> & vds									// 
// 													 )
// {
// 	int n = vWrdPts.size();
// 
// 	for (int i=0;i<n;++i)
// 	{
// 		int idx_2i = 2*i;
// 
// 		Point3d pt3d = vWrdPts[i];
// 		Point2d pt2d = vImgPts[i];
// 
// 		double dx, dy;
// 
// 		Matx<double,2,16> jacob = j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(pt3d.x, pt3d.y, pt3d.z, pt2d.x, pt2d.y,
// 			mK, mR, mt, k1, k2, k3, k4, k5, dx, dy);
// 
// 		double d = sqrt(dx*dx+dy*dy);
// 		vds[i] = d;
// 
// 		f.at<double>(idx_2i)   = dx;
// 		f.at<double>(idx_2i+1) = dy;
// 
// 		for (int jj=0;jj<16;++jj)
// 		{
// 			J.at<double>(idx_2i,jj)   = jacob(0,jj);
// 			J.at<double>(idx_2i+1,jj) = jacob(1,jj);
// 		}
// 	}
// }

// �����һ����������
void distortions::dudv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv)
{
	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;

	double r2 = u2 + v2;

	du = k1*r2*u + k2*r2 + k4*u2 + k5*uv; // k_1 r^2 u+k_2 r^2+k_4 u^2+k_5 uv
	dv = k1*r2*v + k3*r2 + k4*uv + k5*v2; // k_1 r^2 v+k_3 r^2+k_4 uv+k_5 v^2
}

// �����һ����������
// examined on 2015.04.22
void distortions::dudv_brown(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv)
{
	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;

	double r2 = u2 + v2;
	double r4 = r2 * r2;
	double r6 = r4 * r2;

	double f = k1*r2 + k2*r4 + k5*r6; // k_1 r^2+k_2 r^4+k_5 r^6

	du = f*u + 2*k4*u2 + 2*k3*uv + k4*r2; // fu+2k_4 u^2+2k_3 uv+k_4 r^2
	dv = f*v + 2*k3*v2 + 2*k4*uv + k3*r2;
}

// ������������
void distortions::dxdy_weng(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy)
{
	double du,dv;

	dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);

	dx = fx*du + s*dv; //dx = f_x du + s dv
	dy = fy*dv; // dy = f_y dv
}

// ������������
// examined on 2015.04.22
void distortions::dxdy_brown(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy)
{
	double du,dv;

	dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);

	dx = fx*du + s*dv; //dx = f_x du + s dv
	dy = fy*dv; // dy = f_y dv
}

// �Դ�������������ȥ����
void distortions::remove_weng(double fx, double fy, double cx, double cy, double s,
							  double k1, double k2, double k3, double k4, double k5,
							  double dist_x, double dist_y,
							  double & ideal_x, double & ideal_y,
							  int maxIter /*= 32*/, 
							  double eps /*= 1e-10*/
							  )
{
	ideal_x = dist_x;
	ideal_y = dist_y;

	double ideal_x_old = ideal_x;
	double ideal_y_old = ideal_y;

	Matx33d Kinv = calib::invK(fx,fy,cx,cy,s);

	Matx31d xy;
	xy(2) = 1;

	for (int i=0;i<maxIter;++i)
	{
		xy(0) = ideal_x;
		xy(1) = ideal_y;

		Matx31d nxy = Kinv*xy;

		double dx,dy;

		dxdy_weng(fx,fy,s,nxy(0),nxy(1),k1,k2,k3,k4,k5,dx,dy);

		ideal_x = dist_x - dx;
		ideal_y = dist_y - dy;

		if ((fabs(ideal_x-ideal_x_old) < eps) && (fabs(ideal_y-ideal_y_old) < eps))
		{
			break;
		}

		ideal_x_old = ideal_x;
		ideal_y_old = ideal_y;
	}
}

// �Դ�������������ȥ����
void distortions::remove_brown(double fx, double fy, double cx, double cy, double s,
							   double k1, double k2, double k3, double k4, double k5,
							   double dist_x, double dist_y,
							   double & ideal_x, double & ideal_y,
							   int maxIter /*= 32*/, 
							   double eps /*= 1e-10*/
							   )
{
	ideal_x = dist_x;
	ideal_y = dist_y;

	double ideal_x_old = ideal_x;
	double ideal_y_old = ideal_y;

	Matx33d Kinv = calib::invK(fx,fy,cx,cy,s);

	Matx31d xy;
	xy(2) = 1;

	for (int i=0;i<maxIter;++i)
	{
		xy(0) = ideal_x;
		xy(1) = ideal_y;

		Matx31d nxy = Kinv*xy;

		double dx,dy;

		dxdy_brown(fx,fy,s,nxy(0),nxy(1),k1,k2,k3,k4,k5,dx,dy);

		ideal_x = dist_x - dx;
		ideal_y = dist_y - dy;

		if ((fabs(ideal_x-ideal_x_old) < eps) && (fabs(ideal_y-ideal_y_old) < eps))
		{
			break;
		}

		ideal_x_old = ideal_x;
		ideal_y_old = ideal_y;
	}
}

// �������ڡ�Matrix Computations 3rd Edition��һ��P.216�е�Algorithm 5.1.3 (Computes Givens c and s)
// ���������[a; b]'����ʹ��[c s; -s c]' * [a; b]' = [r; 0]'������cos��sin
void calib::Givens(double a, double b, double & c, double & s)
{
	double lbd;

	if (fabs(b) < DBL_EPSILON)
	{
		c = 1;
		s = 0;
	} 
	else
	{
		if (fabs(b) > fabs(a))
		{
			lbd = -a / b;
			s = 1.0 / (sqrt(1 + lbd * lbd));
			c = s * lbd;
		} 
		else
		{
			lbd = -b / a;
			c = 1.0 / (sqrt(1 + lbd * lbd));
			s = c * lbd;
		}
	}
}

// ��m��n�ľ���A����RQ�ֽ⣨Ҫ��m<=n����A = RQ������QΪn��n����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
// ���õ���Givens��ת����ʵ�ֵģ�����Ҫ��m<=n��ԭ���ڱʼǱ�������ϸ����
void calib::RQ_Givens(const Mat & A, Mat & R, Mat & Q)
{
	if (A.rows > A.cols)
	{
		AfxMessageBox("����RQ�ֽ�ľ������������С�ڵ���������");

		return;
	}

	Mat A_cpy = A.clone();

	int m = A.rows;

	Matx22d G;
	Matx41d CS;
	vector<Matx41d> vCSs; // mCS��¼ÿ�μ��������cos��sin��i��k�Լ��Ƿ񻻷���
	double c, s;
	for (int i=m-1;i>0;--i)
	{
		for (int j=0;j<i;++j)
		{
			Matx12d ab;
			ab(0) = A_cpy.at<double>(i,j);
			ab(1) = A_cpy.at<double>(i,j+1);

			Givens(ab(0), ab(1), c, s);

			G(0,0) = s; G(0,1) = -c;
			G(1,0) = c; G(1,1) =  s;

			CS(0) = c;
			CS(1) = s;
			CS(2) = j;
			CS(3) = 1;

			Matx12d tmp = ab * G;

			if (j+1==i)
			{
				if (tmp(1) < 0)
				{
					G(0,1) =  c;
					G(1,1) = -s;
					CS(3)  = -1;
				}
			}

			for (int ii=0;ii<=i;++ii)
			{
				ab(0) = A_cpy.at<double>(ii,j);
				ab(1) = A_cpy.at<double>(ii,j+1);

				tmp = ab * G;

				A_cpy.at<double>(ii,j)   = tmp(0);
				A_cpy.at<double>(ii,j+1) = tmp(1);
			}
			A_cpy.at<double>(i,j) = 0;			

			vCSs.push_back(CS);
		} 
	}

	BOOL lastNegative = FALSE; // �����Ǿ����1�е�1�е�Ԫ���Ƿ�Ϊ����Ĭ���Ƿ�

	if (A_cpy.at<double>(0,0) < 0)
	{
		lastNegative = TRUE;

		A_cpy.at<double>(0,0) = - A_cpy.at<double>(0,0); // ��Ϊ�����򷴺ű�����������ñ�־λ
	}

	R = A_cpy.clone();

	Q = Mat(A.cols,A.cols,CV_64FC1,Scalar(0));

	for (int i=0;i<A.cols;++i)
	{
		Q.at<double>(i,i) = 1;
	}
	if (lastNegative)
	{
		Q.at<double>(0,0) = -1;
	}

	for (int i=vCSs.size()-1;i>=0;--i)
	{
		c=vCSs[i](0);
		s=vCSs[i](1);
		int j = vCSs[i](2);

		if (vCSs[i](3) > 0)
		{
			G(0,0) =  s; G(0,1) = c; 
			G(1,0) = -c; G(1,1) = s; 
		} 
		else
		{
			G(0,0) = s; G(0,1) =  c;
			G(1,0) = c; G(1,1) = -s;
		}

		Matx12d ab,tmp;
		for (int ii=0;ii<A.cols;++ii)
		{
			ab(0) = Q.at<double>(ii,j);
			ab(1) = Q.at<double>(ii,j+1);

			tmp = ab * G;

			Q.at<double>(ii,j)   = tmp(0);
			Q.at<double>(ii,j+1) = tmp(1);
		}
	}
}

// ��3��3�ľ���ʵʩRQ�ֽ�
void calib::RQ3x3_Givens(const Matx33d & A, Matx33d & R, Matx33d & Q)
{
	Mat A_mat(3,3,CV_64FC1);

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			A_mat.at<double>(i,j) = A(i,j);
		}
	}

	Mat R_mat,Q_mat;

	RQ_Givens(A_mat,R_mat,Q_mat);

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			R(i,j) = R_mat.at<double>(i,j);
			Q(i,j) = Q_mat.at<double>(i,j);
		}
	}
}

// ����ת����ת��Ϊ��ת����
Matx33d calib::converse_rotvec_R(double rv1, double rv2, double rv3)
{
	double radian = sqrt(rv1*rv1 + rv2*rv2 + rv3*rv3);

	Matx31d v;
	if (fabs(radian)<DBL_EPSILON) // if it is 1.0E-8 then it means less than 0.002 seconds
	{
		// if rotation angle is approximately 0, then the rotation axis will not be that
		// important again, we can choose any arbitrary unit axis, and the rotation matrix
		// acquired will still be approximately identity matrix
		v(0)=1;
		radian = 0; // Ҫ�ǻ���ֵ��С��ôֱ�ӽ�����Ϊ 0���Ա�֤�����γɵ���ת����϶�Ϊ��λ��
	}
	else
	{
		double rinv = 1.0/radian;
		
		v(0) = rv1*rinv;
		v(1) = rv2*rinv;
		v(2) = rv3*rinv;
	}

	double cosa = cos(radian);
	double sina = sin(radian);

	Matx33d c = cross_mat(v);

	Matx33d I;
	I(0,0) = I(1,1) = I(2,2) = 1;

	Matx33d R = I + sina * c + (1-cosa) * c * c;

	return R;
}

Matx33d calib::converse_rotvec_R(const Matx31d & v)
{
	return converse_rotvec_R(v(0),v(1),v(2));
}

Matx33d calib::converse_rotvec_R(const Matx13d & v)
{
	return converse_rotvec_R(v(0),v(1),v(2));
}

// ������Ԫ����������ת����
Matx33d calib::converse_quaternion_R(double a, double b, double c, double d)
{
	Matx33d R;

	double a2 = a*a;
	double b2 = b*b;
	double c2 = c*c;
	double d2 = d*d;

	double ab = 2*a*b;
	double ac = 2*a*c;
	double ad = 2*a*d;

	double bc = 2*b*c;
	double bd = 2*b*d;

	double cd = 2*c*d;

	R(0,0) = a2 + b2 - c2 - d2;	R(0,1) = -ad + bc;			R(0,2) = ac + bd;
	R(1,0) = ad + bc;			R(1,1) = a2 - b2 + c2 - d2;	R(1,2) = -ab + cd;
	R(2,0) = -ac + bd;			R(2,1) = ab + cd;			R(2,2) = a2 - b2 - c2 + d2;

	return R;
}

// given the rotational radian by axis-X, compute the rotation matrix
Matx33d calib::converse_radX_R(double rad)
{
	Matx33d R;

	double sa = sin(rad);
	double ca = cos(rad);

	R(0,0) = 1;
	R(1,1) = ca;
	R(1,2) = -sa;
	R(2,1) = sa;
	R(2,2) = ca;

	return R;
}

// given the rotational angle by axis-X, compute the rotation matrix
Matx33d calib::converse_angX_R(double ang)
{
	double rad = ang*D2R;

	return converse_radX_R(rad);
}

// given the rotational radian by axis-Y, compute the rotation matrix
Matx33d calib::converse_radY_R(double rad)
{
	Matx33d R;

	double sa = sin(rad);
	double ca = cos(rad);

	R(1,1) = 1;
	R(0,0) = ca;
	R(0,2) = sa;
	R(2,0) = -sa;
	R(2,2) = ca;

	return R;
}

// given the rotational angle by axis-Y, compute the rotation matrix
Matx33d calib::converse_angY_R(double ang)
{
	double rad = ang*D2R;

	return converse_radY_R(rad);
}

// given the rotational radian by axis-Z, compute the rotation matrix
Matx33d calib::converse_radZ_R(double rad)
{
	Matx33d R;

	double sa = sin(rad);
	double ca = cos(rad);

	R(2,2) = 1;
	R(0,0) = ca;
	R(0,1) = -sa;
	R(1,0) = sa;
	R(1,1) = ca;

	return R;
}

// given the rotational angle by axis-Z, compute the rotation matrix
Matx33d calib::converse_angZ_R(double ang)
{
	double rad = ang*D2R;

	return converse_radZ_R(rad);
}

// �����������ת���󣬷���һ�� 3 ά����������Ϊ ��|v|������ v Ϊ��ת��ĵ�λ�������� Ϊ��ת����
Matx31d calib::converse_R_rotvec(const Matx33d & mR)
{
	// according to information given on http://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
	double x = mR(2,1) - mR(1,2);
	double y = mR(0,2) - mR(2,0);
	double z = mR(1,0) - mR(0,1);
	double r = sqrt(x*x + y*y + z*z);
	double t = mR(0,0) + mR(1,1) + mR(2,2);
	double radian = atan2(r, t-1);

	Matx31d v;

	if (fabs(radian)<DBL_EPSILON) // if it is 1.0E-8 then it means less than 0.002 seconds
	{
		v(0)=0; v(1)=0;	v(2)=0;
	}
	else
	{
		double rinv = 1.0/r;
		x*=rinv; y*=rinv; z*=rinv;

		v(0) = x*radian;
		v(1) = y*radian;
		v(2) = z*radian;
	}

	return v;
}

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
Matx33d calib::normalize_2d(const vector<Point2f> & pts, vector<Point2f> & pts_t)
{
	int i;

	int n = pts.size();

	pts_t.clear();

	double sum_x = 0;
	double sum_y = 0;

	for (i=0;i<n;i++)
	{
		sum_x += pts[i].x;
		sum_y += pts[i].y;
	}

	double mean_x = sum_x/n;
	double mean_y = sum_y/n;

	double sum_d = 0;

	for (i=0;i<n;i++)
	{
		double x = pts[i].x - mean_x;
		double y = pts[i].y - mean_y;

		sum_d += sqrt(x*x+y*y);
	}

	double s = sqrt(2.0)*n/sum_d;

	Matx33d mT;
	mT(0,0) = s;
	mT(1,1) = s;
	mT(0,2) = -s*mean_x;
	mT(1,2) = -s*mean_y;
	mT(2,2) = 1;

	for (i=0;i<n;i++)
	{
		Matx31d mPt;
		mPt(0) = pts[i].x;
		mPt(1) = pts[i].y;
		mPt(2) = 1;

		Matx31d mPt_t = mT*mPt;

		Point2f pt_t;
		pt_t.x = mPt_t(0);
		pt_t.y = mPt_t(1);

		pts_t.push_back(pt_t);
	}

	return mT;
}

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
// tested on 2015.04.22
Matx33d calib::normalize_2d(const vector<Point2d> & pts, vector<Point2d> & pts_t)
{
	int i;

	int n = pts.size();

	pts_t.clear();

	double sum_x = 0;
	double sum_y = 0;

	for (i=0;i<n;i++)
	{
		sum_x += pts[i].x;
		sum_y += pts[i].y;
	}

	double mean_x = sum_x/n;
	double mean_y = sum_y/n;

	double sum_d = 0;

	for (i=0;i<n;i++)
	{
		double x = pts[i].x - mean_x;
		double y = pts[i].y - mean_y;

		sum_d += sqrt(x*x+y*y);
	}

	double s = sqrt(2.0)*n/sum_d;

	Matx33d mT;
	mT(0,0) = s;
	mT(1,1) = s;
	mT(0,2) = -s*mean_x;
	mT(1,2) = -s*mean_y;
	mT(2,2) = 1;

	for (i=0;i<n;i++)
	{
		Matx31d mPt;
		mPt(0) = pts[i].x;
		mPt(1) = pts[i].y;
		mPt(2) = 1;

		Matx31d mPt_t = mT*mPt;

		Point2d pt_t;
		pt_t.x = mPt_t(0);
		pt_t.y = mPt_t(1);

		pts_t.push_back(pt_t);
	}

	return mT;
}

// 20150421�������пռ��ƽ������������ϵ�������е㵽���ĵ�ƽ������Ϊ3^0.5
Matx44d calib::normalize_3d(const vector<Point3f> & pts, vector<Point3f> & pts_t)
{
	int n = pts.size();

	pts_t.clear();

	double sum_x = 0;
	double sum_y = 0;
	double sum_z = 0;

	for (int i=0;i<n;++i)
	{
		sum_x += pts[i].x;
		sum_y += pts[i].y;
		sum_z += pts[i].z;
	}

	double mean_x = sum_x/n;
	double mean_y = sum_y/n;
	double mean_z = sum_z/n;

	double sum_d = 0;

	for (int i=0;i<n;++i)
	{
		double x = pts[i].x - mean_x;
		double y = pts[i].y - mean_y;
		double z = pts[i].z - mean_z;

		sum_d += sqrt(x*x+y*y+z*z);
	}

	double s = sqrt(3.0)*n/sum_d;

	Matx44d mT;
	mT(0,0) = s;
	mT(1,1) = s;
	mT(2,2) = s;
	mT(0,3) = -s*mean_x;
	mT(1,3) = -s*mean_y;
	mT(2,3) = -s*mean_z;
	mT(3,3) = 1;

	for (int i=0;i<n;++i)
	{
		Matx41d mPt;
		mPt(0) = pts[i].x;
		mPt(1) = pts[i].y;
		mPt(2) = pts[i].z;
		mPt(3) = 1;

		Matx41d mPt_t = mT*mPt;

		Point3f pt_t;
		pt_t.x = mPt_t(0);
		pt_t.y = mPt_t(1);
		pt_t.z = mPt_t(2);

		pts_t.push_back(pt_t);
	}

	return mT;
}

// 20150421�������пռ��ƽ������������ϵ�������е㵽���ĵ�ƽ������Ϊ3^0.5
// tested on 2015.04.22
Matx44d calib::normalize_3d(const vector<Point3d> & pts, vector<Point3d> & pts_t)
{
	int n = pts.size();

	pts_t.clear();

	double sum_x = 0;
	double sum_y = 0;
	double sum_z = 0;

	for (int i=0;i<n;++i)
	{
		sum_x += pts[i].x;
		sum_y += pts[i].y;
		sum_z += pts[i].z;
	}

	double mean_x = sum_x/n;
	double mean_y = sum_y/n;
	double mean_z = sum_z/n;

	double sum_d = 0;

	for (int i=0;i<n;++i)
	{
		double x = pts[i].x - mean_x;
		double y = pts[i].y - mean_y;
		double z = pts[i].z - mean_z;

		sum_d += sqrt(x*x+y*y+z*z);
	}

	double s = sqrt(3.0)*n/sum_d;

	Matx44d mT;
	mT(0,0) = s;
	mT(1,1) = s;
	mT(2,2) = s;
	mT(0,3) = -s*mean_x;
	mT(1,3) = -s*mean_y;
	mT(2,3) = -s*mean_z;
	mT(3,3) = 1;

	for (int i=0;i<n;++i)
	{
		Matx41d mPt;
		mPt(0) = pts[i].x;
		mPt(1) = pts[i].y;
		mPt(2) = pts[i].z;
		mPt(3) = 1;

		Matx41d mPt_t = mT*mPt;

		Point3d pt_t;
		pt_t.x = mPt_t(0);
		pt_t.y = mPt_t(1);
		pt_t.z = mPt_t(2);

		pts_t.push_back(pt_t);
	}

	return mT;
}

Matx33d calib::cross_mat(const Matx31d & v)
{
	Matx33d m;

	m(0,1)=-v(2); m(0,2)= v(1);
	m(1,0)= v(2); m(1,2)=-v(0);
	m(2,0)=-v(1); m(2,1)= v(0);

	return m;
}

Matx33d calib::cross_mat(const Matx13d & v)
{
	Matx33d m;

	m(0,1)=-v(2); m(0,2)= v(1);
	m(1,0)= v(2); m(1,2)=-v(0);
	m(2,0)=-v(1); m(2,1)= v(0);

	return m;
}

// �������K����������
// tested on 2015.04.22
Matx33d calib::invK(const Matx33d & mK)
{
	Matx33d mInvK;

	double fx = mK(0,0); double fy = mK(1,1);
	double cx = mK(0,2); double cy = mK(1,2);
	double s  = mK(0,1);

	double fx_1 = 1.0/fx;
	double fy_1 = 1.0/fy;
	
	double fxfy_1 = fx_1*fy_1;

	mInvK(0,0) = fx_1;	mInvK(0,1) = -s*fxfy_1;	mInvK(0,2) = -(fy*cx - s*cy)*fxfy_1;
	mInvK(1,1) = fy_1;	mInvK(1,2) = -cy*fy_1;
	mInvK(2,2) = 1;

	return mInvK;
}

Matx33d calib::invK(double fx, double fy, double cx, double cy, double s)
{
	Matx33d mInvK;

	double fx_1 = 1.0/fx;
	double fy_1 = 1.0/fy;

	double fxfy_1 = fx_1*fy_1;

	mInvK(0,0) = fx_1;	mInvK(0,1) = -s*fxfy_1;	mInvK(0,2) = -(fy*cx - s*cy)*fxfy_1;
	mInvK(1,1) = fy_1;	mInvK(1,2) = -cy*fy_1;
	mInvK(2,2) = 1;

	return mInvK;
}

// wx=PX,����PΪ3��4�ľ���XΪ4��1������
Matx34d calib::dlt_34(const Mat & mX,	// ���룺	4��n �ľ���
					  const Mat & mx	// ���룺	2��n �ľ���
					  )
{
	Matx34d mP;

	int n = mX.cols;

	Mat mA(2*n, 12, CV_64FC1, Scalar(0));

	for (int i=0;i<n;++i)
	{
		double X = mX.at<double>(0,i);
		double Y = mX.at<double>(1,i);
		double Z = mX.at<double>(2,i);
		double W = mX.at<double>(3,i);

		double x = mx.at<double>(0,i);
		double y = mx.at<double>(1,i);

		mA.at<double>(2*i,0) = mA.at<double>(2*i+1,4) = X;
		mA.at<double>(2*i,1) = mA.at<double>(2*i+1,5) = Y;
		mA.at<double>(2*i,2) = mA.at<double>(2*i+1,6) = Z;
		mA.at<double>(2*i,3) = mA.at<double>(2*i+1,7) = W;

		mA.at<double>(2*i,8)  = -x*X;
		mA.at<double>(2*i,9)  = -x*Y;
		mA.at<double>(2*i,10) = -x*Z;
		mA.at<double>(2*i,11) = -x*W;

		mA.at<double>(2*i+1,8)  = -y*X;
		mA.at<double>(2*i+1,9)  = -y*Y;
		mA.at<double>(2*i+1,10) = -y*Z;
		mA.at<double>(2*i+1,11) = -y*W;
	}

	Mat mSol;

	SVD::solveZ(mA,mSol); // solve Ax=0

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<4;++j)
		{
			mP(i,j) = mSol.at<double>(i*4+j);
		}
	}

	return mP;
}

// wx=HX,����HΪ3��3�ľ���XΪ3��1������
Matx33d calib::dlt_33(const Mat & mX,	// ���룺	3��n �ľ���
					  const Mat & mx	// ���룺	2��n �ľ���
					  )
{
	Matx33d mH;

	int n = mX.cols;

	Mat mA(2*n, 9, CV_64FC1, Scalar(0));

	for (int i=0;i<n;++i)
	{
		double X = mX.at<double>(0,i);
		double Y = mX.at<double>(1,i);
		double Z = mX.at<double>(2,i);

		double x = mx.at<double>(0,i);
		double y = mx.at<double>(1,i);

		mA.at<double>(2*i,0) = mA.at<double>(2*i+1,3) = X;
		mA.at<double>(2*i,1) = mA.at<double>(2*i+1,4) = Y;
		mA.at<double>(2*i,2) = mA.at<double>(2*i+1,5) = Z;
	
		mA.at<double>(2*i,6) = -x*X;
		mA.at<double>(2*i,7) = -x*Y;
		mA.at<double>(2*i,8) = -x*Z;

		mA.at<double>(2*i+1,6) = -y*X;
		mA.at<double>(2*i+1,7) = -y*Y;
		mA.at<double>(2*i+1,8) = -y*Z;
	}

	Mat mSol;

	SVD::solveZ(mA,mSol); // solve Ax=0

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			mH(i,j) = mSol.at<double>(i*3+j);
		}
	}

	return mH;
}

// �����Ӧ�����Խ�aH�зֽ������ڲκ���ת���󣬲����س߶�����a
double calib::decomp_KR(const Matx33d & mKR,	// ���룺	3��3 ��aKR����Ҳ�������Ӧ
						Matx33d & mK,			// �����	3��3 ������ڲξ���
						Matx33d & mR			// �����	3��3 �������ת����
						)
{
	// RQ�ֽ�
	// ����OpenCV��RQ�ֽ⺯��������֤���жԽ���Ԫ�ض�Ϊ�������������K����ĵ�3��ȫΪ�����������������β��һ��û���ж����
	// ������Ǹ� K(3,3)�Ƿ�Ϊ�����ˣ�����Ҳ�����ã���ô��ȷ��K��Ϊ K=KQ��RΪ R=QR������Q = [1 0 0; 0 1 0; 0 0 -1]��
	// Ҳ���ǰ�K�ĵ�3�к�R�ĵ�3�ж����򼴿ɡ�
//	RQDecomp3x3(mKR, mK, mR);
	// 20150514����Ϊ�����Լ���д��RQ�ֽ⺯��
	RQ3x3_Givens(mKR, mK, mR);

	// ȷ���߶����ӵ�ģ|a|
	double a = mK(2,2);
	double a_1=1.0/a;

	mK=a_1*mK;

	mK(2,2) = 1; // ȷ���ڲξ���ĵ�3�е�3��Ϊ1.0��������ʲô0.9999999999989�����ž�����

	double detR = determinant(mR);

	if (detR<0)
	{
		mR=-mR;
		a=-a;
	}

	return a;
}

// ��ͶӰ��������Խ�aP�зֽ������������
void calib::decomp_P(const Matx34d & mP,// ���룺	3��4 ��ͶӰ����aP
				     Matx33d & mK,		// �����	3��3 ������ڲξ���
				     Matx33d & mR,		// �����	3��3 �������ת����
				     Matx31d & mt		// �����	3��1 �����ƽ������
				     )
{
	Matx33d mP33;
	Matx31d mPc4;

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			mP33(i,j)=mP(i,j);
		}

		mPc4(i)=mP(i,3);
	}

	double a = decomp_KR(mP33,mK,mR);

	double a_1=1.0/a;

	mt = a_1*invK(mK)*mPc4;
}

// �����Խ�a[R|t]�зֽ����������
void calib::decomp_R_t(const Matx34d & mR_t,// ���룺	3��4 ��a[R|t]
					   Matx33d & mR,		// �����	3��3 �������ת����
					   Matx31d & mt			// �����	3��1 �����ƽ������
					   )
{
	Matx33d mR_tmp;
	Matx31d mt_tmp;

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			mR_tmp(i,j)=mR_t(i,j);
		}
		mt_tmp(i) = mR_t(i,3);
	}

	double a3 = determinant(mR_tmp); // a^3
	double a = pow(a3,1.0/3);
	double a_1 = 1.0/a;

	mt = a_1*mt_tmp;

	Matx33d u,vt;
	Matx31d w;

	SVD::compute(mR_tmp, w, u, vt);

	if (a>0)
	{
		mR = u*vt;
	} 
	else
	{
		mR = -u*vt;
	}
}

// �����Խ�aK[r1 r2 t]�зֽ����������
void calib::decomp_H(const Matx33d & mH,// ���룺	3��3 ��aK[r1 r2 t]
				     const Matx33d & mK,// ���룺	��֪�� 3��3 ���ڲξ���
				     Matx33d & mR,		// �����	3��3 �������ת����
				     Matx31d & mt		// �����	3��1 �����ƽ������
				     )
{
	Matx31d r1,r2,t;

	for (int i=0;i<3;++i)
	{
		r1(i) = mH(i,0);
		r2(i) = mH(i,1);
		t(i)  = mH(i,2);
	}

	Matx33d K_1 = invK(mK);

	r1 = K_1*r1;
	r2 = K_1*r2;
	t  = K_1*t;

	double a1 = norm(r1);
	double a2 = norm(r2);

	double a = 0.5*(a1+a2);

	if (t(2)<0)
	{
		a1 = -a1;
		a2 = -a2;
		a  = -a;
	}

	double a1_1 = 1.0/a1;
	double a2_1 = 1.0/a2;
	double a_1  = 1.0/a;

	r1 = a1_1*r1;
	r2 = a2_1*r2;
	mt  = a_1*t;

	Matx31d r3 = cross_mat(r1)*r2; // r3=r1��r2
	double a3 = norm(r3);
	double a3_1 = 1.0/a3;

	r3 = a3_1*r3;

	Matx33d mR_tmp;
	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			mR_tmp(i,0) = r1(i);
			mR_tmp(i,1) = r2(i);
			mR_tmp(i,2) = r3(i);
		}
	}

	Matx33d u,vt;
	Matx31d w;

	SVD::compute(mR_tmp, w, u, vt);

	mR = u*vt;
}

// ����6�����Ͽ��Ƶ����Ա궨���ͶӰ����
Matx34d calib::calib_P(const vector<Point3d> & pts3d, const vector<Point2d> & pts2d)
{
	int npts = pts3d.size();

	Mat mX(4,npts,CV_64FC1);
	Mat mx(2,npts,CV_64FC1);

	for (int i=0;i<npts;++i)
	{
		mX.at<double>(0,i)=pts3d[i].x;
		mX.at<double>(1,i)=pts3d[i].y;
		mX.at<double>(2,i)=pts3d[i].z;
		mX.at<double>(3,i)=1;

		mx.at<double>(0,i)=pts2d[i].x;
		mx.at<double>(1,i)=pts2d[i].y;
	}

	return dlt_34(mX,mx);
}

// ����4���Ͽ��Ƶ�͹���λ�����Ա궨��������ӦH=KR
Matx33d calib::calib_KR(const vector<Point3d> & pts3d, const vector<Point2d> & pts2d, const Point3d & c)
{
	int npts = pts3d.size();

	Mat mX(3,npts,CV_64FC1);
	Mat mx(2,npts,CV_64FC1);

	for (int i=0;i<npts;++i)
	{
		mX.at<double>(0,i)=pts3d[i].x-c.x;
		mX.at<double>(1,i)=pts3d[i].y-c.y;
		mX.at<double>(2,i)=pts3d[i].z-c.z;

		mx.at<double>(0,i)=pts2d[i].x;
		mx.at<double>(1,i)=pts2d[i].y;
	}

	return dlt_33(mX,mx);
}

void optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
										      const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
										      Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
										      Matx33d & mR,						// ����������	���Ż����Ż������ת����
										      Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
										      double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
										      int distType /*= 0*/,				// ���룺	0 weng's, 1 brown's
										      double * info /*= NULL*/,			// output:	runtime info, 5-vector
																				// info[0]:	the initial reprojection error
																				// info[1]:	the final reprojection error
																				// info[2]: final max gradient
																				// info[3]: the number of iterations
																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
										      double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
										      int maxIter /*= 15*/,				// input:	the maximum number of iterations
										      double eps1 /*= 1.0E-8*/,			// input:	threshold
										      double eps2 /*= 1.0E-12*/			// input:	threshold
										      )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 16; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)+=h.at<double>(1);	// fy+=dfy
			mK_new(0,2)+=h.at<double>(2);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(3);	// cy+=dcy
			mK_new(0,1)+=h.at<double>(4);	// s +=ds

			double drv1 = h.at<double>(5);
			double drv2 = h.at<double>(6);
			double drv3 = h.at<double>(7);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(8);
			mt_new(1)+=h.at<double>(9);
			mt_new(2)+=h.at<double>(10);

			for (int i=0;i<5;++i)
			{
				dist_new[i]+=h.at<double>(11+i);
			}

			derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
											 const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
											 Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
											 Matx33d & mR,						// ����������	���Ż����Ż������ת����
											 Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
											 double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
											 int distType /*= 0*/,				// ���룺	0 weng's, 1 brown's
											 double * info /*= NULL*/,			// output:	runtime info, 5-vector
																				// info[0]:	the initial reprojection error
																				// info[1]:	the final reprojection error
																				// info[2]: final max gradient
																				// info[3]: the number of iterations
																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
											 double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
											 int maxIter /*= 15*/,				// input:	the maximum number of iterations
											 double eps1 /*= 1.0E-8*/,			// input:	threshold
											 double eps2 /*= 1.0E-12*/			// input:	threshold
											 )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 15; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)+=h.at<double>(1);	// fy+=dfy
			mK_new(0,2)+=h.at<double>(2);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(3);	// cy+=dcy

			double drv1 = h.at<double>(4);
			double drv2 = h.at<double>(5);
			double drv3 = h.at<double>(6);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(7);
			mt_new(1)+=h.at<double>(8);
			mt_new(2)+=h.at<double>(9);

			for (int i=0;i<5;++i)
			{
				dist_new[i]+=h.at<double>(10+i);
			}

			derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
									      const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
									      Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
									      Matx33d & mR,						// ����������	���Ż����Ż������ת����
									      Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
									      double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
									      int distType /*= 0*/,				// ���룺	0 weng's, 1 brown's
									      double * info /*= NULL*/,			// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									      double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									      int maxIter /*= 15*/,				// input:	the maximum number of iterations
									      double eps1 /*= 1.0E-8*/,			// input:	threshold
									      double eps2 /*= 1.0E-12*/			// input:	threshold
									      )
{
	double f_mean = (mK(0,0)+mK(1,1))*0.5;
	mK(0,0) = mK(1,1) = f_mean; // ȷ�� fx=fy

	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 14; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)=mK_new(0,0);		// fy = fx
			mK_new(0,2)+=h.at<double>(1);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(2);	// cy+=dcy

			double drv1 = h.at<double>(3);
			double drv2 = h.at<double>(4);
			double drv3 = h.at<double>(5);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(6);
			mt_new(1)+=h.at<double>(7);
			mt_new(2)+=h.at<double>(8);

			for (int i=0;i<5;++i)
			{
				dist_new[i]+=h.at<double>(9+i);
			}

			derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
								      const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
									  Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
									  Matx33d & mR,						// ����������	���Ż����Ż������ת����
									  Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
									  double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
									  int distType /*= 0*/,				// ���룺	0 weng's, 1 brown's
									  double * info /*= NULL*/,			// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									  double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									  int maxIter /*= 15*/,				// input:	the maximum number of iterations
									  double eps1 /*= 1.0E-8*/,			// input:	threshold
									  double eps2 /*= 1.0E-12*/			// input:	threshold
									  )
{
	double f_mean = (mK(0,0)+mK(1,1))*0.5;
	mK(0,0) = mK(1,1) = f_mean; // ȷ�� fx=fy

	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 12; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_f_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)=mK_new(0,0);		// fy = fx

			double drv1 = h.at<double>(1);
			double drv2 = h.at<double>(2);
			double drv3 = h.at<double>(3);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(4);
			mt_new(1)+=h.at<double>(5);
			mt_new(2)+=h.at<double>(6);

			for (int i=0;i<5;++i)
			{
				dist_new[i]+=h.at<double>(7+i);
			}

			derivatives::j_f_f_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
								     const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
								     Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
								     Matx33d & mR,						// ����������	���Ż����Ż������ת����
								     Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
								     double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
								     int distType /*= 0*/,				// ���룺	0 weng's, 1 brown's
								     double * info /*= NULL*/,			// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								     double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								     int maxIter /*= 15*/,				// input:	the maximum number of iterations
								     double eps1 /*= 1.0E-8*/,			// input:	threshold
								     double eps2 /*= 1.0E-12*/			// input:	threshold
								     )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 11; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcy_w_t_k1(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)+=h.at<double>(1);	// fy+=dfy
			mK_new(0,2)+=h.at<double>(2);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(3);	// cy+=dcy

			double drv1 = h.at<double>(4);
			double drv2 = h.at<double>(5);
			double drv3 = h.at<double>(6);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(7);
			mt_new(1)+=h.at<double>(8);
			mt_new(2)+=h.at<double>(9);

			for (int i=0;i<1;++i)
			{
				dist_new[i]+=h.at<double>(10+i);
			}

			derivatives::j_f_fxfycxcy_w_t_k1(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
							      const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
								  Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
								  Matx33d & mR,						// ����������	���Ż����Ż������ת����
								  Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
								  double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
								  int distType /*= 0*/,				// ���룺	0 weng's, 1 brown's
								  double * info /*= NULL*/,			// output:	runtime info, 5-vector
																	// info[0]:	the initial reprojection error
																	// info[1]:	the final reprojection error
																	// info[2]: final max gradient
																	// info[3]: the number of iterations
																	// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								  double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								  int maxIter /*= 15*/,				// input:	the maximum number of iterations
								  double eps1 /*= 1.0E-8*/,			// input:	threshold
								  double eps2 /*= 1.0E-12*/			// input:	threshold
								  )
{
	double f_mean = (mK(0,0)+mK(1,1))*0.5;
	mK(0,0) = mK(1,1) = f_mean; // ȷ�� fx=fy

	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 10; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fcxcy_w_t_k1(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)=mK_new(0,0);		// fy = fx
			mK_new(0,2)+=h.at<double>(1);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(2);	// cy+=dcy

			double drv1 = h.at<double>(3);
			double drv2 = h.at<double>(4);
			double drv3 = h.at<double>(5);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(6);
			mt_new(1)+=h.at<double>(7);
			mt_new(2)+=h.at<double>(8);

			for (int i=0;i<1;++i)
			{
				dist_new[i]+=h.at<double>(9+i);
			}

			derivatives::j_f_fcxcy_w_t_k1(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,// ���룺		���Ƶ�����
									   const vector<Point2d> & vImgPts,// ���룺		���Ƶ�۲��������
									   Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
									   Matx33d & mR,					// ����������	���Ż����Ż������ת����
									   Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
									   double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
									   int distType /*= 0*/,			// ���룺	0 weng's, 1 brown's
									   double * info /*= NULL*/,		// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									   double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									   int maxIter /*= 15*/,			// input:	the maximum number of iterations
									   double eps1 /*= 1.0E-8*/,		// input:	threshold
									   double eps2 /*= 1.0E-12*/		// input:	threshold
									   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 12; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcy_w_t_k1k2(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)+=h.at<double>(1);	// fy+=dfy
			mK_new(0,2)+=h.at<double>(2);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(3);	// cy+=dcy

			double drv1 = h.at<double>(4);
			double drv2 = h.at<double>(5);
			double drv3 = h.at<double>(6);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(7);
			mt_new(1)+=h.at<double>(8);
			mt_new(2)+=h.at<double>(9);

			for (int i=0;i<2;++i)
			{
				dist_new[i]+=h.at<double>(10+i);
			}

			derivatives::j_f_fxfycxcy_w_t_k1k2(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void optim::optim_lm_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,// ���룺		���Ƶ�����
								    const vector<Point2d> & vImgPts,// ���룺		���Ƶ�۲��������
								    Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
								    Matx33d & mR,					// ����������	���Ż����Ż������ת����
								    Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
								    double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
								    int distType /*= 0*/,			// ���룺	0 weng's, 1 brown's
								    double * info /*= NULL*/,		// output:	runtime info, 5-vector
																	// info[0]:	the initial reprojection error
																	// info[1]:	the final reprojection error
																	// info[2]: final max gradient
																	// info[3]: the number of iterations
																	// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								    double tau /*= 1.0E-3*/,		// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								    int maxIter /*= 15*/,			// input:	the maximum number of iterations
								    double eps1 /*= 1.0E-8*/,		// input:	threshold
								    double eps2 /*= 1.0E-12*/		// input:	threshold
								    )
{
	double f_mean = (mK(0,0)+mK(1,1))*0.5;
	mK(0,0) = mK(1,1) = f_mean; // ȷ�� fx=fy

	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 11; // number of variables to be optimized
	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	vector<double> vds(nPts);

	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fcxcy_w_t_k1k2(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx_old/nPts);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx33d mK_new = mK;
			Matx33d mR_new = mR;
			Matx31d mt_new = mt;
			double dist_new[5];
			memcpy(dist_new, dist, 5*sizeof(double));

			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
			mK_new(1,1)=mK_new(0,0);		// fy = fx
			mK_new(0,2)+=h.at<double>(1);	// cx+=dcx
			mK_new(1,2)+=h.at<double>(2);	// cy+=dcy

			double drv1 = h.at<double>(3);
			double drv2 = h.at<double>(4);
			double drv3 = h.at<double>(5);

			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
			// ����һֱ�۳���ȥ��֪���᲻����ֽϴ���ۻ����������յľ����ϸ�������ת��������ʣ�
			// Ҫ����������⣬��ô����������Ǻ�������һ��SVD�ֽ⣬R=UV'
			mR_new = dR*mR_new;

			mt_new(0)+=h.at<double>(6);
			mt_new(1)+=h.at<double>(7);
			mt_new(2)+=h.at<double>(8);

			for (int i=0;i<2;++i)
			{
				dist_new[i]+=h.at<double>(9+i);
			}

			derivatives::j_f_fcxcy_w_t_k1k2(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],distType,J,f,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				mK = mK_new;
				mR = mR_new;
				mt = mt_new;
				memcpy(dist, dist_new, 5*sizeof(double));

				A = J.t()*J;	// A=J(x)'J(x)
				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx_old/nPts);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// void optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,		// ���룺		���Ƶ�����
// 												   const vector<Point2d> & vImgPts,		// ���룺		���Ƶ�۲��������
// 												   Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
// 												   Matx33d & mR,						// ����������	���Ż����Ż������ת����
// 												   Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
// 												   double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
// 												   double * info /*= NULL*/,			// output:	runtime info, 5-vector
// 																						// info[0]:	the initial reprojection error
// 																						// info[1]:	the final reprojection error
// 																						// info[2]: final max gradient
// 																						// info[3]: the number of iterations
// 																						// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
// 												   double tau /*= 1.0E-3*/,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
// 												   int maxIter /*= 15*/,				// input:	the maximum number of iterations
// 												   double eps1 /*= 1.0E-8*/,			// input:	threshold
// 												   double eps2 /*= 1.0E-12*/			// input:	threshold
// 												   )
// {
// 	int k = 0;		// ������������
// 	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
// 	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
// 	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
// 	double g_norm;	// �ݶȵ�ģ
// 	double h_norm;	// ��������ģ
// 
// 	double ratio_1_3 = 1.0/3.0;
// 
// 	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
// 	int code = 2; // termination code
// 
// 	int nPts = vWrdPts.size(); // number of control points
// 	int N = 16; // number of variables to be optimized
// 	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��
// 
// 	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
// 	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
// 	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������
// 
// 	vector<double> vds(nPts);
// 
// 	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
// 	derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],J,f,vds);
// 	A = J.t()*J;	// A=J(x)'J(x)
// 	g = J.t()*f;	// g=J(x)'f(x)
// 
// 	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
// 	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
// 	for (int i=0;i<A.rows;++i)
// 	{
// 		I.at<double>(i,i) = 1;
// 	}
// 
// 	Mat tmp = 0.5*f.t()*f;
// 	double Fx_old = tmp.at<double>(0);
// 	double Fx_new, L0_Lh;
// 
// 	double err_rpj_init = sqrt(2*Fx_old/nPts);
// 
// 	g_norm = norm(g,NORM_INF);
// 
// 	// �ݶ�������˵������ƽ̹����
// 	if (g_norm < eps1)
// 	{
// 		found = true;
// 		code = 0;
// 	}
// 
// 	vector<double> Aii;
// 	for (int i=0;i<A.rows;++i)
// 	{
// 		Aii.push_back(A.at<double>(i,i));
// 	}
// 
// 	auto iter = max_element(Aii.begin(),Aii.end());
// 	double max_Aii = *iter;
// 
// 	u = tau * max_Aii; // initial tau
// 
// 	while (!found && k<maxIter)
// 	{
// 		++k;
// 
// 		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�
// 
// 		h_norm = norm(h);
// 
// 		if (h_norm < eps2) // ����������
// 		{
// 			found = true;
// 			code = 1;
// 		} 
// 		else
// 		{
// 			Matx33d mK_new = mK;
// 			Matx33d mR_new = mR;
// 			Matx31d mt_new = mt;
// 			double dist_new[5];
// 			memcpy(dist_new, dist, 5*sizeof(double));
// 
// 			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
// 			mK_new(1,1)+=h.at<double>(1);	// fy+=dfy
// 			mK_new(0,2)+=h.at<double>(2);	// cx+=dcx
// 			mK_new(1,2)+=h.at<double>(3);	// cy+=dcy
// 			mK_new(0,1)+=h.at<double>(4);	// s +=ds
// 
// 			double drv1 = h.at<double>(5);
// 			double drv2 = h.at<double>(6);
// 			double drv3 = h.at<double>(7);
// 
// 			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
// 			mR_new = dR*mR_new;
// 
// 			mt_new(0)+=h.at<double>(8);
// 			mt_new(1)+=h.at<double>(9);
// 			mt_new(2)+=h.at<double>(10);
// 
// 			for (int i=0;i<5;++i)
// 			{
// 				dist_new[i]+=h.at<double>(11+i);
// 			}
// 
// 			derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
// 				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],J,f,vds);
// 
// 			tmp = 0.5*f.t()*f;
// 			Fx_new = tmp.at<double>(0);
// 
// 			tmp = 0.5*h.t()*(u*h-g);
// 			L0_Lh = tmp.at<double>(0);
// 
// 			r = (Fx_old - Fx_new) / L0_Lh;
// 
// 			if (r>0)
// 			{
// 				mK = mK_new;
// 				mR = mR_new;
// 				mt = mt_new;
// 				memcpy(dist, dist_new, 5*sizeof(double));
// 
// 				A = J.t()*J;	// A=J(x)'J(x)
// 				g = J.t()*f;	// g=J(x)'f(x)
// 
// 				Fx_old = Fx_new;
// 
// 				g_norm = norm(g,NORM_INF);
// 
// 				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
// 				{
// 					found = true;
// 					code = 0;
// 				}
// 
// 				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
// 				u *= tmp_db;
// 				v = 2;
// 			} 
// 			else
// 			{
// 				u *= v;
// 				v *= 2;
// 			}
// 		}
// 	}
// 
// 	double err_rpj_final = sqrt(2*Fx_old/nPts);
// 
// 	if (info)
// 	{
// 		info[0] = err_rpj_init;
// 		info[1] = err_rpj_final;
// 		info[2] = g_norm;
// 		info[3] = k;
// 		info[4] = code;
// 	}
// }

// void optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
// 												    const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
// 												    Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
// 												    Matx33d & mR,						// ����������	���Ż����Ż������ת����
// 												    Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
// 												    double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
// 												    double * info /*= NULL*/,			// output:	runtime info, 5-vector
// 													  									// info[0]:	the initial reprojection error
// 																						// info[1]:	the final reprojection error
// 																						// info[2]: final max gradient
// 																						// info[3]: the number of iterations
// 																						// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
// 												    double tau /*= 1.0E-3*/,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
// 												    int maxIter /*= 15*/,				// input:	the maximum number of iterations
// 												    double eps1 /*= 1.0E-8*/,			// input:	threshold
// 												    double eps2 /*= 1.0E-12*/			// input:	threshold
// 												    )
// {
// 	int k = 0;		// ������������
// 	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
// 	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
// 	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
// 	double g_norm;	// �ݶȵ�ģ
// 	double h_norm;	// ��������ģ
// 
// 	double ratio_1_3 = 1.0/3.0;
// 
// 	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
// 	int code = 2; // termination code
// 
// 	int nPts = vWrdPts.size(); // number of control points
// 	int N = 16; // number of variables to be optimized
// 	int M = 2*nPts;	// ÿ�����Ƶ��ṩ 2 ���۲ⷽ��
// 
// 	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
// 	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
// 	Mat A,g,h; // A = J(x)'J(x) ���� F(x)" ; g = J(x)'f(x) Ϊ F(x)'; h Ϊ������
// 
// 	vector<double> vds(nPts);
// 
// 	// ���ɵ�һ�� A = J(x)'J(x)��g = J(x)'f(x) ///////////////////////////////////////////////////////
// 	derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],J,f,vds);
// 	A = J.t()*J;	// A=J(x)'J(x)
// 	g = J.t()*f;	// g=J(x)'f(x)
// 
// 	// ����һ���� A �ߴ�һ���ĵ�λ�� I����������������淽�� (A+uI)dx = -g
// 	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
// 	for (int i=0;i<A.rows;++i)
// 	{
// 		I.at<double>(i,i) = 1;
// 	}
// 
// 	Mat tmp = 0.5*f.t()*f;
// 	double Fx_old = tmp.at<double>(0);
// 	double Fx_new, L0_Lh;
// 
// 	double err_rpj_init = sqrt(2*Fx_old/nPts);
// 
// 	g_norm = norm(g,NORM_INF);
// 
// 	// �ݶ�������˵������ƽ̹����
// 	if (g_norm < eps1)
// 	{
// 		found = true;
// 		code = 0;
// 	}
// 
// 	vector<double> Aii;
// 	for (int i=0;i<A.rows;++i)
// 	{
// 		Aii.push_back(A.at<double>(i,i));
// 	}
// 
// 	auto iter = max_element(Aii.begin(),Aii.end());
// 	double max_Aii = *iter;
// 
// 	u = tau * max_Aii; // initial tau
// 
// 	while (!found && k<maxIter)
// 	{
// 		++k;
// 
// 		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�
// 
// 		h_norm = norm(h);
// 
// 		if (h_norm < eps2) // ����������
// 		{
// 			found = true;
// 			code = 1;
// 		} 
// 		else
// 		{
// 			Matx33d mK_new = mK;
// 			Matx33d mR_new = mR;
// 			Matx31d mt_new = mt;
// 			double dist_new[5];
// 			memcpy(dist_new, dist, 5*sizeof(double));
// 
// 			mK_new(0,0)+=h.at<double>(0);	// fx+=dfx
// 			mK_new(1,1)+=h.at<double>(1);	// fy+=dfy
// 			mK_new(0,2)+=h.at<double>(2);	// cx+=dcx
// 			mK_new(1,2)+=h.at<double>(3);	// cy+=dcy
// 			mK_new(0,1)+=h.at<double>(4);	// s +=ds
// 
// 			double drv1 = h.at<double>(5);
// 			double drv2 = h.at<double>(6);
// 			double drv3 = h.at<double>(7);
// 
// 			Matx33d dR = calib::converse_rotvec_R(drv1, drv2, drv3);
// 			mR_new = dR*mR_new;
// 
// 			mt_new(0)+=h.at<double>(8);
// 			mt_new(1)+=h.at<double>(9);
// 			mt_new(2)+=h.at<double>(10);
// 
// 			for (int i=0;i<5;++i)
// 			{
// 				dist_new[i]+=h.at<double>(11+i);
// 			}
// 
// 			derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(vWrdPts,vImgPts,mK_new,mR_new,mt_new,
// 				dist_new[0],dist_new[1],dist_new[2],dist_new[3],dist_new[4],J,f,vds);
// 
// 			tmp = 0.5*f.t()*f;
// 			Fx_new = tmp.at<double>(0);
// 
// 			tmp = 0.5*h.t()*(u*h-g);
// 			L0_Lh = tmp.at<double>(0);
// 
// 			r = (Fx_old - Fx_new) / L0_Lh;
// 
// 			if (r>0)
// 			{
// 				mK = mK_new;
// 				mR = mR_new;
// 				mt = mt_new;
// 				memcpy(dist, dist_new, 5*sizeof(double));
// 
// 				A = J.t()*J;	// A=J(x)'J(x)
// 				g = J.t()*f;	// g=J(x)'f(x)
// 
// 				Fx_old = Fx_new;
// 
// 				g_norm = norm(g,NORM_INF);
// 
// 				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
// 				{
// 					found = true;
// 					code = 0;
// 				}
// 
// 				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
// 				u *= tmp_db;
// 				v = 2;
// 			} 
// 			else
// 			{
// 				u *= v;
// 				v *= 2;
// 			}
// 		}
// 	}
// 
// 	double err_rpj_final = sqrt(2*Fx_old/nPts);
// 
// 	if (info)
// 	{
// 		info[0] = err_rpj_init;
// 		info[1] = err_rpj_final;
// 		info[2] = g_norm;
// 		info[3] = k;
// 		info[4] = code;
// 	}
// }

// 2015.10.08, find all tracks based on Carl Olsson's algorithm in <Stable structure from motion for unordered image collections>
// global minimum weight version
// 2015.10.22, �ѱ���Ȩ�ص�{<I,J>, weight}�ṹ�� map �ĳ� vector���Ӷ������ڽ��й켣�ں�֮ǰ�ȶ�ͼ��԰�ƥ������������map������ֻ�ܰ�key����
// ֮��Ͱ���ͼ���ȥ�����ںϼ��ɣ�����Ҫ��ÿ�ζ���map�а�ֵ��������ͼ��ԣ����������ɾ��
void SfM_ZZK::FindAllTracks_Olsson(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								   MultiTracks & map_tracks				// output:	all the found tracks
								   )
{
//	std::map<std::pair<int,int>,int> map_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::vector<std::pair<std::pair<int,int>,int>> vec_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	int n_features = 0; // number of all features

	// 1. record all the weights between each IJ //////////////////////////////////////////////////////////////////////////////////
	// and initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		vec_w_IJ.push_back(make_pair(iter->first, w_IJ));
		
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				OneTrack map_one_track;
//				map_one_track.insert(make_pair(I, iter_matches_IJ->queryIdx));
				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				OneTrack map_one_track;
//				map_one_track.insert(make_pair(J, iter_matches_IJ->trainIdx));
				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}
		}
	}

	// sort image pairs according to the number of matches found between them before merging
	sort(vec_w_IJ.begin(),vec_w_IJ.end(),
		[](const std::pair<std::pair<int,int>,int> & a, const std::pair<std::pair<int,int>,int> & b){return a.second>b.second;});

	// 2. merge non-conflicted tracks between currently the most weighted image pair ////////////////////////////////////////////////////
	for (auto iter_max=vec_w_IJ.begin();iter_max!=vec_w_IJ.end();++iter_max)
	{
		auto iter_max_IJ = map_matches.find(iter_max->first);

		const int & I = iter_max->first.first; // image I
		const int & J = iter_max->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter_max_IJ->second; // the pairwise matches found between image I and J

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks.find(iter_Jj->second);

			OneTrack track_merged;
			track_merged.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());
			track_merged.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

			if (track_merged.size() < (iter_track_Ii->second.size() + iter_track_Jj->second.size()))
			{
				// this means that there are conflicts between these two tracks ie the merged track contains more than one feature in one image
				// in this case we do not merge them
				continue;
			}

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
//					auto iter_tmp = map_trackID_Ii.find(*iter);
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
//					auto iter_tmp = map_trackID_Ii.find(*iter);
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Ii);
			}
		}
	}
}

// 20200622�������µ����ݽṹ
void SfM_ZZK::FindAllTracks_Olsson(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,// input:	all pairwise fundamental matrix F, matches and projective reconstruction
								   MultiTracks & map_tracks									// output:	all the found tracks
								   )
{
	//	std::map<std::pair<int,int>,int> map_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::vector<std::pair<std::pair<int,int>,int>> vec_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	int n_features = 0; // number of all features

	// 1. record all the weights between each IJ //////////////////////////////////////////////////////////////////////////////////
	// and initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter= map_F_matches_pWrdPts.begin(); iter!= map_F_matches_pWrdPts.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second.first.second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		vec_w_IJ.push_back(make_pair(iter->first, w_IJ));
		
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				OneTrack map_one_track;
				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				OneTrack map_one_track;
				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}
		}
	}

	// sort image pairs according to the number of matches found between them before merging
	sort(vec_w_IJ.begin(),vec_w_IJ.end(),
		[](const std::pair<std::pair<int,int>,int> & a, const std::pair<std::pair<int,int>,int> & b){return a.second>b.second;});

	// 2. merge non-conflicted tracks between currently the most weighted image pair ////////////////////////////////////////////////////
	for (auto iter_max=vec_w_IJ.begin();iter_max!=vec_w_IJ.end();++iter_max)
	{
		auto iter_max_IJ = map_F_matches_pWrdPts.find(iter_max->first);

		const int & I = iter_max->first.first; // image I
		const int & J = iter_max->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter_max_IJ->second.first.second; // the pairwise matches found between image I and J

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks.find(iter_Jj->second);

			OneTrack track_merged;
			track_merged.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());
			track_merged.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

			if (track_merged.size() < (iter_track_Ii->second.size() + iter_track_Jj->second.size()))
			{
				// this means that there are conflicts between these two tracks ie the merged track contains more than one feature in one image
				// in this case we do not merge them
				continue;
			}

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Ii);
			}
		}
	}
}

// 20220201�������µ����ݽṹ
void SfM_ZZK::FindAllTracks_Olsson(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise fundamental matrix F, matches and projective reconstruction
								   MultiTracksWithFlags & map_tracks							// output:	all the found tracks
								   )
{
	//	std::map<std::pair<int,int>,int> map_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::vector<std::pair<std::pair<int, int>, int>> vec_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::map<std::pair<int, int>, int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	int n_features = 0; // number of all features

	// 1. record all the weights between each IJ //////////////////////////////////////////////////////////////////////////////////
	// and initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter = map_F_matches_pWrdPts.begin(); iter != map_F_matches_pWrdPts.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second.first.second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		vec_w_IJ.push_back(make_pair(iter->first, w_IJ));

		for (auto iter_matches_IJ = vec_matches_IJ.begin(); iter_matches_IJ != vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
//				OneTrack map_one_track;
				trackWithFlags map_one_track;
//				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx, 0)));
				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx, std::vector<int>(1)))); // ÿ������Ȱ���һ����־λ������ʼ����ֵΪ0
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
//				OneTrack map_one_track;
				trackWithFlags map_one_track;
//				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx, 0)));
				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx, std::vector<int>(1)))); // ÿ������Ȱ���һ����־λ������ʼ����ֵΪ0
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}
		}
	}

	// sort image pairs according to the number of matches found between them before merging
	sort(vec_w_IJ.begin(), vec_w_IJ.end(),
		[](const std::pair<std::pair<int, int>, int> & a, const std::pair<std::pair<int, int>, int> & b) {return a.second > b.second; });

	// 2. merge non-conflicted tracks between currently the most weighted image pair ////////////////////////////////////////////////////
	for (auto iter_max = vec_w_IJ.begin(); iter_max != vec_w_IJ.end(); ++iter_max)
	{
		auto iter_max_IJ = map_F_matches_pWrdPts.find(iter_max->first);

		const int & I = iter_max->first.first; // image I
		const int & J = iter_max->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter_max_IJ->second.first.second; // the pairwise matches found between image I and J

		// start merging
		for (auto iter_matches_IJ = vec_matches_IJ.begin(); iter_matches_IJ != vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks.find(iter_Jj->second);

//			OneTrack track_merged;
			trackWithFlags track_merged;
			track_merged.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());
			track_merged.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

			if (track_merged.size() < (iter_track_Ii->second.size() + iter_track_Jj->second.size()))
			{
				// this means that there are conflicts between these two tracks ie the merged track contains more than one feature in one image
				// in this case we do not merge them
				continue;
			}

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter = iter_track_Jj->second.begin(); iter != iter_track_Jj->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first, iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Jj);
			}
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter = iter_track_Ii->second.begin(); iter != iter_track_Ii->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first, iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Ii);
			}
		}
	}
}

// 20151128���ϵ��������ƽṹ
void SfM_ZZK::FindAllTracks_Olsson(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								   MultiTracks_old & map_tracks				// output:	all the found tracks
								   )
{
//	std::map<std::pair<int,int>,int> map_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::vector<std::pair<std::pair<int,int>,int>> vec_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	int n_features = 0; // number of all features

	// 1. record all the weights between each IJ //////////////////////////////////////////////////////////////////////////////////
	// and initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		vec_w_IJ.push_back(make_pair(iter->first, w_IJ));
		
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				OneTrack_old map_one_track;
				map_one_track.insert(make_pair(I, iter_matches_IJ->queryIdx));
//				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				OneTrack_old map_one_track;
				map_one_track.insert(make_pair(J, iter_matches_IJ->trainIdx));
//				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}
		}
	}

	// sort image pairs according to the number of matches found between them before merging
	sort(vec_w_IJ.begin(),vec_w_IJ.end(),
		[](const std::pair<std::pair<int,int>,int> & a, const std::pair<std::pair<int,int>,int> & b){return a.second>b.second;});

	// 2. merge non-conflicted tracks between currently the most weighted image pair ////////////////////////////////////////////////////
	for (auto iter_max=vec_w_IJ.begin();iter_max!=vec_w_IJ.end();++iter_max)
	{
		auto iter_max_IJ = map_matches.find(iter_max->first);

		const int & I = iter_max->first.first; // image I
		const int & J = iter_max->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter_max_IJ->second; // the pairwise matches found between image I and J

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks.find(iter_Jj->second);

			OneTrack_old track_merged;
			track_merged.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());
			track_merged.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

			if (track_merged.size() < (iter_track_Ii->second.size() + iter_track_Jj->second.size()))
			{
				// this means that there are conflicts between these two tracks ie the merged track contains more than one feature in one image
				// in this case we do not merge them
				continue;
			}

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Ii);
			}
		}
	}
}

// 2015.10.08, find all tracks based on Carl Olsson's algorithm in <Stable structure from motion for unordered image collections>
// original version ie local minimum weight version with random starting image
void SfM_ZZK::FindAllTracks_Olsson_Original(const PairWiseMatches & map_matches,	// input:	all pairwise matches
										    MultiTracks & map_tracks				// output:	all the found tracks
										    )
{
	std::map<std::pair<int,int>,int> map_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	int n_features = 0; // number of all features

	int I_max = -1; // the max image index

	// 1. record all the weights between each IJ //////////////////////////////////////////////////////////////////////////////////
	// and initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		if (I>I_max)
		{
			I_max = I;
		}

		if (J>I_max)
		{
			I_max = J;
		}

		map_w_IJ.insert(make_pair(iter->first, w_IJ));

		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				OneTrack map_one_track;
//				map_one_track.insert(make_pair(I, iter_matches_IJ->queryIdx));
				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				OneTrack map_one_track;
//				map_one_track.insert(make_pair(J, iter_matches_IJ->trainIdx));
				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}
		}
	}

	// 2. merge non-conflicted tracks between currently the most weighted image pair ////////////////////////////////////////////////////
	std::map<std::pair<int,int>,int> map_w_IJ_cur; // contains all the weights: collection of {<I,J>, weight}
	int I_cur = I_max;
	for (auto iter=map_w_IJ.begin(); iter!=map_w_IJ.end(); ++iter)
	{
		if (iter->first.first==I_cur || iter->first.second==I_cur)
		{
			map_w_IJ_cur.insert(*iter);
		}
	}

	// image set that has already been grouped
	std::set<int> A;
	A.insert(I_cur);

	while (!map_w_IJ_cur.empty())
	{
		// find currently the most weighted image pair ie the one has the most matches
		auto iter_max = max_element(map_w_IJ_cur.begin(), map_w_IJ_cur.end(), 
			[](const std::pair<std::pair<int,int>,int> & a, const std::pair<std::pair<int,int>,int> & b){return a.second < b.second;});

		auto iter_max_IJ = map_matches.find(iter_max->first);

		const int & I = iter_max->first.first; // image I
		const int & J = iter_max->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter_max_IJ->second; // the pairwise matches found between image I and J

		//////////////////////////////////////////////////////////////////////////
		bool found_new = false;

		if (A.find(I)==A.end())
		{
			I_cur = I;
			found_new = true;
		}
		else
		{
			if (A.find(J)==A.end())
			{
				I_cur = J;
				found_new = true;
			}
		}

		if (found_new)
		{
			A.insert(I_cur);

			auto iter_find = map_w_IJ.find(iter_max->first);
			map_w_IJ.erase(iter_find);

			for (auto iter=map_w_IJ.begin(); iter!=map_w_IJ.end(); ++iter)
			{
				if (iter->first.first==I_cur || iter->first.second==I_cur)
				{
					map_w_IJ_cur.insert(*iter);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks.find(iter_Jj->second);

			OneTrack track_merged;
			track_merged.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());
			track_merged.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

			if (track_merged.size() < (iter_track_Ii->second.size() + iter_track_Jj->second.size()))
			{
				// this means that there are conflicts between these two tracks ie the merged track contains more than one feature in one image
				// in this case we do not merge them
				continue;
			}

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
//					auto iter_tmp = map_trackID_Ii.find(*iter);
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
//					auto iter_tmp = map_trackID_Ii.find(*iter);
					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Ii);
			}
		}

		// remove currently most weighted image pair
		map_w_IJ_cur.erase(iter_max);
	}
}

// 20151128���ϵ��������ƽṹ
void SfM_ZZK::FindAllTracks_Olsson_Original(const PairWiseMatches & map_matches,	// input:	all pairwise matches
										    MultiTracks_old & map_tracks				// output:	all the found tracks
										    )
{
	std::map<std::pair<int,int>,int> map_w_IJ; // contains all the weights: collection of {<I,J>, weight}
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	int n_features = 0; // number of all features

	int I_max = -1; // the max image index

	// 1. record all the weights between each IJ //////////////////////////////////////////////////////////////////////////////////
	// and initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		if (I>I_max)
		{
			I_max = I;
		}

		if (J>I_max)
		{
			I_max = J;
		}

		map_w_IJ.insert(make_pair(iter->first, w_IJ));

		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				OneTrack_old map_one_track;
				map_one_track.insert(make_pair(I, iter_matches_IJ->queryIdx));
//				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				OneTrack_old map_one_track;
				map_one_track.insert(make_pair(J, iter_matches_IJ->trainIdx));
//				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,0)));
				map_tracks.insert(make_pair(n_features, map_one_track));

				++n_features;
			}
		}
	}

	// 2. merge non-conflicted tracks between currently the most weighted image pair ////////////////////////////////////////////////////
	std::map<std::pair<int,int>,int> map_w_IJ_cur; // contains all the weights: collection of {<I,J>, weight}
	int I_cur = I_max;
	for (auto iter=map_w_IJ.begin(); iter!=map_w_IJ.end(); ++iter)
	{
		if (iter->first.first==I_cur || iter->first.second==I_cur)
		{
			map_w_IJ_cur.insert(*iter);
		}
	}

	// image set that has already been grouped
	std::set<int> A;
	A.insert(I_cur);

	while (!map_w_IJ_cur.empty())
	{
		// find currently the most weighted image pair ie the one has the most matches
		auto iter_max = max_element(map_w_IJ_cur.begin(), map_w_IJ_cur.end(), 
			[](const std::pair<std::pair<int,int>,int> & a, const std::pair<std::pair<int,int>,int> & b){return a.second < b.second;});

		auto iter_max_IJ = map_matches.find(iter_max->first);

		const int & I = iter_max->first.first; // image I
		const int & J = iter_max->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter_max_IJ->second; // the pairwise matches found between image I and J

		//////////////////////////////////////////////////////////////////////////
		bool found_new = false;

		if (A.find(I)==A.end())
		{
			I_cur = I;
			found_new = true;
		}
		else
		{
			if (A.find(J)==A.end())
			{
				I_cur = J;
				found_new = true;
			}
		}

		if (found_new)
		{
			A.insert(I_cur);

			auto iter_find = map_w_IJ.find(iter_max->first);
			map_w_IJ.erase(iter_find);

			for (auto iter=map_w_IJ.begin(); iter!=map_w_IJ.end(); ++iter)
			{
				if (iter->first.first==I_cur || iter->first.second==I_cur)
				{
					map_w_IJ_cur.insert(*iter);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks.find(iter_Jj->second);

			OneTrack_old track_merged;
			track_merged.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());
			track_merged.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

			if (track_merged.size() < (iter_track_Ii->second.size() + iter_track_Jj->second.size()))
			{
				// this means that there are conflicts between these two tracks ie the merged track contains more than one feature in one image
				// in this case we do not merge them
				continue;
			}

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first,iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks.erase(iter_track_Ii);
			}
		}

		// remove currently most weighted image pair
		map_w_IJ_cur.erase(iter_max);
	}
}

// 2015.10.21, find all tracks that are connected components, and do not contain any conflict at all
// this method finds the ever least tracks
void SfM_ZZK::FindAllTracks_Least(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								  MultiTracks & map_tracks				// output:	all the found tracks
								  )
{
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	typedef std::pair<int,int> pair_ij;
	typedef std::set<pair_ij> set_pair_ij;
	std::map<int,set_pair_ij> map_tracks_tmp;

	int n_features = 0; // number of all features

	// 1. initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				set_pair_ij set_one_track;
				set_one_track.insert(make_pair(I, iter_matches_IJ->queryIdx));
//				set_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,1)));
				map_tracks_tmp.insert(make_pair(n_features, set_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				set_pair_ij set_one_track;
				set_one_track.insert(make_pair(J, iter_matches_IJ->trainIdx));
//				set_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,1)));
				map_tracks_tmp.insert(make_pair(n_features, set_one_track));

				++n_features;
			}
		}
	}

	// 2. merge all tracks between currently image pair ////////////////////////////////////////////////////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks_tmp.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks_tmp.find(iter_Jj->second);

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first, iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks_tmp.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first, iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks_tmp.erase(iter_track_Ii);
			}
		}
	}

	// remove all the tracks containing conflicts
	for (auto iter=map_tracks_tmp.begin(); iter!=map_tracks_tmp.end(); ++iter)
	{
		OneTrack map_one_track;

		for (auto iter_ij=iter->second.begin(); iter_ij!=iter->second.end(); ++iter_ij)
		{
//			map_one_track.insert(make_pair(iter_ij->first,iter_ij->second));
			map_one_track.insert(make_pair(iter_ij->first,make_pair(iter_ij->second,0)));
		}

		if (map_one_track.size() < iter->second.size())
		{
			// there are conflicts
			continue;
		}

		map_tracks.insert(make_pair(iter->first, map_one_track));
	}
}

// 20151128���ϵ��������ƽṹ
void SfM_ZZK::FindAllTracks_Least(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								  MultiTracks_old & map_tracks				// output:	all the found tracks
								  )
{
	std::map<std::pair<int,int>,int> map_trackID_Ii; // contains all the features and their current trackID: collection of {<I,i>, trackID}

	typedef std::pair<int,int> pair_ij;
	typedef std::set<pair_ij> set_pair_ij;
	std::map<int,set_pair_ij> map_tracks_tmp;

	int n_features = 0; // number of all features

	// 1. initialize every feature as a single track, and build the mapping from map_trackID_Ii to map_tracks using trackID //////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J
		int w_IJ = vec_matches_IJ.size(); // number of matches, considered as the edge weight between any matches found between I and J

		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			// check if the query feature Ii has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(I, iter_matches_IJ->queryIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(I, iter_matches_IJ->queryIdx), n_features));
				set_pair_ij set_one_track;
				set_one_track.insert(make_pair(I, iter_matches_IJ->queryIdx));
//				set_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx,1)));
				map_tracks_tmp.insert(make_pair(n_features, set_one_track));

				++n_features;
			}

			// then check if the train feature Jj has already been grouped into some track
			if (map_trackID_Ii.find(make_pair(J, iter_matches_IJ->trainIdx)) == map_trackID_Ii.end())
			{
				// initialize each feature as a track
				map_trackID_Ii.insert(make_pair(make_pair(J, iter_matches_IJ->trainIdx), n_features));
				set_pair_ij set_one_track;
				set_one_track.insert(make_pair(J, iter_matches_IJ->trainIdx));
//				set_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx,1)));
				map_tracks_tmp.insert(make_pair(n_features, set_one_track));

				++n_features;
			}
		}
	}

	// 2. merge all tracks between currently image pair ////////////////////////////////////////////////////
	for (auto iter=map_matches.begin(); iter!=map_matches.end(); ++iter)
	{
		const int & I = iter->first.first; // image I
		const int & J = iter->first.second; // image J
		const std::vector<DMatch> & vec_matches_IJ = iter->second; // the pairwise matches found between image I and J

		// start merging
		for (auto iter_matches_IJ=vec_matches_IJ.begin(); iter_matches_IJ!=vec_matches_IJ.end(); ++iter_matches_IJ)
		{
			std::pair<int, int> idx_Ii = make_pair(I, iter_matches_IJ->queryIdx);
			std::pair<int, int> idx_Jj = make_pair(J, iter_matches_IJ->trainIdx);

			auto iter_Ii = map_trackID_Ii.find(idx_Ii);
			auto iter_Jj = map_trackID_Ii.find(idx_Jj);

			if (iter_Ii->second == iter_Jj->second)
			{
				// this means that features Ii and Jj are already grouped into the same track, no need to merge them
				continue;
			}

			auto iter_track_Ii = map_tracks_tmp.find(iter_Ii->second);
			auto iter_track_Jj = map_tracks_tmp.find(iter_Jj->second);

			// if there are no conflicts, two tracks are merged
			// the track with smaller trackID is augmented, whereas the other is erased
			// and the trackID of all features in the erased track are updated to the smaller trackID
			if (iter_Ii->second < iter_Jj->second)
			{
				int trackID = iter_track_Ii->first;

				// in this case track of Ii is kept
				iter_track_Ii->second.insert(iter_track_Jj->second.begin(), iter_track_Jj->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Jj->second.begin(); iter!=iter_track_Jj->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first, iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks_tmp.erase(iter_track_Jj);
			} 
			else
			{
				int trackID = iter_track_Jj->first;

				// in this case track of Jj is kept
				iter_track_Jj->second.insert(iter_track_Ii->second.begin(), iter_track_Ii->second.end());

				// update trackID of all features in the to-be-erased track
				for (auto iter=iter_track_Ii->second.begin(); iter!=iter_track_Ii->second.end(); ++iter)
				{
					auto iter_tmp = map_trackID_Ii.find(*iter);
//					auto iter_tmp = map_trackID_Ii.find(make_pair(iter->first, iter->second.first));
					iter_tmp->second = trackID;
				}

				// erase the track with bigger trackID
				map_tracks_tmp.erase(iter_track_Ii);
			}
		}
	}

	// remove all the tracks containing conflicts
	for (auto iter=map_tracks_tmp.begin(); iter!=map_tracks_tmp.end(); ++iter)
	{
		OneTrack_old map_one_track;

		for (auto iter_ij=iter->second.begin(); iter_ij!=iter->second.end(); ++iter_ij)
		{
			map_one_track.insert(make_pair(iter_ij->first,iter_ij->second));
//			map_one_track.insert(make_pair(iter_ij->first,make_pair(iter_ij->second,0)));
		}

		if (map_one_track.size() < iter->second.size())
		{
			// there are conflicts
			continue;
		}

		map_tracks.insert(make_pair(iter->first, map_one_track));
	}
}

// 2015.10.08, build the track length histogram
double SfM_ZZK::BuildTrackLengthHistogram(const MultiTracks & map_tracks,	// input:	all the tracks
									      std::map<int,int> & hist		// output:	the histogram
									      )
{
	for (auto iter = map_tracks.begin(); iter != map_tracks.end(); ++iter)
	{
		int length = iter->second.size();

		auto iter_find = hist.find(length);

		if (iter_find == hist.end())
		{
			// does not exist
			hist.insert(make_pair(length, 1));
		}
		else
		{
			// exist
			++iter_find->second;
		}
	}

	int n_total = 0;
	int sum_length = 0;

	for (auto iter = hist.begin(); iter != hist.end(); ++iter)
	{
		n_total += iter->second;
		sum_length += iter->first*iter->second;
	}

	return sum_length / (double)n_total;
}

// 20151128�������ϵĽṹ
double SfM_ZZK::BuildTrackLengthHistogram(const MultiTracks_old & map_tracks,	// input:	all the tracks
									      std::map<int,int> & hist				// output:	the histogram
									      )
{
	for (auto iter=map_tracks.begin(); iter!=map_tracks.end(); ++iter)
	{
		int length = iter->second.size();

		auto iter_find = hist.find(length);

		if (iter_find == hist.end())
		{
			// does not exist
			hist.insert(make_pair(length,1));
		}
		else
		{
			// exist
			++iter_find->second;
		}
	}

	int n_total = 0;
	int sum_length = 0;

	for (auto iter=hist.begin(); iter!=hist.end(); ++iter)
	{
		n_total+=iter->second;
		sum_length+=iter->first*iter->second;
	}

	return sum_length/(double)n_total;
}

// 2015.10.08, build the track length histogram
// 2015.10.21, and return the average track length
double SfM_ZZK::BuildTrackLengthHistogram(const vector<vector<Point2i>> & allTracks,	// input:	all the tracks
									      std::map<int,int> & hist					// output:	the histogram
									      )
{
	for (auto iter=allTracks.begin(); iter!=allTracks.end(); ++iter)
	{
		int length = iter->size();

		auto iter_find = hist.find(length);

		if (iter_find == hist.end())
		{
			// does not exist
			hist.insert(make_pair(length,1));
		}
		else
		{
			// exist
			++iter_find->second;
		}
	}

	int n_total = 0;
	int sum_length = 0;

	for (auto iter=hist.begin(); iter!=hist.end(); ++iter)
	{
		n_total+=iter->second;
		sum_length+=iter->first*iter->second;
	}

	return sum_length/(double)n_total;
}

// 2015.10.08, build the track length histogram
// 2015.10.21, and return the average track length
// 20220201�������½ṹ
double SfM_ZZK::BuildTrackLengthHistogram(const MultiTracksWithFlags & map_tracks,	// input:	all the tracks
										  std::map<int,int> & hist					// output:	the histogram
										  )
{
	for (auto iter = map_tracks.begin(); iter != map_tracks.end(); ++iter)
	{
		int length = iter->second.size();

		auto iter_find = hist.find(length);

		if (iter_find == hist.end())
		{
			// does not exist
			hist.insert(make_pair(length, 1));
		}
		else
		{
			// exist
			++iter_find->second;
		}
	}

	int n_total = 0;
	int sum_length = 0;

	for (auto iter = hist.begin(); iter != hist.end(); ++iter)
	{
		n_total += iter->second;
		sum_length += iter->first*iter->second;
	}

	return sum_length / (double)n_total;
}

// 20151103, zhaokunz, find image pair good for RO
// rank all image pairs according to the sum of track lengths
void SfM_ZZK::RankImagePairs_TrackLengthSum(const PairWiseMatches & map_matches,// input:	all pairwise matches
											const MultiTracks & map_tracks,		// input:	all the tracks
											vector<pair_ij_k> & pairs			// output:	pairs in descending order
											)
{
	pairs.clear();

	for (auto iter_pair_match=map_matches.begin();iter_pair_match!=map_matches.end();++iter_pair_match)
	{
		int count = 0;

		const int & I = iter_pair_match->first.first; // image I
		const int & J = iter_pair_match->first.second; // image J

		for (auto iter_track=map_tracks.begin();iter_track!=map_tracks.end();++iter_track)
		{
			const OneTrack & track = iter_track->second;

			auto iter_find_I = track.find(I); // first try find image I in this track

			if (iter_find_I==track.end())
			{
				continue;
			}

			auto iter_find_J = track.find(J); // if image I exist then try find image J in this track

			if (iter_find_J==track.end())
			{
				continue;
			}

			count += track.size(); // if this track contains both imge I and J, then add the length of this track to pair (I,J)
		}

		pair_ij_k ij_k;
		ij_k.first.first = I;
		ij_k.first.second = J;
		ij_k.second = count;

		pairs.push_back(ij_k);
	}

	// sort image pairs according to the sum of track lengths in descending order
	sort(pairs.begin(),pairs.end(),
		[](const std::pair<std::pair<int,int>,int> & a, const std::pair<std::pair<int,int>,int> & b){return a.second>b.second;});
}

// 20200622, zhaokunz, �����µ����ݽṹ
void SfM_ZZK::RankImagePairs_TrackLengthSum(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise matches
										    const MultiTracks & map_tracks,								// input:	all the tracks
										    vector<pair_ij_k> & pairs									// output:	pairs in descending order
										    )
{
	pairs.clear();

	for (auto iter_pair_match = map_F_matches_pWrdPts.begin(); iter_pair_match != map_F_matches_pWrdPts.end(); ++iter_pair_match)
	{
		int count = 0;

		const int & I = iter_pair_match->first.first; // image I
		const int & J = iter_pair_match->first.second; // image J

		for (auto iter_track = map_tracks.begin(); iter_track != map_tracks.end(); ++iter_track)
		{
			const OneTrack & track = iter_track->second;

			auto iter_find_I = track.find(I); // first try find image I in this track

			if (iter_find_I == track.end())
			{
				continue;
			}

			auto iter_find_J = track.find(J); // if image I exist then try find image J in this track

			if (iter_find_J == track.end())
			{
				continue;
			}

			count += track.size(); // if this track contains both imge I and J, then add the length of this track to pair (I,J)
		}

		pair_ij_k ij_k;
		ij_k.first.first = I;
		ij_k.first.second = J;
		ij_k.second = count;

		pairs.push_back(ij_k);
	}

	// sort image pairs according to the sum of track lengths in descending order
	sort(pairs.begin(), pairs.end(),
		[](const std::pair<std::pair<int, int>, int> & a, const std::pair<std::pair<int, int>, int> & b) {return a.second>b.second; });
}

// 20220201��zhaokunz�������µ����ݽṹ
void SfM_ZZK::RankImagePairs_TrackLengthSum(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise matches
										    const MultiTracksWithFlags & map_tracks,					// input:	all the tracks
										    vector<pair_ij_k> & pairs									// output:	pairs in descending order
										    )
{
	pairs.clear();

	for (auto iter_pair_match = map_F_matches_pWrdPts.begin(); iter_pair_match != map_F_matches_pWrdPts.end(); ++iter_pair_match)
	{
		int count = 0;

		const int & I = iter_pair_match->first.first; // image I
		const int & J = iter_pair_match->first.second; // image J

		for (auto iter_track = map_tracks.begin(); iter_track != map_tracks.end(); ++iter_track)
		{
//			const OneTrack & track = iter_track->second;
			const trackWithFlags & track = iter_track->second;

			auto iter_find_I = track.find(I); // first try find image I in this track

			if (iter_find_I == track.end())
			{
				continue;
			}

			auto iter_find_J = track.find(J); // if image I exist then try find image J in this track

			if (iter_find_J == track.end())
			{
				continue;
			}

			count += track.size(); // if this track contains both imge I and J, then add the length of this track to pair (I,J)
		}

		pair_ij_k ij_k;
		ij_k.first.first = I;
		ij_k.first.second = J;
		ij_k.second = count;

		pairs.push_back(ij_k);
	}

	// sort image pairs according to the sum of track lengths in descending order
	sort(pairs.begin(), pairs.end(),
		[](const std::pair<std::pair<int, int>, int> & a, const std::pair<std::pair<int, int>, int> & b) {return a.second > b.second; });
}

// 20151108���¼���һ��ͼ���Ҫǰ�������µĵ�
void SfM_ZZK::Triangulation_AddOneImg(PointCloud & map_pointcloud,				// output:	����
									  const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
									  const MultiTracks & map_tracks,			// input:	���е������켣
									  int idx_newImg,							// input:	����ɶ���ͼ���������
									  double thresh_inlier /*= 1.5*/			// input:	�����ж��ڵ����ͶӰ�в���ֵ
									  )
{
	// ͳ������Щ��Ҫ����ǰ������������켣
	typedef std::pair<Point3d,std::pair<int,double>> pair_XYZ_n_e; // <XYZi, ni, ei> �ֱ�洢��ѡ�������ꡢ�ڵ��������ͶӰ�в�
	typedef std::vector<pair_XYZ_n_e> vec_wrdpt_candidate;
	// ������ͳ�Ƶ���Ҫ����ǰ������������켣
	std::map<int,std::vector<pair_XYZ_n_e>> map_tracks_need_triang; // <ID, <XYZ1, n1, e1>, <XYZ2, n2, e2>, ...>

	// 1. ����ÿһ�������켣��ͳ������Щ�����켣��Ҫ����ǰ������
	for (auto iter_track=map_tracks.begin();iter_track!=map_tracks.end();++iter_track)
	{
		const int & trackID = iter_track->first;

		// �ȿ���ǰ�����켣�а��������¼���ͼ���е�������
		auto iter_found_newImgPt = iter_track->second.find(idx_newImg);
		if (iter_found_newImgPt==iter_track->second.end())
		{
			// ˵����ǰ����������켣�в������¼���ͼ���е�������
			continue;
		}

		// �ڰ�����ǰ�¼���ͼ���������ǰ���£��ٿ��������켣�Ƿ������ѱ��ؽ�����
		auto iter_found_objpt = map_pointcloud.find(trackID);
		if (iter_found_objpt!=map_pointcloud.end())
		{
			// ˵���ѱ��ؽ�����
			if (iter_found_newImgPt->second.second)
			{
				// ���¼���ͼ���е��������ѱ���Ϊ�ڵ㣬��ʱʲô��������
				continue;
			} 

			// ���е���˵���¼���ͼ���������㱻��Ϊ��㣬��ʱ��������������ߣ���������ɶ����ͼ�񽻻ᣬ�����ҵ����õ�����������
			// �����Ѿ���һ����ѡ������������
			vec_wrdpt_candidate vec_candidates;
			vec_candidates.push_back(make_pair(iter_found_objpt->second.m_pt,make_pair(0,0)));
			map_tracks_need_triang.insert(make_pair(trackID,vec_candidates));
		} 
		else
		{
			// ˵����δ���ؽ���������Ҫ����ǰ������
			// ��Ŀǰû���κ����еĺ�ѡ����������
			vec_wrdpt_candidate vec_candidates;
			map_tracks_need_triang.insert(make_pair(trackID,vec_candidates));
		}
	}

	// �¼���ͼ��Ĳ���
	const cam_data & cam = cams[idx_newImg];
	const Matx33d & mK = cam.m_K;
	const Matx33d & mR = cam.m_R;
	const Matx31d & mt = cam.m_t;

	// 2. �����¼����ͼ���ÿһ���Ѷ���õ�ͼ��ʵʩǰ������
	for (int i=0;i<cams.size();++i)
	{
		const cam_data & cam_other = cams[i];

		if (!cam_other.m_bOriented || i==idx_newImg)
		{
			// ��ͼ��û�ж����
			continue;
		}

		// �� i ��ͼ��Ĳ���
		const Matx33d & mK_other = cam_other.m_K;
		const Matx33d & mR_other = cam_other.m_R;
		const Matx31d & mt_other = cam_other.m_t;

		vector<int> trackIDs;
		vector<Point2d> imgpts;
		vector<Point2d> imgpts_other;

		// ����ÿ����Ҫ����ǰ������������켣������Щ�����켣ͬʱ���� i ��ͼ���¼����ͼ��۲⵽
		for (auto iter_track_need_triang=map_tracks_need_triang.begin();iter_track_need_triang!=map_tracks_need_triang.end();++iter_track_need_triang)
		{
			const int & trackID = iter_track_need_triang->first;

			auto iter_track = map_tracks.find(trackID);

			auto iter_found_imgi = iter_track->second.find(i);

			if (iter_found_imgi==iter_track->second.end())
			{
				// �ڵ� i ����û���ҵ�
				continue;
			}

			auto iter_found_newImg = iter_track->second.find(idx_newImg);

			// ��¼����ͼ�ϵ��������
			Point2d imgpt, imgpt_other;

			imgpt.x = cam.m_feats.key_points[iter_found_newImg->second.first].pt.x;
			imgpt.y = cam.m_feats.key_points[iter_found_newImg->second.first].pt.y;

			imgpt_other.x = cam_other.m_feats.key_points[iter_found_imgi->second.first].pt.x;
			imgpt_other.y = cam_other.m_feats.key_points[iter_found_imgi->second.first].pt.y;

			trackIDs.push_back(trackID);
			imgpts.push_back(imgpt);
			imgpts_other.push_back(imgpt_other);
		}

		// 20151125, �п�����ͼ֮��û���κ���Ҫ���н���ĵ㣬��ʱֱ��continue
		if (imgpts.size()<1)
		{
			continue;
		}

		// ִ������˫Ŀǰ������
		vector<Point3d> wrdpts;
		vector<Point2d> errs;
		double rpj_err;
		try
		{
			rpj_err = Triangulate_Optimal(imgpts_other, mK_other, mR_other, mt_other, imgpts, mK, mR, mt, wrdpts, errs);
		}
		catch (cv::Exception & e)
		{
			CString str;
			str = e.msg.c_str();
			str+="	error happened in Triangulate_Optimal";
			AfxMessageBox(str);
		}

		for (int j=0;j<wrdpts.size();++j)
		{
			const Point3d & pt3d = wrdpts[j];

			if (errs[j].x >= thresh_inlier || errs[j].y >= thresh_inlier)
			{
				continue;
			}

			// 20151224������������������������λ�ڲ��뽻�������ͼǰ�棬����Ͳ������ˣ���������Ϊһ��ѡ�������
 			Matx31d XYZ;
 			XYZ(0) = pt3d.x;
 			XYZ(1) = pt3d.y;
 			XYZ(2) = pt3d.z;
 
 			Matx31d XYZ_0 = mR*XYZ+mt;
 			Matx31d XYZ_1 = mR_other*XYZ+mt_other;
 
 			if (XYZ_0(2)<=0 || XYZ_1(2)<=0)
 			{
 				// ˵��������һ��ͼ����λ�ڸ��ؽ�����ǰ����
 				continue;
 			}

			// ¼��һ����Ч���������
			pair_XYZ_n_e XYZ_n_e = make_pair(pt3d/*wrdpts[j]*/,make_pair(0,0));
			
			auto iter_found_track_need_triang = map_tracks_need_triang.find(trackIDs[j]);
			iter_found_track_need_triang->second.push_back(XYZ_n_e);
		}
	}

	// 3. �Խ���ǰ����������������켣�ĺ�ѡ�������������ۣ����ȿ��ڵ㼯�Ĵ�С�����ڵ㼯��С��ͬ�������ѡ���ۺ���ͶӰ�в���С���Ǹ�����������
	for (auto iter_track_need_triang=map_tracks_need_triang.begin();iter_track_need_triang!=map_tracks_need_triang.end();++iter_track_need_triang)
	{
		const int & trackID = iter_track_need_triang->first;

		auto iter_track = map_tracks.find(trackID);

		vector<pair_XYZ_n_e> & vec_XYZ_n_e = iter_track_need_triang->second;

		// �����һ����ѡ�����������ƶ�û�еĻ���ֱ�Ӽ�����һ��
		if (vec_XYZ_n_e.size()<1)
		{
			continue;
		}

		// ��������ÿ����ѡ������
		for (auto iter_one_candidate=vec_XYZ_n_e.begin();iter_one_candidate!=vec_XYZ_n_e.end();++iter_one_candidate)
		{
			const Point3d & wrdpt = iter_one_candidate->first;

			Matx31d XYZ;
			XYZ(0) = wrdpt.x;
			XYZ(1) = wrdpt.y;
			XYZ(2) = wrdpt.z;

			int n_inliers = 0;
			double sum_inliers_d2 = 0;

			for (auto iter_oneimgpt=iter_track->second.begin();iter_oneimgpt!=iter_track->second.end();++iter_oneimgpt)
			{
				const int & I = iter_oneimgpt->first;
				const int & i = iter_oneimgpt->second.first;

				const cam_data & camI = cams[I];

				if (!camI.m_bOriented)
				{
					// û��ɶ����ͼ��Ͳ�����ͶӰ
					continue;
				}

				// ȡ���۲���
				Point2d imgpt;
				imgpt.x = camI.m_feats.key_points[i].pt.x;
				imgpt.y = camI.m_feats.key_points[i].pt.y;

				// �����ͶӰ
				Matx31d xyz, XYZ_C;
				try
				{
					XYZ_C = camI.m_R*XYZ + camI.m_t;
					xyz = camI.m_K*XYZ_C;
				}
				catch (cv::Exception & e)
				{
					CString str;
					str = e.msg.c_str();
					str+="	error happened in compute projection";
					AfxMessageBox(str);
				}
				xyz(0)/=xyz(2);
				xyz(1)/=xyz(2);

				double dx = xyz(0)-imgpt.x;
				double dy = xyz(1)-imgpt.y;
				double d = sqrt(dx*dx+dy*dy);

				// 20151227������������ͶӰ�в��㹻С�������⣬���ñ�֤���λ��ͼ��ǰ��
				if (d<thresh_inlier && XYZ_C(2)>0)
				{
					++iter_one_candidate->second.first;
					sum_inliers_d2 += d*d;
					++n_inliers;
				}
			}

			iter_one_candidate->second.second = sqrt(sum_inliers_d2/n_inliers);
		}

		// �Ȱ��ڵ㼯��С�Ӵ�С����
		sort(vec_XYZ_n_e.begin(),vec_XYZ_n_e.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.first>b.second.first;});

		int n_inliers_max = vec_XYZ_n_e[0].second.first;

		if (n_inliers_max<2)
		{
			// ���ٵ��� 2 ��ͼ����Ϊ�ڵ�
			continue;
		}

		vector<pair_XYZ_n_e> vec_XYZ_n_e_max;
		for (int i=0;i<vec_XYZ_n_e.size();++i)
		{
			if (vec_XYZ_n_e[i].second.first==n_inliers_max)
			{
				vec_XYZ_n_e_max.push_back(vec_XYZ_n_e[i]);
			}
		}

		// �����ڵ㼯��С��ͬ�ģ��ٰ�����ͶӰ�в��С��������
		sort(vec_XYZ_n_e_max.begin(),vec_XYZ_n_e_max.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.second<b.second.second;});

		auto iter_found_wrdpt = map_pointcloud.find(trackID);
		if (iter_found_wrdpt!=map_pointcloud.end())
		{
			// ����Ѿ������˾�ֱ�Ӹ�������
			iter_found_wrdpt->second.m_pt = vec_XYZ_n_e_max[0].first;
		} 
		else
		{
			// ����������ڣ��򿪱��µ�
			CloudPoint cloudpt;
			cloudpt.m_idx = trackID;
			cloudpt.m_pt = vec_XYZ_n_e_max[0].first;

			map_pointcloud.insert(make_pair(trackID,cloudpt));
		}

// 		CloudPoint cloudpt;
// 		cloudpt.m_idx = trackID;
// 		cloudpt.m_pt = vec_XYZ_n_e_max[0].first;
// 
// 		map_pointcloud.insert(make_pair(trackID,cloudpt));
	}
}

// 20151111�����������켣�е�ǰ������ɶ����ͼ��ǰ�����ᣬÿ��������õ�һ�����꣬����ȡ֧�ּ������������
int SfM_ZZK::Triangulation_AllImgs(PointCloud & map_pointcloud,			// output:	����
								    const vector<DeepVoid::cam_data> & cams,// input:	����ͼ��
								    const MultiTracks & map_tracks,			// input:	���е������켣
								    double thresh_inlier /*= 1.5*/			// input:	�����ж��ڵ����ͶӰ�в���ֵ
								    )
{
	typedef std::pair<Point3d,std::pair<int,double>> pair_XYZ_n_e; // <XYZi, ni, ei> �ֱ�洢��ѡ�������ꡢ�ڵ��������ͶӰ�в�
	typedef std::vector<pair_XYZ_n_e> vec_wrdpt_candidate;

	int n_inliers_changed = 0;

	for (auto iter_track=map_tracks.begin();iter_track!=map_tracks.end();++iter_track)
	{
		const int & trackID = iter_track->first;

		// ������еĺ�ѡ�������
		vec_wrdpt_candidate vec_XYZ_n_e;

		auto iter_found_wrdpt = map_pointcloud.find(trackID);
		if (iter_found_wrdpt!=map_pointcloud.end())
		{
			// �ѱ��ؽ�����
			pair_XYZ_n_e XYZ_n_e = make_pair(iter_found_wrdpt->second.m_pt,make_pair(0,0));
			vec_XYZ_n_e.push_back(XYZ_n_e);
		}

		int n_curInliers = 0; // ��ǰ�ռ�����ӵ�е��ڵ���

//		vector<int> vIdx_orientedImgs; // ��������ɶ����ͼ������
		vector<Point2d> vImgPts; // ��������ɶ����ͼ���е��������
		vector<Matx33d> vKs; // ����ɶ����ͼ���ڲ���
		vector<Matx33d> vRs; // ����ɶ����ͼ����ת����
		vector<Matx31d> vts; // ����ɶ����ͼ��ƽ������

		for (auto iter_Ii=iter_track->second.begin();iter_Ii!=iter_track->second.end();++iter_Ii)
		{
			const int & I = iter_Ii->first;
			const int & i = iter_Ii->second.first;
			const int & bInlier = iter_Ii->second.second;
			const cam_data & cam_I = cams[I];

			if (!cam_I.m_bOriented)
			{
				continue;
			}

			Point2d imgpt;
			imgpt.x = cam_I.m_feats.key_points[i].pt.x;
			imgpt.y = cam_I.m_feats.key_points[i].pt.y;
			vImgPts.push_back(imgpt);
			vKs.push_back(cam_I.m_K);
			vRs.push_back(cam_I.m_R);
			vts.push_back(cam_I.m_t);

			if (bInlier)
			{
				++n_curInliers;
			}
		}

		// ����ͼ���������˫Ŀ�������ɺ�ѡ�������
		for (int i=0;i<vImgPts.size();++i)
		{
			for (int j=i+1;j<vImgPts.size();++j)
			{
				vector<Point2d> imgpts_other, imgpts;
				imgpts_other.push_back(vImgPts[i]);
				imgpts.push_back(vImgPts[j]);				

				// ִ������˫Ŀǰ������
				vector<Point3d> wrdpts;
				vector<Point2d> errs;
				double rpj_err = Triangulate_Optimal(imgpts_other, vKs[i], vRs[i], vts[i], imgpts, vKs[j], vRs[j], vts[j], wrdpts, errs);

				if (errs[0].x >= thresh_inlier || errs[0].y >= thresh_inlier)
				{
					continue;
				}

				// ¼��һ����Ч���������
				pair_XYZ_n_e XYZ_n_e = make_pair(wrdpts[0],make_pair(0,0));
				vec_XYZ_n_e.push_back(XYZ_n_e);
			}
		}

		// �����һ����ѡ�����������ƶ�û�еĻ���ֱ�Ӽ�����һ��
		if (vec_XYZ_n_e.size()<1)
		{
			continue;
		}

		// ��������ÿ����ѡ������
		for (auto iter_one_candidate=vec_XYZ_n_e.begin();iter_one_candidate!=vec_XYZ_n_e.end();++iter_one_candidate)
		{
			const Point3d & wrdpt = iter_one_candidate->first;

			Matx31d XYZ;
			XYZ(0) = wrdpt.x;
			XYZ(1) = wrdpt.y;
			XYZ(2) = wrdpt.z;

			int n_inliers = 0;
			double sum_inliers_d2 = 0;

			for (int i=0;i<vKs.size();++i)
			{
				// ȡ���۲���
				Point2d imgpt = vImgPts[i];
				
				// �����ͶӰ
				Matx31d xyz = vKs[i]*(vRs[i]*XYZ+vts[i]);
				xyz(0)/=xyz(2);
				xyz(1)/=xyz(2);

				double dx = xyz(0)-imgpt.x;
				double dy = xyz(1)-imgpt.y;
				double d = sqrt(dx*dx+dy*dy);

				if (d<thresh_inlier)
				{
					++iter_one_candidate->second.first;
					sum_inliers_d2 += d*d;
					++n_inliers;
				}
			}

			iter_one_candidate->second.second = sqrt(sum_inliers_d2/n_inliers);
		}

		if (iter_found_wrdpt!=map_pointcloud.end())
		{
			n_curInliers = vec_XYZ_n_e[0].second.first;
		}

		// �Ȱ��ڵ㼯��С�Ӵ�С����
		sort(vec_XYZ_n_e.begin(),vec_XYZ_n_e.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.first>b.second.first;});

		int n_inliers_max = vec_XYZ_n_e[0].second.first;

		if (n_inliers_max<2)
		{
			// ���ٵ��� 2 ��ͼ����Ϊ�ڵ�
			continue;
		}

		vector<pair_XYZ_n_e> vec_XYZ_n_e_max;
		for (int i=0;i<vec_XYZ_n_e.size();++i)
		{
			if (vec_XYZ_n_e[i].second.first==n_inliers_max)
			{
				vec_XYZ_n_e_max.push_back(vec_XYZ_n_e[i]);
			}
		}

		// �����ڵ㼯��С��ͬ�ģ��ٰ�����ͶӰ�в��С��������
		sort(vec_XYZ_n_e_max.begin(),vec_XYZ_n_e_max.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.second<b.second.second;});

		if (iter_found_wrdpt!=map_pointcloud.end())
		{
//			if (n_curInliers<n_inliers_max/* && n_curInliers>1*/)
			if ((n_inliers_max-n_curInliers)>1)
			{
				++n_inliers_changed;
			}

			// ����Ѿ������˾�ֱ�Ӹ�������
			iter_found_wrdpt->second.m_pt = vec_XYZ_n_e_max[0].first;
		} 
		else
		{
			// ����������ڣ��򿪱��µ�
			CloudPoint cloudpt;
			cloudpt.m_idx = trackID;
			cloudpt.m_pt = vec_XYZ_n_e_max[0].first;

			map_pointcloud.insert(make_pair(trackID,cloudpt));
		}

// 		if (n_curInliers<n_inliers_max)
// 		{
// 			++n_inliers_changed;
// 		}
	}

	return n_inliers_changed;
}

// 20151109�������ǰ����
void SfM_ZZK::OutputPointCloud(CString strFile,							// input:	����ļ�·��
							   const PointCloud & map_pointcloud,		// int:		����
							   const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
							   const MultiTracks & map_tracks,			// input:	���е������켣
							   vector<DeepVoid::CloudPoint> & cloud,	// output:	�ϵĵ��ƽṹ��
							   int n_minInilier /*= 2*/					// input:	���ٵ��иø���ͼ��۲⵽�õ�
							   )
{
	cloud.clear();

	FILE * file = fopen(strFile, "w");
	for (auto iter_wrdpt=map_pointcloud.begin();iter_wrdpt!=map_pointcloud.end();++iter_wrdpt)
	{
		const int & trackID = iter_wrdpt->first;

		auto iter_found_track = map_tracks.find(trackID);

		double sumR = 0;
		double sumG = 0;
		double sumB = 0;

		int count = 0;

		for (auto iter_imgpt=iter_found_track->second.begin();iter_imgpt!=iter_found_track->second.end();++iter_imgpt)
		{
			const int & I = iter_imgpt->first;
			const int & i = iter_imgpt->second.first;
			const int & bInlier = iter_imgpt->second.second;

			if (!bInlier)
			{
				continue;
			}

			const cam_data & cam = cams[I];

			sumR += cam.m_feats.rgbs[i][2];
			sumG += cam.m_feats.rgbs[i][1];
			sumB += cam.m_feats.rgbs[i][0];

			++count;
		}

		if (count<n_minInilier)
		{
			continue;
		}

		int R = (int)sumR/count;
		int G = (int)sumG/count;
		int B = (int)sumB/count;

		fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", iter_wrdpt->second.m_pt.x, iter_wrdpt->second.m_pt.y, iter_wrdpt->second.m_pt.z, R, G, B);

		CloudPoint cldpt;
		cldpt.m_idx = trackID;
		cldpt.m_pt = iter_wrdpt->second.m_pt;

		cloud.push_back(cldpt);
	}
	fclose(file);
}

// 20151109�������ǰ����
void SfM_ZZK::OutputPointCloud(CString strFile,							// input:	����ļ�·��
							   const PointCloud & map_pointcloud,		// input:	����
							   const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
							   const MultiTracksWithFlags & map_tracks,	// input:	���е������켣
							   vector<DeepVoid::CloudPoint> & cloud,	// output:	�ϵĵ��ƽṹ��
							   int n_minInilier /*= 2*/					// input:	���ٵ��иø���ͼ��۲⵽�õ�
							   )
{
	cloud.clear();

	FILE * file = fopen(strFile, "w");
	for (auto iter_wrdpt = map_pointcloud.begin(); iter_wrdpt != map_pointcloud.end(); ++iter_wrdpt)
	{
		const int & trackID = iter_wrdpt->first;

		auto iter_found_track = map_tracks.find(trackID);

		double sumR = 0;
		double sumG = 0;
		double sumB = 0;

		int count = 0;

		for (auto iter_imgpt = iter_found_track->second.begin(); iter_imgpt != iter_found_track->second.end(); ++iter_imgpt)
		{
			const int & I = iter_imgpt->first;
			const int & i = iter_imgpt->second.first;
			const int & bInlier = iter_imgpt->second.second[0]; // ��һ����־λ��ʾ�Ƿ�Ϊinlier

			if (!bInlier)
			{
				continue;
			}

			const cam_data & cam = cams[I];

			sumR += cam.m_feats.rgbs[i][2];
			sumG += cam.m_feats.rgbs[i][1];
			sumB += cam.m_feats.rgbs[i][0];

			++count;
		}

		if (count < n_minInilier)
		{
			continue;
		}

		int R = (int)sumR / count;
		int G = (int)sumG / count;
		int B = (int)sumB / count;

		fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", iter_wrdpt->second.m_pt.x, iter_wrdpt->second.m_pt.y, iter_wrdpt->second.m_pt.z, R, G, B);

		CloudPoint cldpt;
		cldpt.m_idx = trackID;
		cldpt.m_pt = iter_wrdpt->second.m_pt;

		cloud.push_back(cldpt);
	}
	fclose(file);
}

// 20160405���ѵ�ǰ�ĵ��ƺ�ͼ���������Viz3d������
void SfM_ZZK::Draw3DScene(viz::Viz3d & wnd3d,						// output:	3D��ʾ�Ĵ���
	                      viz::WCloud & cld,
						  const PointCloud & map_pointcloud,		// input:	����
						  const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ�����
						  const MultiTracks & map_tracks,			// input:	���е������켣
						  int n_minInilier /*= 2*/					// input:	���ٵ��иø���ͼ��۲⵽�õ�
						  )
{
	// Ҫ��ʾ�����ĵ��Ƶ㼰����ɫ
	vector<Point3d> pts_output;
	vector<Point3i> colors_output;

	// ͳ�ƿ���Щ������Ҫ�������ʾ
	for (auto iter_wrdpt = map_pointcloud.begin(); iter_wrdpt != map_pointcloud.end(); ++iter_wrdpt)
	{
		const int & trackID = iter_wrdpt->first;

		auto iter_found_track = map_tracks.find(trackID);

		double sumR = 0;
		double sumG = 0;
		double sumB = 0;

		int count = 0;

		for (auto iter_imgpt = iter_found_track->second.begin(); iter_imgpt != iter_found_track->second.end(); ++iter_imgpt)
		{
			const int & I = iter_imgpt->first;
			const int & i = iter_imgpt->second.first;
			const int & bInlier = iter_imgpt->second.second;

			if (!bInlier)
			{
				continue;
			}

			const cam_data & cam = cams[I];

			sumR += cam.m_feats.rgbs[i][2];
			sumG += cam.m_feats.rgbs[i][1];
			sumB += cam.m_feats.rgbs[i][0];

			++count;
		}

		if (count<n_minInilier)
		{
			continue;
		}

		int R = (int)sumR / count;
		int G = (int)sumG / count;
		int B = (int)sumB / count;

		Point3i color;
		color.x = R;
		color.y = G;
		color.z = B;

		pts_output.push_back(iter_wrdpt->second.m_pt);
		colors_output.push_back(color);
	}

	int n_pts_output = pts_output.size();

	Mat XYZs(1, n_pts_output, CV_64FC3), RGBs(1, n_pts_output, CV_8UC3);

	for (int i = 0; i < n_pts_output; ++i)
	{
		Point3d XYZ = pts_output[i];
		Point3i RGB = colors_output[i];

		XYZs.at<Vec3d>(i).val[0] = XYZ.x;
		XYZs.at<Vec3d>(i).val[1] = XYZ.y;
		XYZs.at<Vec3d>(i).val[2] = XYZ.z;

		RGBs.at<Vec3b>(i).val[0] = RGB.z;
		RGBs.at<Vec3b>(i).val[1] = RGB.y;
		RGBs.at<Vec3b>(i).val[2] = RGB.x;
	}

//	viz::WCloud cld_new(XYZs, RGBs);

	cld = viz::WCloud(XYZs, RGBs);

	wnd3d.showWidget("cloud", cld);

	wnd3d.spinOnce();

	

//	viz::Viz3d wnd3dnew = viz::getWindowByName("3D Window");
//	wnd3dnew.showWidget("cloud", cld);
//	wnd3dnew.spinOnce();
}

// 20151112,ͳ��ÿ�����Ƶ㱻�۲������ֱ��ͼ
double SfM_ZZK::BuildCloudPointInlierHistogram(const PointCloud & map_pointcloud,	// output:	����
											   const MultiTracks & map_tracks,		// input:	���е������켣
											   std::map<int,int> & hist				// output:	the histogram
											   )
{
	for (auto iter_wrdpt=map_pointcloud.begin();iter_wrdpt!=map_pointcloud.end();++iter_wrdpt)
	{
		const int & trackID = iter_wrdpt->first;

		auto iter_found_track = map_tracks.find(trackID);

		int count = 0;

		for (auto iter_imgpt=iter_found_track->second.begin();iter_imgpt!=iter_found_track->second.end();++iter_imgpt)
		{
			const int & I = iter_imgpt->first;
			const int & i = iter_imgpt->second.first;
			const int & bInlier = iter_imgpt->second.second;

			if (!bInlier)
			{
				continue;
			}

			++count;
		}

		auto iter_found_num = hist.find(count);

		if (iter_found_num == hist.end())
		{
			// does not exist
			hist.insert(make_pair(count,1));
		}
		else
		{
			// exist
			++iter_found_num->second;
		}
	}

	int n_total = 0;
	int sum_length = 0;

	for (auto iter=hist.begin(); iter!=hist.end(); ++iter)
	{
		n_total+=iter->second;
		sum_length+=iter->first*iter->second;
	}

	return sum_length/(double)n_total;
}

// optimize Ri based on Rotation Averaging using Newton-Raphson method
// ��ϸ�ο� Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> �е� Algorithm A2
void SfM_ZZK::optim_nr_Ri_Govindu04(const vector<Matx33d> & vRijs,	// ���룺		���й۲�������ת����
								    const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
								    vector<Matx33d> & vRis,			// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
								    int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
								    int * nIter /*= NULL*/,			// �����		ʵ�ʵ�������
								    double * residue /*= NULL*/,	// �����		���յĲв��С
								    int max_iter/* = 32*/,			// ���룺		����������
								    double xEps /*= 1.0E-12*/,		// ���룺		��ֹ����
								    double fEps /*= 1.0E-12*/		// ���룺		��ֹ����
								    )
{
	int n_all = vRis.size(); // ͼ���ܸ���
	int m = vRijs.size(); // ���۲�������ת�ĸ���

	int n_optim;
	if (idx_ref<0)
	{
		// �ο�ͼ������Ϊ-1��˵��û�вο�ͼ������ͼ�����̬���Ż�
		n_optim = n_all;
	} 
	else
	{
		// ��ʱ˵���вο�ͼ���Ż����˲ο�ͼ֮�������ͼ�����̬
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// �ȶ���ÿ�����Ż�ͼ����Jacobian�����г�ʼ��������
	vector<int> vJj;
	if (idx_ref<0)
	{
		for (int i=0;i<n_all;++i)
		{
			vJj.push_back(i);
		}
	} 
	else
	{
		for (int i=0;i<n_all;++i)
		{
			if (i>idx_ref)
			{
				vJj.push_back(i-1);
			} 
			else if (i==idx_ref)
			{
				vJj.push_back(-1);
			}
			else
			{
				vJj.push_back(i);
			}
		}
	}
	
	// ���ɹ̶���Jacobian����
	for (int k=0;k<m;++k)
	{
		int k3 = 3*k;

		int i = vijs[k].x;
		int j = vijs[k].y;

		int i3 = 3*vJj[i];
		int j3 = 3*vJj[j];

		if (i!=idx_ref)
		{
			J.at<double>(k3,   i3)   = -1;
			J.at<double>(k3+1, i3+1) = -1;
			J.at<double>(k3+2, i3+2) = -1;
		} 

		if (j!=idx_ref)
		{
			J.at<double>(k3,   j3)   = 1;
			J.at<double>(k3+1, j3+1) = 1;
			J.at<double>(k3+2, j3+2) = 1;
		}
	}
	
	double fVal_old = 0;
	double fVal_new;
	double hVal;

	int nnn = 0;
	double rrr;

	// ��ʼ�����Ż�
	for (int kk=0;kk<max_iter;++kk)
	{
		// �����ڵ�ǰ��̬�����µĲв���
		for (int k=0;k<m;++k)
		{
			int k3 = 3*k;

			int i = vijs[k].x;
			int j = vijs[k].y;

//			Matx33d dRij = vRijs[k]*vRis[i]*vRis[j].t();		// dRij = RijRiRj'		wij+wi-wj=0
//			Matx33d dRij = vRis[j]*vRis[i].t()*vRijs[k].t();	// dRij = RjRi'Rij'		wj-wi-wij=0
			Matx33d dRij = vRis[j].t()*vRijs[k]*vRis[i];		// dRij = Rj'RijRi		-wj+wij+wi=0

			Matx31d dwij = calib::converse_R_rotvec(dRij);

			f.at<double>(k3)   = dwij(0);
			f.at<double>(k3+1) = dwij(1);
			f.at<double>(k3+2) = dwij(2);
		}
		
		Mat h;

		// J(x0)dx = -f(x0)
		solve(J, f, h, DECOMP_QR);

		fVal_new = norm(f)*norm(f);
		hVal     = norm(h);

		double df2 = fVal_new - fVal_old;

		rrr = sqrt(fVal_new/m);

		if ((fabs(df2) < fEps) || (hVal < xEps))
		{
			break;
		}

		++nnn;

		fVal_old = fVal_new;

		for (int i=0;i<n_all;++i)
		{
			if (i==idx_ref)
			{
				continue;
			}

			int i3 = 3*vJj[i];

			double drv1 = h.at<double>(i3);
			double drv2 = h.at<double>(i3+1);
			double drv3 = h.at<double>(i3+2);

			Matx33d dRi = calib::converse_rotvec_R(drv1, drv2, drv3);

//			vRis[i] = dRi*vRis[i];
			vRis[i] = vRis[i]*dRi;
		}
	}

	if (nIter)
	{
		*nIter = nnn;
	}

	if (residue)
	{
		*residue = rrr;
	}
}

// optimize Ri based on Rotation Averaging using Newton-Raphson method
// ��ϸ�ο� Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> �е� Algorithm A2
void SfM_ZZK::optim_nr_Ri(const vector<Matx33d> & vRijs,// ���룺		���й۲�������ת����
						  const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
						  vector<Matx33d> & vRis,		// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
						  int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
						  int * nIter /*= NULL*/,		// �����		ʵ�ʵ�������
						  double * residue /*= NULL*/,	// �����		���յĲв��С
						  int max_iter /*= 32*/,		// ���룺		����������
						  double xEps /*= 1.0E-12*/,	// ���룺		��ֹ����
						  double fEps /*= 1.0E-12*/		// ���룺		��ֹ����
						  )
{
	int n_all = vRis.size(); // ͼ���ܸ���
	int m = vRijs.size(); // ���۲�������ת�ĸ���

	int n_optim;
	if (idx_ref<0)
	{
		// �ο�ͼ������Ϊ-1��˵��û�вο�ͼ������ͼ�����̬���Ż�
		n_optim = n_all;
	} 
	else
	{
		// ��ʱ˵���вο�ͼ���Ż����˲ο�ͼ֮�������ͼ�����̬
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// �ȶ���ÿ�����Ż�ͼ����Jacobian�����г�ʼ��������
	vector<int> vJj;
	if (idx_ref<0)
	{
		for (int i=0;i<n_all;++i)
		{
			vJj.push_back(i);
		}
	} 
	else
	{
		for (int i=0;i<n_all;++i)
		{
			if (i>idx_ref)
			{
				vJj.push_back(i-1);
			} 
			else if (i==idx_ref)
			{
				vJj.push_back(-1);
			}
			else
			{
				vJj.push_back(i);
			}
		}
	}
	
	// ���ɹ̶���Jacobian����
	for (int k=0;k<m;++k)
	{
		int k3 = 3*k;

		int i = vijs[k].x;
		int j = vijs[k].y;

		int i3 = 3*vJj[i];
		int j3 = 3*vJj[j];

		if (i!=idx_ref)
		{
			J.at<double>(k3,   i3)   = -1;
			J.at<double>(k3+1, i3+1) = -1;
			J.at<double>(k3+2, i3+2) = -1;
		} 

		if (j!=idx_ref)
		{
			J.at<double>(k3,   j3)   = 1;
			J.at<double>(k3+1, j3+1) = 1;
			J.at<double>(k3+2, j3+2) = 1;
		}
	}
	
	double fVal_old = 0;
	double fVal_new;
	double hVal;

	int nnn = 0;
	double rrr;

	// ��ʼ�����Ż�
	for (int kk=0;kk<max_iter;++kk)
	{
		// �����ڵ�ǰ��̬�����µĲв���
		for (int k=0;k<m;++k)
		{
			int k3 = 3*k;

			int i = vijs[k].x;
			int j = vijs[k].y;

//			Matx33d dRij = vRijs[k]*vRis[i]*vRis[j].t();		// dRij = RijRiRj'		wij+wi-wj=0
			Matx33d dRij = vRis[j]*vRis[i].t()*vRijs[k].t();	// dRij = RjRi'Rij'		wj-wi-wij=0
//			Matx33d dRij = vRis[j].t()*vRijs[k]*vRis[i];		// dRij = Rj'RijRi		-wj+wij+wi=0

			Matx31d dwij = calib::converse_R_rotvec(dRij);

			f.at<double>(k3)   = dwij(0);
			f.at<double>(k3+1) = dwij(1);
			f.at<double>(k3+2) = dwij(2);
		}
		
		Mat h;

		// J(x0)dx = -f(x0)
		solve(J, -f, h, DECOMP_QR);

		fVal_new = norm(f)*norm(f);
		hVal     = norm(h);

		double df2 = fVal_new - fVal_old;

		rrr = sqrt(fVal_new/m);

		if ((fabs(df2) < fEps) || (hVal < xEps))
		{
			break;
		}

		++nnn;

		fVal_old = fVal_new;

		for (int i=0;i<n_all;++i)
		{
			if (i==idx_ref)
			{
				continue;
			}

			int i3 = 3*vJj[i];

			double drv1 = h.at<double>(i3);
			double drv2 = h.at<double>(i3+1);
			double drv3 = h.at<double>(i3+2);

			Matx33d dRi = calib::converse_rotvec_R(drv1, drv2, drv3);

			vRis[i] = dRi*vRis[i];
//			vRis[i] = vRis[i]*dRi;
		}
	}

	if (nIter)
	{
		*nIter = nnn;
	}

	if (residue)
	{
		*residue = rrr;
	}
}

// optimize Ri based on Rotation Averaging using Gauss-Newton method
// ��ϸ�ο� Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> �е� Algorithm A2
void SfM_ZZK::optim_gn_Ri(const vector<Matx33d> & vRijs,// ���룺		���й۲�������ת����
						  const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
						  vector<Matx33d> & vRis,		// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
						  int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
						  int * nIter /*= NULL*/,		// �����		ʵ�ʵ�������
						  double * residue /*= NULL*/,	// �����		���յĲв��С
						  int max_iter /*= 32*/,		// ���룺		����������
						  double xEps /*= 1.0E-12*/,	// ���룺		��ֹ����
						  double fEps /*= 1.0E-12*/		// ���룺		��ֹ����
						  )
{
	int n_all = vRis.size(); // ͼ���ܸ���
	int m = vRijs.size(); // ���۲�������ת�ĸ���

	int n_optim;
	if (idx_ref<0)
	{
		// �ο�ͼ������Ϊ-1��˵��û�вο�ͼ������ͼ�����̬���Ż�
		n_optim = n_all;
	} 
	else
	{
		// ��ʱ˵���вο�ͼ���Ż����˲ο�ͼ֮�������ͼ�����̬
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// �ȶ���ÿ�����Ż�ͼ����Jacobian�����г�ʼ��������
	vector<int> vJj;
	if (idx_ref<0)
	{
		for (int i=0;i<n_all;++i)
		{
			vJj.push_back(i);
		}
	} 
	else
	{
		for (int i=0;i<n_all;++i)
		{
			if (i>idx_ref)
			{
				vJj.push_back(i-1);
			} 
			else if (i==idx_ref)
			{
				vJj.push_back(-1);
			}
			else
			{
				vJj.push_back(i);
			}
		}
	}
	
	// ���ɹ̶���Jacobian����
	for (int k=0;k<m;++k)
	{
		int k3 = 3*k;

		int i = vijs[k].x;
		int j = vijs[k].y;

		int i3 = 3*vJj[i];
		int j3 = 3*vJj[j];

		if (i!=idx_ref)
		{
			J.at<double>(k3,   i3)   = -1;
			J.at<double>(k3+1, i3+1) = -1;
			J.at<double>(k3+2, i3+2) = -1;
		} 

		if (j!=idx_ref)
		{
			J.at<double>(k3,   j3)   = 1;
			J.at<double>(k3+1, j3+1) = 1;
			J.at<double>(k3+2, j3+2) = 1;
		}
	}
	
	double fVal_old = 0;
	double fVal_new;
	double hVal;

	int nnn = 0;
	double rrr;

	// ��ʼ�����Ż�
	for (int kk=0;kk<max_iter;++kk)
	{
		// �����ڵ�ǰ��̬�����µĲв���
		for (int k=0;k<m;++k)
		{
			int k3 = 3*k;

			int i = vijs[k].x;
			int j = vijs[k].y;

//			Matx33d dRij = vRijs[k]*vRis[i]*vRis[j].t();		// dRij = RijRiRj'		wij+wi-wj=0
			Matx33d dRij = vRis[j]*vRis[i].t()*vRijs[k].t();	// dRij = RjRi'Rij'		wj-wi-wij=0
//			Matx33d dRij = vRis[j].t()*vRijs[k]*vRis[i];		// dRij = Rj'RijRi		-wj+wij+wi=0

			Matx31d dwij = calib::converse_R_rotvec(dRij);

			f.at<double>(k3)   = dwij(0);
			f.at<double>(k3+1) = dwij(1);
			f.at<double>(k3+2) = dwij(2);
		}
		
		Mat h;

		// J(x0)dx = -f(x0)
		solve(J.t()*J, -J.t()*f, h, DECOMP_CHOLESKY);
		
		fVal_new = norm(f)*norm(f);
		hVal     = norm(h);

		double df2 = fVal_new - fVal_old;

		rrr = sqrt(fVal_new/m);

		if ((fabs(df2) < fEps) || (hVal < xEps))
		{
			break;
		}

		++nnn;

		fVal_old = fVal_new;

		for (int i=0;i<n_all;++i)
		{
			if (i==idx_ref)
			{
				continue;
			}

			int i3 = 3*vJj[i];

			double drv1 = h.at<double>(i3);
			double drv2 = h.at<double>(i3+1);
			double drv3 = h.at<double>(i3+2);

			Matx33d dRi = calib::converse_rotvec_R(drv1, drv2, drv3);

			vRis[i] = dRi*vRis[i];
//			vRis[i] = vRis[i]*dRi;
		}
	}

	if (nIter)
	{
		*nIter = nnn;
	}

	if (residue)
	{
		*residue = rrr;
	}
}

void SfM_ZZK::optim_lm_Ri(const vector<Matx33d> & vRijs,// ���룺		���й۲�������ת����
						  const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
						  vector<Matx33d> & vRis,		// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
						  int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
						  double * info /*= NULL*/,		// output:	runtime info, 5-vector
														// info[0]:	the initial reprojection error
														// info[1]:	the final reprojection error
														// info[2]: final max gradient
														// info[3]: the number of iterations
														// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
						  double tau /*= 1.0E-3*/,		// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
						  int maxIter /*= 64*/,			// input:	the maximum number of iterations
						  double eps1 /*= 1.0E-8*/,		// input:	threshold
						  double eps2 /*= 1.0E-12*/		// input:	threshold
						  )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;	// �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int n_all = vRis.size(); // ͼ���ܸ���
	int m = vRijs.size(); // ���۲�������ת�ĸ���

	int n_optim;
	if (idx_ref<0)
	{
		// �ο�ͼ������Ϊ-1��˵��û�вο�ͼ������ͼ�����̬���Ż�
		n_optim = n_all;
	} 
	else
	{
		// ��ʱ˵���вο�ͼ���Ż����˲ο�ͼ֮�������ͼ�����̬
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// �ȶ���ÿ�����Ż�ͼ����Jacobian�����г�ʼ��������
	vector<int> vJj;
	if (idx_ref<0)
	{
		for (int i=0;i<n_all;++i)
		{
			vJj.push_back(i);
		}
	} 
	else
	{
		for (int i=0;i<n_all;++i)
		{
			if (i>idx_ref)
			{
				vJj.push_back(i-1);
			} 
			else if (i==idx_ref)
			{
				vJj.push_back(-1);
			}
			else
			{
				vJj.push_back(i);
			}
		}
	}

	// ���ɹ̶���Jacobian����
	for (int kk=0;kk<m;++kk)
	{
		int k3 = 3*kk;

		int i = vijs[kk].x;
		int j = vijs[kk].y;

		int i3 = 3*vJj[i];
		int j3 = 3*vJj[j];

		if (i!=idx_ref)
		{
			J.at<double>(k3,   i3)   = -1;
			J.at<double>(k3+1, i3+1) = -1;
			J.at<double>(k3+2, i3+2) = -1;
		} 

		if (j!=idx_ref)
		{
			J.at<double>(k3,   j3)   = 1;
			J.at<double>(k3+1, j3+1) = 1;
			J.at<double>(k3+2, j3+2) = 1;
		}
	}

	Mat A = J.t()*J; // A = J(x)'J(x) ���� F(x)"
	Mat g,h; // g = J(x)'f(x) Ϊ F(x)'; h Ϊ������

	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	// ���ɵ�һ�� g = J(x)'f(x) ///////////////////////////////////////////////////////
	for (int kk=0;kk<m;++kk)
	{
		int k3 = 3*kk;

		int i = vijs[kk].x;
		int j = vijs[kk].y;

//		Matx33d dRij = vRijs[kk]*vRis[i]*vRis[j].t();		// dRij = RijRiRj'		wij+wi-wj=0
		Matx33d dRij = vRis[j]*vRis[i].t()*vRijs[kk].t();	// dRij = RjRi'Rij'		wj-wi-wij=0
//		Matx33d dRij = vRis[j].t()*vRijs[kk]*vRis[i];		// dRij = Rj'RijRi		-wj+wij+wi=0

		Matx31d dwij = calib::converse_R_rotvec(dRij);

		f.at<double>(k3)   = dwij(0);
		f.at<double>(k3+1) = dwij(1);
		f.at<double>(k3+2) = dwij(2);
	}
	g = J.t()*f;	// g=J(x)'f(x)

	Mat tmp = 0.5*f.t()*f;
	double Fx_old = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_init = sqrt(2*Fx_old/m);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int i=0;i<A.rows;++i)
	{
		Aii.push_back(A.at<double>(i,i));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial tau

	while (!found && k<maxIter)
	{
		++k;

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // ��������ϵ��u֮�󣬾���A�϶����ǶԳ������ģ�Ҳ����˵�϶�����ʹ��Cholesky�ֽ�

		h_norm = norm(h);
		
		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> vRis_new = vRis;

			for (int i=0;i<n_all;++i)
			{
				if (i==idx_ref)
				{
					continue;
				}

				int i3 = 3*vJj[i];

				double drv1 = h.at<double>(i3);
				double drv2 = h.at<double>(i3+1);
				double drv3 = h.at<double>(i3+2);

				Matx33d dRi = calib::converse_rotvec_R(drv1, drv2, drv3);

				vRis_new[i] = dRi*vRis_new[i];
			}

			for (int kk=0;kk<m;++kk)
			{
				int k3 = 3*kk;

				int i = vijs[kk].x;
				int j = vijs[kk].y;

//				Matx33d dRij = vRijs[kk]*vRis_new[i]*vRis_new[j].t();		// dRij = RijRiRj'		wij+wi-wj=0
				Matx33d dRij = vRis_new[j]*vRis_new[i].t()*vRijs[kk].t();	// dRij = RjRi'Rij'		wj-wi-wij=0
//				Matx33d dRij = vRis_new[j].t()*vRijs[kk]*vRis_new[i];		// dRij = Rj'RijRi		-wj+wij+wi=0

				Matx31d dwij = calib::converse_R_rotvec(dRij);

				f.at<double>(k3)   = dwij(0);
				f.at<double>(k3+1) = dwij(1);
				f.at<double>(k3+2) = dwij(2);
			}

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0);

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0);

			r = (Fx_old - Fx_new) / L0_Lh;

			if (r>0)
			{
				vRis = vRis_new;

				g = J.t()*f;	// g=J(x)'f(x)

				Fx_old = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_final = sqrt(2*Fx_old/m);

	if (info)
	{
		info[0] = err_init;
		info[1] = err_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,				// ����������n���ռ������
											 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
											 vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
											 vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
											 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
											 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
											 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
											 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											 vector<double> & vds,					// �����ÿ��������ͶӰ�в�
											 double * info /*= NULL*/,				// output:	runtime info, 5-vector
																					// info[0]:	the initial reprojection error
																					// info[1]:	the final reprojection error
																					// info[2]: final max gradient
																					// info[3]: the number of iterations
																					// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
											 double tau /*= 1.0E-3*/,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
											 int maxIter /*= 64*/,					// input:	the maximum number of iterations
											 double eps1 /*= 1.0E-8*/,				// input:	threshold
											 double eps2 /*= 1.0E-12*/				// input:	threshold
											 )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+4*n,1,CV_64FC1,Scalar(0)),g_new(6*m+4*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+4*n,1,CV_64FC1,Scalar(0));

	// ����������ʹ�õ����������꣬����ɷ�������������չ������������
	vector<Point4d> XYZWs;
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];
		Point4d XYZW;
		XYZW.x = XYZ.x;
		XYZW.y = XYZ.y;
		XYZW.z = XYZ.z;
		XYZW.w = 1;

		XYZWs.push_back(XYZW);
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
//	vector<double> vds(l);

	derivatives::j_f_w_t_XYZW(XYZWs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<4;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_4(u,ptrMat,U,V,W,ea,eb,da,db,h); 

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<Point4d> XYZWs_new = XYZWs;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,4,1> dbi = db[ii];
				XYZWs_new[ii].x += dbi(0);
				XYZWs_new[ii].y += dbi(1);
				XYZWs_new[ii].z += dbi(2);
				XYZWs_new[ii].w += dbi(3);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_t_XYZW(XYZWs_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				ts=ts_new;
				XYZWs=XYZWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż�������������껹ԭΪ���������������
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];
		double w_1 = 1.0/XYZW.w;

		Point3d XYZ;
		XYZ.x = XYZW.x*w_1;
		XYZ.y = XYZW.y*w_1;
		XYZ.z = XYZW.z*w_1;

		XYZs[i] = XYZ;
	}

	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// 20151107��iteratively reweighted least squares
// �����ؼ�Ȩ�汾������ Huber Ȩ��
void SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(vector<Point3d> & XYZs,					// ����������n���ռ������
													    const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
													    vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
													    vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
													    const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
													    const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
													    const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
														vector<Matx22d> & covInvs,				// �����l�������������Э�������������(i)=wi*wi
													    const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
													    const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
													    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
														vector<double> & vds,					// �����ÿ��������ͶӰ�в�
													    double tc /*= 3.0*/,					// ���룺���� Huber Ȩ��ʱ�õ��ĳ���
													    double * info /*= NULL*/,				// output:	runtime info, 5-vector
																								// info[0]:	the initial reprojection error
																								// info[1]:	the final reprojection error
																								// info[2]: final max gradient
																								// info[3]: the number of iterations
																								// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
													    double tau /*= 1.0E-3*/,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
													    int maxIter /*= 64*/,					// input:	the maximum number of iterations
													    double eps1 /*= 1.0E-8*/,				// input:	threshold
													    double eps2 /*= 1.0E-12*/				// input:	threshold
													    )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+4*n,1,CV_64FC1,Scalar(0)),g_new(6*m+4*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+4*n,1,CV_64FC1,Scalar(0));

	// ����������ʹ�õ����������꣬����ɷ�������������չ������������
	vector<Point4d> XYZWs;
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];
		Point4d XYZW;
		XYZW.x = XYZ.x;
		XYZW.y = XYZ.y;
		XYZW.z = XYZ.z;
		XYZW.w = 1;

		XYZWs.push_back(XYZW);
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
//	vector<double> vds(l);

	derivatives::j_f_w_t_XYZW_IRLS_Huber(XYZWs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds,tc);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	// 20151111����ͳ���ڵ����ͶӰ�в�
	int n_inliers = 0;
	double sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		/*double dx = f.at<double>(2*i);
		double dy = f.at<double>(2*i+1);
		double d = sqrt(dx*dx+dy*dy);*/

		// 20170831�����ܴ� f ��ȡֵ����Ϊ f ���Ѿ�����Ȩ���ˣ��ж��ڵ㼯��Ӧ�ÿ�ԭʼ��ƫ��ֵ
		double d = vds[i];

		if (d >= tc)
		{
			continue;
		}
		++n_inliers;
		sum_d2 += d*d;
	}
	double err_rpj_init = sqrt(sum_d2 / n_inliers);

//	double err_rpj_init = sqrt(2*Fx/l);
	
	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<4;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_4(u,ptrMat,U,V,W,ea,eb,da,db,h); 

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<Point4d> XYZWs_new = XYZWs;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,4,1> dbi = db[ii];
				XYZWs_new[ii].x += dbi(0);
				XYZWs_new[ii].y += dbi(1);
				XYZWs_new[ii].z += dbi(2);
				XYZWs_new[ii].w += dbi(3);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_t_XYZW_IRLS_Huber(XYZWs_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds,tc);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				ts=ts_new;
				XYZWs=XYZWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż�������������껹ԭΪ���������������
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];
		double w_1 = 1.0/XYZW.w;

		Point3d XYZ;
		XYZ.x = XYZW.x*w_1;
		XYZ.y = XYZW.y*w_1;
		XYZ.z = XYZW.z*w_1;

		XYZs[i] = XYZ;
	}

	// 20151111����ͳ���ڵ����ͶӰ�в�
	n_inliers = 0;
	sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		/*double dx = f.at<double>(2*i);
		double dy = f.at<double>(2*i+1);
		double d = sqrt(dx*dx+dy*dy);*/

		// 20170831�����ܴ� f ��ȡֵ����Ϊ f ���Ѿ�����Ȩ���ˣ��ж��ڵ㼯��Ӧ�ÿ�ԭʼ��ƫ��ֵ
		double d = vds[i];

		if (d >= tc)
		{
			continue;
		}
		++n_inliers;
		sum_d2 += d*d;
	}
	double err_rpj_final = sqrt(sum_d2 / n_inliers);

//	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// 20170820��˫Ŀ�Ӿ��У��̶���ͼ��ͶӰ����Ϊ[I|0]��ϡ���Ż���ͼ��ͶӰ���� P
// �Լ���Ӱ�ؽ������� n ��������� [X Y 1 W]������ Z ����̶�Ϊ 1
// �ú�����ҪĿ���Ǵ��Ż���� P �зֽ�����м�����������Ļ������� F ��
void SBA_ZZK::optim_sparse_lm_P_XiYiWi(Matx34d & P,									// ������������ǰ���Ƶ���ͼ����Ӱ�ռ��е�ͶӰ����
									   vector<Point3d> & XYWs,						// ����������n������XYW���꣬Z����Ĭ��Ϊ1
									   const vector<Point2d> & xysL,				// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������
									   const vector<Point2d> & xysR,				// ���룺n ���ռ������ͼ�й۲��������
									   const vector<Matx22d> & covInvsL,			// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������Э�������������
									   const vector<Matx22d> & covInvsR,			// ���룺n ���ռ������ͼ�й۲��������Э�������������
									   vector<Point2d> & vds,						// �����n ���ռ������������ͼ�е���ͶӰ�в���
									   double * info /*= NULL*/,					// output:	runtime info, 5-vector
																					// info[0]:	the initial reprojection error
																					// info[1]:	the final reprojection error
																					// info[2]: final max gradient
																					// info[3]: the number of iterations
																					// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									   double tau /*= 1.0E-3*/,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									   int maxIter /*= 64*/,						// input:	the maximum number of iterations
									   double eps1 /*= 1.0E-8*/,					// input:	threshold
									   double eps2 /*= 1.0E-12*/					// input:	threshold
									   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ
	double h_thresh;// �����������ж���ֵ eps2*(norm(x)+eps2)
	double F, F_new;// Ŀ�꺯��ֵ 0.5*ft*covInv*f
	double x_norm, x_norm_new; // ��ǰ���Ż�����������ģ����2����L2��|x|2
	double L0_Lh;	// ̩��չ��ʽ�ĺ���ֵ�½���

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int n = XYWs.size(); // ������

	// Mat �ṹ
	Mat g(12 + 3 * n, 1, CV_64FC1, Scalar(0)), g_new(12 + 3 * n, 1, CV_64FC1, Scalar(0));
	Mat h(12 + 3 * n, 1, CV_64FC1, Scalar(0));
	Mat tmp;

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	Matx<double, 12, 12> U, U_new;
	vector<Matx<double, 3, 3>> V(n), V_new(n);
	vector<Matx<double, 12, 3>> W(n), W_new(n);
	Matx<double, 12, 1> ea, ea_new;
	vector<Matx<double, 3, 1>> eb(n), eb_new(n);
	Matx<double, 12, 1> da;
	vector<Matx<double, 3, 1>> db(n);

	derivatives::j_f_P_XYW(P, XYWs, xysL, xysR, covInvsL, covInvsR, U, V, W, ea, eb, F, x_norm, g, vds);
	
	// ��ʼ��ͶӰ�в�
	double err_rpj_init = sqrt(F / n);
	
	g_norm = norm(g, NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	
	for (int i = 0; i < 12; ++i)
	{
		Aii.push_back(U(i, i));
	}
	
	for (int i = 0; i < n; ++i)
	{
		for (int ii = 0; ii < 3; ++ii)
		{
			Aii.push_back(V[i](ii, ii));
		}
	}

	auto iter = max_element(Aii.begin(), Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k < maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solve_sparseLM_F(u, U, V, W, ea, eb, da, db, h);

		h_norm = norm(h);

		h_thresh = eps2*(x_norm + eps2); // ���ݵ�ǰ���Ż�����������ģ��ȷ����������С�Ƿ�������������

//		if (h_norm < eps2) // ����������
		if (h_norm < h_thresh) // 20170901��������Ա����ķ�ʽ�жϸ���������
		{
			found = true;
			code = 1;
		} 
		else
		{
			Matx34d P_new = P;
			vector<Point3d> XYWs_new = XYWs;

			for (int i = 0; i < 3; ++i)
			{
				int i4 = i * 4;

				for (int j = 0; j < 4; ++j)
				{
					P_new(i, j) += da(i4 + j);
				}
			}

			for (int i = 0; i < n; ++i)
			{
				Matx31d dbi = db[i];
				
				XYWs_new[i].x += dbi(0);
				XYWs_new[i].y += dbi(1);
				XYWs_new[i].z += dbi(2);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_P_XYW(P_new, XYWs_new, xysL, xysR, covInvsL, covInvsR, U_new, V_new, W_new, ea_new, eb_new, F_new, x_norm_new, g_new, vds);

			tmp = 0.5*h.t()*(u*h - g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (F - F_new) / L0_Lh;

			if (r > 0)
			{
				// �����²���
				P = P_new;
				XYWs = XYWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				F = F_new;

				// �����Ѿ�����������²���������ģ
				x_norm = x_norm_new;

				g_norm = norm(g, NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(F / n);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(vector<Point3d> & XYZs,					// ����������n���ռ������
										   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
										   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
										   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
										   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
										   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
										   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
										   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
										   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
										   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
										   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
										   double * info /*= NULL*/,				// output:	runtime info, 5-vector
																					// info[0]:	the initial reprojection error
																					// info[1]:	the final reprojection error
																					// info[2]: final max gradient
																					// info[3]: the number of iterations
																					// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
										   double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
										   int maxIter /*= 64*/,					// input:	the maximum number of iterations
										   double eps1 /*= 1.0E-8*/,				// input:	threshold
										   double eps2 /*= 1.0E-12*/				// input:	threshold
										   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+3*n,1,CV_64FC1,Scalar(0)),g_new(6*m+3*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+3*n,1,CV_64FC1,Scalar(0));

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,3,3>> V(n),V_new(n);
	vector<Matx<double,6,3>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,3,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,3,1>> db(n);
	vector<double> vds(l);

	derivatives::j_f_w_t_XYZ(XYZs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<3;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_3(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<Point3d> XYZs_new = XYZs;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,3,1> dbi = db[ii];
				XYZs_new[ii].x += dbi(0);
				XYZs_new[ii].y += dbi(1);
				XYZs_new[ii].z += dbi(2);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_t_XYZ(XYZs_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				ts=ts_new;
				XYZs=XYZs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void SBA_ZZK::optim_sparse_lm_wj_cj_XiYiZiWi(vector<Point3d> & XYZs,				// ����������n���ռ������
											 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
											 vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
											 vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
											 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
											 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
											 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
											 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											 double * info /*= NULL*/,				// output:	runtime info, 5-vector
																					// info[0]:	the initial reprojection error
																					// info[1]:	the final reprojection error
																					// info[2]: final max gradient
																					// info[3]: the number of iterations
																					// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
											 double tau /*= 1.0E-3*/,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
											 int maxIter /*= 64*/,					// input:	the maximum number of iterations
											 double eps1 /*= 1.0E-8*/,				// input:	threshold
											 double eps2 /*= 1.0E-12*/				// input:	threshold
											 )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+4*n,1,CV_64FC1,Scalar(0)),g_new(6*m+4*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+4*n,1,CV_64FC1,Scalar(0));

	// ����������ʹ�õ����������꣬����ɷ�������������չ������������
	vector<Point4d> XYZWs;
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];
		Point4d XYZW;
		XYZW.x = XYZ.x;
		XYZW.y = XYZ.y;
		XYZW.z = XYZ.z;
		XYZW.w = 1;

		XYZWs.push_back(XYZW);
	}

	// ���ڵ���������ʹ�õ��ǹ������꣬���������ɹ�������
	vector<Matx31d> cs(m);
	for (int j=0;j<m;++j)
	{
		cs[j] = -Rs[j].t()*ts[j];
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
	vector<double> vds(l);

	derivatives::j_f_w_c_XYZW(XYZWs,Ks,Rs,cs,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<4;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_4(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> cs_new = cs;
			vector<Point4d> XYZWs_new = XYZWs;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				cs_new[jj](0)+=daj(3);
				cs_new[jj](1)+=daj(4);
				cs_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,4,1> dbi = db[ii];
				XYZWs_new[ii].x += dbi(0);
				XYZWs_new[ii].y += dbi(1);
				XYZWs_new[ii].z += dbi(2);
				XYZWs_new[ii].w += dbi(3);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_c_XYZW(XYZWs_new,Ks,Rs_new,cs_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				cs=cs_new;
				XYZWs=XYZWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż�������������껹ԭΪ���������������
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];
		double w_1 = 1.0/XYZW.w;

		Point3d XYZ;
		XYZ.x = XYZW.x*w_1;
		XYZ.y = XYZW.y*w_1;
		XYZ.z = XYZW.z*w_1;

		XYZs[i] = XYZ;
	}

	// ���Ż��Ĺ�������ת��Ϊƽ���������
	for (int j=0;j<m;++j)
	{
		ts[j] = -Rs[j]*cs[j];
	}

	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void SBA_ZZK::optim_sparse_lm_wj_cj_XiYiZi(vector<Point3d> & XYZs,					// ����������n���ռ������
										   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
										   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
										   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
										   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
										   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
										   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
										   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
										   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
										   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
										   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
										   double * info /*= NULL*/,				// output:	runtime info, 5-vector
																					// info[0]:	the initial reprojection error
																					// info[1]:	the final reprojection error
																					// info[2]: final max gradient
																					// info[3]: the number of iterations
																					// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
										   double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
										   int maxIter /*= 64*/,					// input:	the maximum number of iterations
										   double eps1 /*= 1.0E-8*/,				// input:	threshold
										   double eps2 /*= 1.0E-12*/				// input:	threshold
										   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+3*n,1,CV_64FC1,Scalar(0)),g_new(6*m+3*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+3*n,1,CV_64FC1,Scalar(0));

	// ���ڵ���������ʹ�õ��ǹ������꣬���������ɹ�������
	vector<Matx31d> cs(m);
	for (int j=0;j<m;++j)
	{
		cs[j] = -Rs[j].t()*ts[j];
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,3,3>> V(n),V_new(n);
	vector<Matx<double,6,3>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,3,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,3,1>> db(n);
	vector<double> vds(l);

	derivatives::j_f_w_c_XYZ(XYZs,Ks,Rs,cs,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<3;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_3(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> cs_new = cs;
			vector<Point3d> XYZs_new = XYZs;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				cs_new[jj](0)+=daj(3);
				cs_new[jj](1)+=daj(4);
				cs_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,3,1> dbi = db[ii];
				XYZs_new[ii].x += dbi(0);
				XYZs_new[ii].y += dbi(1);
				XYZs_new[ii].z += dbi(2);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_c_XYZ(XYZs_new,Ks,Rs_new,cs_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				cs=cs_new;
				XYZs=XYZs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż��Ĺ�������ת��Ϊƽ���������
	for (int j=0;j<m;++j)
	{
		ts[j] = -Rs[j]*cs[j];
	}

	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,					// ����������n���ռ������
											   vector<Matx33d> & Ks,					// ����������m��ͼ���ڲ�������
											   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
											   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
											   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
											   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
											   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
											   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											   double * info /*= NULL*/,				// output:	runtime info, 5-vector
																						// info[0]:	the initial reprojection error
																						// info[1]:	the final reprojection error
																						// info[2]: final max gradient
																						// info[3]: the number of iterations
																						// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
											   double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
											   int maxIter /*= 64*/,					// input:	the maximum number of iterations
											   double eps1 /*= 1.0E-8*/,				// input:	threshold
											   double eps2 /*= 1.0E-12*/				// input:	threshold
											   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������
	int M = 1+6*m+4*n; // ���Ż���������
	int N = 2*l; // �ܹ۲ⷽ�̸���

	// Mat �ṹ
	Mat f(N,1,CV_64FC1,Scalar(0));
	Mat g(M,1,CV_64FC1,Scalar(0)),g_new(M,1,CV_64FC1,Scalar(0));
	Mat h(M,1,CV_64FC1,Scalar(0));

	// ����������ʹ�õ����������꣬����ɷ�������������չ������������
	vector<Point4d> XYZWs;
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];
		Point4d XYZW;
		XYZW.x = XYZ.x;
		XYZW.y = XYZ.y;
		XYZW.z = XYZ.z;
		XYZW.w = 1;

		XYZWs.push_back(XYZW);
	}

	// ������ͼ��ĳ�ʼ��Ч������Ϊ���ǵľ�ֵ
	double sum_f = 0;
	for (int j=0;j<m;++j)
	{
		Matx33d K = Ks[j];

		sum_f += K(0,0);
		sum_f += K(1,1);
	}
	double mean_f = 0.5*sum_f/m;
	for (int j=0;j<m;++j)
	{
		Ks[j](0,0) = Ks[j](1,1) = mean_f;
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	Matx<double,1,1> Q, Q_new;
	vector<Matx<double,1,6>> G(m),G_new(m);
	vector<Matx<double,1,4>> H(n),H_new(n);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	Matx<double,1,1> ec, ec_new;
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
	Matx<double,1,1> dc;
	vector<double> vds(l);

	derivatives::j_f_f_w_t_XYZW(XYZWs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,Q,G,H,ea,eb,ec,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	Aii.push_back(Q(0));
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<4;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_1_6_4(u,ptrMat,U,V,W,Q,G,H,ea,eb,ec,da,db,dc,h); 

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Ks_new = Ks;
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<Point4d> XYZWs_new = XYZWs;

			for (int jj=0;jj<m;++jj)
			{
				Ks_new[jj](0,0) += dc(0);
				Ks_new[jj](1,1) += dc(0);

				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,4,1> dbi = db[ii];
				XYZWs_new[ii].x += dbi(0);
				XYZWs_new[ii].y += dbi(1);
				XYZWs_new[ii].z += dbi(2);
				XYZWs_new[ii].w += dbi(3);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_f_w_t_XYZW(XYZWs_new,Ks_new,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,Q_new,G_new,H_new,ea_new,eb_new,ec_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Ks = Ks_new;
				Rs = Rs_new;
				ts = ts_new;
				XYZWs = XYZWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				Q = Q_new;
				G = G_new;
				H = H_new;
				ea = ea_new;
				eb = eb_new;
				ec = ec_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż�������������껹ԭΪ���������������
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];
		double w_1 = 1.0/XYZW.w;

		Point3d XYZ;
		XYZ.x = XYZW.x*w_1;
		XYZ.y = XYZW.y*w_1;
		XYZ.z = XYZW.z*w_1;

		XYZs[i] = XYZ;
	}

	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// 20200607��iteratively reweighted least squares
// �����ؼ�Ȩ�汾������ Huber Ȩ��
void SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_IRLS_Huber(vector<Point3d> & XYZs,					// ����������n���ռ������
														  vector<Matx33d> & Ks,						// ����������m��ͼ���ڲ�������
														  vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
														  vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
														  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
														  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
														  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
														  vector<Matx22d> & covInvs,				// �����l�������������Э�������������(i)=wi*wi
														  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
														  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
														  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
														  vector<double> & vds,						// �����ÿ��������ͶӰ�в�
														  double tc /*= 3.0*/,						// ���룺���� Huber Ȩ��ʱ�õ��ĳ���
														  double * info /*= NULL*/,					// output:	runtime info, 5-vector
																									// info[0]:	the initial reprojection error
																									// info[1]:	the final reprojection error
																									// info[2]: final max gradient
																									// info[3]: the number of iterations
																									// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
														  double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
														  int maxIter /*= 64*/,						// input:	the maximum number of iterations
														  double eps1 /*= 1.0E-8*/,					// input:	threshold
														  double eps2 /*= 1.0E-12*/					// input:	threshold
														  )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������
	int M = 1+6*m+4*n; // ���Ż���������
	int N = 2*l; // �ܹ۲ⷽ�̸���

	// Mat �ṹ
	Mat f(N,1,CV_64FC1,Scalar(0));
	Mat g(M,1,CV_64FC1,Scalar(0)),g_new(M,1,CV_64FC1,Scalar(0));
	Mat h(M,1,CV_64FC1,Scalar(0));

	// ����������ʹ�õ����������꣬����ɷ�������������չ������������
	vector<Point4d> XYZWs;
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];
		Point4d XYZW;
		XYZW.x = XYZ.x;
		XYZW.y = XYZ.y;
		XYZW.z = XYZ.z;
		XYZW.w = 1;

		XYZWs.push_back(XYZW);
	}

	// ������ͼ��ĳ�ʼ��Ч������Ϊ���ǵľ�ֵ
	double sum_f = 0;
	for (int j=0;j<m;++j)
	{
		Matx33d K = Ks[j];

		sum_f += K(0,0);
		sum_f += K(1,1);
	}
	double mean_f = 0.5*sum_f/m;
	for (int j=0;j<m;++j)
	{
		Ks[j](0,0) = Ks[j](1,1) = mean_f;
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	Matx<double,1,1> Q, Q_new;
	vector<Matx<double,1,6>> G(m),G_new(m);
	vector<Matx<double,1,4>> H(n),H_new(n);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	Matx<double,1,1> ec, ec_new;
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
	Matx<double,1,1> dc;
//	vector<double> vds(l);

	derivatives::j_f_f_w_t_XYZW_IRLS_Huber(XYZWs, Ks, Rs, ts, dists, distTypes, xys, covInvs, j_fixed, i_fixed, ptrMat, U, V, W, Q, G, H, ea, eb, ec, f, g, vds, tc);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	// 20200607����ͳ���ڵ����ͶӰ�в�
	int n_inliers = 0;
	double sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		// 20170831�����ܴ� f ��ȡֵ����Ϊ f ���Ѿ�����Ȩ���ˣ��ж��ڵ㼯��Ӧ�ÿ�ԭʼ��ƫ��ֵ
		double d = vds[i];

		if (d >= tc)
		{
			continue;
		}
		++n_inliers;
		sum_d2 += d*d;
	}
	double err_rpj_init = sqrt(sum_d2 / n_inliers);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	Aii.push_back(Q(0));
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<4;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_1_6_4(u,ptrMat,U,V,W,Q,G,H,ea,eb,ec,da,db,dc,h); 

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Ks_new = Ks;
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<Point4d> XYZWs_new = XYZWs;

			for (int jj=0;jj<m;++jj)
			{
				Ks_new[jj](0,0) += dc(0);
				Ks_new[jj](1,1) += dc(0);

				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,4,1> dbi = db[ii];
				XYZWs_new[ii].x += dbi(0);
				XYZWs_new[ii].y += dbi(1);
				XYZWs_new[ii].z += dbi(2);
				XYZWs_new[ii].w += dbi(3);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_f_w_t_XYZW_IRLS_Huber(XYZWs_new, Ks_new, Rs_new, ts_new, dists, distTypes, xys, covInvs, j_fixed, i_fixed,
				ptrMat, U_new, V_new, W_new, Q_new, G_new, H_new, ea_new, eb_new, ec_new, f, g_new, vds, tc);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Ks = Ks_new;
				Rs = Rs_new;
				ts = ts_new;
				XYZWs = XYZWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				Q = Q_new;
				G = G_new;
				H = H_new;
				ea = ea_new;
				eb = eb_new;
				ec = ec_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż�������������껹ԭΪ���������������
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];
		double w_1 = 1.0/XYZW.w;

		Point3d XYZ;
		XYZ.x = XYZW.x*w_1;
		XYZ.y = XYZW.y*w_1;
		XYZ.z = XYZW.z*w_1;

		XYZs[i] = XYZ;
	}

	// 20200607����ͳ���ڵ����ͶӰ�в�
	n_inliers = 0;
	sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		// 20170831�����ܴ� f ��ȡֵ����Ϊ f ���Ѿ�����Ȩ���ˣ��ж��ڵ㼯��Ӧ�ÿ�ԭʼ��ƫ��ֵ
		double d = vds[i];

		if (d >= tc)
		{
			continue;
		}
		++n_inliers;
		sum_d2 += d*d;
	}
	double err_rpj_final = sqrt(sum_d2 / n_inliers);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

void SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_c_fixedBaseline(vector<Point3d> & XYZs,					// ����������n���ռ������
															   vector<Matx33d> & Ks,					// ����������m��ͼ���ڲ�������
															   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
															   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
															   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
															   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
															   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
															   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
															   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
															   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
															   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
															   int k_fixedBaseLine,						// ���룺������ͼ��֮��Ļ��߹̶�������k �ǲ�������һ��
															   int j_fixedBaseLine,						// ���룺������ͼ��֮��Ļ��߹̶�������j �ǵ�����һ��
															   double d_fixedBaseLine,					// ���룺����ͼ��֮��Ĺ̶����߳���
															   double * info /*= NULL*/,				// output:	runtime info, 5-vector
																										// info[0]:	the initial reprojection error
																										// info[1]:	the final reprojection error
																										// info[2]: final max gradient
																										// info[3]: the number of iterations
																										// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
															   double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
															   int maxIter /*= 64*/,					// input:	the maximum number of iterations
															   double eps1 /*= 1.0E-8*/,				// input:	threshold
															   double eps2 /*= 1.0E-12*/				// input:	threshold
															   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size(); // ͼ�����
	int n = XYZs.size(); // ������
	int l = xys.size(); // ����������
	int M = 1+6*m+4*n; // ���Ż���������
	int N = 2*l; // �ܹ۲ⷽ�̸���

	// Mat �ṹ
	Mat f(N,1,CV_64FC1,Scalar(0));
	Mat g(M,1,CV_64FC1,Scalar(0)),g_new(M,1,CV_64FC1,Scalar(0));
	Mat h(M,1,CV_64FC1,Scalar(0));

	// ����������ʹ�õ����������꣬����ɷ�������������չ������������
	vector<Point4d> XYZWs;
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];
		Point4d XYZW;
		XYZW.x = XYZ.x;
		XYZW.y = XYZ.y;
		XYZW.z = XYZ.z;
		XYZW.w = 1;

		XYZWs.push_back(XYZW);
	}

	// ������ͼ��ĳ�ʼ��Ч������Ϊ���ǵľ�ֵ
	double sum_f = 0;
	for (int j=0;j<m;++j)
	{
		Matx33d K = Ks[j];

		sum_f += K(0,0);
		sum_f += K(1,1);
	}
	double mean_f = 0.5*sum_f/m;
	for (int j=0;j<m;++j)
	{
		Ks[j](0,0) = Ks[j](1,1) = mean_f;
	}

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	Matx<double,1,1> Q, Q_new;
	vector<Matx<double,1,6>> G(m),G_new(m);
	vector<Matx<double,1,4>> H(n),H_new(n);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	Matx<double,1,1> ec, ec_new;
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
	Matx<double,1,1> dc;
	vector<double> vds(l);
	vector<Matx<double,1,6>> Dajs(1),Dajs_new(1);
	vector<double> hVals(1),hVals_new(1);

	derivatives::j_f_f_w_t_XYZW_c_fixedBaseline(XYZWs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,
		k_fixedBaseLine,j_fixedBaseLine,d_fixedBaseLine,U,V,W,Q,G,H,ea,eb,ec,f,g,vds,Dajs[0],hVals[0]);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh, constVal;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	Aii.push_back(Q(0));
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		for (int ii=0;ii<4;++ii)
		{
			Aii.push_back(V[i](ii,ii));
		}
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		
		derivatives::solveSBA_1_6_4_Daj(u,ptrMat,U,V,W,Q,G,H,ea,eb,ec,j_fixedBaseLine,Dajs,hVals,da,db,dc,h,constVal);

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Ks_new = Ks;
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<Point4d> XYZWs_new = XYZWs;

			for (int jj=0;jj<m;++jj)
			{
				Ks_new[jj](0,0) += dc(0);
				Ks_new[jj](1,1) += dc(0);

				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				Matx<double,4,1> dbi = db[ii];
				XYZWs_new[ii].x += dbi(0);
				XYZWs_new[ii].y += dbi(1);
				XYZWs_new[ii].z += dbi(2);
				XYZWs_new[ii].w += dbi(3);
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_f_w_t_XYZW_c_fixedBaseline(XYZWs_new,Ks_new,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,
				k_fixedBaseLine,j_fixedBaseLine,d_fixedBaseLine,U_new,V_new,W_new,Q_new,G_new,H_new,ea_new,eb_new,ec_new,f,g_new,vds,Dajs_new[0],hVals_new[0]);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0)+constVal; // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Ks = Ks_new;
				Rs = Rs_new;
				ts = ts_new;
				XYZWs = XYZWs_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				Q = Q_new;
				G = G_new;
				H = H_new;
				ea = ea_new;
				eb = eb_new;
				ec = ec_new;
				g = g_new.clone();
				Dajs = Dajs_new;
				hVals = hVals_new;

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż�������������껹ԭΪ���������������
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];
		double w_1 = 1.0/XYZW.w;

		Point3d XYZ;
		XYZ.x = XYZW.x*w_1;
		XYZ.y = XYZW.y*w_1;
		XYZ.z = XYZW.z*w_1;

		XYZs[i] = XYZ;
	}

	double err_rpj_final = sqrt(2*Fx/l);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// 20150722��ÿ��������������ο�ͼ�����ֵ������
void SBA_ZZK::optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// ����������n���ռ������
									   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
									   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
									   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
									   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
									   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
									   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
									   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
									   const vector<int> & ri_j,				// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
									   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
									   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
									   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Wij�ڸ��������д洢��λ������
									   double * info /*= NULL*/,				// output:	runtime info, 5-vector
																				// info[0]:	the initial reprojection error
																				// info[1]:	the final reprojection error
																				// info[2]: final max gradient
																				// info[3]: the number of iterations
																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									   double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									   int maxIter /*= 64*/,					// input:	the maximum number of iterations
									   double eps1 /*= 1.0E-8*/,				// input:	threshold
									   double eps2 /*= 1.0E-12*/				// input:	threshold
									   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size();	// ͼ�����
	int n = XYZs.size();// ������
	int l = xys.size(); // ����������
	int l_noref = l-n;	// �ǲο����ĸ���

	// ��������ȳ�ֵ���Լ������Ĳο����򣨼��ο�ͼ�еĹ�һ��������꣩
	vector<double> ds(n);
	vector<Matx31d> nxys(n);
	for (int i=0;i<n;++i)
	{
		int k = ri_j[i];

		Point3d XYZ = XYZs[i];

		Matx31d X;
		X(0) = XYZ.x;
		X(1) = XYZ.y;
		X(2) = XYZ.z;

		Matx33d K0 = Ks[k];
		Matx33d R0 = Rs[k];
		Matx31d t0 = ts[k];
		Matx<double,5,1> dist = dists[k];
		int distType = distTypes[k];

		X=R0*X+t0;

		ds[i] = X(2);

		const int * ptr = ptrMat.find<int>(i,k);

		Point2d xy = xys[*ptr];

		// ȥ���õ������������
		double ideal_x, ideal_y;
		if (distType==0)
		{
			distortions::remove_weng(K0(0,0),K0(1,1),K0(0,2),K0(1,2),K0(0,1),
				dist(0),dist(1),dist(2),dist(3),dist(4),xy.x,xy.y,ideal_x,ideal_y);
		} 
		else
		{
			distortions::remove_brown(K0(0,0),K0(1,1),K0(0,2),K0(1,2),K0(0,1),
				dist(0),dist(1),dist(2),dist(3),dist(4),xy.x,xy.y,ideal_x,ideal_y);
		}

		// �����õ�����Ĺ�һ���������
		Matx31d nxy;
		nxy(0) = ideal_x;
		nxy(1) = ideal_y;
		nxy(2) = 1;

		nxys[i] = calib::invK(K0)*nxy;
	}

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+n,1,CV_64FC1,Scalar(0)),g_new(6*m+n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+n,1,CV_64FC1,Scalar(0));

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(0.5*(m+1)*m),U_new(0.5*(m+1)*m); // (m+1)m/2 �� Ujk ����
	vector<Matx<double,1,1>> V(n),V_new(n);
	vector<Matx<double,6,1>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,1,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,1,1>> db(n);
	vector<double> vds(l);

	derivatives::j_f_w_t_w0_t0_d0_new2(ds,Ks,Rs,ts,dists,distTypes,xys,covInvs,nxys,ri_j,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l_noref); // ��ͶӰ�в�ֻ�ǵ��ǲο����ͷ��

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j*m+j-0.5*j*(j+1)](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		Aii.push_back(V[i](0));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_1(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<double> ds_new = ds;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				double dbi = db[ii](0);
		
				ds_new[ii] += dbi;
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_t_w0_t0_d0_new2(ds_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,nxys,ri_j,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				ts=ts_new;
				ds=ds_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż����������ֵȫ����ת��Ϊ������������
	for (int i=0;i<n;++i)
	{
		double d0 = ds[i];
		Matx31d nxy0 = nxys[i];

		int k = ri_j[i];

		Matx33d R0 = Rs[k];
		Matx31d t0 = ts[k];

		Matx31d mX = R0.t()*(d0*nxy0-t0);

		Point3d XYZ;
		XYZ.x = mX(0);
		XYZ.y = mX(1);
		XYZ.z = mX(2);

		XYZs[i] = XYZ;
	}

	double err_rpj_final = sqrt(2*Fx/l_noref);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// 20150722��ÿ��������������ο�ͼ�����ֵ������
void SBA_ZZK::optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// ����������n���ռ������
									   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
									   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
									   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
									   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
									   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
									   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
									   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
									   const vector<Matx31d> & nxys,			// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ��ȥ����һ���������
									   const vector<int> & ri_j,				// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
									   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
									   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
									   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Wij�ڸ��������д洢��λ������
									   double * info /*= NULL*/,				// output:	runtime info, 5-vector
																				// info[0]:	the initial reprojection error
																				// info[1]:	the final reprojection error
																				// info[2]: final max gradient
																				// info[3]: the number of iterations
																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									   double tau /*= 1.0E-3*/,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									   int maxIter /*= 64*/,					// input:	the maximum number of iterations
									   double eps1 /*= 1.0E-8*/,				// input:	threshold
									   double eps2 /*= 1.0E-12*/				// input:	threshold
									   )
{
	int k = 0;		// ������������
	int v = 2;		// ���� u ʱ��Ҫ�õ���һ��������      
	double u;		// LM �Ż��㷨����ؼ�������ϵ�� (J'J + uI)h = -J'f
	double r;		// gain ratio, �����ʣ�������������չ��ʽ�ĺû�
	double g_norm;  // �ݶȵ�ģ
	double h_norm;	// ��������ģ

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // ��ʶ�Ƿ��Ѿ����������������
	int code = 2; // termination code

	int m = Ks.size();	// ͼ�����
	int n = XYZs.size();// ������
	int l = xys.size(); // ����������
	int l_noref = l-n;	// �ǲο����ĸ���

	// ��������ȳ�ֵ���Լ������Ĳο����򣨼��ο�ͼ�еĹ�һ��������꣩
	vector<double> ds(n);
	for (int i=0;i<n;++i)
	{
		int k = ri_j[i];

		Point3d XYZ = XYZs[i];

		Matx31d X;
		X(0) = XYZ.x;
		X(1) = XYZ.y;
		X(2) = XYZ.z;

		Matx33d K0 = Ks[k];
		Matx33d R0 = Rs[k];
		Matx31d t0 = ts[k];
		Matx<double,5,1> dist = dists[k];
		int distType = distTypes[k];

		X=R0*X+t0;

		ds[i] = X(2);
	}

	// Mat �ṹ
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+n,1,CV_64FC1,Scalar(0)),g_new(6*m+n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+n,1,CV_64FC1,Scalar(0));

	// [U W; W' V] ���ڵ�ǰ״̬�µ� J'covInv J ����Ҳ����δ����ķ��򷽳�ϵ������
	// [-ea; -eb] ���ǵ�ǰ״̬�µĲ����ݶ�����
	// [U_new W_new; W_new' V_new] ���Ǵ�ŵĺ�ѡ״̬�µ�δ���㷨�򷽳�ϵ������
	// [-ea_new; -eb_new] ���Ǵ�ŵĺ�ѡ״̬�µĲ����ݶ�����
	// ֻ�е���ѡ״̬����ڵ�ǰ״̬��ʹ��Ŀ�꺯��ֵ�½���ʱ�򣬺�ѡ״̬���ܱ����ɳ�Ϊ��ǰ״̬�����������ƴӵ�ǰ״̬��ʽ�ƶ�����ѡ״̬
	// Ҳֻ���ڴ�ʱ��U_new V_new W_new ea_new eb_new ����ʽȡ�� U V W ea eb
	vector<Matx<double,6,6>> U(0.5*(m+1)*m),U_new(0.5*(m+1)*m); // (m+1)m/2 �� Ujk ����
	vector<Matx<double,1,1>> V(n),V_new(n);
	vector<Matx<double,6,1>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,1,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,1,1>> db(n);
	vector<double> vds(l);

	derivatives::j_f_w_t_w0_t0_d0_new2(ds,Ks,Rs,ts,dists,distTypes,xys,covInvs,nxys,ri_j,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l_noref); // ��ͶӰ�в�ֻ�ǵ��ǲο����ͷ��

	g_norm = norm(g,NORM_INF);

	// �ݶ�������˵������ƽ̹����
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	vector<double> Aii;
	for (int j=0;j<m;++j)
	{
		for (int ii=0;ii<6;++ii)
		{
			Aii.push_back(U[j*m+j-0.5*j*(j+1)](ii,ii));
		}
	}
	for (int i=0;i<n;++i)
	{
		Aii.push_back(V[i](0));
	}

	auto iter = max_element(Aii.begin(),Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu

	while (!found && k<maxIter)
	{
		++k;

		// ���ڵ�ǰ����״̬�µ� [U W; W' V] �Լ��ݶ����� [-ea; -eb] ������ϵ�� u ����������
		derivatives::solveSBA_0_6_1(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // ����������
		{
			found = true;
			code = 1;
		} 
		else
		{
			vector<Matx33d> Rs_new = Rs;
			vector<Matx31d> ts_new = ts;
			vector<double> ds_new = ds;

			for (int jj=0;jj<m;++jj)
			{
				Matx<double,6,1> daj = da[jj];

				Matx33d dR = calib::converse_rotvec_R(daj(0), daj(1), daj(2));

				Rs_new[jj] = dR*Rs_new[jj];

				ts_new[jj](0)+=daj(3);
				ts_new[jj](1)+=daj(4);
				ts_new[jj](2)+=daj(5);
			}

			for (int ii=0;ii<n;++ii)
			{
				double dbi = db[ii](0);
		
				ds_new[ii] += dbi;
			}

			// ���ڸ������õ�һ��ѡ�������ƣ���������ѡ�������ƴ���Ŀ�꺯����Jacobian �����Լ��ݶ�����
			derivatives::j_f_w_t_w0_t0_d0_new2(ds_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,nxys,ri_j,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // ��ѡ��������Ŀ�꺯��ֵ

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // �ڵ�ǰ�����������ݶȺ͸�����Ԥ��������Ŀ�꺯���½���

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// �����²���
				Rs=Rs_new;
				ts=ts_new;
				ds=ds_new;

				// һ�������²������� Jacobian ������ݶ�����
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// �������²�������Ŀ�꺯��ֵ
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // �ݶ�������˵���ִ�ƽ̹����
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			} 
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	// ���Ż����������ֵȫ����ת��Ϊ������������
	for (int i=0;i<n;++i)
	{
		double d0 = ds[i];
		Matx31d nxy0 = nxys[i];

		int k = ri_j[i];

		Matx33d R0 = Rs[k];
		Matx31d t0 = ts[k];

		Matx31d mX = R0.t()*(d0*nxy0-t0);

		Point3d XYZ;
		XYZ.x = mX(0);
		XYZ.y = mX(1);
		XYZ.z = mX(2);

		XYZs[i] = XYZ;
	}

	double err_rpj_final = sqrt(2*Fx/l_noref);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}