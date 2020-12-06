/************************************************************************************/
/* Johnny Zhu's own library, including kinds of self-defined data types and classes.*/
/* Author: Johnny Zhu (DeepVoid) in Melbourne                        Date:2014.03.02*/
/************************************************************************************/

#pragma once

// #include "opencv/cv.h"
// #include "opencv/highgui.h"
// #include "opencv/cvaux.hpp"
#include "opencv2\opencv.hpp"
// #include "opencv2/nonfree/nonfree.hpp"
#include "opencv2\xfeatures2d\nonfree.hpp"
#include "opencv2\viz\vizcore.hpp"

extern "C" void
CUDA_GenerateDSI_BT(int w, int h,					// input: the width and height of stereo images
					const unsigned char * h_imgb,	// input: h*w, the rectified grayscale base image
					const unsigned char * h_imgm,	// input: h*w, the rectified grayscale matching image
					int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
					int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
					double * h_DSI,					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
					int w_block,					// input: how many expected threads per row in a thread block
					int h_block,					// input: how many expected threads per colomn in a thread block
					int dir	= 0						// 0:l2r. 1:r2l. 2:u2b. 3:b2u
					);

extern "C" void
CUDA_GenerateDSI(int w, int h,					// input: the width and height of stereo images
				 int hw, int hh,				// input: the half width and half height of the support window
				 const unsigned char * h_imgb,	// input: h*w, the rectified grayscale base image
				 const unsigned char * h_imgm,	// input: h*w, the rectified grayscale matching image
				 int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
				 int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
				 double * h_DSI,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
				 int w_block,					// input: how many expected threads per row in a thread block
				 int h_block,					// input: how many expected threads per colomn in a thread block
				 int dir = 0,					// 0:l2r. 1:r2l. 2:u2b. 3:b2u
				 int costType = 0               // input: 0:BT,  1:ncc, 2:opencvncc
				 );

extern "C" void
CUDA_CostAggregation_OneDir(int w, int h, int nd,		// input: the width and height of stereo images, and the number of disparities
							const double * h_DSI,		// input: h*w*nd, the Disparity Space Image
							double * h_C,				// output:h*w*nd, the aggregated cost volume along this direction
							double P1,					// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
							double P2,					// input: a larger constant penalty for all larger disparity changes
							int n_block,				// input: how many expected threads in a thread block
							int dir						// 0:l2r. 1:r2l. 2:u2b. 3:b2u
							);

/*extern "C"*/ void
CUDA_CostAggregation_OneDir_new(int w, int h, int nd,		// input: the width and height of stereo images, and the number of disparities
								const double * h_DSI,		// input: h*w*nd, the Disparity Space Image
								double * h_C,				// output:h*w*nd, the aggregated cost volume along this direction
								double P1,					// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
								double P2,					// input: a larger constant penalty for all larger disparity changes
								int n_block,				// input: how many expected threads in a thread block
								int dir						// 0:l2r. 1:r2l. 2:u2b. 3:b2u
								);

/*extern "C"*/ void
CUDA_AddVec_double(double * h_A,		// input & output: A[i] += B[i]
				   const double * h_B,	// input: 
				   int n,				// input: number of elements
				   int nThreads			// input: number of threads in a thread block
				   );

extern "C" void
CUDA_transpose_uchar(unsigned char * h_B,		// output:B = A'
					 const unsigned char * h_A,	// input: A 
					 int w, int h,				// input: width and height of A
					 int nThreads				// input: m, the tile or thread block is m*m
					 );

extern "C" void
CUDA_transpose_tile_uchar(unsigned char * h_B,			// output:B = A'
						  const unsigned char * h_A,	// input: A 
						  int w, int h,					// input: width and height of A
						  int nThreads					// input: m, the tile or thread block is m*m
						  );

// 20170102, 3D thread block
/*extern "C"*/ void
CUDA_GenerateDSI_new(int w, int h,					// input: the width and height of stereo images
					 int hw, int hh,				// input: the half width and half height of the support window
					 const unsigned char * h_imgb,	// input: h*w, the rectified grayscale base image
					 const unsigned char * h_imgm,	// input: h*w, the rectified grayscale matching image
					 int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
					 int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
					 double * h_DSI,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
					 int w_block,					// input: how many expected threads per row in a thread block
					 int h_block,					// input: how many expected threads per colomn in a thread block
					 int k_block,					// input: how many expected threads per pillar in a thread block
					 int nTile,						// input: m, the transpose tile or thread block is m*m
					 int dir = 0,					// input: 0:l2r. 1:r2l. 2:u2b. 3:b2u
					 int costType = 0               // input: 0:BT,  1:ncc, 2:opencvncc
					 );

// 20170119
/*extern "C"*/ void
CUDA_PatchMatch(const unsigned char * h_imgb,	// input: the base gray image
				const unsigned char * h_imgm,	// input: tha matching gray image
				int w_b, int h_b,				// input: the width and height of base image
				int w_m, int h_m,				// input: the width and height of matching image
				double * h_depth,				// output:the estimated depth map of the base image
				double * h_alpha,				// output:the estimated surface normal map of the base image
				double * h_beta,				// output:the estimated surface normal map of the base image
				int nThreads_w,					// input: the number of threads per row of the thread block
				int nThreads_h,					// input: the number of threads per column of the thread block
				unsigned long long randSeed,	// input: the random seed
				double min_d, double max_d,		// input: the minimum and maximum depth
				double min_a, double max_a,		// input: the minimum and maximum alpha
				double min_b, double max_b		// input: the minimum and maximum beta
				);

//extern "C" void
//forCUDA_ShowInfo(const char * info)
//{
//	CString str(info);
//	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(str);
//}

using namespace std;
using namespace cv;

namespace DeepVoid
{

#define	FTOI(a)			( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )

const double R2D = 180/CV_PI;
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

// 定义一个关于旋转顺序的枚举类型
enum RotationOrder
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

// 计算角度的cos值，输入为角度
double cosd(double angle);

// 计算角度的sin值，输入为角度
double sind(double angle);

// 计算角度的tan值，输入为角度
double tand(double angle);

// 计算输入数值的arccosine值，返回值范围为0 - 180°
double acosd(double x);

// 计算输入数值的arcsine值，返回值范围为-90 - 90°
double asind(double x);

// 计算输入数值的arctangent值，返回值范围为-90 - 90°
double atand(double x);

// 计算(y / x)的arctangent值，由于可以综合考虑y和x的符号，因此返回值的范围拓展到四个象限，即-180 - 180°
double atand(double y, double x);

// 判断输入 x 的符号，为正则返回 1，为负则返回 -1
double Sign_Johnny(double x);

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
					uchar & r,				// output:	the interpolated R
					uchar & g,				// output:	the interpolated G
					uchar & b				// output:	the interpolated B
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
					uchar & r,						// output:	the interpolated R
					uchar & g,						// output:	the interpolated G
					uchar & b						// output:	the interpolated B
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

void MakeSureNotOutBorder(int x, int y,				// input:	original center of rect
	                      int & x_new, int & y_new,	// output:	new center of rect, making sure the new rect with the same size are within border
						  int wndSizeHalf,
						  int imgWidth, int imgHeight
						  );

// 20200823，计算一圆形支持区域内的图像灰度质心
bool IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
									  double x, double y,			// input: the center of the region
									  int r,						// input: the radius of the circular region
									  double & xIC, double & yIC	// output:the location of the calculated intensity centroid
									  );

// 20200824，计算一圆形支持区域内的图像灰度质心，通过相对于中心像素坐标偏移的方式表示
bool IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
									  int ix, int iy,				// input: the center of the region
									  int r,						// input: the radius of the circular region
									  double & dxIC, double & dyIC,	// output:the location of the calculated intensity centroid (in terms of offsets)
									  double * pm00_1 = NULL		// output:1/m00
									  );

// 20200824，计算一圆形支持区域内的图像灰度质心，通过相对于中心像素坐标偏移的方式表示
// 20201205，给定图像灰度高斯随机噪声的标准差，输出计算得到的灰度质心坐标不确定度标准差
bool IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
									  int ix, int iy,				// input: the center of the region
									  int r,						// input: the radius of the circular region
									  double & dxIC, double & dyIC,	// output:the location of the calculated intensity centroid (in terms of offsets)
									  double sigma_I,				// input: the standard deviation of the Gaussian random noise of image intensity
									  double & sigma_xIC, double & sigma_yIC	// output: the standard deviation of the x y coordinates of the intensity centroid propagated by the random noise of intensity
									  );

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
bool CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
					int ix, int iy,			// input: the center of the region
					int r,					// input: the radius of the circular region
					double & angle			// output:the location of the calculated intensity centroid (in terms of offsets)
					);

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 20201206，给定图像灰度高斯随机噪声的标准差，输出计算得到的特征方向的不确定度标准差
bool CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
					int ix, int iy,			// input: the center of the region
					int r,					// input: the radius of the circular region
					double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
					double & sigma_angle,	// output:the standard deviation of the corner angle propagated by the random noise of intensity
					double sigma_I = 5.0	// input: the standard deviation of the Gaussian random noise of image intensity					
					);

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
bool CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
					double x, double y,		// input: the center of the region
					int r,					// input: the radius of the circular region
					double & angle			// output:the location of the calculated intensity centroid (in terms of offsets)
					);

// 20200901，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 特征的区域半径由可以可靠确定特征方向角度时的区域大小来确定
// 角度范围为[0,360)
bool FeatureRadiusAngle(const cv::Mat & img,		// input: the input gray scale image
						int ix, int iy,				// input: the center of the region
						int & r,					// output:the estimated radius of the circular region
						double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
						double threshOffset = 1.0,	// input: 当灰度质心偏移量能超过该阈值像素数时才认为可靠确定特征方向，由此定下特征尺度大小
						int r_max = 100				// input: 允许的最大邻域半径值
						);

bool FeatureRadiusAngle(const cv::Mat & img,		// input: the input gray scale image
						double x, double y,			// input: the center of the region
						int & r,					// output:the estimated radius of the circular region
						double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
						double threshOffset = 1.0,	// input: 当灰度质心偏移量能超过该阈值像素数时才认为可靠确定特征方向，由此定下特征尺度大小
						int r_max = 100				// input: 允许的最大邻域半径值
						);

// 20200901，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 特征的区域半径由可以可靠确定特征方向角度时的区域大小来确定
// 角度范围为[0,360)
bool FeatureRadiusAngle_dAng(const cv::Mat & img,		// input: the input gray scale image
							 int ix, int iy,			// input: the center of the region
							 int & r,					// output:the estimated radius of the circular region
							 double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
							 double thresh_dAng = 5.0,	// input: 当当前特征方向相对于上一次迭代的改变量小于该阈值时才认为可靠确定特征方向，由此定下特征尺度大小
							 int r_max = 100			// input: 允许的最大邻域半径值
							 );

bool FeatureRadiusAngle_dAng(const cv::Mat & img,		// input: the input gray scale image
							 double x, double y,		// input: the center of the region
							 int & r,					// output:the estimated radius of the circular region
							 double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
							 double thresh_dAng = 5.0,	// input: 当当前特征方向相对于上一次迭代的改变量小于该阈值时才认为可靠确定特征方向，由此定下特征尺度大小
							 int r_max = 100			// input: 允许的最大邻域半径值
							 );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define MINSCALENUM			0	// the index of the minimal scale
#define MAXSCALENUM			13	// the index of the maximal scale
#define FITSCALENUM			6

const float ImageDisplayScales[] = {0.03125, 0.0625, 0.125, 0.25, 0.5, 0.75, 1, 1.5, 2, 3, 5, 10, 20, 25};

const double sqrt2inv = 0.70710678118654752440084436210485; // 1/sqrt(2)

// zhaokunz, 20140226
enum ChildFrmLayoutType
{
	ChildFrm_Cascade       = 0,     
	ChildFrm_Tabbed        = 1,
	ChildFrm_TileVert      = 2,
	ChildFrm_TileHorz      = 3,
};

BOOL DisplayImage(CDC * pDC,									// pointer to CDC
				  const cv::Mat & img,							// image to be displayed
				  int tl_x, int tl_y, int rect_w, int rect_h	// region to put the image in
				  );

CString GetFileNameOnly(CString filePath);

enum FeatureType
{
	Feature_SIFT_SIFT      = 0,     
	Feature_SURF_SURF      = 1,
	Feature_FAST_SIFT	   = 2,
	Feature_MANUAL_SIFT	   = 3,
	Feature_SIFTFAST_SIFT  = 4,
	Feature_ORB_SIFT       = 5,
};

struct Features
{
	std::vector<cv::KeyPoint> key_points; // 20200703，做一个小改动，这个对象存融合的所有特征，下面的特征描述矩阵同此
	cv::Mat descriptors;
	std::vector<int> tracks;

	// 20200703，这里把不同类型的特征及描述矩阵按类型来存
	//std::vector<cv::KeyPoint> keypts_sift;
	//cv::Mat descrps_sift;

	//std::vector<cv::KeyPoint> keypts_fast;
	//cv::Mat descrps_fast;

	// zhaokunz, 20140324, sometimes multiple keypoints correspond to the same image point
	// like the SIFT features, a feature can have multiple orientations
	// this idx indicates the unique index of this keypoint, if two keypoints correspond to the same image point
	// their indices will be the same, otherwise will be different
	std::vector<int> idx_pt;

	// 20150214, zhaokunz, 所有特征点处通过双线性插值得到的的颜色信息
	std::vector<cv::Vec3b> rgbs;

	FeatureType type;

	Features()
	{
		type = Feature_SIFT_SIFT;
	};

	Features & operator = (const Features & otherFeat)
	{
		if (this != &otherFeat)
		{
			type = otherFeat.type;
			key_points = otherFeat.key_points;
			//keypts_sift = otherFeat.keypts_sift;
			//keypts_fast = otherFeat.keypts_fast;
			descriptors = otherFeat.descriptors.clone();
			//descrps_sift = otherFeat.descrps_sift.clone();
			//descrps_fast = otherFeat.descrps_fast.clone();
			tracks = otherFeat.tracks;
			idx_pt = otherFeat.idx_pt;
			rgbs = otherFeat.rgbs;
		}

		return *this;
	};

	Features(const Features & feat)
	{
		*this = feat;
	};

	void clear(void)
	{
		key_points.clear();
		descriptors.release();
		tracks.clear();
		idx_pt.clear();
		rgbs.clear();
	};

	void push_back(const Features & otherFeat)
	{
		key_points.insert(key_points.end(), otherFeat.key_points.begin(), otherFeat.key_points.end());
		descriptors.push_back(otherFeat.descriptors/*.clone()*/);	// 20200728，测试过了，没必要 .clone()，因为是直接 copy 的值，而非指针，严格测试过了。
		tracks.insert(tracks.end(), otherFeat.tracks.begin(), otherFeat.tracks.end());
		idx_pt.insert(idx_pt.end(), otherFeat.idx_pt.begin(), otherFeat.idx_pt.end());
		rgbs.insert(rgbs.end(), otherFeat.rgbs.begin(), otherFeat.rgbs.end());
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
	cv::Matx33d m_uncertaintyEllipsoid;			// 20200711, uncertainty ellipsoid, with each row being one of the three orthogonal semi-axes (1 sigma level)
	double m_rltUctt;							// 20200717, the overall uncertainty sigma divided by mean observation distance

	CloudPoint()
	{
		m_idx = -1;
		m_pt.x = m_pt.y = m_pt.z = 0;
		m_rltUctt = -1;
	};

	CloudPoint & operator = (const CloudPoint & otherPt)
	{
		if (this != &otherPt)
		{
			m_idx = otherPt.m_idx;
			m_pt = otherPt.m_pt;
			m_vImgInfos = otherPt.m_vImgInfos;
			m_uncertaintyEllipsoid = otherPt.m_uncertaintyEllipsoid;
			m_rltUctt = otherPt.m_rltUctt;
		}

		return *this;
	};

	CloudPoint(const CloudPoint & pt)
	{
		*this = pt;
	};
};

struct cam_data
{
	double fx; double fy;   // equivalent focal length
	double cx; double cy;   // principal point
	double s;	            // skew factor

	double R[9];            // rotation matrix
	double t[3];            // translation vector

	double k[5];            // distortion

	// 20150214, zhaokunz, 改用Matx结构体
	Matx33d m_K;
	Matx33d m_R;
	Matx31d m_t;
	Matx<double, 5, 1> m_dist;

	bool m_bCalibed;	// whether interior calibrated or not
	bool m_bOriented;	// whether exterior oriented or not
	//////////////////////////////////////////////////////////////////////////
	
	int dist_type;          // distortion type, 0 is Weng's, 1 is D.C.Brown's

	Features m_feats;		// image features
	Features m_subFeats;	// image sub feature set
	Features m_featsBlob;	// blob features extracted automatically
	Features m_featsCorner;	// corner features extracted automatically
	Features m_featsManual; // features extracted manually

	int m_nSiftElected;		// 前多少名 sift 特征点入选参加 SfM
	int m_nFastElected;		// 前多少名 FAST 特征点入选参加 SfM
	int m_nManualElected;	// 前多少名手提点入选参加 SfM

	cv::Matx33d m_optCtrUncertEllipsoid; // 20200711, uncertainty ellipsoid of the optical center, with each row being one of the three orthogonal semi - axes(1 sigma level)

	cam_data()
	{
		fx = -1;	fy = -1;
		cx = -1;	cy = -1;
		s  = -1;

// 		R = m_R.val;
// 		t = m_t.val;
// 		k = m_dist.val;

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

		m_bCalibed = false;
		m_bOriented = false;

		m_nSiftElected = 0;
		m_nFastElected = 0;
		m_nManualElected = 0;
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
			m_subFeats = otherCam.m_subFeats;
			m_featsBlob = otherCam.m_featsBlob;
			m_featsCorner = otherCam.m_featsCorner;
			m_featsManual = otherCam.m_featsManual;

			m_optCtrUncertEllipsoid = otherCam.m_optCtrUncertEllipsoid;

			m_K = otherCam.m_K;
			m_R = otherCam.m_R;
			m_t = otherCam.m_t;
			m_bCalibed = otherCam.m_bCalibed;
			m_bOriented = otherCam.m_bOriented;

			m_nSiftElected = otherCam.m_nSiftElected;
			m_nFastElected = otherCam.m_nFastElected;
			m_nManualElected = otherCam.m_nManualElected;
		}

		return *this;
	};

	cam_data(const cam_data & cam)
	{
		*this = cam;
	};

	void ExtractSiftFeatures(const cv::Mat & img, int nfeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.03, double edgeThreshold = 10, double sigma = 1.6)
	{
		m_featsBlob.clear(); // 先把之前的清掉

		auto f2d = cv::xfeatures2d::SIFT::create(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);

		// 提取 sift 特征点
		f2d->detect(img, m_featsBlob.key_points);

		// 按特征 size 从大到小对 sift 特征点进行排序
		sort(m_featsBlob.key_points.begin(), m_featsBlob.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.size > b.size; });

		// 生成特征描述向量
		f2d->compute(img, m_featsBlob.key_points, m_featsBlob.descriptors);

		// 20201116，把特征点输出到图像中 //////////////////////////////////////////////
// 		cv::Mat imgOut;
// 		cv::drawKeypoints(img, m_featsBlob.key_points, imgOut, Scalar(0, 255, 0));
// 
// 		CString strInfo;
// 		strInfo.Format("C:\\Users\\DeepV\\Desktop\\缩比图像\\jpg\\000000.jpg");
// 		imwrite(strInfo.GetBuffer(), imgOut);
		//////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////
// 		FILE * file = fopen("C:\\Users\\DeepV\\Desktop\\desp_sift.txt", "w");
// 		for (int i = 0; i < m_featsBlob.descriptors.rows; ++i)
// 		{
// 			for (int j = 0; j < m_featsBlob.descriptors.cols; ++j)
// 			{
// 				fprintf(file, "%lf	", m_featsBlob.descriptors.at<float>(i, j));
// 			}
// 			fprintf(file, "\n");
// 		}
// 		fclose(file);
		//////////////////////////////////////////////////////////////////

		m_featsBlob.type = Feature_SIFT_SIFT; // sift keypoints + sift descriptors

		// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
		int n = m_featsBlob.key_points.size();
		KeyPoint kpt_pre;
		kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
		kpt_pre.size = -1000;
		int idx_imgPt;
		for (int i = 0; i < n; ++i)
		{
			m_featsBlob.tracks.push_back(-1);

			KeyPoint kpt_cur = m_featsBlob.key_points[i];
			if (fabs(kpt_cur.pt.x - kpt_pre.pt.x) < 1.0E-12 && fabs(kpt_cur.pt.y - kpt_pre.pt.y) < 1.0E-12
				&& fabs(kpt_cur.size - kpt_pre.size) < 1.0E-12)
			{
				// indicating that current keypoint is identical to the previous keypoint
				m_featsBlob.idx_pt.push_back(idx_imgPt);
			}
			else
			{
				m_featsBlob.idx_pt.push_back(i);
				idx_imgPt = i;
			}

			kpt_pre.pt.x = kpt_cur.pt.x;
			kpt_pre.pt.y = kpt_cur.pt.y;
			kpt_pre.size = kpt_cur.size;

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r, g, b;
			Vec3b rgb;
			if (BilinearInterp(img, kpt_cur.pt.x, kpt_cur.pt.y, r, g, b))
			{
				rgb[0] = b;
				rgb[1] = g;
				rgb[2] = r;
			}
			m_featsBlob.rgbs.push_back(rgb);
		}
	};

	void ExtractFASTFeatures(const cv::Mat & img, double thresholdOffset = 1.0, int r_max = 100, int thresholdFast = 20, bool nonmaxSuppressionFast = true, int typeFast = cv::FastFeatureDetector::TYPE_9_16,
		int nfeaturesSift = 0, int nOctaveLayersSift = 3, double contrastThresholdSift = 0.03, double edgeThresholdSift = 10, double sigmaSift = 1.6)
	{
		m_featsCorner.clear(); // 先把之前的清掉

		// 提取 fast 角点特征 //////////////////////////////////////////////////////
		// 20200706，先把图像转换成灰度图再提取FAST特征，因为opencv fast算子只能适用于灰度图（sift是彩色和灰度皆可），而lightroom处理完的图片导出时会被自动转为3通道图，难怪fast提取的特征总有偏移
		cv::Mat im_gray;

		int nChannel = img.channels();

		if (nChannel == 1) // 本身就是灰度图
		{
			im_gray = img;
		}
		else // 彩图
		{
			cv::cvtColor(img, im_gray, CV_RGB2GRAY);
		}
		
		cv::FAST(im_gray, m_featsCorner.key_points, thresholdFast, nonmaxSuppressionFast, typeFast);
		//////////////////////////////////////////////////////////////////////////

		// 按照 response 从大到小对 fast 特征点进行排序
		sort(m_featsCorner.key_points.begin(), m_featsCorner.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });

		int n = m_featsCorner.key_points.size();

		// 20200830，为每个 FAST 特征点计算一个特征方向 ////////////////////////////
		for (int i = 0; i < n; ++i)
		{
			cv::KeyPoint & keypt = m_featsCorner.key_points[i];

//			keypt.size = size;

			int r/* = (size - 1)*0.5*/;
			double angle;

// 			if (CornerAngle_IC(im_gray, keypt.pt.x, keypt.pt.y, r, angle))
// 			{
// 				if (angle < 0) // 确保最终的角度范围符合 opencv keypoint::angle 的取值范围，即 [0,360)
// 				{
// 					angle += 360;
// 				}
// 
// 				keypt.angle = angle;
// 			}

			// 20200901，同时估计特征尺度和方向 ////////////////////////////////////////
// 			if (FeatureRadiusAngle(im_gray, keypt.pt.x, keypt.pt.y, r, angle, thresholdOffset, r_max))
// 			{
// 				keypt.size = (2 * r + 1)*0.33333333333; // keypoint::size 表征特征的直径 diameter
// 				keypt.angle = angle;
// 			}

			if (FeatureRadiusAngle_dAng(im_gray, keypt.pt.x, keypt.pt.y, r, angle, 3.0))
			{
				keypt.size = 2 * r + 1; // keypoint::size 表征特征的直径 diameter
				keypt.angle = angle;
			}
			//////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////

		// 生成特征描述向量
		auto f2d = cv::xfeatures2d::SIFT::create(nfeaturesSift, nOctaveLayersSift, contrastThresholdSift, edgeThresholdSift, sigmaSift);
		f2d->compute(img, m_featsCorner.key_points, m_featsCorner.descriptors);

		///////////////////////////////////////////////////////////////////
// 		FILE * file = fopen("C:\\Users\\DeepV\\Desktop\\desp_fast.txt", "w");
// 		for (int i = 0; i < m_featsCorner.descriptors.rows; ++i)
// 		{
// 			for (int j = 0; j < m_featsCorner.descriptors.cols; ++j)
// 			{
// 				fprintf(file, "%lf	", m_featsCorner.descriptors.at<float>(i, j));
// 			}
// 			fprintf(file, "\n");
// 		}
// 		fclose(file);
		//////////////////////////////////////////////////////////////////

		m_featsCorner.type = Feature_FAST_SIFT; // fast keypoints + sift descriptors

		// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
		for (int i = 0; i < n; ++i)
		{
			const cv::KeyPoint & keypt = m_featsCorner.key_points[i];

			m_featsCorner.tracks.push_back(-1);
			m_featsCorner.idx_pt.push_back(i);

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r, g, b;
			Vec3b rgb;
			if (BilinearInterp(img, keypt.pt.x, keypt.pt.y, r, g, b))
			{
				rgb[0] = b;
				rgb[1] = g;
				rgb[2] = r;
			}
			m_featsCorner.rgbs.push_back(rgb);
		}
	};

	void ExtractORBFeatures(const cv::Mat & img, int nfeatures = 500, float scaleFactor = 1.2f, int nlevels = 8, int edgeThreshold = 31,
		int firstLevel = 0, int WTA_K = 2, int scoreType = cv::ORB::HARRIS_SCORE, int patchSize = 31, int fastThreshold = 20,
		int nfeaturesSift = 0, int nOctaveLayersSift = 3, double contrastThresholdSift = 0.03, double edgeThresholdSift = 10, double sigmaSift = 1.6)
	{
		m_featsCorner.clear(); // 先把之前的清掉

		//////////////////////////////////////////////////////////////////////////

		auto orb = cv::ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize, fastThreshold);
		orb->detect(img, m_featsCorner.key_points);

// 		vector<double> devs;
// 
// 		double sum = 0;
// 		for (int i = 0; i < keyptsORB.size(); ++i)
// 		{
// 			const KeyPoint & kypt = keyptsORB[i];
// 
// 			int r = (kypt.size - 1)*0.5;
// 
// 			double angle;
// 			if (CornerAngle_IC(im_gray, kypt.pt.x, kypt.pt.y, r, angle))
// 			{
// 				if (angle < 0) // 确保最终的角度范围符合 opencv keypoint::angle 的取值范围，即 [0,360)
// 				{
// 					angle += 360;
// 				}
// 			}
// 
// 			double ddd = std::abs(kypt.angle - angle);
// 
// 			devs.push_back(ddd);
// 
// 			sum += ddd;
// 		}
// 
// 		double tmp = sum / keyptsORB.size();
		//////////////////////////////////////////////////////////////////////////

		// 按照 response 从大到小对 fast 特征点进行排序
		sort(m_featsCorner.key_points.begin(), m_featsCorner.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });

		// 生成特征描述向量
		auto f2d = cv::xfeatures2d::SIFT::create(nfeaturesSift, nOctaveLayersSift, contrastThresholdSift, edgeThresholdSift, sigmaSift);
		f2d->compute(img, m_featsCorner.key_points, m_featsCorner.descriptors);

		m_featsCorner.type = Feature_ORB_SIFT; // ORB keypoints + sift descriptors

		// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
		int n = m_featsCorner.key_points.size();
		for (int i = 0; i < n; ++i)
		{
			const cv::KeyPoint & keypt = m_featsCorner.key_points[i];

			m_featsCorner.tracks.push_back(-1);
			m_featsCorner.idx_pt.push_back(i);

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r, g, b;
			Vec3b rgb;
			if (BilinearInterp(img, keypt.pt.x, keypt.pt.y, r, g, b))
			{
				rgb[0] = b;
				rgb[1] = g;
				rgb[2] = r;
			}
			m_featsCorner.rgbs.push_back(rgb);
		}
	};

	void GenSfMFeatures(int nSfMFeatures = 8192, int nPrptFeatures = 150)
	{
		int nSift = m_featsBlob.key_points.size();
		int nFast = m_featsCorner.key_points.size();
		int nManual = m_featsManual.key_points.size();

		// 先清空
		m_feats.clear();
		m_subFeats.clear();

		// 暂时先合成个大的
		Features featsTmp = m_featsBlob;
		featsTmp.push_back(m_featsCorner);

		// 然后截取为最终的，并录入
		int nSize = featsTmp.key_points.size();
		int nSmaller = nSize < nSfMFeatures ? nSize : nSfMFeatures;

		m_feats.key_points.insert(m_feats.key_points.end(), featsTmp.key_points.begin(), featsTmp.key_points.begin() + nSmaller);
		m_feats.descriptors = featsTmp.descriptors.rowRange(cv::Range(0, nSmaller));
		m_feats.tracks.insert(m_feats.tracks.end(), featsTmp.tracks.begin(), featsTmp.tracks.begin() + nSmaller);
		m_feats.idx_pt.insert(m_feats.idx_pt.end(), featsTmp.idx_pt.begin(), featsTmp.idx_pt.begin() + nSmaller);
		m_feats.rgbs.insert(m_feats.rgbs.end(), featsTmp.rgbs.begin(), featsTmp.rgbs.begin() + nSmaller);

		// 再把手提点加进来
		m_feats.push_back(m_featsManual);

		int nFinal = m_feats.key_points.size();

		// 统计各类点入选参与 SfM 特征点的个数
		m_nSiftElected = nSift < nSfMFeatures ? nSift : nSfMFeatures;
		m_nFastElected = nSmaller - m_nSiftElected;
		m_nManualElected = nManual; // 即全部手提点都入选

		// 更新参与 SfM 的特征点的统一编号
		for (int i = m_nSiftElected; i < nFinal; ++i)
		{
			m_feats.idx_pt[i] = i;
		}

		// 最后生成“先发制人”的特征点集
		if (nPrptFeatures < nFinal)
		{
			m_subFeats.key_points.insert(m_subFeats.key_points.end(), m_feats.key_points.begin(), m_feats.key_points.begin() + nPrptFeatures);
			m_subFeats.descriptors = m_feats.descriptors.rowRange(cv::Range(0, nPrptFeatures));
			m_subFeats.tracks.insert(m_subFeats.tracks.end(), m_feats.tracks.begin(), m_feats.tracks.begin() + nPrptFeatures);
			m_subFeats.idx_pt.insert(m_subFeats.idx_pt.end(), m_feats.idx_pt.begin(), m_feats.idx_pt.begin() + nPrptFeatures);
			m_subFeats.rgbs.insert(m_subFeats.rgbs.end(), m_feats.rgbs.begin(), m_feats.rgbs.begin() + nPrptFeatures);
		}
	};

	void DeleteAllFeatures()
	{
		m_featsBlob.clear();
		m_featsCorner.clear();
		m_featsManual.clear();
		m_feats.clear();
		m_subFeats.clear();

		m_nSiftElected = 0;
		m_nFastElected = 0;
		m_nManualElected = 0;
	};
};

// CvMat wrapper here /////////////////////////////////////////////////////////////////////////////////

#define EPSILON_ZHU		1.110223E-16

// 矩阵类CMatrix
// 本矩阵类采用了引用计数机制，提高了函数调用效率，但同时也导致了语句可读性的下降
// 例如：CMatrix m1, m2; m1 = m2 并不是将m2数据区的内容复制到m1数据区，而是让m1和m2中的m_pMat指向同一个矩阵头，因此m1和m2也就索引了同一块数据区，在改变m1矩阵
// 某元素值的同时，m2对应元素的值也发生了改变，即m1 = m2 这句话会使得这两个矩阵完全相关联起来，不再独立，CMatrix m1 = m2效果是一样的。
// 若想有两个完全独立的矩阵（即各个矩阵有各自独立的数据区），并将其中一个矩阵的数据复制到另一个矩阵的数据区的话，可以通过语句 CMatrix m1 = m2.Clone()。

// 定义矩阵翻转模式的枚举类型
enum FlipMode
{
	FLIPMODE_X,
	FLIPMODE_Y,
	FLIPMODE_XY
};

enum MosaicMode
{
	MOSAICMODE_DOWN,
	MOSAICMODE_UP,
	MOSAICMODE_RIGHT,
	MOSAICMODE_LEFT
};

enum InverseMode
{
	INVERSEMODE_LU,
	INVERSEMODE_SVD,
	INVERSEMODE_SVD_SYM
};

enum SolveMode
{
	SOLVEMODE_LU,
	SOLVEMODE_SVD,
	SOLVEMODE_SVD_SYM
};

enum RandMode
{
	RANDMODE_UNIFORM,  // Uniform distribution
	RANDMODE_GAUSSIAN  // Gaussian distribution
};

enum NormMode
{
	NORMMODE_C,
	NORMMODE_L1,
	NORMMODE_L2
};

enum WriteMode
{
	WRITEMODE_LF,
	WRITEMODE_E
};

class CMatrix
{
public:
	CMatrix();
	CMatrix(int nRow, int nColumn, double initVal = 0);
	CMatrix(CvMat * pCvMat);
	CMatrix(const CMatrix & mat);                                     // 拷贝构造函数，用于传参以及返回类对象
	/*CMatrix(int nRow, int nColumn, double * pInitArray);*/
	virtual ~CMatrix();

	// 属性
public:                                                
	int     m_nRow;                                                   // 矩阵行数
	int     m_nCol;                                                   // 矩阵列数
	CvMat * m_pMat;                                                   // 封装的OpenCV的矩阵结构体的指针

private:
	/*CvMat * m_pMat;                                                   // 封装的OpenCV的矩阵结构体的指针*/

	// 操作
public:
	double & operator () (int i, int j) const;                        // 索引方式从1开始
	double & operator () (int i)        const;                        // 若矩阵是向量形式，不管是行向量还是列向量，则可通过一维索引方式对矩阵元素进行取值或赋值

	CMatrix & operator = (const CMatrix & mSource);                   // 重载赋值运算符，使得本对象和mSource中的m_pMat指向同一个矩阵头，因此也拥有同一块引用计数存储区和数据区，改变本对象的数据会使得mSource的数据也发生变化
	CMatrix & operator = (double val);                                // 把矩阵所有元素的值置换为val

	CMatrix operator + (const CMatrix & mAdd) const;                  // 重载加号运算符
	CMatrix operator + (double val) const;                            // 矩阵所有元素的值加val
	friend CMatrix operator + (double val, const CMatrix & mAdd);     // 矩阵所有元素的值左加一个val

	CMatrix operator - (const CMatrix & mSub) const;				  // 重载减号运算符
	CMatrix operator - (double val) const;                            // 矩阵所有元素的值减val
	CMatrix operator - () const;                                      // 矩阵所有元素取反，单目运算符重载
	friend CMatrix operator - (double val, const CMatrix & mSub);	  // 矩阵所有元素的值左减一个val

	CMatrix operator * (const CMatrix & mMul) const;				  // 重载乘法运算符
	CMatrix operator * (double val) const;                            // 右乘一个数
	friend CMatrix operator * (double val, const CMatrix & mMul);     // 左乘一个数

	CMatrix operator / (const CMatrix & mDiv) const;                  // 矩阵除以一个相同尺寸的矩阵，两矩阵各个元素对应相除，并非一矩阵乘以另一矩阵的逆
	CMatrix operator / (double val) const;                            // 矩阵各个元素除以val
	friend CMatrix operator / (double val, const CMatrix & mDiv);     // 一个数除以一个矩阵

	void operator += (const CMatrix & mAdd);                          // 矩阵自加一矩阵
	void operator += (double val);                                    // 矩阵自加一数

	void operator -= (const CMatrix & mSub);                          // 矩阵自减一矩阵
	void operator -= (double val);                                    // 矩阵自减一数

	void operator *= (const CMatrix & mMul);                          // 矩阵自乘一矩阵，是指和相同尺寸矩阵各个元素对应相乘
	void operator *= (double val);                                    // 矩阵自乘一数

	void operator /= (const CMatrix & mDiv);                          // 矩阵自除一矩阵，是指和相同尺寸矩阵各个元素对应相除
	void operator /= (double val);                                    // 矩阵自除一数



	void Release(void);                                               // 具有引用计数机制的矩阵释放函数



	BOOL IsNull(void)       const;                                    // 是否是空矩阵，即m_pMat是否为NULL
	BOOL IsZero(void)       const;                                    // 是否是零矩阵
	BOOL IsVector(void)     const;                                    // 是否是向量
	BOOL IsRowVec(void)     const;                                    // 是否是行向量
	BOOL IsColVec(void)     const;                                    // 是否是列向量
	BOOL IsSquare(void)     const;                                    // 是否是方阵
	BOOL IsSymmetric(void)  const;                                    // 是否是对称阵
	BOOL IsAntiSymm(void)   const;                                    // 是否是反对称阵
	BOOL IsDiagonal(void)   const;                                    // 是否是对角阵，不一定非要是方阵
	BOOL IsIdentity(void)   const;                                    // 是否是单位阵
	BOOL IsSymmPosDef(void) const;                                    // 是否是对称正定矩阵，利用了Cholesky分解



	CMatrix Clone(void)     const;                                    // 返回的对象中的m_pMat指向一个新的矩阵头，并索引了一块新的数据区，且新的数据区中的数据是对本对象索引数据区中数据的拷贝
	CMatrix GetRow(int i)   const;                                    // 返回矩阵对象的某一行，也为CMatrix对象，索引方式从1开始
	CMatrix GetCol(int i)   const;                                    // 返回矩阵对象的某一列，也为CMatrix对象，索引方式从1开始
	CMatrix GetRect(int rowTL, int colTL, int width, int height) const; // 返回矩阵对象的某一矩形块，也为一矩阵对象，索引方式从1开始
	CMatrix GetDiag(void)   const;                                    // 返回矩阵对角线上的元素
	CMatrix Transpose(void) const;                                    // 矩阵转置
	CMatrix Inverse(void)   const;                                    // 矩阵求逆
	CMatrix Pow(double power) const;                                  // 所有的矩阵元素求幂
	CMatrix Log(void)       const;                                    // 所有的矩阵元素取对数
	CMatrix Max(void)       const;                                    // 找到矩阵的全局最大值及其索引，返回矩阵的第一行为矩阵最大值所在元素的行索引，第二行为其列索引，第三行为此最大值
	CMatrix Min(void)       const;                                    // 找到矩阵的全局最小值及其索引，返回矩阵的第一行为矩阵最小值所在元素的行索引，第二行为其列索引，第三行为此最小值
	CMatrix Find(double val, double eps = 1.0E-10) const;             // 在矩阵中找到值为val的元素，返回这些元素的行列索引号，返回矩阵的第一行为行索引，第二行为列索引
	CMatrix Abs(void)       const;                                    // 返回的矩阵中所有元素为原矩阵对应元素的绝对值
	CMatrix Atand(void)     const;                                    // 返回的矩阵中的元素为原矩阵中对应元素的反正弦值



	void SetRow(int i, const CMatrix & mSrc);                         // 将已有的一行数据mSrc赋值给矩阵的第i行，索引方式从1开始
	void SetCol(int i, const CMatrix & mSrc);                         // 将已有的一列数据mSrc赋值给矩阵的第i列，索引方式从1开始
	void SetRect(int rowTL, int colTL, const CMatrix & mSrc);         // 将已有的一个矩阵mSrc赋值给矩阵的某一块矩形区域，被赋值的矩形区域大小由mSrc的尺寸决定
	void AddOneRow(double initVal = 0);                               // 在原矩阵的最后一行后面再添一行形成新的拓展的矩阵
	void AddOneCol(double initVal = 0);                               // 在原矩阵的最后一列后面再添一列形成新的拓展的矩阵
	void Flip(FlipMode flipMode);                                     // 根据输入的翻转模式，让矩阵绕x轴翻转，或绕y轴翻转，亦或是既绕x轴又绕y轴翻转
	void Mosaic(const CMatrix & mOther, MosaicMode mode = MOSAICMODE_DOWN); // 将另一个矩阵按一定模式拼接到本矩阵上
	void Reshape(int newRow, int newCol);                             // 改变矩阵的尺寸，但矩阵元素的总数依然不变
	void VectorizeByRow(void);                                        // 将矩阵按行排列成矢量形式
	void VectorizeByCol(void);                                        // 将矩阵按列排列成矢量形式



	double Det(void)                         const;				      // 计算矩阵行列式的值
	double Trace(void)                       const;                   // 计算矩阵的迹
	int    Rank(double eps = 1.0E-10)        const;                   // 计算矩阵的秩
	double InvCond(void)                     const;                   // 计算矩阵条件数的倒数，即最小奇异值与最大奇异值的比值
	double Mean(void)                        const;                   // 计算矩阵所有元素的平均值
	double StdDev(void)                      const;                   // 计算矩阵所有元素的标准差
	int    CountNonZero(void)                const;                   // 统计矩阵非零元素的个数
	double Sum(void)                         const;                   // 对矩阵中所有元素求和
	double Norm(NormMode mode = NORMMODE_L2) const;                   // 计算矩阵的范数
	int    Length(void)                      const;                   // 若CMatrix对象是向量则返回向量长度，若为矩阵，则返回拥有元素最多的那个维度上的元素个数
	double Dev(void)                         const;                   // 返回 sqrt(sum(m(i,j)^2) / n)



	friend CMatrix Mul(const CMatrix & m1, const CMatrix & m2);       // 相同大小的矩阵各个元素对应相乘
	friend CMatrix Div(const CMatrix & m1, const CMatrix & m2);       // 相同大小的矩阵各个元素对应相除
	friend CMatrix ExtendRows(const CMatrix & mMat, int row);         // 将输入的矩阵按row行进行拓展
	friend CMatrix ExtendCols(const CMatrix & mMat, int col);         // 将输入的矩阵按col列进行拓展
	friend CMatrix RepMat    (const CMatrix & mMat, int row, int col);// 将输入的矩阵按row行和col列进行拓展
	friend CMatrix Rand(int row, int col, RandMode randMode, double param1, double param2); // 生成由随机数组成的矩阵，randMode为何种分布的随机数，param1和param2在为uniform分布时分别表示上下界，在gaussian时分别为随机数的平均值和标准差
	friend CMatrix Solve(const CMatrix & mA, const CMatrix & mB);     // 求解Ax=B
	friend CMatrix Cross(const CMatrix & mVec1, const CMatrix & mVec2);// 求解俩三维向量的叉乘，mVec1×mVec2
	friend void    RQ3x3(const CMatrix & mSrc, CMatrix & mR, CMatrix & mQ);// 对3×3的矩阵进行RQ分解：A = RQ，其中R为对角线元素全为正的上三角矩阵，Q则为正交矩阵
	friend CMatrix GenI(int n);                                       // 生成n×n的单位阵
	friend CMatrix GenCrossMat(const CMatrix & mVec);                 // 根据输入的三维向量v生成其对应的叉乘矩阵[v]
	friend void    House(const CMatrix & mX, CMatrix & mVec, double & beta); // 根据输入的向量mX，生成相应的Householder vector mVec，以及β
	friend void    Givens(double a, double b, double & c, double & s);// 根据输入的[a; b]'计算使得[c s; -s c]' * [a; b]' = [r; 0]'成立的cos和sin



	void    QR_Householder(CMatrix & mR, CMatrix & mQ = CMatrix(0)) const;  // 对m×n的矩阵A进行QR分解：A = QR，其中Q为m×m的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
	void    QR_Givens(CMatrix & mR, CMatrix & mQ = CMatrix(0)) const;       // 对m×n的矩阵A进行QR分解：A = QR，其中Q为m×m的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
	void    RQ_Givens(CMatrix & mR, CMatrix & mQ = CMatrix(0)) const;       // 对m×n的矩阵A进行RQ分解（要求m<=n）：A = RQ，其中Q为n×n的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
	CMatrix Chol_Lower(double eps = EPSILON_ZHU)   const;                   // 对对称正定矩阵实施Cholesky分解 A = LL'，返回对角线上元素全为正的下三角矩阵L，此函数也用来判断对称矩阵是否正定，分解成功则返回的矩阵对象m_pMat不为NULL，否则为NULL
	CMatrix Chol_Upper(double eps = EPSILON_ZHU)   const;                   // 对对称正定矩阵实施Cholesky分解 A = UU'，返回对角线上元素全为正的上三角矩阵U，此函数也用来判断对称矩阵是否正定，分解成功则返回的矩阵对象m_pMat不为NULL，否则为NULL
	void    SVD(CMatrix & mD, CMatrix & mU = CMatrix(0), CMatrix & mV = CMatrix(0)) const; // 对矩阵实施SVD分解，A = UDV'



	void Write(CString filePath, WriteMode mode = WRITEMODE_LF) const;      // 把矩阵写入指定文件
	BOOL Read(CString filePath);                                            // 从文件读入数据到矩阵对象
	void PrintMatrix2Screen(CString title = "", WriteMode mode = WRITEMODE_LF) const;     // 通过AfxMessageBox把矩阵显示在屏幕上
};
///////////////////////////////////////////////////////////////////////////////////////////////////////

// levmar related structures ////////////////////////////////////////////////////////////////////////
struct levmar_mImgPts1_mImgPts2_bx
{
	CMatrix mImgPts1;
	CMatrix mImgPts2;
	double bx;
};

struct levmar_calib_data
{
	cam_data camData;
	vector<Point3d> objPts;
};
///////////////////////////////////////////////////////////////////////////////////////////////////

// std related ////////////////////////////////////////////////////////////////////////////////////

// Return whether first element is greater than the second
bool Greater_DMatch_queryIdx(DMatch match1, DMatch match2);
bool Greater_DMatch_trainIdx(DMatch match1, DMatch match2);

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 尚总的解方程及矩阵操作相关 /////////////////////////////////////////////////////////////////////////////////
bool EquSlvPdI_611(double *M, double *B, int m, int n, double *S);
bool MtxInvSP_611(double *A, int n);
bool MtxPdI_611(double *A, int m, int n);
void MtxMlt_611(double *A, double *B, int m, int n, int s, double *AB);
void MtxTrs_611(double *A, int m, int n, double *AT);
void ATAMlt_611(double *A, int m, int n, double *ATA);

bool M_Inversion_611(double *MatI, double *MatO, int n);
int M_DecompSVD_611(double *a,int m,int n,double *u,double *v,double eps,int ka);
void ppp_611(double *a,double *e,double *s,double *v,int m,int n);
void sss_611(double *fg,double *cs);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 对 1 维数组进行冒泡法排序，可以升序排列也可降序排列，并输出原数组各元素索引在排完序的新数组中的排列
void Sort_Bubble(double * p,			// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按升序或者降序排列
				 int n,					// 输入：数组长度，元素个数
				 int * idx,				// 输出：排序完成后各元素原来的索引号的排列情况，索引从 0 开始
				 BOOL bIsAscent = TRUE	// 输入：是升序，还是降序排列
				 );

// 对 1 维数组根据输入的索引来进行重排列
void Sort_byIndex_double(double * p,	// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按 idx 中的索引来进行重排列
				         int n,			// 输入：数组长度，元素个数
				         int * idx		// 输入：索引值，函数就是根据这个索引参数中的索引来对数组中的各元素进行重排列的，索引从 0 开始
				         );

// 对 1 维数组根据输入的索引来进行重排列
void Sort_byIndex_int(int * p,			// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按 idx 中的索引来进行重排列
				      int n,			// 输入：数组长度，元素个数
				      int * idx			// 输入：索引值，函数就是根据这个索引参数中的索引来对数组中的各元素进行重排列的，索引从 0 开始
				      );

// 对 2 维数组的每一列按某一行的数据进行升序或者降序排列
void Sort2_byRow(double ** p,			// 输入兼输出：待排序的 2 维数组
				 int w, int h,			// 输入：2 维数组的宽高
				 int row = 0,			// 输入：按某一行的数据对所有列进行排序，默认为第 1 行，索引从 0 开始
				 BOOL bIsAscent = TRUE	// 输入：按升序还是降序排列，默认为升序
				 );

// 对 2 维数组的每一行按某一列的数据进行升序或者降序排列
void Sort2_byCol(double ** p,			// 输入兼输出：待排序的 2 维数组
				 int w, int h,			// 输入：2 维数组的宽高
				 int col = 0,			// 输入：按某一列的数据对所有行进行排序，默认为第 1 行，索引从 0 开始
				 BOOL bIsAscent = TRUE	// 输入：按升序还是降序排列，默认为升序
				 );

// 找到并返回 1 维数组中的最小值，并输出最小值在数组中的索引
double FindArrayMin(double * p,					// 输入：待考察的 1 维数组
					int n,						// 输入：数组的长度
					int & idx,					// 输出：找到的数组最小值的索引，从 0 开始
					double * pMinThresh = NULL	// even the minimal value is supposed to be larger than this threshold
					);

// 找到并返回 1 维数组中的最大值，并输出最大值在数组中的索引
double FindArrayMax(double * p,	// 输入：待考察的 1 维数组
					int n,		// 输入：数组的长度
					int & idx	// 输出：找到的数组最大值的索引，从 0 开始
					);

// 找到并返回 1 维数组中的最大值，并输出最大值在数组中的索引
BYTE FindArrayMax_BYTE(BYTE * p,	// 输入：待考察的 1 维数组
					   int n,		// 输入：数组的长度
					   int & idx	// 输出：找到的数组最大值的索引，从 0 开始
					   );

// 找到并返回 1 维数组中的最大值，并输出最大值在数组中的索引
int FindArrayMax_int(int * p,		// 输入：待考察的 1 维数组
					 int n,			// 输入：数组的长度
					 int & idx		// 输出：找到的数组最大值的索引，从 0 开始
					 );

// 两个 double 型的变量互换数值
void Swap_double(double & A, double & B);
// 两个 int 型的变量互换数值
void Swap_int(int & A, int & B);

// The same procedure with drawing a line, except that this function output
// the points on the line, instead of actually drawing a line.
void GetLinePts(int x0, int y0,			// input: integers, the coordinates of the starting point
				int x1, int y1,			// input: integers, the coordinates of the ending point
				int * px, int * py,		// output:the generated n points on the line
				int & n					// output:the amount of points in the output
				);

void SaveCameraData(CString path, const cam_data & cam);
void ReadCameraData(CString path, cam_data & cam);

void DetermineInterval(double val_max, double val_min, double val_cur, double radius, double & inter_max, double & inter_min);

// compute exp((-1/2)*(x-miu)^2/sigma^2)
double exp_miu_sigma(double x, double miu, double sigma);

// 2020.06.14, instead of drawing all the matches across two images,
// this func trys to draw matches within only the reference/left image
// with a 'line' starting from one feature, and ending at the matching feature.
void drawMatchesRefImg(const cv::Mat & img1,								// input: the reference/left image
					   const std::vector<cv::KeyPoint> & keypoints1,		// input: keypoints found in the reference/left image
					   const std::vector<cv::KeyPoint> & keypoints2,		// input: keypoints found in the matching image
					   const std::vector<cv::DMatch> & matches1to2,			// input: matches found from ref image to matching image
					   cv::Mat & outImg,									// output: the output image with matches drawn
					   const Scalar & matchColor = Scalar(0, 255, 0),		// input: the color of the matched points connecting lines 
					   const Scalar & singlePointColor = Scalar(0, 0, 255)	// input: the color of the unmatched points
					   );

// 20200717，给定相对不确定度水平，给出相应的色彩值（RG色彩域），服务于点云不确定度可视化
void getRGColorforRelativeUncertainty(double uctt,		// input: the given relative uncertainty
									  double val_worst,	// input: the worst value for relative uncertainty, which is set to color Red
									  double val_best,	// input: the best value for relative uncertainty, which is set to color Green
									  uchar & r,			// output: the computed R
									  uchar & g,			// output: the computed G
									  uchar & b			// output: the computed B
									  );

// 20200717，给定相对不确定度水平，给出相应的色彩值（RG色彩域），服务于点云不确定度可视化
void getRGColorforRelativeUncertainty(double uctt,		// input: the given relative uncertainty
									  double val_worst,	// input: the worst value for relative uncertainty, which is set to color Red
									  double val_best,	// input: the best value for relative uncertainty, which is set to color Green
									  int & r,			// output: the computed R
									  int & g,			// output: the computed G
									  int & b			// output: the computed B
									  );

// 20200729
// void ExtractSiftFeatures(Features & feats,
// 						 const cv::Mat & img,
// 						 int nfeatures = 0,					// The number of best features to retain. The features are ranked by their scores (measured in SIFT algorithm as the local contrast)
// 						 int nOctaveLayers = 3,				// The number of layers in each octave. 3 is the value used in D.Lowe paper.The number of octaves is computed automatically from the image resolution.
// 						 double contrastThreshold = 0.03,	// The contrast threshold used to filter out weak features in semi-uniform (low - contrast) regions.The larger the threshold, the less features are produced by the detector.
// 						 double edgeThreshold = 10,			// The threshold used to filter out edge-like features. Note that the its meaning is different from the contrastThreshold, i.e.the larger the edgeThreshold, the less features are filtered out(more features are retained).
// 						 double sigma = 1.6					// The sigma of the Gaussian applied to the input image at the octave \#0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number.
// 						 );
// 
// 20200729
// void ExtractFASTFeatures(Features & feats,
// 						 const cv::Mat & img,
// 						 int thresholdFast = 20,				// threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.
// 						 bool nonmaxSuppressionFast = true,		// if true, non-maximum suppression is applied to detected corners (keypoints).
// 						 int typeFast = cv::FastFeatureDetector::TYPE_9_16,	// one of the three neighborhoods as defined in the paper: FastFeatureDetector::TYPE_9_16, FastFeatureDetector::TYPE_7_12, FastFeatureDetector::TYPE_5_8
// 						 int nfeaturesSift = 0,					// The number of best features to retain. The features are ranked by their scores (measured in SIFT algorithm as the local contrast)
// 						 int nOctaveLayersSift = 3,				// The number of layers in each octave. 3 is the value used in D.Lowe paper.The number of octaves is computed automatically from the image resolution.
// 						 double contrastThresholdSift = 0.03,	// The contrast threshold used to filter out weak features in semi-uniform (low - contrast) regions.The larger the threshold, the less features are produced by the detector.
// 						 double edgeThresholdSift = 10,			// The threshold used to filter out edge-like features. Note that the its meaning is different from the contrastThreshold, i.e.the larger the edgeThreshold, the less features are filtered out(more features are retained).
// 						 double sigmaSift = 1.6					// The sigma of the Gaussian applied to the input image at the octave \#0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number.
// 						 );
// 
// void GenSfMFeatures(cam_data & cam, int & nSiftElected, int & nFastElected, int & nManualElected, int nSfMFeatures = 8192, int nPrptFeatures = 150);

}