/************************************************************************************/
/* Johnny Zhu's own library, including kinds of self-defined data types and classes.*/
/* Author: Johnny Zhu (DeepVoid) in Melbourne                        Date:2014.03.02*/
/************************************************************************************/

#pragma once

// #include "opencv/cv.h"
// #include "opencv/highgui.h"
// #include "opencv/cvaux.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/nonfree/nonfree.hpp"
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define MINSCALENUM			0	// the index of the minimal scale
#define MAXSCALENUM			13	// the index of the maximal scale
#define FITSCALENUM			6

const float ImageDisplayScales[] = {0.03125, 0.0625, 0.125, 0.25, 0.5, 0.75, 1, 1.5, 2, 3, 5, 10, 20, 25};

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
	Feature_SIFT       = 0,     
	Feature_SURF       = 1,
};

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

	// 20150214, zhaokunz, 所有特征点处通过双线性插值得到的的颜色信息
	std::vector<cv::Vec3b> rgbs;

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
			rgbs = otherFeat.rgbs;
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

	bool m_bCalibed;	// whether interior calibrated or not
	bool m_bOriented;	// whether exterior oriented or not
	//////////////////////////////////////////////////////////////////////////
	
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

		m_bCalibed = false;
		m_bOriented = false;
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

			m_K = otherCam.m_K;
			m_R = otherCam.m_R;
			m_t = otherCam.m_t;
			m_bCalibed = otherCam.m_bCalibed;
			m_bOriented = otherCam.m_bOriented;
		}

		return *this;
	};

	cam_data(const cam_data & cam)
	{
		*this = cam;
	};
};

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

}