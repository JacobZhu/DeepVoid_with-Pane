/************************************************************************************/
/* Johnny Zhu's own library, calibration and 3d reconstruction related algorithms.  */
/* Author: Johnny Zhu (DeepVoid) in Melbourne                        Date:2014.03.03*/
/************************************************************************************/

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

namespace DeepVoid
{

// 根据提供的绕X轴顺时针旋转的角度（角度表示非弧度）得到4×4的旋转矩阵Rx
CMatrix GenRX(double angle);

// 根据提供的绕X轴顺时针旋转的弧度得到4×4的旋转矩阵Rx
CMatrix GenRX_Radian(double radian);
// 20150114
Matx33d GenRX_Radian_CV(double radian);

// 根据提供的绕Y轴顺时针旋转的角度（角度表示非弧度）得到4×4的旋转矩阵Ry
CMatrix GenRY(double angle);

// 根据提供的绕Y轴顺时针旋转的弧度得到4×4的旋转矩阵Ry
CMatrix GenRY_Radian(double radian);
// 20150114
Matx33d GenRY_Radian_CV(double radian);

// 根据提供的绕Z轴顺时针旋转的角度（角度表示非弧度）得到4×4的旋转矩阵Rz
CMatrix GenRZ(double angle);

// 根据提供的绕Z轴顺时针旋转的弧度得到4×4的旋转矩阵Rz
CMatrix GenRZ_Radian(double radian);
// 20150114
Matx33d GenRZ_Radian_CV(double radian);

// 根据提供的世界坐标系原点在像机坐标系中坐标t生成4×4的平移矩阵T
CMatrix GenT(double X, double Y, double Z);

// 根据提供的绕 XYZ 三轴旋转的角度（角度表示非弧度）以及旋转顺序来确定4×4的旋转矩阵R
CMatrix GenR_Euler(double angleX, double angleY, double angleZ, RotationOrder order = XYZ);

// 根据提供的绕 XYZ 三轴旋转的弧度以及旋转顺序来确定 4×4 的旋转矩阵 R
CMatrix GenR_Euler_Radian(double radianX, double radianY, double radianZ, RotationOrder order = XYZ);
// 20150114
Matx33d GenR_Euler_Radian_CV(double radianX, double radianY, double radianZ, RotationOrder order = XYZ);

// 根据输入的旋转向量（即旋转弧度和旋转轴单位向量的乘积），利用Rodrigues旋转公式生成3×3旋转矩阵:R = Icosθ + [n]sinθ + nn'(1 - cosθ)
// 或者R = I + [n]sinθ + [n]^2(1 - cosθ)
CMatrix GenR_Rodrigues(const CMatrix & mRotVec);

// 根据输入的旋转向量（即旋转弧度和旋转轴单位向量的乘积），利用Rodrigues旋转公式生成3×3旋转矩阵:R = Icosθ + [n]sinθ + nn'(1 - cosθ)
// 或者R = I + [n]sinθ + [n]^2(1 - cosθ)
CMatrix GenR_Rodrigues(double rv1, double rv2, double rv3);

// 从旋转矩阵中分解出αβ和γ三个角度值，分别为偏航角、俯仰角和滚转角，先绕Z轴旋转形成偏航角，再绕Y轴旋转形成俯仰角，最后绕X轴旋转形成滚转角，形成的旋转矩阵为R = RxRyRz
// X轴定义为目标轴向，Y轴定义为目标水平径向，Z轴定义为目标垂直径向
CMatrix GetEulerAngleFromR(const CMatrix & mR);

// 根据输入的旋转矩阵，返回一个 3 维的向量，其为 θ|v|，其中 v 为旋转轴的单位向量，θ 为旋转弧度
CMatrix GetRotVecFromR(const CMatrix & mR);

// 根据输入的旋转矩阵，返回一个 3 维的向量，其为 θ|v|，其中 v 为旋转轴的单位向量，θ 为旋转弧度
Matx31d GetRotVecFromR(const Matx33d & mR);

// 已知不共线的三个点的世界坐标系坐标，利用这三个点建立一个局部坐标系，并输出这一局部坐标系与世界坐标系之间的旋转平移矩阵Hlw
// pt1->pt2的向量方向为x轴方向，(pt1->pt2)×(pt1->pt3)为z轴方向，(pt1->pt2)×(pt1->pt3)×(pt1->pt2)为y轴方向
CMatrix BuildCoordSysBy3Pt(CMatrix mWrdPt);

// 根据输入的各目的投影矩阵，以及各目中目标的像点坐标对目标进行多目交会
CMatrix Intersect(const CMatrix & mImgPt, const CMatrix & mProjArray);

// 已知像机的投影矩阵mP，计算空间点 4×n的矩阵mWrdPt(X, Y, Z, 1) 投影在像面上的理想像点 3×n的矩阵mImgPt(x, y, 1)，其中n为空间点个数，也即图像点个数
CMatrix Proj2IdealImgPt_3Dto2D(const CMatrix & mWrdPt, const CMatrix & mP);

// 为了完全仿真真实的像机成像，引入视景体（视锥体）的概念，在视景体外面的空间点不参与投影，直接赋值成某一不合理像点坐标；在视景体内部的空间点按照正常的透视模型进行成像投影
CMatrix Proj2IdealImgPt_3Dto2D_VisualCone(const CMatrix & mWrdPts, // 4×n的矩阵，控制点齐次坐标矩阵
										  double imgWidth,         // 仿真像机的像面宽度
										  double imgHeight,        // 仿真像机的像面高度
										  const CMatrix & mK,      // 3×4的像机内参数矩阵
										  const CMatrix & mRT);    // 4×4的像机外参数矩阵

double GetAngleBetweenOpticalAxes_Radian(Matx33d mR);
double GetAngleBetweenOpticalAxes_Angle(Matx33d mR);

// return the computed XYZ given the depth
Matx31d GetXYZ_givenDepth(const Matx33d & mK,					// input:	3*3, camera matrix
						  const Matx33d & mR,					// input:	3*3, rotation matrix
					      const Matx31d & mt,					// input:	3*1, translation vector
						  double img_x, double img_y,			// input:	the distortion free image coordinates of the point
						  double depth							// input:	the depth of this point relative to this camera
						  );

// return the computed XYZ given the depth
Matx31d GetXYZ_givenDepth(const Matx33d & mR,					// input:	3*3, rotation matrix
					      const Matx31d & mt,					// input:	3*1, translation vector
						  double nimgx, double nimgy,			// input:	the distortion free normalized image coordinates of the point
						  double depth							// input:	the depth of this point relative to this camera
						  );


// this function first match features by feature descriptors
// then use epipolar geometry based RANSAC to compute fundamental matrix
// and output refined matches with outliers discarded
// Mat CalibFundamentalMat_RANSAC_Features(const Features & feat1,		// input:	n1 features extracted from the 1st image
// 									    const Features & feat2,		// input:	n2 features extracted from the 2nd image
// 									    vector<DMatch> & matches,	// output:	matches obtained after feature matching and RANSAC
// 									    double param1 = 3,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
// 									    double param2 = 3,			// input:	the distance threshold between point and epiline, used in RANSAC stage
// 									    double param3 = 0.99		// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
// 									    );

// this function first match features by feature descriptors
// then use epipolar geometry based RANSAC to compute fundamental matrix
// and output refined matches with outliers discarded
Matx33d CalibFundamentalMat_RANSAC_Features(const Features & feat1,		// input:	n1 features extracted from the 1st image
											const Features & feat2,		// input:	n2 features extracted from the 2nd image
											vector<DMatch> & matches,	// output:	matches obtained after feature matching and RANSAC
											double param1 = 3.,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
											double param2 = 3.,			// input:	the distance threshold between point and epiline, used in RANSAC stage
											double param3 = 0.99		// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
											);

// this function first match features by feature descriptors
// then use epipolar geometry based RANSAC to compute fundamental matrix
// and output refined matches with outliers discarded
Matx33d CalibFundamentalMat_RANSAC_Features_idxImgPt(const Features & feat1,		// input:	n1 features extracted from the 1st image
													 const Features & feat2,		// input:	n2 features extracted from the 2nd image
													 vector<DMatch> & matches,	// output:	matches obtained after feature matching and RANSAC
													 double param1 = 3.,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
													 double param2 = 3.,			// input:	the distance threshold between point and epiline, used in RANSAC stage
													 double param3 = 0.99		// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
													 );

// match features in two input images based on RANSAC, coordinates of input features are supposed to be distortion free
// and each feature in one image may be matched with more than one feature in the other image
Matx33d CalibFundamentalMat_RANSAC_Features_Original(const Features & feat1,		// input:	n1 features extracted from the 1st image
													 const Features & feat2,		// input:	n2 features extracted from the 2nd image
													 vector<DMatch> & matches,	// output:	matches obtained after feature matching and RANSAC
													 double param1 = 3.,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
													 double param2 = 3.,			// input:	the distance threshold between point and epiline, used in RANSAC stage
													 double param3 = 0.99		// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
													 );

// 20150113, zhaokunz
// 1. get initial matches based on descriptors
// 2. refine matches and get initial fundamental matrix using RANSAC
// 3. optimize fundamental matrix using only inliers
// 4. augment inlier set using optimized fundamental matrix
void Get_F_Matches(const Features & feats0,		// input:	n1 features extracted from the 1st image
				   const Features & feats1,		// input:	n2 features extracted from the 2nd image
				   Matx33d & mF,				// output:	the estimated fundamental matrix
				   vector<DMatch> & matches,	// output:	matches obtained after feature matching and RANSAC
				   double thresh_p2l = 3.,		// input:	the distance threshold between point and epiline, used in RANSAC stage
				   double thresh_conf = 0.99,	// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
				   int maxIter = 10,			// input:	the maximum number of iterations
				   double xEps = 1.0E-8,		// input:	threshold
				   double fEps = 1.0E-6			// input:	threshold
				   );

// 20150128, zhaokunz
// 1. get initial matches based on descriptors
// 2. refine matches and get initial fundamental matrix using RANSAC
// 3. optimize fundamental matrix using only inliers
// 4. augment inlier set using optimized fundamental matrix
bool Get_F_Matches_knn(const Features & feats0,				// input:	n1 features extracted from the 1st image
					   const Features & feats1,				// input:	n2 features extracted from the 2nd image
					   Matx33d & mF,						// output:	the estimated fundamental matrix
					   vector<DMatch> & matches,			// output:	matches obtained after feature matching and RANSAC
					   bool bOptim = true,					// input:	whether optimize F using Golden Standard algorithm or not
					   double thresh_ratioTest = 0.3,		// input:	the ratio threshold for ratio test
					   double thresh_minInlierRatio = 0.5,	// input:	the allowed minimum ratio of inliers
					   double thresh_p2l = 3.,				// input:	the distance threshold between point and epiline, used in RANSAC stage
					   double thresh_conf = 0.99,			// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
					   int maxIter = 10,					// input:	the maximum number of iterations
					   double xEps = 1.0E-8,				// input:	threshold
					   double fEps = 1.0E-6					// input:	threshold
					   );

// 20150128, zhaokunz, output those matches that pass the ratio test
void ratioTest(const vector<vector<DMatch>> & matches_knn,	// input:	knn matches
			   vector<DMatch> & matches,						// output:	matches that have past the ratio test
			   double ratio = 0.3							// input:	the maximum allowed ratio between the best match and 2nd best match
			   );

// 20150129, zhaokunz, output those matches that pass the symmetry test
void symmetryTest(const vector<DMatch> & matches01,		// input:	matches generated from 0->1
				  const vector<DMatch> & matches10,		// input:	matches generated from 1->0
				  vector<DMatch> & matches				// output:	
				  );

// first, feature matching, then compute fundamental matrix and filter outlier matches using RANSAC
// once we get fundamental matrix, given intrinsic parameters we can get initial essential matrix,
// which includes 4 possible relative orientation [R|t] between image 1 and 2, then we find the
// true one using method described in <Multiple View Geometry> by Richard Hartley, this function
// do not do bundle adjustment after [R|t] obtained, only the linear solution
// this function returns the final fundamental 3*3 matrix
// Mat RelativeOrientation_RANSAC_Features_Linear(const cam_data & cam1,		// input:	all the information about the image 1
// 											   const cam_data & cam2,		// input:	all the information about the image 2
// 											   Mat & mP,					// output:	3*4 matrix, the relative orientation of these two images [R|t]
// 											   vector<CloudPoint> & clouds,	// output:	the reconstructed cloud points in reference camera frame, which is the first image
// 											   double param1 = 3,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
// 											   double param2 = 3,			// input:	the distance threshold between point and epiline, used in RANSAC stage
// 											   double param3 = 0.99			// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
// 											   );

// first, feature matching, then compute fundamental matrix and filter outlier matches using RANSAC
// once we get fundamental matrix, given intrinsic parameters we can get initial essential matrix,
// which includes 4 possible relative orientation [R|t] between image 1 and 2, then we find the
// true one using method described in <Multiple View Geometry> by Richard Hartley, this function
// do not do bundle adjustment after [R|t] obtained, only the linear solution
// this function returns the final fundamental 3*3 matrix
CMatrix RelativeOrientation_RANSAC_Features_Linear(const cam_data & cam1,		// input:	all the information about the image 1
												   const cam_data & cam2,		// input:	all the information about the image 2
												   CMatrix & mP,				// output:	3*4 matrix, the relative orientation of these two images [R|t]
												   vector<CloudPoint> & clouds,	// output:	the reconstructed cloud points in reference camera frame, which is the first image
												   double param1 = 3,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
												   double param2 = 3,			// input:	the distance threshold between point and epiline, used in RANSAC stage
												   double param3 = 0.99			// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
												   );

// given a essential matrix, find the true one using method described in <Multiple View Geometry> by Richard Hartley
// void DisambiguateRT_givenE(const vector<Point2f> & imgPts1,					// input:	normalized image coordinates
// 						   const vector<Point2f> & imgPts2,					// input:	normalized image coordinates
// 						   const Mat & mE,									// input:	the given essential matrix
// 						   Mat & mP,										// output:	3*4 matrix, the relative orientation of these two images [R|t]
// 						   vector<Point3d> & wrdPts							// output:	the reconstructed 3d points in reference camera frame, which is the first image
// 						   );

// 利用 8 以上归一化像点对应来通过《计算机视觉中的多视图几何》一书 p193 页中的归一化 8 点算法10.1求解两视图之间的本质矩阵，返回 3×3 的两视图之间的本质矩阵
void DisambiguateRT_givenE(const CMatrix & mImgPts1,        // input:	3*n or 2*n matrix, normalized image coordinates
						   const CMatrix & mImgPts2,        // input:	3*n or 2*n matrix, normalized image coordinates
						   const CMatrix & mE,				// input:	3*3 matrix, the given essential matrix
						   CMatrix & mP,					// output:	3*4 matrix, the relative orientation of these two images [R|t]
						   CMatrix & mWrdPts				// output:	4*n matrix, the reconstructed 3d points in reference camera frame, which is the first image
						   );

// Remove D.C.Brown image distortion 
void RemoveDistortion_DCBrown(double Cx, double Cy,								// input：pricipal point
							  double Fx, double Fy,								// input：equvalent focal length
							  const double * k,									// input: D.C.Brown's distortion
							  double distorted_Ix, double distorted_Iy,			// input：distorted coordinates
							  double &ideal_Ix, double &ideal_Iy,				// output:coordinates with distortion removed
							  int maxIteration = 32,							// input：maximal iterations
							  double eps = 1e-10								// input：conditions to quit iteration
							  );

// remove D.C.Brown distortions of all feature points
void RemoveDistortion_DCBrown(cam_data & cam);

// calculate the distorted image coordinates of corresponding ideal ones
void CalcuDistedImgPt_DCBrown(double Cx, double Cy,							// input：pricipal point
							  double Fx, double Fy,							// input：equvalent focal length
							  const double * k,								// input：D.C.Brown's distortion
																			// k1：2nd order radial distortion r^2
																			// k2：4th order radial distortion r^4
																			// k3：tangential distortion
																			// k4：tangential distortion
																			// k5：6th order radial distortion r^6
							  double ideal_Ix, double ideal_Iy,				// input：ideal image coordinates
							  double &distorted_Ix, double &distorted_Iy	// output：distorted image coordinates
							  );

// 采用 D.C.Brown 的 5 系数像差模型，和 weng 的 5 系数像差模型有所不同，输出为 3×n 的齐次像点坐标
CMatrix CalcuDistedImgPt_DCBrown(const CMatrix & mImgPtIdeal,               // 输入：理想的像点坐标，可以为 3×n 的齐次像点坐标，也可以为 2×n 的非齐次像点坐标 
								 const CMatrix & mK,                        // 输入：像机内参数矩阵，可以为 3×4 的矩阵，也可为 3×3 的矩阵
								 const CMatrix & mDist                      // 输入：D.C.Brown 像差系数，5 维的向量
								 );

// 根据输入的像机内参数矩阵以及Weng's像差系数计算与每个输入理想像点对应的像差，并返回
CMatrix CalcuDistortion_Weng(const CMatrix & mImgPtIdeal, const CMatrix & mK, const CMatrix & mDist, double k6 = 0);

// 根据输入的像机内参数矩阵以及Weng's像差系数计算与每个输入理想像点对应的带像差像点，并返回 
// 输入输出皆为 3×n 的齐次像点坐标
CMatrix CalcuDistedImgPt_Weng(const CMatrix & mImgPtIdeal, const CMatrix & mK, const CMatrix & mDist);

bool ExteriorOrientation_PnP_RANSAC(vector<cam_data> & vCams,		// input&output:	all the images
									int idx_cam1,					// input:	the index of the first camera
									int idx_cam2,					// input:	the index of the second camera
									vector<CloudPoint> & clouds,	// input&output:	the reconstructed cloud points in reference camera frame, which is the first image	 
									double param1 = 3,				// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
									double param2 = 1,				// input:	the distance threshold between point and epiline, used in RANSAC stage
									double param3 = 0.99,			// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
									double param4 = 1,				// input:	the allowed level of reprojection error, used for RANSAC to determine outlier
									double param5 = 0.75			// input:	the allowed minimum ratio of inliers within all reconstructed matches
									);

bool ExteriorOrientation_PnP_RANSAC_Round(vector<cam_data> & vCams,		// input&output:	all the images
										  vector<int> & status,			// input&output:	if status[i] = 0, means that vCams[i] is still not oriented yet
										  int idx_cam,					// input:	the index of current image
										  vector<CloudPoint> & clouds,	// input&output:	the reconstructed cloud points in reference camera frame, which is the first image	 
										  double param1 = 3,			// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
										  double param2 = 1,			// input:	the distance threshold between point and epiline, used in RANSAC stage
										  double param3 = 0.99,			// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
										  double param4 = 1,			// input:	the allowed level of reprojection error, used for RANSAC to determine outlier
										  double param5 = 0.75			// input:	the allowed minimum ratio of inliers within all reconstructed matches
										  );

void ExteriorOrientation_PnP_RANSAC_All(vector<cam_data> & vCams,		// input&output:	all the images
										vector<int> & status,			// input&output:	if status[i] = 0, means that vCams[i] is still not oriented yet
										vector<CloudPoint> & clouds,	// input&output:	the reconstructed cloud points in reference camera frame, which is the first image	 
										double param1 = 3,				// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
										double param2 = 1,				// input:	the distance threshold between point and epiline, used in RANSAC stage
										double param3 = 0.99,			// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
										double param4 = 1,				// input:	the allowed level of reprojection error, used for RANSAC to determine outlier
										double param5 = 0.75			// input:	the allowed minimum ratio of inliers within all reconstructed matches
										);

// Photogrammetric Iterative Relative Orientation (PIRO) Related ////////////////////////////////////////////////////////////////////////

void der_f_PIRO_omg_fai(double nx1, double ny1,								// input: normalized image coordinates in reference image
						double nx2, double ny2,								// input: normalized image coordinates in the other image
						double omg, double fai,								// input: radians, current two angles representing the direction of the unit baseline vector
						double AX,  double AY, double AZ,					// input: radians, current relative rotation angles between reference image and the other image
						double & df_domg, double & df_dfai,					// output:current derivatives of omg and fai
						double & df_dAX, double & df_dAY, double & df_dAZ,	// output:current derivatives of AX, AY and AZ
						double & f											// output:current value of objective function
						);

void der_f_PIRO_Y_Z(double nx1, double ny1,								// input: normalized image coordinates in reference image
					double nx2, double ny2,								// input: normalized image coordinates in the other image
					double Y, double Z,									// input: current Y and Z coordinates of the other image's optical center
					double AX,  double AY, double AZ,					// input: radians, current relative rotation angles between reference image and the other image
					double & df_dY, double & df_dZ,						// output:current derivatives of Y and Z
					double & df_dAX, double & df_dAY, double & df_dAZ,	// output:current derivatives of AX, AY and AZ
					double & f,											// output:current value of objective function
					double & df_dnx1, double & df_dny1,					// output:current derivatives of nx1 and ny1
					double & df_dnx2, double & df_dny2,					// output:current derivatives of nx2 and ny2
					double bx = 1										// input: default fixed value for X coordinate of the other image's optical center in reference
					);

// 20150114, zhaokunz, output all derivatives and objective function
void der_f_PIRO(double nx1, double ny1,								// input: normalized image coordinates in reference image
				double nx2, double ny2,								// input: normalized image coordinates in the other image
				double X, double Y, double Z,						// input: current X, Y and Z coordinates of the other image's optical center
				double AX,  double AY, double AZ,					// input: radians, current relative rotation angles between reference image and the other image
				double & df_dX, double & df_dY, double & df_dZ,		// output:current derivatives of Y and Z
				double & df_dAX, double & df_dAY, double & df_dAZ,	// output:current derivatives of AX, AY and AZ
				double & f,											// output:current value of objective function
				double & df_dnx1, double & df_dny1,					// output:current derivatives of nx1 and ny1
				double & df_dnx2, double & df_dny2					// output:current derivatives of nx2 and ny2
				);

void get_omg_fai_fromXYZ(double X, double Y, double Z,		// input: the coordinates of optical center of the other image in reference image frame
						 double & omg, double & fai			// output:radians, the two angles corresponding to unit baseline vector
						 );

void get_XYZ_fromOmgFai(double omg, double fai,				// input: radians, the two angles corresponding to unit baseline vector
						double & X, double & Y, double & Z	// output:the coordinates of optical center of the other image in reference image frame
						);

void Optim_GaussNewton_PIRO_omg_fai_AXAYAZ(const CMatrix & mImgPts1,									// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on reference image
	                                       const CMatrix & mImgPts2,									// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on the other image
										   double omg_init, double fai_init,							// input: radians, initial values of omg and fai
										   double AX_init, double AY_init, double AZ_init,				// input: radians, initial values of AX,AY,AZ
										   double & omg_optim, double & fai_optim,						// output:radians, optimized values of omg and fai
										   double & AX_optim, double & AY_optim, double & AZ_optim,		// output:radians, optimized values of AX,AY,AZ
										   int maxIter = 128,											// input: max iteration
										   double xEps = 1.0E-12,										// input: threshold
										   double fEps = 1.0E-12,										// input: threshold
										   int * iterNum = NULL											// output:iteration number when quiting
										   );

void Optim_GaussNewton_PIRO_omg_fai_AXAYAZ(const CMatrix & mImgPts1,									// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on reference image
	                                       const CMatrix & mImgPts2,									// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on the other image
										   CMatrix & mRT,												// input&output: 4*4 matrix, the initial relative orientation and the optimized one
										   int maxIter = 128,											// input: max iteration
										   double xEps = 1.0E-12,										// input: threshold
										   double fEps = 1.0E-12,										// input: threshold
										   int * iterNum = NULL											// output:iteration number when quiting
										   );

void PIRO_Y_Z_AXAYAZ_GaussNewton(const CMatrix & mImgPts1,											// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on reference image
	                                   const CMatrix & mImgPts2,									// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on the other image
									   double Y_init, double Z_init,								// input: initial values of Y and Z
									   double AX_init, double AY_init, double AZ_init,				// input: radians, initial values of AX,AY,AZ
									   double & Y_optim, double & Z_optim,							// output:optimized values of Y and Z
									   double & AX_optim, double & AY_optim, double & AZ_optim,		// output:radians, optimized values of AX,AY,AZ
									   double sigma_nxny,											// input: the standard deviation of normalized image coordinates, sigma_xy/fx
									   double & Y_stdev, double & Z_stdev,							// output:optimized values of Y and Z
									   double & AX_stdev, double & AY_stdev, double & AZ_stdev,		// output:radians, optimized values of AX,AY,AZ
									   int maxIter = 128,											// input: max iteration
									   double xEps = 1.0E-12,										// input: threshold
									   double fEps = 1.0E-12,										// input: threshold
									   double bx = 1,												// input: default fixed value for X coordinate of the other image's optical center in reference
									   int * iterNum = NULL											// output:iteration number when quiting
									   );

void PIRO_Y_Z_AXAYAZ_GaussNewton(const CMatrix & mImgPts1,											// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on reference image
	                                   const CMatrix & mImgPts2,									// input: 2*n or 3*n matrix, the distortion free normalized image coordinates on the other image
									   CMatrix & mRT,												// output:4*4 matrix, the output relative orientation
									   double sigma_nxny,											// input: the standard deviation of normalized image coordinates, sigma_xy/fx
									   double & Y_stdev, double & Z_stdev,							// output:optimized values of Y and Z
									   double & AX_stdev, double & AY_stdev, double & AZ_stdev,		// output:radians, optimized values of AX,AY,AZ
									   int maxIter = 128,											// input: max iteration
									   double xEps = 1.0E-12,										// input: threshold
									   double fEps = 1.0E-12,										// input: threshold
									   double bx = 1,												// input: default fixed value for X coordinate of the other image's optical center in reference
									   int * iterNum = NULL											// output:iteration number when quiting
									   );

// 20150114, zhaokunz, photogrammetric iterative relative orientation
// fix the X coordinate of the optical center of the matching image to given value, then optimize Y and Z
void PIRO_Y_Z_AXAYAZ_GN(const vector<Point2d> & nimgpts0,							// input:	distortion free normalized image points in the reference image
						const vector<Point2d> & nimgpts1,							// input:	distortion free normalized image points in the matching image
						double Y_init, double Z_init,								// input:	initial values of Y and Z
						double AX_init, double AY_init, double AZ_init,				// input:	radians, initial values of AX,AY,AZ
						double & Y_optim, double & Z_optim,							// output:	optimized values of Y and Z
						double & AX_optim, double & AY_optim, double & AZ_optim,	// output:	radians, optimized values of AX,AY,AZ
						double bx = 1.0,											// input:	default fixed value for X coordinate of the matching image's optical center in reference
						int maxIter = 128,											// input:	max iteration
						double xEps = 1.0E-12,										// input:	threshold
						double fEps = 1.0E-12										// input:	threshold
					    );

// 20150114, zhaokunz, photogrammetric iterative relative orientation
// fix the Y coordinate of the optical center of the matching image to given value, then optimize X and Z
void PIRO_X_Z_AXAYAZ_GN(const vector<Point2d> & nimgpts0,							// input:	distortion free normalized image points in the reference image
						const vector<Point2d> & nimgpts1,							// input:	distortion free normalized image points in the matching image
						double X_init, double Z_init,								// input:	initial values of X and Z
						double AX_init, double AY_init, double AZ_init,				// input:	radians, initial values of AX,AY,AZ
						double & X_optim, double & Z_optim,							// output:	optimized values of X and Z
						double & AX_optim, double & AY_optim, double & AZ_optim,	// output:	radians, optimized values of AX,AY,AZ
						double by = 1.0,											// input:	default fixed value for Y coordinate of the matching image's optical center in reference
						int maxIter = 128,											// input:	max iteration
						double xEps = 1.0E-12,										// input:	threshold
						double fEps = 1.0E-12										// input:	threshold
					    );

// 20150115, zhaokunz, photogrammetric iterative relative orientation
// fix the Z coordinate of the optical center of the matching image to given value, then optimize X and Y
void PIRO_X_Y_AXAYAZ_GN(const vector<Point2d> & nimgpts0,							// input:	distortion free normalized image points in the reference image
						const vector<Point2d> & nimgpts1,							// input:	distortion free normalized image points in the matching image
						double X_init, double Y_init,								// input:	initial values of X and Y
						double AX_init, double AY_init, double AZ_init,				// input:	radians, initial values of AX,AY,AZ
						double & X_optim, double & Y_optim,							// output:	optimized values of X and Y
						double & AX_optim, double & AY_optim, double & AZ_optim,	// output:	radians, optimized values of AX,AY,AZ
						double bz = 1.0,											// input:	default fixed value for Z coordinate of the matching image's optical center in reference
						int maxIter = 128,											// input:	max iteration
						double xEps = 1.0E-12,										// input:	threshold
						double fEps = 1.0E-12										// input:	threshold
					    );

// 20150114, zhaokunz
double PIRO_GN(const vector<Point2d> & imgpts0,	// input:	measured distortion free image points in reference image
			   const vector<Point2d> & imgpts1,	// input:	measured distortion free image points in matching image
			   const Matx33d & mK0,				// input:	the calibration matrix of the reference image
			   const Matx33d & mK1,				// input:	the calibration matrix of the matching image
			   Matx33d & mR,					// output:	the relative rotation matrix between the matching and reference image
			   Matx31d & mt,					// output:	the relative translation vector between the matching and reference image
			   vector<Point3d> & wrdpts,		// output:	the reconstructed 3D coordinates of all correspondences
			   int maxIter = 128,				// input:	max iteration
			   double xEps = 1.0E-12,			// input:	threshold
			   double fEps = 1.0E-12			// input:	threshold
			   );

bool RelativeOrientation_RANSAC_Features_PIRO(const cam_data & cam1,			// input:	all the information about the image 1
											  const cam_data & cam2,			// input:	all the information about the image 2
											  int idx_cam1,						// input:	the index of the first camera
											  int idx_cam2,						// input:	the index of the second camera
											  CMatrix & mRT,					// output:	4*4 matrix, the relative orientation of these two images [R|t; 0 0 0 1]
											  vector<CloudPoint> & clouds,		// output:	the reconstructed cloud points in reference camera frame, which is the first image	 
											  double param1 = 3,				// input:	defining "good" matches (i.e. whose distance is less than param1*min_dist) in feature matching stage
											  double param2 = 3,				// input:	the distance threshold between point and epiline, used in RANSAC stage
											  double param3 = 0.99,				// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
											  double thresh_stdev_YZ = 0.08,	// input:	the threshold of standard deviation of Y and Z
											  double thresh_reprojErr = 1,		// input:	the threshold of the reprojection error in pixels
											  double thresh_pyErr = 0.001,		// input:	the threshold of the y-parallax error
											  double thresh_ang = 30			// input:	the threshold of angle between optical axes
											  );

// this PIRO func conduct ro with given matches
// and the feature points in both cam_data are supposed to be distortion free
bool RelativeOrientation_RANSAC_Features_PIRO_givenMatches(const cam_data & cam1,			// input:	all the information about the image 1
														   const cam_data & cam2,			// input:	all the information about the image 2
														   int idx_cam1,					// input:	the index of the first camera
														   int idx_cam2,					// input:	the index of the second camera
														   const vector<DMatch> & matches,	// input:	the given matches
														   CMatrix & mRT,					// output:	4*4 matrix, the relative orientation of these two images [R|t; 0 0 0 1]
														   vector<CloudPoint> & clouds,		// output:	the reconstructed cloud points in reference camera frame, which is the first image	
														   double thresh_stdev_YZ = 0.08,	// input:	the threshold of standard deviation of Y and Z
														   double thresh_reprojErr = 1,		// input:	the threshold of the reprojection error in pixels
														   double thresh_pyErr = 0.001,		// input:	the threshold of the y-parallax error
														   double thresh_ang = 30			// input:	the threshold of angle between optical axes
														   );

// 20150115, zhaokunz, this PIRO func conduct ro with given matches
// and the feature points in both cam_data are supposed to be distortion free
bool RelativeOrientation_Features_PIRO_givenMatches(const cam_data & cam1,			// input:	all the information about the image 1
													const cam_data & cam2,			// input:	all the information about the image 2
													int idx_cam1,					// input:	the index of the first camera
													int idx_cam2,					// input:	the index of the second camera
													const vector<DMatch> & matches,	// input:	the given matches
													CMatrix & mRT,					// output:	4*4 matrix, the relative orientation of these two images [R|t; 0 0 0 1]
													vector<CloudPoint> & clouds,	// output:	the reconstructed cloud points in reference camera frame, which is the first image
													double thresh_reprojErr = 1		// input:	the threshold of the reprojection error in pixels
													);

void Triangulate_PIRO_py(double nx1, double ny1, double nz1,	// input:	the normalized image coordinates in reference image
						 double nx2, double ny2, double nz2,	// input:	the normalized image coordinates in the other image
						 const CMatrix & mRT,					// input:	the relative orientation
						 double & X, double & Y, double & Z,	// output:	the reconstructed 3d coordinates
						 double & py							// output:	the y-parallax
						 );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*--------------------------------------------------------------*/
/* 求非线性优化各种目标函数相关的导数，即Jacobian matrix        */

// 图像重投影误差的目标函数 fi = ((xi - xi')^2 + (yi - yi')^2)^0.5，其中 i 表示第 i 个控制点
// dfi/dx = (xi - xi')/fi   dfi/dy = (yi - yi')/fi，函数输入n个控制点的话，函数就输入一个n×2的Jacobian matrix
// 其中每一行为(dfi/dx	dfi/dy)
// mImgPtReproj和mImgPtReal保证尺寸一致即可，齐次或非齐次都可，只要尺寸一致
CMatrix Jacobian_Fi_xy(const CMatrix & mImgPtReproj, const CMatrix & mImgPtReal);

// 归一化的像点坐标(nx, ny)对旋转矩阵每个元素ri，以及平移向量 (tx, ty, tz) 总共12个元素进行求导，输出 2×12 的Jacobian matrix
// mRT为4×4的矩阵，mWrdPt为一维列向量
CMatrix Jacobian_nxny_RT(const CMatrix & mRT, const CMatrix & mWrdPt);

// 正常像点坐标(x, y)对旋转矩阵每个元素ri，以及平移向量 (tx, ty, tz) 总共12个元素进行求导，输出 2×12 的Jacobian matrix
// mRT为4×4的矩阵，mWrdPt为一维列向量，mK为像机内参数矩阵3×3或3×4的皆可
CMatrix Jacobian_xy_RT(const CMatrix & mRT, const CMatrix & mK, const CMatrix & mWrdPt);

// 正常像点坐标(x, y)对旋转矩阵每个元素ri，以及平移向量 (tx, ty, tz) 总共12个元素进行求导，输出 2×12 的Jacobian matrix
CMatrix Jacobian_xy_RT(const CMatrix & mRT,        // 输入： 4×4 的像机外参数矩阵
					   const CMatrix & mK,         // 输入： 3×4 或者 3×3 的像机内参数矩阵
					   double X,                   // 输入： 该点的齐次空间坐标
					   double Y,                   // 输入： 该点的齐次空间坐标
					   double Z,                   // 输入： 该点的齐次空间坐标
					   double W                    // 输入： 该点的齐次空间坐标
					   );

// 正常像点坐标(x, y)对内参数fx, fy, cx, cy进行求导，输出2×4的Jacobian matrix
// mRT为4×4的矩阵，mWrdPt为一维列向量
CMatrix Jacobian_xy_fxfycxcy(const CMatrix & mRT, const CMatrix & mWrdPt);

// Weng's像差(du, dv)对归一化像点坐标(nx, ny)进行求导，输出2×2的Jacobian matrix
// mImgPtNorm为一归一化像点坐标，mK为内参数矩阵，3×3或3×4皆可，mDist为Weng's像差系数
CMatrix Jacobian_dudv_xyNorm_Weng(const CMatrix & mImgPtNorm, const CMatrix & mK, const CMatrix & mDist);

// Weng's像差(du, dv)对理想像点坐标(x, y)进行求导，输出2×2的Jacobian matrix
// mImgPt为一理想像点坐标，mK为内参数矩阵，3×3或3×4皆可，mDist为Weng's像差系数
CMatrix Jacobian_dudv_xy_Weng(const CMatrix & mImgPt, const CMatrix & mK, const CMatrix & mDist);

// 带像差的重投影像点坐标对理想像点坐标(x, y)进行求导，输出2×2的Jacobian matrix
// mImgPt为一理想像点坐标，mK为内参数矩阵，3×3或3×4皆可，mDist为Weng's像差系数
CMatrix Jacobian_distxy_xy_Weng(const CMatrix & mImgPt, const CMatrix & mK, const CMatrix & mDist);

// 带像差的重投影像点坐标对5个weng's像差系数进行求导，输出2×5的Jacobian matrix
// mImgPt为线性重投影的理想像点坐标，mK为内参数矩阵，3×3或3×4皆可
CMatrix Jacobian_distxy_ki_Weng(const CMatrix & mImgPt, const CMatrix & mK);

// 外参矩阵的12个元素对6个外参数(yaw, pitch, roll, tx, ty, tz)进行求导，输出12×6的Jacobian matrix
// mExtPara为6维的列向量
CMatrix Jacobian_RT_ExtPara(const CMatrix & mExtPara);

// 利用消隐线优化像机相对姿态角时，3个目标函数分别对旋转矩阵各元素求导生成的雅各比矩阵
// 输入 mNorm 应为 1 单位向量
CMatrix Jacobian_Fi_R_VanishingLineOptimAngles(const CMatrix & mNorm);

// 利用 2 个以上的像机对 单个 物点空间坐标进行光束法平差时，每个像机上该物点的理想投影点对其空间坐标 (X, Y, Z) 分别求偏导形成的 Jacobian Matrix
// 因此若参与该 单个 物点空间坐标平差的目数为 n ，则输出即为一 2n×3 的 Jacobian Matrix
CMatrix Jacobian_xy_XYZ(double X,                                 // 输入： 待优化的该物点空间坐标初值
					    double Y,                                 // 输入： 待优化的该物点空间坐标初值
						double Z,                                 // 输入： 待优化的该物点空间坐标初值
						const CMatrix & mKs,                      // 输入： 参与光束法平差优化物点空间坐标的各像机内参数矩阵，至少需要 2 个以上的像机参与该物点的光束法平差，每个像机的内参数矩阵皆应为 3×4 的矩阵，因此若有 n 个像机，则该输入应为 3×4n 的矩阵
						const CMatrix & mRTs                      // 输入： 参与光束法平差优化物点空间坐标的各像机外参数矩阵，至少需要 2 个以上的像机参与该物点的光束法平差，每个像机的外参数矩阵皆应为 4×4 的矩阵，因此若有 n 个像机，则该输入应为 4×4n 的矩阵
						);

// 投影像点坐标对对应的空间点世界坐标求导
Matx23d Jacobian_xy_XYZ(const Matx33d & mKR,			
						const Matx31d & mKt,
						double X, double Y, double Z,
						double & imgx, double & imgy
						);

// zhaokunz, 20150108, 投影像点坐标对对应的空间点世界坐标求导
Matx23d Jacobian_xy_XYZ(const Matx34d & mP,
						double X, double Y, double Z
						);

// zhaokunz, 20150108, 投影像点坐标对camera matrix求导
Matx<double,2,12> Jacobian_xy_P(const Matx34d & mP,
							    double X, double Y, double Z
							    );

// 理想像点坐标对空间点的齐次空间坐标求导的 Jacobian Matrix，返回 2×4 的 Jacobian Matrix
CMatrix Jacobian_xy_XYZW(double X,                                // 输入： 该空间点当前的空间位置
						 double Y,                                // 输入： 该空间点当前的空间位置
						 double Z,                                // 输入： 该空间点当前的空间位置
						 double W,                                // 输入： 该空间点当前的空间位置
						 const CMatrix & mK,                      // 输入： 像机的内参数矩阵，3×4 或者 3×3 的矩阵皆可
						 const CMatrix & mRT                      // 输入： 像机的外参数矩阵，4×4 的矩阵
						 );

// D.C.Brown's 像差 (du, dv) 对归一化像点坐标 (nx, ny) 进行求导，输出 2×2 的 Jacobian matrix
CMatrix Jacobian_dudv_xyNorm_DCBrown(double x_n,                  // 输入： 该像点的归一化像点坐标
									 double y_n,                  // 输入： 该像点的归一化像点坐标
									 const CMatrix & mK,          // 输入： 像机的内参数矩阵 3×4 或 3×3 的矩阵皆可
									 const CMatrix & mDist        // 输入： 像机的 5 系数 D.C.Brown 像差系数
									 );

// D.C.Brown's 像差 (du, dv) 对物点的线性重投影的理想像点 (x, y) 进行求导,输出 2×2 的 Jacobian matrix
CMatrix Jacobian_dudv_xy_DCBrown(double x,                        // 输入： 线性重投影的理想像点坐标
								 double y,                        // 输入： 线性重投影的理想像点坐标
								 const CMatrix & mK,              // 输入： 像机的内参数矩阵 3×4 或 3×3 的矩阵皆可
								 const CMatrix & mDist            // 输入： 像机的 5 系数 D.C.Brown 像差系数
								 );

// 带 D.C.Brown's 像差的重投影像点坐标对理想像点坐标 (x, y) 进行求导，输出 2×2 的 Jacobian matrix
CMatrix Jacobian_distxy_xy_DCBrown(double x,                      // 输入： 线性重投影的理想像点坐标
								   double y,                      // 输入： 线性重投影的理想像点坐标
								   const CMatrix & mK,            // 输入： 像机的内参数矩阵 3×4 或 3×3 的矩阵皆可
								   const CMatrix & mDist          // 输入： 像机的 5 系数 D.C.Brown 像差系数
								   );

// 带像差的重投影像点坐标对 5 个 D.C.Brown's 像差系数进行求导，输出2×5的Jacobian matrix
CMatrix Jacobian_distxy_ki_DCBrown(double x,                      // 输入： 线性重投影的理想像点坐标
								   double y,                      // 输入： 线性重投影的理想像点坐标
								   const CMatrix & mK             // 输入： 像机的内参数矩阵 3×4 或 3×3 的矩阵皆可
								   );

// 该函数是计算某点在坐标系 2 中的坐标对坐标系 1 和 2 刚体变换矩阵 H12 各元素求导的 Jacobian Matrix，X2 = H12 * X1，返回一 3×12 的矩阵
CMatrix Jacobian_XYZ_RT(double X,                                 // 输入： 该点在坐标系 1 中的坐标
						double Y,								  // 输入： 该点在坐标系 1 中的坐标
						double Z                                  // 输入： 该点在坐标系 1 中的坐标
						);

// 某点表征的三维点世界坐标对其沿视线方向的深度，也就是Zc坐标的导数
void Jacobian_XYZ_depth(const Matx33d & mR,					// input: rotation matrix
						double nimgx, double nimgy,			// input: normalized image points
						double & dX_dd,						// output: dX/dd
						double & dY_dd,						// output: dY/dd
						double & dZ_dd						// output: dZ/dd
						);

// support window中某像素表征的三维点世界坐标对该support window的dr hx hy求导
Matx33d Jacobian_XYZ_drhxhy(const Matx33d & mR,				// input:	rotation matrix
							double u, double v,				// input:	local offset coordinates of this pixel w.r.t central pixel
							double nimgx, double nimgy		// input:	normalized image points representing the direction
							);

// support window中某像素表征的三维点世界坐标对该support window的dr hx hy求导
Matx33d Jacobian_XYZ_drhxhy(const Matx31d & mRtx,			// input:	R'x(u,v), transpose of R * normalized image points of (u,v)
							double u, double v				// input:	local offset coordinates of this pixel w.r.t central pixel
							);

// 某刚体变换矩阵 RT 的逆 inv(RT) 对该矩阵的求导
CMatrix Jacobian_invRT_RT(const CMatrix & mRT);						// 输入： 当前的刚体变换矩阵								

// 旋转平移矩阵 RT 对旋转向量 v 的导数阵
// 其中 R = I + [n]sin(a) + [n]^2(1 - cos(a))，其中 n 为单位的旋转轴向量，a 为绕旋转轴旋转的角度
// 输出为 12×6 的 Jacobian Matrix
CMatrix Jacobian_RT_ExtPara_RotVec(const CMatrix & mRotVec);		// 输入： 当前像机的 3 维的旋转向量 v，|v| 为旋转角度大小，v 为旋转轴

// 当前的旋转平移矩阵 RT 对单位四元数的导数阵
// 输出为 12×7 的 Jacobian Matrix
CMatrix Jacobian_RT_ExtPara_Quaternion(const CMatrix & q);			// 输入： 当前像机旋转的 4 维单位四元数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Structure from Motion related //////////////////////////////////////////////////////////////////////////

void FindAllMatchingTracks_with121mappingcheck(vector<cam_data> & allCams,
											   const vector<vector<DMatch>> & allMatches,
											   vector<vector<Point2i>> & allTracks
											   );

void FindAllMatchingTracks(vector<cam_data> & allCams,
						   const vector<vector<DMatch>> & allMatches,
						   vector<vector<Point2i>> & allTracks
						   );

// return the index of the image observing the most reconstructed tracks
int FindImgObservingMostTracks_traversal(const vector<cam_data> & allCams,	// input:	all images
										 const vector<CloudPoint> & clouds,	// input:	all reconstructed tracks
										 const vector<int> & status,			// input:	status[i]=1 means that image i is not considered
										 int & nPts							// output:	the maximum number of tracks observed by single image 
										 );

// return the index of the image observing the most reconstructed tracks
int FindImgObservingMostTracks_binarySearch(const vector<cam_data> & allCams,	// input:	all images
											const vector<CloudPoint> & clouds,	// input:	all reconstructed tracks
											const vector<int> & status,			// input:	status[i]=1 means that image i is not considered
											int & nPts							// output:	the maximum number of tracks observed by single image 
											);

// return the index of the image observing the most reconstructed tracks
int FindImgObservingMostTracks_hashSet(const vector<cam_data> & allCams,	// input:	all images
									   const vector<CloudPoint> & clouds,	// input:	all reconstructed tracks
									   const vector<int> & status,			// input:	status[i]=1 means that image i is not considered
									   int & nPts							// output:	the maximum number of tracks observed by single image 
									   );

// 20150127, zhaokunz, return the index of the image observing the most reconstructed tracks
void GetReconstructedTrackNum(const vector<cam_data> & allCams,	// input:	all images
							  const vector<CloudPoint> & clouds,// input:	all reconstructed tracks
							  const vector<int> & status,		// input:	status[i]=1 means that image i is not considered
							  vector<Point2i> & pairs			// output:	the maximum number of tracks observed by single image 
							  );

// 20150127, zhaokunz, find image pair good for RO
void FindPairObservingMostCommonTracks(const vector<cam_data> & allCams,			// input:	all images
									   const vector<vector<Point2i>> & allTracks,	// input:	all feature tracks
									   vector<Point2i> & pairs						// output:	the found pairs in descendent ordering
									   );

// 20150203, zhaokunz, try to rank all other oriented images for dense matching with designated reference image based on certain measure
void RankMatchingImages(const vector<cam_data> & allCams,	// input:	all images
						const vector<CloudPoint> & clouds,	// input:	all reconstructed tracks
						int idxRefImg,						// input:	the index of the reference image
						vector<int> & matchingImgs			// output:	the indices of all other matching images, in descending ordering 
						);

// 20150204, zhaokunz, try to score all other oriented images for dense matching with designated reference image based on certain measure
void ScoreMatchingImages(const vector<cam_data> & allCams,			// input:	all images
						 const vector<CloudPoint> & clouds,			// input:	all reconstructed tracks
						 const vector<vector<DMatch>> & allMatches,	// input:	all pair-wise matches
						 int idxRefImg,								// input:	the index of the reference image
						 vector<Point2d> & scores,					// output:	scores of all other images, with x being the index, and y being the score, in descending order
						 double ang_desired = 15					// input:	the desired triangulation angle
						 );

// feature points in all images are supposed to be distortion free
bool ExteriorOrientation_PnP_RANSAC(vector<cam_data> & vCams,					// input&output:	all the images
									int idx_cam,								// input:	the index of the image to be oriented
									vector<CloudPoint> & clouds,				// input&output:	the reconstructed cloud points in reference camera frame, which is the first image
									const vector<vector<Point2i>> & allTracks,	// input:	all the tracks
									double thresh_rpj_inlier = 1,				// input:	the allowed level of reprojection error, used for RANSAC to determine outlier
									double thresh_ratio_inlier = 0.5			// input:	the allowed minimum ratio of inliers within all reconstructed matches
									);

// feature points in all images are supposed to be distortion free
// detach all the outliers found in PnP RANSAC with corresponding tracks 
bool ExteriorOrientation_PnP_RANSAC_detachOutliers(vector<cam_data> & vCams,					// input&output:	all the images
												   int idx_cam,								// input:	the index of the image to be oriented
												   vector<CloudPoint> & clouds,				// input&output:	the reconstructed cloud points in reference camera frame, which is the first image
												   const vector<vector<Point2i>> & allTracks,	// input:	all the tracks
												   double thresh_rpj_inlier = 1,				// input:	the allowed level of reprojection error, used for RANSAC to determine outlier
												   double thresh_ratio_inlier = 0.5			// input:	the allowed minimum ratio of inliers within all reconstructed matches
												   );

// 20150124, zhaokunz, feature points in all images are supposed to be distortion free
bool EO_PnP_RANSAC(vector<cam_data> & vCams,					// input&output:	all the images
				   int idx_refimg,								// input:	the reference image, whose R=I and t = [0,0,0]'
				   int idx_cam,									// input:	the index of the image to be oriented
				   vector<CloudPoint> & clouds,					// input&output:	the reconstructed cloud points in reference camera frame, which is the first image
				   const vector<vector<Point2i>> & allTracks,	// input:	all the tracks
				   double thresh_rpj_inlier = 1,				// input:	the allowed level of reprojection error, used for RANSAC to determine outlier
				   double thresh_ratio_inlier = 0.5				// input:	the allowed minimum ratio of inliers within all reconstructed matches
				   );

// 利用匹配路径中的所有点来进行交会得到其空间坐标，返回其 4 维的其次空间坐标
// feature points in all images are supposed to be distortion free
int Intersect_oneTrack(const vector<Point2i> & track,		// 输入：匹配路径，里面包含每个匹配点所在的视图索引号以及像点索引号
					   const vector<cam_data> & allCams,	// 输入：所有视图的所有信息
					   CMatrix & mWrdPt,					// output: the reconstructed point
					   double * reprojErr = NULL			// 输出：交会的点在参与交会各视图上的重投影残差的统计量				
					   );

void FillCloudPoints_RBG_RpjErr(const vector<cam_data> & vCams,	// input:	all image data
								vector<CloudPoint> & clouds		// input&output:	all the existing cloud points
								);

double MeanMinDistance_3D(const vector<CloudPoint> & clouds);

void Clustering_3D_byDistance(const vector<CloudPoint> & clouds,	// input:	cloud points
							  vector<vector<int>> & clusters,		// output:	all clusters
							  double thresh_distance				// input:	the distance threshold, if distance between two 3d points is smaller than this threshold, they belong to the same cluster
							  );

void FindAllGoodImagePairs(const vector<cam_data> & vCams,
						   vector<Point2i> & pairs
						   );

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitRndField(const Matx33d & mK,				// input:	the camera matrix
	              const Matx33d & mR,				// input:	the rotation matrix
				  const Matx31d & mt,				// input:	the translation vector
				  const vector<CloudPoint> & clouds,// input:	the cloud points
				  double angLimit,					// input:	the angular range limit of the normal of every object point, in angle, not radian
				  int imgWidth, int imgHeight,		// input:	the size of the image
				  Mat & rndField_depth,				// output:	the 32FC1 matrix with the same size of the image, the initial depth
				  Mat & rndField_incre_x,			// output:	the 32FC1 matrix with the same size of the image, the initial hx
				  Mat & rndField_incre_y			// output:	the 32FC1 matrix with the same size of the image, the initial hy
				  );

// 20140914, self-contained version
void InitRndField(const Matx33d & mK,	// input:	the camera matrix
	              const Matx33d & mR,	// input:	the rotation matrix
				  const Matx31d & mt,	// input:	the translation vector
				  double depth_min,		// input:	the minimal depth
				  double depth_max,		// input:	the maximal depth
				  double angLimit,		// input:	the angular range limit of the normal of every object point, in angle, not radian
				  int width, int height,// input:	the size of the image
				  Mat & mDepth,			// output:	the 32FC1 matrix with the same size of the image, the initial depth
				  Mat & mHx,			// output:	the 32FC1 matrix with the same size of the image, the initial hx
				  Mat & mHy				// output:	the 32FC1 matrix with the same size of the image, the initial hy
				  );

void Scan_Update_rndField(const vector<cam_data> & vCams,	// input:	all camera data
						  const vector<CloudPoint> & clouds,// input:	the cloud points
						  double angLimit,					// input:	the angular range limit of the normal of every object point, in angle, not radian
						  int idx_refimg,					// input:	the index of the reference image
						  Mat & mDepth,						// input&output:	the 32FC1 matrix with the same size of the image, the initial depth, it's updated as output
						  Mat & mIncre_x,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hx, it's updated as output
						  Mat & mIncre_y,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hy, it's updated as output
						  Mat & mScore,						// input&output:	the score matrix corresponding to the 3 parameter state matrix, it's updated as output
						  int iter,							// input:	iteration
						  int size = 5,						// input:	the window size of the image patch, should be odd number
						  double thresh_ncc = 0.8			// input:	the threshold within which to be considered as a successful ncc
						  );

void Scan_Update_rndField(const vector<cam_data> & vCams,	// input:	all camera data
						  const vector<Mat> & vImgs,		// input:	all images
						  const vector<CloudPoint> & clouds,// input:	the cloud points
						  double angLimit,					// input:	the angular range limit of the normal of every object point, in angle, not radian
						  int idx_refimg,					// input:	the index of the reference image
						  Mat & mDepth,						// input&output:	the 32FC1 matrix with the same size of the image, the initial depth, it's updated as output
						  Mat & mIncre_x,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hx, it's updated as output
						  Mat & mIncre_y,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hy, it's updated as output
						  Mat & mScore,						// input&output:	the score matrix corresponding to the 3 parameter state matrix, it's updated as output
						  int iter,							// input:	iteration
						  int size = 5,						// input:	the window size of the image patch, should be odd number
						  double thresh_ncc = 0.8			// input:	the threshold within which to be considered as a successful ncc
						  );

void Scan_Update_rndField_original(const vector<cam_data> & vCams,	// input:	all camera data
								   const vector<Mat> & vImgs,		// input:	all images
								   const vector<CloudPoint> & clouds,// input:	the cloud points
								   double angLimit,					// input:	the angular range limit of the normal of every object point, in angle, not radian
								   int idx_refimg,					// input:	the index of the reference image
								   Mat & mDepth,					// input&output:	the 32FC1 matrix with the same size of the image, the initial depth, it's updated as output
								   Mat & mIncre_x,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hx, it's updated as output
								   Mat & mIncre_y,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hy, it's updated as output
								   Mat & mScore,					// input&output:	the score matrix corresponding to the 3 parameter state matrix, it's updated as output
								   int iter,						// input:	iteration
								   int mincams = 1,
								   double factor = 0.5,
								   int nRandSamp = 6,
								   int size = 5,					// input:	the window size of the image patch, should be odd number
								   double thresh_ncc = 0.8			// input:	the threshold within which to be considered as a successful ncc
								   );

void Scan_Update_rndField_original_visi(const vector<cam_data> & vCams,	// input:	all camera data
										const vector<Mat> & vImgs,		// input:	all images
										const vector<CloudPoint> & clouds,// input:	the cloud points
										double angLimit,				// input:	the angular range limit of the normal of every object point, in angle, not radian
										int idx_refimg,					// input:	the index of the reference image
										Mat & mDepth,					// input&output:	the 32FC1 matrix with the same size of the image, the initial depth, it's updated as output
										Mat & mIncre_x,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hx, it's updated as output
										Mat & mIncre_y,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hy, it's updated as output
										Mat & mScore,					// input&output:	the score matrix corresponding to the 3 parameter state matrix, it's updated as output
										Mat & mVisi,					// output:	the 8UC1 matrix with the same size of the image
										Mat & mVisiN,					// output:	the 8UC1 matrix with the same size of the image
										int iter,						// input:	iteration
										int mincams = 1,
										double factor = 0.5,
										int nRandSamp = 6,
										int size = 5,					// input:	the window size of the image patch, should be odd number
										double thresh_ncc = 0.8			// input:	the threshold within which to be considered as a successful ncc
										);

void Scan_Update_rndField_original_fixedvisi(const vector<cam_data> & vCams,// input:	all camera data
											 const vector<Mat> & vImgs,		// input:	all images
											 const vector<CloudPoint> & clouds,// input:	the cloud points
											 double angLimit,				// input:	the angular range limit of the normal of every object point, in angle, not radian
											 int idx_refimg,				// input:	the index of the reference image
											 Mat & mDepth,					// input&output:	the 32FC1 matrix with the same size of the image, the initial depth, it's updated as output
											 Mat & mIncre_x,				// input&output:	the 32FC1 matrix with the same size of the image, the initial hx, it's updated as output
											 Mat & mIncre_y,				// input&output:	the 32FC1 matrix with the same size of the image, the initial hy, it's updated as output
											 Mat & mScore,					// input&output:	the score matrix corresponding to the 3 parameter state matrix, it's updated as output
											 const Mat & mVisi,				// input:	the 8UC1 matrix with the same size of the image, fixed visibility
											 int iter,						// input:	iteration
											 int mincams = 1,
											 double factor = 0.5,
											 int nRandSamp = 6,
											 int size = 5					// input:	the window size of the image patch, should be odd number
											 );

void Scan_Update_rndField_original_visi_new(const vector<cam_data> & vCams,	// input:	all camera data
											const vector<Mat> & vImgs,		// input:	all images
											const vector<CloudPoint> & clouds,// input:	the cloud points
											double angLimit,				// input:	the angular range limit of the normal of every object point, in angle, not radian
											int idx_refimg,					// input:	the index of the reference image
											Mat & mDepth,					// input&output:	the 32FC1 matrix with the same size of the image, the initial depth, it's updated as output
											Mat & mIncre_x,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hx, it's updated as output
											Mat & mIncre_y,					// input&output:	the 32FC1 matrix with the same size of the image, the initial hy, it's updated as output
											Mat & mScore,					// input&output:	the score matrix corresponding to the 3 parameter state matrix, it's updated as output
											Mat & mVisi,					// output:	the 8UC1 matrix with the same size of the image
											Mat & mVisiN,					// output:	the 8UC1 matrix with the same size of the image
											int iter,						// input:	iteration
											int mincams = 1,
											double factor = 0.5,
											int nRandSamp = 6,
											int size = 5,					// input:	the window size of the image patch, should be odd number
											double thresh_ncc = 0.8,		// input:	the threshold within which to be considered as a successful ncc
											double ratio_2ndto1st = 0.05
											);

void PatchMatch_withViewPropagation(const vector<cam_data> & vCams,		// input:	all camera data
									const vector<Mat> & vImgs,			// input:	all images
									const vector<CloudPoint> & clouds,	// input:	the cloud points
									vector<Mat> & vDepths,				// output:	all the depth maps
									vector<Mat> & vHxs,					// output:	all the hx maps
									vector<Mat> & vHys,					// output:	all the hy maps
									vector<Mat> & vScores,				// output:	all the score maps
									vector<Mat> & vVisis,				// output:	all the visibility maps
									vector<Mat> & vVisiNs,				// output:	all the visible image number maps
									int size = 5,						// input:	the window size of the image patch, should be odd number
									double thresh_ncc = 0.8,			// input:	the threshold within which to be considered as a successful ncc
									double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
									int maxIter = 4,					// input:	maximum iteration
									int mincams = 1,
									double factor = 0.5,
									int nRandSamp = 6
									);

// 20140728, propagate depth according to depth gradients of previous pixels
void PatchMatch_withViewPropagation_140728(const vector<cam_data> & vCams,		// input:	all camera data
										   const vector<Mat> & vImgs,			// input:	all images
										   const vector<CloudPoint> & clouds,	// input:	the cloud points
										   vector<Mat> & vDepths,				// output:	all the depth maps
										   vector<Mat> & vHxs,					// output:	all the hx maps
										   vector<Mat> & vHys,					// output:	all the hy maps
										   vector<Mat> & vScores,				// output:	all the score maps
										   vector<Mat> & vVisis,				// output:	all the visibility maps
										   vector<Mat> & vVisiNs,				// output:	all the visible image number maps
										   int size = 5,						// input:	the window size of the image patch, should be odd number
										   double thresh_ncc = 0.8,				// input:	the threshold within which to be considered as a successful ncc
										   double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
										   int maxIter = 4,						// input:	maximum iteration
										   int mincams = 1,
										   double factor = 0.5,
										   int nRandSamp = 6
										   );

// 20140802, conduct patchmatch for reference image with every support image at each time
void PatchMatch_140802(const cam_data & cam0,				// input:	reference image
					   const vector<cam_data> & vCams,		// input:	all support images
					   const Mat & img0,					// input:	reference image
					   const vector<Mat> & vImgs,			// input:	all support images
					   const vector<CloudPoint> & clouds,	// input:	the cloud points
					   vector<Mat> & vDepths,				// output:	all the depth maps
					   vector<Mat> & vHxs,					// output:	all the hx maps
					   vector<Mat> & vHys,					// output:	all the hy maps
					   vector<Mat> & vScores,				// output:	all the score maps
					   int size = 5,						// input:	the window size of the image patch, should be odd number
					   double thresh_ncc = 0.8,				// input:	the threshold within which to be considered as a successful ncc
					   double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
					   int maxIter = 4,						// input:	maximum iteration
					   int mincams = 1,
					   double factor = 0.5,
					   int nRandSamp = 6
					   );

// 20140805, conduct patchmatch for reference image with certain designated visible support images
void PatchMatch_140805(const cam_data & cam0,				// input:	reference image
					   const vector<cam_data> & vCams,		// input:	all support images
					   const Mat & img0,					// input:	reference image
					   const vector<Mat> & vImgs,			// input:	all support images
					   const vector<CloudPoint> & clouds,	// input:	the cloud points
					   Mat & mDepth,						// in&output:	initial depth map of reference image as input, and optimized as output
					   Mat & mHx,							// in&output:	initial hx map of reference image as input, and optimized as output
					   Mat & mHy,							// in&output:	initial hy map of reference image as input, and optimized as output
					   Mat & mScore,						// in&output:	initial score map of reference image as input, and optimized as output
					   const Mat & mVisi,					// input:	the visibility map
					   int size = 5,						// input:	the window size of the image patch, should be odd number
					   double thresh_ncc = 0.8,				// input:	the threshold within which to be considered as a successful ncc
					   double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
					   int maxIter = 4,						// input:	maximum iteration
					   int mincams = 1,
					   double factor = 0.5,
					   int nRandSamp = 6
					   );

// back to the original multi-view patchmatch version, with the visibility determined according to certain ncc threshold
void PatchMatch_141209(const Matx33d & mK0,					// input:	interior matrix of the reference image
					   const Matx33d & mR0,					// input:	rotation matrix of the reference image
					   const Matx31d & mt0,					// input:	translation vectors of the reference image
					   const Mat & img0,					// input:	the reference image
					   const vector<Matx33d> & vKs,			// input:	interior matrix of all support images
					   const vector<Matx33d> & vRs,			// input:	rotation matrix of all support images
					   const vector<Matx31d> & vts,			// input:	translation vectors of all support images
					   const vector<Mat> & vImgs,			// input:	all support images
					   const vector<CloudPoint> & clouds,	// input:	the given sparse cloud points
					   Mat & mDepth,						// output:	the depth map
					   Mat & mHx,							// output:	the hx map
					   Mat & mHy,							// output:	the hy map
					   Mat & mScore,						// output:	the score map
					   Mat & mVisi,							// output:	the visi map
					   int size = 5,						// input:	the window size of the image patch, should be odd number
					   double thresh_ncc = 0.8,				// input:	the threshold within which to be considered as a successful ncc
					   double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
					   int maxIter = 4,						// input:	maximum iteration
					   double factor = 0.5,
					   int nRandSamp = 6
					   );

void PatchMatch_withViewPropagation_fixedVisi(const vector<cam_data> & vCams,	// input:	all camera data
											  const vector<Mat> & vImgs,		// input:	all images
											  vector<Mat> & vDepths,			// output:	all the depth maps
											  vector<Mat> & vHxs,				// output:	all the hx maps
											  vector<Mat> & vHys,				// output:	all the hy maps
											  vector<Mat> & vScores,			// output:	all the score maps
											  const vector<Mat> & vVisis,		// input:	all the visibility maps
											  const vector<Mat> & vVisiNs,		// input:	all the visible image number maps
											  int size = 5,						// input:	the window size of the image patch, should be odd number
											  double thresh_ncc = 0.8,			// input:	the threshold within which to be considered as a successful ncc
											  double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
											  double thresh_visiAng = 75,		
											  int maxIter = 4,					// input:	maximum iteration
											  int mincams = 1,
											  double factor = 0.5,
											  int nRandSamp = 6
											  );

// void VisibilityUpdate(const vector<cam_data> & vCams,	// input:	all camera data
// 					  const vector<Mat> & vDepths,		// input
// 					  const vector<Mat> & vHxs,			// input
// 					  const vector<Mat> & vHys,			// input
// 					  vector<Mat> & vScores,			// input&output
// 					  const vector<Mat> & vVisis,		// input
// 					  vector<Mat> & vVisis_updated,		// output:	updated visibility
// 					  vector<Mat> & vVisisN_updated,	// output:	updated
// 					  double thresh_ratio = 0.01			
// 					  );

void VisibilityUpdate(const vector<cam_data> & vCams,	// input:	all camera data
					  const vector<Mat> & vDepths,		// input
					  const vector<Mat> & vHxs,			// input
					  const vector<Mat> & vHys,			// input
					  vector<Mat> & vScores,			// input&output
					  vector<Mat> & vVisis,				// input&output
					  vector<Mat> & vVisisN,			// input&output
					  double thresh_ratio = 0.01			
					  );

void VisibilityUpdate(const vector<cam_data> & vCams,	// input:	all camera data
					  const vector<Mat> & vImgs,		// input:	all images
					  const vector<Mat> & vDepths,		// input
					  const vector<Mat> & vHxs,			// input
					  const vector<Mat> & vHys,			// input
					  vector<Mat> & vScores,			// input&output
					  vector<Mat> & vVisis,				// input&output
					  vector<Mat> & vVisisN,			// input&output
					  int size = 5,						// input:	the window size of the image patch, should be odd number
					  double thresh_ncc = 0.6,
					  double thresh_ratio = 0.01			
					  );

void MPGC(const vector<cam_data> & vCams,		// input:	all camera data
		  const vector<Mat> & vImgs,			// input:	all images
		  vector<Mat> & vDepths,				// input&output:	all the depth maps
		  vector<Mat> & vHxs,					// input&output:	all the hx maps
		  vector<Mat> & vHys,					// input&output:	all the hy maps
		  vector<Mat> & vScores,				// input&output:	all the score maps
		  vector<Mat> & vVisis,					// input&output:	all the visibility maps
		  vector<Mat> & vVisiNs,				// input&output:	all the visible image number maps
		  int size = 5,						// input:	the window size of the image patch, should be odd number
		  bool bFixCk = false,				// input:	fix all ck or not
		  int maxIter = 20,				// input: max iteration
		  double xEps = 1.0E-8,			// input: threshold
		  double fEps = 1.0E-6			// input: threshold
		  );

void CheckOnePixel_givenOneParamSet(const vector<cam_data> & vCams,	// input:	all camera data
									int idx_refimg,					// input:	the index of the reference image
									int x, int y,					// input:	the indices of the pixel to be checked
									double depth,					// input:	the depth assigned to this pixel
									double hx, double hy,			// input:	the depth incremental factor along x and y axis assigned to this pixel
									vector<double> & scores,		// output:	all the scores obained by compare the patch in reference image and other patches mapped in other images
									int size = 5					// input:	the window size of the image patch
									);

void CheckOnePixel_givenOneParamSet(const vector<cam_data> & vCams,	// input:	all camera data
									const vector<Mat> & vImgs,		// input:	all images
									int idx_refimg,					// input:	the index of the reference image
									int x, int y,					// input:	the indices of the pixel to be checked
									double depth,					// input:	the depth assigned to this pixel
									double hx, double hy,			// input:	the depth incremental factor along x and y axis assigned to this pixel
									vector<double> & scores,		// output:	all the scores obained by compare the patch in reference image and other patches mapped in other images
									int size = 5					// input:	the window size of the image patch
									);

void CheckOnePixel_givenOneParamSet(const vector<Matx33d> & vKs,
									const vector<Matx33d> & vRs,
									const vector<Matx31d> & vts,
									const vector<Mat> & vImgs,		// input:	all images
									int idx_refimg,					// input:	the index of the reference image
									int x, int y,					// input:	the indices of the pixel to be checked
									double depth,					// input:	the depth assigned to this pixel
									double hx, double hy,			// input:	the depth incremental factor along x and y axis assigned to this pixel
									vector<double> & scores,		// output:	all the scores obained by compare the patch in reference image and other patches mapped in other images
									int size = 5					// input:	the window size of the image patch
									);

void CheckOnePixel_givenOneParamSet_normalangle(const vector<Matx33d> & vKs,
												const vector<Matx33d> & vRs,
												const vector<Matx31d> & vts,
												const vector<double> & vfx_1,
												const vector<double> & vfy_1,
												const vector<Mat> & vImgs,		// input:	all images
												int idx_refimg,					// input:	the index of the reference image
												int x, int y,					// input:	the indices of the pixel to be checked
												double depth,					// input:	the depth assigned to this pixel
												double hx, double hy,			// input:	the depth incremental factor along x and y axis assigned to this pixel
												vector<double> & scores,		// output:	all the scores obained by compare the patch in reference image and other patches mapped in other images
												int size = 5,					// input:	the window size of the image patch
												double thresh_angle = 60
												);

// 20140729, output the angles between current patch normal estimate and all the sight lines
void CheckOnePixel_givenOneParamSet_outputAngles(const vector<Matx33d> & vKs,
												 const vector<Matx33d> & vRs,
												 const vector<Matx31d> & vts,
												 const vector<double> & vfx_1,
												 const vector<double> & vfy_1,
												 const vector<Mat> & vImgs,		// input:	all images
												 int idx_refimg,					// input:	the index of the reference image
												 int x, int y,					// input:	the indices of the pixel to be checked
												 double depth,					// input:	the depth assigned to this pixel
												 double hx, double hy,			// input:	the depth incremental factor along x and y axis assigned to this pixel
												 vector<double> & scores,		// output:	all the scores obained by compare the patch in reference image and other patches mapped in other images
												 vector<double> & angles,		// output:	the angles between current patch normal estimate and all the sight lines
												 int size = 5					// input:	the window size of the image patch
												 );

// 20140802, check one parameter set wrt one particular image not all support images
void CheckOnePixel_givenOneParamSet_oneImg(const Matx33d & mK0, const Matx33d & mK,	// input: mK0 reference image, mK one specific support image
										   const Matx33d & mR0, const Matx33d & mR,	// input: mR0 reference image, mR one specific support image
										   const Matx31d & mt0, const Matx31d & mt,	// input: mt0 reference image, mt one specific support image
										   double fx0_1, double fx_1,				// input: fx0_1 = 1/fx0, fx_1 = 1/fx
										   double fy0_1, double fy_1,				// input: fy0_1 = 1/fy0, fy_1 = 1/fy
										   const Mat & img0, const Mat & img,		// input: reference image and the support image
										   int x, int y,							// input:	the indices of the pixel to be checked
										   double depth,							// input:	the depth assigned to this pixel
										   double hx, double hy,					// input:	the depth incremental factor along x and y axis assigned to this pixel
										   double & score,							// output:	evaluation score of current parameter set
										   double & angle,							// output:	the angle between current patch normal estimate and the sight line wrt support image
										   int size = 5								// input:	the window size of the image patch
										   );

// 20140914, self-contained version, check one parameter set wrt one particular image not all support images
void CheckOnePixel_givenOneParamSet_oneImg(const Matx33d & mK0, 	// input: mK0 reference image
										   const Matx33d & mR0, 	// input: mR0 reference image
										   const Matx31d & mt0, 	// input: mt0 reference image
										   const Mat & img0,		// input: the reference image
										   double fx0_1,			// input: fx0_1 = 1/fx0
										   double fy0_1, 			// input: fy0_1 = 1/fy0
										   const Matx33d & mK,		// input: mK one specific support image
										   const Matx33d & mR,		// input: mR one specific support image
										   const Matx31d & mt,		// input: mt one specific support image
										   const Mat & img,			// input: the support image
										   double fx_1,				// input: fx_1 = 1/fx
										   double fy_1,				// input: fy_1 = 1/fy
										   int x, int y,			// input:	the indices of the pixel to be checked
										   double depth,			// input:	the depth assigned to this pixel
										   double hx, double hy,	// input:	the depth incremental factor along x and y axis assigned to this pixel
										   double & score,			// output:	evaluation score of current parameter set
										   double & angle,			// output:	the angle between current patch normal estimate and the sight line wrt support image
										   int size = 5				// input:	the window size of the image patch
										   );

// 20140805, check one parameter set wrt multiple designated visible images not all support images
void CheckOnePixel_givenOneParamSet_mulImg(const Matx33d & mK0, const vector<Matx33d> & vKs,	// input: mK0 reference image, vKs multiple designated visible images
										   const Matx33d & mR0, const vector<Matx33d> & vRs,	// input: mR0 reference image, vRs multiple designated visible images
										   const Matx31d & mt0, const vector<Matx31d> & vts,	// input: mt0 reference image, vts multiple designated visible images
										   double fx0_1, const vector<double> & vfx_1,			// input: fx0_1 = 1/fx0, fx_1 = 1/fx
										   double fy0_1, const vector<double> & vfy_1,			// input: fy0_1 = 1/fy0, fy_1 = 1/fy
										   const Mat & img0, const vector<Mat> & vImgs,			// input: reference image and all the support images
										   int x, int y,										// input:	the indices of the pixel to be checked
										   double depth,										// input:	the depth assigned to this pixel
										   double hx, double hy,								// input:	the depth incremental factor along x and y axis assigned to this pixel
										   uchar visi,											// input:	the designated visibility wrt the support images
										   double & score,										// output:	evaluation score of current parameter set
										   int size = 5											// input:	the window size of the image patch
										   );

// 20141210, check one parameter set wrt all support images
void CheckOnePixel_givenOneParamSet_allSptImgs(const Matx33d & mK0, 		// input: mK0 reference image 
											   const Matx33d & mR0, 		// input: mR0 reference image vRs multiple designated visible images
											   const Matx31d & mt0,			// input: mt0 reference image
											   const Mat & img0,			// input: img0 reference image
											   double fx0_1,				// input: fx0_1 = 1/fx0
											   double fy0_1,				// input: fy0_1 = 1/fy0 
											   const vector<Matx33d> & vKs,	// input: vKs all support images
											   const vector<Matx33d> & vRs, // input: vRs all support images
											   const vector<Matx31d> & vts,	// input: vts all support images
											   const vector<Mat> & vImgs,	// input: all the support images
											   const vector<double> & vfx_1,// input: all the support images
											   const vector<double> & vfy_1,// input: all the support images
											   int x, int y,				// input: the indices of the pixel to be checked
											   double depth,				// input: the depth assigned to this pixel
											   double hx, double hy,		// input: the depth incremental factor along x and y axis assigned to this pixel
											   vector<double> & scores,		// output:all the scores obained by compare the patch in reference image and other patches mapped in other images
											   vector<double> & angles,		// output:the angles between current patch normal estimate and all the sight lines
											   int size = 5					// input: the window size of the image patch
											   );

double GetScore(const vector<double> & vScores,
				double thresh = 0.8,
				int minCam = 3,
				int * nVisi = NULL
				);

double GetScore_angleWeighted(const vector<double> & vScores,
							  const vector<double> & vWeights,
							  double thresh = 0.8,
							  int minCam = 3,
							  int * nVisi = NULL
							  );

double GetScore_allWeighted(const vector<double> & vScores,
							const vector<double> & vAngs,
							const vector<double> & vWeights_camangs,
							double thresh_ncc = 0.6,
							double thresh_ang = 90,
							int * nVisi = NULL
							);

// 20140801 output the weights in percentages
double GetScore_allWeighted(const vector<double> & vScores,
							const vector<double> & vAngs,
							const vector<double> & vWeights_camangs,
							vector<double> & vWeights_percent,
							double thresh_ncc = 0.6,
							double thresh_ang = 90,
							int * nVisi = NULL
							);

double GetScore_fixedVisi(const vector<double> & vScores, uchar visi);

uchar GetVisibilityVector_uchar(const vector<double> & vScores, double thresh = 0.6);

uchar GetVisibilityVector_uchar(const vector<bool> & vbools);

void InterpVisiVector_uchar(uchar visi, vector<bool> & vec, int * nValid = NULL);

// get the confidence value proposed by M. Goesele in <Multi-view revisited>
double GetConfidence(const vector<double> & vScores,
					 double thresh = 0.6,
					 int minCam = 1
					 );

void EvaluateOneParamSet(const vector<cam_data> & vCams,	// input:	all camera data
						 int idx_refimg,					// input:	the index of the reference image
						 const Mat & mDepth,	// input:	given depth map, the 32FC1 matrix
						 const Mat & mIncre_x,	// input:	given hx map, the 32FC1 matrix
						 const Mat & mIncre_y,	// input:	given hy map, the 32FC1 matrix
						 Mat & mScore,			// output:	the output score matrix corresponding to given parameter set
						 int size = 5,			// input:	the window size of the image patch, should be odd number
						 double thresh_ncc = 0.8// input:	the threshold within which to be considered as a successful ncc
						 );

void EvaluateOneParamSet(const vector<cam_data> & vCams,	// input:	all camera data
						 const vector<Mat> & vImgs,			// input:	all images
						 int idx_refimg,					// input:	the index of the reference image
						 const Mat & mDepth,	// input:	given depth map, the 32FC1 matrix
						 const Mat & mIncre_x,	// input:	given hx map, the 32FC1 matrix
						 const Mat & mIncre_y,	// input:	given hy map, the 32FC1 matrix
						 Mat & mScore,			// output:	the output score matrix corresponding to given parameter set
						 int mincam = 3,
						 int size = 5,			// input:	the window size of the image patch, should be odd number
						 double thresh_ncc = 0.8// input:	the threshold within which to be considered as a successful ncc
						 );

void WriteDepthMap(CString strFile,		// input:	the file to write
				   const cam_data & cam,// input:	the camera parameters
				   const Mat & img,		// input:	the image
				   const Mat & mDepth,	// input:	given depth map relative to the given image
				   const Mat & mScores	// input:	the confidence map of the given depth map, if -1 means the corresponding depth is invalid
				   );

void OutputPointCloud(CString strFile,		// input:	the file to write
					  const cam_data & cam,	// input:	the camera parameters
					  const Mat & img,		// input:	the image
					  const Mat & mDepth,	// input:	given depth map relative to the given image
					  const Mat & mHx,
					  const Mat & mHy,
					  const Mat & mScores	// input:	the confidence map of the given depth map, if -1 means the corresponding depth is invalid
					  );

void OutputPointCloud(CString strFile,		// input:	the file to write
					  const Matx33d & mK,	// input:	the camera parameters
					  const Matx33d & mR,
					  const Matx31d & mt,
					  double fx_1,
					  double fy_1,
					  const Mat & img,		// input:	the image
					  const Mat & mDepth,	// input:	given depth map relative to the given image
					  const Mat & mHx,
					  const Mat & mHy,
					  const Mat & mScores	// input:	the confidence map of the given depth map, if -1 means the corresponding depth is invalid
					  );

void GetNormColorField(const cam_data & cam,// input:	the camera parameters
					   const Mat & mDepth,	// input:	given depth map relative to the given image
					   const Mat & mHx,
					   const Mat & mHy,
					   Mat & mNormColor		// output:	3 channel matrix
					   );

void GetNormColorField(const Matx33d & mK,	// input:	the camera parameters
					   const Matx33d & mR,
					   const Matx31d & mt,
					   double fx_1,
					   double fy_1,
					   const Mat & mDepth,	// input:	given depth map relative to the given image
					   const Mat & mHx,
					   const Mat & mHy,
					   Mat & mNormColor		// output:	3 channel matrix
					   );

void WriteDepthMap(CString strFile,		// input:	the file to write
				   const cam_data & cam,// input:	the camera parameters
				   const Mat & img,		// input:	the image
				   const Mat & mDepth	// input:	given depth map relative to the given image
				   );

// ///////////////////////////////////////////////////////////////////////////////////////////////////

bool MPGC_get_patch_gx_gy_roberts(const Mat & img,					// input:	the image data to be resampled
								  const vector<Point2d> & vImgPts,	// input:	the image coordinates of all the samples
								  int patchWidth, int patchHeight,	// input:	the size of the image patch
								  Mat & mPatch,						// output:	the resampled image patch
								  Mat & mGx,						// output:	the dI/dx matrix of every pixel in image patch
								  Mat & mGy							// output:	the dI/dy matrix of every pixel in image patch
								  );

bool MPGC_get_patch_gx_gy_scharr(const Mat & img,					// input:	the image data to be resampled
								 const vector<Point2d> & vImgPts,	// input:	the image coordinates of all the samples
								 int patchWidth, int patchHeight,	// input:	the size of the image patch
								 Mat & mPatch,						// output:	the resampled image patch
								 Mat & mGx,							// output:	the dI/dx matrix of every pixel in image patch
								 Mat & mGy							// output:	the dI/dy matrix of every pixel in image patch
								 );

// return the jacobian matrix of kth image
bool f_jacob_fk_drhxhyck_patchref_uchar(const Mat & patch_ref,					// input:	the patch image in reference image
						 const Mat & img,						// input:	the kth image, supposed to be distortion free
					     const Matx33d & mK,					// input:	the kth image's intrinsic parameters
					     const Matx33d & mR,					// input:	the kth image's rotation matrix
					     const Matx31d & mt,					// input:	the kth image's translation vector
					     const vector<Point3d> & vPts3D,		// input:	the spatial coordinates of every points in the patch
					     const vector<Point3d> & vNImgPts_ref,	// input:	the normalized image coordinates of every points in the patch in reference image
					     double ck,								// input:	current estimate of the photometric factor
					     Mat & mJacob,							// output:	the jacobian matrix
					     Mat & mF,								// output:	the objective vector 
						 Mat & patch_rsamp						// output:	the resampled patch in kth image
					     );

// return the jacobian matrix of kth image
bool f_jacob_fk_drhxhyck_patchref_float(const Mat & patch_ref,					// input:	the patch image in reference image
										const Mat & img,						// input:	the kth image, supposed to be distortion free
										const Matx33d & mK,						// input:	the kth image's intrinsic parameters
										const Matx33d & mR,						// input:	the kth image's rotation matrix
										const Matx31d & mt,						// input:	the kth image's translation vector
										const vector<Point3d> & vPts3D,			// input:	the spatial coordinates of every points in the patch
										const vector<Point3d> & vNImgPts_ref,	// input:	the normalized image coordinates of every points in the patch in reference image
										double ck,								// input:	current estimate of the photometric factor
										Mat & mJacob,							// output:	the jacobian matrix
										Mat & mF,								// output:	the objective vector 
										Mat & patch_rsamp						// output:	the resampled patch in kth image
										);

// 20140904 return the objective vector and jacobian matrix of kth image
bool f_jacob_fk_drhxhyck_patchref_float_20140904(const Mat & patch_ref,					// input:	the patch image in reference image
												 const Mat & img,						// input:	the kth image, supposed to be distortion free
												 const Matx33d & mKR,					// input:	the kth image's intrinsic parameters
												 const Matx31d & mKt,					// input:	the kth image's rotation matrix
												 const vector<Matx31d> & vXYZs,			// input:	the spatial coordinates of every points in the patch
												 const vector<Matx31d> & vRtxs_ref,		// input:	the normalized image coordinates of every points in the patch in reference image
												 double ck,								// input:	current estimate of the photometric factor
												 Mat & mJacob,							// output:	the jacobian matrix
												 Mat & mF,								// output:	the objective vector 
												 Mat & patch_rsamp						// output:	the resampled patch in kth image
												 );

// 20140911 return the objective vector and jacobian matrix of kth image
bool f_jacob_fk_drhxhyck_patchref_float_20140911(const Mat & patch_ref,					// input:	the patch image in reference image
												 const Mat & img,						// input:	the kth image, supposed to be distortion free
												 const Mat & mask,						// input:	mask
												 int nValid,							// input:	number of valid pixels in mask
												 const Matx33d & mK,
												 const Matx33d & mR,
												 const Matx33d & mKR,					// input:	the kth image's intrinsic parameters
												 const Matx31d & mKt,					// input:	the kth image's rotation matrix
												 double fx_1, double fy_1,
												 const Matx31d & mnw,					// input:	the normal vector of reference patch in world coordinate
												 const vector<Matx31d> & vXYZs,			// input:	the spatial coordinates of every points in the patch
												 const vector<Matx31d> & vRtxs_ref,		// input:	the normalized image coordinates of every points in the patch in reference image
												 double ck,								// input:	current estimate of the photometric factor
												 Mat & mJacob,							// output:	the jacobian matrix
												 Mat & mF,								// output:	the objective vector 
												 double & ncc,							// output:	the ncc
												 bool & bOppo							// output:	whether if normal is opposite or not
												 );

bool optim_gn_drhxhyck(const vector<cam_data> & vCams,	// input:	all camera data
					   const vector<Mat> & vImgs,		// input:	all images
					   const vector<int> & vIdxVisiCams,// input:	indices of all visible cameras
					   int idx_refimg,					// input:	the index of the reference image
					   int x, int y,					// input:	the indices of the pixel to be checked
					   int patchWidth, int patchHeight,	// input:	the patch size
					   double depth_init,				// input:	initial depth of this pixel
					   double hx_init,					// input:	initial normal of this pixel
					   double hy_init,					// input:	initial normal of this pixel
					   double & depth_optim,			// output:	optimized depth of this pixel
					   double & hx_optim,				// output:	optimized normal of this pixel
					   double & hy_optim,				// output:	optimized normal of this pixel
					   bool bFixCk = false,				// input:	fix all ck or not
					   int maxIter = 128,				// input: max iteration
					   double xEps = 1.0E-8,			// input: threshold
					   double fEps = 1.0E-6,			// input: threshold
					   int * iterNum = NULL				// output:iteration number when quiting
					   );

bool optim_gn_drhxhyck_new(const vector<cam_data> & vCams,	// input:	all camera data
						   const vector<Mat> & vImgs,		// input:	all images
						   const vector<int> & vIdxVisiCams,// input:	indices of all visible cameras
						   int idx_refimg,					// input:	the index of the reference image
						   int x, int y,					// input:	the indices of the pixel to be checked
						   int patchWidth, int patchHeight,	// input:	the patch size
						   double depth_init,				// input:	initial depth of this pixel
						   double hx_init,					// input:	initial normal of this pixel
						   double hy_init,					// input:	initial normal of this pixel
						   double & depth_optim,			// output:	optimized depth of this pixel
						   double & hx_optim,				// output:	optimized normal of this pixel
						   double & hy_optim,				// output:	optimized normal of this pixel
						   bool bFixCk = false,				// input:	fix all ck or not
						   int maxIter = 128,				// input: max iteration
						   double xEps = 1.0E-8,			// input: threshold
						   double fEps = 1.0E-6,			// input: threshold
						   int * iterNum = NULL				// output:iteration number when quiting
						   );

bool optim_gn_drhxhyck(const vector<Matx33d> & vKs,
					   const vector<Matx33d> & vRs,
					   const vector<Matx31d> & vts,
					   const vector<Mat> & vImgs,		// input:	all images
					   const vector<int> & vIdxVisiCams,// input:	indices of all visible cameras
					   int idx_refimg,					// input:	the index of the reference image
					   int x, int y,					// input:	the indices of the pixel to be checked
					   int patchWidth, int patchHeight,	// input:	the patch size
					   double depth_init,				// input:	initial depth of this pixel
					   double hx_init,					// input:	initial normal of this pixel
					   double hy_init,					// input:	initial normal of this pixel
					   double & depth_optim,			// output:	optimized depth of this pixel
					   double & hx_optim,				// output:	optimized normal of this pixel
					   double & hy_optim,				// output:	optimized normal of this pixel
					   bool bFixCk = false,				// input:	fix all ck or not
					   int maxIter = 128,				// input: max iteration
					   double xEps = 1.0E-8,			// input: threshold
					   double fEps = 1.0E-6,			// input: threshold
					   int * iterNum = NULL				// output:iteration number when quiting
					   );

// 20140604
bool optim_gn_drhxhyck_NCCcontrolled(const vector<Matx33d> & vKs,
								     const vector<Matx33d> & vRs,
								     const vector<Matx31d> & vts,
								     const vector<Mat> & vImgs,			// input:	all images
								     const vector<int> & vIdxVisiCams,	// input:	indices of all visible cameras
								     int idx_refimg,					// input:	the index of the reference image
								     int x, int y,						// input:	the indices of the pixel to be checked
								     int patchWidth, int patchHeight,	// input:	the patch size
								     double depth_init,					// input:	initial depth of this pixel
								     double hx_init,					// input:	initial normal of this pixel
								     double hy_init,					// input:	initial normal of this pixel
								     double & depth_optim,				// output:	optimized depth of this pixel
								     double & hx_optim,					// output:	optimized normal of this pixel
								     double & hy_optim,					// output:	optimized normal of this pixel
								     int maxIter = 128,					// input: max iteration
								     double xEps = 1.0E-8,				// input: threshold
								     double fEps = 1.0E-6,				// input: threshold
								     int * iterNum = NULL				// output:iteration number when quiting
								     );

// 20140904
bool optim_gn_drhxhyck_NCCcontrolled(const vector<Matx33d> & vKs,
								     const vector<Matx33d> & vRs,
								     const vector<Matx31d> & vts,
									 const vector<double> & vfx_1,		// input:	
									 const vector<double> & vfy_1,		// input:
								     const vector<Mat> & vImgs,			// input:	all images
								     const vector<vector<int>> & vIdxSupports,// input:	all images' support images' index
								     int idx_refimg,					// input:	the index of the reference image
								     int x, int y,						// input:	the indices of the pixel to be checked
									 uchar visi,						// input:	visibility within support images
								     int patchWidth, int patchHeight,	// input:	the patch size
								     double depth_init,					// input:	initial depth of this pixel
								     double hx_init,					// input:	initial normal of this pixel
								     double hy_init,					// input:	initial normal of this pixel
									 double score_init,					// input:	initial mean ncc value
								     double & depth_optim,				// output:	optimized depth of this pixel
								     double & hx_optim,					// output:	optimized normal of this pixel
								     double & hy_optim,					// output:	optimized normal of this pixel
									 double & score_optim,				// output:	optimized mean ncc value
								     int maxIter = 128,					// input: max iteration
								     double xEps = 1.0E-8,				// input: threshold
								     double fEps = 1.0E-6,				// input: threshold
								     int * iterNum = NULL				// output:iteration number when quiting
								     );

// 20140911 with masks to ensure unimodal MPGC
bool optim_gn_drhxhyck_NCCcontrolled_masks(const vector<Matx33d> & vKs,
										   const vector<Matx33d> & vRs,
										   const vector<Matx31d> & vts,
										   const vector<double> & vfx_1,			// input:	
										   const vector<double> & vfy_1,			// input:
										   const vector<Mat> & vImgs,				// input:	all images
										   const vector<vector<int>> & vIdxSupports,// input:	all images' support images' index
										   const vector<Mat> & vMasks,				// input:	pre-determined masks, one for each visible image
										   const vector<int> & vNum,				// input:	number of valid pixels in each mask
										   int idx_refimg,							// input:	the index of the reference image
										   int x, int y,							// input:	the indices of the pixel to be checked
										   uchar visi,								// input:	visibility within support images
										   int patchWidth, int patchHeight,			// input:	the patch size
										   double depth_init,						// input:	initial depth of this pixel
										   double hx_init,							// input:	initial normal of this pixel
										   double hy_init,							// input:	initial normal of this pixel
										   double score_init,						// input:	initial mean ncc value
										   double & depth_optim,					// output:	optimized depth of this pixel
										   double & hx_optim,						// output:	optimized normal of this pixel
										   double & hy_optim,						// output:	optimized normal of this pixel
										   double & score_optim,					// output:	optimized mean ncc value
										   int maxIter = 128,						// input: max iteration
										   double xEps = 1.0E-8,					// input: threshold
										   double fEps = 1.0E-6,					// input: threshold
										   int * iterNum = NULL						// output:iteration number when quiting
										   );

// 20140913, self-contained, with masks to ensure unimodal MPGC
bool optim_gn_drhxhyck_NCCcontrolled_masks(const Matx33d & mK0,						// input:	interior matrix of reference image
										   const Matx33d & mR0,						// input:	rotation matrix of reference image
										   const Matx31d & mt0,						// input:	translation vector of reference image
										   const Mat & img0,						// input:	reference image
										   const vector<Matx33d> & vKs,				// input:	interior matrix of all visible support images
										   const vector<Matx33d> & vRs,				// input:	rotation matrix of all visible support images
										   const vector<Matx31d> & vts,				// input:	translation vectors of all visible support images
										   const vector<Mat> & vImgs,				// input:	images of all visible support images
										   const vector<Mat> & vMasks,				// input:	pre-determined masks, one for each visible image
										   const vector<int> & vNum,				// input:	number of valid pixels in each mask, one for each visible image
										   int x, int y,							// input:	the indices of the pixel to be checked
										   int patchWidth, int patchHeight,			// input:	the patch size
										   double depth_init,						// input:	initial depth of this pixel
										   double hx_init,							// input:	initial normal of this pixel
										   double hy_init,							// input:	initial normal of this pixel
										   double score_init,						// input:	initial mean ncc value
										   double & depth_optim,					// output:	optimized depth of this pixel
										   double & hx_optim,						// output:	optimized normal of this pixel
										   double & hy_optim,						// output:	optimized normal of this pixel
										   double & score_optim,					// output:	optimized mean ncc value
										   int maxIter = 128,						// input: max iteration
										   double xEps = 1.0E-8,					// input: threshold
										   double fEps = 1.0E-6,					// input: threshold
										   int * iterNum = NULL						// output:iteration number when quiting
										   );

// 20140903
void optim_gn_depth_minimizingProjError(const Matx33d & mR0,
										const Matx31d & mt0,
										const vector<Matx33d> & vKs,
										const vector<Matx33d> & vRs,
										const vector<Matx31d> & vts,
										const vector<double> & vDs,
										double nimgx, double nimgy,
										double & depth_optim,
										int maxIter = 128,
										double xEps = 1.0E-8,				// input: threshold
										double fEps = 1.0E-6				// input: threshold
										);

// 20140604
bool optim_gn_drhxhyck_NCCcontrolled(const vector<cam_data> & vCams,	// input:	all camera data
								     const vector<Mat> & vImgs,			// input:	all images
								     const vector<int> & vIdxVisiCams,	// input:	indices of all visible cameras
								     int idx_refimg,					// input:	the index of the reference image
								     int x, int y,						// input:	the indices of the pixel to be checked
								     int patchWidth, int patchHeight,	// input:	the patch size
								     double depth_init,					// input:	initial depth of this pixel
								     double hx_init,					// input:	initial normal of this pixel
								     double hy_init,					// input:	initial normal of this pixel
								     double & depth_optim,				// output:	optimized depth of this pixel
								     double & hx_optim,					// output:	optimized normal of this pixel
								     double & hy_optim,					// output:	optimized normal of this pixel
								     int maxIter = 128,					// input: max iteration
								     double xEps = 1.0E-8,				// input: threshold
								     double fEps = 1.0E-6,				// input: threshold
								     int * iterNum = NULL				// output:iteration number when quiting
								     );

// the normal n(a,b,1) is the opposite normal, the actual normal is -n(-a,-b,-1)
void get_normal_givendrhxhy(double fx, double fy,		// input:	equivalent focal length
							double nimgx, double nimgy,	// input:	the normalized image point
							double dr,					// input:	the depth of given pixel
							double hx, double hy,		// input:	the depth gradient
							double & a, double & b		// output:	the normal is n(a,b,1)
							);

void get_hxhy_givendrnormal(double fx, double fy,		// input:	equivalent focal length
							double nimgx, double nimgy,	// input:	the normalized image point
							double dr,					// input:	the depth of given pixel
							const Matx31d & mn,			// input:	the normal
							double & hx, double & hy	// output:	the normal is n(a,b,1)
							);

bool isvalid_hxhy(double fx, double fy,			// input:	equivalent focal length
				  double nimgx, double nimgy,	// input:	the normalized image point
				  double dr,					// input:	the depth of given pixel
				  double hx, double hy,			// input:	the depth gradient
				  double thresh = 0.1			// input:	threshold 
				  );

void f_jacob_uvrou(const Matx33d & mKr, const Matx33d & mRr, const Matx31d & mtr,
				   const Matx33d & mK,  const Matx33d & mR,  const Matx31d & mt,
				   double d, double hx, double hy,
				   double x0, double y0,
				   double xk, double yk,
				   double u, double v, double rou,
				   Matx33d & mJ,
				   Matx31d & mF
				   );

void optim_gn_uvrou(const Matx33d & mKr, const Matx33d & mRr, const Matx31d & mtr,
				    const Matx33d & mK,  const Matx33d & mR,  const Matx31d & mt,
				    double d, double hx, double hy,
				    double x0, double y0,
				    double xk, double yk,
					double & u_optim, double & v_optim,
					double & dk,
					int maxIter = 8,
					double imgptEps = 0.00001,
					int * iterNum = NULL
				    );

void optim_gn_uvrou_initialdk(const Matx33d & mKr, const Matx33d & mRr, const Matx31d & mtr,
							  const Matx33d & mK,  const Matx33d & mR,  const Matx31d & mt,
							  double d, double hx, double hy,
							  double x0, double y0,
							  double xk, double yk,
							  double & u_optim, double & v_optim,	// input&output
							  double & dk,							// input&output
							  int maxIter = 8,
							  double imgptEps = 0.00001,
							  int * iterNum = NULL
							  );

void optim_gn_uvrou(const cam_data & cam0,
				    const cam_data & cam,
				    double d, double hx, double hy,
				    double x0, double y0,
				    double xk, double yk,
					double & u_optim, double & v_optim,
					double & dk,
					int maxIter = 8,
					double imgptEps = 0.00001,
					int * iterNum = NULL
				    );

// 20150331, compute the Jacobian matrix and function values at current estimates
void J_f_optim_P_XYZ(int nPts,							// input:	number of correspondences 
					 const vector<Point2d> & vImgPts0,	// input:	the measured image points in the left image
					 const vector<Point2d> & vImgPts1,	// input:	the measured image points in the right image
					 const Mat & x,						// input:	current estimates
					 Mat & J,							// output:	Jacobian matrix at current estimates, should be initialized outside the function
					 Mat & f,							// output:	function values at current estimates, should be initialized outside the function
					 vector<double> & vds0,				// output:	the reprojection error vector in the 1st image, should be initialized outside the function
					 vector<double> & vds1				// output:	the reprojection error vector in the 2nd image, should be initialized outside the function
					 );

// zhaokunz, 20150109, given camera matrix of the left image [I|0], the right image [P]
// given the measured image points in both images and the initial projective reconstructed world coordinates
// optimize the camera matrix [P] of the right image and the projective reconstructed world coordinates
// using the Gauss-Newton method
bool optim_gn_P_XYZ(const vector<Point2d> & vImgPts0,		// input:	the measured image points in the left image
				    const vector<Point2d> & vImgPts1,		// input:	the measured image points in the right image
				    const Matx34d & mP_init,				// input:	the initial camera matrix of the right image
				    const vector<Point3d> & vWrdPts_init,	// input:	the initial projective XYZ coordinates of world points
				    Matx34d & mP_optim,						// output:	the optimized camera matrix of the right image
				    vector<Point3d> & vWrdPts_optim,		// output:	the optimized projective XYZ coordinates of world points
				    double & err_rpj,						// output:	the reprojection error
				    int maxIter = 10,						// input:	the maximum number of iterations
				    double xEps = 1.0E-8,					// input:	threshold
				    double fEps = 1.0E-6					// input:	threshold
				    );

// zhaokunz, 20150331, given camera matrix of the left image [I|0], the right image [P]
// given the measured image points in both images and the initial projective reconstructed world coordinates
// optimize the camera matrix [P] of the right image and the projective reconstructed world coordinates
// using the Levenberg-Marquardt method
void optim_lm_P_XYZ(const vector<Point2d> & vImgPts0,		// input:	the measured image points in the left image
				    const vector<Point2d> & vImgPts1,		// input:	the measured image points in the right image
				    const Matx34d & mP_init,				// input:	the initial camera matrix of the right image
				    const vector<Point3d> & vWrdPts_init,	// input:	the initial projective XYZ coordinates of world points
				    Matx34d & mP_optim,						// output:	the optimized camera matrix of the right image
				    vector<Point3d> & vWrdPts_optim,		// output:	the optimized projective XYZ coordinates of world points
					vector<double> & info,					// output:	runtime info, 5-vector
															// info[0]:	the initial reprojection error
															// info[1]:	the final reprojection error
															// info[2]: final max gradient
															// info[3]: the number of iterations
															// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
				    double tau = 1.0E-3,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
				    int maxIter = 15,						// input:	the maximum number of iterations
				    double eps1 = 1.0E-8,					// input:	threshold
				    double eps2 = 1.0E-12					// input:	threshold
				    );

// 20150110, zhaokunz, optimize the given fundamental matrix according to the golden standard algorithm
// gauss-newton
bool optim_gn_F(const vector<Point2d> & vImgPts0,	// input:	the measured image points in the left image
			    const vector<Point2d> & vImgPts1,	// input:	the measured image points in the right image
			    const Matx33d & mF_init,			// input:	the initial fundamental matrix
			    Matx33d & mF_optim,					// output:	the optimized fundamental matrix
			    int maxIter = 10,					// input:	the maximum number of iterations
			    double xEps = 1.0E-8,				// input:	threshold
			    double fEps = 1.0E-6				// input:	threshold
			    );

// 20150401, zhaokunz, optimize the given fundamental matrix according to the golden standard algorithm
// Levenberg-Marquardt
void optim_lm_F(const vector<Point2d> & vImgPts0,	// input:	the measured image points in the left image
			    const vector<Point2d> & vImgPts1,	// input:	the measured image points in the right image
			    const Matx33d & mF_init,			// input:	the initial fundamental matrix
			    Matx33d & mF_optim,					// output:	the optimized fundamental matrix
				double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
				int maxIter = 15,					// input:	the maximum number of iterations
				double eps1 = 1.0E-8,				// input:	threshold
				double eps2 = 1.0E-12				// input:	threshold
			    );

void PropagateDepth2OtherImage(const cam_data & cam0, const cam_data & cam,
							   const Mat & mDepth0, const Mat & mHx0, const Mat & mHy0, const Mat & mScore0,
							   Mat & mDepth, double & mindepth, double & maxdepth
							   );

void DepthConsistencyCheck(const vector<cam_data> & vCams,
						   const vector<Mat> & vImgs,		// input:	all images
						   const vector<Mat> & vDepths, const vector<Mat> & vHxs, const vector<Mat> & vHys,
						   vector<Mat> & vDepths_final, vector<Mat> & vHxs_final, vector<Mat> & vHys_final,
						   vector<Point2d> & vMinMax_d, vector<Point2d> & vMinMax_hx, vector<Point2d> & vMinMax_hy,
						   int wndSize = 5,
						   double thresh_ncc = 0.5,
						   double thresh_radio = 0.001
						   );

// 20140828
void DepthConsistencyCheck(const vector<Matx33d> & vKs,				// input:	all interior matrix
						   const vector<Matx33d> & vRs,				// input:	all rotation matrix
						   const vector<Matx31d> & vts,				// input:	all translation vectors
						   const vector<double> & vfx_1,			// input:	
						   const vector<double> & vfy_1,			// input:
						   const vector<vector<int>> & vIdxSupports,// input:	all images' support images' index
						   int idx_ref,								// input:	the reference image index
						   const vector<Mat> & vDepths,
						   const vector<Mat> & vHxs,
						   const vector<Mat> & vHys,
						   vector<Mat> & vScores,					// output:	pixels do not pass check are set to invalids
						   const vector<Mat> & vVisis,
						   double thresh_ratio = 0.001
						   );

// 20140913, self-contained version
void DepthConsistencyCheck(const Matx33d & mK0,						// input:	interior matrix of reference image
						   const Matx33d & mR0,						// input:	rotation matrix of reference image
						   const Matx31d & mt0,						// input:	translation vector of reference image
						   const Mat & mDepth0,						// input:	depth map of reference image
						   const Mat & mVisi0,						// input:	visibility map of reference image
						   Mat & mScore0,							// output:	pixels do not pass check are set to invalids
						   const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
						   const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
						   const vector<Matx31d> & vts,				// input:	translation vectors of all support images
						   const vector<Mat> & vDepths,				// input:	depth maps of all support images
						   const vector<Mat> & vHxs,				// input:	hx maps of all support images
						   const vector<Mat> & vHys,				// input:	hy maps of all support images
						   double thresh_ratio = 0.001
						   );

void DepthConsistencyCheck_QualityEvaluation(const vector<cam_data> & vCams,
											 const vector<Mat> & vImgs,		// input:	all images
											 const vector<Mat> & vDepths, const vector<Mat> & vHxs, const vector<Mat> & vHys,
											 vector<Mat> & vDepths_final, vector<Mat> & vHxs_final, vector<Mat> & vHys_final, vector<Mat> & vQuality_final,
											 vector<Point2d> & vMinMax_d, vector<Point2d> & vMinMax_hx, vector<Point2d> & vMinMax_hy, vector<Point2d> & vMinMax_quality,
											 int wndSize = 5,
											 double thresh_ncc = 0.5,
											 double thresh_radio = 0.001
											 );

void DepthConsistencyCheck_QualityEvaluation_Goesele(const vector<cam_data> & vCams,
													 const vector<Mat> & vImgs,		// input:	all images
													 const vector<Mat> & vSilhouettes,	// input: silhouettes of objects
													 const vector<Mat> & vDepths, const vector<Mat> & vHxs, const vector<Mat> & vHys, const vector<Mat> & vVisis,
													 vector<Mat> & vDepths_final, vector<Mat> & vHxs_final, vector<Mat> & vHys_final, vector<Mat> & vQuality_final, vector<Mat> & vQuality_G_final,
													 vector<Point2d> & vMinMax_d, vector<Point2d> & vMinMax_hx, vector<Point2d> & vMinMax_hy, vector<Point2d> & vMinMax_quality,
													 int wndSize = 5,
													 double thresh_ncc = 0.5,
													 double thresh_radio = 0.001
													 );

// 20140605, reject bad depth by setting its score to -1
void DepthConsistencyCheck_QualityEvaluation_visi(const vector<cam_data> & vCams,
												  const vector<Mat> & vImgs,		// input:	all images
												  const vector<Mat> & vSilhouettes,	// input: silhouettes of objects
												  const vector<Mat> & vDepths, const vector<Mat> & vHxs, const vector<Mat> & vHys, const vector<Mat> & vVisis,
												  vector<Mat> & vScores, vector<Mat> & vQuality,
												  int wndSize = 5,
												  double thresh_radio = 0.001
												  );

// 20140731, reject bad depth by setting its score to -1
void DepthConsistencyCheck_QualityEvaluation_visi_20140731(const vector<cam_data> & vCams,
														   const vector<Mat> & vImgs,		// input:	all images
														   const vector<Mat> & vSilhouettes,	// input: silhouettes of objects
														   const vector<Mat> & vDepths, const vector<Mat> & vHxs, const vector<Mat> & vHys, const vector<Mat> & vVisis,
														   vector<Mat> & vScores,
														   int wndSize = 5,
														   double thresh_radio = 0.001
														   );

void DepthConsistencyCheck_QualityEvaluation_visi(const vector<cam_data> & vCams,
												  const vector<Mat> & vImgs,		// input:	all images
												  const vector<Mat> & vDepths, const vector<Mat> & vHxs, const vector<Mat> & vHys, const vector<Mat> & vVisis,
												  vector<Mat> & vDepths_final, vector<Mat> & vHxs_final, vector<Mat> & vHys_final, vector<Mat> & vVisis_diff, vector<Mat> & vQuality_final,
												  vector<Point2d> & vMinMax_d, vector<Point2d> & vMinMax_hx, vector<Point2d> & vMinMax_hy, vector<Point2d> & vMinMax_quality,
												  int wndSize = 5,
												  double thresh_ncc = 0.5,
												  double thresh_radio = 0.001
												  );

void OutputSupportWindow(CString strPath,
						 const Matx33d & mK, const Matx33d & mR, const Matx31d & mt,
						 int x, int y,
						 double depth,
						 double hx,
						 double hy,
						 int w, int h,
						 uchar r, uchar g, uchar b,
						 int nSampleNorm = 4
						 );

// 20140731, first cluster depth map into regions according to depth increment between 4 neighbors
// then remove small depth regions if their area is below some threshold
void RemoveSmallDepthRegions_4(const cam_data & camInfo,
							 const Mat & img,
							 const Mat & mDepth,
							 Mat & mScore,
							 double thresh_ang = 85,
							 int thresh_area = 15
							 );

// 20140828
void RemoveSmallDepthRegions_4(const Matx33d & mK0,			// input:	mK0 reference image
							   const Matx33d & mR0,			// input:	mR0 reference image
							   const Matx31d & mt0,			// input:	mt0 reference image
							   double fx0_1, 				// input:	fx0_1 = 1/fx0
							   double fy0_1, 				// input:	fy0_1 = 1/fy0
							   const Mat & img,
							   int idx_ref,
							   const Mat & mDepth,
							   const Mat & mHx,
							   const Mat & mHy,
							   Mat & mScore,
							   double thresh_ang = 85,
							   int thresh_area = 15
							   );

// 20140913, self-contained version
void RemoveSmallDepthRegions_4(const Matx33d & mK,			// input:	reference image
							   const Matx33d & mR,			// input:	reference image
							   const Matx31d & mt,			// input:	reference image
							   const Mat & mDepth,
							   Mat & mScore,
							   double thresh_ratio = 0.01,
							   int thresh_area = 15
							   );

// 20140910 为指定的深度在其周围进行聚类，最后看那些深度能和指定深度聚到一起，也就是看存不存在depth discontinuities
void Clustering_depth_4(const Mat & mDepth,			// input:	input depth map
					    int x, int y,				// input:	designated depth
					    Mat & mOut,					// output:	uchar matrix, if 1 means similar depth with designated depth, if 0 means not
						int & nNum,					// output:	number of elements in final cluster
					    double ratio = 0.01			// input:	the depth difference threshold
					    );

// 20140910 为指定的深度在其周围进行聚类，最后看那些深度能和指定深度聚到一起，也就是看存不存在depth discontinuities
void Clustering_depth_8(const Mat & mDepth,			// input:	input depth map
					    int x, int y,				// input:	designated depth
					    Mat & mOut,					// output:	uchar matrix, if 1 means similar depth with designated depth, if 0 means not
						int & nNum,					// output:	number of elements in final cluster
					    double ratio = 0.01			// input:	the depth difference threshold
					    );

// 20140911 为指定的深度图进行聚类标记
void Labelling_depth_4(const Mat & mDepth,			// input:	input depth map
					   Mat & mLabel,				// output:	uchar matrix, if 1 means similar depth with designated depth, if 0 means not
					   int idx_ref,
					   double ratio = 0.01			// input:	the depth difference threshold
					   );

// 20140801, first cluster depth map into regions according to depth increment between 8 neighbors
// then remove small depth regions if their area is below some threshold
void RemoveSmallDepthRegions_8(const cam_data & camInfo,
							 const Mat & img,
							 const Mat & mDepth,
							 Mat & mScore,
							 double thresh_ang = 85,
							 int thresh_area = 15
							 );

void ExtractMostLikelyDepth_NCC_WTA(const cam_data & cam0,
								    const Mat & img0,
								    const vector<Mat> & vDepths,
								    const vector<Mat> & vHxs,
								    const vector<Mat> & vHys,
								    const vector<Mat> & vScores
								    );

void ExtractMostLikelyDepth_DP(const cam_data & cam0,		// input:	EO and IO of reference image
							   const Mat & img0,			// input:	reference image
							   const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
							   const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
							   const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
							   const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
							   Mat & mSel					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
							   );

void ExtractMostLikelyDepth_DP(const Matx33d & mK0,			// input:	mK0 reference image
							   const Matx33d & mR0,			// input:	mR0 reference image
							   const Matx31d & mt0,			// input:	mt0 reference image
							   double fx0_1, 				// input:	fx0_1 = 1/fx0
							   double fy0_1, 				// input:	fy0_1 = 1/fy0
							   const Mat & img0,			// input:	reference image
							   int idx_ref,
							   const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
							   const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
							   const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
							   const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
							   Mat & mSel					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
							   );

void Extract_MRF_d_DP_withInvalids(const cam_data & cam0,		// input:	EO and IO of reference image
								   const Mat & img0,			// input:	reference image
								   const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
								   const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
								   const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
								   const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
								   Mat & mSel					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
								   );

void Extract_MRF_d_DP_withInvalids(const Matx33d & mK0,			// input:	mK0 reference image
								   const Matx33d & mR0,			// input:	mR0 reference image
								   const Matx31d & mt0,			// input:	mt0 reference image
								   double fx0_1, 				// input:	fx0_1 = 1/fx0
								   double fy0_1, 				// input:	fy0_1 = 1/fy0
								   const Mat & img0,			// input:	reference image
								   int idx_ref,
								   const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
								   const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
								   const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
								   const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
								   Mat & mSel,					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
								   Mat & mDepth,				// output:	selected best depth map
								   Mat & mHx,					// output:	selected best hx map
								   Mat & mHy,					// output:	selected best hy map
								   Mat & mScore					// output:	selected best score map
								   );

// complete MRF model, data term is considered, and smoothness term is used on both depth and normals
void Extract_MRF_ncc_d_n_DP_withInvalids(const Matx33d & mK0,			// input:	mK0 reference image
										 const Matx33d & mR0,			// input:	mR0 reference image
										 const Matx31d & mt0,			// input:	mt0 reference image
										 double fx0_1, 					// input:	fx0_1 = 1/fx0
										 double fy0_1, 					// input:	fy0_1 = 1/fy0
										 const Mat & img0,				// input:	reference image
										 int idx_ref,
										 const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
										 const vector<Mat> & vHxs,		// input:	all hx map relative to reference wrt each support image
										 const vector<Mat> & vHys,		// input:	all hy map relative to reference wrt each support image
										 const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
										 Mat & mSel,					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
										 Mat & mDepth,					// output:	selected best depth map
										 Mat & mHx,						// output:	selected best hx map
										 Mat & mHy,						// output:	selected best hy map
										 Mat & mScore,					// output:	selected best score map
										 double P1,						// input:
										 double P2						// input:
										 );

// 20140914, self-contained version, complete MRF model, data term is considered, and smoothness term is used on both depth and normals
void Extract_MRF_ncc_d_n_DP_withInvalids(const Matx33d & mK0,			// input:	mK0 reference image
										 const Matx33d & mR0,			// input:	mR0 reference image
										 const Matx31d & mt0,			// input:	mt0 reference image
										 const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
										 const vector<Mat> & vHxs,		// input:	all hx map relative to reference wrt each support image
										 const vector<Mat> & vHys,		// input:	all hy map relative to reference wrt each support image
										 const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
										 Mat & mSel,					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
										 Mat & mDepth,					// output:	selected best depth map
										 Mat & mHx,						// output:	selected best hx map
										 Mat & mHy,						// output:	selected best hy map
										 Mat & mScore,					// output:	selected best score map
										 double P1,						// input:
										 double P2						// input:
										 );

void DP_step(int xr_1, int yr_1,
			 int xr, int yr,
			 const vector<Mat> & vDepths,
			 const vector<double> & Lr_1,
			 vector<double> & Lr
			 );

// considering invalid values, just like in my SGM function
void DP_step_withInvalids(int xr_1, int yr_1,
						  int xr, int yr,
						  const vector<Mat> & vDepths,
						  const vector<Mat> & vScores,
						  const vector<double> & Lr_1,
						  vector<double> & Lr
						  );

// DP used in complete MRF model, including data term, smoothness terms on both depth and normals
void DP_step_withInvalids(const Matx33d & mK,			// input:	mK0 reference image
						  const Matx33d & mR,			// input:	mR0 reference image
						  const Matx31d & mt,			// input:	mt0 reference image
						  double fx_1, 					// input:	fx0_1 = 1/fx0
						  double fy_1,					// input:	fy0_1 = 1/fy0
						  int xr_1, int yr_1,			// input:	previous pixel
						  int xr, int yr,				// input:	current pixel
						  const vector<Mat> & vDepths,	// input:	all depth candidates
						  const vector<Mat> & vHxs,		// input:	all hx map relative to reference wrt each support image
						  const vector<Mat> & vHys,		// input:	all hy map relative to reference wrt each support image
						  const vector<Mat> & vScores,	
						  double pi_1,					// input:	1/π
						  double P1,					// input:
						  double P2,					// input:
						  const vector<double> & Lr_1,
						  vector<double> & Lr
						  );

void AugmentVisibility_basedonMostLikelyDepth(const cam_data & cam0,			// input:	EO and IO of reference image
											  const vector<cam_data> & vCams,	// input:	EO and IO of all support images
											  const Mat & img0,					// input:	reference image
											  const vector<Mat> & vDepths,		// input:	all depth map relative to reference wrt each support image
											  const vector<Mat> & vHxs,			// input:	all hx map relative to reference wrt each support image
											  const vector<Mat> & vHys,			// input:	all hy map relative to reference wrt each support image
											  const vector<Mat> & vScores,		// input:	all score map relative to reference wrt each support image
											  const Mat & mSel,					// input:	selected best support image index for each pixel who generates the most likely depth with reference image
											  Mat & mVisi,						// output:	augmented visibility within support image set based on selected most likely depth
											  double thresh_sigma = 1			// input:	image coordinate threshold for grouping depths with the most likely depth
											  );

void AugmentVisibility_basedonMostLikelyDepth(const vector<Matx33d> & vKs,				// input:	all interior matrix
											  const vector<Matx33d> & vRs,				// input:	all rotation matrix
											  const vector<Matx31d> & vts,				// input:	all translation vectors
											  const vector<double> & vfx_1,				// input:	
											  const vector<double> & vfy_1,				// input:
											  const vector<Mat> & vImgs,				// input:	all images
											  const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
											  int idx_ref,								// input:	the reference image index
											  const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
											  const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
											  const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
											  const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
											  const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
											  Mat & mVisi,								// output:	augmented visibility within support image set based on selected most likely depth
											  double thresh_sigma = 1					// input:	image coordinate threshold for grouping depths with the most likely depth
											  );

// 20140908, take into account not only the depth but also the normal
void AugmentVisibility_basedonMostLikelyDepthandNormals(const vector<Matx33d> & vKs,				// input:	all interior matrix
													    const vector<Matx33d> & vRs,				// input:	all rotation matrix
													    const vector<Matx31d> & vts,				// input:	all translation vectors
													    const vector<double> & vfx_1,				// input:	
													    const vector<double> & vfy_1,				// input:
													    const vector<Mat> & vImgs,				// input:	all images
													    const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
													    int idx_ref,								// input:	the reference image index
													    const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
													    const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
													    const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
													    const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
													    const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
													    Mat & mVisi,								// output:	augmented visibility within support image set based on selected most likely depth
													    double thresh_sigma = 1,					// input:	image coordinate threshold for grouping depths with the most likely depth
														double thresh_ang = 90						// input:
													    );

// 20140914, self-contained version, take into account not only the depth but also the normal
void AugmentVisibility_basedonMostLikelyDepthandNormals(const Matx33d & mK0,						// input:	interior matrix of the reference image
														const Matx33d & mR0,						// input:	rotation matrix of the reference image
														const Matx31d & mt0,						// input:	translation vector of the reference image
														const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
													    const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
													    const vector<Matx31d> & vts,				// input:	translation vectors of all support images
													    const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
													    const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
													    const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
													    const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
													    const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
													    Mat & mVisi,								// output:	augmented visibility within support image set based on selected most likely depth
													    double thresh_sigma = 1,					// input:	image coordinate threshold for grouping depths with the most likely depth
														double thresh_ang = 90						// input:
													    );

// 20141013, self-contained version, take into account not only the depth but also the normal
void AugmentVisibility_basedonMostLikelyDepthandNormals_SURE(const Matx33d & mK0,						// input:	interior matrix of the reference image
															 const Matx33d & mR0,						// input:	rotation matrix of the reference image
															 const Matx31d & mt0,						// input:	translation vector of the reference image
															 const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
															 const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
															 const vector<Matx31d> & vts,				// input:	translation vectors of all support images
															 const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
															 const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
															 const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
															 const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
															 const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
															 Mat & mVisi,								// output:	augmented visibility within support image set based on selected most likely depth
															 double thresh_sigma = 1,					// input:	image coordinate threshold for grouping depths with the most likely depth
															 double thresh_ang = 90						// input:
															 );

// 20141218, self-contained version, take into account not only the depth but also the normal
// one support image produce multiple candidates from different scale
// given dij denoting depth map generated from ith pyramid level with jth support image
// the depth maps in input vDepths are arranged as [d00 | d01 | d10 | d11 | d20 | d21 | d30 | d31]
void AugmentVisibility_basedonMostLikelyDepthandNormals_SURE_MultiScales(const Matx33d & mK0,						// input:	interior matrix of the reference image
																		 const Matx33d & mR0,						// input:	rotation matrix of the reference image
																		 const Matx31d & mt0,						// input:	translation vector of the reference image
																		 const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
																		 const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
																		 const vector<Matx31d> & vts,				// input:	translation vectors of all support images
																		 const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
																		 const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
																		 const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
																		 const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
																		 const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
																		 Mat & mVisi,								// output:	augmented visibility within support image set based on selected most likely depth
																		 double thresh_sigma = 1,					// input:	image coordinate threshold for grouping depths with the most likely depth
																		 double thresh_ang = 90						// input:
																		 );

// 20140904, visibility determined, then start depth fusion or optimization by taking account all observations
void DepthFusion_minimizingProjErrors(const vector<Matx33d> & vKs,				// input:	all interior matrix
									  const vector<Matx33d> & vRs,				// input:	all rotation matrix
									  const vector<Matx31d> & vts,				// input:	all translation vectors
									  const vector<double> & vfx_1,				// input:	
									  const vector<double> & vfy_1,				// input:
									  const vector<Mat> & vImgs,				// input:	all images
									  const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
									  int idx_ref,								// input:	the reference image index
									  const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
									  const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
									  const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
									  const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
									  const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
									  const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
									  Mat & mDepth,								// output:	fused depths
									  Mat & mHx,								// output:	fused hxs
									  Mat & mHy,								// output:	fused hys
									  Mat & mScore,								// output:	fused scores
									  int maxIter = 128,
									  double xEps = 1.0E-8,				// input: threshold
									  double fEps = 1.0E-6				// input: threshold
									  );

// 20140913, self-contained version, visibility determined, then start depth fusion or optimization by taking account all observations
void DepthFusion_minimizingProjErrors(const Matx33d & mK0,						// input:	interior matrix of reference image
								      const Matx33d & mR0,						// input:	rotation matrix of reference image
								      const Matx31d & mt0,						// input:	translation vector of reference image
									  const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
									  const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
									  const vector<Matx31d> & vts,				// input:	translation vectors of all support images
									  const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
									  const vector<Mat> & vHxs,					// input:	all hx map relative to reference wrt each support image
									  const vector<Mat> & vHys,					// input:	all hy map relative to reference wrt each support image
									  const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
									  const Mat & mSel,							// input:	selected best support image index for each pixel who generates the most likely depth with reference image
									  const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
									  Mat & mDepth,								// output:	fused depths
									  Mat & mHx,								// output:	fused hxs
									  Mat & mHy,								// output:	fused hys
									  Mat & mScore,								// output:	fused scores
									  int maxIter = 128,
									  double xEps = 1.0E-8,				// input: threshold
									  double fEps = 1.0E-6				// input: threshold
									  );

// 20140905, visibility determined, use the mean values of depth,hx and hy as initial values to optimize final depth and normal by MPGC
void MPGC_20140905(const vector<Matx33d> & vKs,				// input:	all interior matrix
				   const vector<Matx33d> & vRs,				// input:	all rotation matrix
				   const vector<Matx31d> & vts,				// input:	all translation vectors
				   const vector<double> & vfx_1,			// input:	
				   const vector<double> & vfy_1,			// input:
				   const vector<Mat> & vImgs,				// input:	all images
				   const vector<vector<int>> & vIdxSupports,// input:	all images' support images' index
				   int idx_ref,								// input:	the reference image index
				   const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
				   const vector<Mat> & vHxs,				// input:	all hx map relative to reference wrt each support image
				   const vector<Mat> & vHys,				// input:	all hy map relative to reference wrt each support image
				   const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
				   const Mat & mSel,						// input:	selected best support image index for each pixel who generates the most likely depth with reference image
				   const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
				   Mat & mDepth,							// output:	fused depths
				   Mat & mHx,								// output:	fused hxs
				   Mat & mHy,								// output:	fused hys
				   Mat & mScore,							// output:	fused scores
				   int pw,									// input:	image patch width
				   int ph,									// input:	image patch height
				   int maxIter = 128,
				   double xEps = 1.0E-8,					// input: threshold
				   double fEps = 1.0E-6						// input: threshold
				   );

// 20140910, visibility determined, use the mean values of depth,hx and hy as initial values to optimize final depth and normal by MPGC
void MPGC_20140910(const vector<Matx33d> & vKs,				// input:	all interior matrix
				   const vector<Matx33d> & vRs,				// input:	all rotation matrix
				   const vector<Matx31d> & vts,				// input:	all translation vectors
				   const vector<double> & vfx_1,			// input:	
				   const vector<double> & vfy_1,			// input:
				   const vector<Mat> & vImgs,				// input:	all images
				   const vector<vector<int>> & vIdxSupports,// input:	all images' support images' index
				   int idx_ref,								// input:	the reference image index
				   const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
				   const vector<Mat> & vHxs,				// input:	all hx map relative to reference wrt each support image
				   const vector<Mat> & vHys,				// input:	all hy map relative to reference wrt each support image
				   const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
				   const Mat & mDepth_ML,
				   const Mat & mHx_ML,
				   const Mat & mHy_ML,
				   const Mat & mSel,						// input:	selected best support image index for each pixel who generates the most likely depth with reference image
				   const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
				   Mat & mDepth,							// output:	fused depths
				   Mat & mHx,								// output:	fused hxs
				   Mat & mHy,								// output:	fused hys
				   Mat & mScore,							// output:	fused scores
				   int pw,									// input:	image patch width
				   int ph,									// input:	image patch height
				   double ratio = 0.01,
				   int maxIter = 128,
				   double xEps = 1.0E-8,					// input: threshold
				   double fEps = 1.0E-6						// input: threshold
				   );

// 20140910, self-contained, visibility determined, use the mean values of depth,hx and hy as initial values to optimize final depth and normal by MPGC
void MPGC_20140910(const Matx33d & mK0,						// input:	interior matrix of reference image
				   const Matx33d & mR0,						// input:	rotation matrix of reference image
				   const Matx31d & mt0,						// input:	translation vector of reference image
				   const Mat & img0,						// input:	reference image
				   const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
				   const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
				   const vector<Matx31d> & vts,				// input:	translation vectors of all support images
				   const vector<Mat> & vImgs,				// input:	images of all support images
				   const vector<Mat> & vDepths,				// input:	all depth map relative to reference wrt each support image
				   const vector<Mat> & vHxs,				// input:	all hx map relative to reference wrt each support image
				   const vector<Mat> & vHys,				// input:	all hy map relative to reference wrt each support image
				   const vector<Mat> & vScores,				// input:	all score map relative to reference wrt each support image
				   const Mat & mDepth_ML,
				   const Mat & mHx_ML,
				   const Mat & mHy_ML,
				   const Mat & mSel,						// input:	selected best support image index for each pixel who generates the most likely depth with reference image
				   const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
				   Mat & mDepth,							// output:	fused depths
				   Mat & mHx,								// output:	fused hxs
				   Mat & mHy,								// output:	fused hys
				   Mat & mScore,							// output:	fused scores
				   int pw,									// input:	image patch width
				   int ph,									// input:	image patch height
				   double ratio = 0.01,
				   int maxIter = 128,
				   double xEps = 1.0E-8,					// input: threshold
				   double fEps = 1.0E-6						// input: threshold
				   );

// 20150207, self-contained, visibility determined, use the most likely values of depth,hx and hy as initial values to optimize final depth and normal by MPGC
void MPGC_20150207(const Matx33d & mK0,						// input:	interior matrix of reference image
				   const Matx33d & mR0,						// input:	rotation matrix of reference image
				   const Matx31d & mt0,						// input:	translation vector of reference image
				   const Mat & img0,						// input:	reference image
				   const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
				   const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
				   const vector<Matx31d> & vts,				// input:	translation vectors of all support images
				   const vector<Mat> & vImgs,				// input:	images of all support images
				   const Mat & mDepth_ML,
				   const Mat & mHx_ML,
				   const Mat & mHy_ML,
				   const Mat & mScore_ML,					
				   const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
				   Mat & mDepth,							// output:	fused depths
				   Mat & mHx,								// output:	fused hxs
				   Mat & mHy,								// output:	fused hys
				   Mat & mScore,							// output:	fused scores
				   int pw,									// input:	image patch width
				   int ph,									// input:	image patch height
				   int maxIter = 128,
				   double xEps = 1.0E-8,					// input: threshold
				   double fEps = 1.0E-6						// input: threshold
				   );

// 20141215, self-contained, do mpgc with only one support image
void MPGC_Binocular_20141215(const Matx33d & mK0,	// input:	interior matrix of reference image
						     const Matx33d & mR0,	// input:	rotation matrix of reference image
						     const Matx31d & mt0,	// input:	translation vector of reference image
						     const Mat & img0,		// input:	reference image
						     const Matx33d & mK,	// input:	interior matrix of the support image
						     const Matx33d & mR,	// input:	rotation matrix of the support image
						     const Matx31d & mt,	// input:	translation vectors of the support image
						     const Mat & img,		// input:	the support image
						     Mat & mDepth,			// in&output:	initial and optimized depth
						     Mat & mHx,				// in&output:	initial and optimized hx
						     Mat & mHy,				// in&output:	initial and optimized hy
						     Mat & mScore,			// in&output:	initial and optimized score
						     int pw,				// input:	image patch width
						     int ph,				// input:	image patch height
						     int maxIter = 128,
						     double xEps = 1.0E-8,	// input: threshold
						     double fEps = 1.0E-6	// input: threshold
						     );

// invalidate those pixels whose normal estimate is obviously wrong by setting its score to -1
void InvalidatePixels_byNormal(const cam_data & cam0, const cam_data & cam,
							   const Mat & mDepth,
							   const Mat & mHx,
							   const Mat & mHy,
							   Mat & mScore,
							   int k,
							   double thresh_ang = 90
							   );

// invalidate those pixels whose normal estimate is obviously wrong by setting its score to -1
void InvalidatePixels_byNormal(const Matx33d & mK0, const Matx33d & mK,		// input: mK0 reference image, mK one specific support image
							   const Matx33d & mR0, const Matx33d & mR,		// input: mR0 reference image, mR one specific support image
							   const Matx31d & mt0, const Matx31d & mt,		// input: mt0 reference image, mt one specific support image
							   double fx0_1, double fx_1,					// input: fx0_1 = 1/fx0, fx_1 = 1/fx
							   double fy0_1, double fy_1,					// input: fy0_1 = 1/fy0, fy_1 = 1/fy
							   int idx0, int idx,
							   const Mat & mDepth,
							   const Mat & mHx,
							   const Mat & mHy,
							   Mat & mScore,
							   double thresh_ang = 90,
							   double thresh_ncc = 1.0E-4
							   );

// 20140914, self-contained version, invalidate those pixels whose normal estimate is obviously wrong by setting its score to -1
void InvalidatePixels_byNormal(const Matx33d & mK0, 		// input:	mK0 reference image
							   const Matx33d & mR0, 		// input:	mR0 reference image
							   const Matx31d & mt0, 		// input:	mt0 reference image
							   const Matx33d & mK,			// input:	mK one specific support image
							   const Matx33d & mR,			// input:	mR one specific support image
							   const Matx31d & mt,			// input:	mt one specific support image
							   const Mat & mDepth,
							   const Mat & mHx,
							   const Mat & mHy,
							   Mat & mScore,
							   double thresh_ang = 90,
							   double thresh_ncc = 1.0E-4
							   );

//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// whole MVDE methods package

// 20140827 with simplified MRF model only considering depth
void MVDE_package_01(const vector<cam_data> & vCams,			// input:	all images' orientations
					 const vector<Mat> & vImgs,					// input:	all images
					 const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
					 const vector<CloudPoint> & clouds,			// input:	the cloud points
					 vector<Mat> & vDepths,						// output:	all generated depths
					 vector<Mat> & vHxs,						// output:	all generated depth gradients
					 vector<Mat> & vHys,						// output:	all generated depth gradients
					 vector<Mat> & vScores,						// output:	all scores, and -1 indicates an invalid estimate
					 vector<Mat> & vVisis,						// output:	all estimated visibilities within corresponding support image set
					 int size = 5,								// input:	the window size of the image patch, should be odd number
					 double angLimit = 80,						// input:	the angular range limit of the normal of every object point, in angle, not radian
					 int maxIter = 4,							// input:	maximum iteration
					 double factor = 0.5,
					 int nRandSamp = 6,
					 double thresh_ang = 90,					// input:	the normal angle constraint, if this value is 360, then no constraint at all
					 double thresh_ncc = 0.95,					// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
					 double thresh_imgpt_sigma = 1,
					 double thresh_ratio = 0.001,
					 double thresh_ang_removeSmall = 85,
					 double thresh_area_removeSmall = 15,
					 int maxIter_optim = 128,
					 double xEps = 1.0E-8,						// input: threshold
					 double fEps = 1.0E-6						// input: threshold
					 );

// 20140909 with complete MRF model
void MVDE_package_02(const vector<cam_data> & vCams,			// input:	all images' orientations
					 const vector<Mat> & vImgs,					// input:	all images
					 const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
					 const vector<CloudPoint> & clouds,			// input:	the cloud points
					 vector<Mat> & vDepths,						// output:	all generated depths
					 vector<Mat> & vHxs,						// output:	all generated depth gradients
					 vector<Mat> & vHys,						// output:	all generated depth gradients
					 vector<Mat> & vScores,						// output:	all scores, and -1 indicates an invalid estimate
					 vector<Mat> & vVisis,						// output:	all estimated visibilities within corresponding support image set
					 int size = 5,								// input:	the window size of the image patch, should be odd number
					 double angLimit = 80,						// input:	the angular range limit of the normal of every object point, in angle, not radian
					 int maxIter = 4,							// input:	maximum iteration
					 double factor = 0.5,
					 int nRandSamp = 6,
					 double thresh_ang = 90,					// input:	the normal angle constraint, if this value is 360, then no constraint at all
					 double thresh_ncc = 0.95,					// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
					 double P1 = 1.5,
					 double P2 = 0.1,
					 double thresh_imgpt_sigma = 1,
					 double thresh_ratio = 0.001,
					 double thresh_ratio_depthdiscon = 0.01,
					 double thresh_ang_removeSmall = 85,
					 double thresh_area_removeSmall = 15,
					 int maxIter_optim = 128,
					 double xEps = 1.0E-8,						// input: threshold
					 double fEps = 1.0E-6						// input: threshold
					 );

// 20140914, parallel version, with complete MRF model
void MVDE_package_final(const CString & path_output,				// input:	the path of output file folder
						const vector<cam_data> & vCams,				// input:	all images' orientations
					    const vector<CString> & vPaths_imgs,		// input:	file paths of all input images
						const vector<Mat> & vSilhouettes,			// input:	Silhouettes of all images
					    const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
					    const vector<vector<CloudPoint>> & vClouds,	// input:	all point clouds, one for each input image
					    vector<Mat> & vDepths,						// output:	all generated depths
					    vector<Mat> & vHxs,							// output:	all generated depth gradients
					    vector<Mat> & vHys,							// output:	all generated depth gradients
					    vector<Mat> & vScores,						// output:	all scores, and -1 indicates an invalid estimate
					    vector<Mat> & vVisis,						// output:	all estimated visibilities within corresponding support image set
					    int size = 5,								// input:	the window size of the image patch, should be odd number
					    double angLimit = 80,						// input:	the angular range limit of the normal of every object point, in angle, not radian
					    int maxIter = 4,							// input:	maximum iteration
					    double factor = 0.5,
					    int nRandSamp = 6,
					    double thresh_ang = 90,						// input:	the normal angle constraint, if this value is 360, then no constraint at all
					    double thresh_ncc = 0.95,					// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
					    double P1 = 1.5,
					    double P2 = 0.1,
					    double thresh_imgpt_sigma = 1,
					    double thresh_ratio = 0.001,
					    double thresh_area_removeSmall = 15,
					    int maxIter_optim = 128,
					    double xEps = 1.0E-8,						// input: threshold
					    double fEps = 1.0E-6,						// input: threshold
						bool bMPGCFinal = true						// input: use MPGC optimized or directly Most likely results
					    );

// 20141209, for Zhou's data, the very original Multi-view patchmatch version
void MVDE_package_141209(const CString & path_output,					// input:	the path of output file folder
						 const vector<cam_data> & vCams,				// input:	all images' orientations
					     const vector<CString> & vPaths_imgs,			// input:	file paths of all input images
						 const vector<Mat> & vSilhouettes,			// input:	Silhouettes of all images
					     const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
					     const vector<vector<CloudPoint>> & vClouds,	// input:	all point clouds, one for each input image
					     vector<Mat> & vDepths,						// output:	all generated depths
					     vector<Mat> & vHxs,							// output:	all generated depth gradients
					     vector<Mat> & vHys,							// output:	all generated depth gradients
					     vector<Mat> & vScores,						// output:	all scores, and -1 indicates an invalid estimate
					     vector<Mat> & vVisis,						// output:	all estimated visibilities within corresponding support image set
					     int size = 5,								// input:	the window size of the image patch, should be odd number
					     double angLimit = 80,						// input:	the angular range limit of the normal of every object point, in angle, not radian
					     int maxIter = 4,								// input:	maximum iteration
					     double factor = 0.5,
					     int nRandSamp = 6,
					     double thresh_ang = 90,						// input:	the normal angle constraint, if this value is 360, then no constraint at all
					     double thresh_ncc = 0.95,					// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
					     double P1 = 1.5,
					     double P2 = 0.1,
					     double thresh_imgpt_sigma = 1,
					     double thresh_ratio = 0.001,
					     double thresh_area_removeSmall = 15,
					     int maxIter_optim = 128,
					     double xEps = 1.0E-8,						// input: threshold
					     double fEps = 1.0E-6							// input: threshold
					     );

// 20150206, for Zhou's data, the very original Multi-view patchmatch version
void MVDE_package_150206(const CString & path_output,				// input:	the path of output file folder
						 const vector<cam_data> & vCams,			// input:	all images' orientations
					     const vector<CString> & vPaths_imgs,		// input:	file paths of all input images
						 const vector<Mat> & vSilhouettes,			// input:	Silhouettes of all images
					     const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
					     const vector<CloudPoint> & vClouds,		// input:	all point clouds, one for each input image
					     vector<Mat> & vDepths,						// output:	all generated depths
					     vector<Mat> & vHxs,						// output:	all generated depth gradients
					     vector<Mat> & vHys,						// output:	all generated depth gradients
					     vector<Mat> & vScores,						// output:	all scores, and -1 indicates an invalid estimate
					     vector<Mat> & vVisis,						// output:	all estimated visibilities within corresponding support image set
					     int size = 5,								// input:	the window size of the image patch, should be odd number
					     double angLimit = 80,						// input:	the angular range limit of the normal of every object point, in angle, not radian
					     int maxIter = 4,							// input:	maximum iteration
					     double factor = 0.5,
					     int nRandSamp = 6,
					     double thresh_ang = 90,					// input:	the normal angle constraint, if this value is 360, then no constraint at all
					     double thresh_ncc = 0.95,					// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
					     double P1 = 1.5,
					     double P2 = 0.1,
					     double thresh_imgpt_sigma = 1,
						 int nLevel = 4,							// input:	number of pyramid levels
						 int idxOutLevel = 0,						// input:	output final results at pyramid level idxOutLevel, default is level 0, i.e. the highest resolution level
					     double thresh_ratio = 0.001,
					     double thresh_WH_Ratio_removeSmall = 0.02,	// input:	
					     int maxIter_optim = 128,
					     double xEps = 1.0E-8,						// input: threshold
					     double fEps = 1.0E-6,						// input: threshold
						 bool bMPGCFinal = true						// input: use MPGC optimized or directly Most likely results
					     );

// 20140827, conduct patchmatch for reference image with every support image at each time without view propagation
void PatchMatch_Binocular(const vector<Matx33d> & vKs,				// input:	all interior matrix
						  const vector<Matx33d> & vRs,				// input:	all rotation matrix
						  const vector<Matx31d> & vts,				// input:	all translation vectors
						  const vector<double> & vfx_1,				// input:	
						  const vector<double> & vfy_1,				// input:
					      const vector<Mat> & vImgs,				// input:	all images
						  const vector<vector<int>> & vIdxSupports,	// input:	all images' support images' index
					      const vector<CloudPoint> & clouds,		// input:	the cloud points
						  const vector<double> & vd_max,			// input:	maximal depth of each depth map
						  const vector<double> & vd_min,			// input:	minimal depth of each depth map
						  const vector<double> & vh_max,			// input:	maximal depth gradient of each depth map
						  const vector<double> & vh_min,			// input:	minimal depth gradient of each depth map
						  int idx_ref,								// input:	the reference image index
					      vector<Mat> & vDepths,					// output:	all the depth maps
					      vector<Mat> & vHxs,						// output:	all the hx maps
					      vector<Mat> & vHys,						// output:	all the hy maps
					      vector<Mat> & vScores,					// output:	all the score maps
					      int size = 5,								// input:	the window size of the image patch, should be odd number
					      double angLimit = 80,						// input:	the angular range limit of the normal of every object point, in angle, not radian
					      int maxIter = 4,							// input:	maximum iteration
					      double factor = 0.5,
					      int nRandSamp = 6
					      );

// 20140914, self-contained version, conduct patchmatch for reference image with every support image at each time without view propagation
void PatchMatch_Binocular(const Matx33d & mK0,				// input:	interior matrix of the reference image
						  const Matx33d & mR0,				// input:	rotation matrix of the reference image
						  const Matx31d & mt0,				// input:	translation vectors of the reference image
						  const Mat & img0,					// input:	the reference image
						  const vector<Matx33d> & vKs,		// input:	interior matrix of all support images
						  const vector<Matx33d> & vRs,		// input:	rotation matrix of all support images
						  const vector<Matx31d> & vts,		// input:	translation vectors of all support images
					      const vector<Mat> & vImgs,		// input:	all support images
					      const vector<CloudPoint> & clouds,// input:	the given sparse cloud points
					      vector<Mat> & vDepths,			// output:	all the depth maps
					      vector<Mat> & vHxs,				// output:	all the hx maps
					      vector<Mat> & vHys,				// output:	all the hy maps
					      vector<Mat> & vScores,			// output:	all the score maps
					      int size = 5,						// input:	the window size of the image patch, should be odd number
					      double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
					      int maxIter = 4,					// input:	maximum iteration
					      double factor = 0.5,
					      int nRandSamp = 6
					      );

// 20141013, self-contained version, conduct patchmatch for reference image with every support image at each time without view propagation
// with propagation between different layers.
void PatchMatch_Binocular_3DPropagation(const Matx33d & mK0,				// input:	interior matrix of the reference image
									    const Matx33d & mR0,				// input:	rotation matrix of the reference image
									    const Matx31d & mt0,				// input:	translation vectors of the reference image
									    const Mat & img0,					// input:	the reference image
									    const vector<Matx33d> & vKs,		// input:	interior matrix of all support images
									    const vector<Matx33d> & vRs,		// input:	rotation matrix of all support images
									    const vector<Matx31d> & vts,		// input:	translation vectors of all support images
									    const vector<Mat> & vImgs,			// input:	all support images
									    const vector<CloudPoint> & clouds,	// input:	the given sparse cloud points
									    vector<Mat> & vDepths,				// output:	all the depth maps
									    vector<Mat> & vHxs,					// output:	all the hx maps
									    vector<Mat> & vHys,					// output:	all the hy maps
									    vector<Mat> & vScores,				// output:	all the score maps
									    int size = 5,						// input:	the window size of the image patch, should be odd number
									    double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
									    int maxIter = 4,					// input:	maximum iteration
									    double factor = 0.5,
									    int nRandSamp = 6
									    );

// 20141215, self-contained version, conduct patchmatch for reference image with every support image at each time without view propagation
// with propagation between different layers.
// with initial estimates propagated from higher level on pyramid or lower resolution image.
void PatchMatch_Binocular_3DPropagation_givenInit(const Matx33d & mK0,				// input:	interior matrix of the reference image
												  const Matx33d & mR0,				// input:	rotation matrix of the reference image
												  const Matx31d & mt0,				// input:	translation vectors of the reference image
												  const Mat & img0,					// input:	the reference image
												  const vector<Matx33d> & vKs,		// input:	interior matrix of all support images
												  const vector<Matx33d> & vRs,		// input:	rotation matrix of all support images
												  const vector<Matx31d> & vts,		// input:	translation vectors of all support images
												  const vector<Mat> & vImgs,		// input:	all support images
												  const vector<CloudPoint> & clouds,// input:	the given sparse cloud points
												  vector<Mat> & vDepths,			// in&output:	all the depth maps
												  vector<Mat> & vHxs,				// in&output:	all the hx maps
												  vector<Mat> & vHys,				// in&output:	all the hy maps
												  vector<Mat> & vScores,			// in&output:	all the score maps
												  int size = 5,						// input:	the window size of the image patch, should be odd number
												  double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
												  int maxIter = 4,					// input:	maximum iteration
												  double factor = 0.5,				// input:	decreasing factor
												  double radius_factor = 0.01,		// input:	sampling radius factor
												  int nRandSamp = 6
												  );

// 20141013
void ComputeDepthUncertainBoundary(double imgxj0, double imgyj0,		// input:	the matching image point in support image
								   double dj0,							// input:	the depth of the reconstructed object point w.r.t the support image
								   double a1, double a2, double a3,		// input:	[a1 a2 a3]' = KjRjRr'nx
								   double & ddmin,						// output:	the depth decrement of the lower boundary w.r.t the reconstructed depth
								   double & ddmax,						// output:	the depth decrement of the upper boundary w.r.t the reconstructed depth
								   double thresh_imgpt_sigma = 1		// input:	the uncertainty of the matching image point 		
								   );

// enforce silhouette-consistency constraint
void VisualHullConstraint(int idx,							// input:	the index of current checked image
						  const vector<Matx33d> & vKs,		// input:	interior matrix of all images
						  const vector<Matx33d> & vRs,		// input:	rotation matrix of all images
						  const vector<Matx31d> & vts,		// input:	translation vectors of all images
						  const vector<Mat> & vSilhouettes,	// input:	silhouettes of objects
						  const Mat & mDepth,				// input:	depth map estimate
						  Mat & mScore,						// output:	the score map
						  int radius = 2					// input:
						  );

// 通过AfxMessageBox把矩阵显示在屏幕上
void PrintMatrix2Screen_double(const Mat & mat,
							   CString title = "",
							   WriteMode mode = WRITEMODE_LF
							   );

// 通过AfxMessageBox把矩阵显示在屏幕上
void PrintMatrix2Screen_uchar(const Mat & mat,
							  CString title = "",
							  WriteMode mode = WRITEMODE_LF
							  );

void SaveMat2File_double(CString path, const Mat & mat, WriteMode mode = WRITEMODE_LF);

// 计算两向量间夹角的余弦
double ComputeCosa(const Matx31d & vec1, const Matx31d & vec2);

// 计算两向量间夹角
double ComputeAngle(const Matx31d & vec1, const Matx31d & vec2);

void SaveParaField2Img(CString & path, const Mat & field);
void SaveScoreField2Img(CString & path, const Mat & mScore);

// Downsampling
void DownSample(const Mat & img,	// input:	original image
			    Mat & img_samp,		// output:	down sampled image
				int n = 2			// input:	sample one pixel every n pixels
				);

// augment the value field of a lower resolution image into a higher resolution image
void AugmentField(const Mat & field,		// input:	lower resolution image to be augmented
				  Mat & field_aug,			// output:	the augmented field
				  int width, int height,	// input:	size of augmented field
				  int n = 2,				// input:	replicate value of one pixel for n pixels
				  double factor = 1			// input:	optional input, factor
				  );

// augment the value field of a lower resolution image into a higher resolution image
void AugmentField_Interp(const Mat & field,		// input:	lower resolution image to be augmented
						 Mat & field_aug,			// output:	the augmented field
						 int width, int height,	// input:	size of augmented field
						 int n = 2,				// input:	replicate value of one pixel for n pixels
						 double factor = 1			// input:	optional input, factor
						 );

// pyramid 20141211
void Pyramid(const Matx33d & mK0,				// input:	interior matrix of the reference image
			 const Matx33d & mR0,				// input:	rotation matrix of the reference image
			 const Matx31d & mt0,				// input:	translation vectors of the reference image
			 const Mat & img0,					// input:	the reference image
			 const vector<Matx33d> & vKs,		// input:	interior matrix of all support images
			 const vector<Matx33d> & vRs,		// input:	rotation matrix of all support images
			 const vector<Matx31d> & vts,		// input:	translation vectors of all support images
			 const vector<Mat> & vImgs,			// input:	all support images
			 const vector<CloudPoint> & clouds,	// input:	the given sparse cloud points
			 int size = 5,						// input:	the window size of the image patch, should be odd number
			 double thresh_ncc = 0.8,			// input:	the threshold within which to be considered as a successful ncc
			 double angLimit = 80,				// input:	the angular range limit of the normal of every object point, in angle, not radian
			 int maxIter = 4,					// input:	maximum iteration
			 double factor = 0.5,				// input:	decreasing factor
			 int nRandSamp = 6,					// input:	number of times of random sampling
			 int nLevel = 4,					// input:	number of pyramid levels
			 int idxOutLevel = 0				// input:	output final results at pyramid level idxOutLevel, default is level 0, i.e. the highest resolution level
			 );

// 20150207, zhaokunz, pyramid
// 目标是通过采用金字塔机制（应对若纹理、减少计算量）为后续的多图 MPGC 提供合适的深度和法向的优化初值，以及每个像素相对于每幅支持图像的可见性
// 函数可以输出低于源图像分辨率的深度及法向图，并不一定非得是源尺寸
// 当然如果输出低分辨率的深度及法向图，输出的深度及法向图的内参数也是成比例改变的
void SurfaceMapEstimation_oneImg_Pyramid(const Matx33d & mK0,					// input:	interior matrix of the reference image
									     const Matx33d & mR0,					// input:	rotation matrix of the reference image
									     const Matx31d & mt0,					// input:	translation vectors of the reference image
									     const Mat & img0,						// input:	the reference image
									     const vector<Matx33d> & vKs,			// input:	interior matrix of all support images
									     const vector<Matx33d> & vRs,			// input:	rotation matrix of all support images
									     const vector<Matx31d> & vts,			// input:	translation vectors of all support images
									     const vector<Mat> & vImgs,				// input:	all support images
									     const vector<CloudPoint> & clouds,		// input:	the given sparse cloud points
										 CString path_output,					// input:	the path of output file folder
										 Mat & mDepth_final,					// output:	final depth map，为输出层的分辨率
										 Mat & mHx_final,						// output:	final hx map，为输出层的分辨率
										 Mat & mHy_final,						// output:	final hy map，为输出层的分辨率
										 Mat & mScore_final,					// output:	final score map，为输出层的分辨率
										 Mat & mVisi_final,						// output:	final visi map，为输出层的分辨率
										 Mat & img0_outLevel,					// output:	the low resolution reference image in the out level
										 int size = 5,							// input:	the window size of the image patch, should be odd number
										 double angLimit = 80,					// input:	the angular range limit of the normal of every object point, in angle, not radian
										 int maxIter = 4,						// input:	maximum iteration
										 double factor = 0.5,					// input:	PatchMatch的时候随机采样的半径缩小因子
										 int nRandSamp = 6,						// input:	PatchMatch中在随机搜索环节进行随机采样的次数
										 double thresh_ang = 90,				// input:	the normal angle constraint, if this value is 360, then no constraint at all
										 double thresh_ncc = 0.95,				// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
										 double P1 = 1.5,						// input:	MRF时候对深度加的惩罚因子
										 double P2 = 0.1,						// input:	MRF时候对法向加的惩罚因子
										 double thresh_imgpt_sigma = 1,			// input:	经验的像点匹配精度，拓展可见性时需要用到
									     int nLevel = 4,						// input:	number of pyramid levels
									     int idxOutLevel = 0,					// input:	output final results at pyramid level idxOutLevel, default is level 0, i.e. the highest resolution level
										 int maxIter_optim = 128,				
										 double xEps = 1.0E-8,					// input: threshold
										 double fEps = 1.0E-6,					// input: threshold
										 bool bMPGCFinal = true					// input: use MPGC optimized or directly Most likely results
									     );

void SaveFeatures2File(CString path, const vector<KeyPoint> & keypoints);
void SaveFeatures2File(CString path, const Features & feats);

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
Matx33d NormalizePts_2D(const vector<Point2f> & pts, vector<Point2f> & pts_t);

// zhaokunz, 20150107, compute the epipole in the left image from fundamental matrix
// Fe = 0, i.e. e is the right null-vector of F
Matx31d Epipole_Left(const Matx33d & mF);

// zhaokunz, 20150107, compute the epipole in the right image from fundamental matrix
// Fte' = 0, i.e. e' is the right null-vector of Ft
Matx31d Epipole_Right(const Matx33d & mF);

Matx33d CrossMat(const Matx31d & v);
Matx33d CrossMat(const Matx13d & v);

// P0 = [I|0], P1 = [[e']x F | e']
void GetCameraMatfromF(const Matx33d & mF, Matx34d & mP0, Matx34d & mP1);

// 20150111, zhaokunz, 计算返回二维点坐标 (x, y) 至二维直线 Ax + By + C = 0 的距离
double CalcuDistance_Pt2Line_2D(double x, double y,					// 二维点坐标
								double A, double B, double C		// 二维直线表达式
								);

// 20150111, zhaokunz, get the inliers given the fundamental matrix
int GetInliers(const vector<KeyPoint> & pts0,	// input:	keypoints in the left image
			   const vector<KeyPoint> & pts1,	// input:	keypoints in the right image
			   const Matx33d & mF,				// input:	the given fundamental matrix
			   const vector<DMatch> & matches,	// input:	the given initial matches
			   vector<uchar> & status,			// output:	with 1 indicating an inlier, and 0 indicating an outlier
			   double thresh = 3.0				// input:	the point to epipolar line distance threshold
			   );

// 20150112, zhaokunz, get the inliers given the fundamental matrix
int GetInliers(const vector<KeyPoint> & pts0,	// input:	keypoints in the left image
			   const vector<KeyPoint> & pts1,	// input:	keypoints in the right image
			   const Matx33d & mF,				// input:	the given fundamental matrix
			   const vector<DMatch> & matches,	// input:	the given initial matches
			   vector<DMatch> & matches_out,	// output:	all inliers
			   double thresh = 3.0				// input:	the point to epipolar line distance threshold
			   );

// 20150131, zhaokunz, 
void GetInliers_knn(const vector<KeyPoint> & pts0,			// input:	keypoints in the left image
					const vector<KeyPoint> & pts1,			// input:	keypoints in the right image
					const Matx33d & mF,						// input:	the given fundamental matrix
					const vector<vector<DMatch>> & matches,	// input:	the given initial matches
					vector<DMatch> & matches_out,			// output:	all inliers
					double thresh = 3.0						// input:	the point to epipolar line distance threshold
					);

// 20150112, zhaokunz, delete identical matches
void DeleteIdenticalMatches(const vector<DMatch> & matches, vector<DMatch> & matches_out);
// 20150112, zhaokunz, enforce one-to-one constraint
void EnsureOne2OneMatches(const vector<DMatch> & matches, vector<DMatch> & matches_out);

// 20150112, zhaokunz
void Optim_F_Matches(const vector<KeyPoint> & keys0,		// input:	keypoints in the left image
					 const vector<KeyPoint> & keys1,		// input:	keypoints in the right image
					 const Matx33d & mF_init,				// input:	the initial fundamental matrix
					 const vector<DMatch> & matches_init,	// input:	the big set of matches, containing outliers
					 Matx33d & mF_optim,					// output:	the optimized fundamental matrix using updated set of matches
					 vector<DMatch> & matches_optim,		// output:	the optimized set of inlier matches
					 int maxIter = 10,						// input:	the maximum number of iterations
					 double xEps = 1.0E-8,					// input:	threshold
					 double fEps = 1.0E-6,					// input:	threshold
					 double thresh = 3.0					// input:	the point to epipolar line distance threshold
					 );

// 20150114, zhaokunz, P=K[I|0], P'=K'[R|t]
// F = inv(K'')[t]xRinv(K)
Matx33d GetF_Stereo(const Matx33d & mK0,	// input:	calibration matrix of the reference image
					const Matx33d & mK1,	// input:	calibration matrix of the matching image
					const Matx33d & mR1,	// input:	rotation matrix of the matching image
					const Matx31d & mt1		// input:	translation vector of the matching image
					);

// 20150125, zhaokunz, the optimal triangulation based on two view epipolar geometry
double Triangulate_Optimal(const vector<Point2d> & imgpts0,	// input:	the distortion free measured image points in 1st image
						   const Matx33d & mK0,				// input:	the calibration matrix of the 1st image
						   const Matx33d & mR0,				// input:	the rotation matrix of the 1st image
						   const Matx31d & mt0,				// input:	the translation vector of the 1st image
						   const vector<Point2d> & imgpts1,	// input:	the distortion free measured image points in 2nd image
						   const Matx33d & mK1,				// input:	the calibration matrix of the 2nd image
						   const Matx33d & mR1,				// input:	the rotation matrix of the 2nd image
						   const Matx31d & mt1,				// input:	the translation vector of the 2nd image
						   vector<Point3d> & wrdpts,		// output:	the triangulated 3d points
						   vector<Point2d> & errs			// output:	the triangulation errors
						   );

void OutputPointCloud(CString strFile,
					  const vector<cam_data> & cams,
					  vector<CloudPoint> & cloud
					  );

//////////////////////////////////////////////////////////////////////////

}