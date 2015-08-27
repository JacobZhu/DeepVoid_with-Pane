/****************************************************************************************************************************/
/* Johnny Zhu's own library, trying to wrap the functions from Levmar lib, and make it for my own purpose.                  */
/* Author: Johnny Zhu (朱肇昆)                                                                             Date:2013.08.29  */
/****************************************************************************************************************************/

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

namespace DeepVoid
{

//#define MAX_CALIBPTS 100		// 用来标定的控制点的最大个数

// struct calib_data 
// {
// 	cam_data cdata;				// 包含待标定像机的参数
// 
// 	int n_pts;					// 空间三维控制点的个数
// 
// 	double X[MAX_CALIBPTS];		// 空间三维控制点的坐标
// 	double Y[MAX_CALIBPTS];		// 空间三维控制点的坐标
// 	double Z[MAX_CALIBPTS];		// 空间三维控制点的坐标
// };

// 解读优化函数输出的有关优化过程的各信息量
CString InterpretOptimInfo(double * info,				// 10 维的向量，用来输出优化过程的相关信息
						   const CMatrix & mCovar		// 各优化参数估计的协方差矩阵
						   );


/**************************************************************************************************************************************************/
/* 这里是不同的优化函数要用到的不同的求解重投影像点的函数                                                                                         */
/**************************************************************************************************************************************************/


// 该函数在优化第 2 幅视图相对于第 1 幅视图的外参数以及空间点在第 1 幅视图坐标系中的坐标时用来计算所有空间点在两幅视图中的所有重投影像点坐标
void proj_relative_XYZ_ext_euler(double * p,			// 待优化的参数，为 m 维的，在该函数中待优化的是 2 号视图相对于 1 号视图的 3 个欧拉角和 3 个平移向量，以及所有 a 个点的 3 维空间坐标
								 double * hx,			// 根据待优化参数形成的重投影观测量，为 n 维的向量
								 int m,					// 待优化参数的维数，要是有 a 个空间点，则 m = 3*a+6
								 int n,					// 观测向量的维数，要是有 a 个空间点，则总共的观测向量维数是 n = 4*a
								 void * adata			// 传进来的已有信息，针对该重投影函数来说的话，需要传进来的信息主要有像机的内参数，以及像差系数
								 );

// 该函数在优化用欧拉角和平移向量表示的视图外参数时用来计算各空间点在视图上的重投影像点坐标，以供 dlevmar_dif 函数调用
void proj_ext_rotvec(double * p,			// 待优化的参数，为 m 维的，在该函数中待优化的是 3 个旋转向量和 3 个平移向量，因此 m = 6
					 double * hx,			// 根据待优化参数形成的重投影观测量，为 n 维的向量
					 int m,					// 待优化参数的维数
					 int n,					// 观测向量的维数
					 void * adata			// 传进来的已有信息，针对该重投影函数来说的话，需要传进来的信息主要有像机的内参数，以及各空间三维点的坐标
					 );

void f_PIRO_Y_Z_AXAYAZ(double * p,			// Y,Z,AX,AY,AZ
					   double * hx,			// f=det(b,r1,r2)
					   int m,				// dimension of parameters
					   int n,				// dimension of observation equations i.e. number of f
					   void * adata			// containing distortion-free normalized image points i.e. CMatrix mImgPts1 and CMatrix mImgPts2
					   );

/**************************************************************************************************************************************************/


/**************************************************************************************************************************************************/
/* kinds of jacobian matrix																														  */
/**************************************************************************************************************************************************/

void j_PIRO_Y_Z_AXAYAZ(double * p,			// Y,Z,AX,AY,AZ
					   double * j,			// f=det(b,r1,r2)
					   int m,				// dimension of parameters
					   int n,				// dimension of observation equations i.e. number of f
					   void * adata			// containing distortion-free normalized image points i.e. CMatrix mImgPts1 and CMatrix mImgPts2
					   );

void j_ext_rotvec(double * p,			// rv1, rv2, rv3 and tx, ty, tz
				  double * j,			// jacobian
				  int m,				// dimension of parameters
				  int n,				// dimension of observation equations
				  void * adata			// 
				  );

/**************************************************************************************************************************************************/


/**************************************************************************************************************************************************/
/* 这里是基于 levmar.lib 库封装的不同的优化函数                                                                                                   */
/**************************************************************************************************************************************************/


// 优化第 2 幅视图相对于第 1 幅视图的外参数以及空间点在第 1 幅视图坐标系中的坐标
void optim_levmar_relative_XYZ_ext_euler(CMatrix & mWrdPts,				// 输入兼输出：4×n的矩阵，控制点的三维坐标 
										 const CMatrix & mImgPts1,		// 输入：3×n或2×n的矩阵，空间点在 1 号视图中的图像坐标
										 const CMatrix & mImgPts2,		// 输入：3×n或2×n的矩阵，空间点在 2 号视图中的图像坐标
										 const CMatrix & mK1,			// 输入：3×3或3×4的矩阵，1 号视图的内参数矩阵
										 const CMatrix & mK2,			// 输入：3×3或3×4的矩阵，2 号视图的内参数矩阵
										 CMatrix & mRT,					// 输入兼输出：4×4的矩阵，2 号视图相对于 1 号视图的初始外参数矩阵
										 const CMatrix & mDist1,		// 输入：5×1的 1 号视图的像差系数
										 const CMatrix & mDist2,		// 输入：5×1的 2 号视图的像差系数
										 int   distType1,				// 输入：1 号视图的像差类型，选择是 Weng's (0) 还是 D.C.Brown's (1)，都是 5 个像差系数
										 int   distType2,				// 输入：2 号视图的像差类型，选择是 Weng's (0) 还是 D.C.Brown's (1)，都是 5 个像差系数
										 int   maxIter,					// 输入：最大迭代次数
										 double * opts = NULL,			// 输入：5 维的向量，用来控制优化过程的参数，如果输入 NULL 则采用默认参数，其中
																		// opts[0]，levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																		// opts[1]，当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-17
																		// opts[2]，当待优化参数 2 范数的相对变化量小于该阈值时优化结束，默认为 1.0E-17
																		// opts[3]，当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-17
																		// opts[4]，采用有限差分估计雅阁比矩阵的时候要用到的步长参数，当该参数小于 0 时，雅阁比矩阵式采用中心差分来估计的，这样更准确，但会更慢些，默认为正的情况下，是采用前向差分估计的，默认为 1.0E-06
										 double * info = NULL,			// 输出：10 维的向量，用来输出优化过程的相关信息，具体每个输出元素的解释可以参考相关的信息解读函数
										 CMatrix & mCovar = CMatrix(0),	// 输出：各估计参数之间的协方差矩阵
										 int * iterNum = NULL			// 输出：迭代次数
										 );

// 已知像机内参数及像差系数，利用 levmar 代码库的函数优化像机的外参数 姿态由旋转向量表示， tx, ty, tz，也就是说同时优化像机姿态角和光心位置，共6个待优化参数
// 要求 3 个以上控制点参与优化
void optim_levmar_ext_rotvec(const vector<Point3d> & objPts,	// 输入：4×n的矩阵，控制点的三维坐标 
							 const vector<Point2d> & imgPts,	// 输入：3×n或2×n的矩阵，控制点的图像坐标
							 const Matx33d & mK,				// 输入：3×3或3×4的矩阵，像机的初始内参数矩阵
							 Matx33d & mR,						// 输入兼输出：4×4的像机初始外参数矩阵
							 Matx31d & mt,						// 输入兼输出：平移向量
							 const CMatrix & mDist,				// 输入：5×1的像机初始像差系数
							 int   distType,					// 输入：像差类型，选择是 Weng's (0) 还是 D.C.Brown's (1)，都是 5 个像差系数
							 int   maxIter,						// 输入：最大迭代次数
							 double * opts = NULL,				// 输入：5 维的向量，用来控制优化过程的参数，如果输入 NULL 则采用默认参数，其中
																// opts[0]，levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																// opts[1]，当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-17
																// opts[2]，当待优化参数 2 范数的相对变化量小于该阈值时优化结束，默认为 1.0E-17
																// opts[3]，当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-17
																// opts[4]，采用有限差分估计雅阁比矩阵的时候要用到的步长参数，当该参数小于 0 时，雅阁比矩阵式采用中心差分来估计的，这样更准确，但会更慢些，默认为正的情况下，是采用前向差分估计的，默认为 1.0E-06
							 double * info = NULL,				// 输出：10 维的向量，用来输出优化过程的相关信息，具体每个输出元素的解释可以参考相关的信息解读函数
							 CMatrix & mCovar = CMatrix(0),     // 输出：各估计参数之间的协方差矩阵
							 int * iterNum = NULL				// 输出：迭代次数
							 );

void PIRO_Y_Z_AXAYAZ_levmar(const CMatrix & mImgPts1,									// input: 3*n matrix, the distortion free normalized image coordinates on reference image
	                        const CMatrix & mImgPts2,									// input: 3*n matrix, the distortion free normalized image coordinates on the other image
							CMatrix & mRT,												// output:4*4 matrix, the output relative orientation
							double bx = 1,												// input: default fixed value for X coordinate of the other image's optical center in reference
							int   maxIter = 128,										// input：max iteration
							double * opts = NULL,										// 输入：4 维的向量，用来控制优化过程的参数，如果输入 NULL 则采用默认参数，其中
																						// opts[0]，levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																						// opts[1]，当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-17
																						// opts[2]，当待优化参数 2 范数的相对变化量小于该阈值时优化结束，默认为 1.0E-17
																						// opts[3]，当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-17
							double * info = NULL,										// 输出：10 维的向量，用来输出优化过程的相关信息，具体每个输出元素的解释可以参考相关的信息解读函数
							CMatrix & mCovar = CMatrix(0),								// 输出：各估计参数之间的协方差矩阵
							int * iterNum = NULL										// 输出：迭代次数
							);

/**************************************************************************************************************************************************/

}