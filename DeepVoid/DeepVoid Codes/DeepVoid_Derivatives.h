// 包含所有的求导函数

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

// 20150525
struct Point4d
{
	double x, y, z, w; // the point coordinates

	Point4d()
	{
		x=y=z=w=0;
	};

	Point4d(double _x, double _y, double _z, double _w)
	{
		x=_x;
		y=_y;
		z=_z;
		w=_w;
	};

	Point4d & operator = (const Point4d & otherPt)
	{
		if (this != &otherPt)
		{
			x=otherPt.x;
			y=otherPt.y;
			z=otherPt.z;
			w=otherPt.w;
		}

		return *this;
	};

	Point4d(const Point4d & pt)
	{
		*this = pt;
	};
};

namespace derivatives
{

// 线性成像点坐标对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> der_xy_fxfycxcys(const Matx33d & mR,	// input:	current rotation matrix
								  const Matx31d & mt,	// input:	current translation vector
								  const Matx31d & mX	// input:	current object coordinates
								  );

// 线性成像点坐标对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> der_xy_fxfycxcys(double u, double v);

// 线性成像点坐标对fx,fy,cx,cy共4个内参数求导
Matx<double,2,4> der_xy_fxfycxcy(double u, double v);

// 线性成像点坐标对f,cx,cy共3个内参数求导
Matx<double,2,3> der_xy_fcxcy(double u, double v);

// 线性成像点坐标对f共1个内参数求导
Matx<double,2,1> der_xy_f(double u, double v);

// 线性成像点的归一化像点坐标对其线性成像点坐标求导
Matx22d der_uv_xy(double fx, double fy, double s);

// 线性成像点坐标对其归一化像点坐标求导
Matx22d der_xy_uv(double fx, double fy, double s);

// 归一化像点坐标对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> der_uv_fxfycxcys(double x, double y, double fx, double fy, double cx, double cy, double s);

// 归一化像点畸变量对归一化像点坐标求导
Matx22d der_dudv_uv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5);

// 归一化像点畸变量对归一化像点坐标求导
Matx22d der_dudv_uv_brown(double u, double v, double k1, double k2, double k3, double k4, double k5);

// 像点畸变量对归一化像点畸变量求导
Matx22d der_dxdy_dudv(double fx, double fy, double s);

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> der_dxdy_fxfycxcys(double du, double dv);

// 像点畸变量对fx,fy,cx,cy,共4个内参数求导
Matx<double,2,4> der_dxdy_fxfycxcy(double du, double dv);

// 像点畸变量对f,cx,cy,共3个内参数求导
Matx<double,2,3> der_dxdy_fcxcy(double du, double dv);

// 像点畸变量对f共1个内参数求导
Matx<double,2,1> der_dxdy_f(double du, double dv);

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> der_dxdy_fxfycxcys_weng(double u, double v, double k1, double k2, double k3, double k4, double k5);

// 像点畸变量对fx,fy,cx,cy共4个内参数求导
Matx<double,2,4> der_dxdy_fxfycxcy_weng(double u, double v, double k1, double k2, double k3, double k4, double k5);

// 像点畸变量对fx,fy,cx,cy,s共5个内参数求导
Matx<double,2,5> der_dxdy_fxfycxcys_brown(double u, double v, double k1, double k2, double k3, double k4, double k5);

// 像点畸变量对fx,fy,cx,cy,s共4个内参数求导
Matx<double,2,4> der_dxdy_fxfycxcy_brown(double u, double v, double k1, double k2, double k3, double k4, double k5);

// 线性成像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
Matx<double,2,12> der_xy_Rt(double fx, double fy, double s, const Matx33d & mR, const Matx31d & mt, const Matx31d & mX);

// 归一化像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
Matx<double,2,12> der_uv_Rt(const Matx33d & mR, const Matx31d & mt, const Matx31d & mX);

// 归一化像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
Matx<double,2,12> der_uv_Rt(const Matx33d & mR, const Matx31d & mt, double X, double Y, double Z, double W);

// 归一化像点坐标对旋转矩阵 R 各元素以及平移向量 t 各元素求导
// 其中 v1 = r11 X + r12 Y + r13 Z + tx W
//		v2 = r21 X + r22 Y + r23 Z + ty W
//		v3 = r31 X + r32 Y + r33 Z + tz W
Matx<double,2,12> der_uv_Rt(double v1, double v2, double v3, double X, double Y, double Z, double W);

// 归一化像点坐标对旋转矩阵 R 各元素以及光心坐标 c 求导
// 其中 v1 = r11 (X-Cx) + r12 (Y-Cy) + r13 (Z-Cz)
//		v2 = r21 (X-Cx) + r22 (Y-Cy) + r23 (Z-Cz)
//		v3 = r31 (X-Cx) + r32 (Y-Cy) + r33 (Z-Cz)
// Matx<double,2,12> der_uv_Rc(const Matx33d & R, double u, double v, double v1, double v2, double v3, double X, double Y, double Z, double Cx, double Cy, double Cz);

// 归一化像点坐标对旋转矩阵 R 各元素以及光心坐标 c 求导
// 其中 v1 = r11 (X-WCx) + r12 (Y-WCy) + r13 (Z-WCz)
//		v2 = r21 (X-WCx) + r22 (Y-WCy) + r23 (Z-WCz)
//		v3 = r31 (X-WCx) + r32 (Y-WCy) + r33 (Z-WCz)
Matx<double,2,12> der_uv_Rc(const Matx33d & R, double u, double v, double v1, double v2, double v3, double X, double Y, double Z, double W, double Cx, double Cy, double Cz);

// 归一化像点坐标对空间点齐次坐标XYZW求导
// 其中 v1 = r11 X + r12 Y + r13 Z + tx W
//		v2 = r21 X + r22 Y + r23 Z + ty W
//		v3 = r31 X + r32 Y + r33 Z + tz W
Matx<double,2,4> der_uv_XYZW(const Matx33d & R, const Matx31d & t, double v1, double v2, double v3);

// 归一化像点坐标对空间点坐标XYZ求导
// 其中 v1 = r11 X + r12 Y + r13 Z + tx
//		v2 = r21 X + r22 Y + r23 Z + ty
//		v3 = r31 X + r32 Y + r33 Z + tz
Matx<double,2,3> der_uv_XYZ(const Matx33d & R, double v1, double v2, double v3);

// 旋转矩阵R各元素对旋转向量w各元素求导，按照R=dR*R0，dR=I+[w]的方式求得的
Matx<double,9,3> der_R_w(const Matx33d & mR);

// 刚体变换矩阵[R|t;0 1]各元素对旋转向量w各元素以及平移向量各元素求导，按照R=dR*R0，dR=I+[w]的方式求得的
Matx<double,12,6> der_Rt_wt(const Matx33d & mR);

// 旋转矩阵 R 以及光心坐标 c 对旋转向量 w 各元素以及光心坐标 c 各元素求导，按照R=dR*R0，dR=I+[w]的方式求得的
Matx<double,12,6> der_Rc_wc(const Matx33d & mR);

// 归一化像点畸变量对weng's像差系数求导
Matx<double,2,5> der_dudv_ki_weng(double u, double v);

// 归一化像点畸变量对brown's像差系数求导
Matx<double,2,5> der_dudv_ki_brown(double u, double v);


// 各类约束方程对像机参数求导 /////////////////////////////////////////////////////////////////////////////

// constraint: 固定当前图与某图之间的基线长为某值: ||Rjck+tj||^2 - d^2 = 0
// 该约束方程对当前图的旋转矩阵以及平移向量求导数
Matx<double,1,12> der_c_fixedBaseline_Rt(double Xkw,double Ykw, double Zkw,			// 图 k 的光心在世界系中的坐标
										 double Xkj,double Ykj, double Zkj			// 图 k 的光心在当前图像系中的坐标
										 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////


Matx<double,2,16> j_f_fxfycxcys_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
											   double x, double y,				// 该控制点的实际观测像点坐标
											   const Matx33d & mK,				// 当前的内参数估计
											   const Matx33d & mR,				// 当前的姿态估计
											   const Matx31d & mt,				// 当前的平移向量估计
											   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
											   int distType,					// 像差类型，0 weng's, 1 brown's
											   double & dx, double & dy			// 当前估计下的重投影残差
											   );

Matx<double,2,15> j_f_fxfycxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,		// 该控制点的坐标
											  double x, double y,				// 该控制点的实际观测像点坐标
											  const Matx33d & mK,				// 当前的内参数估计
											  const Matx33d & mR,				// 当前的姿态估计
											  const Matx31d & mt,				// 当前的平移向量估计
											  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
											  int distType,						// 像差类型，0 weng's, 1 brown's
											  double & dx, double & dy			// 当前估计下的重投影残差
											  );

Matx<double,2,14> j_f_fcxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
										   double x, double y,				// 该控制点的实际观测像点坐标
										   const Matx33d & mK,				// 当前的内参数估计
										   const Matx33d & mR,				// 当前的姿态估计
										   const Matx31d & mt,				// 当前的平移向量估计
										   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
										   int distType,					// 像差类型，0 weng's, 1 brown's
										   double & dx, double & dy			// 当前估计下的重投影残差
										   );

Matx<double,2,12> j_f_f_w_t_k1k2k3k4k5(double X, double Y, double Z,	// 该控制点的坐标
									   double x, double y,				// 该控制点的实际观测像点坐标
									   const Matx33d & mK,				// 当前的内参数估计
									   const Matx33d & mR,				// 当前的姿态估计
									   const Matx31d & mt,				// 当前的平移向量估计
									   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
									   int distType,					// 像差类型，0 weng's, 1 brown's
									   double & dx, double & dy			// 当前估计下的重投影残差
									   );

Matx<double,2,11> j_f_fxfycxcy_w_t_k1(double X, double Y, double Z,		// 该控制点的坐标
									  double x, double y,				// 该控制点的实际观测像点坐标
									  const Matx33d & mK,				// 当前的内参数估计
									  const Matx33d & mR,				// 当前的姿态估计
									  const Matx31d & mt,				// 当前的平移向量估计
									  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
									  int distType,						// 像差类型，0 weng's, 1 brown's
									  double & dx, double & dy			// 当前估计下的重投影残差
									  );

Matx<double,2,10> j_f_fcxcy_w_t_k1(double X, double Y, double Z,	// 该控制点的坐标
								   double x, double y,				// 该控制点的实际观测像点坐标
								   const Matx33d & mK,				// 当前的内参数估计
								   const Matx33d & mR,				// 当前的姿态估计
								   const Matx31d & mt,				// 当前的平移向量估计
								   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
								   int distType,					// 像差类型，0 weng's, 1 brown's
								   double & dx, double & dy			// 当前估计下的重投影残差
								   );

Matx<double,2,12> j_f_fxfycxcy_w_t_k1k2(double X, double Y, double Z,	// 该控制点的坐标
										double x, double y,				// 该控制点的实际观测像点坐标
										const Matx33d & mK,				// 当前的内参数估计
										const Matx33d & mR,				// 当前的姿态估计
										const Matx31d & mt,				// 当前的平移向量估计
										double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
										int distType,					// 像差类型，0 weng's, 1 brown's
										double & dx, double & dy		// 当前估计下的重投影残差
										);

Matx<double,2,11> j_f_fcxcy_w_t_k1k2(double X, double Y, double Z,	// 该控制点的坐标
								     double x, double y,			// 该控制点的实际观测像点坐标
								     const Matx33d & mK,			// 当前的内参数估计
								     const Matx33d & mR,			// 当前的姿态估计
								     const Matx31d & mt,			// 当前的平移向量估计
								     double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
								     int distType,					// 像差类型，0 weng's, 1 brown's
								     double & dx, double & dy		// 当前估计下的重投影残差
								     );

// Bundle adjustment 相关的求导和评价函数 ////////////////////////////////
void j_f_w_t_XYZW(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
				  double x, double y,						// 该控制点的实际观测像点坐标
				  const Matx33d & mK,						// 当前的内参数估计
				  const Matx33d & mR,						// 当前的姿态估计
				  const Matx31d & mt,						// 当前的平移向量估计
				  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
				  int distType,								// 像差类型，0 weng's, 1 brown's
				  Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
				  Matx<double,2,4> & B,						// 重投影像点坐标对空间点坐标XYZW求的导
				  double & dx, double & dy					// 当前估计下的重投影残差
				  );

void j_f_w_t_XYZ(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
				 double x, double y,						// 该控制点的实际观测像点坐标
				 const Matx33d & mK,						// 当前的内参数估计
				 const Matx33d & mR,						// 当前的姿态估计
				 const Matx31d & mt,						// 当前的平移向量估计
				 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
				 int distType,								// 像差类型，0 weng's, 1 brown's
				 Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
				 Matx<double,2,3> & B,						// 重投影像点坐标对空间点坐标 XYZ 求的导
				 double & dx, double & dy					// 当前估计下的重投影残差
				 );

void j_f_w_c_XYZW(double X, double Y, double Z, double W,	// 该控制点的齐次坐标
				  double x, double y,						// 该控制点的实际观测像点坐标
				  const Matx33d & mK,						// 当前的内参数估计
				  const Matx33d & mR,						// 当前的姿态估计
				  const Matx31d & mc,						// 当前的光心坐标估计
				  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
				  int distType,								// 像差类型，0 weng's, 1 brown's
				  Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
				  Matx<double,2,4> & B,						// 重投影像点坐标对空间点坐标 XYZW 求的导
				  double & dx, double & dy					// 当前估计下的重投影残差
				  );

void j_f_w_c_XYZ(double X, double Y, double Z, double W,	// 该控制点的齐次坐标，此处 W 应为 1
				 double x, double y,						// 该控制点的实际观测像点坐标
				 const Matx33d & mK,						// 当前的内参数估计
				 const Matx33d & mR,						// 当前的姿态估计
				 const Matx31d & mc,						// 当前的光心坐标估计
				 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
				 int distType,								// 像差类型，0 weng's, 1 brown's
				 Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
				 Matx<double,2,3> & B,						// 重投影像点坐标对空间点坐标 XYZ 求的导
				 double & dx, double & dy					// 当前估计下的重投影残差
				 );

void j_f_f_w_t_XYZW(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
				    double x, double y,							// 该控制点的实际观测像点坐标
					const Matx33d & mK,							// 当前的内参数估计
					const Matx33d & mR,							// 当前的姿态估计
					const Matx31d & mt,							// 当前的平移向量估计
					double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
					int distType,								// 像差类型，0 weng's, 1 brown's
					Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
					Matx<double,2,4> & B,						// 重投影像点坐标对空间点坐标XYZW求的导
					Matx<double,2,1> & C,						// 重投影像点坐标对一个等效焦距 f 求导
					double & dx, double & dy					// 当前估计下的重投影残差
					);

void j_f_f_w_c_XYZ(double X, double Y, double Z,	double W,	// 该控制点的齐次坐标
				   double x, double y,							// 该控制点的实际观测像点坐标
				   const Matx33d & mK,							// 当前的内参数估计
				   const Matx33d & mR,							// 当前的姿态估计
				   const Matx31d & mc,							// 当前的光心坐标估计
				   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
				   int distType,								// 像差类型，0 weng's, 1 brown's
				   Matx<double,2,6> & A,						// 重投影像点坐标对像机参数求的导
				   Matx<double,2,3> & B,						// 重投影像点坐标对空间点坐标XYZ求的导
				   Matx<double,2,1> & C,						// 重投影像点坐标对一个等效焦距 f 求导
				   double & dx, double & dy						// 当前估计下的重投影残差
				   );

// 基于深度的 BA 方法中，一个可调整像点坐标 xij 对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void j_f_w_t_w0_t0_d0(double d0,				// 输入：当前该物点相对于其参考图像的深度估计
					  double x, double y,		// 输入：该物点于当前成像图像中的观测像点坐标
					  const Matx33d & mK,		// 输入：当前成像图像的内参数估计
					  const Matx33d & mR,		// 输入：当前成像图像的姿态估计
					  const Matx31d & mt,		// 输入：当前成像图像的平移向量估计
					  double k1,double k2,double k3, double k4, double k5,	// 输入：当前成像图像的像差系数估计
					  int distType,				// 输入：当前成像图像的像差类型，0 weng's, 1 brown's
					  const Matx31d & nx0,		// 输入：该物点于其参考图像系中的归一化像点坐标，表征了其点心射线方向
					  const Matx33d & mR0,		// 输入：当前参考图像的姿态估计
					  const Matx31d & mt0,		// 输入：当前参考图像的平移向量估计
					  Matx<double,2,6> & A,		// 输出：重投影像点坐标对成像图像参数求的导
					  Matx<double,2,1> & B,		// 输出：重投影像点坐标对其深度求的导
					  Matx<double,2,6> & A0,	// 输出：重投影像点坐标对参考图像参数求的导
					  double & dx, double & dy	// 输出：当前估计下的重投影残差
					  );


// void j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
// 				  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
// 				  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
// 				  const vector<Matx31d> & ts,				// 输入：m个图像平移向量
// 				  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
// 				  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
// 				  const vector<Point2d> & xys,				// 输入：所有图像上的像点坐标，最多最多为 m*n 个
// 				  const vector<int> & j_fixed,				// 输入：哪些图像的参数是固定的，如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
// 				  const vector<int> & i_fixed,				// 输入：哪些空间点坐标是固定的，如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
// 				  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
// 				  vector<Matx<double,2,6>> & A,				// 输出：所有的Aij按行主序排成一个向量
// 				  vector<Matx<double,2,4>> & B,				// 输出：所有的Bij按行主序排成一个向量
// 				  vector<Matx21d> & e						// 输出：所有的 eij = xij - Q(aj,bi) 按行主序排成一个向量
// 				  );

void j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
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
				  vector<Matx<double,4,4>> & V,				// 输出：n个Vi矩阵，仅跟空间点坐标有关
				  vector<Matx<double,6,4>> & W,				// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
				  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
				  vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
				  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
				  Mat & g,									// 输出：6*m+4*n维的参数梯度
				  vector<double> & vds						// 输出：l个像点的重投影残差量
				  );

void j_f_w_t_XYZ(const vector<Point3d> & XYZs,			// 输入：n个空间点XYZ坐标
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
				 vector<Matx<double,3,3>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
				 vector<Matx<double,6,3>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
				 vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
				 vector<Matx<double,3,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
				 Mat & f,								// 输出：2*l个像点残差量，其实也就是评价的目标函数值
				 Mat & g,								// 输出：6*m+3*n维的参数梯度
				 vector<double> & vds					// 输出：l个像点的重投影残差量
				 );

void j_f_w_c_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
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
				  vector<Matx<double,4,4>> & V,				// 输出：n个Vi矩阵，仅跟空间点坐标有关
				  vector<Matx<double,6,4>> & W,				// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
				  vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
				  vector<Matx<double,4,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
				  Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
				  Mat & g,									// 输出：6*m+4*n维的参数梯度
				  vector<double> & vds						// 输出：l个像点的重投影残差量
				  );

void j_f_w_c_XYZ(const vector<Point3d> & XYZs,			// 输入：n个空间点XYZ坐标
				 const vector<Matx33d> & Ks,			// 输入：m个图像内参数矩阵
				 const vector<Matx33d> & Rs,			// 输入：m个图像旋转矩阵
				 const vector<Matx31d> & cs,			// 输入：m个图像光心坐标
				 const vector<Matx<double,5,1>> & dists,// 输入：m个图像像差系数
				 const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
				 const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
				 const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
				 const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
				 const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
				 const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
				 vector<Matx<double,6,6>> & U,			// 输出：m个Uj矩阵，仅跟图像参数有关
				 vector<Matx<double,3,3>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
				 vector<Matx<double,6,3>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
				 vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
				 vector<Matx<double,3,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
				 Mat & f,								// 输出：2*l个像点残差量，其实也就是评价的目标函数值
				 Mat & g,								// 输出：6*m+3*n维的参数梯度
				 vector<double> & vds					// 输出：l个像点的重投影残差量
				 );

void j_f_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
					const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
				    const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
				    const vector<Matx31d> & ts,				// 输入：m个图像平移向量
				    const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
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
				    );

void j_f_f_w_c_XYZ(const vector<Point3d> & XYZs,			// 输入：n个空间点XYZ坐标
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
				   vector<Matx<double,3,3>> & V,			// 输出：n个Vi矩阵，仅跟空间点坐标有关
				   vector<Matx<double,6,3>> & W,			// 输出：l个Wij矩阵，同时跟空间点及其观测图像有关
				   Matx<double,1,1> & Q,					// 输出：唯一一个Q矩阵，只跟共参数有关
				   vector<Matx<double,1,6>> & G,			// 输出：m个Gj矩阵，同时和独有及共有图像参数有关
				   vector<Matx<double,1,3>> & H,			// 输出：n个Hi矩阵，同时和共有图像参数以及物点有关
				   vector<Matx<double,6,1>> & ea,			// 输出：m个eaj残差向量，仅跟图像参数有关
				   vector<Matx<double,3,1>> & eb,			// 输出：n个ebi残差向量，仅跟空间点坐标有关
				   Matx<double,1,1> & ec,					// 输出：唯一一个ec残差向量，仅跟共有图像参数有关
				   Mat & f,									// 输出：2*l个像点残差量，其实也就是评价的目标函数值
				   Mat & g,									// 输出：1+6*m+3*n维的参数梯度
				   vector<double> & vds						// 输出：l个像点的重投影残差量
				   );

void j_f_f_w_t_XYZW_c_fixedBaseline(const vector<Point4d> & XYZWs,			// 输入：n个空间点XYZW坐标
									const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
									const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
									const vector<Matx31d> & ts,				// 输入：m个图像平移向量
									const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
									const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
									const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
									const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
									const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
									const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
									const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									int k_fixedBaseLine,					// 输入：哪两个图像之间的基线固定不动，k 是不调整的一端
									int j_fixedBaseLine,					// 输入：哪两个图像之间的基线固定不动，j 是调整的一端
									double d_fixedBaseLine,					// 输入：两幅图像之间的固定基线长度
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
									Matx<double,1,6> & Daj,					// 输出：约束方程对固定基线的 j 端外参数求导
									double & h								// 输出：约束方程当前的值
									);

// 基于深度的 BA 方法中，所有可调整像点坐标 xij 对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void j_f_w_t_w0_t0_d0(const vector<double> & ds,				// 输入：n个空间点关联的深度值
					  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
					  const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
					  const vector<Matx31d> & ts,				// 输入：m个图像平移向量
					  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
					  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
					  const vector<Point2d> & xys,				// 输入：l个所有图像上的可调整像点坐标，最多最多为 m*n 个
					  const vector<Matx22d> & covInvs,			// 输入：l个所有可调整像点坐标协方差矩阵的逆矩阵
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
					  );

// 基于深度的 BA 方法中，所有像点坐标 xij （包括可调整的非参考像点以及不可调整的参考像点）对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void j_f_w_t_w0_t0_d0_new(const vector<double> & ds,			// 输入：n个空间点关联的深度值
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
					  );

// 基于深度的 BA 方法中，所有像点坐标 xij （包括可调整的非参考像点以及不可调整的参考像点）对其成像图的外参数 w t 求导
// 还要对其参考图的外参数 w0 t0 求导
// 最后还要对该物点于其参考图中的深度 d0 进行求导
void j_f_w_t_w0_t0_d0_new2(const vector<double> & ds,			// 输入：n个空间点关联的深度值
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
					  );

// 求解 0 个像机共参数，6 个像机独有参数和 4 个物点独有参数的 Sparse Bundle Adjustment 问题的增广正规方程
void solveSBA_0_6_4(double u,								// 输入：阻尼系数
					const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
					const vector<Matx<double,6,6>> & U,		// 输入：m个Uj矩阵，仅跟图像参数有关
					const vector<Matx<double,4,4>> & V,		// 输入：n个Vi矩阵，仅跟空间点坐标有关
					const vector<Matx<double,6,4>> & W,		// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
					const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
					const vector<Matx<double,4,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
					vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
					vector<Matx<double,4,1>> & db,			// 输出：n个空间点坐标的改正量
					Mat & h									// 输出：6*m+4*n维的参数改正量
					);

void solveSBA_0_6_3(double u,								// 输入：阻尼系数
				    const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
				    const vector<Matx<double,6,6>> & U,		// 输入：m个Uj矩阵，仅跟图像参数有关
				    const vector<Matx<double,3,3>> & V,		// 输入：n个Vi矩阵，仅跟空间点坐标有关
				    const vector<Matx<double,6,3>> & W,		// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
				    const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
				    const vector<Matx<double,3,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
				    vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
				    vector<Matx<double,3,1>> & db,			// 输出：n个空间点坐标的改正量
				    Mat & h									// 输出：6*m+3*n维的参数改正量
				    );

void solveSBA_1_6_4(double u,								// 输入：阻尼系数
					const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
					const vector<Matx<double,6,6>> & U,		// 输入：m个Uj矩阵，仅跟图像参数有关
					const vector<Matx<double,4,4>> & V,		// 输入：n个Vi矩阵，仅跟空间点坐标有关
					const vector<Matx<double,6,4>> & W,		// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
					const Matx<double,1,1> & Q,				// 输入：唯一一个Q矩阵，只跟共参数有关
					const vector<Matx<double,1,6>> & G,		// 输入：m个Gj矩阵，同时和独有及共有图像参数有关
					const vector<Matx<double,1,4>> & H,		// 输入：n个Hi矩阵，同时和共有图像参数以及物点有关
					const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
					const vector<Matx<double,4,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点坐标有关
					const Matx<double,1,1> & ec,			// 输入：唯一一个ec残差向量，仅跟共有图像参数有关
					vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
					vector<Matx<double,4,1>> & db,			// 输出：n个空间点坐标的改正量
					Matx<double,1,1> & dc,					// 输出：共有图像参数的改正量
					Mat & h									// 输出：1+6*m+4*n维的参数改正量
					);

void solveSBA_1_6_4_Daj(double u,								// 输入：阻尼系数
						const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
						const vector<Matx<double,6,6>> & U,		// 输入：m个Uj矩阵，仅跟图像参数有关
						const vector<Matx<double,4,4>> & V,		// 输入：n个Vi矩阵，仅跟空间点坐标有关
						const vector<Matx<double,6,4>> & W,		// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
						const Matx<double,1,1> & Q,				// 输入：唯一一个Q矩阵，只跟共参数有关
						const vector<Matx<double,1,6>> & G,		// 输入：m个Gj矩阵，同时和独有及共有图像参数有关
						const vector<Matx<double,1,4>> & H,		// 输入：n个Hi矩阵，同时和共有图像参数以及物点有关
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
						);

// 20150721,物点是由一维的深度来表示
void solveSBA_0_6_1(double u,								// 输入：阻尼系数
				    const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引
				    const vector<Matx<double,6,6>> & U,		// 输入：总共(m+1)m/2个 Ujk 矩阵，仅跟图像参数有关
				    const vector<Matx<double,1,1>> & V,		// 输入：n个Vi矩阵，仅跟空间点深度有关
				    const vector<Matx<double,6,1>> & W,		// 输入：l个Wij矩阵，同时跟空间点深度及其观测图像有关
				    const vector<Matx<double,6,1>> & ea,	// 输入：m个eaj残差向量，仅跟图像参数有关
				    const vector<Matx<double,1,1>> & eb,	// 输入：n个ebi残差向量，仅跟空间点深度有关
				    vector<Matx<double,6,1>> & da,			// 输出：m个图像参数的改正量
				    vector<Matx<double,1,1>> & db,			// 输出：n个空间点深度的改正量
				    Mat & h									// 输出：6*m+n维的参数改正量
				    );

// void covarianceSBA_0_6_4(const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
// 						 const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
// 						 const vector<Matx<double,4,4>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
// 						 const vector<Matx<double,6,4>> & W		// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
// 						 );

void covarianceSBA_0_6_3(const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
						 const vector<Matx<double,6,6>> & U,	// 输入：m个Uj矩阵，仅跟图像参数有关
						 const vector<Matx<double,3,3>> & V,	// 输入：n个Vi矩阵，仅跟空间点坐标有关
						 const vector<Matx<double,6,3>> & W,	// 输入：l个Wij矩阵，同时跟空间点及其观测图像有关
						 vector<Matx<double,6,6>> & cov_a,		// 输出：m个图像独有参数的协方差矩阵
						 vector<Matx<double,3,3>> & cov_b		// 输出：n个空间点的坐标的协方差矩阵
						 );

void covarianceSBA_1_6_3(const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
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
						 );

void covarianceSBA_wj_cj_XiYiZi(const vector<Point3d> & XYZs,			// 输入：n个空间点坐标
								const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								const vector<Matx33d> & Rs,				// 输入：m个图像旋转矩阵
								const vector<Matx31d> & ts,				// 输入：m个图像平移向量
								const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								vector<Matx<double,6,6>> & cov_a,		// 输出：m个图像独有参数的协方差阵
								vector<Matx<double,3,3>> & cov_b		// 输出：n个空间点的坐标的协方差阵
								);

void covarianceSBA_f_wj_cj_XiYiZi(const vector<Point3d> & XYZs,				// 输入：n个空间点坐标
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
								  vector<Matx<double,6,6>> & cov_a,			// 输出：m个图像独有参数的协方差阵
								  vector<Matx<double,3,3>> & cov_b,			// 输出：n个空间点的坐标的协方差阵
								  Matx<double,1,1> & cov_c,					// 输出：共参数的协方差阵
								  vector<Matx<double,1,6>> & cov_ca			// 输出：图像共参数和各图像独有参数之间的协方差阵
								  );

//////////////////////////////////////////////////////////////////////////

void j_f_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
								  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
								  const Matx33d & mK,									// 当前的内参数估计
								  const Matx33d & mR,									// 当前的姿态估计
								  const Matx31d & mt,									// 当前的平移向量估计
								  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
								  int distType,											// 像差类型，0 weng's, 1 brown's
								  Mat & J,												// 输出：	Jacobian矩阵
								  Mat & f,												// 输出：	评估的目标函数值
								  vector<double> & vds									// 
								  );

void j_f_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
								 const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
								 const Matx33d & mK,									// 当前的内参数估计
								 const Matx33d & mR,									// 当前的姿态估计
								 const Matx31d & mt,									// 当前的平移向量估计
								 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
								 int distType,											// 像差类型，0 weng's, 1 brown's
								 Mat & J,												// 输出：	Jacobian矩阵
								 Mat & f,												// 输出：	评估的目标函数值
								 vector<double> & vds									// 
								 );

void j_f_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
							  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
							  const Matx33d & mK,									// 当前的内参数估计
							  const Matx33d & mR,									// 当前的姿态估计
							  const Matx31d & mt,									// 当前的平移向量估计
							  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
							  int distType,											// 像差类型，0 weng's, 1 brown's
							  Mat & J,												// 输出：	Jacobian矩阵
							  Mat & f,												// 输出：	评估的目标函数值
							  vector<double> & vds									// 
							  );

void j_f_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
						  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
						  const Matx33d & mK,									// 当前的内参数估计
						  const Matx33d & mR,									// 当前的姿态估计
						  const Matx31d & mt,									// 当前的平移向量估计
						  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
						  int distType,											// 像差类型，0 weng's, 1 brown's
						  Mat & J,												// 输出：	Jacobian矩阵
						  Mat & f,												// 输出：	评估的目标函数值
						  vector<double> & vds									// 
						  );

void j_f_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
						 const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
						 const Matx33d & mK,									// 当前的内参数估计
						 const Matx33d & mR,									// 当前的姿态估计
						 const Matx31d & mt,									// 当前的平移向量估计
						 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
						 int distType,											// 像差类型，0 weng's, 1 brown's
						 Mat & J,												// 输出：	Jacobian矩阵
						 Mat & f,												// 输出：	评估的目标函数值
						 vector<double> & vds									// 
						 );

void j_f_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
					  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
					  const Matx33d & mK,									// 当前的内参数估计
					  const Matx33d & mR,									// 当前的姿态估计
					  const Matx31d & mt,									// 当前的平移向量估计
					  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
					  int distType,											// 像差类型，0 weng's, 1 brown's
					  Mat & J,												// 输出：	Jacobian矩阵
					  Mat & f,												// 输出：	评估的目标函数值
					  vector<double> & vds									// 
					  );

void j_f_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
						   const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
						   const Matx33d & mK,									// 当前的内参数估计
						   const Matx33d & mR,									// 当前的姿态估计
						   const Matx31d & mt,									// 当前的平移向量估计
						   double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
						   int distType,										// 像差类型，0 weng's, 1 brown's
						   Mat & J,												// 输出：	Jacobian矩阵
						   Mat & f,												// 输出：	评估的目标函数值
						   vector<double> & vds									// 
						   );

void j_f_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,					// 输入：		控制点坐标
						const vector<Point2d> & vImgPts,					// 输入：		控制点观测像点坐标
						const Matx33d & mK,									// 当前的内参数估计
						const Matx33d & mR,									// 当前的姿态估计
						const Matx31d & mt,									// 当前的平移向量估计
						double k1,double k2,double k3, double k4, double k5,// 当前的像差系数估计
						int distType,										// 像差类型，0 weng's, 1 brown's
						Mat & J,											// 输出：	Jacobian矩阵
						Mat & f,											// 输出：	评估的目标函数值
						vector<double> & vds								// 
						);

// Matx<double,2,16> j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(double X, double Y, double Z,	// 该控制点的坐标
// 													double x, double y,				// 该控制点的实际观测像点坐标
// 													const Matx33d & mK,				// 当前的内参数估计
// 													const Matx33d & mR,				// 当前的姿态估计
// 													const Matx31d & mt,				// 当前的平移向量估计
// 													double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
// 													double & dx, double & dy		// 当前估计下的重投影残差
// 													);

// Matx<double,2,16> j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(double X, double Y, double Z,	// 该控制点的坐标
// 													 double x, double y,			// 该控制点的实际观测像点坐标
// 													 const Matx33d & mK,			// 当前的内参数估计
// 													 const Matx33d & mR,			// 当前的姿态估计
// 													 const Matx31d & mt,			// 当前的平移向量估计
// 													 double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
// 													 double & dx, double & dy		// 当前估计下的重投影残差
// 													 );

// void j_f_fxfycxcy_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,						// 输入：		控制点坐标
// 									  const vector<Point2d> & vImgPts,						// 输入：		控制点观测像点坐标
// 									  const Matx33d & mK,									// 当前的内参数估计
// 									  const Matx33d & mR,									// 当前的姿态估计
// 									  const Matx31d & mt,									// 当前的平移向量估计
// 									  double k1,double k2,double k3, double k4, double k5,	// 当前的像差系数估计
// 									  Mat & J,												// 输出：	Jacobian矩阵
// 									  Mat & f,												// 输出：	评估的目标函数值
// 									  vector<double> & vds									// 
// 									  );
// 
// void j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,					// 输入：		控制点坐标
// 									    const vector<Point2d> & vImgPts,					// 输入：		控制点观测像点坐标
// 									    const Matx33d & mK,									// 当前的内参数估计
// 									    const Matx33d & mR,									// 当前的姿态估计
// 									    const Matx31d & mt,									// 当前的平移向量估计
// 									    double k1,double k2,double k3, double k4, double k5,// 当前的像差系数估计
// 									    Mat & J,											// 输出：	Jacobian矩阵
// 									    Mat & f,											// 输出：	评估的目标函数值
// 									    vector<double> & vds								// 
// 									    );

}

namespace distortions
{

// 计算归一化像点畸变量
void dudv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv);

// 计算归一化像点畸变量
// examined on 2015.04.22
void dudv_brown(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv);

// 计算像点畸变量
void dxdy_weng(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy);

// 计算像点畸变量
// examined on 2015.04.22
void dxdy_brown(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy);

// 对带像差像点坐标做去像差处理
void remove_weng(double fx, double fy, double cx, double cy, double s,
				 double k1, double k2, double k3, double k4, double k5,
	             double dist_x, double dist_y,
				 double & ideal_x, double & ideal_y,
				 int maxIter = 32, 
				 double eps = 1e-10
				 );

// 对带像差像点坐标做去像差处理
void remove_brown(double fx, double fy, double cx, double cy, double s,
				  double k1, double k2, double k3, double k4, double k5,
	              double dist_x, double dist_y,
				  double & ideal_x, double & ideal_y,
				  int maxIter = 32, 
				  double eps = 1e-10
				  );

}

namespace calib
{

// 方法基于《Matrix Computations 3rd Edition》一书P.216中的Algorithm 5.1.3 (Computes Givens c and s)
// 根据输入的[a; b]'计算使得[c s; -s c]' * [a; b]' = [r; 0]'成立的cos和sin
void Givens(double a, double b, double & c, double & s);

// 对m×n的矩阵A进行RQ分解（要求m<=n）：A = RQ，其中Q为n×n的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
// 利用的是Givens旋转方法实现的，至于要求m<=n的原因在笔记本内有详细解释
void RQ_Givens(const Mat & A, Mat & R, Mat & Q);

// 对3×3的矩阵实施RQ分解
void RQ3x3_Givens(const Matx33d & A, Matx33d & R, Matx33d & Q);

// 根据输入的旋转矩阵，返回一个 3 维的向量，其为 θ|v|，其中 v 为旋转轴的单位向量，θ 为旋转弧度
Matx31d converse_R_rotvec(const Matx33d & mR);

// 将旋转向量转换为旋转矩阵
Matx33d converse_rotvec_R(double rv1, double rv2, double rv3);
Matx33d converse_rotvec_R(const Matx31d & v);
Matx33d converse_rotvec_R(const Matx13d & v);

// 给定四元数，生成旋转矩阵
Matx33d converse_quaternion_R(double a, double b, double c, double d);

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
Matx33d normalize_2d(const vector<Point2f> & pts, vector<Point2f> & pts_t);

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
// tested on 2015.04.22
Matx33d normalize_2d(const vector<Point2d> & pts, vector<Point2d> & pts_t);

// 20150421，把所有空间点平移至质心坐标系，且所有点到质心的平均距离为3^0.5
Matx44d normalize_3d(const vector<Point3f> & pts, vector<Point3f> & pts_t);

// 20150421，把所有空间点平移至质心坐标系，且所有点到质心的平均距离为3^0.5
// tested on 2015.04.22
Matx44d normalize_3d(const vector<Point3d> & pts, vector<Point3d> & pts_t);

Matx33d cross_mat(const Matx31d & v);
Matx33d cross_mat(const Matx13d & v);

// 返回输出K矩阵的逆矩阵
// tested on 2015.04.22
Matx33d invK(const Matx33d & mK);
Matx33d invK(double fx, double fy, double cx, double cy, double s);

// wx=PX,其中P为3×4的矩阵，X为4×1的向量
Matx34d dlt_34(const Mat & mX,	// 输入：	4×n 的矩阵
			   const Mat & mx	// 输入：	2×n 的矩阵
			   );

// wx=HX,其中H为3×3的矩阵，X为3×1的向量
Matx33d dlt_33(const Mat & mX,	// 输入：	3×n 的矩阵
			   const Mat & mx	// 输入：	2×n 的矩阵
			   );

// 从无穷单应的线性解aH中分解出像机内参和旋转矩阵，并返回尺度因子a
double decomp_KR(const Matx33d & mKR,	// 输入：	3×3 的aKR矩阵，也就是无穷单应
				 Matx33d & mK,			// 输出：	3×3 的像机内参矩阵
				 Matx33d & mR			// 输出：	3×3 的像机旋转矩阵
				 );

// 从投影矩阵的线性解aP中分解出像机内外参数
void decomp_P(const Matx34d & mP,	// 输入：	3×4 的投影矩阵aP
			  Matx33d & mK,			// 输出：	3×3 的像机内参矩阵
			  Matx33d & mR,			// 输出：	3×3 的像机旋转矩阵
			  Matx31d & mt			// 输出：	3×1 的像机平移向量
			  );

// 从线性解a[R|t]中分解出像机外参数
void decomp_R_t(const Matx34d & mR_t,	// 输入：	3×4 的a[R|t]
				Matx33d & mR,			// 输出：	3×3 的像机旋转矩阵
				Matx31d & mt			// 输出：	3×1 的像机平移向量
				);

// 从线性解aK[r1 r2 t]中分解出像机外参数
void decomp_H(const Matx33d & mH,	// 输入：	3×3 的aK[r1 r2 t]
			  const Matx33d & mK,	// 输入：	已知的 3×3 的内参矩阵
			  Matx33d & mR,			// 输出：	3×3 的像机旋转矩阵
			  Matx31d & mt			// 输出：	3×1 的像机平移向量
			  );

// 利用6个以上控制点线性标定像机投影矩阵
Matx34d calib_P(const vector<Point3d> & pts3d, const vector<Point2d> & pts2d);

// 利用4以上控制点和光心位置线性标定像机的无穷单应H=KR
Matx33d calib_KR(const vector<Point3d> & pts3d, const vector<Point2d> & pts2d, const Point3d & c);

}

namespace optim
{
void optim_lm_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
									   const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
									   Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
									   Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
									   Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
									   double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
									   int distType = 0,				// 输入：	0 weng's, 1 brown's
									   double * info = NULL,			// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									   int maxIter = 15,				// input:	the maximum number of iterations
									   double eps1 = 1.0E-8,			// input:	threshold
									   double eps2 = 1.0E-12			// input:	threshold
									   );

void optim_lm_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
									  const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
									  Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
									  Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
									  Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
									  double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
									  int distType = 0,					// 输入：	0 weng's, 1 brown's
									  double * info = NULL,				// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									  double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									  int maxIter = 15,					// input:	the maximum number of iterations
									  double eps1 = 1.0E-8,				// input:	threshold
									  double eps2 = 1.0E-12				// input:	threshold
									  );

void optim_lm_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
								   const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
								   Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
								   Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
								   Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
								   double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
								   int distType = 0,				// 输入：	0 weng's, 1 brown's
								   double * info = NULL,			// output:	runtime info, 5-vector
																	// info[0]:	the initial reprojection error
																	// info[1]:	the final reprojection error
																	// info[2]: final max gradient
																	// info[3]: the number of iterations
																	// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								   int maxIter = 15,				// input:	the maximum number of iterations
								   double eps1 = 1.0E-8,			// input:	threshold
								   double eps2 = 1.0E-12			// input:	threshold
								   );

void optim_lm_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
							   const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
							   Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
							   Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
							   Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
							   double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
							   int distType = 0,				// 输入：	0 weng's, 1 brown's
							   double * info = NULL,			// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							   int maxIter = 15,				// input:	the maximum number of iterations
							   double eps1 = 1.0E-8,			// input:	threshold
							   double eps2 = 1.0E-12			// input:	threshold
							   );

void optim_lm_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
							  const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
							  Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
							  Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
							  Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
							  double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
							  int distType = 0,					// 输入：	0 weng's, 1 brown's
							  double * info = NULL,				// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 15,					// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,				// input:	threshold
							  double eps2 = 1.0E-12				// input:	threshold
							  );

void optim_lm_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
						   const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
						   Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
						   Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
						   Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
						   double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
						   int distType = 0,				// 输入：	0 weng's, 1 brown's
						   double * info = NULL,			// output:	runtime info, 5-vector
															// info[0]:	the initial reprojection error
															// info[1]:	the final reprojection error
															// info[2]: final max gradient
															// info[3]: the number of iterations
															// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
						   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
						   int maxIter = 15,				// input:	the maximum number of iterations
						   double eps1 = 1.0E-8,			// input:	threshold
						   double eps2 = 1.0E-12			// input:	threshold
						   );

void optim_lm_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,// 输入：		控制点坐标
								const vector<Point2d> & vImgPts,// 输入：		控制点观测像点坐标
								Matx33d & mK,					// 输入兼输出：	待优化及优化后的内参数矩阵
								Matx33d & mR,					// 输入兼输出：	待优化及优化后的旋转矩阵
								Matx31d & mt,					// 输入兼输出：	待优化及优化后的平移向量
								double * dist,					// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
								int distType = 0,				// 输入：	0 weng's, 1 brown's
								double * info = NULL,			// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								double tau = 1.0E-3,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								int maxIter = 15,				// input:	the maximum number of iterations
								double eps1 = 1.0E-8,			// input:	threshold
								double eps2 = 1.0E-12			// input:	threshold
								);

void optim_lm_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
							 const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
						     Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
						     Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
						     Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
						     double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
						     int distType = 0,					// 输入：	0 weng's, 1 brown's
						     double * info = NULL,				// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
						     double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
						     int maxIter = 15,					// input:	the maximum number of iterations
						     double eps1 = 1.0E-8,				// input:	threshold
						     double eps2 = 1.0E-12				// input:	threshold
						     );

// void optim_lm_fxfycxcys_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
// 											const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
// 											Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
// 											Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
// 											Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
// 											double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
// 											double * info = NULL,				// output:	runtime info, 5-vector
// 																				// info[0]:	the initial reprojection error
// 																				// info[1]:	the final reprojection error
// 																				// info[2]: final max gradient
// 																				// info[3]: the number of iterations
// 																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
// 											double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
// 											int maxIter = 15,					// input:	the maximum number of iterations
// 											double eps1 = 1.0E-8,				// input:	threshold
// 											double eps2 = 1.0E-12				// input:	threshold
// 											);

// void optim_lm_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,	// 输入：		控制点坐标
// 											 const vector<Point2d> & vImgPts,	// 输入：		控制点观测像点坐标
// 											 Matx33d & mK,						// 输入兼输出：	待优化及优化后的内参数矩阵
// 											 Matx33d & mR,						// 输入兼输出：	待优化及优化后的旋转矩阵
// 											 Matx31d & mt,						// 输入兼输出：	待优化及优化后的平移向量
// 											 double * dist,						// 输入兼输出：	double dist[5]，待优化及优化后的weng's像差系数
// 											 double * info = NULL,				// output:	runtime info, 5-vector
// 																				// info[0]:	the initial reprojection error
// 																				// info[1]:	the final reprojection error
// 																				// info[2]: final max gradient
// 																				// info[3]: the number of iterations
// 																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
// 											 double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
// 											 int maxIter = 15,					// input:	the maximum number of iterations
// 											 double eps1 = 1.0E-8,				// input:	threshold
// 											 double eps2 = 1.0E-12				// input:	threshold
// 											 );
}

namespace SfM_ZZK
{
// optimize Ri based on Rotation Averaging using Newton-Raphson method
// 详细参考 Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> 中的 Algorithm A2
void optim_nr_Ri_Govindu04(const vector<Matx33d> & vRijs,	// 输入：		所有观测的相对旋转矩阵
						   const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
						   vector<Matx33d> & vRis,			// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
						   int idx_ref,						// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
						   int * nIter = NULL,				// 输出：		实际迭代次数
						   double * residue = NULL,			// 输出：		最终的残差大小
						   int max_iter = 32,				// 输入：		最大迭代次数
						   double xEps = 1.0E-12,			// 输入：		终止条件
						   double fEps = 1.0E-12			// 输入：		终止条件
						   );

// optimize Ri based on Rotation Averaging using Newton-Raphson method
// 详细参考 Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> 中的 Algorithm A2
void optim_nr_Ri(const vector<Matx33d> & vRijs,	// 输入：		所有观测的相对旋转矩阵
				 const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
				 vector<Matx33d> & vRis,		// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
				 int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
				 int * nIter = NULL,			// 输出：		实际迭代次数
				 double * residue = NULL,		// 输出：		最终的残差大小
				 int max_iter = 32,				// 输入：		最大迭代次数
				 double xEps = 1.0E-12,			// 输入：		终止条件
				 double fEps = 1.0E-12			// 输入：		终止条件
				 );

// optimize Ri based on Rotation Averaging using Gauss-Newton method
// 详细参考 Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> 中的 Algorithm A2
void optim_gn_Ri(const vector<Matx33d> & vRijs,	// 输入：		所有观测的相对旋转矩阵
				 const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
				 vector<Matx33d> & vRis,		// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
				 int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
				 int * nIter = NULL,			// 输出：		实际迭代次数
				 double * residue = NULL,		// 输出：		最终的残差大小
				 int max_iter = 32,				// 输入：		最大迭代次数
				 double xEps = 1.0E-12,			// 输入：		终止条件
				 double fEps = 1.0E-12			// 输入：		终止条件
				 );

void optim_lm_Ri(const vector<Matx33d> & vRijs,	// 输入：		所有观测的相对旋转矩阵
				 const vector<Point2i> & vijs,	// 输入：		每个相对旋转矩阵的ij索引号，从0开始
				 vector<Matx33d> & vRis,		// 输入&输出：	所有图像的初始旋转矩阵以及优化后的旋转矩阵
				 int idx_ref,					// 输入：		不动的参考图像的索引，如果没有不动的参考图像，就输入该索引为-1
				 double * info = NULL,			// output:	runtime info, 5-vector
												// info[0]:	the initial reprojection error
												// info[1]:	the final reprojection error
												// info[2]: final max gradient
												// info[3]: the number of iterations
												// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
				 double tau = 1.0E-3,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
				 int maxIter = 64,				// input:	the maximum number of iterations
				 double eps1 = 1.0E-8,			// input:	threshold
				 double eps2 = 1.0E-12			// input:	threshold
				 );

}

namespace SBA_ZZK
{

void optim_sparse_lm_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
								    const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								    vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
								    vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
								    const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								    const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								    const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								    const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								    const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								    const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								    const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									double * info = NULL,					// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									double tau = 1.0E-3,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									int maxIter = 64,						// input:	the maximum number of iterations
									double eps1 = 1.0E-8,					// input:	threshold
									double eps2 = 1.0E-12					// input:	threshold
								    );

void optim_sparse_lm_wj_tj_XiYiZi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
								  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								  vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
								  vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
								  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								  double * info = NULL,						// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								  int maxIter = 64,							// input:	the maximum number of iterations
								  double eps1 = 1.0E-8,						// input:	threshold
								  double eps2 = 1.0E-12						// input:	threshold
								  );

void optim_sparse_lm_wj_cj_XiYiZiWi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
								    const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								    vector<Matx33d> & Rs,					// 输入兼输出：m个图像旋转矩阵
								    vector<Matx31d> & ts,					// 输入兼输出：m个图像平移向量
								    const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								    const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								    const vector<Point2d> & xys,			// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								    const vector<Matx22d> & covInvs,		// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								    const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								    const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								    const SparseMat & ptrMat,				// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									double * info = NULL,					// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									double tau = 1.0E-3,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									int maxIter = 64,						// input:	the maximum number of iterations
									double eps1 = 1.0E-8,					// input:	threshold
									double eps2 = 1.0E-12					// input:	threshold
								    );

void optim_sparse_lm_wj_cj_XiYiZi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
								  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
								  vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
								  vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
								  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
								  double * info = NULL,						// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								  int maxIter = 64,							// input:	the maximum number of iterations
								  double eps1 = 1.0E-8,						// input:	threshold
								  double eps2 = 1.0E-12						// input:	threshold
								  );

void optim_sparse_lm_f_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
									  vector<Matx33d> & Ks,						// 输入兼输出：m个图像内参数矩阵
								      vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
								      vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
								      const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
								      const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
								      const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
								      const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
								      const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
								      const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
								      const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
									  double * info = NULL,						// output:	runtime info, 5-vector
																				// info[0]:	the initial reprojection error
																				// info[1]:	the final reprojection error
																				// info[2]: final max gradient
																				// info[3]: the number of iterations
																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									  int maxIter = 64,							// input:	the maximum number of iterations
									  double eps1 = 1.0E-8,						// input:	threshold
									  double eps2 = 1.0E-12						// input:	threshold
								      );

void optim_sparse_lm_f_wj_tj_XiYiZiWi_c_fixedBaseline(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
													  vector<Matx33d> & Ks,						// 输入兼输出：m个图像内参数矩阵
													  vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
													  vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
													  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
													  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
													  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
													  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
													  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
													  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
													  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
													  int k_fixedBaseLine,						// 输入：哪两个图像之间的基线固定不动，k 是不调整的一端
													  int j_fixedBaseLine,						// 输入：哪两个图像之间的基线固定不动，j 是调整的一端
													  double d_fixedBaseLine,					// 输入：两幅图像之间的固定基线长度
													  double * info = NULL,						// output:	runtime info, 5-vector
																								// info[0]:	the initial reprojection error
																								// info[1]:	the final reprojection error
																								// info[2]: final max gradient
																								// info[3]: the number of iterations
																								// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
													  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
													  int maxIter = 64,							// input:	the maximum number of iterations
													  double eps1 = 1.0E-8,						// input:	threshold
													  double eps2 = 1.0E-12						// input:	threshold
													  );

// 20150722，每个物点由相对于其参考图的深度值来描述
void optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
							  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							  vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
							  vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
							  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							  const vector<int> & ri_j,					// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
							  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Wij在各自向量中存储的位置索引
							  double * info = NULL,						// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 64,							// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,						// input:	threshold
							  double eps2 = 1.0E-12						// input:	threshold
							  );

// 20150728，每个物点由相对于其参考图的深度值来描述
// 每个物点关联的去像差归一化像点坐标是作为已有输入，而非在函数内部生成
void optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// 输入兼输出：n个空间点坐标
							  const vector<Matx33d> & Ks,				// 输入：m个图像内参数矩阵
							  vector<Matx33d> & Rs,						// 输入兼输出：m个图像旋转矩阵
							  vector<Matx31d> & ts,						// 输入兼输出：m个图像平移向量
							  const vector<Matx<double,5,1>> & dists,	// 输入：m个图像像差系数
							  const vector<int> & distTypes,			// 输入：m个图像的像差系数类型
							  const vector<Point2d> & xys,				// 输入：l个所有图像上的像点坐标，最多最多为 m*n 个
							  const vector<Matx22d> & covInvs,			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
							  const vector<Matx31d> & nxys,				// 输入：n个物点关联的于其参考图像系中的观测视线方向，也即去像差归一化像点坐标
							  const vector<int> & ri_j,					// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j
							  const vector<uchar> & j_fixed,			// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
							  const vector<uchar> & i_fixed,			// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
							  const SparseMat & ptrMat,					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Wij在各自向量中存储的位置索引
							  double * info = NULL,						// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 64,							// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,						// input:	threshold
							  double eps2 = 1.0E-12						// input:	threshold
							  );

}