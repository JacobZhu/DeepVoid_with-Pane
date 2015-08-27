/****************************************************************************************************************************/
/* Johnny Zhu's own library, trying to wrap the functions from Sparse Bundle Adjustment lib, and make it for my own purpose.*/
/* Author: Johnny Zhu (朱肇昆)                                                                             Date:2013.08.15  */
/****************************************************************************************************************************/

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

namespace DeepVoid
{

// 该函数在优化用欧拉角和平移向量表示的视图外参数以及各物点世界坐标时用来计算第 i 个物点在第 j 幅视图上的重投影像点坐标，以供 sba 的函数调用
void proj_ext_euler_str(int j,					// 当前视图的编号
						int i,					// 当前点的编号
						double * aj,			// 当前视图的外参数，前 3 个为视图姿态角 yaw, pitch, roll， 后 3 个为视图的平移向量 [tx, ty, tz]
						double * bi,			// 当前点的三维坐标，[X, Y, Z]
						double * xij,           // 当前点在当前视图中的重投影像点坐标，[x, y]
						void * adata			// 传进来的视图额外的已有信息，这里主要传进来视图的内参数以及像差类型和像差系数，为了计算带像差重投影像点
						);

// 该函数在优化用旋转向量和平移向量表示的视图外参数以及各物点世界坐标时用来计算第 i 个物点在第 j 幅视图上的重投影像点坐标，以供 sba 的函数调用
void proj_ext_rotvec_str(int j,					// 当前视图的编号
						 int i,					// 当前点的编号
						 double * aj,			// 当前视图的外参数，前 3 个为旋转向量， 后 3 个为视图的平移向量 [tx, ty, tz]
						 double * bi,			// 当前点的三维坐标，[X, Y, Z]
						 double * xij,          // 当前点在当前视图中的重投影像点坐标，[x, y]
						 void * adata			// 传进来的视图额外的已有信息，这里主要传进来视图的内参数以及像差类型和像差系数，为了计算带像差重投影像点
						 );

void jacob_ext_euler_str(int j,					// current index of image
						 int i,					// current index of object point
						 double * aj,			// parameter of current image, [yaw pitch roll tx ty tz]'
						 double * bi,			// current object point coordinates [X Y Z]'
						 double * Aij,			// the bi's projection on image aj derivative with respective to aj
						 double * Bij,			// the bi's projection on image aj derivative with respective to bi
						 void * adata			// extra data
						 );

void jacob_ext_rotvec_str(int j,				// current index of image
						  int i,				// current index of object point
						  double * aj,			// parameter of current image, [rv1 rv2 rv3 tx ty tz]'
						  double * bi,			// current object point coordinates [X Y Z]'
						  double * Aij,			// the bi's projection on image aj derivative with respective to aj
						  double * Bij,			// the bi's projection on image aj derivative with respective to bi
						  void * adata			// extra data
						  );

// 该函数封装了 Sparse Bundle Adjustment 库的光束法平差函数
// 返回所有重投影像点个数
int  optim_sba_levmar_XYZ_ext_euler(vector<CloudPoint> & cloudPts,		// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
									vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新

									int itermax = 1024,					// 输入：最大迭代次数

									double * opts = NULL,				// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																		// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																		// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																		// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																		// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																		// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

									double * info = NULL				// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																		// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																		// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																		// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																		// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																		// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																		// info[5]，# iterations，					总迭代次数
																		// info[6]，reason for terminating，		迭代结束原因：
																													// 1. 目标函数对优化参数导数太小
																													// 2. 改正量，即优化参数变化太小
																													// 3. 达到最大迭代次数
																													// 4. 残差相对变化太小
																													// 5. too many attempts to increase damping. Restart with increased mu
																													// 6. 残差太小
																													// 7. stopped by invalid (i.e. NaN or Inf) "func" values. This is a user error
																		// info[7]，# function evaluations，		目标函数调用次数
																		// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																		// info[9]，# number of linear systems solved，求解线性方程组的个数
									);

// 该函数封装了 Sparse Bundle Adjustment 库的光束法平差函数
// 返回所有重投影像点个数
int  optim_sba_levmar_XYZ_ext_rotvec(vector<CloudPoint> & cloudPts,		// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
									 vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
									 int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

									 int itermax = 1024,				// 输入：最大迭代次数

									 double * opts = NULL,				// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																		// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																		// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																		// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																		// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																		// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

									 double * info = NULL				// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																		// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																		// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																		// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																		// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																		// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																		// info[5]，# iterations，					总迭代次数
																		// info[6]，reason for terminating，		迭代结束原因：
																													// 1. 目标函数对优化参数导数太小
																													// 2. 改正量，即优化参数变化太小
																													// 3. 达到最大迭代次数
																													// 4. 残差相对变化太小
																													// 5. 残差太小
																													// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																													//	  definite normal equations matrix. Typically, this indicates a programming error in the
																													//    user-supplied Jacobian.
																													// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																													//    This signals a programming error in the user-supplied projection function func.
																		// info[7]，# function evaluations，		目标函数调用次数
																		// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																		// info[9]，# number of linear systems solved，求解线性方程组的个数
									 );

// 该函数封装了 Sparse Bundle Adjustment 库的光束法平差函数
// 返回所有重投影像点个数
int  optim_sba_levmar_XYZ_ext_rotvec_own(vector<CloudPoint> & cloudPts,		// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
									 vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
									 int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

									 int itermax = 1024,				// 输入：最大迭代次数

									 double * opts = NULL,				// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																		// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																		// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																		// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																		// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																		// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

									 double * info = NULL				// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																		// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																		// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																		// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																		// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																		// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																		// info[5]，# iterations，					总迭代次数
																		// info[6]，reason for terminating，		迭代结束原因：
																													// 1. 目标函数对优化参数导数太小
																													// 2. 改正量，即优化参数变化太小
																													// 3. 达到最大迭代次数
																													// 4. 残差相对变化太小
																													// 5. 残差太小
																													// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																													//	  definite normal equations matrix. Typically, this indicates a programming error in the
																													//    user-supplied Jacobian.
																													// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																													//    This signals a programming error in the user-supplied projection function func.
																		// info[7]，# function evaluations，		目标函数调用次数
																		// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																		// info[9]，# number of linear systems solved，求解线性方程组的个数
									 );

// iteratively run sba multiple times, until no outliers are detected
int optim_sba_levmar_XYZ_ext_rotvec_iterative(vector<CloudPoint> & cloudPts,		// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
											  vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
											  const vector<vector<Point2i>> & allTracks,
											  int idx_refimg,					// input:	the reference image, whose R=I, t=[0,0,0]'
											  int itermax = 1024,				// 输入：最大迭代次数
											  double thresh_rpjErr = 1,			// input:	the threshold to determine outliers
											  double * opts = NULL,				
											  double * info = NULL
											  );
}