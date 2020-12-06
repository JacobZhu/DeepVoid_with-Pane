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

// ����һ��������ת˳���ö������
enum RotationOrder
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

// ����Ƕȵ�cosֵ������Ϊ�Ƕ�
double cosd(double angle);

// ����Ƕȵ�sinֵ������Ϊ�Ƕ�
double sind(double angle);

// ����Ƕȵ�tanֵ������Ϊ�Ƕ�
double tand(double angle);

// ����������ֵ��arccosineֵ������ֵ��ΧΪ0 - 180��
double acosd(double x);

// ����������ֵ��arcsineֵ������ֵ��ΧΪ-90 - 90��
double asind(double x);

// ����������ֵ��arctangentֵ������ֵ��ΧΪ-90 - 90��
double atand(double x);

// ����(y / x)��arctangentֵ�����ڿ����ۺϿ���y��x�ķ��ţ���˷���ֵ�ķ�Χ��չ���ĸ����ޣ���-180 - 180��
double atand(double y, double x);

// �ж����� x �ķ��ţ�Ϊ���򷵻� 1��Ϊ���򷵻� -1
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

// 20200823������һԲ��֧�������ڵ�ͼ��Ҷ�����
bool IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
									  double x, double y,			// input: the center of the region
									  int r,						// input: the radius of the circular region
									  double & xIC, double & yIC	// output:the location of the calculated intensity centroid
									  );

// 20200824������һԲ��֧�������ڵ�ͼ��Ҷ����ģ�ͨ�������������������ƫ�Ƶķ�ʽ��ʾ
bool IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
									  int ix, int iy,				// input: the center of the region
									  int r,						// input: the radius of the circular region
									  double & dxIC, double & dyIC,	// output:the location of the calculated intensity centroid (in terms of offsets)
									  double * pm00_1 = NULL		// output:1/m00
									  );

// 20200824������һԲ��֧�������ڵ�ͼ��Ҷ����ģ�ͨ�������������������ƫ�Ƶķ�ʽ��ʾ
// 20201205������ͼ��Ҷȸ�˹��������ı�׼��������õ��ĻҶ��������겻ȷ���ȱ�׼��
bool IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
									  int ix, int iy,				// input: the center of the region
									  int r,						// input: the radius of the circular region
									  double & dxIC, double & dyIC,	// output:the location of the calculated intensity centroid (in terms of offsets)
									  double sigma_I,				// input: the standard deviation of the Gaussian random noise of image intensity
									  double & sigma_xIC, double & sigma_yIC	// output: the standard deviation of the x y coordinates of the intensity centroid propagated by the random noise of intensity
									  );

// 20200825��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
bool CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
					int ix, int iy,			// input: the center of the region
					int r,					// input: the radius of the circular region
					double & angle			// output:the location of the calculated intensity centroid (in terms of offsets)
					);

// 20200825��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
// 20201206������ͼ��Ҷȸ�˹��������ı�׼��������õ�����������Ĳ�ȷ���ȱ�׼��
bool CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
					int ix, int iy,			// input: the center of the region
					int r,					// input: the radius of the circular region
					double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
					double & sigma_angle,	// output:the standard deviation of the corner angle propagated by the random noise of intensity
					double sigma_I = 5.0	// input: the standard deviation of the Gaussian random noise of image intensity					
					);

// 20200825��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
bool CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
					double x, double y,		// input: the center of the region
					int r,					// input: the radius of the circular region
					double & angle			// output:the location of the calculated intensity centroid (in terms of offsets)
					);

// 20200901��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
// ����������뾶�ɿ��Կɿ�ȷ����������Ƕ�ʱ�������С��ȷ��
// �Ƕȷ�ΧΪ[0,360)
bool FeatureRadiusAngle(const cv::Mat & img,		// input: the input gray scale image
						int ix, int iy,				// input: the center of the region
						int & r,					// output:the estimated radius of the circular region
						double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
						double threshOffset = 1.0,	// input: ���Ҷ�����ƫ�����ܳ�������ֵ������ʱ����Ϊ�ɿ�ȷ�����������ɴ˶��������߶ȴ�С
						int r_max = 100				// input: ������������뾶ֵ
						);

bool FeatureRadiusAngle(const cv::Mat & img,		// input: the input gray scale image
						double x, double y,			// input: the center of the region
						int & r,					// output:the estimated radius of the circular region
						double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
						double threshOffset = 1.0,	// input: ���Ҷ�����ƫ�����ܳ�������ֵ������ʱ����Ϊ�ɿ�ȷ�����������ɴ˶��������߶ȴ�С
						int r_max = 100				// input: ������������뾶ֵ
						);

// 20200901��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
// ����������뾶�ɿ��Կɿ�ȷ����������Ƕ�ʱ�������С��ȷ��
// �Ƕȷ�ΧΪ[0,360)
bool FeatureRadiusAngle_dAng(const cv::Mat & img,		// input: the input gray scale image
							 int ix, int iy,			// input: the center of the region
							 int & r,					// output:the estimated radius of the circular region
							 double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
							 double thresh_dAng = 5.0,	// input: ����ǰ���������������һ�ε����ĸı���С�ڸ���ֵʱ����Ϊ�ɿ�ȷ�����������ɴ˶��������߶ȴ�С
							 int r_max = 100			// input: ������������뾶ֵ
							 );

bool FeatureRadiusAngle_dAng(const cv::Mat & img,		// input: the input gray scale image
							 double x, double y,		// input: the center of the region
							 int & r,					// output:the estimated radius of the circular region
							 double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
							 double thresh_dAng = 5.0,	// input: ����ǰ���������������һ�ε����ĸı���С�ڸ���ֵʱ����Ϊ�ɿ�ȷ�����������ɴ˶��������߶ȴ�С
							 int r_max = 100			// input: ������������뾶ֵ
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
	std::vector<cv::KeyPoint> key_points; // 20200703����һ��С�Ķ������������ںϵ����������������������������ͬ��
	cv::Mat descriptors;
	std::vector<int> tracks;

	// 20200703������Ѳ�ͬ���͵�����������������������
	//std::vector<cv::KeyPoint> keypts_sift;
	//cv::Mat descrps_sift;

	//std::vector<cv::KeyPoint> keypts_fast;
	//cv::Mat descrps_fast;

	// zhaokunz, 20140324, sometimes multiple keypoints correspond to the same image point
	// like the SIFT features, a feature can have multiple orientations
	// this idx indicates the unique index of this keypoint, if two keypoints correspond to the same image point
	// their indices will be the same, otherwise will be different
	std::vector<int> idx_pt;

	// 20150214, zhaokunz, ���������㴦ͨ��˫���Բ�ֵ�õ��ĵ���ɫ��Ϣ
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
		descriptors.push_back(otherFeat.descriptors/*.clone()*/);	// 20200728�����Թ��ˣ�û��Ҫ .clone()����Ϊ��ֱ�� copy ��ֵ������ָ�룬�ϸ���Թ��ˡ�
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

	// 20150214, zhaokunz, ����Matx�ṹ��
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

	int m_nSiftElected;		// ǰ������ sift ��������ѡ�μ� SfM
	int m_nFastElected;		// ǰ������ FAST ��������ѡ�μ� SfM
	int m_nManualElected;	// ǰ�������������ѡ�μ� SfM

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
		m_featsBlob.clear(); // �Ȱ�֮ǰ�����

		auto f2d = cv::xfeatures2d::SIFT::create(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);

		// ��ȡ sift ������
		f2d->detect(img, m_featsBlob.key_points);

		// ������ size �Ӵ�С�� sift �������������
		sort(m_featsBlob.key_points.begin(), m_featsBlob.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.size > b.size; });

		// ����������������
		f2d->compute(img, m_featsBlob.key_points, m_featsBlob.descriptors);

		// 20201116���������������ͼ���� //////////////////////////////////////////////
// 		cv::Mat imgOut;
// 		cv::drawKeypoints(img, m_featsBlob.key_points, imgOut, Scalar(0, 255, 0));
// 
// 		CString strInfo;
// 		strInfo.Format("C:\\Users\\DeepV\\Desktop\\����ͼ��\\jpg\\000000.jpg");
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

		// ������Ҫ��Ϊ�˽� sift �������ظ�λ�õ�������ͬ���������Ϊͳһ��ȫ�ֱ�ţ�����ÿ�������㴦��ɫ��ֵ��ֵ����
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

			// 20150215, zhaokunz, �Ѹ����������ɫ��Ϣ��ֵ����
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
		m_featsCorner.clear(); // �Ȱ�֮ǰ�����

		// ��ȡ fast �ǵ����� //////////////////////////////////////////////////////
		// 20200706���Ȱ�ͼ��ת���ɻҶ�ͼ����ȡFAST��������Ϊopencv fast����ֻ�������ڻҶ�ͼ��sift�ǲ�ɫ�ͻҶȽԿɣ�����lightroom�������ͼƬ����ʱ�ᱻ�Զ�תΪ3ͨ��ͼ���ѹ�fast��ȡ����������ƫ��
		cv::Mat im_gray;

		int nChannel = img.channels();

		if (nChannel == 1) // ������ǻҶ�ͼ
		{
			im_gray = img;
		}
		else // ��ͼ
		{
			cv::cvtColor(img, im_gray, CV_RGB2GRAY);
		}
		
		cv::FAST(im_gray, m_featsCorner.key_points, thresholdFast, nonmaxSuppressionFast, typeFast);
		//////////////////////////////////////////////////////////////////////////

		// ���� response �Ӵ�С�� fast �������������
		sort(m_featsCorner.key_points.begin(), m_featsCorner.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });

		int n = m_featsCorner.key_points.size();

		// 20200830��Ϊÿ�� FAST ���������һ���������� ////////////////////////////
		for (int i = 0; i < n; ++i)
		{
			cv::KeyPoint & keypt = m_featsCorner.key_points[i];

//			keypt.size = size;

			int r/* = (size - 1)*0.5*/;
			double angle;

// 			if (CornerAngle_IC(im_gray, keypt.pt.x, keypt.pt.y, r, angle))
// 			{
// 				if (angle < 0) // ȷ�����յĽǶȷ�Χ���� opencv keypoint::angle ��ȡֵ��Χ���� [0,360)
// 				{
// 					angle += 360;
// 				}
// 
// 				keypt.angle = angle;
// 			}

			// 20200901��ͬʱ���������߶Ⱥͷ��� ////////////////////////////////////////
// 			if (FeatureRadiusAngle(im_gray, keypt.pt.x, keypt.pt.y, r, angle, thresholdOffset, r_max))
// 			{
// 				keypt.size = (2 * r + 1)*0.33333333333; // keypoint::size ����������ֱ�� diameter
// 				keypt.angle = angle;
// 			}

			if (FeatureRadiusAngle_dAng(im_gray, keypt.pt.x, keypt.pt.y, r, angle, 3.0))
			{
				keypt.size = 2 * r + 1; // keypoint::size ����������ֱ�� diameter
				keypt.angle = angle;
			}
			//////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////

		// ����������������
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

		// ������Ҫ��Ϊ�˽� sift �������ظ�λ�õ�������ͬ���������Ϊͳһ��ȫ�ֱ�ţ�����ÿ�������㴦��ɫ��ֵ��ֵ����
		for (int i = 0; i < n; ++i)
		{
			const cv::KeyPoint & keypt = m_featsCorner.key_points[i];

			m_featsCorner.tracks.push_back(-1);
			m_featsCorner.idx_pt.push_back(i);

			// 20150215, zhaokunz, �Ѹ����������ɫ��Ϣ��ֵ����
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
		m_featsCorner.clear(); // �Ȱ�֮ǰ�����

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
// 				if (angle < 0) // ȷ�����յĽǶȷ�Χ���� opencv keypoint::angle ��ȡֵ��Χ���� [0,360)
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

		// ���� response �Ӵ�С�� fast �������������
		sort(m_featsCorner.key_points.begin(), m_featsCorner.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });

		// ����������������
		auto f2d = cv::xfeatures2d::SIFT::create(nfeaturesSift, nOctaveLayersSift, contrastThresholdSift, edgeThresholdSift, sigmaSift);
		f2d->compute(img, m_featsCorner.key_points, m_featsCorner.descriptors);

		m_featsCorner.type = Feature_ORB_SIFT; // ORB keypoints + sift descriptors

		// ������Ҫ��Ϊ�˽� sift �������ظ�λ�õ�������ͬ���������Ϊͳһ��ȫ�ֱ�ţ�����ÿ�������㴦��ɫ��ֵ��ֵ����
		int n = m_featsCorner.key_points.size();
		for (int i = 0; i < n; ++i)
		{
			const cv::KeyPoint & keypt = m_featsCorner.key_points[i];

			m_featsCorner.tracks.push_back(-1);
			m_featsCorner.idx_pt.push_back(i);

			// 20150215, zhaokunz, �Ѹ����������ɫ��Ϣ��ֵ����
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

		// �����
		m_feats.clear();
		m_subFeats.clear();

		// ��ʱ�Ⱥϳɸ����
		Features featsTmp = m_featsBlob;
		featsTmp.push_back(m_featsCorner);

		// Ȼ���ȡΪ���յģ���¼��
		int nSize = featsTmp.key_points.size();
		int nSmaller = nSize < nSfMFeatures ? nSize : nSfMFeatures;

		m_feats.key_points.insert(m_feats.key_points.end(), featsTmp.key_points.begin(), featsTmp.key_points.begin() + nSmaller);
		m_feats.descriptors = featsTmp.descriptors.rowRange(cv::Range(0, nSmaller));
		m_feats.tracks.insert(m_feats.tracks.end(), featsTmp.tracks.begin(), featsTmp.tracks.begin() + nSmaller);
		m_feats.idx_pt.insert(m_feats.idx_pt.end(), featsTmp.idx_pt.begin(), featsTmp.idx_pt.begin() + nSmaller);
		m_feats.rgbs.insert(m_feats.rgbs.end(), featsTmp.rgbs.begin(), featsTmp.rgbs.begin() + nSmaller);

		// �ٰ������ӽ���
		m_feats.push_back(m_featsManual);

		int nFinal = m_feats.key_points.size();

		// ͳ�Ƹ������ѡ���� SfM ������ĸ���
		m_nSiftElected = nSift < nSfMFeatures ? nSift : nSfMFeatures;
		m_nFastElected = nSmaller - m_nSiftElected;
		m_nManualElected = nManual; // ��ȫ������㶼��ѡ

		// ���²��� SfM ���������ͳһ���
		for (int i = m_nSiftElected; i < nFinal; ++i)
		{
			m_feats.idx_pt[i] = i;
		}

		// ������ɡ��ȷ����ˡ��������㼯
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

// ������CMatrix
// ����������������ü������ƣ�����˺�������Ч�ʣ���ͬʱҲ���������ɶ��Ե��½�
// ���磺CMatrix m1, m2; m1 = m2 �����ǽ�m2�����������ݸ��Ƶ�m1��������������m1��m2�е�m_pMatָ��ͬһ������ͷ�����m1��m2Ҳ��������ͬһ�����������ڸı�m1����
// ĳԪ��ֵ��ͬʱ��m2��ӦԪ�ص�ֵҲ�����˸ı䣬��m1 = m2 ��仰��ʹ��������������ȫ��������������ٶ�����CMatrix m1 = m2Ч����һ���ġ�
// ������������ȫ�����ľ��󣨼����������и��Զ�����������������������һ����������ݸ��Ƶ���һ��������������Ļ�������ͨ����� CMatrix m1 = m2.Clone()��

// �������תģʽ��ö������
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
	CMatrix(const CMatrix & mat);                                     // �������캯�������ڴ����Լ����������
	/*CMatrix(int nRow, int nColumn, double * pInitArray);*/
	virtual ~CMatrix();

	// ����
public:                                                
	int     m_nRow;                                                   // ��������
	int     m_nCol;                                                   // ��������
	CvMat * m_pMat;                                                   // ��װ��OpenCV�ľ���ṹ���ָ��

private:
	/*CvMat * m_pMat;                                                   // ��װ��OpenCV�ľ���ṹ���ָ��*/

	// ����
public:
	double & operator () (int i, int j) const;                        // ������ʽ��1��ʼ
	double & operator () (int i)        const;                        // ��������������ʽ�����������������������������ͨ��һά������ʽ�Ծ���Ԫ�ؽ���ȡֵ��ֵ

	CMatrix & operator = (const CMatrix & mSource);                   // ���ظ�ֵ�������ʹ�ñ������mSource�е�m_pMatָ��ͬһ������ͷ�����Ҳӵ��ͬһ�����ü����洢�������������ı䱾��������ݻ�ʹ��mSource������Ҳ�����仯
	CMatrix & operator = (double val);                                // �Ѿ�������Ԫ�ص�ֵ�û�Ϊval

	CMatrix operator + (const CMatrix & mAdd) const;                  // ���ؼӺ������
	CMatrix operator + (double val) const;                            // ��������Ԫ�ص�ֵ��val
	friend CMatrix operator + (double val, const CMatrix & mAdd);     // ��������Ԫ�ص�ֵ���һ��val

	CMatrix operator - (const CMatrix & mSub) const;				  // ���ؼ��������
	CMatrix operator - (double val) const;                            // ��������Ԫ�ص�ֵ��val
	CMatrix operator - () const;                                      // ��������Ԫ��ȡ������Ŀ���������
	friend CMatrix operator - (double val, const CMatrix & mSub);	  // ��������Ԫ�ص�ֵ���һ��val

	CMatrix operator * (const CMatrix & mMul) const;				  // ���س˷������
	CMatrix operator * (double val) const;                            // �ҳ�һ����
	friend CMatrix operator * (double val, const CMatrix & mMul);     // ���һ����

	CMatrix operator / (const CMatrix & mDiv) const;                  // �������һ����ͬ�ߴ�ľ������������Ԫ�ض�Ӧ���������һ���������һ�������
	CMatrix operator / (double val) const;                            // �������Ԫ�س���val
	friend CMatrix operator / (double val, const CMatrix & mDiv);     // һ��������һ������

	void operator += (const CMatrix & mAdd);                          // �����Լ�һ����
	void operator += (double val);                                    // �����Լ�һ��

	void operator -= (const CMatrix & mSub);                          // �����Լ�һ����
	void operator -= (double val);                                    // �����Լ�һ��

	void operator *= (const CMatrix & mMul);                          // �����Գ�һ������ָ����ͬ�ߴ�������Ԫ�ض�Ӧ���
	void operator *= (double val);                                    // �����Գ�һ��

	void operator /= (const CMatrix & mDiv);                          // �����Գ�һ������ָ����ͬ�ߴ�������Ԫ�ض�Ӧ���
	void operator /= (double val);                                    // �����Գ�һ��



	void Release(void);                                               // �������ü������Ƶľ����ͷź���



	BOOL IsNull(void)       const;                                    // �Ƿ��ǿվ��󣬼�m_pMat�Ƿ�ΪNULL
	BOOL IsZero(void)       const;                                    // �Ƿ��������
	BOOL IsVector(void)     const;                                    // �Ƿ�������
	BOOL IsRowVec(void)     const;                                    // �Ƿ���������
	BOOL IsColVec(void)     const;                                    // �Ƿ���������
	BOOL IsSquare(void)     const;                                    // �Ƿ��Ƿ���
	BOOL IsSymmetric(void)  const;                                    // �Ƿ��ǶԳ���
	BOOL IsAntiSymm(void)   const;                                    // �Ƿ��Ƿ��Գ���
	BOOL IsDiagonal(void)   const;                                    // �Ƿ��ǶԽ��󣬲�һ����Ҫ�Ƿ���
	BOOL IsIdentity(void)   const;                                    // �Ƿ��ǵ�λ��
	BOOL IsSymmPosDef(void) const;                                    // �Ƿ��ǶԳ���������������Cholesky�ֽ�



	CMatrix Clone(void)     const;                                    // ���صĶ����е�m_pMatָ��һ���µľ���ͷ����������һ���µ������������µ��������е������ǶԱ��������������������ݵĿ���
	CMatrix GetRow(int i)   const;                                    // ���ؾ�������ĳһ�У�ҲΪCMatrix����������ʽ��1��ʼ
	CMatrix GetCol(int i)   const;                                    // ���ؾ�������ĳһ�У�ҲΪCMatrix����������ʽ��1��ʼ
	CMatrix GetRect(int rowTL, int colTL, int width, int height) const; // ���ؾ�������ĳһ���ο飬ҲΪһ�������������ʽ��1��ʼ
	CMatrix GetDiag(void)   const;                                    // ���ؾ���Խ����ϵ�Ԫ��
	CMatrix Transpose(void) const;                                    // ����ת��
	CMatrix Inverse(void)   const;                                    // ��������
	CMatrix Pow(double power) const;                                  // ���еľ���Ԫ������
	CMatrix Log(void)       const;                                    // ���еľ���Ԫ��ȡ����
	CMatrix Max(void)       const;                                    // �ҵ������ȫ�����ֵ�������������ؾ���ĵ�һ��Ϊ�������ֵ����Ԫ�ص����������ڶ���Ϊ����������������Ϊ�����ֵ
	CMatrix Min(void)       const;                                    // �ҵ������ȫ����Сֵ�������������ؾ���ĵ�һ��Ϊ������Сֵ����Ԫ�ص����������ڶ���Ϊ����������������Ϊ����Сֵ
	CMatrix Find(double val, double eps = 1.0E-10) const;             // �ھ������ҵ�ֵΪval��Ԫ�أ�������ЩԪ�ص����������ţ����ؾ���ĵ�һ��Ϊ���������ڶ���Ϊ������
	CMatrix Abs(void)       const;                                    // ���صľ���������Ԫ��Ϊԭ�����ӦԪ�صľ���ֵ
	CMatrix Atand(void)     const;                                    // ���صľ����е�Ԫ��Ϊԭ�����ж�ӦԪ�صķ�����ֵ



	void SetRow(int i, const CMatrix & mSrc);                         // �����е�һ������mSrc��ֵ������ĵ�i�У�������ʽ��1��ʼ
	void SetCol(int i, const CMatrix & mSrc);                         // �����е�һ������mSrc��ֵ������ĵ�i�У�������ʽ��1��ʼ
	void SetRect(int rowTL, int colTL, const CMatrix & mSrc);         // �����е�һ������mSrc��ֵ�������ĳһ��������򣬱���ֵ�ľ��������С��mSrc�ĳߴ����
	void AddOneRow(double initVal = 0);                               // ��ԭ��������һ�к�������һ���γ��µ���չ�ľ���
	void AddOneCol(double initVal = 0);                               // ��ԭ��������һ�к�������һ���γ��µ���չ�ľ���
	void Flip(FlipMode flipMode);                                     // ��������ķ�תģʽ���þ�����x�ᷭת������y�ᷭת������Ǽ���x������y�ᷭת
	void Mosaic(const CMatrix & mOther, MosaicMode mode = MOSAICMODE_DOWN); // ����һ������һ��ģʽƴ�ӵ���������
	void Reshape(int newRow, int newCol);                             // �ı����ĳߴ磬������Ԫ�ص�������Ȼ����
	void VectorizeByRow(void);                                        // �����������г�ʸ����ʽ
	void VectorizeByCol(void);                                        // �����������г�ʸ����ʽ



	double Det(void)                         const;				      // �����������ʽ��ֵ
	double Trace(void)                       const;                   // �������ļ�
	int    Rank(double eps = 1.0E-10)        const;                   // ����������
	double InvCond(void)                     const;                   // ��������������ĵ���������С����ֵ���������ֵ�ı�ֵ
	double Mean(void)                        const;                   // �����������Ԫ�ص�ƽ��ֵ
	double StdDev(void)                      const;                   // �����������Ԫ�صı�׼��
	int    CountNonZero(void)                const;                   // ͳ�ƾ������Ԫ�صĸ���
	double Sum(void)                         const;                   // �Ծ���������Ԫ�����
	double Norm(NormMode mode = NORMMODE_L2) const;                   // �������ķ���
	int    Length(void)                      const;                   // ��CMatrix�����������򷵻��������ȣ���Ϊ�����򷵻�ӵ��Ԫ�������Ǹ�ά���ϵ�Ԫ�ظ���
	double Dev(void)                         const;                   // ���� sqrt(sum(m(i,j)^2) / n)



	friend CMatrix Mul(const CMatrix & m1, const CMatrix & m2);       // ��ͬ��С�ľ������Ԫ�ض�Ӧ���
	friend CMatrix Div(const CMatrix & m1, const CMatrix & m2);       // ��ͬ��С�ľ������Ԫ�ض�Ӧ���
	friend CMatrix ExtendRows(const CMatrix & mMat, int row);         // ������ľ���row�н�����չ
	friend CMatrix ExtendCols(const CMatrix & mMat, int col);         // ������ľ���col�н�����չ
	friend CMatrix RepMat    (const CMatrix & mMat, int row, int col);// ������ľ���row�к�col�н�����չ
	friend CMatrix Rand(int row, int col, RandMode randMode, double param1, double param2); // �������������ɵľ���randModeΪ���ֲַ����������param1��param2��Ϊuniform�ֲ�ʱ�ֱ��ʾ���½磬��gaussianʱ�ֱ�Ϊ�������ƽ��ֵ�ͱ�׼��
	friend CMatrix Solve(const CMatrix & mA, const CMatrix & mB);     // ���Ax=B
	friend CMatrix Cross(const CMatrix & mVec1, const CMatrix & mVec2);// �������ά�����Ĳ�ˣ�mVec1��mVec2
	friend void    RQ3x3(const CMatrix & mSrc, CMatrix & mR, CMatrix & mQ);// ��3��3�ľ������RQ�ֽ⣺A = RQ������RΪ�Խ���Ԫ��ȫΪ���������Ǿ���Q��Ϊ��������
	friend CMatrix GenI(int n);                                       // ����n��n�ĵ�λ��
	friend CMatrix GenCrossMat(const CMatrix & mVec);                 // �����������ά����v�������Ӧ�Ĳ�˾���[v]
	friend void    House(const CMatrix & mX, CMatrix & mVec, double & beta); // �������������mX��������Ӧ��Householder vector mVec���Լ���
	friend void    Givens(double a, double b, double & c, double & s);// ���������[a; b]'����ʹ��[c s; -s c]' * [a; b]' = [r; 0]'������cos��sin



	void    QR_Householder(CMatrix & mR, CMatrix & mQ = CMatrix(0)) const;  // ��m��n�ľ���A����QR�ֽ⣺A = QR������QΪm��m����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
	void    QR_Givens(CMatrix & mR, CMatrix & mQ = CMatrix(0)) const;       // ��m��n�ľ���A����QR�ֽ⣺A = QR������QΪm��m����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
	void    RQ_Givens(CMatrix & mR, CMatrix & mQ = CMatrix(0)) const;       // ��m��n�ľ���A����RQ�ֽ⣨Ҫ��m<=n����A = RQ������QΪn��n����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
	CMatrix Chol_Lower(double eps = EPSILON_ZHU)   const;                   // �ԶԳ���������ʵʩCholesky�ֽ� A = LL'�����ضԽ�����Ԫ��ȫΪ���������Ǿ���L���˺���Ҳ�����ж϶Գƾ����Ƿ��������ֽ�ɹ��򷵻صľ������m_pMat��ΪNULL������ΪNULL
	CMatrix Chol_Upper(double eps = EPSILON_ZHU)   const;                   // �ԶԳ���������ʵʩCholesky�ֽ� A = UU'�����ضԽ�����Ԫ��ȫΪ���������Ǿ���U���˺���Ҳ�����ж϶Գƾ����Ƿ��������ֽ�ɹ��򷵻صľ������m_pMat��ΪNULL������ΪNULL
	void    SVD(CMatrix & mD, CMatrix & mU = CMatrix(0), CMatrix & mV = CMatrix(0)) const; // �Ծ���ʵʩSVD�ֽ⣬A = UDV'



	void Write(CString filePath, WriteMode mode = WRITEMODE_LF) const;      // �Ѿ���д��ָ���ļ�
	BOOL Read(CString filePath);                                            // ���ļ��������ݵ��������
	void PrintMatrix2Screen(CString title = "", WriteMode mode = WRITEMODE_LF) const;     // ͨ��AfxMessageBox�Ѿ�����ʾ����Ļ��
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
// ���ܵĽⷽ�̼����������� /////////////////////////////////////////////////////////////////////////////////
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

// �� 1 ά�������ð�ݷ����򣬿�����������Ҳ�ɽ������У������ԭ�����Ԫ����������������������е�����
void Sort_Bubble(double * p,			// ��������������������飬����ִ����������е�Ԫ�ؽ���������߽�������
				 int n,					// ���룺���鳤�ȣ�Ԫ�ظ���
				 int * idx,				// �����������ɺ��Ԫ��ԭ���������ŵ���������������� 0 ��ʼ
				 BOOL bIsAscent = TRUE	// ���룺�����򣬻��ǽ�������
				 );

// �� 1 ά����������������������������
void Sort_byIndex_double(double * p,	// ��������������������飬����ִ����������е�Ԫ�ؽ��� idx �е�����������������
				         int n,			// ���룺���鳤�ȣ�Ԫ�ظ���
				         int * idx		// ���룺����ֵ���������Ǹ���������������е��������������еĸ�Ԫ�ؽ��������еģ������� 0 ��ʼ
				         );

// �� 1 ά����������������������������
void Sort_byIndex_int(int * p,			// ��������������������飬����ִ����������е�Ԫ�ؽ��� idx �е�����������������
				      int n,			// ���룺���鳤�ȣ�Ԫ�ظ���
				      int * idx			// ���룺����ֵ���������Ǹ���������������е��������������еĸ�Ԫ�ؽ��������еģ������� 0 ��ʼ
				      );

// �� 2 ά�����ÿһ�а�ĳһ�е����ݽ���������߽�������
void Sort2_byRow(double ** p,			// ����������������� 2 ά����
				 int w, int h,			// ���룺2 ά����Ŀ��
				 int row = 0,			// ���룺��ĳһ�е����ݶ������н�������Ĭ��Ϊ�� 1 �У������� 0 ��ʼ
				 BOOL bIsAscent = TRUE	// ���룺�������ǽ������У�Ĭ��Ϊ����
				 );

// �� 2 ά�����ÿһ�а�ĳһ�е����ݽ���������߽�������
void Sort2_byCol(double ** p,			// ����������������� 2 ά����
				 int w, int h,			// ���룺2 ά����Ŀ��
				 int col = 0,			// ���룺��ĳһ�е����ݶ������н�������Ĭ��Ϊ�� 1 �У������� 0 ��ʼ
				 BOOL bIsAscent = TRUE	// ���룺�������ǽ������У�Ĭ��Ϊ����
				 );

// �ҵ������� 1 ά�����е���Сֵ���������Сֵ�������е�����
double FindArrayMin(double * p,					// ���룺������� 1 ά����
					int n,						// ���룺����ĳ���
					int & idx,					// ������ҵ���������Сֵ���������� 0 ��ʼ
					double * pMinThresh = NULL	// even the minimal value is supposed to be larger than this threshold
					);

// �ҵ������� 1 ά�����е����ֵ����������ֵ�������е�����
double FindArrayMax(double * p,	// ���룺������� 1 ά����
					int n,		// ���룺����ĳ���
					int & idx	// ������ҵ����������ֵ���������� 0 ��ʼ
					);

// �ҵ������� 1 ά�����е����ֵ����������ֵ�������е�����
BYTE FindArrayMax_BYTE(BYTE * p,	// ���룺������� 1 ά����
					   int n,		// ���룺����ĳ���
					   int & idx	// ������ҵ����������ֵ���������� 0 ��ʼ
					   );

// �ҵ������� 1 ά�����е����ֵ����������ֵ�������е�����
int FindArrayMax_int(int * p,		// ���룺������� 1 ά����
					 int n,			// ���룺����ĳ���
					 int & idx		// ������ҵ����������ֵ���������� 0 ��ʼ
					 );

// ���� double �͵ı���������ֵ
void Swap_double(double & A, double & B);
// ���� int �͵ı���������ֵ
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

// 20200717��������Բ�ȷ����ˮƽ��������Ӧ��ɫ��ֵ��RGɫ���򣩣������ڵ��Ʋ�ȷ���ȿ��ӻ�
void getRGColorforRelativeUncertainty(double uctt,		// input: the given relative uncertainty
									  double val_worst,	// input: the worst value for relative uncertainty, which is set to color Red
									  double val_best,	// input: the best value for relative uncertainty, which is set to color Green
									  uchar & r,			// output: the computed R
									  uchar & g,			// output: the computed G
									  uchar & b			// output: the computed B
									  );

// 20200717��������Բ�ȷ����ˮƽ��������Ӧ��ɫ��ֵ��RGɫ���򣩣������ڵ��Ʋ�ȷ���ȿ��ӻ�
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