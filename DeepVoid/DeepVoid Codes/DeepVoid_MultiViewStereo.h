/*************************************************************************************/
/* All Multi-View Stereo related functions are here.                                 */
/* Author: Jacob Zhu (朱肇昆)                         Date:2015.02.09                */
/*************************************************************************************/

#pragma once

// #include "DeepVoid Codes/DeepVoid_DataType.h"

namespace DeepVoid_MVS
{

const double R2D = 180.0/CV_PI;
const double D2R = CV_PI/180.0;

// zhaokunz, 20140406, fast floor and ceil, downloaded from http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
inline int floor_fast(double fp)
{
	return int(fp + 65536.) - 65536;
}

inline int ceil_fast(double fp)
{
	return 65536 - int(65536. - fp);
}

inline int floor_fast_noshift(double fp)
{
	int i = int(fp); 
	return i > fp ? i - 1 : i;
}


inline int ceil_fast_noshift(double fp)
{
	int i = int(fp);
	return i < fp ? i + 1 : i;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 计算角度的cos值，输入为角度
double cosd(double angle);

// 图像特征类型枚举
enum FeatureType
{
	Feature_SIFT       = 0,
	Feature_SURT       = 1,
};

// 图像特征封装的结构
struct Features
{
	std::vector<cv::KeyPoint> key_points;
	cv::Mat descriptors;
	std::vector<int> tracks;

	// zhaokunz, 20140324, sometimes multiple keypoints correspond to the same image point
	// like the SIFT features, a feature can have multiple orientations
	// this idx indicates the unique index of this keypoint, if two keypoints correspond to the same image point
	// their indices will be the same, otherwise will be different
	std::vector<int> idx_pt;

	FeatureType type;

	Features()
	{
		type = Feature_SIFT;
	};

	Features & operator = (const Features & otherFeat)
	{
		if (this != &otherFeat)
		{
			type = otherFeat.type;
			key_points = otherFeat.key_points;
			descriptors = otherFeat.descriptors.clone();
			tracks = otherFeat.tracks;
			idx_pt = otherFeat.idx_pt;
		}

		return *this;
	};

	Features(const Features & feat)
	{
		*this = feat;
	};
};

struct CloudPoint_ImgInfo
{
	int m_idxImg;					// indicates in which image is this cloud point observed
	int m_idxImgPt;					// indicates which image point does this cloud point correspond to in this image
	Vec3b m_rgb;					// what is the RGB value of this cloud point in this image
	double m_rpjErr;				// what is the reprojection error of this cloud point in this image

	CloudPoint_ImgInfo()
	{
		m_idxImg = m_idxImgPt = -1;
		m_rgb.val[0] = m_rgb.val[1] = m_rgb.val[2] = 0;
		m_rpjErr = 0;
	};

	CloudPoint_ImgInfo & operator = (const CloudPoint_ImgInfo & otherInfo)
	{
		if (this != &otherInfo)
		{
			m_idxImg = otherInfo.m_idxImg;
			m_idxImgPt = otherInfo.m_idxImgPt;
			m_rgb = otherInfo.m_rgb;
			m_rpjErr = otherInfo.m_rpjErr;
		}

		return *this;
	};

	// 20150124, zhaokunz, override the == operator, so that functions like find(first, end, value) can be used to determine equality
	bool operator == (const CloudPoint_ImgInfo & otherInfo)
	{
		// the imgInfo equals the other one only if they are from the same image and the same image point
		return (m_idxImg == otherInfo.m_idxImg && m_idxImgPt == otherInfo.m_idxImgPt);
	};

	CloudPoint_ImgInfo(const CloudPoint_ImgInfo & info)
	{
		*this = info;
	};
};

struct CloudPoint
{
	int m_idx;									// the global index of this cloud point in the whole point cloud
	Point3d m_pt;								// the 3d coordinates of this cloud point
	vector<CloudPoint_ImgInfo> m_vImgInfos;		// all the image infos of this cloud point in all visible images

	CloudPoint()
	{
		m_idx = -1;
		m_pt.x = m_pt.y = m_pt.z = 0;
	};

	CloudPoint & operator = (const CloudPoint & otherPt)
	{
		if (this != &otherPt)
		{
			m_idx = otherPt.m_idx;
			m_pt = otherPt.m_pt;
			m_vImgInfos = otherPt.m_vImgInfos;
		}

		return *this;
	};

	CloudPoint(const CloudPoint & pt)
	{
		*this = pt;
	};
};

// 像机参数结构体
struct cam_data
{
	double fx; double fy;   // equivalent focal length
	double cx; double cy;   // principal point
	double s;	            // skew factor

	double R[9];            // rotation matrix
	double t[3];            // translation vector

	double k[5];            // distortion

	int dist_type;          // distortion type, 0 is Weng's, 1 is D.C.Brown's

	Features m_feats;		// image features

	cam_data()
	{
		fx = -1;	fy = -1;
		cx = -1;	cy = -1;
		s  = -1;

		int i;
		for (i = 0; i < 9; i++)
		{
			R[i] = -100;
		}

		for (i = 0; i < 3; i++)
		{
			t[i] = -1;
		}

		for (i = 0; i < 5; i++)
		{
			k[i] = -1;
		}
	};

	cam_data & operator = (const cam_data & otherCam)
	{
		if (this != &otherCam)
		{
			fx = otherCam.fx; fy = otherCam.fy;
			cx = otherCam.cx; cy = otherCam.cy;
			s  = otherCam.s;

			memcpy(R, otherCam.R, 9*sizeof(double));
			memcpy(t, otherCam.t, 3*sizeof(double));
			memcpy(k, otherCam.k, 5*sizeof(double));

			dist_type = otherCam.dist_type;

			m_feats = otherCam.m_feats;
		}

		return *this;
	};

	cam_data(const cam_data & cam)
	{
		*this = cam;
	};
};

// 20150209, 多视图立体视觉的总函数
void MVS_Pyramid(const CString & path_output,				// input:	the path of output file folder
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

// Downsampling
void DownSample(const Mat & img,	// input:	original image
			    Mat & img_samp,		// output:	down sampled image
				int n = 2			// input:	sample one pixel every n pixels
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

void MakeSureNotOutBorder(int x, int y,				// input:	original center of rect
	                      int & x_new, int & y_new,	// output:	new center of rect, making sure the new rect with the same size are within border
						  int wndSizeHalf,
						  int imgWidth, int imgHeight
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

// the normal n(a,b,1) is the opposite normal, the actual normal is -n(-a,-b,-1)
void get_normal_givendrhxhy(double fx, double fy,		// input:	equivalent focal length
							double nimgx, double nimgy,	// input:	the normalized image point
							double dr,					// input:	the depth of given pixel
							double hx, double hy,		// input:	the depth gradient
							double & a, double & b		// output:	the normal is n(a,b,1)
							);

// Implementation of bilinear interpolation
double BilinearInterp(double x, double y,	// the coordinates of position to be interpolated
					  double x1,			// the x coordinate of topleft f1 and bottomleft f3
					  double x2,			// the x coordinate of topright f2 and bottomright f4
					  double y1,			// the y coordinate of topleft f1 and topright f2
					  double y2,			// the y coordinate of bottomleft f3 and bottomright f4
					  double f1, double f2, double f3, double f4	// the corresponding values of topleft,topright,bottomleft and bottomright points
					  );

bool BilinearInterp(const Mat & img,		// input:	the image data
					double x, double y,		// input:	the coordinates of position to be interpolated
					double & r,				// output:	the interpolated R
					double & g,				// output:	the interpolated G
					double & b				// output:	the interpolated B
					);

bool BilinearInterp(const Matx33d & mK,				// input:	the camera matrix
					const Matx33d & mR,				// input:	the rotation matrix
					const Matx31d & mt,				// input:	the translation vector
					const Mat & img,				// input:	the input image
					double X, double Y, double Z,	// input:	the coordinates of position to be interpolated
					double & r,						// output:	the interpolated R
					double & g,						// output:	the interpolated G
					double & b,						// output:	the interpolated B
					double * imgpt_x = NULL,
					double * imgpt_y = NULL
					);

void DetermineInterval(double val_max, double val_min, double val_cur, double radius, double & inter_max, double & inter_min);

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

bool MPGC_get_patch_gx_gy_roberts(const Mat & img,					// input:	the image data to be resampled
								  const vector<Point2d> & vImgPts,	// input:	the image coordinates of all the samples
								  int patchWidth, int patchHeight,	// input:	the size of the image patch
								  Mat & mPatch,						// output:	the resampled image patch
								  Mat & mGx,						// output:	the dI/dx matrix of every pixel in image patch
								  Mat & mGy							// output:	the dI/dy matrix of every pixel in image patch
								  );

// support window中某像素表征的三维点世界坐标对该support window的dr hx hy求导
Matx33d Jacobian_XYZ_drhxhy(const Matx31d & mRtx,			// input:	R'x(u,v), transpose of R * normalized image points of (u,v)
							double u, double v				// input:	local offset coordinates of this pixel w.r.t central pixel
							);

// 投影像点坐标对对应的空间点世界坐标求导
Matx23d Jacobian_xy_XYZ(const Matx33d & mKR,			
						const Matx31d & mKt,
						double X, double Y, double Z,
						double & imgx, double & imgy
						);

// 计算两向量间夹角的余弦
double ComputeCosa(const Matx31d & vec1, const Matx31d & vec2);

// augment the value field of a lower resolution image into a higher resolution image
void AugmentField_Interp(const Mat & field,		// input:	lower resolution image to be augmented
						 Mat & field_aug,			// output:	the augmented field
						 int width, int height,	// input:	size of augmented field
						 int n = 2,				// input:	replicate value of one pixel for n pixels
						 double factor = 1			// input:	optional input, factor
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

// 20141013
void ComputeDepthUncertainBoundary(double imgxj0, double imgyj0,		// input:	the matching image point in support image
								   double dj0,							// input:	the depth of the reconstructed object point w.r.t the support image
								   double a1, double a2, double a3,		// input:	[a1 a2 a3]' = KjRjRr'nx
								   double & ddmin,						// output:	the depth decrement of the lower boundary w.r.t the reconstructed depth
								   double & ddmax,						// output:	the depth decrement of the upper boundary w.r.t the reconstructed depth
								   double thresh_imgpt_sigma = 1		// input:	the uncertainty of the matching image point 		
								   );

uchar GetVisibilityVector_uchar(const vector<bool> & vbools);

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

void InterpVisiVector_uchar(uchar visi, vector<bool> & vec, int * nValid = NULL);

void SaveParaField2Img(CString & path, const Mat & field);
void SaveScoreField2Img(CString & path, const Mat & mScore);

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

// 20140913, self-contained version
void RemoveSmallDepthRegions_4(const Matx33d & mK,			// input:	reference image
							   const Matx33d & mR,			// input:	reference image
							   const Matx31d & mt,			// input:	reference image
							   const Mat & mDepth,
							   Mat & mScore,
							   double thresh_ratio = 0.01,
							   int thresh_area = 15
							   );

// 20150204, zhaokunz, try to score all other oriented images for dense matching with designated reference image based on certain measure
void ScoreMatchingImages(const vector<cam_data> & allCams,			// input:	all images
						 const vector<CloudPoint> & clouds,			// input:	all reconstructed tracks
						 const vector<vector<DMatch>> & allMatches,	// input:	all pair-wise matches
						 int idxRefImg,								// input:	the index of the reference image
						 vector<Point2d> & scores,					// output:	scores of all other images, with x being the index, and y being the score, in descending order
						 double ang_desired = 15					// input:	the desired triangulation angle
						 );

// compute exp((-1/2)*(x-miu)^2/sigma^2)
double exp_miu_sigma(double x, double miu, double sigma);
}