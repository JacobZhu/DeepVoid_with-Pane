#include "stdafx.h"

// 线性成像点坐标对fx,fy,cx,cy,s共5个内参数求导
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

// 线性成像点坐标对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> derivatives::der_xy_fxfycxcys(double u, double v)
{
	Matx<double,2,5> ders;

	// [u 0 1 0 v]
	// [0 v 0 1 0]
	ders(0,0) = u;	ders(0,2) = 1;	ders(0,4) = v;
	ders(1,1) = v;	ders(1,3) = 1;

	return ders;
}

// 线性成像点坐标对fx,fy,cx,cy共4个内参数求导
Matx<double,2,4> derivatives::der_xy_fxfycxcy(double u, double v)
{
	Matx<double,2,4> ders;

	// [u 0 1 0]
	// [0 v 0 1]
	ders(0,0) = u;	ders(0,2) = 1;
	ders(1,1) = v;	ders(1,3) = 1;

	return ders;
}

// 线性成像点坐标对f,cx,cy共3个内参数求导
Matx<double,2,3> derivatives::der_xy_fcxcy(double u, double v)
{
	Matx<double,2,3> ders;

	// [u 1 0]
	// [v 0 1]
	ders(0,0) = u;	ders(0,1) = 1;
	ders(1,0) = v;	ders(1,2) = 1;

	return ders;
}

// 线性成像点坐标对f共1个内参数求导
Matx<double,2,1> derivatives::der_xy_f(double u, double v)
{
	Matx<double,2,1> ders;

	// [u]
	// [v]
	ders(0) = u;
	ders(1) = v;

	return ders;
}

// 线性成像点的归一化像点坐标对其线性成像点坐标求导
Matx22d derivatives::der_uv_xy(double fx, double fy, double s)
{
	Matx22d ders;

	double fx_1 = 1.0/fx;
	double fy_1 = 1.0/fy;

	ders(0,0)=fx_1;	ders(0,1)=-s*fx_1*fy_1;
	ders(1,1)=fy_1;

	return ders;
}

// 线性成像点坐标对其归一化像点坐标求导
Matx22d derivatives::der_xy_uv(double fx, double fy, double s)
{
	Matx22d ders;

	ders(0,0) = fx;	ders(0,1) = s;
	ders(1,1) = fy;

	return ders;
}

// 归一化像点坐标对fx,fy,cx,cy,s共5个内参数求导
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

// 归一化像点畸变量对归一化像点坐标求导
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

// 归一化像点畸变量对归一化像点坐标求导
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

// 像点基变量对归一化像点畸变量求导
Matx22d derivatives::der_dxdy_dudv(double fx, double fy, double s)
{
	Matx22d ders;

	ders(0,0) = fx;
	ders(0,1) = s;
	ders(1,1) = fy;

	return ders;
}

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
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

// 像点畸变量对fx,fy,cx,cy,共4个内参数求导
Matx<double,2,4> derivatives::der_dxdy_fxfycxcy(double du, double dv)
{
	Matx<double,2,4> ders;

	// [du  0  0  0]
	// [0  dv  0  0]
	ders(0,0) = du;
	ders(1,1) = dv;

	return ders;
}

// 像点畸变量对f,cx,cy,共3个内参数求导
Matx<double,2,3> derivatives::der_dxdy_fcxcy(double du, double dv)
{
	Matx<double,2,3> ders;

	// [du 0 0]
	// [dv 0 0]
	ders(0,0) = du;
	ders(1,0) = dv;

	return ders;
}

// 像点畸变量对f共1个内参数求导
Matx<double,2,1> derivatives::der_dxdy_f(double du, double dv)
{
	Matx<double,2,1> ders;

	// [du]
	// [dv]
	ders(0) = du;
	ders(1) = dv;

	return ders;
}

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
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

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,4> derivatives::der_dxdy_fxfycxcy_weng(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx<double,2,4> ders;

	double du,dv;

	distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);

	ders(0,0) = du;
	ders(1,1) = dv;

	return ders;
}

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
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

// 像点畸变量对fx,fy,cx,cy,s共4个内参数求导
Matx<double,2,4> derivatives::der_dxdy_fxfycxcy_brown(double u, double v, double k1, double k2, double k3, double k4, double k5)
{
	Matx<double,2,4> ders;

	double du,dv;

	distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);

	ders(0,0) = du;
	ders(1,1) = dv;

	return ders;
}

// 线性成像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
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

// 归一化像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
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

// 归一化像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
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

// 归一化像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
// 其中 v1 = r11 X + r12 Y + r13 Z + tx W
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

// 归一化像点坐标对旋转矩阵 R 各元素以及光心坐标 c 求导
// 其中 v1 = r11 (X-Cx) + r12 (Y-Cy) + r13 (Z-Cz)
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

// 归一化像点坐标对旋转矩阵 R 各元素以及光心坐标 c 求导
// 其中 v1 = r11 (X-WCx) + r12 (Y-WCy) + r13 (Z-WCz)
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

// 归一化像点坐标对空间点齐次坐标XYZW求导
// 其中 v1 = r11 X + r12 Y + r13 Z + tx W
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

// 归一化像点坐标对空间点坐标XYZ求导
// 其中 v1 = r11 X + r12 Y + r13 Z + tx
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

// 旋转矩阵R各元素对旋转向量w各元素求导，按照R=dR*R0，dR=I+[w]的方式求得的
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

// 刚体变换矩阵[R|t;0 1]各元素对旋转向量w各元素以及平移向量各元素求导，按照R=dR*R0，dR=I+[w]的方式求得的
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

// 旋转矩阵 R 以及光心坐标 c 对旋转向量 w 各元素以及光心坐标 c 各元素求导，按照R=dR*R0，dR=I+[w]的方式求得的
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

// 归一化像点畸变量对weng's像差系数求导
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

// 归一化像点畸变量对brown's像差系数求导
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

// constraint: 固定当前图与某图之间的基线长为某值: ||Rjck+tj||^2 - d^2 = 0
// 该约束方程对当前图的旋转矩阵以及平移向量求导数
Matx<double,1,12> derivatives::der_c_fixedBaseline_Rt(double Xkw,double Ykw, double Zkw,	// 图 k 的光心在世界系中的坐标
													  double Xkj,double Ykj, double Zkj		// 图 k 的光心在当前图像系中的坐标
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

// Matx<double,2,16> derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(double X, double Y, double Z,	// 该控制点的坐标
// 																 double x, double y,			// 该控制点的实际观测像点坐标
// 																 const Matx33d & mK,			// 当前的内参数估计
// 																 const Matx33d & mR,			// 当前的姿态估计
// 																 const Matx31d & mt,			// 当前的平移向量估计
// 																 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
// 																 double & dx, double & dy		// 当前估计下的重投影残差
// 																 )
// {
// 	Matx31d mX;
// 	mX(0)=X; mX(1)=Y; mX(2)=Z;
// 
// 	Matx31d mx = mR*mX+mt;
// 	double mx2_1 = 1.0/mx(2);
// 
// 	// 首先得到归一化像点坐标
// 	mx(0)*=mx2_1;
// 	mx(1)*=mx2_1;
// 	mx(2)=1;
// 
// 	double u = mx(0);
// 	double v = mx(1);
// 
// 	// 再通过内参数得到线性成像点坐标
// 	mx = mK*mx;
// 	double x0 = mx(0);
// 	double y0 = mx(1);
// 
// 	double fx = mK(0,0);	double fy = mK(1,1);
// 	double cx = mK(0,2);	double cy = mK(1,2);
// 	double s  = mK(0,1);	
// 
// 	// 计算畸变量
// 	double ddx,ddy;
// 	distortions::dxdy_weng(fx,fy,s,u,v,k1,k2,k3,k4,k5,ddx,ddy);
// 
// 	dx = x0+ddx-x;
// 	dy = y0+ddy-y;
// 
// 
// 	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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
// 	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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
// 	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,16> derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
															double x, double y,				// 该控制点的实际观测像点坐标
															const Matx33d & mK,				// 当前的内参数估计
															const Matx33d & mR,				// 当前的姿态估计
															const Matx31d & mt,				// 当前的平移向量估计
															double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
															int distType,					// 像差类型，0 weng's, 1 brown's
															double & dx, double & dy		// 当前估计下的重投影残差
															)
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv
	
	// 计算重投影残差量
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,5> j_xy_fxfycxcys = der_xy_fxfycxcys(u,v); // dx/da

	Matx<double,2,5> j_dxdy_fxfycxcys = der_dxdy_fxfycxcys(du,dv);	

	Matx<double,2,5> j_f_fxfycxcys = j_xy_fxfycxcys + j_dxdy_fxfycxcys; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,15> derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
														   double x, double y,				// 该控制点的实际观测像点坐标
														   const Matx33d & mK,				// 当前的内参数估计
														   const Matx33d & mR,				// 当前的姿态估计
														   const Matx31d & mt,				// 当前的平移向量估计
														   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
														   int distType,					// 像差类型，0 weng's, 1 brown's
														   double & dx, double & dy			// 当前估计下的重投影残差
														   )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_xy_fxfycxcy = der_xy_fxfycxcy(u,v); // dx/da

	Matx<double,2,4> j_dxdy_fxfycxcy = der_dxdy_fxfycxcy(du,dv);
	
	Matx<double,2,4> j_f_fxfycxcy = j_xy_fxfycxcy + j_dxdy_fxfycxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,14> derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
													    double x, double y,				// 该控制点的实际观测像点坐标
													    const Matx33d & mK,				// 当前的内参数估计
													    const Matx33d & mR,				// 当前的姿态估计
													    const Matx31d & mt,				// 当前的平移向量估计
													    double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
													    int distType,					// 像差类型，0 weng's, 1 brown's
													    double & dx, double & dy		// 当前估计下的重投影残差
													    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da

	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);

	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,12> derivatives::j_f_f_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
												    double x, double y,				// 该控制点的实际观测像点坐标
												    const Matx33d & mK,				// 当前的内参数估计
												    const Matx33d & mR,				// 当前的姿态估计
												    const Matx31d & mt,				// 当前的平移向量估计
												    double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
												    int distType,					// 像差类型，0 weng's, 1 brown's
												    double & dx, double & dy		// 当前估计下的重投影残差
												    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,1> j_xy_f = der_xy_f(u,v); // dx/da

	Matx<double,2,1> j_dxdy_f = der_dxdy_f(du,dv);

	Matx<double,2,1> j_f_f = j_xy_f + j_dxdy_f; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,11> derivatives::j_f_fxfycxcy_w_t_k1(double X, double Y, double Z,	// 该控制点的坐标
												   double x, double y,				// 该控制点的实际观测像点坐标
												   const Matx33d & mK,				// 当前的内参数估计
												   const Matx33d & mR,				// 当前的姿态估计
												   const Matx31d & mt,				// 当前的平移向量估计
												   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
												   int distType,					// 像差类型，0 weng's, 1 brown's
												   double & dx, double & dy			// 当前估计下的重投影残差
												   )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_xy_fxfycxcy = der_xy_fxfycxcy(u,v); // dx/da

	Matx<double,2,4> j_dxdy_fxfycxcy = der_dxdy_fxfycxcy(du,dv);

	Matx<double,2,4> j_f_fxfycxcy = j_xy_fxfycxcy + j_dxdy_fxfycxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,10> derivatives::j_f_fcxcy_w_t_k1(double X, double Y, double Z,	// 该控制点的坐标
											    double x, double y,				// 该控制点的实际观测像点坐标
											    const Matx33d & mK,				// 当前的内参数估计
											    const Matx33d & mR,				// 当前的姿态估计
											    const Matx31d & mt,				// 当前的平移向量估计
											    double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
											    int distType,					// 像差类型，0 weng's, 1 brown's
											    double & dx, double & dy		// 当前估计下的重投影残差
											    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da

	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);

	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,12> derivatives::j_f_fxfycxcy_w_t_k1k2(double X, double Y, double Z,	// 该控制点的坐标
													 double x, double y,			// 该控制点的实际观测像点坐标
													 const Matx33d & mK,			// 当前的内参数估计
													 const Matx33d & mR,			// 当前的姿态估计
													 const Matx31d & mt,			// 当前的平移向量估计
													 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
													 int distType,					// 像差类型，0 weng's, 1 brown's
													 double & dx, double & dy		// 当前估计下的重投影残差
													 )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_xy_fxfycxcy = der_xy_fxfycxcy(u,v); // dx/da

	Matx<double,2,4> j_dxdy_fxfycxcy = der_dxdy_fxfycxcy(du,dv);

	Matx<double,2,4> j_f_fxfycxcy = j_xy_fxfycxcy + j_dxdy_fxfycxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

Matx<double,2,11> derivatives::j_f_fcxcy_w_t_k1k2(double X, double Y, double Z,	// 该控制点的坐标
												  double x, double y,			// 该控制点的实际观测像点坐标
												  const Matx33d & mK,			// 当前的内参数估计
												  const Matx33d & mR,			// 当前的姿态估计
												  const Matx31d & mt,			// 当前的平移向量估计
												  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
												  int distType,					// 像差类型，0 weng's, 1 brown's
												  double & dx, double & dy		// 当前估计下的重投影残差
												  )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+mt;
	double mx2_1 = 1.0/mx(2);

	// 首先得到归一化像点坐标
	mx(0)*=mx2_1;
	mx(1)*=mx2_1;
	mx(2)=1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da

	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);

	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

void derivatives::j_f_w_t_XYZW(double X, double Y, double Z, double W,	// 该控制点的坐标
							   double x, double y,						// 该控制点的实际观测像点坐标
							   const Matx33d & mK,						// 当前的内参数估计
							   const Matx33d & mR,						// 当前的姿态估计
							   const Matx31d & mt,						// 当前的平移向量估计
							   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
							   int distType,							// 像差类型，0 weng's, 1 brown's
							   Matx<double,2,6> & A,					// 重投影像点坐标对像机参数求的导
							   Matx<double,2,4> & B,					// 重投影像点坐标对空间点坐标XYZW求的导
							   double & dx, double & dy					// 当前估计下的重投影残差
							   )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对空间点坐标求导 ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_uv_XYZW = der_uv_XYZW(mR,mt,V1,V2,V3);

	B = j_f_uv * j_uv_XYZW; // B = j_f_XYZW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_w_t_XYZ(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
							  double x, double y,						// 该控制点的实际观测像点坐标
							  const Matx33d & mK,						// 当前的内参数估计
							  const Matx33d & mR,						// 当前的姿态估计
							  const Matx31d & mt,						// 当前的平移向量估计
							  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
							  int distType,								// 像差类型，0 weng's, 1 brown's
							  Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
							  Matx<double,2,3> & B,						// 重投影像点坐标对空间点坐标 XYZ 求的导
							  double & dx, double & dy					// 当前估计下的重投影残差
							  )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对空间点坐标求导 ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	B = j_f_uv * j_uv_XYZ; // B = j_f_XYZ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_w_c_XYZW(double X, double Y, double Z, double W,		// 该控制点的齐次坐标
							   double x, double y,							// 该控制点的实际观测像点坐标
							   const Matx33d & mK,							// 当前的内参数估计
							   const Matx33d & mR,							// 当前的姿态估计
							   const Matx31d & mc,							// 当前的光心坐标估计
							   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
							   int distType,								// 像差类型，0 weng's, 1 brown's
							   Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
							   Matx<double,2,4> & B,						// 重投影像点坐标对空间点坐标 XYZW 求的导
							   double & dx, double & dy						// 当前估计下的重投影残差
							   )
{
	Matx31d mt = -mR*mc; // 计算平移向量 t = -RC

	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*(mX-W*mc);

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对空间点坐标求导 ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_uv_XYZW = der_uv_XYZW(mR,mt,V1,V2,V3);

	B = j_f_uv * j_uv_XYZW; // B = j_f_XYZW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_w_c_XYZ(double X, double Y, double Z, double W,	// 该控制点的齐次坐标，此处 W 应为 1
							  double x, double y,						// 该控制点的实际观测像点坐标
							  const Matx33d & mK,						// 当前的内参数估计
							  const Matx33d & mR,						// 当前的姿态估计
							  const Matx31d & mc,						// 当前的光心坐标估计
							  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
							  int distType,								// 像差类型，0 weng's, 1 brown's
							  Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
							  Matx<double,2,3> & B,						// 重投影像点坐标对空间点坐标 XYZ 求的导
							  double & dx, double & dy					// 当前估计下的重投影残差
							  )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*(mX-W*mc);

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对空间点坐标求导 ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	B = j_f_uv * j_uv_XYZ; // B = j_f_XYZ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_f_w_t_XYZW(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
								 double x, double y,						// 该控制点的实际观测像点坐标
								 const Matx33d & mK,						// 当前的内参数估计
								 const Matx33d & mR,						// 当前的姿态估计
								 const Matx31d & mt,						// 当前的平移向量估计
								 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
								 int distType,								// 像差类型，0 weng's, 1 brown's
								 Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
								 Matx<double,2,4> & B,						// 重投影像点坐标对空间点坐标XYZW求的导
								 Matx<double,2,1> & C,						// 重投影像点坐标对一个等效焦距 f 求导
								 double & dx, double & dy					// 当前估计下的重投影残差
								 )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*mX+W*mt;

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,1> j_xy_f = der_xy_f(u,v); // dx/da

	Matx<double,2,1> j_dxdy_f = der_dxdy_f(du,dv);

	C = j_xy_f + j_dxdy_f; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对空间点坐标求导 ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,4> j_uv_XYZW = der_uv_XYZW(mR,mt,V1,V2,V3);

	B = j_f_uv * j_uv_XYZW; // B = j_f_XYZW
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void derivatives::j_f_f_w_c_XYZ(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
							    double x, double y,							// 该控制点的实际观测像点坐标
							    const Matx33d & mK,							// 当前的内参数估计
							    const Matx33d & mR,							// 当前的姿态估计
							    const Matx31d & mc,							// 当前的光心坐标估计
							    double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
							    int distType,								// 像差类型，0 weng's, 1 brown's
							    Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
							    Matx<double,2,3> & B,						// 重投影像点坐标对空间点坐标XYZ求的导
							    Matx<double,2,1> & C,						// 重投影像点坐标对一个等效焦距 f 求导
							    double & dx, double & dy					// 当前估计下的重投影残差
							    )
{
	Matx31d mX;
	mX(0)=X; mX(1)=Y; mX(2)=Z;

	Matx31d mx = mR*(mX-W*mc);

	double V1 = mx(0); // r11X + r12Y + r13Z + txW
	double V2 = mx(1); // r21X + r22Y + r23Z + tyW
	double V3 = mx(2); // r31X + r32Y + r33Z + tzW
	double V3_1 = 1.0/V3;

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,1> j_xy_f = der_xy_f(u,v); // dx/da

	Matx<double,2,1> j_dxdy_f = der_dxdy_f(du,dv);

	C = j_xy_f + j_dxdy_f; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对空间点坐标求导 ///////////////////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	B = j_f_uv * j_uv_XYZ; // B = j_f_XYZ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// 基于深度的 BA 方法中，一个可调整像点坐标 xij 对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void derivatives::j_f_w_t_w0_t0_d0(double d0,				// 输入：当前该物点相对于其参考图像的深度估计
								   double x, double y,		// 输入：该物点于当前成像图像中的观测像点坐标
								   const Matx33d & mK,		// 输入：当前成像图像的内参数估计
								   const Matx33d & mR,		// 输入：当前成像图像的姿态估计
								   const Matx31d & mt,		// 输入：当前成像图像的平移向量估计
								   double k1,double k2,double k3, double k4, double k5,	// 输入：当前成像图像的像差系数估计
								   int distType,			// 输入：当前成像图像的像差类型，0 weng's, 1 brown's
								   const Matx31d & nx0,		// 输入：该物点于其参考图像系中的归一化像点坐标，表征了其点心射线方向
								   const Matx33d & mR0,		// 输入：当前参考图像的姿态估计
								   const Matx31d & mt0,		// 输入：当前参考图像的平移向量估计
								   Matx<double,2,6> & A,	// 输出：重投影像点坐标对成像图像参数求的导
								   Matx<double,2,1> & B,	// 输出：重投影像点坐标对其深度求的导
								   Matx<double,2,6> & A0,	// 输出：重投影像点坐标对参考图像参数求的导
								   double & dx, double & dy	// 输出：当前估计下的重投影残差
								   )
{
	Matx31d mX;

	Matx33d mR0t = mR0.t();
	Matx31d mtmp31 = d0*nx0-mt0;

	// 基于当前相对于参考图的深度估计以及参考图当前的参数估计来得到物点的世界坐标
	mX = mR0t*mtmp31;

	double X = mX(0);
	double Y = mX(1);
	double Z = mX(2);
	double W = 1;

	// 该基于深度生成的物点坐标再分别对 R0, t0, d0 求导
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

	// 首先得到归一化像点坐标
	mx(0) = V1*V3_1; // u = V1/V3
	mx(1) = V2*V3_1; // v = V2/V3
	mx(2) = 1;

	double u = mx(0);
	double v = mx(1);

	// 再通过内参数得到线性成像点坐标
	mx = mK*mx;
	double x0 = mx(0);
	double y0 = mx(1);

	double fx = mK(0,0);	double fy = mK(1,1);
	double cx = mK(0,2);	double cy = mK(1,2);
	double s  = mK(0,1);	

	// 先计算归一化像点畸变量
	double du,dv;

	if (0==distType)
	{
		distortions::dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);
	} 
	else
	{
		distortions::dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);
	}

	// 进而计算像点畸变量
	double ddx,ddy;

	ddx = fx*du + s*dv; // dx = f_x du + s dv
	ddy = fy*dv;		// dy = f_y dv

	// 计算重投影残差
	dx = x0+ddx-x;
	dy = y0+ddy-y;


	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,3> j_xy_fcxcy = der_xy_fcxcy(u,v); // dx/da
// 
// 	Matx<double,2,3> j_dxdy_fcxcy = der_dxdy_fcxcy(du,dv);
// 
// 	Matx<double,2,3> j_f_fcxcy = j_xy_fcxcy + j_dxdy_fcxcy; // df/da = dx/da + ddx/da
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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


	// 对参考图像参数以及深度深度求导 /////////////////////////////////////////////////////////////////////////////////
	Matx<double,2,3> j_uv_XYZ = der_uv_XYZ(mR,V1,V2,V3);

	Matx<double,2,3> j_f_XYZ = j_f_uv * j_uv_XYZ; // j_f_XYZ

	A0 = j_f_XYZ * j_XYZ_R0t0 * der_Rt_wt(mR0); // A0 = j_f_w0t0

	B = j_f_XYZ * j_XYZ_d0; // B = j_f_d0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// void derivatives::j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
// 							   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
// 							   const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
// 							   const vector<Matx31d> & ts,				// 输入：m个图像平移向量
// 							   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
// 							   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
// 							   const vector<Point2d> & xys,				// 输入：所有图像上的像点坐标，最多最多为 m*n 个
// 							   const vector<int> & j_fixed,				// 输入：哪些图像的参数是固定的，如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
// 							   const vector<int> & i_fixed,				// 输入：哪些空间点坐标是固定的，如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
// 							   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
// 							   vector<Matx<double,2,6>> & A,			// 输出：所有的Aij按行主序排成一个向量
// 							   vector<Matx<double,2,4>> & B,			// 输出：所有的Bij按行主序排成一个向量
// 							   vector<Matx21d> & e						// 输出：所有的 eij = xij - Q(aj,bi) 按行主序排成一个向量
// 							   )
// {
// 	int n = XYZWs.size(); // 空间点个数
// 	int m = Ks.size(); // 图像个数
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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
// 	// 把所有固定参数图像的 Aij 置 0
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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
// 	// 把所有固定坐标的空间点的 Bij 置 0
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
							   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							   const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
							   const vector<Matx31d> & ts,				// 输入：m个图像平移向量
							   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
							   vector<Matx<double,6,6>> & U,			// 输出：m个Uj矩阵，仅跟图像参数有关
							   vector<Matx<double,4,4>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
							   vector<Matx<double,6,4>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
							   vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
							   vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
							   Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
							   Mat & g,									// 输出：6*m+4*n维的参数梯度
							   vector<double> & vds						// 输出：l个像点的重投影残差量
							   )
{
	int n = XYZWs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,4>> B(l); // 所有的 Bij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}
			
			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// 按列扫，计算所有的 Uj、eaj 和 Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi;
		Matx<double,4,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_w_t_XYZ(const vector<Point3d> & XYZs,				// 输入：n个空间点XYZ坐标
							  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
							  const vector<Matx31d> & ts,				// 输入：m个图像平移向量
							  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
							  vector<Matx<double,6,6>> & U,				// 输出：m个Uj矩阵，仅跟图像参数有关
							  vector<Matx<double,3,3>> & V,				// 输出：n个Vi矩阵，仅跟空间点坐标有关
							  vector<Matx<double,6,3>> & W,				// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
							  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
							  vector<Matx<double,3,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
							  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
							  Mat & g,									// 输出：6*m+3*n维的参数梯度
							  vector<double> & vds						// 输出：l个像点的重投影残差量
							  )
{
	int n = XYZs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,3>> B(l); // 所有的 Bij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// 按列扫，计算所有的 Uj、eaj 和 Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi;
		Matx<double,3,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_w_c_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
							   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							   const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
							   const vector<Matx31d> & cs,				// 输入：m个图像光心坐标
							   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
							   vector<Matx<double,6,6>> & U,			// 输出：m个Uj矩阵，仅跟图像参数有关
							   vector<Matx<double,4,4>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
							   vector<Matx<double,6,4>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
							   vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
							   vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
							   Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
							   Mat & g,									// 输出：6*m+4*n维的参数梯度
							   vector<double> & vds						// 输出：l个像点的重投影残差量
							   )
{
	int n = XYZWs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,4>> B(l); // 所有的 Bij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// 按列扫，计算所有的 Uj、eaj 和 Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi;
		Matx<double,4,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_w_c_XYZ(const vector<Point3d> & XYZs,				// 输入：n个空间点XYZ坐标
							  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
							  const vector<Matx31d> & cs,				// 输入：m个图像光心坐标
							  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
							  vector<Matx<double,6,6>> & U,				// 输出：m个Uj矩阵，仅跟图像参数有关
							  vector<Matx<double,3,3>> & V,				// 输出：n个Vi矩阵，仅跟空间点坐标有关
							  vector<Matx<double,6,3>> & W,				// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
							  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
							  vector<Matx<double,3,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
							  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
							  Mat & g,									// 输出：6*m+3*n维的参数梯度
							  vector<double> & vds						// 输出：l个像点的重投影残差量
							  )
{
	int n = XYZs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,3>> B(l); // 所有的 Bij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			e[*ptr] = eij;			
		}
	}

	// 按列扫，计算所有的 Uj、eaj 和 Wij
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj;
		Matx<double,6,1> eaj;

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi;
		Matx<double,3,1> ebi;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
								 const vector<Matx33d> & Ks,			// 输入：m个图像内参数矩阵
								 const vector<Matx33d> & Rs,			// 输入：m个图像旋转矩阵
								 const vector<Matx31d> & ts,			// 输入：m个图像平移向量
								 const vector<Matx<double,5,1>> & dists,// 输入：m个图像像差系数
								 const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								 const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								 const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								 const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								 const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								 vector<Matx<double,6,6>> & U,			// 输出：m个Uj矩阵，仅跟图像参数有关
								 vector<Matx<double,4,4>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
								 vector<Matx<double,6,4>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
								 Matx<double,1,1> & Q,					// 输出：唯一一个Q矩阵，只跟共参数有关
								 vector<Matx<double,1,6>> & G,			// 输出：m个Gj矩阵，同时和独有及共有图像参数有关
								 vector<Matx<double,1,4>> & H,			// 输出：n个Hi矩阵，同时和共有图像参数以及物点有关
								 vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
								 vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
								 Matx<double,1,1> & ec,					// 输出：唯一一个ec残差向量，仅跟共有图像参数有关
								 Mat & f,								// 输出：2*l个像点残差量，其实也就是评价的目标函数值
								 Mat & g,								// 输出：1+6*m+4*n维的参数梯度
								 vector<double> & vds					// 输出：l个像点的重投影残差量
								 )
{
	int n = XYZWs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,4>> B(l); // 所有的 Bij 矩阵
	vector<Matx<double,2,1>> C(l); // 所有的 Cij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// 按列扫，计算所有的 Uj、eaj、Wij、Q=sum(Qij)、Gj 和 ec
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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_f_w_c_XYZ(const vector<Point3d> & XYZs,			// 输入：n个空间点XYZ坐标
							    const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							    const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
							    const vector<Matx31d> & cs,				// 输入：m个图像光心坐标
							    const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							    const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							    const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							    const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							    const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							    const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							    const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
							    vector<Matx<double,6,6>> & U,			// 输出：m个Uj矩阵，仅跟图像参数有关
							    vector<Matx<double,3,3>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
							    vector<Matx<double,6,3>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
							    Matx<double,1,1> & Q,					// 输出：唯一一个Q矩阵，只跟共参数有关
							    vector<Matx<double,1,6>> & G,			// 输出：m个Gj矩阵，同时和独有及共有图像参数有关
							    vector<Matx<double,1,3>> & H,			// 输出：n个Hi矩阵，同时和共有图像参数以及物点有关
							    vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
							    vector<Matx<double,3,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
							    Matx<double,1,1> & ec,					// 输出：唯一一个ec残差向量，仅跟共有图像参数有关
							    Mat & f,								// 输出：2*l个像点残差量，其实也就是评价的目标函数值
							    Mat & g,								// 输出：1+6*m+3*n维的参数梯度
							    vector<double> & vds					// 输出：l个像点的重投影残差量
							    )
{
	int n = XYZs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,3>> B(l); // 所有的 Bij 矩阵
	vector<Matx<double,2,1>> C(l); // 所有的 Cij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// 按列扫，计算所有的 Uj、eaj、Wij、Q=sum(Qij)、Gj 和 ec
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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::j_f_f_w_t_XYZW_c_fixedBaseline(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
												 const vector<Matx33d> & Ks,			// 输入：m个图像内参数矩阵
												 const vector<Matx33d> & Rs,			// 输入：m个图像旋转矩阵
												 const vector<Matx31d> & ts,			// 输入：m个图像平移向量
												 const vector<Matx<double,5,1>> & dists,// 输入：m个图像像差系数
												 const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
												 const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
												 const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
												 const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
												 const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
												 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
												 int k_fixedBaseLine,					// 输入：哪两个图像之间的基线固定不动，k 是不调整的一端
												 int j_fixedBaseLine,					// 输入：哪两个图像之间的基线固定不动，j 是调整的一端
												 double d_fixedBaseLine,				// 输入：两幅图像之间的固定基线长度
												 vector<Matx<double,6,6>> & U,			// 输出：m个Uj矩阵，仅跟图像参数有关
												 vector<Matx<double,4,4>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
												 vector<Matx<double,6,4>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
												 Matx<double,1,1> & Q,					// 输出：唯一一个Q矩阵，只跟共参数有关
												 vector<Matx<double,1,6>> & G,			// 输出：m个Gj矩阵，同时和独有及共有图像参数有关
												 vector<Matx<double,1,4>> & H,			// 输出：n个Hi矩阵，同时和共有图像参数以及物点有关
												 vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
												 vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
												 Matx<double,1,1> & ec,					// 输出：唯一一个ec残差向量，仅跟共有图像参数有关
												 Mat & f,								// 输出：2*l个像点残差量，其实也就是评价的目标函数值
												 Mat & g,								// 输出：1+6*m+4*n维的参数梯度
												 vector<double> & vds,					// 输出：l个像点的重投影残差量
												 Matx<double,1,6> & Daj,				// 输出：约束方程对固定基线的 j 端外参数求导
												 double & h								// 输出：约束方程当前的值
												 )
{
	int n = XYZWs.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,4>> B(l); // 所有的 Bij 矩阵
	vector<Matx<double,2,1>> C(l); // 所有的 Cij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij 和 eij
	for (int i=0;i<n;++i)
	{
		Point4d XYZW = XYZWs[i];

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			C[*ptr] = Cij;
			e[*ptr] = eij;			
		}
	}

	Matx<double,1,1> Q_tmp, ec_tmp;

	// 按列扫，计算所有的 Uj、eaj、Wij、Q=sum(Qij)、Gj 和 ec
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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 按行扫，计算所有的 Vi 和 ebi
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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

	// 评价约束方程 h(x)
	Matx33d Rj = Rs[j_fixedBaseLine];
	Matx31d Ckw = -Rs[k_fixedBaseLine].t() * ts[k_fixedBaseLine];
	Matx31d Ckj = Rj * Ckw + ts[j_fixedBaseLine];
	double Cx_kj = Ckj(0);
	double Cy_kj = Ckj(1);
	double Cz_kj = Ckj(2);

	h = Cx_kj*Cx_kj + Cy_kj*Cy_kj + Cz_kj*Cz_kj - d_fixedBaseLine*d_fixedBaseLine;

	Daj = der_c_fixedBaseline_Rt(Ckw(0),Ckw(1),Ckw(2),Cx_kj,Cy_kj,Cz_kj) * der_Rt_wt(Rj);
}

// 基于深度的 BA 方法中，所有可调整像点坐标 xij 对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void derivatives::j_f_w_t_w0_t0_d0(const vector<double> & ds,				// 输入：n个空间点关联的深度值
								   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								   const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
								   const vector<Matx31d> & ts,				// 输入：m个图像平移向量
								   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								   const vector<Point2d> & xys,				// 输入：l个所有图像上的可调整像点坐标，最多最多为 m*n 个
								   const vector<Matx22d> & covInvs,			// 输入：l个所有可调整像点坐标协方差矩阵的逆矩阵
								   const vector<Matx31d> & nxys,			// 输入：n个物点关联的于其参考图像系中的观测视线方向，也即归一化像点坐标
								   const vector<int> & ri_j,				// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
								   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点深度是固定的（i_fixed[i]=1），如果点 i 深度固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								   vector<Matx<double,6,6>> & U,			// 输出：总共(m+1)m/2个Ujk矩阵，仅跟图像参数有关
								   vector<double> & V,						// 输出：n个Vi矩阵，仅跟深度有关
								   vector<Matx<double,6,1>> & W,			// 输出：l个Wij矩阵，同时跟深度及其观测图像有关
								   vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
								   vector<double> & eb,						// 输出：n个ebi残差向量，仅跟空间点坐标有关
								   Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
								   Mat & g,									// 输出：6*m+n维的参数梯度
								   vector<double> & vds						// 输出：l个像点的重投影残差量
								   )
{
	int n = ds.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵，即像点 xij 对图像 j 的参数求导
	vector<Matx<double,2,1>> B(l); // 所有的 Bij 矩阵，即像点 xij 对物点 i 的深度求导
	vector<Matx<double,2,6>> C(l); // 所有的 Cij 矩阵，即像点 xij 对其参考图像 k 的参数求导
	vector<Matx21d> e(l); // 所有的 eij 残差向量
	vector<vector<int>> rj_i(m); // 记录所有以图 j 为参考图像的物点 i，一个 j 可以对应多个 i

	// 计算所有的 Aij、Bij、Cij 和 eij
	for (int i=0;i<n;++i)
	{
		double d0 = ds[i];		// 点 i 的深度
		int idx0 = ri_j[i];		// 点 i 参考图像的索引
		Matx31d nxy0 = nxys[i];	// 点 i 关联的归一化像点坐标
		Matx33d R0 = Rs[idx0];	// 点 i 参考图像的旋转矩阵
		Matx31d t0 = ts[idx0];	// 点 i 参考图像的平移向量

		rj_i[idx0].push_back(i);

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			if (!j_fixed[idx0])
			{
				// 只有当点 i 的参考图参数参与优化时，才更新 Cij，否则不动作，这就要求所有的 Cij 在 C 中一开始就全部初始化为 0 矩阵
				C[*ptr] = Cij;
			}

			e[*ptr] = eij;
		}
	}

	// 生成所有的 Ujj
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Ujj;

		// 先考虑所有不以图 j 为参考图，但在图 j 上被观测到的像点
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx22d cij = covInvs[*ptr];

			Ujj += Aij.t()*cij*Aij;
		}

		vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i

		// 再考虑所有以图 j 为参考图，被其它各图像观测到的像点
		for (int ii=0;ii<i_j.size();++ii)
		{
			int i = i_j[ii];

			for (int k=0;k<m;++k)
			{
				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xik 不存在
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

	// 生成所有的 Ujk
	for (int j=0;j<m;++j)
	{
		for (int k=j+1;k<m;++k)
		{
			Matx<double,6,6> Ujk;

			// 先考虑所有不以图 j 而以图 k 为参考图，在图 j 上被观测到的像点
			for (int i=0;i<n;++i)
			{
				// 如果点 i 的参考图不是图 k 那么直接 continue
				if (ri_j[i] != k)
				{
					continue;
				}

				const int * ptr = ptrMat.find<int>(i,j);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xij 不存在
					continue;
				}

				Matx<double,2,6> Aij = A[*ptr];
				Matx22d cij = covInvs[*ptr];
				Matx<double,2,6> Cij = C[*ptr];

				Ujk += Aij.t()*cij*Cij;
			}

			vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i

			// 再考虑所有以图 j 为参考图，被图 k 观测到的像点
			for (int ii=0;ii<i_j.size();++ii)
			{
				int i = i_j[ii];

				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xik 不存在
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

	// 尝试在生成各 Ujj 的同时，生成所有的 Ujk
// 	U = vector<Matx<double,6,6>>(0.5*m*(m+1));
// 
// 	for (int j=0;j<m;++j)
// 	{
// 		Matx<double,6,6> Ujj;
// 
// 		// 先考虑所有不以图 j 为参考图，但在图 j 上被观测到的像点
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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
// 			// 循环中，同时考虑以具体的图 k 为参考图，在图 j 上被观测到的像点
// 			int k = ri_j[i]; // 像点 ij 的参考图索引
// 
// 			if (k>j) // 只有上三角中才有 Ujk
// 			{
// 				int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 				U[tmpidx] += Aijtcij*Cij;
// 			}
// 		}
// 
// 		vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i
// 
// 		// 再考虑所有以图 j 为参考图，被其它各图像观测到的像点
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
// 					// 如果 ptr == NULL，说明像点 xik 不存在
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
// 				// 循环中，同时考虑以图 j 为参考图，被具体的图 k 观测到的像点
// 				if (k>j) // 只有上三角中才有 Ujk
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

	// 生成 Wij，同时和点 i 以及图 j 相关
	for (int j=0;j<m;++j)
	{
		for (int i=0;i<n;++i)
		{
			if (ri_j[i]!=j)
			{
				// 如果点 i 的参考图不是 j，那么同时和点 i 及图 j 相关的像点就只有 xij 了
				const int * ptr = ptrMat.find<int>(i,j);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xij 不存在
					continue;
				}

				Matx<double,2,6> Aik = A[*ptr];
				Matx22d cik = covInvs[*ptr];
				Matx<double,2,6> Cik = C[*ptr];
			}
			else
			{
				// 如果点 i 的参考图是 j，那么同时和点 i 及图 j 相关的像点就是所有的{xik}了

			}
		}
	}


	// 按列扫，计算所有的 Uj、eaj 和 Wij
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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
// 	// 按行扫，计算所有的 Vi 和 ebi
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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

// 基于深度的 BA 方法中，所有像点坐标 xij （包括可调整的非参考像点以及不可调整的参考像点）对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void derivatives::j_f_w_t_w0_t0_d0_new(const vector<double> & ds,			// 输入：n个空间点关联的深度值
								  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
								  const vector<Matx31d> & ts,				// 输入：m个图像平移向量
								  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								  const vector<Point2d> & xys,				// 输入：l个观测像点坐标，最多为 m*n 个
								  const vector<Matx22d> & covInvs,			// 输入：l个观测像点坐标协方差矩阵的逆矩阵
								  const vector<Matx31d> & nxys,				// 输入：n个物点关联的于其参考图像系中的观测视线方向，也即归一化像点坐标
								  const vector<int> & ri_j,					// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
								  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点深度是固定的（i_fixed[i]=1），如果点 i 深度固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								  vector<Matx<double,6,6>> & U,				// 输出：总共(m+1)m/2个Ujk矩阵，仅跟图像参数有关
								  vector<double> & V,						// 输出：n个Vi矩阵，仅跟深度有关
								  vector<Matx<double,6,1>> & W,				// 输出：l个Wij矩阵，同时跟深度及其观测图像有关
								  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
								  vector<double> & eb,						// 输出：n个ebi残差向量，仅跟空间点坐标有关
								  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
								  Mat & g,									// 输出：6*m+n维的参数梯度
								  vector<double> & vds						// 输出：l个像点的重投影残差量
								  )
{
	int n = ds.size(); // 空间点个数
	int m = Ks.size(); // 图像个数
	int l = xys.size(); // 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵，即像点 xij 对图像 j 的参数求导
	vector<Matx<double,2,1>> B(l); // 所有的 Bij 矩阵，即像点 xij 对物点 i 的深度求导
	vector<Matx<double,2,6>> C(l); // 所有的 Cij 矩阵，即像点 xij 对其参考图像 k 的参数求导
	vector<Matx21d> e(l); // 所有的 eij 残差向量
	vector<vector<int>> rj_i(m); // 记录所有以图 j 为参考图像的物点 i，一个 j 可以对应多个 i

	// 计算所有的 Aij、Bij、Cij 和 eij
	for (int i=0;i<n;++i)
	{
		double d0 = ds[i];		// 点 i 的深度
		int idx0 = ri_j[i];		// 点 i 参考图像的索引
		Matx31d nxy0 = nxys[i];	// 点 i 关联的归一化像点坐标
		Matx33d R0 = Rs[idx0];	// 点 i 参考图像的旋转矩阵
		Matx31d t0 = ts[idx0];	// 点 i 参考图像的平移向量

		rj_i[idx0].push_back(i);

		for (int j=0;j<m;++j)
		{
			if (j==idx0)
			{
				// 此时说明像点 xij 是肯定存在的，且为点 i 的参考像点，参考像点 xij 是固定不动的，不随任何参数改变
				// 即 Aij=Bij=Cij,j=0，另外残差量 eij=0 显然也成立
				continue;
			}

			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
				// 只有当 j 图不固定，参与优化时，才更新 Aij，否则不动作，这就要求所有的 Aij 在 A 中一开始就全部初始化为 0 矩阵
				A[*ptr] = Aij;
			}

			if (!i_fixed[i])
			{
				// 只有当 i 点不固定，参与优化时，才更新 Bij，否则不动作，这就要求所有的 Bij 在 B 中一开始就全部初始化为 0 矩阵
				B[*ptr] = Bij;
			}

			if (!j_fixed[idx0])
			{
				// 只有当点 i 的参考图参数参与优化时，才更新 Cij，否则不动作，这就要求所有的 Cij 在 C 中一开始就全部初始化为 0 矩阵
				C[*ptr] = Cij;
			}

			e[*ptr] = eij;
		}
	}

	// 生成所有的 Ujj
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Ujj;

		// 先考虑所有不以图 j 为参考图，但在图 j 上被观测到的像点
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx22d cij = covInvs[*ptr];

			Ujj += Aij.t()*cij*Aij;
		}

		vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i

		// 再考虑所有以图 j 为参考图，被其它各图像观测到的像点
		for (int ii=0;ii<i_j.size();++ii)
		{
			int i = i_j[ii];

			for (int k=0;k<m;++k)
			{
				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xik 不存在
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

	// 生成所有的 Ujk
	for (int j=0;j<m;++j)
	{
		for (int k=j+1;k<m;++k)
		{
			Matx<double,6,6> Ujk;

			// 先考虑所有不以图 j 而以图 k 为参考图，在图 j 上被观测到的像点
			for (int i=0;i<n;++i)
			{
				// 如果点 i 的参考图不是图 k 那么直接 continue
				if (ri_j[i] != k)
				{
					continue;
				}

				const int * ptr = ptrMat.find<int>(i,j);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xij 不存在
					continue;
				}

				Matx<double,2,6> Aij = A[*ptr];
				Matx22d cij = covInvs[*ptr];
				Matx<double,2,6> Cij = C[*ptr];

				Ujk += Aij.t()*cij*Cij;
			}

			vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i

			// 再考虑所有以图 j 为参考图，被图 k 观测到的像点
			for (int ii=0;ii<i_j.size();++ii)
			{
				int i = i_j[ii];

				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xik 不存在
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

	// 尝试在生成各 Ujj 的同时，生成所有的 Ujk
// 	U = vector<Matx<double,6,6>>(0.5*m*(m+1));
// 
// 	for (int j=0;j<m;++j)
// 	{
// 		Matx<double,6,6> Ujj;
// 
// 		// 先考虑所有不以图 j 为参考图，但在图 j 上被观测到的像点
// 		for (int i=0;i<n;++i)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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
// 			// 循环中，同时考虑以具体的图 k 为参考图，在图 j 上被观测到的像点
// 			int k = ri_j[i]; // 像点 ij 的参考图索引
// 
// 			if (k>j) // 只有上三角中才有 Ujk
// 			{
// 				int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
// 				U[tmpidx] += Aijtcij*Cij;
// 			}
// 		}
// 
// 		vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i
// 
// 		// 再考虑所有以图 j 为参考图，被其它各图像观测到的像点
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
// 					// 如果 ptr == NULL，说明像点 xik 不存在
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
// 				// 循环中，同时考虑以图 j 为参考图，被具体的图 k 观测到的像点
// 				if (k>j) // 只有上三角中才有 Ujk
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

	// 生成 Wij，同时和点 i 以及图 j 相关
	for (int j=0;j<m;++j)
	{
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在，那么 Wij=0
				continue;
			}

			if (ri_j[i]!=j)
			{
				// 如果点 i 的参考图不是 j，那么同时和点 i 及图 j 相关的像点就只有 xij 了
				Matx<double,2,6> Aij = A[*ptr];
				Matx22d cij = covInvs[*ptr];
				Matx<double,2,1> Bij = B[*ptr];

				W[*ptr] = Aij.t()*cij*Bij;
			}
			else
			{
				Matx<double,6,1> Wij;

				// 如果点 i 的参考图是 j，那么同时和点 i 及图 j 相关的像点就是所有的{xik}了
				for (int k=0;k<m;++k)
				{
					const int * ptr = ptrMat.find<int>(i,k);

					if (NULL == ptr)
					{
						// 如果 ptr == NULL，说明像点 xik 不存在
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

	// 尝试在生成各 Ujj 的同时，生成所有的 Ujk，及 Wij
	U = vector<Matx<double,6,6>>(0.5*m*(m+1));

	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Ujj;

		// 先考虑所有不以图 j 为参考图，但在图 j 上被观测到的像点
		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Matx<double,2,6> Aij = A[*ptr];
			Matx<double,2,1> Bij = B[*ptr];
			Matx22d cij = covInvs[*ptr];
			Matx<double,2,6> Cij = C[*ptr];

			Matx<double,6,2> Aijtcij = Aij.t()*cij;

			Ujj += Aijtcij*Aij;

			// 循环中，同时考虑以具体的图 k 为参考图，在图 j 上被观测到的像点
			int k = ri_j[i]; // 像点 ij 的参考图索引

			if (k>j) // 只有上三角中才有 Ujk
			{
				int tmpidx = j*m+k-0.5*j*(j+1); // i*m+j-i(i+1)/2
				U[tmpidx] += Aijtcij*Cij;
			}

			if (ri_j[i]!=j)
			{
				W[*ptr] = Aijtcij*Bij;
			}
		}

		vector<int> i_j = rj_i[j]; // 所有以图 j 为参考图像的物点 i

		// 再考虑所有以图 j 为参考图，被其它各图像观测到的像点
		for (int ii=0;ii<i_j.size();++ii)
		{
			int i = i_j[ii];

			Matx<double,6,1> Wij;

			for (int k=0;k<m;++k)
			{
				const int * ptr = ptrMat.find<int>(i,k);

				if (NULL == ptr)
				{
					// 如果 ptr == NULL，说明像点 xik 不存在
					continue;
				}

				Matx<double,2,6> Aik = A[*ptr];
				Matx<double,2,1> Bik = B[*ptr];
				Matx22d cik = covInvs[*ptr];
				Matx<double,2,6> Cik = C[*ptr];

				Matx<double,6,2> Ciktcik = Cik.t()*cik;

				Ujj += Ciktcik*Cik;

				// 循环中，同时考虑以图 j 为参考图，被具体的图 k 观测到的像点
				if (k>j) // 只有上三角中才有 Ujk
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


	// 按列扫，计算所有的 Uj、eaj 和 Wij
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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
// 	// 按行扫，计算所有的 Vi 和 ebi
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
// 				// 如果 ptr == NULL，说明像点 xij 不存在
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

// 基于深度的 BA 方法中，所有像点坐标 xij （包括可调整的非参考像点以及不可调整的参考像点）对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void derivatives::j_f_w_t_w0_t0_d0_new2(const vector<double> & ds,			// 输入：n个空间点关联的深度值
									  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
									  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
									  const vector<Matx31d> & ts,				// 输入：m个图像平移向量
									  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
									  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
									  const vector<Point2d> & xys,				// 输入：l个观测像点坐标，最多为 m*n 个
									  const vector<Matx22d> & covInvs,			// 输入：l个观测像点坐标协方差矩阵的逆矩阵
									  const vector<Matx31d> & nxys,				// 输入：n个物点关联的于其参考图像系中的观测视线方向，也即归一化像点坐标
									  const vector<int> & ri_j,					// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
									  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
									  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点深度是固定的（i_fixed[i]=1），如果点 i 深度固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
									  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									  vector<Matx<double,6,6>> & U,				// 输出：总共(m+1)m/2个Ujk矩阵，仅跟图像参数有关
									  vector<Matx<double,1,1>> & V,				// 输出：n个Vi矩阵，仅跟深度有关
									  vector<Matx<double,6,1>> & W,				// 输出：l个Wij矩阵，同时跟深度及其观测图像有关
									  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
									  vector<Matx<double,1,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
									  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
									  Mat & g,									// 输出：6*m+n维的参数梯度
									  vector<double> & vds						// 输出：l个像点的重投影残差量
									  )
{
	int n = ds.size();	 // 空间点个数
	int m = Ks.size();	 // 图像个数
	int l = xys.size();	 // 所有观测像点的个数

	// 初始化全置 0
	U = vector<Matx<double,6,6>>((m+1)*m*0.5);
	V = vector<Matx<double,1,1>>(n);
	W = vector<Matx<double,6,1>>(l);
	ea = vector<Matx<double,6,1>>(m);
	eb = vector<Matx<double,1,1>>(n);

	// 遍历所有观测像点，生成所有的 Ujj,Ujk,Wij,Vi,eaj,ebi
	for (int i=0;i<n;++i)
	{
		int k = ri_j[i];		// 点 i 的参考图像

		double d0 = ds[i];		// 点 i 的深度
		Matx31d nxy0 = nxys[i];	// 点 i 关联的归一化像点坐标
		Matx33d R0 = Rs[k];		// 点 i 参考图像的旋转矩阵
		Matx31d t0 = ts[k];		// 点 i 参考图像的平移向量

		for (int j=0;j<m;++j)
		{
			if (j==k)
			{
				// 此时说明 j 是点 i 的参考图像，像点 xij 是肯定存在的，且为点 i 的参考像点
				// 参考像点 xij 不随任何参数改变，也不存在重投影残差，即 Aij=Bij=Cij=eij=0
				continue;
			}

			const int * ptr_ij = ptrMat.find<int>(i,j);

			if (NULL==ptr_ij)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			int idx_ij = (*ptr_ij);

			// 先计算生成 Aij,Bij,Cij,eij ////////////////////////////////////////////
			Matx33d K = Ks[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];
			int distType = distTypes[j];

			Point2d xy = xys[idx_ij];

			// 观测像点 xij 同时含有了点 i，成像图 j，以及参考图 k 的信息
			Matx<double,2,6> Aij;			// xij 对成像图 j 求导
			Matx<double,2,1> Bij;			// xij 对点 i 求导
			Matx<double,2,6> Cij;			// xij 对参考图 k 求导
			Matx22d cij = covInvs[idx_ij];	// xij 的权矩阵
			Matx21d eij;					// xij 的重投影残差

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
				// 如果图 j 参数固定，那么 Aij=0
				Aij = Matx<double,2,6>();
			}

			if (i_fixed[i])
			{
				// 如果点 i 固定，那么 Bij=0
				Bij = Matx<double,2,1>();
			}

			if (j_fixed[k])
			{
				// 如果点 i 参考图 k 的参数固定，那么 Cij=0
				Cij = Matx<double,2,6>();
			}
			//////////////////////////////////////////////////////////////////////////

			Matx<double,6,2> Aijtcij = Aij.t()*cij;
			Matx<double,1,2> Bijtcij = Bij.t()*cij;
			Matx<double,6,2> Cijtcij = Cij.t()*cij;

			U[j*m+j-0.5*j*(j+1)] += Aijtcij*Aij;		// 跟成像图 j 有关
			U[k*m+k-0.5*k*(k+1)] += Cijtcij*Cij;		// 跟参考图 k 有关

			if (j<k)
			{
				U[j*m+k-0.5*j*(j+1)] += Aijtcij*Cij;	// Ujk 的一项，同时和成像图 j 以及参考图 k 有关
			}

			if (k<j)
			{
				U[k*m+j-0.5*k*(k+1)] += Cijtcij*Aij;	// Ukj 的一项，同时和成像图 j 以及参考图 k 有关
			}

			W[idx_ij] += Aijtcij*Bij;					// Wij，同时和点 i 以及图 j 有关

			const int * ptr_ik = ptrMat.find<int>(i,k);	
			int idx_ik = (*ptr_ik);

			W[idx_ik] += Cijtcij*Bij;					// Wik，同时和点 i 以及参考图 k 有关

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

void derivatives::solveSBA_0_6_4(double u,								// 输入：阻尼系数
								 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								 const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
								 const vector<Matx<double,4,4>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
								 const vector<Matx<double,6,4>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
								 const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
								 const vector<Matx<double,4,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
								 vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
								 vector<Matx<double,4,1>> & db,			// 输出：n个空间点坐标的改正量
								 Mat & h								// 输出：6*m+4*n维的参数改正量
								 )
{
	int m = U.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数

	int M = 6*m; // 总的图像参数个数

	vector<Matx<double,6,6>> U_aug(m); // 存放 Uj' 矩阵
	vector<Matx<double,4,4>> V_aug(n); // 存放 inv(Vi') 矩阵
	vector<Matx<double,6,4>> Y(l); // 存放所有的 Yij = Wij * inv(Vi') 矩阵

	// 形成增广矩阵 Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	// 形成增广矩阵的逆 inv(Vi') = (Vi + uI)^(-1)
	for (int i=0;i<n;++i)
	{
		Matx<double,4,4> Vi_aug = V[i];

		for (int k=0;k<4;++k)
		{
			Vi_aug(k,k)+=u;
		}

		V_aug[i] = Vi_aug.inv(DECOMP_CHOLESKY);
	}
	
	// 形成所有的 Yij = Wij * inv(Vi') 矩阵
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Y[*ptr] = W[*ptr] * V_aug[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // 所有图像参数的改正量

	// 形成所有的 Sjk 矩阵
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
// 					// 必须是 i 点同时被图像 j 和 k 观测到
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
// 			// 填充大的 S 矩阵
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += U_aug[j];

				// 填充大的 S 矩阵
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// 形成所有的 ej 残差量
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

	// 解方程 S*ha = e 得到所有图像参数的改正量
	solve(S, e, ha, DECOMP_CHOLESKY);

	int m6 = m*6;

	for (int ii=0;ii<m6;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// 录入到输出中
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

	// 再进一步求解各空间点坐标的改正量
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

void derivatives::solveSBA_0_6_3(double u,								// 输入：阻尼系数
							     const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
							     const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
							     const vector<Matx<double,3,3>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
							     const vector<Matx<double,6,3>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
							     const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
							     const vector<Matx<double,3,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
							     vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
							     vector<Matx<double,3,1>> & db,			// 输出：n个空间点坐标的改正量
							     Mat & h								// 输出：6*m+3*n维的参数改正量
							     )
{
	int m = U.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数

	int M = 6*m; // 总的图像参数个数

	vector<Matx<double,6,6>> U_aug(m); // 存放 Uj' 矩阵
	vector<Matx<double,3,3>> V_aug(n); // 存放 inv(Vi') 矩阵
	vector<Matx<double,6,3>> Y(l); // 存放所有的 Yij = Wij * inv(Vi') 矩阵

	// 形成增广矩阵 Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	// 形成增广矩阵的逆 inv(Vi') = (Vi + uI)^(-1)
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi_aug = V[i];

		for (int k=0;k<3;++k)
		{
			Vi_aug(k,k)+=u;
		}

		V_aug[i] = Vi_aug.inv(DECOMP_CHOLESKY);
	}
	
	// 形成所有的 Yij = Wij * inv(Vi') 矩阵
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Y[*ptr] = W[*ptr] * V_aug[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // 所有图像参数的改正量

	// 形成所有的 Sjk 矩阵
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
// 					// 必须是 i 点同时被图像 j 和 k 观测到
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
// 			// 填充大的 S 矩阵
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += U_aug[j];

				// 填充大的 S 矩阵
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// 形成所有的 ej 残差量
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

	// 解方程 S*ha = e 得到所有图像参数的改正量
	solve(S, e, ha, DECOMP_CHOLESKY);

	int m6 = m*6;

	for (int ii=0;ii<m6;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// 录入到输出中
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

	// 再进一步求解各空间点坐标的改正量
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

void derivatives::solveSBA_1_6_4(double u,								// 输入：阻尼系数
								 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								 const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
								 const vector<Matx<double,4,4>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
								 const vector<Matx<double,6,4>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
								 const Matx<double,1,1> & Q,			// 输入：唯一一个Q矩阵，只跟共参数有关
								 const vector<Matx<double,1,6>> & G,	// 输入：m个Gj矩阵，同时和独有及共有图像参数有关
								 const vector<Matx<double,1,4>> & H,	// 输入：n个Hi矩阵，同时和共有图像参数以及物点有关
								 const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
								 const vector<Matx<double,4,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
								 const Matx<double,1,1> & ec,			// 输入：唯一一个ec残差向量，仅跟共有图像参数有关
								 vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
							 	 vector<Matx<double,4,1>> & db,			// 输出：n个空间点坐标的改正量
								 Matx<double,1,1> & dc,					// 输出：共有图像参数的改正量
								 Mat & h								// 输出：1+6*m+4*n维的参数改正量
								 )
{
	int m = U.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数

	int M = 6*m+1; // 总的图像参数个数，包括了各图像独有参数以及共有参数

	vector<Matx<double,6,6>> U_aug(m); // 存放 Uj' 矩阵
	vector<Matx<double,4,4>> V_aug(n); // 存放 inv(Vi') 矩阵
	vector<Matx<double,6,4>> Y(l); // 存放所有的 Yij = Wij * inv(Vi') 矩阵
	Matx<double,1,1> Q_aug = Q; // 经过阻尼系数增广的共参数的增广 Q 矩阵
	vector<Matx<double,1,4>> HV(n); // 存放 Hi * inv(Vi') 矩阵

	// 形成增广矩阵 Q' = Q + uI
	Q_aug(0)+=u;

	// 形成增广矩阵 Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	Matx<double,1,1> Q_HVH = Q_aug; // 正规方程的左上角部分 Q' - H*inv(V')*Ht
	vector<Matx<double,1,6>> G_HVW(m); // 正规方程的右上角部分 G - H*inv(V')*Wt
	Matx<double,1,1> ec_HVeb = ec; // ec - H*inv(V')*eb

	// 形成增广矩阵的逆 inv(Vi') = (Vi + uI)^(-1)
	// 以及各个 Hi*inv(Vi')
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
	
	// 形成所有的 Yij = Wij * inv(Vi') 矩阵
	for (int j=0;j<m;++j)
	{
		Matx<double,1,6> G_HVWj = G[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
	Mat ha; // 所有图像参数的改正量

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += U_aug[j];

				// 填充大的 S 矩阵
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j_start+ii,k_start+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j_start+ii,k_start+jj) = S.at<double>(k_start+jj,j_start+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// 形成所有的 ej 残差量
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

	// 解方程 S*ha = e 得到所有图像参数的改正量
	solve(S, e, ha, DECOMP_CHOLESKY);

	for (int ii=0;ii<M;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// 录入到输出中
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

	// 再进一步求解各空间点坐标的改正量
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

void derivatives::solveSBA_1_6_4_Daj(double u,								// 输入：阻尼系数
									 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									 const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
									 const vector<Matx<double,4,4>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
									 const vector<Matx<double,6,4>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
									 const Matx<double,1,1> & Q,			// 输入：唯一一个Q矩阵，只跟共参数有关
									 const vector<Matx<double,1,6>> & G,	// 输入：m个Gj矩阵，同时和独有及共有图像参数有关
									 const vector<Matx<double,1,4>> & H,	// 输入：n个Hi矩阵，同时和共有图像参数以及物点有关
									 const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
									 const vector<Matx<double,4,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
									 const Matx<double,1,1> & ec,			// 输入：唯一一个ec残差向量，仅跟共有图像参数有关
									 int j_constrained,						// 输入：被约束的图像的索引
									 const vector<Matx<double,1,6>> & Daj,	// 输入：每个约束方程对第 j 个图像独有参数的导数
									 const vector<double> & hs,				// 输入：每个约束方程在当前估计下的值
									 vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
									 vector<Matx<double,4,1>> & db,			// 输出：n个空间点坐标的改正量
									 Matx<double,1,1> & dc,					// 输出：共有图像参数的改正量
									 Mat & h,								// 输出：1+6*m+4*n维的参数改正量
									 double & val							// 输出：0.5*dx'C'lamda
									 )
{
	int m = U.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数
	int nc = hs.size(); // 约束方程的个数

	int M = 6*m+1; // 总的图像参数个数，包括了各图像独有参数以及共有参数

	vector<Matx<double,6,6>> U_aug(m); // 存放 Uj' 矩阵
	vector<Matx<double,4,4>> V_aug(n); // 存放 inv(Vi') 矩阵
	vector<Matx<double,6,4>> Y(l); // 存放所有的 Yij = Wij * inv(Vi') 矩阵
	Matx<double,1,1> Q_aug = Q; // 经过阻尼系数增广的共参数的增广 Q 矩阵
	vector<Matx<double,1,4>> HV(n); // 存放 Hi * inv(Vi') 矩阵

	// 形成增广矩阵 Q' = Q + uI
	Q_aug(0)+=u;

	// 形成增广矩阵 Uj' = Uj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		U_aug[j] = Uj_aug;
	}

	Matx<double,1,1> Q_HVH = Q_aug; // 正规方程的左上角部分 Q' - H*inv(V')*Ht
	vector<Matx<double,1,6>> G_HVW(m); // 正规方程的右上角部分 G - H*inv(V')*Wt
	Matx<double,1,1> ec_HVeb = ec; // ec - H*inv(V')*eb

	// 形成增广矩阵的逆 inv(Vi') = (Vi + uI)^(-1)
	// 以及各个 Hi*inv(Vi')
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

	// 形成所有的 Yij = Wij * inv(Vi') 矩阵
	for (int j=0;j<m;++j)
	{
		Matx<double,1,6> G_HVWj = G[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
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
	Mat ha; // 所有图像参数的改正量

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += U_aug[j];

				// 填充大的 S 矩阵
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j_start+ii,k_start+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j_start+ii,k_start+jj) = S.at<double>(k_start+jj,j_start+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// 先求解 lagrange multipliers ////////////////////////////////////////////////////////////////////////////
	int idx_tmp = 1+j_constrained*6;

	S = S.inv(DECOMP_CHOLESKY); // 此时 S = inv(S)

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
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Kajbi -= Kajaks[k]*Y[*ptr];
		}

		Kajbis[i] = Kajbi;
	}

	// 生成求解 lagrange multipliers 的方程组的系数阵，以及向量项
	Mat A(nc, nc, CV_64FC1, Scalar(0));
	Mat b(nc, 1, CV_64FC1, Scalar(0));
	Mat lmd; // 求解的 lagrange multipliers

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
	
	// 解方程 A*lmd = b 得到 lagrange multipliers
	solve(A, b, lmd, DECOMP_CHOLESKY);

	vector<double> lmds(nc);

	for (int i=0;i<nc;++i)
	{
		lmds[i] = lmd.at<double>(i);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 形成所有的 ej 残差量
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

		// 约束添加项 ///////////////////////////
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

	// 解方程 S*ha = e 得到所有图像参数的改正量
	ha = S*e;

	for (int ii=0;ii<M;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// 录入到输出中
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

	// 再进一步求解各空间点坐标的改正量
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

	// 计算 0.5*dx'C'lamda
	Matx<double,6,1> vTmp;
	for (int k=0;k<nc;++k)
	{
		vTmp += Daj[k].t()*lmds[k];
	}
	Matx<double,1,1> vVal = 0.5*da[j_constrained].t()*vTmp;

	val = vVal(0);
}

// 20150721,物点是由一维的深度来表示
void derivatives::solveSBA_0_6_1(double u,								// 输入：阻尼系数
								 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引
								 const vector<Matx<double,6,6>> & U,	// 输入：总共(m+1)m/2个 Ujk 矩阵，仅跟图像参数有关
								 const vector<Matx<double,1,1>> & V,	// 输入：n个Vi矩阵，仅跟空间点深度有关
								 const vector<Matx<double,6,1>> & W,	// 输入：l个Wij矩阵，同时跟空间点深度及其观测图像有关
								 const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
								 const vector<Matx<double,1,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点深度有关
								 vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
								 vector<Matx<double,1,1>> & db,			// 输出：n个空间点深度的改正量
								 Mat & h								// 输出：6*m+n维的参数改正量
								 )
{
	int m = ea.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数

	int M = 6*m; // 总的图像参数个数

	vector<Matx<double,6,6>> Ujj_aug(m);// 存放 Ujj' 矩阵
	vector<Matx<double,1,1>> V_aug(n);	// 存放 inv(Vi') 矩阵
	vector<Matx<double,6,1>> Y(l);		// 存放所有的 Yij = Wij * inv(Vi') 矩阵

	// 形成增广矩阵 Ujj' = Ujj + uI
	for (int j=0;j<m;++j)
	{
		Matx<double,6,6> Uj_aug = U[j*m+j-0.5*j*(j+1)];

		for (int k=0;k<6;++k)
		{
			Uj_aug(k,k)+=u;
		}

		Ujj_aug[j] = Uj_aug;
	}

	// 形成增广矩阵的逆 inv(Vi') = (Vi + uI)^(-1)
	for (int i=0;i<n;++i)
	{
		double Vi = V[i](0);

		V_aug[i](0) = 1/(Vi+u);
	}
	
	// 形成所有的 Yij = Wij * inv(Vi') 矩阵
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			int idx_ij = (*ptr);

			Y[idx_ij] = W[idx_ij] * V_aug[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0));
	Mat e(M, 1, CV_64FC1, Scalar(0));
	Mat ha; // 所有图像参数的改正量

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += Ujj_aug[j];

				// 填充大的 S 矩阵
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

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	// 形成所有的 ej 残差量
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

	// 解方程 S*ha = e 得到所有图像参数的改正量
	solve(S, e, ha, DECOMP_CHOLESKY);

	int m6 = m*6;

	for (int ii=0;ii<m6;++ii)
	{
		h.at<double>(ii) = ha.at<double>(ii);
	}

	// 录入到输出中
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

	// 再进一步求解各空间点深度的改正量
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

// void derivatives::covarianceSBA_0_6_4(const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
// 									  const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
// 									  const vector<Matx<double,4,4>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
// 									  const vector<Matx<double,6,4>> & W	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
// 									  )
// {
// 	int m = U.size(); // 图像个数
// 	int n = V.size(); // 空间点个数
// 	int l = W.size(); // 所有像点个数
// 
// 	int M = 6*m; // 总的图像参数个数
// 
// 	vector<Matx<double,4,4>> V_inv(n); // 存放 inv(Vi) 矩阵
// 	vector<Matx<double,6,4>> Y(l); // 存放所有的 Yij = Wij * inv(Vi) 矩阵
// 
// 	// 形成逆矩阵 inv(Vi)
// 	for (int i=0;i<n;++i)
// 	{
// 		V_inv[i] = V[i].inv(DECOMP_CHOLESKY);
// 	}
// 
// 	// 形成所有的 Yij = Wij * inv(Vi) 矩阵
// 	for (int i=0;i<n;++i)
// 	{
// 		for (int j=0;j<m;++j)
// 		{
// 			const int * ptr = ptrMat.find<int>(i,j);
// 
// 			if (NULL == ptr)
// 			{
// 				// 如果 ptr == NULL，说明像点 xij 不存在
// 				continue;
// 			}
// 
// 			Y[*ptr] = W[*ptr] * V_inv[i];
// 		}
// 	}
// 
// 	Mat S(M, M, CV_64FC1, Scalar(0)); // U-W*inv(V)*W'
// 
// 	// 形成所有的 Sjk 矩阵
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
// // 					// 必须是 i 点同时被图像 j 和 k 观测到
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
// // 			// 填充大的 S 矩阵
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
// 	// 形成所有的 Sjk 矩阵
// 	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
// 	// 能提速大概 35% 的样子
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
// 					// 必须是 i 点同时被图像 j 和 k 观测到
// 					continue;
// 				}
// 
// 				Sjk -= Y[*ptr1] * W[*ptr2].t();
// 			}
// 
// 			if (j==k) // 对角线上
// 			{
// 				Sjk += U[j];
// 
// 				// 填充大的 S 矩阵
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
// 			// 填充大的 S 矩阵，上三角元素
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
// 	Mat cov_a = S.inv(DECOMP_SVD); // 像机参数的协方差矩阵即为矩阵 U-W*inv(V)*W' 的伪逆
// }

void derivatives::covarianceSBA_0_6_3(const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									  const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
									  const vector<Matx<double,3,3>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
									  const vector<Matx<double,6,3>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
									  vector<Matx<double,6,6>> & cov_a,		// 输出：m个图像独有参数的协方差矩阵
									  vector<Matx<double,3,3>> & cov_b		// 输出：n个空间点的坐标的协方差矩阵
									  )
{
	cov_a.clear();
	cov_b.clear();

	int m = U.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数

	int M = 6*m; // 总的图像参数个数

	vector<Matx<double,3,3>> V_inv(n); // 存放 inv(Vi) 矩阵
	vector<Matx<double,6,3>> Y(l); // 存放所有的 Yij = Wij * inv(Vi) 矩阵

	// 形成逆矩阵 inv(Vi)
	for (int i=0;i<n;++i)
	{
		V_inv[i] = V[i].inv(DECOMP_CHOLESKY);
	}
	
	// 形成所有的 Yij = Wij * inv(Vi) 矩阵
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Y[*ptr] = W[*ptr] * V_inv[i];
		}
	}

	Mat S(M, M, CV_64FC1, Scalar(0)); // U-W*inv(V)*W'

	// 形成所有的 Sjk 矩阵
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
// 					// 必须是 i 点同时被图像 j 和 k 观测到
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
// 			// 填充大的 S 矩阵
// 			for (int ii=0;ii<6;++ii)
// 			{
// 				for (int jj=0;jj<6;++jj)
// 				{
// 					S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
// 				}
// 			}
// 		}
// 	}

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += U[j];

				// 填充大的 S 矩阵
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j6+ii,k6+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j6+ii,k6+jj) = S.at<double>(k6+jj,j6+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	Mat mcov_a = S.inv(DECOMP_SVD); // 像机参数的协方差矩阵即为矩阵 U-W*inv(V)*W' 的伪逆

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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::covarianceSBA_1_6_3(const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									  const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
									  const vector<Matx<double,3,3>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
									  const vector<Matx<double,6,3>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
									  const Matx<double,1,1> & Q,			// 输入：唯一一个Q矩阵，只跟共参数有关
									  const vector<Matx<double,1,6>> & G,	// 输入：m个Gj矩阵，同时和独有及共有图像参数有关
									  const vector<Matx<double,1,3>> & H,	// 输入：n个Hi矩阵，同时和共有图像参数以及物点有关
									  vector<Matx<double,6,6>> & cov_a,		// 输出：m个图像独有参数的协方差阵
									  vector<Matx<double,3,3>> & cov_b,		// 输出：n个空间点的坐标的协方差阵
									  Matx<double,1,1> & cov_c,				// 输出：共参数的协方差阵
									  vector<Matx<double,1,6>> & cov_ca		// 输出：图像共参数和各图像独有参数之间的协方差阵
									  )
{
	cov_a.clear();
	cov_b.clear();
	cov_ca.clear();

	int m = U.size(); // 图像个数
	int n = V.size(); // 空间点个数
	int l = W.size(); // 所有像点个数

	int M = 6*m+1; // 总的图像参数个数，包括了各图像独有参数以及共有参数

	vector<Matx<double,3,3>> V_inv(n); // 存放 inv(Vi) 矩阵
	vector<Matx<double,6,3>> Y(l); // 存放所有的 Yij = Wij * inv(Vi) 矩阵
	vector<Matx<double,1,3>> HV(n); // 存放 Hi * inv(Vi) 矩阵

	Matx<double,1,1> Q_HVH = Q; // 正规方程的左上角部分 Q - H*inv(V)*Ht
	vector<Matx<double,1,6>> G_HVW(m); // 正规方程的右上角部分 G - H*inv(V)*Wt

	// 形成逆 inv(Vi)
	// 以及各个 Hi*inv(Vi)
	for (int i=0;i<n;++i)
	{
		Matx<double,3,3> Vi_inv = V[i].inv(DECOMP_CHOLESKY);

		V_inv[i] = Vi_inv;

		Matx<double,1,3> Hi = H[i];

		Matx<double,1,3> HiVi = Hi * Vi_inv; // Hi * inv(Vi)

		HV[i] = HiVi;

		Q_HVH -= HiVi*Hi.t();
	}

	// 形成所有的 Yij = Wij * inv(Vi) 矩阵
	for (int j=0;j<m;++j)
	{
		Matx<double,1,6> G_HVWj = G[j];

		for (int i=0;i<n;++i)
		{
			const int * ptr = ptrMat.find<int>(i,j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			Matx<double,6,3> Wij = W[*ptr];

			Y[*ptr] = Wij * V_inv[i];
			G_HVWj -= HV[i]*Wij.t();
		}

		G_HVW[j] = G_HVWj;
	}

	Mat S(M, M, CV_64FC1, Scalar(0));

	// 形成所有的 Sjk 矩阵
	// 由于 S 矩阵是对称阵，所以只生成上三角和对角线元素即可，下三角元素直接转置过去即可。
	// 能提速大概 35% 的样子
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
					// 必须是 i 点同时被图像 j 和 k 观测到
					continue;
				}

				Sjk -= Y[*ptr1] * W[*ptr2].t();
			}

			if (j==k) // 对角线上
			{
				Sjk += U[j];

				// 填充大的 S 矩阵
				for (int ii=0;ii<6;++ii)
				{
					for (int jj=0;jj<6;++jj)
					{
						S.at<double>(j_start+ii,k_start+jj) = Sjk(ii,jj);
					}
				}

				continue;
			}

			// 填充大的 S 矩阵，上三角元素
			for (int ii=0;ii<6;++ii)
			{
				for (int jj=0;jj<6;++jj)
				{
					S.at<double>(j_start+ii,k_start+jj) = S.at<double>(k_start+jj,j_start+ii) = Sjk(ii,jj);
				}
			}
		}
	}

	Mat mcov_d = S.inv(DECOMP_SVD); // 像机独有参数以及共有参数的协方差阵

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
				// 如果 ptr == NULL，说明像点 xij 不存在
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

void derivatives::covarianceSBA_wj_cj_XiYiZi(const vector<Point3d> & XYZs,			// 输入：n个空间点坐标
											 const vector<Matx33d> & Ks,			// 输入：m个图像内参数矩阵
											 const vector<Matx33d> & Rs,			// 输入：m个图像旋转矩阵
											 const vector<Matx31d> & ts,			// 输入：m个图像平移向量
											 const vector<Matx<double,5,1>> & dists,// 输入：m个图像像差系数
											 const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
											 const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
											 const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
											 const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
											 const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
											 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
											 vector<Matx<double,6,6>> & cov_a,		// 输出：m个图像独有参数的协方差阵
											 vector<Matx<double,3,3>> & cov_b		// 输出：n个空间点的坐标的协方差阵
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

void derivatives::covarianceSBA_f_wj_cj_XiYiZi(const vector<Point3d> & XYZs,			// 输入：n个空间点坐标
											   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
											   const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
											   const vector<Matx31d> & ts,				// 输入：m个图像平移向量
											   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
											   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
											   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
											   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
											   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
											   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
											   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
											   vector<Matx<double,6,6>> & cov_a,		// 输出：m个图像独有参数的协方差阵
											   vector<Matx<double,3,3>> & cov_b,		// 输出：n个空间点的坐标的协方差阵
											   Matx<double,1,1> & cov_c,				// 输出：共参数的协方差阵
											   vector<Matx<double,1,6>> & cov_ca		// 输出：图像共参数和各图像独有参数之间的协方差阵
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

// Matx<double,2,16> derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(double X, double Y, double Z,	// 该控制点的坐标
// 																  double x, double y,			// 该控制点的实际观测像点坐标
// 																  const Matx33d & mK,			// 当前的内参数估计
// 																  const Matx33d & mR,			// 当前的姿态估计
// 																  const Matx31d & mt,			// 当前的平移向量估计
// 																  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
// 																  double & dx, double & dy		// 当前估计下的重投影残差
// 																  )
// {
// 	Matx31d mX;
// 	mX(0)=X; mX(1)=Y; mX(2)=Z;
// 
// 	Matx31d mx = mR*mX+mt;
// 	double mx2_1 = 1.0/mx(2);
// 
// 	// 首先得到归一化像点坐标
// 	mx(0)*=mx2_1;
// 	mx(1)*=mx2_1;
// 	mx(2)=1;
// 
// 	double u = mx(0);
// 	double v = mx(1);
// 
// 	// 再通过内参数得到线性成像点坐标
// 	mx = mK*mx;
// 	double x0 = mx(0);
// 	double y0 = mx(1);
// 
// 	double fx = mK(0,0);	double fy = mK(1,1);
// 	double cx = mK(0,2);	double cy = mK(1,2);
// 	double s  = mK(0,1);	
// 
// 	// 计算畸变量
// 	double ddx,ddy;
// 	distortions::dxdy_brown(fx,fy,s,u,v,k1,k2,k3,k4,k5,ddx,ddy);
// 
// 	dx = x0+ddx-x;
// 	dy = y0+ddy-y;
// 
// 
// 	// 对内参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
// 	Matx<double,2,5> j_xy_fxfycxcys = der_xy_fxfycxcys(u,v); // dx/da
// 
// 	Matx<double,2,5> j_dxdy_fxfycxcys = der_dxdy_fxfycxcys_brown(u,v,k1,k2,k3,k4,k5); // ddx/da
// 
// 	Matx<double,2,5> j_f_fxfycxcys = j_xy_fxfycxcys + j_dxdy_fxfycxcys; // df/da = dx/da + ddx/da
// 	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 	// 对外参数求导 ///////////////////////////////////////////////////////////////////////////////////////////////////
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
// 	// 对像差系数求导 /////////////////////////////////////////////////////////////////////////////////////////////////
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

void derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
											   const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
											   const Matx33d & mK,									// 当前的内参数估计
											   const Matx33d & mR,									// 当前的姿态估计
											   const Matx31d & mt,									// 当前的平移向量估计
											   double k1,double k2,double k3,double k4,double k5,	// 当前的像差系数估计
											   int distType,										// 像差类型，0 weng's, 1 brown's
											   Mat & J,												// 输出：	Jacobian矩阵
											   Mat & f,												// 输出：	评估的目标函数值
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

void derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
											  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
											  const Matx33d & mK,									// 当前的内参数估计
											  const Matx33d & mR,									// 当前的姿态估计
											  const Matx31d & mt,									// 当前的平移向量估计
											  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
											  int distType,											// 像差类型，0 weng's, 1 brown's
											  Mat & J,												// 输出：	Jacobian矩阵
											  Mat & f,												// 输出：	评估的目标函数值
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

void derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
										   const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
										   const Matx33d & mK,									// 当前的内参数估计
										   const Matx33d & mR,									// 当前的姿态估计
										   const Matx31d & mt,									// 当前的平移向量估计
										   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
										   int distType,										// 像差类型，0 weng's, 1 brown's
										   Mat & J,												// 输出：	Jacobian矩阵
										   Mat & f,												// 输出：	评估的目标函数值
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

void derivatives::j_f_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
									   const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
									   const Matx33d & mK,									// 当前的内参数估计
									   const Matx33d & mR,									// 当前的姿态估计
									   const Matx31d & mt,									// 当前的平移向量估计
									   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
									   int distType,										// 像差类型，0 weng's, 1 brown's
									   Mat & J,												// 输出：	Jacobian矩阵
									   Mat & f,												// 输出：	评估的目标函数值
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

void derivatives::j_f_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
									  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
									  const Matx33d & mK,									// 当前的内参数估计
									  const Matx33d & mR,									// 当前的姿态估计
									  const Matx31d & mt,									// 当前的平移向量估计
									  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
									  int distType,											// 像差类型，0 weng's, 1 brown's
									  Mat & J,												// 输出：	Jacobian矩阵
									  Mat & f,												// 输出：	评估的目标函数值
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

void derivatives::j_f_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
								   const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
								   const Matx33d & mK,									// 当前的内参数估计
								   const Matx33d & mR,									// 当前的姿态估计
								   const Matx31d & mt,									// 当前的平移向量估计
								   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
								   int distType,										// 像差类型，0 weng's, 1 brown's
								   Mat & J,												// 输出：	Jacobian矩阵
								   Mat & f,												// 输出：	评估的目标函数值
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

void derivatives::j_f_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,					// 输入：		控制点坐标
									    const vector<Point2d> & vImgPts,					// 输入：		控制点观测像点坐标
									    const Matx33d & mK,									// 当前的内参数估计
									    const Matx33d & mR,									// 当前的姿态估计
									    const Matx31d & mt,									// 当前的平移向量估计
									    double k1,double k2,double k3, double k4, double k5,// 当前的像差系数估计
									    int distType,										// 像差类型，0 weng's, 1 brown's
									    Mat & J,											// 输出：	Jacobian矩阵
									    Mat & f,											// 输出：	评估的目标函数值
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

void derivatives::j_f_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
									 const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
									 const Matx33d & mK,									// 当前的内参数估计
									 const Matx33d & mR,									// 当前的姿态估计
									 const Matx31d & mt,									// 当前的平移向量估计
									 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
									 int distType,											// 像差类型，0 weng's, 1 brown's
									 Mat & J,												// 输出：	Jacobian矩阵
									 Mat & f,												// 输出：	评估的目标函数值
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

// void derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
// 												   const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
// 												   const Matx33d & mK,									// 当前的内参数估计
// 												   const Matx33d & mR,									// 当前的姿态估计
// 												   const Matx31d & mt,									// 当前的平移向量估计
// 												   double k1,double k2,double k3,double k4,double k5,	// 当前的像差系数估计
// 												   Mat & J,												// 输出：	Jacobian矩阵
// 												   Mat & f,												// 输出：	评估的目标函数值
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

// void derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
// 													 const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
// 													 const Matx33d & mK,									// 当前的内参数估计
// 													 const Matx33d & mR,									// 当前的姿态估计
// 													 const Matx31d & mt,									// 当前的平移向量估计
// 													 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
// 													 Mat & J,												// 输出：	Jacobian矩阵
// 													 Mat & f,												// 输出：	评估的目标函数值
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

// 计算归一化像点畸变量
void distortions::dudv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv)
{
	double u2 = u*u;
	double v2 = v*v;
	double uv = u*v;

	double r2 = u2 + v2;

	du = k1*r2*u + k2*r2 + k4*u2 + k5*uv; // k_1 r^2 u+k_2 r^2+k_4 u^2+k_5 uv
	dv = k1*r2*v + k3*r2 + k4*uv + k5*v2; // k_1 r^2 v+k_3 r^2+k_4 uv+k_5 v^2
}

// 计算归一化像点畸变量
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

// 计算像点畸变量
void distortions::dxdy_weng(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy)
{
	double du,dv;

	dudv_weng(u,v,k1,k2,k3,k4,k5,du,dv);

	dx = fx*du + s*dv; //dx = f_x du + s dv
	dy = fy*dv; // dy = f_y dv
}

// 计算像点畸变量
// examined on 2015.04.22
void distortions::dxdy_brown(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy)
{
	double du,dv;

	dudv_brown(u,v,k1,k2,k3,k4,k5,du,dv);

	dx = fx*du + s*dv; //dx = f_x du + s dv
	dy = fy*dv; // dy = f_y dv
}

// 对带像差像点坐标做去像差处理
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

// 对带像差像点坐标做去像差处理
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

// 方法基于《Matrix Computations 3rd Edition》一书P.216中的Algorithm 5.1.3 (Computes Givens c and s)
// 根据输入的[a; b]'计算使得[c s; -s c]' * [a; b]' = [r; 0]'成立的cos和sin
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

// 对m×n的矩阵A进行RQ分解（要求m<=n）：A = RQ，其中Q为n×n的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
// 利用的是Givens旋转方法实现的，至于要求m<=n的原因在笔记本内有详细解释
void calib::RQ_Givens(const Mat & A, Mat & R, Mat & Q)
{
	if (A.rows > A.cols)
	{
		AfxMessageBox("进行RQ分解的矩阵的行数必须小于等于列数！");

		return;
	}

	Mat A_cpy = A.clone();

	int m = A.rows;

	Matx22d G;
	Matx41d CS;
	vector<Matx41d> vCSs; // mCS记录每次计算出来的cos、sin、i、k以及是否换符号
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

	BOOL lastNegative = FALSE; // 上三角矩阵第1行第1列的元素是否为负，默认是否

	if (A_cpy.at<double>(0,0) < 0)
	{
		lastNegative = TRUE;

		A_cpy.at<double>(0,0) = - A_cpy.at<double>(0,0); // 若为负，则反号变成正数，并置标志位
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

// 对3×3的矩阵实施RQ分解
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

// 将旋转向量转换为旋转矩阵
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
		radian = 0; // 要是弧度值过小那么直接将其置为 0，以保证后续形成的旋转矩阵肯定为单位阵
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

// 给定四元数，生成旋转矩阵
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

// 根据输入的旋转矩阵，返回一个 3 维的向量，其为 θ|v|，其中 v 为旋转轴的单位向量，θ 为旋转弧度
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

// 20150421，把所有空间点平移至质心坐标系，且所有点到质心的平均距离为3^0.5
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

// 20150421，把所有空间点平移至质心坐标系，且所有点到质心的平均距离为3^0.5
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

// 返回输出K矩阵的逆矩阵
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

// wx=PX,其中P为3×4的矩阵，X为4×1的向量
Matx34d calib::dlt_34(const Mat & mX,	// 输入：	4×n 的矩阵
					  const Mat & mx	// 输入：	2×n 的矩阵
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

// wx=HX,其中H为3×3的矩阵，X为3×1的向量
Matx33d calib::dlt_33(const Mat & mX,	// 输入：	3×n 的矩阵
					  const Mat & mx	// 输入：	2×n 的矩阵
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

// 从无穷单应的线性解aH中分解出像机内参和旋转矩阵，并返回尺度因子a
double calib::decomp_KR(const Matx33d & mKR,	// 输入：	3×3 的aKR矩阵，也就是无穷单应
						Matx33d & mK,			// 输出：	3×3 的像机内参矩阵
						Matx33d & mR			// 输出：	3×3 的像机旋转矩阵
						)
{
	// RQ分解
	// 发现OpenCV的RQ分解函数并不保证所有对角线元素都为正，经常会出现K矩阵的第3列全为负数，看来它最后收尾那一下没有判断最后
	// 残余的那个 K(3,3)是否为负数了，倒是也可以用，那么正确的K将为 K=KQ，R为 R=QR，其中Q = [1 0 0; 0 1 0; 0 0 -1]，
	// 也就是把K的第3列和R的第3行都反向即可。
//	RQDecomp3x3(mKR, mK, mR);
	// 20150514，改为采用自己编写的RQ分解函数
	RQ3x3_Givens(mKR, mK, mR);

	// 确定尺度因子的模|a|
	double a = mK(2,2);
	double a_1=1.0/a;

	mK=a_1*mK;

	mK(2,2) = 1; // 确保内参矩阵的第3行第3列为1.0，而不是什么0.9999999999989，看着就难受

	double detR = determinant(mR);

	if (detR<0)
	{
		mR=-mR;
		a=-a;
	}

	return a;
}

// 从投影矩阵的线性解aP中分解出像机内外参数
void calib::decomp_P(const Matx34d & mP,// 输入：	3×4 的投影矩阵aP
				     Matx33d & mK,		// 输出：	3×3 的像机内参矩阵
				     Matx33d & mR,		// 输出：	3×3 的像机旋转矩阵
				     Matx31d & mt		// 输出：	3×1 的像机平移向量
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

// 从线性解a[R|t]中分解出像机外参数
void calib::decomp_R_t(const Matx34d & mR_t,// 输入：	3×4 的a[R|t]
					   Matx33d & mR,		// 输出：	3×3 的像机旋转矩阵
					   Matx31d & mt			// 输出：	3×1 的像机平移向量
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

// 从线性解aK[r1 r2 t]中分解出像机外参数
void calib::decomp_H(const Matx33d & mH,// 输入：	3×3 的aK[r1 r2 t]
				     const Matx33d & mK,// 输入：	已知的 3×3 的内参矩阵
				     Matx33d & mR,		// 输出：	3×3 的像机旋转矩阵
				     Matx31d & mt		// 输出：	3×1 的像机平移向量
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

	Matx31d r3 = cross_mat(r1)*r2; // r3=r1×r2
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

// 利用6个以上控制点线性标定像机投影矩阵
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

// 利用4以上控制点和光心位置线性标定像机的无穷单应H=KR
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

void optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
										      const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
										      Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
										      Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
										      Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
										      double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
										      int distType /*= 0*/,				// 输入：	0 weng's, 1 brown's
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 16; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
											 const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
											 Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
											 Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
											 Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
											 double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
											 int distType /*= 0*/,				// 输入：	0 weng's, 1 brown's
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 15; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcy_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
									      const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
									      Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
									      Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
									      Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
									      double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
									      int distType /*= 0*/,				// 输入：	0 weng's, 1 brown's
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
	mK(0,0) = mK(1,1) = f_mean; // 确保 fx=fy

	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 14; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fcxcy_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
								      const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
									  Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
									  Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
									  Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
									  double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
									  int distType /*= 0*/,				// 输入：	0 weng's, 1 brown's
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
	mK(0,0) = mK(1,1) = f_mean; // 确保 fx=fy

	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 12; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_f_w_t_k1k2k3k4k5(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
								     const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
								     Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
								     Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
								     Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
								     double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
								     int distType /*= 0*/,				// 输入：	0 weng's, 1 brown's
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 11; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcy_w_t_k1(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
							      const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
								  Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
								  Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
								  Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
								  double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
								  int distType /*= 0*/,				// 输入：	0 weng's, 1 brown's
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
	mK(0,0) = mK(1,1) = f_mean; // 确保 fx=fy

	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 10; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fcxcy_w_t_k1(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,// 输入：		控制点坐标
									   const vector<Point2d> & vImgPts,// 输入：		控制点观测像点坐标
									   Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
									   Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
									   Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
									   double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
									   int distType /*= 0*/,			// 输入：	0 weng's, 1 brown's
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 12; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fxfycxcy_w_t_k1k2(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void optim::optim_lm_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,// 输入：		控制点坐标
								    const vector<Point2d> & vImgPts,// 输入：		控制点观测像点坐标
								    Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
								    Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
								    Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
								    double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
								    int distType /*= 0*/,			// 输入：	0 weng's, 1 brown's
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
	mK(0,0) = mK(1,1) = f_mean; // 确保 fx=fy

	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int nPts = vWrdPts.size(); // number of control points
	int N = 11; // number of variables to be optimized
	int M = 2*nPts;	// 每个控制点提供 2 个观测方程

	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量

	vector<double> vds(nPts);

	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
	derivatives::j_f_fcxcy_w_t_k1k2(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],distType,J,f,vds);
	A = J.t()*J;	// A=J(x)'J(x)
	g = J.t()*f;	// g=J(x)'f(x)

	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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
			// 这里一直累乘下去不知道会不会出现较大的累积误差，导致最终的矩阵不严格满足旋转矩阵的性质？
			// 要是有这个问题，那么解决方法就是后续跟进一个SVD分解，R=UV'
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

// void optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,		// 输入：		控制点坐标
// 												   const vector<Point2d> & vImgPts,		// 输入：		控制点观测像点坐标
// 												   Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
// 												   Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
// 												   Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
// 												   double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
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
// 	int k = 0;		// 迭代次数索引
// 	int v = 2;		// 更新 u 时需要用到的一个控制量      
// 	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
// 	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
// 	double g_norm;	// 梯度的模
// 	double h_norm;	// 改正量的模
// 
// 	double ratio_1_3 = 1.0/3.0;
// 
// 	bool found = false; // 标识是否已经满足迭代收敛条件
// 	int code = 2; // termination code
// 
// 	int nPts = vWrdPts.size(); // number of control points
// 	int N = 16; // number of variables to be optimized
// 	int M = 2*nPts;	// 每个控制点提供 2 个观测方程
// 
// 	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
// 	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
// 	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量
// 
// 	vector<double> vds(nPts);
// 
// 	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
// 	derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],J,f,vds);
// 	A = J.t()*J;	// A=J(x)'J(x)
// 	g = J.t()*f;	// g=J(x)'f(x)
// 
// 	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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
// 	// 梯度收敛，说明已在平坦区域
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
// 		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解
// 
// 		h_norm = norm(h);
// 
// 		if (h_norm < eps2) // 改正量收敛
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
// 				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

// void optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
// 												    const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
// 												    Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
// 												    Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
// 												    Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
// 												    double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
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
// 	int k = 0;		// 迭代次数索引
// 	int v = 2;		// 更新 u 时需要用到的一个控制量      
// 	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
// 	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
// 	double g_norm;	// 梯度的模
// 	double h_norm;	// 改正量的模
// 
// 	double ratio_1_3 = 1.0/3.0;
// 
// 	bool found = false; // 标识是否已经满足迭代收敛条件
// 	int code = 2; // termination code
// 
// 	int nPts = vWrdPts.size(); // number of control points
// 	int N = 16; // number of variables to be optimized
// 	int M = 2*nPts;	// 每个控制点提供 2 个观测方程
// 
// 	Mat J(M, N, CV_64FC1, Scalar(0)); // the jacobian matrix
// 	Mat f(M, 1, CV_64FC1, Scalar(0)); // the error vector
// 	Mat A,g,h; // A = J(x)'J(x) 近似 F(x)" ; g = J(x)'f(x) 为 F(x)'; h 为改正量
// 
// 	vector<double> vds(nPts);
// 
// 	// 生成第一个 A = J(x)'J(x)，g = J(x)'f(x) ///////////////////////////////////////////////////////
// 	derivatives::j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(vWrdPts,vImgPts,mK,mR,mt,dist[0],dist[1],dist[2],dist[3],dist[4],J,f,vds);
// 	A = J.t()*J;	// A=J(x)'J(x)
// 	g = J.t()*f;	// g=J(x)'f(x)
// 
// 	// 生成一个和 A 尺寸一样的单位阵 I，方便后续解算正规方程 (A+uI)dx = -g
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
// 	// 梯度收敛，说明已在平坦区域
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
// 		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解
// 
// 		h_norm = norm(h);
// 
// 		if (h_norm < eps2) // 改正量收敛
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
// 				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

// optimize Ri based on Rotation Averaging using Newton-Raphson method
// 详细参考 Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> 中的 Algorithm A2
void SfM_ZZK::optim_nr_Ri_Govindu04(const vector<Matx33d> & vRijs,	// 输入：		所有观测的相对旋转矩阵
								    const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
								    vector<Matx33d> & vRis,			// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
								    int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
								    int * nIter /*= NULL*/,			// 输出：		实际迭代次数
								    double * residue /*= NULL*/,	// 输出：		最终的残差大小
								    int max_iter/* = 32*/,			// 输入：		最大迭代次数
								    double xEps /*= 1.0E-12*/,		// 输入：		终止条件
								    double fEps /*= 1.0E-12*/		// 输入：		终止条件
								    )
{
	int n_all = vRis.size(); // 图像总个数
	int m = vRijs.size(); // 所观测的相对旋转的个数

	int n_optim;
	if (idx_ref<0)
	{
		// 参考图像索引为-1，说明没有参考图像，所有图像的姿态都优化
		n_optim = n_all;
	} 
	else
	{
		// 这时说明有参考图像，优化除了参考图之外的所有图像的姿态
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// 先定出每个待优化图像在Jacobian矩阵中初始列索引号
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
	
	// 生成固定的Jacobian矩阵
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

	// 开始迭代优化
	for (int kk=0;kk<max_iter;++kk)
	{
		// 考察在当前姿态估计下的残差量
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
// 详细参考 Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> 中的 Algorithm A2
void SfM_ZZK::optim_nr_Ri(const vector<Matx33d> & vRijs,// 输入：		所有观测的相对旋转矩阵
						  const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
						  vector<Matx33d> & vRis,		// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
						  int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
						  int * nIter /*= NULL*/,		// 输出：		实际迭代次数
						  double * residue /*= NULL*/,	// 输出：		最终的残差大小
						  int max_iter /*= 32*/,		// 输入：		最大迭代次数
						  double xEps /*= 1.0E-12*/,	// 输入：		终止条件
						  double fEps /*= 1.0E-12*/		// 输入：		终止条件
						  )
{
	int n_all = vRis.size(); // 图像总个数
	int m = vRijs.size(); // 所观测的相对旋转的个数

	int n_optim;
	if (idx_ref<0)
	{
		// 参考图像索引为-1，说明没有参考图像，所有图像的姿态都优化
		n_optim = n_all;
	} 
	else
	{
		// 这时说明有参考图像，优化除了参考图之外的所有图像的姿态
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// 先定出每个待优化图像在Jacobian矩阵中初始列索引号
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
	
	// 生成固定的Jacobian矩阵
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

	// 开始迭代优化
	for (int kk=0;kk<max_iter;++kk)
	{
		// 考察在当前姿态估计下的残差量
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
// 详细参考 Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> 中的 Algorithm A2
void SfM_ZZK::optim_gn_Ri(const vector<Matx33d> & vRijs,// 输入：		所有观测的相对旋转矩阵
						  const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
						  vector<Matx33d> & vRis,		// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
						  int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
						  int * nIter /*= NULL*/,		// 输出：		实际迭代次数
						  double * residue /*= NULL*/,	// 输出：		最终的残差大小
						  int max_iter /*= 32*/,		// 输入：		最大迭代次数
						  double xEps /*= 1.0E-12*/,	// 输入：		终止条件
						  double fEps /*= 1.0E-12*/		// 输入：		终止条件
						  )
{
	int n_all = vRis.size(); // 图像总个数
	int m = vRijs.size(); // 所观测的相对旋转的个数

	int n_optim;
	if (idx_ref<0)
	{
		// 参考图像索引为-1，说明没有参考图像，所有图像的姿态都优化
		n_optim = n_all;
	} 
	else
	{
		// 这时说明有参考图像，优化除了参考图之外的所有图像的姿态
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// 先定出每个待优化图像在Jacobian矩阵中初始列索引号
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
	
	// 生成固定的Jacobian矩阵
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

	// 开始迭代优化
	for (int kk=0;kk<max_iter;++kk)
	{
		// 考察在当前姿态估计下的残差量
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

void SfM_ZZK::optim_lm_Ri(const vector<Matx33d> & vRijs,// 输入：		所有观测的相对旋转矩阵
						  const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
						  vector<Matx33d> & vRis,		// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
						  int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;	// 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int n_all = vRis.size(); // 图像总个数
	int m = vRijs.size(); // 所观测的相对旋转的个数

	int n_optim;
	if (idx_ref<0)
	{
		// 参考图像索引为-1，说明没有参考图像，所有图像的姿态都优化
		n_optim = n_all;
	} 
	else
	{
		// 这时说明有参考图像，优化除了参考图之外的所有图像的姿态
		n_optim = n_all-1;
	}

	Mat J(3*m, 3*n_optim, CV_64FC1, Scalar(0)), f(3*m, 1, CV_64FC1, Scalar(0));

	// 先定出每个待优化图像在Jacobian矩阵中初始列索引号
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

	// 生成固定的Jacobian矩阵
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

	Mat A = J.t()*J; // A = J(x)'J(x) 近似 F(x)"
	Mat g,h; // g = J(x)'f(x) 为 F(x)'; h 为改正量

	Mat I(A.rows, A.cols, CV_64FC1, Scalar(0));
	for (int i=0;i<A.rows;++i)
	{
		I.at<double>(i,i) = 1;
	}

	// 生成第一个 g = J(x)'f(x) ///////////////////////////////////////////////////////
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

	// 梯度收敛，说明已在平坦区域
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

		solve(A+u*I, -g, h, DECOMP_CHOLESKY); // 加入阻尼系数u之后，矩阵A肯定会是对称正定的，也就是说肯定可以使用Cholesky分解

		h_norm = norm(h);
		
		if (h_norm < eps2) // 改正量收敛
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

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,				// 输入兼输出：n个空间点坐标
											 const vector<Matx33d> & Ks,			// 输入：m个图像内参数矩阵
											 vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
											 vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
											 const vector<Matx<double,5,1>> & dists,// 输入：m个图像像差系数
											 const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
											 const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
											 const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
											 const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
											 const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
											 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size(); // 图像个数
	int n = XYZs.size(); // 物点个数
	int l = xys.size(); // 所有像点个数

	// Mat 结构
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+4*n,1,CV_64FC1,Scalar(0)),g_new(6*m+4*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+4*n,1,CV_64FC1,Scalar(0));

	// 迭代过程中使用的齐次物点坐标，因此由非齐次物点坐标拓展出齐次物点坐标
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

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
	vector<Matx<double,6,6>> U(m),U_new(m);
	vector<Matx<double,4,4>> V(n),V_new(n);
	vector<Matx<double,6,4>> W(l),W_new(l);
	vector<Matx<double,6,1>> ea(m),ea_new(m);
	vector<Matx<double,4,1>> eb(n),eb_new(n);
	vector<Matx<double,6,1>> da(m);
	vector<Matx<double,4,1>> db(n);
	vector<double> vds(l);

	derivatives::j_f_w_t_XYZW(XYZWs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	double err_rpj_init = sqrt(2*Fx/l);

	g_norm = norm(g,NORM_INF);

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_0_6_4(u,ptrMat,U,V,W,ea,eb,da,db,h); 

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_w_t_XYZW(XYZWs_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Rs=Rs_new;
				ts=ts_new;
				XYZWs=XYZWs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化完的齐次物点坐标还原为非齐次物点坐标输出
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

void SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
										   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
										   vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
										   vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
										   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
										   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
										   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
										   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
										   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
										   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
										   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size(); // 图像个数
	int n = XYZs.size(); // 物点个数
	int l = xys.size(); // 所有像点个数

	// Mat 结构
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+3*n,1,CV_64FC1,Scalar(0)),g_new(6*m+3*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+3*n,1,CV_64FC1,Scalar(0));

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
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

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_0_6_3(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_w_t_XYZ(XYZs_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Rs=Rs_new;
				ts=ts_new;
				XYZs=XYZs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

void SBA_ZZK::optim_sparse_lm_wj_cj_XiYiZiWi(vector<Point3d> & XYZs,				// 输入兼输出：n个空间点坐标
											 const vector<Matx33d> & Ks,			// 输入：m个图像内参数矩阵
											 vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
											 vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
											 const vector<Matx<double,5,1>> & dists,// 输入：m个图像像差系数
											 const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
											 const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
											 const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
											 const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
											 const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
											 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size(); // 图像个数
	int n = XYZs.size(); // 物点个数
	int l = xys.size(); // 所有像点个数

	// Mat 结构
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+4*n,1,CV_64FC1,Scalar(0)),g_new(6*m+4*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+4*n,1,CV_64FC1,Scalar(0));

	// 迭代过程中使用的齐次物点坐标，因此由非齐次物点坐标拓展出齐次物点坐标
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

	// 由于迭代过程中使用的是光心坐标，这里先生成光心坐标
	vector<Matx31d> cs(m);
	for (int j=0;j<m;++j)
	{
		cs[j] = -Rs[j].t()*ts[j];
	}

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
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

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_0_6_4(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_w_c_XYZW(XYZWs_new,Ks,Rs_new,cs_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Rs=Rs_new;
				cs=cs_new;
				XYZWs=XYZWs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化完的齐次物点坐标还原为非齐次物点坐标输出
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

	// 把优化的光心坐标转换为平移向量输出
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

void SBA_ZZK::optim_sparse_lm_wj_cj_XiYiZi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
										   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
										   vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
										   vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
										   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
										   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
										   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
										   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
										   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
										   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
										   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size(); // 图像个数
	int n = XYZs.size(); // 物点个数
	int l = xys.size(); // 所有像点个数

	// Mat 结构
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+3*n,1,CV_64FC1,Scalar(0)),g_new(6*m+3*n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+3*n,1,CV_64FC1,Scalar(0));

	// 由于迭代过程中使用的是光心坐标，这里先生成光心坐标
	vector<Matx31d> cs(m);
	for (int j=0;j<m;++j)
	{
		cs[j] = -Rs[j].t()*ts[j];
	}

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
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

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_0_6_3(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_w_c_XYZ(XYZs_new,Ks,Rs_new,cs_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Rs=Rs_new;
				cs=cs_new;
				XYZs=XYZs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化的光心坐标转换为平移向量输出
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

void SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
											   vector<Matx33d> & Ks,					// 输入兼输出：m个图像内参数矩阵
											   vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
											   vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
											   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
											   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
											   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
											   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
											   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
											   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
											   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size(); // 图像个数
	int n = XYZs.size(); // 物点个数
	int l = xys.size(); // 所有像点个数
	int M = 1+6*m+4*n; // 总优化参数个数
	int N = 2*l; // 总观测方程个数

	// Mat 结构
	Mat f(N,1,CV_64FC1,Scalar(0));
	Mat g(M,1,CV_64FC1,Scalar(0)),g_new(M,1,CV_64FC1,Scalar(0));
	Mat h(M,1,CV_64FC1,Scalar(0));

	// 迭代过程中使用的齐次物点坐标，因此由非齐次物点坐标拓展出齐次物点坐标
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

	// 把所有图像的初始等效焦距置为它们的均值
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

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
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

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_1_6_4(u,ptrMat,U,V,W,Q,G,H,ea,eb,ec,da,db,dc,h); 

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_f_w_t_XYZW(XYZWs_new,Ks_new,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,Q_new,G_new,H_new,ea_new,eb_new,ec_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Ks = Ks_new;
				Rs = Rs_new;
				ts = ts_new;
				XYZWs = XYZWs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
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

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化完的齐次物点坐标还原为非齐次物点坐标输出
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

void SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_c_fixedBaseline(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
															   vector<Matx33d> & Ks,					// 输入兼输出：m个图像内参数矩阵
															   vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
															   vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
															   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
															   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
															   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
															   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
															   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
															   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
															   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
															   int k_fixedBaseLine,						// 输入：哪两个图像之间的基线固定不动，k 是不调整的一端
															   int j_fixedBaseLine,						// 输入：哪两个图像之间的基线固定不动，j 是调整的一端
															   double d_fixedBaseLine,					// 输入：两幅图像之间的固定基线长度
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size(); // 图像个数
	int n = XYZs.size(); // 物点个数
	int l = xys.size(); // 所有像点个数
	int M = 1+6*m+4*n; // 总优化参数个数
	int N = 2*l; // 总观测方程个数

	// Mat 结构
	Mat f(N,1,CV_64FC1,Scalar(0));
	Mat g(M,1,CV_64FC1,Scalar(0)),g_new(M,1,CV_64FC1,Scalar(0));
	Mat h(M,1,CV_64FC1,Scalar(0));

	// 迭代过程中使用的齐次物点坐标，因此由非齐次物点坐标拓展出齐次物点坐标
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

	// 把所有图像的初始等效焦距置为它们的均值
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

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
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

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		
		derivatives::solveSBA_1_6_4_Daj(u,ptrMat,U,V,W,Q,G,H,ea,eb,ec,j_fixedBaseLine,Dajs,hVals,da,db,dc,h,constVal);

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_f_w_t_XYZW_c_fixedBaseline(XYZWs_new,Ks_new,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,
				k_fixedBaseLine,j_fixedBaseLine,d_fixedBaseLine,U_new,V_new,W_new,Q_new,G_new,H_new,ea_new,eb_new,ec_new,f,g_new,vds,Dajs_new[0],hVals_new[0]);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0)+constVal; // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Ks = Ks_new;
				Rs = Rs_new;
				ts = ts_new;
				XYZWs = XYZWs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
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

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化完的齐次物点坐标还原为非齐次物点坐标输出
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

// 20150722，每个物点由相对于其参考图的深度值来描述
void SBA_ZZK::optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
									   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
									   vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
									   vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
									   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
									   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
									   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
									   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
									   const vector<int> & ri_j,				// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
									   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
									   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
									   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Wij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size();	// 图像个数
	int n = XYZs.size();// 物点个数
	int l = xys.size(); // 所有像点个数
	int l_noref = l-n;	// 非参考像点的个数

	// 先生成深度初值，以及各物点的参考方向（即参考图中的归一化像点坐标）
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

		// 去像差得到理想像点坐标
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

		// 进而得到理想的归一化像点坐标
		Matx31d nxy;
		nxy(0) = ideal_x;
		nxy(1) = ideal_y;
		nxy(2) = 1;

		nxys[i] = calib::invK(K0)*nxy;
	}

	// Mat 结构
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+n,1,CV_64FC1,Scalar(0)),g_new(6*m+n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+n,1,CV_64FC1,Scalar(0));

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
	vector<Matx<double,6,6>> U(0.5*(m+1)*m),U_new(0.5*(m+1)*m); // (m+1)m/2 个 Ujk 矩阵
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

	double err_rpj_init = sqrt(2*Fx/l_noref); // 重投影残差只记到非参考像点头上

	g_norm = norm(g,NORM_INF);

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_0_6_1(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_w_t_w0_t0_d0_new2(ds_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,nxys,ri_j,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Rs=Rs_new;
				ts=ts_new;
				ds=ds_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化完的物点深度值全重新转化为物点的世界坐标
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

// 20150722，每个物点由相对于其参考图的深度值来描述
void SBA_ZZK::optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
									   const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
									   vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
									   vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
									   const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
									   const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
									   const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
									   const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
									   const vector<Matx31d> & nxys,			// 输入：n个物点关联的于其参考图像系中的观测视线方向，也即去像差归一化像点坐标
									   const vector<int> & ri_j,				// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
									   const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
									   const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
									   const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Wij在各自向量中存储的位置索引
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int m = Ks.size();	// 图像个数
	int n = XYZs.size();// 物点个数
	int l = xys.size(); // 所有像点个数
	int l_noref = l-n;	// 非参考像点的个数

	// 先生成深度初值，以及各物点的参考方向（即参考图中的归一化像点坐标）
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

	// Mat 结构
	Mat f(2*l,1,CV_64FC1,Scalar(0));
	Mat g(6*m+n,1,CV_64FC1,Scalar(0)),g_new(6*m+n,1,CV_64FC1,Scalar(0));
	Mat h(6*m+n,1,CV_64FC1,Scalar(0));

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
	vector<Matx<double,6,6>> U(0.5*(m+1)*m),U_new(0.5*(m+1)*m); // (m+1)m/2 个 Ujk 矩阵
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

	double err_rpj_init = sqrt(2*Fx/l_noref); // 重投影残差只记到非参考像点头上

	g_norm = norm(g,NORM_INF);

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solveSBA_0_6_1(u,ptrMat,U,V,W,ea,eb,da,db,h);

		h_norm = norm(h);

		if (h_norm < eps2) // 改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_w_t_w0_t0_d0_new2(ds_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,nxys,ri_j,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds);

			tmp = 0.5*f.t()*f;
			Fx_new = tmp.at<double>(0); // 候选参数处的目标函数值

			tmp = 0.5*h.t()*(u*h-g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (Fx - Fx_new) / L0_Lh;

			if (r>0)
			{
				// 采纳新参数
				Rs=Rs_new;
				ts=ts_new;
				ds=ds_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				Fx = Fx_new;

				g_norm = norm(g,NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
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

	// 把优化完的物点深度值全重新转化为物点的世界坐标
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