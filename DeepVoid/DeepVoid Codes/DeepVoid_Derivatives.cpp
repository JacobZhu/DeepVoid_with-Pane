#include "stdafx.h"

// 20151107, 计算 Huber 权重
// 默认残差 x 肯定是正值，例如重投影残差这种
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

// 20170808, for sparse LM of fundamental matrix
// 线性成像点坐标对空间点齐次坐标XYZW求导
// 其中 f1 = P11 X + P12 Y + P13 Z + P14 W
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
// 线性成像点坐标对空间点齐次坐标XYW求导，Z坐标固定为1，即导数为 0
// 其中 f1 = P11 X + P12 Y + P13 Z + P14 W
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
// 线性成像点坐标对投影矩阵的12个元素求导
// 其中 f1 = P11 X + P12 Y + P13 Z + P14 W
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

// 20220206，给定像素坐标、灰度值，以及当前的最小二乘图像匹配参数估计{h0,h1,a0,a1,a2,b0,b1,b2}
// 输出当前的匹配误差 f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
// 以及 f 对所有 8 个匹配参数的导数
// 其中 x' = a0 + a1*x + a2*y
//      y' = b0 + b1*x + b2*y
bool derivatives::j_f_hi_ai_bi(double x, double y,				// 输入：给定参考图像中的像素坐标
							   double I,						// 输入：参考图像(x,y)点处的图像灰度值
							   const Mat & img,					// 输入：匹配图像
							   double h0, double h1,			// 输入：最小二乘图像匹配参数
							   double a0, double a1, double a2,	// 输入：最小二乘图像匹配参数
							   double b0, double b1, double b2,	// 输入：最小二乘图像匹配参数
							   double & f,						// 输出：f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
							   Matx<double, 1, 8> & J			// 输出：f 对所有 8 个匹配参数的导数
						       )
{
	// 参考图像坐标经二维 affine 变换至匹配图像坐标
	double x1 = a0 + a1*x + a2*y;
	double y1 = b0 + b1*x + b2*y;

	double r, g, b;
	double rl, gl, bl; // rgb of the I(x-1, y)，左点
	double rr, gr, br; // rgb of the I(x+1, y)，右点
	double ru, gu, bu; // rgb of the I(x, y-1)，上点
	double rb, gb, bb; // rgb of the I(x, y+1)，下点

	if (BilinearInterp(img, x1, y1, r, g, b) &&
		BilinearInterp(img, x1 - 1, y1, rl, gl, bl) && BilinearInterp(img, x1 + 1, y1, rr, gr, br) &&
		BilinearInterp(img, x1, y1 - 1, ru, gu, bu) && BilinearInterp(img, x1, y1 + 1, rb, gb, bb))
	{
		double I1 = r;
		double I1_left = rl;
		double I1_right = rr;
		double I1_up = ru;
		double I1_bottom = rb;

		f = h0 + h1*I1 - I;

		double dI1_dx1 = (I1_right - I1_left)*0.5;	// dI/dx = (I(x+1,y)-I(x-1,y))/2
		double dI1_dy1 = (I1_bottom - I1_up)*0.5;	// dI/dy = (I(x,y+1)-I(x,y-1))/2

		J(0) = 1;			// df/dh0
		J(1) = I1;			// df/dh1

		J(2) = h1*dI1_dx1;	// df/da0
		J(3) = J(2)*x;		// df/da1
		J(4) = J(2)*y;		// df/da2

		J(5) = h1*dI1_dy1;	// df/db0
		J(6) = J(5)*x;		// df/db1
		J(7) = J(5)*y;		// df/db2

		return true;
	}
	else
	{
		return false;
	}
}

// 20220206，给定像素坐标、RGB值，以及当前的最小二乘图像匹配参数估计{h0,h1,a0,a1,a2,b0,b1,b2}
// 输出当前的匹配误差 f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
// 以及 f 对所有 8 个匹配参数的导数
// 其中 x' = a0 + a1*x + a2*y
//      y' = b0 + b1*x + b2*y
bool derivatives::j_f_hi_ai_bi(double x, double y,				// 输入：给定参考图像中的像素坐标
							   double R, double G, double B,	// 输入：参考图像(x,y)点处的RGB值
							   const Mat & img,					// 输入：匹配图像
							   double h0, double h1,			// 输入：最小二乘图像匹配参数
							   double a0, double a1, double a2,	// 输入：最小二乘图像匹配参数
							   double b0, double b1, double b2,	// 输入：最小二乘图像匹配参数
							   Matx31d & f,						// 输出：f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
							   Matx<double, 3, 8> & J			// 输出：f 对所有 8 个匹配参数的导数
						       )
{
	// 参考图像坐标经二维 affine 变换至匹配图像坐标
	double x1 = a0 + a1*x + a2*y;
	double y1 = b0 + b1*x + b2*y;

	double r, g, b;
	double rl, gl, bl; // rgb of the I(x-1, y)，左点
	double rr, gr, br; // rgb of the I(x+1, y)，右点
	double ru, gu, bu; // rgb of the I(x, y-1)，上点
	double rb, gb, bb; // rgb of the I(x, y+1)，下点

	if (BilinearInterp(img, x1, y1, r, g, b) &&
		BilinearInterp(img, x1 - 1, y1, rl, gl, bl) && BilinearInterp(img, x1 + 1, y1, rr, gr, br) &&
		BilinearInterp(img, x1, y1 - 1, ru, gu, bu) && BilinearInterp(img, x1, y1 + 1, rb, gb, bb))
	{
		//////////////////////////////////////////////////////////////////////////
		f(0) = h0 + h1*r - R;
		f(1) = h0 + h1*g - G;
		f(2) = h0 + h1*b - B;

		//////////////////////////////////////////////////////////////////////////
		double dr_dx1 = (rr - rl)*0.5;	// dI/dx = (I(x+1,y)-I(x-1,y))/2
		double dr_dy1 = (rb - ru)*0.5;	// dI/dy = (I(x,y+1)-I(x,y-1))/2

		J(0, 0) = 1;			// df/dh0
		J(0, 1) = r;			// df/dh1

		J(0, 2) = h1*dr_dx1;	// df/da0
		J(0, 3) = J(0, 2)*x;	// df/da1
		J(0, 4) = J(0, 2)*y;	// df/da2

		J(0, 5) = h1*dr_dy1;	// df/db0
		J(0, 6) = J(0, 5)*x;	// df/db1
		J(0, 7) = J(0, 5)*y;	// df/db2

		//////////////////////////////////////////////////////////////////////////
		double dg_dx1 = (gr - gl)*0.5;	// dI/dx = (I(x+1,y)-I(x-1,y))/2
		double dg_dy1 = (gb - gu)*0.5;	// dI/dy = (I(x,y+1)-I(x,y-1))/2

		J(1, 0) = 1;			// df/dh0
		J(1, 1) = g;			// df/dh1

		J(1, 2) = h1*dg_dx1;	// df/da0
		J(1, 3) = J(1, 2)*x;	// df/da1
		J(1, 4) = J(1, 2)*y;	// df/da2

		J(1, 5) = h1*dg_dy1;	// df/db0
		J(1, 6) = J(1, 5)*x;	// df/db1
		J(1, 7) = J(1, 5)*y;	// df/db2

		//////////////////////////////////////////////////////////////////////////
		double db_dx1 = (br - bl)*0.5;	// dI/dx = (I(x+1,y)-I(x-1,y))/2
		double db_dy1 = (bb - bu)*0.5;	// dI/dy = (I(x,y+1)-I(x,y-1))/2

		J(2, 0) = 1;			// df/dh0
		J(2, 1) = b;			// df/dh1

		J(2, 2) = h1*db_dx1;	// df/da0
		J(2, 3) = J(2, 2)*x;	// df/da1
		J(2, 4) = J(2, 2)*y;	// df/da2

		J(2, 5) = h1*db_dy1;	// df/db0
		J(2, 6) = J(2, 5)*x;	// df/db1
		J(2, 7) = J(2, 5)*y;	// df/db2

		return true;
	}
	else
	{
		return false;
	}
}

// 20220214
// 给定当前刚体变换参数 a（缩放比例）, R, t，X1 = aR*X0 + t
// 评估当前的刚体变换残差 f = aR*X0 + t - X1
// 给出当前残差向量对变换参数的 Jacobian 矩阵
void derivatives::j_f_a_w_t(const Matx31d & X0,			// 输入：X1 = aRX0 + t
						    const Matx31d & X1,			// 输入：X1 = aRX0 + t
						    double a,					// 输入：当前尺度估计 // const Matx<double, 7, 1> & params,	// 输入：当前刚体变换参数 a（缩放比例）, angX, angY, angZ, tx, ty, tz
						    const Matx33d & R,			// 输入：当前旋转矩阵估计
						    const Matx31d & t,			// 输入：当前平移向量估计
						    Matx31d & f,				// 输出：f = aRX0 + t - X1
						    Matx<double, 3, 7> & J		// 输出：df/dparams
						    )
{
	Matx31d X1_rpj = a*R*X0 + t;

	f = X1_rpj - X1;

	Matx31d der_f_a = R*X0;
//	Matx33d der_f_t = Matx33d::eye();

	Matx<double, 3, 9> der_f_R;

	double aX0 = a*X0(0);
	double aY0 = a*X0(1);
	double aZ0 = a*X0(2);

	der_f_R(0, 0) = der_f_R(1, 3) = der_f_R(2, 6) = aX0;
	der_f_R(0, 1) = der_f_R(1, 4) = der_f_R(2, 7) = aY0;
	der_f_R(0, 2) = der_f_R(1, 5) = der_f_R(2, 8) = aZ0;

	Matx<double, 9, 3> d_R_w = derivatives::der_R_w(R);

	Matx33d der_f_w = der_f_R*d_R_w;

	for (int i = 0; i < 3; ++i)
	{
		J(i, 0) = der_f_a(i);
		J(i, 1) = der_f_w(i, 0);
		J(i, 2) = der_f_w(i, 1);
		J(i, 3) = der_f_w(i, 2);
	}

	J(0, 4) = J(1, 5) = J(2, 6) = 1; // der_f_t 赋进去
}

// 20220214
// 给定当前刚体变换参数 a（缩放比例）, R, t，X1 = aR*X0 + t
// 评估当前的刚体变换残差 f = aR*X0 + t - X1
// 给出当前残差向量对变换参数的 Jacobian 矩阵
void derivatives::H_g_a_w_t(const vector<Matx31d> & X0s,		// 输入：X1 = aRX0 + t
							const vector<Matx31d> & X1s,		// 输入：X1 = aRX0 + t
							double a,							// 输入：当前尺度估计 // const Matx<double, 7, 1> & params,	// 输入：当前刚体变换参数 a（缩放比例）, angX, angY, angZ, tx, ty, tz
							const Matx33d & R,					// 输入：当前旋转矩阵估计
							const Matx31d & t,					// 输入：当前平移向量估计
							Matx<double, 7, 7> & H,				// 输出：Hessian 矩阵 H = J'WJ
							Matx<double, 7, 1> & g,				// 输出：所有 8 个参数的梯度向量 g = J'Wf
							double & F,							// 输出：总目标函数值 F = 0.5*f'Wf
							vector<Matx31d> & fs				// 输出：f = aRX0 + t - X1
							)
{
	int n = X0s.size();

	// 各输出先清空归零
	H = Matx<double, 7, 7>::zeros();
	g = Matx<double, 7, 1>::zeros();

	F = 0;

	fs.clear();

	for (int i = 0; i < n; ++i)
	{
		const Matx31d & X0 = X0s[i];
		const Matx31d & X1 = X1s[i];

		Matx31d fi;
		Matx<double, 3, 7> Ji;
		Matx33d Wi = Matx33d::eye();

		derivatives::j_f_a_w_t(X0, X1, a, R, t, fi, Ji);

		H += Ji.t()*Wi*Ji;
		g += Ji.t()*Wi*fi;
		Matx<double, 1, 1> fiWifi = 0.5*fi.t()*Wi*fi;

		F += fiWifi(0);

		fs.push_back(fi);
	}
}

// 20220207，最小二乘图像匹配优化中计算 Hessian 矩阵 H 和参数梯度向量 g
bool derivatives::H_g_hi_ai_bi(const vector<Point2d> & xys,		// 输入：参考图像中各参考像素的坐标
							   const vector<Vec3d> & RGBs,		// 输入：参考图像中各参考像素的RBG值，double型，[0]:R，[1]:G，[2]:B
							   const Mat & img,					// 输入：匹配图像
							   const Matx<double, 8, 1> & x,	// 输入：当前最小二乘图像匹配 8 个参数估计：h0,h1,a0,a1,a2,b0,b1,b2
							   Matx<double, 8, 8> & H,			// 输出：Hessian 矩阵 H = J'WJ
							   Matx<double, 8, 1> & g,			// 输出：所有 8 个参数的梯度向量 g = J'Wf
							   double & F,						// 输出：总目标函数值 F = 0.5*f'Wf
							   vector<Matx31d> & fs,			// 输出：参考图像中每个参考像素的预测误差
							   int IRLS /*= 0*/,				// 输入：是否进行迭代重加权 0：否；1：Huber；2：...
							   double e_Huber /*= 50*/			// 输入：Huber IRLS 的阈值
						       )
{
	int n = xys.size(); // 参考窗口中参考像素总个数

	double h0 = x(0);
	double h1 = x(1);

	double a0 = x(2);
	double a1 = x(3);
	double a2 = x(4);

	double b0 = x(5);
	double b1 = x(6);
	double b2 = x(7);

	// 各输出先清空归零
	H = Matx<double, 8, 8>::zeros();
	g = Matx<double, 8, 1>::zeros();

	F = 0;

	fs.clear();

	for (int i = 0; i < n; ++i)
	{
		const Point2d & xy = xys[i];
		const Vec3d & I = RGBs[i];

		double x = xy.x;
		double y = xy.y;

		double R = I[0];
		double G = I[1];
		double B = I[2];

		Matx31d fi;
		Matx<double, 3, 8> Ji;
		Matx33d Wi = Matx33d::eye();	// 权值矩阵默认为单位阵

		if (derivatives::j_f_hi_ai_bi(x, y, R, G, B, img, h0, h1, a0, a1, a2, b0, b1, b2, fi, Ji))
		{
			if (IRLS == 1)
			{
				double L2fi = norm(fi);
				double wi_Huber = derivatives::weight_Huber(L2fi, e_Huber);
				Wi(0, 0) = Wi(1, 1) = Wi(2, 2) = wi_Huber*wi_Huber;
			}

			H += Ji.t()*Wi*Ji;
			g += Ji.t()*Wi*fi;
			Matx<double, 1, 1> fiWifi = 0.5*fi.t()*Wi*fi;

			F += fiWifi(0);

			fs.push_back(fi);
		} 
		else
		{
			return false;	// 说明有哪个参考像素大概率是越界了
		}		
	}

	return true;
}

// 20220208，给定一组像点坐标对应，线性求解 2D 仿射变换参数 a0,a1,a2,b0,b1,b2
// x1 = a0 + a1x0 + a2y0
// y1 = b0 + b1x0 + b2y0
void derivatives::compute_affine_2D(const vector<Point2d> & xys0,			// 输入：所有参考窗口中的像素坐标
								    const vector<Point2d> & xys1,			// 输入：参考窗口中所有像素于匹配图像中的投影像素的坐标
								    double & a0, double & a1, double & a2,	// 输出：求解出来的 2D 仿射变换参数
								    double & b0, double & b1, double & b2	// 输出：求解出来的 2D 仿射变换参数
								    )
{
	Matx<double, 6, 6> H;
	Matx<double, 6, 1> g, x;
	Matx<double, 2, 6> Ai;
	Matx<double, 2, 1> bi;

	Ai(0, 0) = Ai(1, 3) = 1;

	for (int i = 0; i < xys0.size(); ++i)
	{
		const Point2d & xy0 = xys0[i];
		const Point2d & xy1 = xys1[i];

		Ai(0, 1) = Ai(1, 4) = xy0.x;
		Ai(0, 2) = Ai(1, 5) = xy0.y;

		bi(0) = xy1.x;
		bi(1) = xy1.y;

		H += Ai.t()*Ai;
		g += Ai.t()*bi;
	}

	// 解方程 AtA*x = Atb
	solve(H, g, x, DECOMP_CHOLESKY);

	a0 = x(0);
	a1 = x(1);
	a2 = x(2);
	b0 = x(3);
	b1 = x(4);
	b2 = x(5);
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

// 20151107, iteratively reweighted least square IRLS
// 迭代重加权模式，为了应对 outliers
// 采用 Huber 权重
void derivatives::j_f_w_t_XYZW_IRLS_Huber(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
										  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
										  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
										  const vector<Matx31d> & ts,				// 输入：m个图像平移向量
										  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
										  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
										  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
										  vector<Matx22d> & covInvs,				// 输出：l个所有像点坐标协方差矩阵的逆矩阵，也即各对角线元素 (i)=wi*wi，即每个观测量权重的平方，权重是由观测量自身的重投影残差来决定的
										  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
										  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
										  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
										  vector<Matx<double,6,6>> & U,				// 输出：m个Uj矩阵，仅跟图像参数有关
										  vector<Matx<double,4,4>> & V,				// 输出：n个Vi矩阵，仅跟空间点坐标有关
										  vector<Matx<double,6,4>> & W,				// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
										  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
										  vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
										  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值，准确来说应该是 wi*fi，还应该乘上权重值
										  Mat & g,									// 输出：6*m+4*n维的参数梯度
										  vector<double> & vds,						// 输出：l个像点的重投影残差量
										  double tc /*= 3.0*/						// 输入：对于重投影残差 d 小于 tc 的观测量权重全定为1，否则定权重为 tc/d
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

			double d = sqrt(dx*dx + dy*dy);

			// 依据重投影残差计算像点的 Huber 权重
			double weight = weight_Huber(d,tc);
			double weight2 = weight*weight;

			int idx2 = 2*(*ptr);

			// 加权的目标函数
			f.at<double>(idx2)   = weight*dx;
			f.at<double>(idx2+1) = weight*dy;

			// 给权值矩阵赋值
			Matx22d & covInv = covInvs[*ptr];
			covInv(0,0) = covInv(1,1) = weight2;

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

// 20170811，一个图像对，左图的投影矩阵固定为[I|0]，另一个为P（待优化的量）
// 还输入一系列在射影空间中重建出来的待优化的物点坐标，它们的 Z 坐标固定为1（因为在左图中有像点就注定Z坐标不可能为0），因此4维齐次坐标就退化为了XYW三维坐标表示
// 该函数主要用于非线性迭代优化两图间的基础矩阵 F
void derivatives::j_f_P_XYW(const Matx34d & P,				// 输入：当前右图投影矩阵 P 的估计
						    double X, double Y, double W,	// 输入：当前该物点的 X Y W 坐标估计
						    double x0, double y0,			// 输入：该物点于左参考图中的的实际观测像点坐标
						    double x1, double y1,			// 输入：该物点于右图中的的实际观测像点坐标
						    Matx<double,2,12> & A1,			// 输出：该物点于右图中的重投影像点坐标对其投影矩阵 P 各元素求的导数
						    Matx<double,2,3> & B1,			// 输出：该物点于右图中的重投影像点坐标对其 X Y W 坐标求的导数
						    double & dx0, double & dy0,		// 输出：当前估计下，该物点于左参考图中的重投影残差
						    double & dx1, double & dy1		// 输出：当前估计下，该物点于右图中的重投影残差
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

	// 物点于右图中的重投影像点坐标
	double rx1 = f1*g_1;
	double ry1 = f2*g_1;

	A1 = der_xy_P(X, Y, 1, W, f1, f2, g);
	B1 = der_xy_XYW(P, f1, f2, g);

	// [X Y 1 W] 于左参考图 [I|0] 中重投影像点坐标就是 [X Y]
	dx0 = X - x0;
	dy0 = Y - y0;

	dx1 = rx1 - x1;
	dy1 = ry1 - y1;
}

// 20170809，一个图像对，左图的投影矩阵固定为[I|0]，另一个为P（待优化的量）
// 还输入一系列在射影空间中重建出来的待优化的物点坐标，它们的 Z 坐标固定为1（因为在左图中有像点就注定Z坐标不可能为0），因此4维齐次坐标就退化为了XYW三维坐标表示
// 该函数主要用于非线性迭代优化两图间的基础矩阵 F
void derivatives::j_f_P_XYW(const Matx34d & P,									// 输入：当前估计的右图在射影空间中的投影矩阵
						    const vector<Point3d> & XYWs,						// 输入：当前估计的 n 个空间点的 XYW 坐标
							const vector<Point2d> & xysL,						// 输入：n 个空间点于左图（参考图）中观测像点坐标
							const vector<Point2d> & xysR,						// 输入：n 个空间点于右图中观测像点坐标
						    const vector<Matx22d> & covInvsL,					// 输入：n 个空间点于左图（参考图）中观测像点坐标协方差矩阵的逆矩阵
						    const vector<Matx22d> & covInvsR,					// 输入：n 个空间点于右图中观测像点坐标协方差矩阵的逆矩阵
						    Matx<double, 12, 12> & U,							// 输出：1 个 U 矩阵，仅和 P 有关
						    vector<Matx<double, 3, 3>> & V,						// 输出：n 个 Vi 矩阵，仅和空间点坐标有关
						    vector<Matx<double, 12, 3>> & W,					// 输出：n 个 Wi 矩阵，同时和 P 以及空间点坐标有关
						    Matx<double, 12, 1> & ea,							// 输出：1 个 ea 残差向量
						    vector<Matx<double, 3, 1>> & eb,					// 输出：n 个 ebi 残差向量
						    double & F,											// 输出：当前的目标函数值 0.5*ft*covinv*f
							double & x_norm,									// 输出：当前待优化参数向量的模，即2范数L2，|x|2
						    Mat & g,											// 输出：12+3*n维的参数梯度
						    vector<Point2d> & vds								// 输出：n 个空间点于左右两幅图中的重投影残差量
						    )
{
	int n = XYWs.size(); // 物点个数

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

// 20170816，稀疏LM优化基础矩阵中的稀疏求解正规方程的函数
void derivatives::solve_sparseLM_F(double u,								// 输入：阻尼系数
								 const Matx<double,12,12> & U,			// 输入：1个U矩阵，仅跟右图的投影矩阵P有关
								 const vector<Matx<double,3,3>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
								 const vector<Matx<double,12,3>> & W,	// 输入：n个Wi矩阵，同时跟空间点及P有关
								 const Matx<double,12,1> & ea,			// 输入：1个ea残差向量，仅跟P有关
								 const vector<Matx<double,3,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
								 Matx<double,12,1> & da,				// 输出：1个P的改正量
								 vector<Matx<double,3,1>> & db,			// 输出：n个空间点坐标的改正量
								 Mat & h								// 输出：12+3*n维的参数改正量
								 )
{
	int n = V.size(); // 空间点个数

	Matx<double, 12, 12> U_aug = U; // 存放 U' 矩阵
	vector<Matx<double, 3, 3>> V_aug(n); // 存放 inv(Vi') 矩阵
	vector<Matx<double, 12, 3>> Y(n); // 存放所有的 Yi = Wi * inv(Vi') 矩阵

	// 形成增广矩阵 U' = U + uI
	for (int k = 0; k < 12; ++k)
	{
		U_aug(k, k) += u;
	}	

	// 形成增广矩阵的逆 inv(Vi') = (Vi + uI)^(-1)
	// 以及所有的 Yi = Wi * inv(Vi') 矩阵
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

	// 填充 S 和 e
	for (int i = 0; i < n; ++i)
	{
		S -= Y[i] * W[i].t();
		e -= Y[i] * eb[i];
	}
	
	// 解方程 S*da = e 得到 P 的改正量
	solve(S, e, da, DECOMP_CHOLESKY);

	for (int i = 0; i < 12; ++i)
	{
		h.at<double>(i) = da(i);
	}

	// 再进一步求解各空间点坐标的改正量
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

	// 计算所有的 Aij、Bij、Cij 和 eij
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

// 20200604, iteratively reweighted least square, IRLS
// 迭代重加权模式，为了应对 outliers
// 采用 Huber 权重
void derivatives::j_f_f_w_t_XYZW_IRLS_Huber(const vector<Point4d> & XYZWs,				// 输入：n个空间点XYZW坐标
											const vector<Matx33d> & Ks,					// 输入：m个图像内参数矩阵
											const vector<Matx33d> & Rs,					// 输入：m个图像旋转矩阵
											const vector<Matx31d> & ts,					// 输入：m个图像平移向量
											const vector<Matx<double, 5, 1>> & dists,	// 输入：m个图像像差系数
											const vector<int> & distTypes,				// 输入：m个图像的像差系数类型
											const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
											vector<Matx22d> & covInvs,					// 输出：l个所有像点坐标协方差矩阵的逆矩阵，也即各对角线元素 (i)=wi*wi，即每个观测量权重的平方，权重是由观测量自身的重投影残差来决定的
											const vector<uchar> & j_fixed,				// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
											const vector<uchar> & i_fixed,				// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
											const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
											vector<Matx<double, 6, 6>> & U,				// 输出：m个Uj矩阵，仅跟图像参数有关
											vector<Matx<double, 4, 4>> & V,				// 输出：n个Vi矩阵，仅跟空间点坐标有关
											vector<Matx<double, 6, 4>> & W,				// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
											Matx<double, 1, 1> & Q,						// 输出：唯一一个Q矩阵，只跟共参数有关
											vector<Matx<double, 1, 6>> & G,				// 输出：m个Gj矩阵，同时和独有及共有图像参数有关
											vector<Matx<double, 1, 4>> & H,				// 输出：n个Hi矩阵，同时和共有图像参数以及物点有关
											vector<Matx<double, 6, 1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
											vector<Matx<double, 4, 1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
											Matx<double, 1, 1> & ec,					// 输出：唯一一个ec残差向量，仅跟共有图像参数有关
											Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
											Mat & g,									// 输出：1+6*m+4*n维的参数梯度
											vector<double> & vds,						// 输出：l个像点的重投影残差量
											double tc /*= 3.0*/							// 输入：对于重投影残差 d 小于 tc 的观测量权重全定为1，否则定权重为 tc/d
											)
{
	int n = XYZWs.size();	// 空间点个数
	int m = Ks.size();		// 图像个数
	int l = xys.size();		// 所有观测像点的个数

	vector<Matx<double,2,6>> A(l); // 所有的 Aij 矩阵
	vector<Matx<double,2,4>> B(l); // 所有的 Bij 矩阵
	vector<Matx<double,2,1>> C(l); // 所有的 Cij 矩阵
	vector<Matx21d> e(l); // 所有的 eij 残差向量

	// 计算所有的 Aij、Bij、Cij 和 eij
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

			double d = sqrt(dx*dx + dy*dy);

			// 依据重投影残差计算像点的 Huber 权重
			double weight = weight_Huber(d,tc);
			double weight2 = weight*weight;

			int idx2 = 2*(*ptr);

			// 加权的目标函数
			f.at<double>(idx2)	 = weight*dx;
			f.at<double>(idx2+1) = weight*dy;

			// 给权值矩阵赋值
			Matx22d & covInv = covInvs[*ptr];
			covInv(0,0) = covInv(1,1) = weight2;

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

// 2015.10.08, find all tracks based on Carl Olsson's algorithm in <Stable structure from motion for unordered image collections>
// global minimum weight version
// 2015.10.22, 把保存权重的{<I,J>, weight}结构从 map 改成 vector，从而可以在进行轨迹融合之前先对图像对按匹配数进行排序（map容器是只能按key排序）
// 之后就挨个图像对去进行融合即可，不需要再每次都于map中按值搜索最大的图像对，并于最后将其删除
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

// 20200622，采用新的数据结构
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

// 20220201，采用新的数据结构
void SfM_ZZK::FindAllTracks_Olsson(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise fundamental matrix F, matches and projective reconstruction
								   MultiTracksWithFlags & map_tracks,							// output:	all the found tracks
								   int nFlagImgpt /*= 1*/										// input:	特征轨迹中的每个像点预设多少个标志位
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
				map_one_track.insert(make_pair(I, make_pair(iter_matches_IJ->queryIdx, std::vector<int>(nFlagImgpt)))); // 每个像点先安排 n 个标志位，并初始化其值为0
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
				map_one_track.insert(make_pair(J, make_pair(iter_matches_IJ->trainIdx, std::vector<int>(nFlagImgpt)))); // 每个像点先安排 n 个标志位，并初始化其值为0
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

// 20151128，老的特征估计结构
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

// 20151128，老的特征估计结构
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

// 20151128，老的特征估计结构
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

// 20151128，采用老的结构
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
// 20220201，采用新结构
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

// 20200622, zhaokunz, 改用新的数据结构
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

// 20220201，zhaokunz，改用新的数据结构
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

// 20151108，新加入一幅图像后，要前方交会新的点
void SfM_ZZK::Triangulation_AddOneImg(PointCloud & map_pointcloud,				// output:	点云
									  const vector<DeepVoid::cam_data> & cams,	// input:	所有图像
									  const MultiTracks & map_tracks,			// input:	所有的特征轨迹
									  int idx_newImg,							// input:	新完成定向图像的索引号
									  double thresh_inlier /*= 1.5*/			// input:	用来判断内点的重投影残差阈值
									  )
{
	// 统计有哪些需要进行前方交会的特征轨迹
	typedef std::pair<Point3d,std::pair<int,double>> pair_XYZ_n_e; // <XYZi, ni, ei> 分别存储候选物点的坐标、内点个数和重投影残差
	typedef std::vector<pair_XYZ_n_e> vec_wrdpt_candidate;
	// 这里是统计的需要进行前方交会的特征轨迹
	std::map<int,std::vector<pair_XYZ_n_e>> map_tracks_need_triang; // <ID, <XYZ1, n1, e1>, <XYZ2, n2, e2>, ...>

	// 1. 考察每一条特征轨迹，统计有哪些特征轨迹需要进行前方交会
	for (auto iter_track=map_tracks.begin();iter_track!=map_tracks.end();++iter_track)
	{
		const int & trackID = iter_track->first;

		// 先看当前特征轨迹中包不包含新加入图像中的特征点
		auto iter_found_newImgPt = iter_track->second.find(idx_newImg);
		if (iter_found_newImgPt==iter_track->second.end())
		{
			// 说明当前考察的特征轨迹中不包含新加入图像中的特征点
			continue;
		}

		// 在包含当前新加入图像特征点的前提下，再看该特征轨迹是否事先已被重建出来
		auto iter_found_objpt = map_pointcloud.find(trackID);
		if (iter_found_objpt!=map_pointcloud.end())
		{
			// 说明已被重建出来
			if (iter_found_newImgPt->second.second)
			{
				// 且新加入图像中的特征点已被判为内点，这时什么都不用做
				continue;
			} 

			// 运行到此说明新加入图像中特征点被判为外点，这时该特征点可以申诉，和其它完成定向的图像交会，以期找到更好的物点坐标估计
			// 且其已经有一个候选物点坐标估计了
			vec_wrdpt_candidate vec_candidates;
			vec_candidates.push_back(make_pair(iter_found_objpt->second.m_pt,make_pair(0,0)));
			map_tracks_need_triang.insert(make_pair(trackID,vec_candidates));
		} 
		else
		{
			// 说明还未被重建出来，需要进行前方交会
			// 且目前没有任何已有的候选物点坐标估计
			vec_wrdpt_candidate vec_candidates;
			map_tracks_need_triang.insert(make_pair(trackID,vec_candidates));
		}
	}

	// 新加入图像的参数
	const cam_data & cam = cams[idx_newImg];
	const Matx33d & mK = cam.m_K;
	const Matx33d & mR = cam.m_R;
	const Matx31d & mt = cam.m_t;

	// 2. 利用新加入的图像和每一幅已定向好的图像实施前方交会
	for (int i=0;i<cams.size();++i)
	{
		const cam_data & cam_other = cams[i];

		if (!cam_other.m_bOriented || i==idx_newImg)
		{
			// 该图像没有定向好
			continue;
		}

		// 第 i 幅图像的参数
		const Matx33d & mK_other = cam_other.m_K;
		const Matx33d & mR_other = cam_other.m_R;
		const Matx31d & mt_other = cam_other.m_t;

		vector<int> trackIDs;
		vector<Point2d> imgpts;
		vector<Point2d> imgpts_other;

		// 遍历每个需要进行前方交会的特征轨迹，看哪些特征轨迹同时被第 i 幅图和新加入的图像观测到
		for (auto iter_track_need_triang=map_tracks_need_triang.begin();iter_track_need_triang!=map_tracks_need_triang.end();++iter_track_need_triang)
		{
			const int & trackID = iter_track_need_triang->first;

			auto iter_track = map_tracks.find(trackID);

			auto iter_found_imgi = iter_track->second.find(i);

			if (iter_found_imgi==iter_track->second.end())
			{
				// 于第 i 幅中没被找到
				continue;
			}

			auto iter_found_newImg = iter_track->second.find(idx_newImg);

			// 记录两幅图上的像点坐标
			Point2d imgpt, imgpt_other;

			imgpt.x = cam.m_feats.key_points[iter_found_newImg->second.first].pt.x;
			imgpt.y = cam.m_feats.key_points[iter_found_newImg->second.first].pt.y;

			imgpt_other.x = cam_other.m_feats.key_points[iter_found_imgi->second.first].pt.x;
			imgpt_other.y = cam_other.m_feats.key_points[iter_found_imgi->second.first].pt.y;

			trackIDs.push_back(trackID);
			imgpts.push_back(imgpt);
			imgpts_other.push_back(imgpt_other);
		}

		// 20151125, 有可能两图之间没有任何需要进行交会的点，这时直接continue
		if (imgpts.size()<1)
		{
			continue;
		}

		// 执行最优双目前方交会
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

			// 20151224，交会出来的物点坐标必须得是位于参与交会的两幅图前面，否则就不合理了，不采纳其为一候选物点坐标
 			Matx31d XYZ;
 			XYZ(0) = pt3d.x;
 			XYZ(1) = pt3d.y;
 			XYZ(2) = pt3d.z;
 
 			Matx31d XYZ_0 = mR*XYZ+mt;
 			Matx31d XYZ_1 = mR_other*XYZ+mt_other;
 
 			if (XYZ_0(2)<=0 || XYZ_1(2)<=0)
 			{
 				// 说明至少有一幅图像是位于该重建物点的前面了
 				continue;
 			}

			// 录入一个有效的物点坐标
			pair_XYZ_n_e XYZ_n_e = make_pair(pt3d/*wrdpts[j]*/,make_pair(0,0));
			
			auto iter_found_track_need_triang = map_tracks_need_triang.find(trackIDs[j]);
			iter_found_track_need_triang->second.push_back(XYZ_n_e);
		}
	}

	// 3. 对进行前方交会的所有特征轨迹的候选物点坐标进行评价，优先看内点集的大小，在内点集大小相同的情况下选择综合重投影残差最小的那个物点坐标输出
	for (auto iter_track_need_triang=map_tracks_need_triang.begin();iter_track_need_triang!=map_tracks_need_triang.end();++iter_track_need_triang)
	{
		const int & trackID = iter_track_need_triang->first;

		auto iter_track = map_tracks.find(trackID);

		vector<pair_XYZ_n_e> & vec_XYZ_n_e = iter_track_need_triang->second;

		// 如果连一个候选的物点坐标估计都没有的话就直接继续下一个
		if (vec_XYZ_n_e.size()<1)
		{
			continue;
		}

		// 考察评估每个候选物点估计
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
					// 没完成定向的图像就不做重投影
					continue;
				}

				// 取出观测量
				Point2d imgpt;
				imgpt.x = camI.m_feats.key_points[i].pt.x;
				imgpt.y = camI.m_feats.key_points[i].pt.y;

				// 物点重投影
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

				// 20151227，除了满足重投影残差足够小的条件外，还得保证物点位于图像前面
				if (d<thresh_inlier && XYZ_C(2)>0)
				{
					++iter_one_candidate->second.first;
					sum_inliers_d2 += d*d;
					++n_inliers;
				}
			}

			iter_one_candidate->second.second = sqrt(sum_inliers_d2/n_inliers);
		}

		// 先按内点集大小从大到小排列
		sort(vec_XYZ_n_e.begin(),vec_XYZ_n_e.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.first>b.second.first;});

		int n_inliers_max = vec_XYZ_n_e[0].second.first;

		if (n_inliers_max<2)
		{
			// 至少得有 2 个图像判为内点
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

		// 对于内点集大小相同的，再按照重投影残差从小到大排列
		sort(vec_XYZ_n_e_max.begin(),vec_XYZ_n_e_max.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.second<b.second.second;});

		auto iter_found_wrdpt = map_pointcloud.find(trackID);
		if (iter_found_wrdpt!=map_pointcloud.end())
		{
			// 如果已经存在了就直接更新坐标
			iter_found_wrdpt->second.m_pt = vec_XYZ_n_e_max[0].first;
		} 
		else
		{
			// 如果还不存在，则开辟新的
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


// 20151108，新加入一幅图像后，要前方交会新的点
// 20220202，采用新结构MultiTracksWithFlags
void SfM_ZZK::Triangulation_AddOneImg(PointCloud & map_pointcloud,				// output:	点云
									  const vector<DeepVoid::cam_data> & cams,	// input:	所有图像
									  const MultiTracksWithFlags & map_tracks,	// input:	所有的特征轨迹
									  int idx_newImg,							// input:	新完成定向图像的索引号
									  double thresh_inlier /*= 1.5*/			// input:	用来判断内点的重投影残差阈值
									  )
{
	// 统计有哪些需要进行前方交会的特征轨迹
	typedef std::pair<Point3d, std::pair<int, double>> pair_XYZ_n_e; // <XYZi, ni, ei> 分别存储候选物点的坐标、内点个数和重投影残差
	typedef std::vector<pair_XYZ_n_e> vec_wrdpt_candidate;
	// 这里是统计的需要进行前方交会的特征轨迹
	std::map<int, std::vector<pair_XYZ_n_e>> map_tracks_need_triang; // <ID, <XYZ1, n1, e1>, <XYZ2, n2, e2>, ...>

	// 1. 考察每一条特征轨迹，统计有哪些特征轨迹需要进行前方交会
	for (auto iter_track = map_tracks.begin(); iter_track != map_tracks.end(); ++iter_track)
	{
		const int & trackID = iter_track->first;

		// 先看当前特征轨迹中包不包含新加入图像中的特征点
		auto iter_found_newImgPt = iter_track->second.find(idx_newImg);
		if (iter_found_newImgPt == iter_track->second.end())
		{
			// 说明当前考察的特征轨迹中不包含新加入图像中的特征点
			continue;
		}

		// 在包含当前新加入图像特征点的前提下，再看该特征轨迹是否事先已被重建出来
		auto iter_found_objpt = map_pointcloud.find(trackID);
		if (iter_found_objpt != map_pointcloud.end())
		{
			// 说明已被重建出来
			if (iter_found_newImgPt->second.second[0]) // 20220202，第一个标志位为内点标志位
			{
				// 且新加入图像中的特征点已被判为内点，这时什么都不用做
				continue;
			}

			// 运行到此说明新加入图像中特征点被判为外点，这时该特征点可以申诉，和其它完成定向的图像交会，以期找到更好的物点坐标估计
			// 且其已经有一个候选物点坐标估计了
			vec_wrdpt_candidate vec_candidates;
			vec_candidates.push_back(make_pair(iter_found_objpt->second.m_pt, make_pair(0, 0)));
			map_tracks_need_triang.insert(make_pair(trackID, vec_candidates));
		}
		else
		{
			// 说明还未被重建出来，需要进行前方交会
			// 且目前没有任何已有的候选物点坐标估计
			vec_wrdpt_candidate vec_candidates;
			map_tracks_need_triang.insert(make_pair(trackID, vec_candidates));
		}
	}

	// 新加入图像的参数
	const cam_data & cam = cams[idx_newImg];
	const Matx33d & mK = cam.m_K;
	const Matx33d & mR = cam.m_R;
	const Matx31d & mt = cam.m_t;

	// 2. 利用新加入的图像和每一幅已定向好的图像实施前方交会
	for (int i = 0; i < cams.size(); ++i)
	{
		const cam_data & cam_other = cams[i];

		if (!cam_other.m_bOriented || i == idx_newImg)
		{
			// 该图像没有定向好
			continue;
		}

		// 第 i 幅图像的参数
		const Matx33d & mK_other = cam_other.m_K;
		const Matx33d & mR_other = cam_other.m_R;
		const Matx31d & mt_other = cam_other.m_t;

		vector<int> trackIDs;
		vector<Point2d> imgpts;
		vector<Point2d> imgpts_other;

		// 遍历每个需要进行前方交会的特征轨迹，看哪些特征轨迹同时被第 i 幅图和新加入的图像观测到
		for (auto iter_track_need_triang = map_tracks_need_triang.begin(); iter_track_need_triang != map_tracks_need_triang.end(); ++iter_track_need_triang)
		{
			const int & trackID = iter_track_need_triang->first;

			auto iter_track = map_tracks.find(trackID);

			auto iter_found_imgi = iter_track->second.find(i);

			if (iter_found_imgi == iter_track->second.end())
			{
				// 于第 i 幅中没被找到
				continue;
			}

			auto iter_found_newImg = iter_track->second.find(idx_newImg);

			// 记录两幅图上的像点坐标
			Point2d imgpt, imgpt_other;

			imgpt.x = cam.m_feats.key_points[iter_found_newImg->second.first].pt.x;
			imgpt.y = cam.m_feats.key_points[iter_found_newImg->second.first].pt.y;

			imgpt_other.x = cam_other.m_feats.key_points[iter_found_imgi->second.first].pt.x;
			imgpt_other.y = cam_other.m_feats.key_points[iter_found_imgi->second.first].pt.y;

			trackIDs.push_back(trackID);
			imgpts.push_back(imgpt);
			imgpts_other.push_back(imgpt_other);
		}

		// 20151125, 有可能两图之间没有任何需要进行交会的点，这时直接continue
		if (imgpts.size() < 1)
		{
			continue;
		}

		// 执行最优双目前方交会
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
			str += "	error happened in Triangulate_Optimal";
			AfxMessageBox(str);
		}

		for (int j = 0; j < wrdpts.size(); ++j)
		{
			const Point3d & pt3d = wrdpts[j];

			if (errs[j].x >= thresh_inlier || errs[j].y >= thresh_inlier)
			{
				continue;
			}

			// 20151224，交会出来的物点坐标必须得是位于参与交会的两幅图前面，否则就不合理了，不采纳其为一候选物点坐标
			Matx31d XYZ;
			XYZ(0) = pt3d.x;
			XYZ(1) = pt3d.y;
			XYZ(2) = pt3d.z;

			Matx31d XYZ_0 = mR*XYZ + mt;
			Matx31d XYZ_1 = mR_other*XYZ + mt_other;

			if (XYZ_0(2) <= 0 || XYZ_1(2) <= 0)
			{
				// 说明至少有一幅图像是位于该重建物点的前面了
				continue;
			}

			// 录入一个有效的物点坐标
			pair_XYZ_n_e XYZ_n_e = make_pair(pt3d/*wrdpts[j]*/, make_pair(0, 0));

			auto iter_found_track_need_triang = map_tracks_need_triang.find(trackIDs[j]);
			iter_found_track_need_triang->second.push_back(XYZ_n_e);
		}
	}

	// 3. 对进行前方交会的所有特征轨迹的候选物点坐标进行评价，优先看内点集的大小，在内点集大小相同的情况下选择综合重投影残差最小的那个物点坐标输出
	for (auto iter_track_need_triang = map_tracks_need_triang.begin(); iter_track_need_triang != map_tracks_need_triang.end(); ++iter_track_need_triang)
	{
		const int & trackID = iter_track_need_triang->first;

		auto iter_track = map_tracks.find(trackID);

		vector<pair_XYZ_n_e> & vec_XYZ_n_e = iter_track_need_triang->second;

		// 如果连一个候选的物点坐标估计都没有的话就直接继续下一个
		if (vec_XYZ_n_e.size() < 1)
		{
			continue;
		}

		// 考察评估每个候选物点估计
		for (auto iter_one_candidate = vec_XYZ_n_e.begin(); iter_one_candidate != vec_XYZ_n_e.end(); ++iter_one_candidate)
		{
			const Point3d & wrdpt = iter_one_candidate->first;

			Matx31d XYZ;
			XYZ(0) = wrdpt.x;
			XYZ(1) = wrdpt.y;
			XYZ(2) = wrdpt.z;

			int n_inliers = 0;
			double sum_inliers_d2 = 0;

			for (auto iter_oneimgpt = iter_track->second.begin(); iter_oneimgpt != iter_track->second.end(); ++iter_oneimgpt)
			{
				const int & I = iter_oneimgpt->first;
				const int & i = iter_oneimgpt->second.first;

				const cam_data & camI = cams[I];

				if (!camI.m_bOriented)
				{
					// 没完成定向的图像就不做重投影
					continue;
				}

				// 取出观测量
				Point2d imgpt;
				imgpt.x = camI.m_feats.key_points[i].pt.x;
				imgpt.y = camI.m_feats.key_points[i].pt.y;

				// 物点重投影
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
					str += "	error happened in compute projection";
					AfxMessageBox(str);
				}
				xyz(0) /= xyz(2);
				xyz(1) /= xyz(2);

				double dx = xyz(0) - imgpt.x;
				double dy = xyz(1) - imgpt.y;
				double d = sqrt(dx*dx + dy*dy);

				// 20151227，除了满足重投影残差足够小的条件外，还得保证物点位于图像前面
				if (d < thresh_inlier && XYZ_C(2)>0)
				{
					++iter_one_candidate->second.first;
					sum_inliers_d2 += d*d;
					++n_inliers;
				}
			}

			iter_one_candidate->second.second = sqrt(sum_inliers_d2 / n_inliers);
		}

		// 先按内点集大小从大到小排列
		sort(vec_XYZ_n_e.begin(), vec_XYZ_n_e.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b) {return a.second.first > b.second.first; });

		int n_inliers_max = vec_XYZ_n_e[0].second.first;

		if (n_inliers_max < 2)
		{
			// 至少得有 2 个图像判为内点
			continue;
		}

		vector<pair_XYZ_n_e> vec_XYZ_n_e_max;
		for (int i = 0; i < vec_XYZ_n_e.size(); ++i)
		{
			if (vec_XYZ_n_e[i].second.first == n_inliers_max)
			{
				vec_XYZ_n_e_max.push_back(vec_XYZ_n_e[i]);
			}
		}

		// 对于内点集大小相同的，再按照重投影残差从小到大排列
		sort(vec_XYZ_n_e_max.begin(), vec_XYZ_n_e_max.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b) {return a.second.second < b.second.second; });

		auto iter_found_wrdpt = map_pointcloud.find(trackID);
		if (iter_found_wrdpt != map_pointcloud.end())
		{
			// 如果已经存在了就直接更新坐标
			iter_found_wrdpt->second.m_pt = vec_XYZ_n_e_max[0].first;
		}
		else
		{
			// 如果还不存在，则开辟新的
			CloudPoint cloudpt;
			cloudpt.m_idx = trackID;
			cloudpt.m_pt = vec_XYZ_n_e_max[0].first;

			map_pointcloud.insert(make_pair(trackID, cloudpt));
		}
	}
}

// 20151111，利用特征轨迹中当前所有完成定向的图做前方交会，每两两交会得到一个坐标，最终取支持集最大的坐标更新
int SfM_ZZK::Triangulation_AllImgs(PointCloud & map_pointcloud,			// output:	点云
								    const vector<DeepVoid::cam_data> & cams,// input:	所有图像
								    const MultiTracks & map_tracks,			// input:	所有的特征轨迹
								    double thresh_inlier /*= 1.5*/			// input:	用来判断内点的重投影残差阈值
								    )
{
	typedef std::pair<Point3d,std::pair<int,double>> pair_XYZ_n_e; // <XYZi, ni, ei> 分别存储候选物点的坐标、内点个数和重投影残差
	typedef std::vector<pair_XYZ_n_e> vec_wrdpt_candidate;

	int n_inliers_changed = 0;

	for (auto iter_track=map_tracks.begin();iter_track!=map_tracks.end();++iter_track)
	{
		const int & trackID = iter_track->first;

		// 存放所有的候选物点坐标
		vec_wrdpt_candidate vec_XYZ_n_e;

		auto iter_found_wrdpt = map_pointcloud.find(trackID);
		if (iter_found_wrdpt!=map_pointcloud.end())
		{
			// 已被重建出来
			pair_XYZ_n_e XYZ_n_e = make_pair(iter_found_wrdpt->second.m_pt,make_pair(0,0));
			vec_XYZ_n_e.push_back(XYZ_n_e);
		}

		int n_curInliers = 0; // 当前空间坐标拥有的内点数

//		vector<int> vIdx_orientedImgs; // 所有已完成定向的图像索引
		vector<Point2d> vImgPts; // 所有已完成定向的图像中的像点坐标
		vector<Matx33d> vKs; // 已完成定向的图像内参数
		vector<Matx33d> vRs; // 已完成定向的图像旋转矩阵
		vector<Matx31d> vts; // 已完成定向的图像平移向量

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

		// 两两图像进行最优双目交会生成候选物点坐标
		for (int i=0;i<vImgPts.size();++i)
		{
			for (int j=i+1;j<vImgPts.size();++j)
			{
				vector<Point2d> imgpts_other, imgpts;
				imgpts_other.push_back(vImgPts[i]);
				imgpts.push_back(vImgPts[j]);				

				// 执行最优双目前方交会
				vector<Point3d> wrdpts;
				vector<Point2d> errs;
				double rpj_err = Triangulate_Optimal(imgpts_other, vKs[i], vRs[i], vts[i], imgpts, vKs[j], vRs[j], vts[j], wrdpts, errs);

				if (errs[0].x >= thresh_inlier || errs[0].y >= thresh_inlier)
				{
					continue;
				}

				// 录入一个有效的物点坐标
				pair_XYZ_n_e XYZ_n_e = make_pair(wrdpts[0],make_pair(0,0));
				vec_XYZ_n_e.push_back(XYZ_n_e);
			}
		}

		// 如果连一个候选的物点坐标估计都没有的话就直接继续下一个
		if (vec_XYZ_n_e.size()<1)
		{
			continue;
		}

		// 考察评估每个候选物点估计
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
				// 取出观测量
				Point2d imgpt = vImgPts[i];
				
				// 物点重投影
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

		// 先按内点集大小从大到小排列
		sort(vec_XYZ_n_e.begin(),vec_XYZ_n_e.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.first>b.second.first;});

		int n_inliers_max = vec_XYZ_n_e[0].second.first;

		if (n_inliers_max<2)
		{
			// 至少得有 2 个图像判为内点
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

		// 对于内点集大小相同的，再按照重投影残差从小到大排列
		sort(vec_XYZ_n_e_max.begin(),vec_XYZ_n_e_max.end(),
			[](const pair_XYZ_n_e & a, const pair_XYZ_n_e & b){return a.second.second<b.second.second;});

		if (iter_found_wrdpt!=map_pointcloud.end())
		{
//			if (n_curInliers<n_inliers_max/* && n_curInliers>1*/)
			if ((n_inliers_max-n_curInliers)>1)
			{
				++n_inliers_changed;
			}

			// 如果已经存在了就直接更新坐标
			iter_found_wrdpt->second.m_pt = vec_XYZ_n_e_max[0].first;
		} 
		else
		{
			// 如果还不存在，则开辟新的
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

// 20151109，输出当前点云
void SfM_ZZK::OutputPointCloud(CString strFile,							// input:	输出文件路径
							   const PointCloud & map_pointcloud,		// int:		点云
							   const vector<DeepVoid::cam_data> & cams,	// input:	所有图像
							   const MultiTracks & map_tracks,			// input:	所有的特征轨迹
							   vector<DeepVoid::CloudPoint> & cloud,	// output:	老的点云结构体
							   int n_minInilier /*= 2*/					// input:	至少得有该个数图像观测到该点
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

// 20220201，输出当前点云
void SfM_ZZK::OutputPointCloud(CString strFile,							// input:	输出文件路径
							   const PointCloud & map_pointcloud,		// input:	点云
							   const vector<DeepVoid::cam_data> & cams,	// input:	所有图像
							   const MultiTracksWithFlags & map_tracks,	// input:	所有的特征轨迹
							   vector<DeepVoid::CloudPoint> & cloud,	// output:	老的点云结构体
							   int n_minInilier /*= 2*/					// input:	至少得有该个数图像观测到该点
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
			const int & bInlier = iter_imgpt->second.second[0]; // 第一个标志位表示是否为inlier

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

// 20160405，把当前的点云和图像参数画于Viz3d窗口中
void SfM_ZZK::Draw3DScene(viz::Viz3d & wnd3d,						// output:	3D显示的窗口
	                      viz::WCloud & cld,
						  const PointCloud & map_pointcloud,		// input:	点云
						  const vector<DeepVoid::cam_data> & cams,	// input:	所有图像参数
						  const MultiTracks & map_tracks,			// input:	所有的特征轨迹
						  int n_minInilier /*= 2*/					// input:	至少得有该个数图像观测到该点
						  )
{
	// 要显示出来的点云点及其颜色
	vector<Point3d> pts_output;
	vector<Point3i> colors_output;

	// 统计看哪些点满足要求可以显示
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

// 20151112,统计每个点云点被观测次数的直方图
double SfM_ZZK::BuildCloudPointInlierHistogram(const PointCloud & map_pointcloud,	// output:	点云
											   const MultiTracks & map_tracks,		// input:	所有的特征轨迹
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

// 20151112,统计每个点云点被观测次数的直方图
// 20220202，采用新的数据结构
double SfM_ZZK::BuildCloudPointInlierHistogram(const PointCloud & map_pointcloud,		// output:	点云
											   const MultiTracksWithFlags & map_tracks,	// input:	所有的特征轨迹
											   std::map<int,int> & hist					// output:	the histogram
											   )
{
	for (auto iter_wrdpt = map_pointcloud.begin(); iter_wrdpt != map_pointcloud.end(); ++iter_wrdpt)
	{
		const int & trackID = iter_wrdpt->first;

		auto iter_found_track = map_tracks.find(trackID);

		int count = 0;

		for (auto iter_imgpt = iter_found_track->second.begin(); iter_imgpt != iter_found_track->second.end(); ++iter_imgpt)
		{
			const int & I = iter_imgpt->first;
			const int & i = iter_imgpt->second.first;
			const int & bInlier = iter_imgpt->second.second[0]; // 20220202，第一个标志位指明该点是否内点

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
			hist.insert(make_pair(count, 1));
		}
		else
		{
			// exist
			++iter_found_num->second;
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
											 vector<double> & vds,					// 输出：每个像点的重投影残差
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
//	vector<double> vds(l);

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

// 20151107，iteratively reweighted least squares
// 迭代重加权版本，采用 Huber 权重
void SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
													    const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
													    vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
													    vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
													    const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
													    const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
													    const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
														vector<Matx22d> & covInvs,				// 输出：l个所有像点坐标协方差矩阵的逆矩阵，(i)=wi*wi
													    const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
													    const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
													    const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
														vector<double> & vds,					// 输出：每个像点的重投影残差
													    double tc /*= 3.0*/,					// 输入：计算 Huber 权重时用到的常量
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
//	vector<double> vds(l);

	derivatives::j_f_w_t_XYZW_IRLS_Huber(XYZWs,Ks,Rs,ts,dists,distTypes,xys,covInvs,j_fixed,i_fixed,ptrMat,U,V,W,ea,eb,f,g,vds,tc);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	// 20151111，仅统计内点的重投影残差
	int n_inliers = 0;
	double sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		/*double dx = f.at<double>(2*i);
		double dy = f.at<double>(2*i+1);
		double d = sqrt(dx*dx+dy*dy);*/

		// 20170831，不能从 f 中取值，因为 f 中已经乘了权重了，判断内点集，应该看原始的偏差值
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
			derivatives::j_f_w_t_XYZW_IRLS_Huber(XYZWs_new,Ks,Rs_new,ts_new,dists,distTypes,xys,covInvs,j_fixed,i_fixed,
				ptrMat,U_new,V_new,W_new,ea_new,eb_new,f,g_new,vds,tc);

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

	// 20151111，仅统计内点的重投影残差
	n_inliers = 0;
	sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		/*double dx = f.at<double>(2*i);
		double dy = f.at<double>(2*i+1);
		double d = sqrt(dx*dx+dy*dy);*/

		// 20170831，不能从 f 中取值，因为 f 中已经乘了权重了，判断内点集，应该看原始的偏差值
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

// 20170820，双目视觉中，固定左图的投影矩阵为[I|0]，稀疏优化右图的投影矩阵 P
// 以及射影重建出来的 n 个物点坐标 [X Y 1 W]，其中 Z 坐标固定为 1
// 该函数主要目标是从优化完的 P 中分解出具有几何最优意义的基础矩阵 F 来
void SBA_ZZK::optim_sparse_lm_P_XiYiWi(Matx34d & P,									// 输入兼输出：当前估计的右图在射影空间中的投影矩阵
									   vector<Point3d> & XYWs,						// 输入兼输出：n个物点的XYW坐标，Z坐标默认为1
									   const vector<Point2d> & xysL,				// 输入：n 个空间点于左图（参考图）中观测像点坐标
									   const vector<Point2d> & xysR,				// 输入：n 个空间点于右图中观测像点坐标
									   const vector<Matx22d> & covInvsL,			// 输入：n 个空间点于左图（参考图）中观测像点坐标协方差矩阵的逆矩阵
									   const vector<Matx22d> & covInvsR,			// 输入：n 个空间点于右图中观测像点坐标协方差矩阵的逆矩阵
									   vector<Point2d> & vds,						// 输出：n 个空间点于左右两幅图中的重投影残差量
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
	int k = 0;		// 迭代次数索引
	int v = 2;		// 更新 u 时需要用到的一个控制量      
	double u;		// LM 优化算法中最关键的阻尼系数 (J'J + uI)h = -J'f
	double r;		// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;  // 梯度的模
	double h_norm;	// 改正量的模
	double h_thresh;// 改正量收敛判断阈值 eps2*(norm(x)+eps2)
	double F, F_new;// 目标函数值 0.5*ft*covInv*f
	double x_norm, x_norm_new; // 当前待优化参数向量的模，即2范数L2，|x|2
	double L0_Lh;	// 泰勒展开式的函数值下降量

	double ratio_1_3 = 1.0/3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2; // termination code

	int n = XYWs.size(); // 物点个数

	// Mat 结构
	Mat g(12 + 3 * n, 1, CV_64FC1, Scalar(0)), g_new(12 + 3 * n, 1, CV_64FC1, Scalar(0));
	Mat h(12 + 3 * n, 1, CV_64FC1, Scalar(0));
	Mat tmp;

	// [U W; W' V] 是在当前状态下的 J'covInv J 矩阵，也即还未增广的法向方程系数矩阵
	// [-ea; -eb] 则是当前状态下的参数梯度向量
	// [U_new W_new; W_new' V_new] 则是存放的候选状态下的未增广法向方程系数矩阵
	// [-ea_new; -eb_new] 则是存放的候选状态下的参数梯度向量
	// 只有当候选状态相对于当前状态是使得目标函数值下降的时候，候选状态才能被采纳成为当前状态，即参数估计从当前状态正式移动到候选状态
	// 也只有在此时，U_new V_new W_new ea_new eb_new 才正式取代 U V W ea eb
	Matx<double, 12, 12> U, U_new;
	vector<Matx<double, 3, 3>> V(n), V_new(n);
	vector<Matx<double, 12, 3>> W(n), W_new(n);
	Matx<double, 12, 1> ea, ea_new;
	vector<Matx<double, 3, 1>> eb(n), eb_new(n);
	Matx<double, 12, 1> da;
	vector<Matx<double, 3, 1>> db(n);

	derivatives::j_f_P_XYW(P, XYWs, xysL, xysR, covInvsL, covInvsR, U, V, W, ea, eb, F, x_norm, g, vds);
	
	// 初始重投影残差
	double err_rpj_init = sqrt(F / n);
	
	g_norm = norm(g, NORM_INF);

	// 梯度收敛，说明已在平坦区域
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

		// 基于当前所在状态下的 [U W; W' V] 以及梯度向量 [-ea; -eb] 和阻尼系数 u 来求解改正量
		derivatives::solve_sparseLM_F(u, U, V, W, ea, eb, da, db, h);

		h_norm = norm(h);

		h_thresh = eps2*(x_norm + eps2); // 根据当前待优化参数向量的模来确定改正量大小是否满足收敛条件

//		if (h_norm < eps2) // 改正量收敛
		if (h_norm < h_thresh) // 20170901，改用相对比例的方式判断改正量收敛
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

			// 基于改正量得到一候选参数估计，并评估候选参数估计处的目标函数，Jacobian 矩阵以及梯度向量
			derivatives::j_f_P_XYW(P_new, XYWs_new, xysL, xysR, covInvsL, covInvsR, U_new, V_new, W_new, ea_new, eb_new, F_new, x_norm_new, g_new, vds);

			tmp = 0.5*h.t()*(u*h - g);
			L0_Lh = tmp.at<double>(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (F - F_new) / L0_Lh;

			if (r > 0)
			{
				// 采纳新参数
				P = P_new;
				XYWs = XYWs_new;

				// 一并采纳新参数处的 Jacobian 矩阵和梯度向量
				U = U_new;
				V = V_new;
				W = W_new;
				ea = ea_new;
				eb = eb_new;
				g = g_new.clone();

				// 还采纳新参数处的目标函数值
				F = F_new;

				// 采纳已经计算出来的新参数向量的模
				x_norm = x_norm_new;

				g_norm = norm(g, NORM_INF);

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

// 20200607，iteratively reweighted least squares
// 迭代重加权版本，采用 Huber 权重
void SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_IRLS_Huber(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
														  vector<Matx33d> & Ks,						// 输入兼输出：m个图像内参数矩阵
														  vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
														  vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
														  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
														  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
														  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
														  vector<Matx22d> & covInvs,				// 输出：l个所有像点坐标协方差矩阵的逆矩阵，(i)=wi*wi
														  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
														  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
														  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
														  vector<double> & vds,						// 输出：每个像点的重投影残差
														  double tc /*= 3.0*/,						// 输入：计算 Huber 权重时用到的常量
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
//	vector<double> vds(l);

	derivatives::j_f_f_w_t_XYZW_IRLS_Huber(XYZWs, Ks, Rs, ts, dists, distTypes, xys, covInvs, j_fixed, i_fixed, ptrMat, U, V, W, Q, G, H, ea, eb, ec, f, g, vds, tc);

	Mat tmp = 0.5*f.t()*f;
	double Fx = tmp.at<double>(0);
	double Fx_new, L0_Lh;

	// 20200607，仅统计内点的重投影残差
	int n_inliers = 0;
	double sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		// 20170831，不能从 f 中取值，因为 f 中已经乘了权重了，判断内点集，应该看原始的偏差值
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
			derivatives::j_f_f_w_t_XYZW_IRLS_Huber(XYZWs_new, Ks_new, Rs_new, ts_new, dists, distTypes, xys, covInvs, j_fixed, i_fixed,
				ptrMat, U_new, V_new, W_new, Q_new, G_new, H_new, ea_new, eb_new, ec_new, f, g_new, vds, tc);

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

	// 20200607，仅统计内点的重投影残差
	n_inliers = 0;
	sum_d2 = 0;
	for (int i = 0; i < l; ++i)
	{
		// 20170831，不能从 f 中取值，因为 f 中已经乘了权重了，判断内点集，应该看原始的偏差值
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