#include "stdafx.h"

BOOL DeepVoid::DisplayImage(CDC * pDC,									// pointer to CDC
							const cv::Mat & img,							// image to be displayed
							int tl_x, int tl_y, int rect_w, int rect_h	// region to put the image in
							)
{
	if (!pDC || img.empty() || rect_w<=0 || rect_h<=0)
	{
		return FALSE;
	}

	uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
	BITMAPINFO * bmi = (BITMAPINFO *)buffer;

	int img_w = img.cols;
	int img_h = img.rows;
	int img_c = img.channels();

	int bpp = 8 * img_c; // bits per pixel

	BITMAPINFOHEADER * bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = img_w;
	bmih->biHeight = -img_h;
	bmih->biPlanes = 1;
	bmih->biBitCount = bpp;
	bmih->biCompression = BI_RGB;

	if (8 == bpp)
	{
		RGBQUAD * palette = bmi->bmiColors;
		int i;
		for (i=0;i<256;i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	if (rect_w < img_w)
	{
		SetStretchBltMode(pDC->m_hDC, HALFTONE);
	} 
	else
	{
		SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
	}

	if ((img_w*img_c)%4 != 0 && img.isContinuous())
	{
		CvSize size;
		size.width = img_w;	size.height = img_h;
		IplImage * imgTmp = cvCreateImage(size, 8, img_c);
//		img.copyTo(cv::Mat(imgTmp));
		img.copyTo(cv::cvarrToMat(imgTmp)); // 20160316, cv::Mat(const IplImage * img) is removed in opencv 3.x, use cv::cvarrToMat instead.

		::StretchDIBits(pDC->m_hDC, tl_x, tl_y, rect_w, rect_h,
			0, 0, img_w, img_h, imgTmp->imageData, bmi, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&imgTmp);
	} 
	else
	{
		::StretchDIBits(pDC->m_hDC, tl_x, tl_y, rect_w, rect_h,
			0, 0, img_w, img_h, img.data, bmi, DIB_RGB_COLORS, SRCCOPY);
	}

	return TRUE;
}

CString DeepVoid::GetFileNameOnly(CString filePath)
{
	int index = filePath.GetLength() - filePath.ReverseFind('\\') - 1;
	CString strFileName = filePath.Right(index);
	return strFileName;
}

// ����Ƕȵ�cosֵ������Ϊ�Ƕ�
double DeepVoid::cosd(double angle)
{
	double radian = angle * CV_PI / 180;

	return cos(radian);
}

// ����Ƕȵ�sinֵ������Ϊ�Ƕ�
double DeepVoid::sind(double angle)
{
	double radian = angle * CV_PI / 180;

	return sin(radian);
}

// ����Ƕȵ�tanֵ������Ϊ�Ƕ�
double DeepVoid::tand(double angle)
{
	double radian = angle * CV_PI / 180;

	return tan(radian);
}

// ����������ֵ��arccosineֵ������ֵ��ΧΪ0 - 180��
double DeepVoid::acosd(double x)
{
	double radian = acos(x);

	return radian * 180 / CV_PI;
}

// ����������ֵ��arcsineֵ������ֵ��ΧΪ-90 - 90��
double DeepVoid::asind(double x)
{
	double radian = asin(x);

	return radian * 180 / CV_PI;
}

// ����������ֵ��arctangentֵ������ֵ��ΧΪ-90 - 90��
double DeepVoid::atand(double x)
{
	double radian = atan(x);

	return radian * 180 / CV_PI;
}

// ����(y / x)��arctangentֵ�����ڿ����ۺϿ���y��x�ķ��ţ���˷���ֵ�ķ�Χ��չ���ĸ����ޣ���-180 - 180��
double DeepVoid::atand(double y, double x)
{
	double radian = atan2(y, x);

	return radian * 180 / CV_PI;
}

// �ж����� x �ķ��ţ�Ϊ���򷵻� 1��Ϊ���򷵻� -1
double DeepVoid::Sign_Johnny(double x)
{
	if (x > 0)
	{
		return 1;
	} 
	else if (x < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

// Implementation of bilinear interpolation
double DeepVoid::BilinearInterp(double x, double y,	// the coordinates of position to be interpolated
								double x1,			// the x coordinate of topleft f1 and bottomleft f3
								double x2,			// the x coordinate of topright f2 and bottomright f4
								double y1,			// the y coordinate of topleft f1 and topright f2
								double y2,			// the y coordinate of bottomleft f3 and bottomright f4
								double f1, double f2, double f3, double f4	// the corresponding values of topleft,topright,bottomleft and bottomright points
								)
{
	
	double x2x1 = x2 - x1;
	double x2x1_1 = 1/x2x1;

	double x2x  = x2 - x;
	double xx1  = x  - x1;

	double y2y1 = y2 - y1;
	double y2y1_1 = 1/y2y1;

	double y2y  = y2 - y;
	double yy1  = y  - y1;

	double Q1 = (x2x*f1+xx1*f2) * x2x1_1;
	double Q2 = (x2x*f3+xx1*f4) * x2x1_1;

	return (yy1*Q2+y2y*Q1) * y2y1_1;
}

bool DeepVoid::BilinearInterp(const Mat & img,		// input:	the image data
							  double x, double y,	// input:	the coordinates of position to be interpolated
							  uchar & r,			// output:	the interpolated R
							  uchar & g,			// output:	the interpolated G
							  uchar & b				// output:	the interpolated B
							  )
{
	int i;

	int nc = img.channels();

// 	int x1 = floor(x);	// the x coordinate of the two left points
// 	int x2 = ceil(x);	// the x coordinate of the two right points
// 	int y1 = floor(y);	// the y coordinate of the two top points
// 	int y2 = ceil(y);	// the y coordinate of the two bottom points

	// zhaokunz, 20140406, use the fast floor and abandon usage of ceil, since x2=x1+1, y2=y1+1
	int x1 = floor_fast(x);	// the x coordinate of the two left points
	int x2 = x1+1;			// the x coordinate of the two right points
	int y1 = floor_fast(y);	// the y coordinate of the two top points
	int y2 = y1+1;			// the y coordinate of the two bottom points

	if (x1<0 || y1<0 || x2>=img.cols || y2>=img.rows)
	{
		return false;
	}

	if (1 == nc)
	{
		Scalar pix = img.at<uchar>(y1, x1); // topleft
		uchar f1 = pix.val[0];

		pix = img.at<uchar>(y1, x2); // topright
		uchar f2 = pix.val[0];

		pix = img.at<uchar>(y2, x1); // bottomleft
		uchar f3 = pix.val[0];

		pix = img.at<uchar>(y2, x2); // bottomright
		uchar f4 = pix.val[0];

		uchar f = (uchar)BilinearInterp(x, y, x1, x2, y1, y2, f1, f2, f3, f4);

		r=g=b=f;
	} 
	else
	{
		vector<uchar> vF;

		for (i=0;i<3;i++)
		{
			Vec3b pix = img.at<Vec3b>(y1, x1); // topleft
			uchar f1 = pix.val[i];

			pix = img.at<Vec3b>(y1, x2); // topright
			uchar f2 = pix.val[i];

			pix = img.at<Vec3b>(y2, x1); // bottomleft
			uchar f3 = pix.val[i];

			pix = img.at<Vec3b>(y2, x2); // bottomright
			uchar f4 = pix.val[i];

			uchar f = (uchar)BilinearInterp(x, y, x1, x2, y1, y2, f1, f2, f3, f4);

			vF.push_back(f);
		}

		r = vF[2];
		g = vF[1];
		b = vF[0];
	}
	return true;
}

bool DeepVoid::BilinearInterp(const Mat & img,		// input:	the image data
							  double x, double y,	// input:	the coordinates of position to be interpolated
							  double & r,			// output:	the interpolated R
							  double & g,			// output:	the interpolated G
							  double & b			// output:	the interpolated B
							  )
{
	int i;

	int nc = img.channels();

	// zhaokunz, 20140406, use the fast floor and abandon usage of ceil, since x2=x1+1, y2=y1+1
	int x1 = floor_fast(x);	// the x coordinate of the two left points
	int x2 = x1+1;			// the x coordinate of the two right points
	int y1 = floor_fast(y);	// the y coordinate of the two top points
	int y2 = y1+1;			// the y coordinate of the two bottom points

	if (x1<0 || y1<0 || x2>=img.cols || y2>=img.rows)
	{
		return false;
	}

	if (1 == nc)
	{
		Scalar pix = img.at<uchar>(y1, x1); // topleft
		double f1 = pix.val[0];

		pix = img.at<uchar>(y1, x2); // topright
		double f2 = pix.val[0];

		pix = img.at<uchar>(y2, x1); // bottomleft
		double f3 = pix.val[0];

		pix = img.at<uchar>(y2, x2); // bottomright
		double f4 = pix.val[0];

		double f = BilinearInterp(x, y, x1, x2, y1, y2, f1, f2, f3, f4);

		r=g=b=f;
	} 
	else
	{
		vector<double> vF;

		for (i=0;i<3;i++)
		{
			Vec3b pix = img.at<Vec3b>(y1, x1); // topleft
			double f1 = pix.val[i];

			pix = img.at<Vec3b>(y1, x2); // topright
			double f2 = pix.val[i];

			pix = img.at<Vec3b>(y2, x1); // bottomleft
			double f3 = pix.val[i];

			pix = img.at<Vec3b>(y2, x2); // bottomright
			double f4 = pix.val[i];

			double f = BilinearInterp(x, y, x1, x2, y1, y2, f1, f2, f3, f4);

			vF.push_back(f);
		}

		r = vF[2];
		g = vF[1];
		b = vF[0];
	}
	return true;
}

bool DeepVoid::BilinearInterp(const Matx33d & mK,				// input:	the camera matrix
							  const Matx33d & mR,				// input:	the rotation matrix
							  const Matx31d & mt,				// input:	the translation vector
							  const Mat & img,				// input:	the input image
							  double X, double Y, double Z,	// input:	the coordinates of position to be interpolated
							  uchar & r,						// output:	the interpolated R
							  uchar & g,						// output:	the interpolated G
							  uchar & b						// output:	the interpolated B
							  )
{
	Matx31d mX;
	mX(0) = X; mX(1) = Y; mX(2) = Z;

	Matx33d mKR = mK*mR;
	Matx31d mKt = mK*mt;

	Matx31d mx = mKR*mX + mKt;

	double z_1 = 1/mx(2);

	double img_x = mx(0)*z_1;
	double img_y = mx(1)*z_1;

	return BilinearInterp(img, img_x, img_y, r, g, b);
}

bool DeepVoid::BilinearInterp(const Matx33d & mK,			// input:	the camera matrix
							  const Matx33d & mR,			// input:	the rotation matrix
							  const Matx31d & mt,			// input:	the translation vector
							  const Mat & img,				// input:	the input image
							  double X, double Y, double Z,	// input:	the coordinates of position to be interpolated
							  double & r,					// output:	the interpolated R
							  double & g,					// output:	the interpolated G
							  double & b,					// output:	the interpolated B
							  double * imgpt_x /*= NULL*/,
							  double * imgpt_y /*= NULL*/
							  )
{
	Matx31d mX;
	mX(0) = X; mX(1) = Y; mX(2) = Z;

	Matx33d mKR = mK*mR;
	Matx31d mKt = mK*mt;

	Matx31d mx = mKR*mX + mKt;

	double z_1 = 1/mx(2);

	double img_x = mx(0)*z_1;
	double img_y = mx(1)*z_1;

	if (imgpt_x && imgpt_y)
	{
		*imgpt_x = img_x;
		*imgpt_y = img_y;
	}

	return BilinearInterp(img, img_x, img_y, r, g, b);
}

void DeepVoid::MakeSureNotOutBorder(int x, int y,				// input:	original center of rect
									int & x_new, int & y_new,	// output:	new center of rect, making sure the new rect with the same size are within border
									int wndSizeHalf,
									int imgWidth, int imgHeight
									)
{
	x_new = x;
	y_new = y;

	if (x-wndSizeHalf<0)
	{
		x_new = wndSizeHalf;
	}
	if (x+wndSizeHalf>=imgWidth)
	{
		x_new = imgWidth-1-wndSizeHalf;
	}

	if (y-wndSizeHalf<0)
	{
		y_new = wndSizeHalf;
	}
	if (y+wndSizeHalf>=imgHeight)
	{
		y_new = imgHeight-1-wndSizeHalf;
	}
}

bool DeepVoid::IntensityCentroid_CircularRegion(const cv::Mat & img, /* input: the input gray scale image */ double x, double y, /* input: the center of the region */ int r, /* input: the radius of the circular region */ double & xIC, double & yIC /* output:the location of the calculated intensity centroid */)
{
	int w = img.cols;
	int h = img.rows;

	// ȷ����ǰ������ڵľ�����������
	int ix = (int)x;
	int iy = (int)y;

	int m00 = 0;
	int m10 = 0;
	int m01 = 0;

	for (int di = -r; di <= r; ++di)
	{
		int i = iy + di;

		if (i < 0 || i >= h)
		{
			continue;
		}

		int di2 = di*di;

		for (int dj = -r; dj <= r; ++dj)
		{
			int j = ix + dj;

			if (j < 0 || j >= w)
			{
				continue;
			}

			int dj2 = dj*dj;

			double rr = std::sqrt(di2 + dj2);

			if (rr > r) // ȷ��Բ������
			{
				continue;
			}

			int I = img.at<uchar>(i, j);

			m00 += I;	// m00 = sum(I)
			m10 += /*j*/dj*I; // m10 = sum(xI)
			m01 += /*i*/di*I; // m01 = sum(yI)
		}
	}

	// �����ڵ�ͼ��Ҷ�ȫ�ڣ���ȫΪ 0�����п��ܳ��� m00 ��Ϊ 0 �����
	if (m00 == 0)
	{
		return false;
	}

	double m00_1 = 1.0 / m00;

	xIC = m00_1*m10;
	yIC = m00_1*m01;

	return true;
}

// 20200824������һԲ��֧�������ڵ�ͼ��Ҷ����ģ�ͨ�������������������ƫ�Ƶķ�ʽ��ʾ
bool DeepVoid::IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
											    int ix, int iy,					// input: the center of the region
											    int r,							// input: the radius of the circular region
											    double & dxIC, double & dyIC	// output:the location of the calculated intensity centroid (in terms of offsets)
											    )
{
	int w = img.cols;
	int h = img.rows;

	int m00 = 0;
	int m10 = 0;
	int m01 = 0;

	for (int di = -r; di <= r; ++di)
	{
		int i = iy + di;

		if (i < 0 || i >= h)
		{
			continue;
		}

		int di2 = di*di;

		for (int dj = -r; dj <= r; ++dj)
		{
			int j = ix + dj;

			if (j < 0 || j >= w)
			{
				continue;
			}

			int dj2 = dj*dj;

			double rr = std::sqrt(di2 + dj2);

			if (rr > r) // ȷ��Բ������
			{
				continue;
			}

			int I = img.at<uchar>(i, j);

			m00 += I;		// m00 = sum(I)
			m10 += dj*I;	// m10 = sum(xI)
			m01 += di*I;	// m01 = sum(yI)
		}
	}

	// �����ڵ�ͼ��Ҷ�ȫ�ڣ���ȫΪ 0�����п��ܳ��� m00 ��Ϊ 0 �����
	if (m00 == 0)
	{
		return false;
	}

	double m00_1 = 1.0 / m00;

	dxIC = m00_1*m10;
	dyIC = m00_1*m01;

	return true;
}

// 20200825��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
bool DeepVoid::CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
							  int ix, int iy,		// input: the center of the region
							  int r,				// input: the radius of the circular region
							  double & angle		// output:the location of the calculated intensity centroid (in terms of offsets)
							  )
{
	double dx, dy;

	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy)) // ˵��ͼ�������ڻҶ�ֵȫΪ 0����ȫ��
	{
		return false;
	}

	double radian = std::atan2(dy, dx); // [-��; +��]

	// ������������������
// 	if (isnan(radian) || isinf(radian))
// 	{
// 		return false;
// 	}
	
	angle = radian*R2D;

	return true;
}

// 20200825��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
bool DeepVoid::CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
							  double x, double y,	// input: the center of the region
							  int r,				// input: the radius of the circular region
							  double & angle		// output:the location of the calculated intensity centroid (in terms of offsets)
							  )
{
	// ȷ����ǰ������ڵľ�����������
	int ix = (int)x;
	int iy = (int)y;

	return CornerAngle_IC(img, ix, iy, r, angle);
}

// 20200901��ͨ������һԲ��֧��������ͼ��Ҷ�����ƫ�����ķ�ʽ����ýǵ������ķ���
// ����������뾶�ɿ��Կɿ�ȷ����������Ƕ�ʱ�������С��ȷ��
// �Ƕȷ�ΧΪ[0,360)
bool DeepVoid::FeatureRadiusAngle(const cv::Mat & img,			// input: the input gray scale image
								  int ix, int iy,				// input: the center of the region
								  int & r,						// output:the estimated radius of the circular region
								  double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
								  double threshOffset /*= 1.0*/,// input: ���Ҷ�����ƫ�����ܳ�������ֵ������ʱ����Ϊ�ɿ�ȷ�����������ɴ˶��������߶ȴ�С
								  int r_max /*= 100*/			// input: ������������뾶ֵ
								  )
{
	double thd2 = threshOffset*threshOffset;

	r = 2; // ��һ����ʼ������뾶

	double dx, dy;

	// ֱ������һ�Σ���������ƫ���������⿴���������Ƿ�Ϊȫ�ڣ����ȫ��Ҳû��Ҫ���к����ļ�����
	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy)) // ˵��ͼ�������ڻҶ�ֵȫΪ 0����ȫ��
	{
		return false;
	}

	double d2 = dx*dx + dy*dy;

	while (d2 < thd2)
	{
		++r;

		if (r > r_max)
		{
			return false;
			/*break;*/
		}

		IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy);

		d2 = dx*dx + dy*dy;
	}

	double radian = std::atan2(dy, dx); // [-��; +��]

	angle = radian*R2D;

	if (angle < 0) // ȷ�����յĽǶȷ�Χ���� opencv keypoint::angle ��ȡֵ��Χ���� [0,360)
	{
		angle += 360;
	}

	return true;
}

bool DeepVoid::FeatureRadiusAngle(const cv::Mat & img,			// input: the input gray scale image
								  double x, double y,			// input: the center of the region
								  int & r,						// output:the estimated radius of the circular region
								  double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
								  double threshOffset /*= 1.0*/,// input: ���Ҷ�����ƫ�����ܳ�������ֵ������ʱ����Ϊ�ɿ�ȷ�����������ɴ˶��������߶ȴ�С
								  int r_max /*= 100*/			// input: ������������뾶ֵ
								  )
{
	// ȷ����ǰ������ڵľ�����������
	int ix = (int)x;
	int iy = (int)y;

	return FeatureRadiusAngle(img, ix, iy, r, angle, threshOffset, r_max);
}

// CvMat wrapper here : Implementation of class CMatrix ////////////////////////////////////////////////////////
DeepVoid::CMatrix::CMatrix()
{
	m_pMat = cvCreateMat(1, 1, CV_64FC1);
	m_nRow = m_nCol = 1;
	m_pMat->data.db[0] = 0;
}

DeepVoid::CMatrix::CMatrix(int nRow, int nColumn, double initVal/* = 0*/)
{
	m_pMat = cvCreateMat(nRow, nColumn, CV_64FC1);
	m_nRow = nRow;
	m_nCol = nColumn;

	int i,j;
	for (i = 0; i < m_nRow; i++)
	{
		for (j = 0; j < m_nCol; j++)
		{
			m_pMat->data.db[i * m_nCol + j] = initVal;
		}
	}
}

DeepVoid::CMatrix::CMatrix(CvMat * pCvMat)
{
	m_pMat = pCvMat;
	if (m_pMat)
	{
		m_nRow = pCvMat->rows;
		m_nCol = pCvMat->cols;
	} 
	else
	{
		m_nRow = m_nCol = 0;
	}	
}

DeepVoid::CMatrix::CMatrix(const CMatrix & mat)
{
	m_pMat = NULL;
	m_nRow = m_nCol = 0;

	*this = mat;
}

// CMatrix::CMatrix(int nRow, int nColumn, double * pInitArray)
// {
// 	m_pMat = &cvMat(nRow, nColumn, CV_64FC1, pInitArray);
// }

DeepVoid::CMatrix::~CMatrix()
{
	Release();
}

double & DeepVoid::CMatrix::operator () (int i, int j) const
{
	return m_pMat->data.db[(i - 1) * m_pMat->cols + (j - 1)];
}

double & DeepVoid::CMatrix::operator () (int i) const
{
	if (IsColVec())
	{
		return m_pMat->data.db[(i - 1) * m_pMat->cols];
	} 
	else
	{
		return m_pMat->data.db[i - 1];
	}
}

CMatrix & DeepVoid::CMatrix::operator = (const CMatrix & mSource)
{
	if (this == &mSource)
	{
		return *this;
	} 
	else
	{
		Release();
		m_pMat = mSource.m_pMat;

		if (m_pMat)
		{
			++m_pMat->hdr_refcount;
			m_nRow = mSource.m_pMat->rows;
			m_nCol = mSource.m_pMat->cols;	
		} 
		else
		{
			m_nRow = 0;
			m_nCol = 0;
		}

		return *this;
	}
}

CMatrix & DeepVoid::CMatrix::operator = (double val)
{
	int i,j;
	for (i = 0; i < m_nRow; i++)
	{
		for (j = 0; j < m_nCol; j++)
		{
			m_pMat->data.db[i * m_nCol + j] = val;
		}
	}

	return *this;
}

CMatrix DeepVoid::CMatrix::operator + (const CMatrix & mAdd) const
{
	if ((m_nRow != mAdd.m_nRow) || (m_nCol != mAdd.m_nCol))
	{
		AfxMessageBox("����ӷ���������������С��һ�£�");
		return CMatrix(0);
	} 
	else
	{
		CMatrix mResult(m_nRow, m_nCol);
		cvAdd(m_pMat, mAdd.m_pMat, mResult.m_pMat);
		return mResult;
	}
}

CMatrix DeepVoid::CMatrix::operator + (double val) const
{
	CvScalar scaVal = cvScalar(val);
	CMatrix mResult(m_nRow, m_nCol);
	cvAddS(m_pMat, scaVal, mResult.m_pMat);
	return mResult;
}

CMatrix DeepVoid::operator + (double val, const CMatrix & mAdd)
{
	return mAdd + val;
}

CMatrix DeepVoid::CMatrix::operator - (const CMatrix & mSub) const
{
	if ((m_nRow != mSub.m_nRow) || (m_nCol != mSub.m_nCol))
	{
		AfxMessageBox("���������������������С��һ�£�");
		return CMatrix(0);
	} 
	else
	{
		CMatrix mResult(m_nRow, m_nCol);
		cvSub(m_pMat, mSub.m_pMat, mResult.m_pMat);
		return mResult;
	}
}

CMatrix DeepVoid::CMatrix::operator - (double val) const
{
	CvScalar scaVal = cvScalar(val);
	CMatrix mResult(m_nRow, m_nCol);
	cvSubS(m_pMat, scaVal, mResult.m_pMat);
	return mResult;
}

CMatrix DeepVoid::CMatrix::operator - () const
{
	return 0 - (*this);
}

CMatrix DeepVoid::operator - (double val, const CMatrix & mSub)
{
	CvScalar scaVal = cvScalar(val);
	CMatrix mResult(mSub.m_nRow, mSub.m_nCol);
	cvSubRS(mSub.m_pMat, scaVal, mResult.m_pMat);
	return mResult;
}

CMatrix DeepVoid::CMatrix::operator * (const CMatrix & mMul) const
{
	if (m_nCol != mMul.m_nRow)
	{
		AfxMessageBox("����˷���������������С��ƥ�䣡");
		return CMatrix(0);
	} 
	else
	{
		CMatrix mResult(m_nRow, mMul.m_nCol);
		cvGEMM(m_pMat, mMul.m_pMat, 1, NULL, 0, mResult.m_pMat, 0);
		return mResult;
	}
}

CMatrix DeepVoid::CMatrix::operator * (double val) const
{
	CMatrix mResult(m_nRow, m_nCol);
	CMatrix mTmp(m_nRow, m_nCol, 1);
	cvMul(m_pMat, mTmp.m_pMat, mResult.m_pMat, val);
	return mResult;
}

CMatrix DeepVoid::operator * (double val, const CMatrix & mMul)
{
	return mMul * val;
}

CMatrix DeepVoid::CMatrix::operator / (const CMatrix & mDiv) const
{
	return Div(*this, mDiv);
}

CMatrix DeepVoid::CMatrix::operator / (double val) const
{
	CMatrix mResult(m_nRow, m_nCol);
	CMatrix mTmp(m_nRow, m_nCol, val);
	cvDiv(m_pMat, mTmp.m_pMat, mResult.m_pMat);
	return mResult;
}

CMatrix DeepVoid::operator / (double val, const CMatrix & mDiv)
{
	CMatrix mResult(mDiv.m_nRow, mDiv.m_nCol);
	cvDiv(NULL, mDiv.m_pMat, mResult.m_pMat, val);
	return mResult;
}

void DeepVoid::CMatrix::operator += (const CMatrix & mAdd)
{
	*this = *this + mAdd;
}

void DeepVoid::CMatrix::operator += (double val)
{
	*this = *this + val;
}

void DeepVoid::CMatrix::operator -= (const CMatrix & mSub)
{
	*this = *this - mSub;
}

void DeepVoid::CMatrix::operator -= (double val)
{
	*this = *this - val;
}

void DeepVoid::CMatrix::operator *= (const CMatrix & mMul)
{
	*this = Mul(*this, mMul);
}

void DeepVoid::CMatrix::operator *= (double val)
{
	*this = *this * val;
}

void DeepVoid::CMatrix::operator /= (const CMatrix & mDiv)
{
	*this = Div(*this, mDiv);
}

void DeepVoid::CMatrix::operator /= (double val)
{
	*this = *this / val;
}

void DeepVoid::CMatrix::Release(void)
{
	if (m_pMat)
	{
		if (--m_pMat->hdr_refcount == 0)
		{
			cvReleaseMat(&m_pMat);
		}
		m_pMat = NULL;
		m_nRow = m_nCol = 0;
	}
}

BOOL DeepVoid::CMatrix::IsNull(void) const
{
	if (NULL == m_pMat)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

BOOL DeepVoid::CMatrix::IsZero(void) const
{
	int i,j;

	for (i = 1; i < m_nRow + 1; i++)
	{
		for (j = 1; j < m_nCol + 1; j++)
		{
			if ((*this)(i, j) != 0.0)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL DeepVoid::CMatrix::IsVector(void) const
{
	if ((m_nRow == 1 && m_nCol > 1) || (m_nCol == 1 && m_nRow > 1))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL DeepVoid::CMatrix::IsRowVec(void) const
{
	if (m_nRow == 1 && m_nCol > 1)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

BOOL DeepVoid::CMatrix::IsColVec(void) const
{
	if (m_nCol == 1 && m_nRow > 1)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

BOOL DeepVoid::CMatrix::IsSquare(void) const
{
	if (m_nRow != m_nCol)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL DeepVoid::CMatrix::IsSymmetric(void) const
{
	if (!IsSquare())
	{
		return FALSE;
	}

	int i,j;

	for (i = 1; i < m_nRow; i++)
	{
		for (j = i + 1; j < m_nCol + 1; j++)
		{
			if ((*this)(i, j) != (*this)(j, i))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL DeepVoid::CMatrix::IsAntiSymm(void) const
{
	if (!IsSquare())
	{
		return FALSE;
	}

	int i,j;

	for (i = 1; i < m_nRow; i++)
	{
		for (j = i + 1; j < m_nCol + 1; j++)
		{
			if ((*this)(i, j) != -(*this)(j, i))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

// ��һ����Ҫ�Ƿ���
BOOL DeepVoid::CMatrix::IsDiagonal(void) const
{
	int i,j;

	for (i = 1; i < m_nRow + 1; i++)
	{
		for (j = 1; j < m_nCol + 1; j++)
		{
			if ((i != j) && ((*this)(i, j) != 0.0))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL DeepVoid::CMatrix::IsIdentity(void) const
{
	if (!IsSquare())
	{
		return FALSE;
	}

	if (!IsDiagonal())
	{
		return FALSE;
	}

	int i;

	for (i = 1; i < m_nRow + 1; i++)
	{
		if ((*this)(i, i) != 1.0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL DeepVoid::CMatrix::IsSymmPosDef(void) const
{
	// ����Cholesky�ֽ⣬���ɹ�CMatrix��m_pMat��Ϊ�գ��������Symmetric positive definite������m_pMatΪ�գ�˵��������Symmetric positive definite
	CMatrix mTmp = Chol_Lower();

	if (NULL == mTmp.m_pMat)
	{
		return FALSE;
	}

	return TRUE;
}

CMatrix DeepVoid::CMatrix::Clone(void) const
{
	if (m_pMat)
	{
		return CMatrix(cvCloneMat(m_pMat));
	} 
	else
	{
		return CMatrix(0);
	}
}

CMatrix DeepVoid::CMatrix::GetRow(int i) const
{
	if (i < 1 || i > m_nRow)
	{
		AfxMessageBox("����������Խ�磡");
		return CMatrix(0);
	} 
	else
	{
		CvMat * pMatTemp = cvCreateMatHeader(1, m_nCol, CV_64FC1);
		cvGetRow(m_pMat, pMatTemp, i - 1);
		CvMat * pClonedRow = cvCloneMat(pMatTemp);
		cvReleaseMat(&pMatTemp);
		return CMatrix(pClonedRow);
	}
}

void DeepVoid::CMatrix::SetRow(int i, const CMatrix & mSrc)
{
	if (m_nCol != mSrc.m_nCol)
	{
		AfxMessageBox("����Դ�о����뱾����ߴ粻ƥ�䣡");
	} 
	else
	{
		CvMat * pMatTemp = cvCreateMatHeader(1, m_nCol, CV_64FC1);
		cvGetRow(m_pMat, pMatTemp, i - 1);
		cvCopy(mSrc.m_pMat, pMatTemp);
		cvReleaseMat(&pMatTemp);
	}
}

CMatrix DeepVoid::CMatrix::GetCol(int i) const
{
	if (i < 1 || i > m_nCol)
	{
		AfxMessageBox("����������Խ�磡");
		return CMatrix(0);
	} 
	else
	{
		CvMat * pMatTemp = cvCreateMatHeader(m_nRow, 1, CV_64FC1);
		cvGetCol(m_pMat, pMatTemp, i - 1);
		CvMat * pClonedCol = cvCloneMat(pMatTemp);
		cvReleaseMat(&pMatTemp);
		return CMatrix(pClonedCol);
	}
}

void DeepVoid::CMatrix::SetCol(int i, const CMatrix & mSrc)
{
	if (m_nRow != mSrc.m_nRow)
	{
		AfxMessageBox("����Դ�о����뱾����ߴ粻ƥ�䣡");
	} 
	else
	{
		CvMat * pMatTemp = cvCreateMatHeader(m_nRow, 1, CV_64FC1);
		cvGetCol(m_pMat, pMatTemp, i - 1);
		cvCopy(mSrc.m_pMat, pMatTemp);
		cvReleaseMat(&pMatTemp);
	}
}

CMatrix DeepVoid::CMatrix::GetRect(int rowTL, int colTL, int width, int height) const
{
	if ((rowTL < 1) || (rowTL > m_nRow) || (width < 1) || (colTL + width - 1 > m_nCol) || (colTL < 1) || (colTL > m_nCol) || (height < 1) || (rowTL + height - 1 > m_nRow))
	{
		AfxMessageBox("������ο�Խ�磡");
		return CMatrix(0);
	} 
	else
	{
		CvRect rect;
		rect.x = colTL - 1;
		rect.y = rowTL - 1;
		rect.width = width;
		rect.height = height;

		CvMat * pMatTemp = cvCreateMatHeader(height, width, CV_64FC1);
		cvGetSubRect(m_pMat, pMatTemp, rect);
		CvMat * pClonedRect = cvCloneMat(pMatTemp);
		cvReleaseMat(&pMatTemp);
		return CMatrix(pClonedRect);
	}
}

void DeepVoid::CMatrix::SetRect(int rowTL, int colTL, const CMatrix & mSrc)
{
	if ((rowTL < 1) || (rowTL > m_nRow) || (mSrc.m_nCol < 1) || (colTL + mSrc.m_nCol - 1 > m_nCol) || (colTL < 1) || (colTL > m_nCol) || (mSrc.m_nRow < 1) || (rowTL + mSrc.m_nRow - 1 > m_nRow))
	{
		AfxMessageBox("����Դ������ο�Խ�磡");
	} 
	else
	{
		CvRect rect;
		rect.x = colTL - 1;
		rect.y = rowTL - 1;
		rect.width = mSrc.m_nCol;
		rect.height = mSrc.m_nRow;

		CvMat * pMatTemp = cvCreateMatHeader(rect.height, rect.width, CV_64FC1);
		cvGetSubRect(m_pMat, pMatTemp, rect);
		cvCopy(mSrc.m_pMat, pMatTemp);
		cvReleaseMat(&pMatTemp);
	}
}

CMatrix DeepVoid::CMatrix::GetDiag(void) const
{
	int r = m_nRow > m_nCol ? m_nCol : m_nRow;

	CMatrix mDiag(r, 1);

	for (int i = 1; i < r + 1; i++)
	{
		mDiag(i, 1) = (*this)(i, i);
	}

	return mDiag;
}

void DeepVoid::CMatrix::AddOneRow(double initVal /* = 0 */)
{
	CMatrix mNew(m_nRow + 1, m_nCol, initVal);

	mNew.SetRect(1, 1, *this);

	*this = mNew;
}

void DeepVoid::CMatrix::AddOneCol(double initVal /* = 0 */)
{
	CMatrix mNew(m_nRow, m_nCol + 1, initVal);

	mNew.SetRect(1, 1, *this);

	*this = mNew;
}

void DeepVoid::CMatrix::Flip(FlipMode flipMode)
{
	switch (flipMode)
	{
	case FLIPMODE_X:
		cvFlip(m_pMat, NULL, 0);
		break;
	case FLIPMODE_Y:
		cvFlip(m_pMat, NULL, 1);
		break;
	case FLIPMODE_XY:
		cvFlip(m_pMat, NULL, -1);
		break;
	default:
		break;
	}
}

CMatrix DeepVoid::CMatrix::Transpose(void) const
{
	CMatrix mTrsped(m_nCol, m_nRow);
	cvTranspose(m_pMat, mTrsped.m_pMat);
	/**this = mTrsped;*/
	return mTrsped;
}

CMatrix DeepVoid::CMatrix::Inverse(void) const
{
	double invCond;
	CMatrix mInv(m_nCol, m_nRow);

	if (IsSquare())
	{
		// �����ж��Ƿ��ǶԳ��������󣬶Գ���������һ������
		if (IsSymmPosDef())
		{
			invCond = cvInvert(m_pMat, mInv.m_pMat, CV_SVD_SYM);
			return mInv;
		}

		// �����ǶԳ��������󣬶��ǶԳƷ������������Ǹ�һ��ķǶԳƷ���ʱ�����ж��Ƿ���棬�����棬�����ȫѡ��ԪGaussian-Jordan��([A|E] -> [E|inv(A)])������
		// �������棬����������SVD����������pseudo-inverse�����еķǷ���ֻ��pseudo-inverse
		if (cvInvert(m_pMat, mInv.m_pMat, CV_LU) != 0)
		{
			return mInv;
		}
	} 

	// �������ǷǷ��󣬻����ǳ��Գ���������֮��ķ����в�����ķ���ʱ������SVD��������pseudo-inverse
	invCond = cvInvert(m_pMat, mInv.m_pMat, CV_SVD);
	return mInv;
}

CMatrix DeepVoid::CMatrix::Pow(double power) const
{
	CMatrix mTmp(m_nRow, m_nCol);

	double intTmp = cvRound(power);
	double residue = power - intTmp;

	if (residue != 0.0)
	{
		CvMat * pMask = cvCreateMat(m_nRow, m_nCol, CV_8UC1);

		cvCmpS(m_pMat, 0, pMask, CV_CMP_LT);

		cvPow(m_pMat, mTmp.m_pMat, power);

		cvSubRS(mTmp.m_pMat, cvScalarAll(0), mTmp.m_pMat, pMask);

		cvReleaseMat(&pMask);
	} 
	else
	{
		cvPow(m_pMat, mTmp.m_pMat, power);
	}

	return mTmp;
}

CMatrix DeepVoid::CMatrix::Log(void) const
{
	CMatrix mTmp(m_nRow, m_nCol);

	cvLog(m_pMat, mTmp.m_pMat);

	return mTmp;
}

CMatrix DeepVoid::CMatrix::Max(void) const
{
	CMatrix mReturned(2, 1);

	double maxVal = (*this)(1, 1);
	int i, j;
	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			if ((*this)(i, j) > maxVal)
			{
				maxVal = (*this)(i, j);
			}
		}
	}

	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			if ((*this)(i, j) == maxVal)
			{
				mReturned.AddOneCol();
				mReturned(1, mReturned.m_nCol) = i;
				mReturned(2, mReturned.m_nCol) = j;
			}
		}
	}

	mReturned = mReturned.GetRect(1, 2, mReturned.m_nCol - 1, mReturned.m_nRow);
	mReturned.AddOneRow(maxVal);

	return mReturned;
}

CMatrix DeepVoid::CMatrix::Min(void) const
{
	CMatrix mReturned(2, 1);

	double minVal = (*this)(1, 1);
	int i, j;
	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			if ((*this)(i, j) < minVal)
			{
				minVal = (*this)(i, j);
			}
		}
	}

	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			if ((*this)(i, j) == minVal)
			{
				mReturned.AddOneCol();
				mReturned(1, mReturned.m_nCol) = i;
				mReturned(2, mReturned.m_nCol) = j;
			}
		}
	}

	mReturned = mReturned.GetRect(1, 2, mReturned.m_nCol - 1, mReturned.m_nRow);
	mReturned.AddOneRow(minVal);

	return mReturned;
}

CMatrix DeepVoid::CMatrix::Find(double val, double eps /*= 1.0E-10*/) const
{
	CMatrix mIndex(2, 1), mTmp(2, 1);

	int i,j;
	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			if (fabs((*this)(i, j) - val) < eps) 
			{
				mTmp(1) = i;
				mTmp(2) = j;

				mIndex.Mosaic(mTmp, MOSAICMODE_RIGHT);
			}
		}
	}

	if (mIndex.m_nCol == 1)
	{
		return CMatrix(0);
	} 
	else
	{
		return mIndex.GetRect(1, 2, mIndex.m_nCol - 1, mIndex.m_nRow);
	}
}

CMatrix DeepVoid::CMatrix::Abs(void) const
{
	CMatrix mAbs(m_nRow, m_nCol);

	cvAbs(m_pMat, mAbs.m_pMat);

	return mAbs;
}

CMatrix DeepVoid::CMatrix::Atand(void) const
{
	CMatrix mAngle(m_nRow, m_nCol);

	double a, b;

	int i,j;

	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			a = (*this)(i, j); // ����������ǲ���ֱ���� (i, j)��ֻ�� (*this)(i, j)
			b = atand(a);
			mAngle(i, j) = b;
		}
	}

	return mAngle;
}

CMatrix DeepVoid::Solve(const CMatrix & mA, const CMatrix & mB)
{
	// �ж�ϵ������mA�ͷ����ұ���mB�������Ƿ�һ�£���һ�¾ͷ��ؿվ���
	if (mA.m_nRow != mB.m_nRow)
	{
		AfxMessageBox("Row number of A and B don't match when solving Ax = B !");
		return CMatrix(0);
	}

	int i;
	int m = mA.m_nRow;
	int n = mA.m_nCol;

	CMatrix mX(mA.m_nCol, mB.m_nCol);

	// �ж�������η�����Ax=0���Ƿ���η�����Ax=B
	if (!mB.IsZero())
	{
		// ���ж��ǲ��ǶԳ��������ǵĻ�������ԶԳ�������Ľⷨ�ⷽ�̣����ǵĻ��������ʧ�ܾͼ�������LU��
		if (mA.IsSymmPosDef())
		{
			if (1 == cvSolve(mA.m_pMat, mB.m_pMat, mX.m_pMat, CV_SVD_SYM))
			{
				return mX;
			}
		}

		// ��LU�����ⷽ�̵Ļ���˼���ǣ��Ȼ�A=LU��LUx=B�����Ⱥ�����forward substitution��back substitution��y=L\B��x=U\y
		// LU����ֻ��A�����ȵ�ʱ�����Ч�����в�����ʱ�᷵��0ֵ�����ʧ�ܡ�����OpenCV��LU�ⷽ�̺���ֻ����m>=n��A����m<n����ֱ�ӳ���
		// �������޾�ȷ��ʱ����LU�����ܵõ���С���˽⣬�Һ�ʱ��

		// 2011.04.14���Σ�ԭ����CV_LU����������ȷ��ǰ�����A�����ȣ�����ͨ�����Է��֣�OpenCV��LU���������ܺܺõ��жϾ���A�Ƿ��ȣ�����
		// һ�����Ե�6��4����Ϊ3�ľ��󣬴˺���δ�жϳ��併�Ȼ��������沽�裬������������x�����Ǵ���ģ�ֻ�е�����һ����3��ȫΪ0������3��
		// ����ʱ�˺������жϳ��併�ȣ��Ӷ����������ڶԾ��ȵĸ�Ҫ���ش����δ˺�����ͳһֻ��SVD����ⷽ����
		// 		if (m >= n)
		// 		{
		// 			if (1 == cvSolve(mA.m_pMat, mB.m_pMat, mX.m_pMat, CV_LU))
		// 			{
		// 				return mX;
		// 			}
		// 		}

		// ��ǰ��������ʧЧ������²���SVD�ķ�����ⷽ�̣��ھ���A�������ȵ�����£�Ҫ�Ƿ��̴��ھ�ȷ��˷�����||x||��С�ľ�ȷ��
		// Ҫ�Ƿ��̲����ھ�ȷ��˷�����||x||��С����С���˽⣻A����������£������̴��ھ�ȷ�⣬��˷�����Ψһ�ľ�ȷ�⣬�����̲�����
		// ��ȷ�⣬�˷�����Ψһ����С���˽�
		if (1 == cvSolve(mA.m_pMat, mB.m_pMat, mX.m_pMat, CV_SVD))
		{
			return mX;
		} 
		else
		{
			return CMatrix(0);
		}
	} 

	// ������Ax=0������SVD��A=UDV'��ʹ||Ax||��С������||x||=1�Ľ�x����V�����һ��
	else
	{
		CMatrix mDiag(m, n);
		CMatrix mV(n, n);

		cvSVD(mA.m_pMat, mDiag.m_pMat, NULL, mV.m_pMat);

		for (i = 1; i < mX.m_nCol + 1; i++)
		{
			mX.SetCol(i, mV.GetCol(n));
		}

		return mX;
	}
}

CMatrix DeepVoid::Cross(const CMatrix & mVec1, const CMatrix & mVec2)
{
	if (!mVec1.IsVector() || !mVec2.IsVector())
	{
		return CMatrix(0);
	}

	if ((mVec1.m_nRow != 3 && mVec1.m_nCol != 3) || (mVec2.m_nRow != 3 && mVec2.m_nCol != 3))
	{
		return CMatrix(0);
	}

	CMatrix mTmp(3, 1);

	if (mVec1.IsColVec() && mVec2.IsColVec())
	{
		// cvCrossProduct�������ֻ������������Ϊ���룬���ص�Ҳ��������
		cvCrossProduct(mVec1.m_pMat, mVec2.m_pMat, mTmp.m_pMat);
	}
	else if (mVec1.IsRowVec() && mVec2.IsColVec())
	{
		cvCrossProduct(mVec1.Transpose().m_pMat, mVec2.m_pMat, mTmp.m_pMat);
	}
	else if (mVec1.IsColVec() && mVec2.IsRowVec())
	{
		cvCrossProduct(mVec1.m_pMat, mVec2.Transpose().m_pMat, mTmp.m_pMat);
	} 
	else
	{
		cvCrossProduct(mVec1.Transpose().m_pMat, mVec2.Transpose().m_pMat, mTmp.m_pMat);
	}

	return mTmp;
}

// ��3��3�ľ������RQ�ֽ⣺A = RQ������RΪ�Խ���Ԫ��ȫΪ���������Ǿ���Q��Ϊ��������
void DeepVoid::RQ3x3(const CMatrix & mSrc, CMatrix & mR, CMatrix & mQ)
{
	if (mSrc.m_nRow != 3 || mSrc.m_nCol != 3)
	{
		mR = CMatrix(0);
		mQ = CMatrix(0);
		return;
	}

	mR = CMatrix(3, 3);
	mQ = CMatrix(3, 3);

	cvRQDecomp3x3(mSrc.m_pMat, mR.m_pMat, mQ.m_pMat);
}

// �������ڡ�Matrix Computations 3rd Edition��һ��P.210�е�Algorithm 5.1.1 (Householder Vector)
// Given x��Rn��this function computes v��Rn with v(1) = 1 and �¡�R such that P = In - ��vv' is orthogonal and Px = |x|e1
// Ҫ������mXΪ������
void DeepVoid::House(const CMatrix & mX, CMatrix & mVec, double & beta)
{
	int n = mX.m_nRow;

	double sigma;
	if (n != 1)
	{
		CMatrix mSigma = mX.GetRect(2, 1, 1, n - 1).Transpose() * mX.GetRect(2, 1, 1, n - 1);
		sigma = mSigma(1);

		mVec = CMatrix(n, 1);
		mVec(1) = 1;
		mVec.SetRect(2, 1, mX.GetRect(2, 1, 1, n - 1));
	} 
	else
	{
		sigma = 0;

		mVec = CMatrix(n, 1);
		mVec(1) = 1;
	}


	if (fabs(sigma) < EPSILON_ZHU)
	{
		beta = 0;
	} 
	else
	{
		double x1 = mX(1);
		double miu = sqrt(x1 * x1 + sigma);

		if (x1 <= 0.0)
		{
			mVec(1) = x1 - miu;
		} 
		else
		{
			mVec(1) = - sigma / (x1 + miu);
		}

		double v1 = mVec(1);

		beta = 2 * v1 * v1 / (sigma + v1 * v1);

		mVec = mVec / v1;
	}
}

// �������ڡ�Matrix Computations 3rd Edition��һ��P.216�е�Algorithm 5.1.3 (Computes Givens c and s)
// ���������[a; b]'����ʹ��[c s; -s c]' * [a; b]' = [r; 0]'������cos��sin
void DeepVoid::Givens(double a, double b, double & c, double & s)
{
	double lbd;

	if (fabs(b) < EPSILON_ZHU)
	{
		c = 1;
		s = 0;
	} 
	else
	{
		if (fabs(b) > fabs(a))
		{
			lbd = -a / b;
			s = 1 / (sqrt(1 + lbd * lbd));
			c = s * lbd;
		} 
		else
		{
			lbd = -b / a;
			c = 1 / (sqrt(1 + lbd * lbd));
			s = c * lbd;
		}
	}
}

// ����n��n�ĵ�λ��
CMatrix DeepVoid::GenI(int n)
{
	CMatrix mI(n, n);

	int i;
	for (i = 1; i <= n; i++)
	{
		mI(i, i) = 1;
	}

	return mI;
}

// �����������ά����v�������Ӧ�Ĳ�˾���[v]
CMatrix DeepVoid::GenCrossMat(const CMatrix & mVec)
{
	if (!mVec.IsVector())
	{
		return CMatrix(0);
	}

	if (mVec.m_nRow != 3 && mVec.m_nCol != 3)
	{
		return CMatrix(0);
	}

	CMatrix mCross(3, 3);

	double v1 = mVec(1);
	double v2 = mVec(2);
	double v3 = mVec(3);

	mCross(1, 2) = -v3; mCross(1, 3) =  v2;
	mCross(2, 1) =  v3; mCross(2, 3) = -v1;
	mCross(3, 1) = -v2; mCross(3, 2) =  v1;

	return mCross;
}

void DeepVoid::CMatrix::Mosaic(const CMatrix & mOther, MosaicMode mode /* = MOSAICMODE_DOWN */)
{
	switch (mode)
	{
	case MOSAICMODE_DOWN:
		if (m_nCol != mOther.m_nCol)
		{
			AfxMessageBox("��ƴ�ӵľ��������뱾����ƥ�䣡");
		} 
		else
		{
			CMatrix mExted(m_nRow + mOther.m_nRow, m_nCol);
			mExted.SetRect(1, 1, *this);
			mExted.SetRect(m_nRow + 1, 1, mOther);
			*this = mExted;
		}
		break;

	case MOSAICMODE_UP:
		if (m_nCol != mOther.m_nCol)
		{
			AfxMessageBox("��ƴ�ӵľ��������뱾����ƥ�䣡");
		} 
		else
		{
			CMatrix mExted(m_nRow + mOther.m_nRow, m_nCol);
			mExted.SetRect(1, 1, mOther);
			mExted.SetRect(mOther.m_nRow + 1, 1, *this);
			*this = mExted;
		}
		break;

	case MOSAICMODE_RIGHT:
		if (m_nRow != mOther.m_nRow)
		{
			AfxMessageBox("��ƴ�ӵľ��������뱾����ƥ�䣡");
		} 
		else
		{
			CMatrix mExted(m_nRow, m_nCol + mOther.m_nCol);
			mExted.SetRect(1, 1, *this);
			mExted.SetRect(1, m_nCol + 1, mOther);
			*this = mExted;
		}
		break;

	case MOSAICMODE_LEFT:
		if (m_nRow != mOther.m_nRow)
		{
			AfxMessageBox("��ƴ�ӵľ��������뱾����ƥ�䣡");
		} 
		else
		{
			CMatrix mExted(m_nRow, m_nCol + mOther.m_nCol);
			mExted.SetRect(1, 1, mOther);
			mExted.SetRect(1, mOther.m_nCol + 1, *this);
			*this = mExted;
		}
		break;

	default:
		break;
	}
}

void DeepVoid::CMatrix::Reshape(int newRow, int newCol)
{
	if (newRow * newCol != m_nRow * m_nCol)
	{
		AfxMessageBox("Reshape failed!");
		return;
	}

	m_pMat->cols = m_pMat->width  = newCol;
	m_pMat->rows = m_pMat->height = newRow;
	m_pMat->step = m_pMat->step * newCol / m_nCol;
	m_nRow = newRow;
	m_nCol = newCol;
}

void DeepVoid::CMatrix::VectorizeByRow(void)
{
	Reshape(m_nRow * m_nCol, 1);
}

void DeepVoid::CMatrix::VectorizeByCol(void)
{
	*this = Transpose();

	Reshape(m_nRow * m_nCol, 1);
}

// CMatrix CMatrix::GetRect(CRect rect)
// {
// 	return GetRect(rect.TopLeft().y, rect.TopLeft().x, rect.Width(), rect.Height());
// }

double DeepVoid::CMatrix::Det(void) const
{
	return cvDet(m_pMat);
}

double DeepVoid::CMatrix::Trace(void) const
{
	CvScalar temp = cvTrace(m_pMat);
	return temp.val[0];
}

// ����SVD�����ȣ���MATLAB����һ��
int DeepVoid::CMatrix::Rank(double eps /*= 1.0E-10*/) const
{
	// ����SVD�ֽ�õ�A=UDV'�е�D���󣬶Խ����ϵ�����ֵ����������
	CMatrix mDiag(m_nRow, m_nCol);
	cvSVD(m_pMat, mDiag.m_pMat);

	// ͳ�ƴ�����ֵeps������ֵ�ĸ�����Ϊ�������
	int r = m_nRow > m_nCol ? m_nCol : m_nRow;
	int maxMN = m_nRow > m_nCol ? m_nRow : m_nCol;
	int rank = 0;

	for (int i = 0; i < r; i++)
	{
		/*double val = mDiag(i + 1, i + 1);*/

		if (mDiag(i + 1, i + 1) > maxMN * eps)
		{
			rank++;
		}
	}

	return rank;
}

// ��������������ĵ���������С����ֵ���������ֵ�ı�ֵ
// Խ�ӽ���1Խ�ã�Խ�ӽ�0˵������Խ�ӽ�����
double DeepVoid::CMatrix::InvCond(void) const
{
	CMatrix mInv(m_nCol, m_nRow);
	return cvInvert(m_pMat, mInv.m_pMat, CV_SVD);
}

double DeepVoid::CMatrix::Mean(void) const
{
	CvScalar temp = cvAvg(m_pMat);
	return temp.val[0];
}

double DeepVoid::CMatrix::StdDev(void) const
{
	CvScalar sdv,avg;
	cvAvgSdv(m_pMat, &avg, &sdv);
	return sdv.val[0];
}

int DeepVoid::CMatrix::CountNonZero(void) const
{
	return cvCountNonZero(m_pMat);
}

double DeepVoid::CMatrix::Sum(void) const
{
	double mean = Mean();
	int num = m_nRow * m_nCol;
	return mean * num;
}

double DeepVoid::CMatrix::Norm(NormMode mode /* = NORMMODE_L2 */) const
{
	switch (mode)
	{
	case NORMMODE_C:
		return cvNorm(m_pMat, NULL, CV_C);
		break;

	case NORMMODE_L1:
		return cvNorm(m_pMat, NULL, CV_L1);
		break;

	case NORMMODE_L2:
		return cvNorm(m_pMat, NULL, CV_L2);
		break;

	default:
		break;
	}
}

int DeepVoid::CMatrix::Length(void) const
{
	if (m_nRow >= m_nCol)
	{
		return m_nRow;
	} 
	else
	{
		return m_nCol;
	}
}

double DeepVoid::CMatrix::Dev(void) const
{
	double sum = 0;

	int i,j;
	for (i = 1; i <= m_nRow; i++)
	{
		for (j = 1; j <= m_nCol; j++)
		{
			double val = (*this)(i, j);
			sum += val * val;
		}
	}

	double mean = sum / (m_nRow * m_nCol);

	return sqrt(mean);
}

CMatrix DeepVoid::Mul(const CMatrix & m1, const CMatrix & m2)
{
	if ((m1.m_nRow != m2.m_nRow) || (m1.m_nCol != m2.m_nCol))
	{
		AfxMessageBox("��������ӦԪ�������������������С��һ�£�");
		return CMatrix(0);
	} 
	else
	{
		CMatrix mResult(m1.m_nRow, m1.m_nCol);
		cvMul(m1.m_pMat, m2.m_pMat, mResult.m_pMat);
		return mResult;
	}
}

CMatrix DeepVoid::Div(const CMatrix & m1, const CMatrix & m2)
{
	if ((m2.m_nRow == 1) && (m2.m_nCol == 1))
	{
		CMatrix mTmp = RepMat(m2, m1.m_nRow, m1.m_nCol);
		CMatrix mResult(m1.m_nRow, m1.m_nCol);
		cvDiv(m1.m_pMat, mTmp.m_pMat, mResult.m_pMat);
		return mResult;
	}
	else if ((m1.m_nRow != m2.m_nRow) || (m1.m_nCol != m2.m_nCol))
	{
		AfxMessageBox("��������ӦԪ�������������������С��һ�£�");
		return CMatrix(0);
	} 
	else
	{
		CMatrix mResult(m1.m_nRow, m1.m_nCol);
		cvDiv(m1.m_pMat, m2.m_pMat, mResult.m_pMat);
		return mResult;
	}
}

CMatrix DeepVoid::ExtendRows(const CMatrix & mMat, int row)
{
	// 	if (mRow.m_nRow != 1)
	// 	{
	// 		AfxMessageBox("Row extending failed!");
	// 		return CMatrix(0);
	// 	}

	int i;

	CMatrix mExtended = mMat.Clone();

	for (i = 1; i < row; i++)
	{
		mExtended.Mosaic(mMat, MOSAICMODE_DOWN);
	}

	return mExtended;
}

CMatrix DeepVoid::ExtendCols(const CMatrix & mMat, int col)
{
	// 	if (mCol.m_nCol != 1)
	// 	{
	// 		AfxMessageBox("Column extending failed!");
	// 		return CMatrix(0);
	// 	}

	int i;

	CMatrix mExtended = mMat.Clone();

	for (i = 1; i < col; i++)
	{
		mExtended.Mosaic(mMat, MOSAICMODE_RIGHT);
	}

	return mExtended;
}

CMatrix DeepVoid::RepMat(const CMatrix & mMat, int row, int col)
{
	CMatrix mExtended = ExtendRows(mMat, row); // �Ȱ�����չ

	mExtended = ExtendCols(mExtended, col); // �ٰ�����չ

	return mExtended;
}



CvRNG rng_state = cvRNG(0xffffffff);
CMatrix DeepVoid::Rand(int row, int col, RandMode randMode, double param1, double param2)
{
	CMatrix mRand(row, col);

	switch (randMode)
	{
	case RANDMODE_UNIFORM:
		cvRandArr(&rng_state, mRand.m_pMat, CV_RAND_UNI, cvRealScalar(param1), cvRealScalar(param2));
		break;
	case RANDMODE_GAUSSIAN:
		cvRandArr(&rng_state, mRand.m_pMat, CV_RAND_NORMAL, cvRealScalar(param1), cvRealScalar(param2));
		break;
	default:
		break;
	}

	return mRand;
}

void DeepVoid::CMatrix::Write(CString filePath, WriteMode mode/* = WRITEMODE_LF*/) const
{
	FILE * pFile = fopen(filePath, "a");
	if (m_pMat)
	{
		int rows = m_pMat->rows;
		int cols = m_pMat->cols;

		int i,j;

		fprintf(pFile, "(%d, %d)\n\n", rows, cols);

		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				switch (mode)
				{
				case WRITEMODE_E:
					fprintf(pFile, "%.12e	", m_pMat->data.db[i * cols + j]);
					break;

				case WRITEMODE_LF:
					fprintf(pFile, "%lf	", m_pMat->data.db[i * cols + j]);
					break;

				default:
					break;
				}
			}
			fprintf(pFile, "\n");
		}
		fprintf(pFile, "\n\n");
	} 
	else
	{
		fprintf(pFile, "(0, 0)\n\n\n");
	}
	fclose(pFile);
}

BOOL DeepVoid::CMatrix::Read(CString filePath)
{
	CFile file;
	CFileException ex;

	if (!file.Open(filePath, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		// ���ļ���������ʾ����ԭ�򲢷���BOOL
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString tmp;
		tmp.Format("�޷��������������ļ�: %s", szError);
		AfxMessageBox(tmp);
		/*_tprintf_s(_T("�޷��������������ļ�: %1024s"), szError);*/
		return FALSE;
	} 
	else
	{
		file.Close();

		FILE * pFile = fopen(filePath, "r");

		int rows, columns; 
		fscanf(pFile, "(%d, %d)", &rows, &columns);

		CMatrix mTmp(rows, columns);
		int i, j;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < columns; j++)
			{
				fscanf(pFile, "%lf", &mTmp(i + 1, j + 1));
			}
		}
		fclose(pFile);

		*this = mTmp;
		return TRUE;
	}
}

void DeepVoid::CMatrix::PrintMatrix2Screen(CString title /* =  */, WriteMode mode /* = WRITEMODE_LF */) const
{
	if (IsNull())
	{
		if (title != "")
		{
			CString str;
			str.Format("%s\n\nNull Matrix!", title);
			AfxMessageBox(str);
		} 
		else
		{
			AfxMessageBox("Null Matrix!");
		}
	} 
	else
	{
		CString sMat, sEle;

		int i, j;
		for (i = 1; i <= m_nRow; i++)
		{
			for (j = 1; j <= m_nCol; j++)
			{
				if (mode == WRITEMODE_LF)
				{
					sEle.Format("%lf	", (*this)(i, j));
				} 
				else
				{
					sEle.Format("%.12e	", (*this)(i, j));
				}

				sMat += sEle;
			}
			sEle.Format("\n");
			sMat += sEle;
		}

		if (title != "")
		{
			sEle.Format("%s\n\n", title);
			sMat = sEle + sMat;
		}

		AfxMessageBox(sMat);	
	}
}


// ��m��n�ľ���A����QR�ֽ⣺A = QR������QΪm��m����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
// �������ڡ�Matrix Computations 3rd Edition��һ��P.224�е�Algorithm 5.2.1 (Householder QR)
// Householder QR �� Givens QR ��ͬһ������ A ���зֽ�ʱ���ܵõ���ͬ�������Ǿ��� R������һ����֤�ֽ�õ�
// ���������� Q1 �� Q2 ��ȫ��ͬ�����������Ǿ��� R Ϊ�����Ȳ��ܱ�֤ Q1 �� Q2 ��ȫ��ͬ���� R'(Q1' - Q2') = 0
// ���� (Q1' - Q2') ֻ��0�⣬���� Householder QR �ֽ�õ��� Q �� MATLAB �ֽ�Ľ��ʼ��һ�£�˵�� MATLAB ��
// QR �ֽ⺯�����õ�Ҳ�� Householder ����
void DeepVoid::CMatrix::QR_Householder(CMatrix & mR, CMatrix & mQ /* = CMatrix */) const
{
	CMatrix mA = Clone();
	CMatrix mV, mTmp, mBeta;
	double  beta;

	int n;
	if (m_nRow >= m_nCol)
	{
		n = m_nCol;
	} 
	else
	{
		n = m_nRow;
	}

	int j;
	for (j = 1; j <= n/*m_nCol*/; j++)
	{
		House(mA.GetRect(j, j, 1, m_nRow - j + 1), mV, beta);

		mBeta.AddOneRow(beta);

		mTmp = (GenI(m_nRow - j + 1) - beta * mV * mV.Transpose()) * mA.GetRect(j, j, m_nCol - j + 1, m_nRow - j + 1);

		mA.SetRect(j, j, mTmp);

		if (j < m_nRow)
		{
			mA.SetRect(j + 1, j, mV.GetRect(2, 1, 1, m_nRow - j));
		}
	}

	BOOL lastNegative = FALSE; // �����Ǿ����n�е�n�е�Ԫ���Ƿ�Ϊ����Ĭ���Ƿ�

	if (mA(n, n) < 0.0)
	{
		lastNegative = TRUE;

		mTmp = -1 * mA.GetRect(n, n, m_nCol - n + 1, 1); // ��Ϊ���������л��ţ���֤�Խ���Ԫ��ȫΪ�������ñ�־λ
		mA.SetRect(n, n, mTmp); 
	}

	mR = CMatrix(m_nRow, m_nCol);

	mBeta = mBeta.GetRect(2, 1, 1, mBeta.m_nRow - 1);

	for (j = 1; j <= n; j++)
	{
		mR.SetRect(j, j, mA.GetRect(j, j, m_nCol - j + 1, 1));
	}


	// ����õ���������Q�ż���Q������Ҫʱ�Ͳ�����
	if (!mQ.IsNull())
	{
		mQ = GenI(m_nRow);

		for (j = n; j >= 1; j--)
		{
			mV = CMatrix(m_nRow - j + 1, 1);

			if (j == m_nRow)
			{
				mV(1) = 1;
			} 
			else
			{
				mV(1) = 1;
				mV.SetRect(2, 1, mA.GetRect(j + 1, j, 1, m_nRow - j));
			}

			mTmp = (GenI(m_nRow - j + 1) - mBeta(j) * mV * mV.Transpose()) * mQ.GetRect(j, j, m_nRow - j + 1, m_nRow - j + 1);

			mQ.SetRect(j, j, mTmp);
		}

		if (lastNegative)
		{
			mTmp = -1 * mQ.GetCol(n);
			mQ.SetCol(n, mTmp);
		}
	}
}

// ��m��n�ľ���A����QR�ֽ⣺A = QR������QΪm��m����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
// �������ڡ�Matrix Computations 3rd Edition��һ��P.227�е�Algorithm 5.2.2 (Givens QR)
// Householder QR �� Givens QR ��ͬһ������ A ���зֽ�ʱ���ܵõ���ͬ�������Ǿ��� R������һ����֤�ֽ�õ�
// ���������� Q1 �� Q2 ��ȫ��ͬ�����������Ǿ��� R Ϊ�����Ȳ��ܱ�֤ Q1 �� Q2 ��ȫ��ͬ���� R'(Q1' - Q2') = 0
// ���� (Q1' - Q2') ֻ��0�⣬���� Householder QR �ֽ�õ��� Q �� MATLAB �ֽ�Ľ��ʼ��һ�£�˵�� MATLAB ��
// QR �ֽ⺯�����õ�Ҳ�� Householder ����
void DeepVoid::CMatrix::QR_Givens(CMatrix & mR, CMatrix & mQ /* = CMatrix */) const
{
	CMatrix mA = Clone();

	int m = m_nRow;
	int n;
	if (m_nRow >= m_nCol)
	{
		n = m_nCol;
	} 
	else
	{
		n = m_nRow;
	}

	CMatrix mG(2, 2);
	CMatrix mTmp;
	CMatrix mCSs(4, 1), mCS(4, 1); // mCS��¼ÿ�μ��������cos��sin��i��k�Լ��Ƿ񻻷���
	double c, s;
	int j, i;
	for (j = 1; j <= n; j++)
	{
		for (i = m; i >= j + 1; i--)
		{
			Givens(mA(i - 1, j), mA(i, j), c, s);

			mG(1, 1) = c; mG(1, 2) = -s;
			mG(2, 1) = s; mG(2, 2) =  c;

			mCS(1) = c;
			mCS(2) = s;
			mCS(3) = i - 1;
			mCS(4) = 0;

			mTmp = mG * mA.GetRect(i - 1, j, m_nCol - j + 1, 2);


			// ��������ÿһ�еĶԽ���Ԫ��ʱ����һ��ʼ��[c -s; s c]���õ��ĶԽ�����Ԫ���Ǹ�ֵʱ
			// ����������ȡΪ[-c s; s c]����ʱ���ܱ�֤�������Ԫ��Ϊ0��ͬʱ�����ܱ�֤�ֽ�õ���
			// �����Ǿ���Խ����ϵ�Ԫ��ȫΪ��ֵ
			if (i == j + 1)
			{
				if (mTmp(1, 1) < 0.0)
				{
					mG(1, 1) = -c; mG(1, 2) = s;
					mTmp = mG * mA.GetRect(i - 1, j, m_nCol - j + 1, 2);		
					mCS(4) = 1;
				}
			}

			mCSs.Mosaic(mCS, MOSAICMODE_RIGHT);

			mA.SetRect(i - 1, j, mTmp);
		}
	}

	BOOL lastNegative = FALSE; // �����Ǿ����n�е�n�е�Ԫ���Ƿ�Ϊ����Ĭ���Ƿ�

	if (mA(n, n) < 0.0)
	{
		lastNegative = TRUE;

		mTmp = -1 * mA.GetRect(n, n, m_nCol - n + 1, 1); // ��Ϊ���������л��ţ���֤�Խ���Ԫ��ȫΪ�������ñ�־λ
		mA.SetRect(n, n, mTmp); 
	}

	if (mCSs.m_nCol > 1)
	{
		mCSs = mCSs.GetRect(1, 2, mCSs.m_nCol - 1, 4);
	}
	else
	{
		mCSs = CMatrix(0);
	}

	mR = CMatrix(m_nRow, m_nCol);

	for (j = 1; j <= n; j++)
	{
		mR.SetRect(j, j, mA.GetRect(j, j, m_nCol - j + 1, 1));
	}


	// ����õ���������Q�ż���Q������Ҫʱ�Ͳ�����
	if (!mQ.IsNull())
	{
		mQ = GenI(m_nRow);

		if (!mCSs.IsNull())
		{
			int r = mCSs.m_nCol; // ���ж�������������

			for (j = 1; j <= r; j++)
			{
				c = mCSs(1, j);
				s = mCSs(2, j);
				i = mCSs(3, j);

				if (mCSs(4, j) == 0.0)
				{
					mG(1, 1) =  c; mG(1, 2) = s;
					mG(2, 1) = -s; mG(2, 2) = c;
				} 
				else
				{
					mG(1, 1) = -c; mG(1, 2) = s;
					mG(2, 1) =  s; mG(2, 2) = c;
				}

				mTmp = mQ.GetRect(1, i, 2, m_nRow) * mG;

				mQ.SetRect(1, i, mTmp);
			}
		}

		if (lastNegative)
		{
			mTmp = -1 * mQ.GetCol(n);
			mQ.SetCol(n, mTmp);
		}
	}
}

// ��m��n�ľ���A����RQ�ֽ⣨Ҫ��m<=n����A = RQ������QΪn��n����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
// ���õ���Givens��ת����ʵ�ֵģ�����Ҫ��m<=n��ԭ���ڱʼǱ�������ϸ����
void DeepVoid::CMatrix::RQ_Givens(CMatrix & mR, CMatrix & mQ /* = CMatrix */) const
{
	if (m_nRow > m_nCol)
	{
		AfxMessageBox("����RQ�ֽ�ľ������������С�ڵ���������");

		mR = CMatrix(0);

		if (!mQ.IsNull())
		{
			mQ = CMatrix(0);
		}

		return;
	}

	CMatrix mA = Clone();

	int m = m_nRow;

	CMatrix mG(2, 2);
	CMatrix mTmp;
	CMatrix mCSs(4, 1), mCS(4, 1); // mCS��¼ÿ�μ��������cos��sin��i��k�Լ��Ƿ񻻷���
	double c, s;
	int i, j;
	for (i = m; i >= 2; i--)
	{
		for (j = 1; j <= i - 1; j++)
		{
			Givens(mA(i, j), mA(i, j + 1), c, s);

			mG(1, 1) = s; mG(1, 2) = -c;
			mG(2, 1) = c; mG(2, 2) =  s;

			mCS(1) = c;
			mCS(2) = s;
			mCS(3) = j;
			mCS(4) = 0;

			mTmp = mA.GetRect(1, j, 2, i) * mG;

			if (j + 1 == i)
			{
				if (mTmp(mTmp.m_nRow, 2) < 0.0)
				{
					mG(1, 2) =  c;
					mG(2, 2) = -s;
					mTmp = mA.GetRect(1, j, 2, i) * mG;
					mCS(4) = 1;
				}
			}

			mCSs.Mosaic(mCS, MOSAICMODE_RIGHT);

			mA.SetRect(1, j, mTmp);
		} 
	}

	BOOL lastNegative = FALSE; // �����Ǿ����1�е�1�е�Ԫ���Ƿ�Ϊ����Ĭ���Ƿ�

	if (mA(1, 1) < 0.0)
	{
		lastNegative = TRUE;

		mA(1, 1) = - mA(1, 1); // ��Ϊ�����򷴺ű�����������ñ�־λ
	}


	if (mCSs.m_nCol > 1)
	{
		mCSs = mCSs.GetRect(1, 2, mCSs.m_nCol - 1, 4);
	} 
	else
	{
		mCSs = CMatrix(0);
	}


	mR = CMatrix(m_nRow, m_nCol);

	for (j = 1; j <= m; j++)
	{
		mR.SetRect(j, j, mA.GetRect(j, j, m_nCol - j + 1, 1));
	}

	// ����õ���������Q�ż���Q������Ҫʱ�Ͳ�����
	if (!mQ.IsNull())
	{
		mQ = GenI(m_nCol);

		if (lastNegative)
		{
			mQ(1, 1) = -1;
		}

		if (!mCSs.IsNull())
		{
			int r = mCSs.m_nCol; // ���ж�������������

			for (j = r; j >= 1; j--)
			{
				c = mCSs(1, j);
				s = mCSs(2, j);
				i = mCSs(3, j);

				if (mCSs(4, j) == 0.0)
				{
					mG(1, 1) =  s; mG(1, 2) = c; 
					mG(2, 1) = -c; mG(2, 2) = s; 
				} 
				else
				{
					mG(1, 1) = s; mG(1, 2) =  c;
					mG(2, 1) = c; mG(2, 2) = -s;
				}

				mTmp = mQ.GetRect(1, i, 2, m_nCol) * mG;

				mQ.SetRect(1, i, mTmp);
			}
		}	
	}
}

// �������ڡ�Matrix Computations 3rd Edition��һ��P.144�е�Algorithm 4.2.1 (Cholesky: Gaxpy Version)
// �ԶԳ���������ʵʩCholesky�ֽ� A = LL'�����ضԽ�����Ԫ��ȫΪ���������Ǿ���L���˺���Ҳ�����ж϶Գƾ����Ƿ��������ֽ�ɹ��򷵻صľ������m_pMat��ΪNULL������ΪNULL
CMatrix DeepVoid::CMatrix::Chol_Lower(double eps /* = EPSILON_ZHU */) const
{
	// �����жϾ����Ƿ�Գ�
	if (!IsSymmetric())
	{
		return CMatrix(0);
	}

	CMatrix mL = Clone();

	int i,j;

	for (j = 1; j < mL.m_nRow + 1; j++)
	{
		if (j > 1)
		{
			// A(j:n, j) = A(j:n, j) - A(j:n, 1:j-1)A(j, 1:j-1)'
			mL.SetRect(j, j, mL.GetRect(j, j, 1, mL.m_nRow - j + 1) - mL.GetRect(j, 1, j - 1, mL.m_nRow - j + 1) * mL.GetRect(j, 1, j - 1, 1).Transpose());
		}

		double tempValue = mL(j, j);
		if (fabs(tempValue) < eps || tempValue < 0.0)
		{
			return CMatrix(0);
		}

		double sqrtTmp = sqrt(mL(j, j));

		// A(j:n, j) = A(j:n, j) / sqrt(A(j,j))
		mL.SetRect(j, j, mL.GetRect(j, j, 1, mL.m_nRow - j + 1) / sqrtTmp);
	}

	// ��L�������ǲ�����0
	for (i = 1; i < mL.m_nRow; i++)
	{
		for (j = i + 1; j < mL.m_nCol + 1; j++)
		{
			mL(i, j) = 0.0;
		}
	}

	return mL;
}


// �ԶԳ���������ʵʩCholesky�ֽ� A = UU'�����ضԽ�����Ԫ��ȫΪ���������Ǿ���U���˺���Ҳ�����ж϶Գƾ����Ƿ��������ֽ�ɹ��򷵻صľ������m_pMat��ΪNULL������ΪNULL
CMatrix DeepVoid::CMatrix::Chol_Upper(double eps /* = EPSILON_ZHU */) const
{
	CMatrix mL = Chol_Lower(eps);

	// ��mLΪNull˵��Cholesky�ֽ�ʧ�ܣ�����CMatrix(0)
	if (mL.IsNull())
	{
		return mL;
	}

	// ��mL��ΪNull˵�� A = LL'�ֽ�ɹ�����������RQ�ֽ⽫L�ֽ�� L = UQ��UΪ�����Ǿ���QΪ��������
	CMatrix mU;

	mL.RQ_Givens(mU);

	return mU;
}

void DeepVoid::CMatrix::SVD(CMatrix & mD, CMatrix & mU /*= CMatrix(0)*/, CMatrix & mV /*= CMatrix(0)*/) const
{
	mD = CMatrix(m_nRow, m_nCol);

	if (!mU.IsNull())
	{
		mU = CMatrix(m_nRow, m_nRow);
	} 

	if (!mV.IsNull())
	{
		mV = CMatrix(m_nCol, m_nCol);
	}

	cvSVD(m_pMat, mD.m_pMat, mU.m_pMat, mV.m_pMat);
}

///////////////////////////////////////////////////////////////////////////////////////////

// std related ////////////////////////////////////////////////////////////////////////////////////

// Return whether first element is greater than the second
bool Greater_DMatch_queryIdx(DMatch match1, DMatch match2)
{
	return match1.queryIdx > match2.queryIdx;
}

bool Greater_DMatch_trainIdx(DMatch match1, DMatch match2)
{
	return match1.trainIdx > match2.trainIdx;
}

///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���ܵĽⷽ�̼����������� /////////////////////////////////////////////////////////////////////////////////
// ��ⳬ�����Է�����(����α������С���˽�)

bool DeepVoid::EquSlvPdI_611(double *M, double *B, int m, int n, double *S) // M[m*n]*S[n]=B[m]
{
	if(!MtxPdI_611(M, m, n))
		return false;
	MtxMlt_611(M, B, n, m, 1, S);
	return true;
}

bool DeepVoid::MtxPdI_611(double *A, int m, int n)
{
	double *ATA = new double[n * n];	
	ATAMlt_611(A, m, n, ATA); 

	if( !MtxInvSP_611(ATA, n) )
	{		
		delete[] ATA;
		return false;
	}

	double *AT = new double[n * m];

	MtxTrs_611(A, m, n, AT);
	MtxMlt_611(ATA, AT, n, n, m, A);

	delete[] ATA;	delete[] AT;
	return true;
}

// �Գ�����������棨������ǶԳ�������-- ����ѭ�����±�ŷ�,�Գ����Ż��洢
bool DeepVoid::MtxInvSP_611(double *A, int n)
{
	int i, j, k, m;
	double w, g;
	double *b = new double[n];
	int nn = n * n;	int nn_1 = nn - 1;	int nn_1_n = nn_1 - n;	int n_1 = n - 1;	int n__1 = n + 1;
	int jn;
	for(k = 0; k < n; k++)
	{
		w = A[0];
		if(fabs(w) < 1E-9) 			
		{
			delete[] b;
			return false;
		}	
		m = n_1 - k;
		jn = n;
		for(j = 1; j < n; j++)
		{
			int jn_i;
			g = A[jn];
			b[j] = (j > m) ? g / w : -g / w;
			for(i = 1; i <= j; i++) 
			{
				jn_i = jn + i;		A[jn_i - n__1] = A[jn_i] + g * b[i];
			}
			jn += n;
		}
		A[nn_1] = 1 / w;
		for(j = 1; j< n; j++)	
			A[nn_1_n + j] = b[j];
	}
	jn = 0;
	for(j = 0; j < n_1; j++)
	{
		int in = jn + n;
		for(i = j + 1; i < n; i++) 
		{
			A[jn + i] = A[in + j];
			in += n;
		}
		jn += n;
	}
	delete[] b;
	return true;
}

// һ��������
void DeepVoid::MtxMlt_611(double *A, double *B, int m, int n, int s, double *AB) // A(m*n) * B(n*s) = AB(m*s)
{
	int i, j, l, k, js, jn;
	jn = 0;	js = 0;
	for (j = 0; j < m; j++)
	{
		for (i = 0; i < s; i++)
		{ 
			k = js + i;	AB[k] = 0;
			int ls = 0;
			for (l = 0; l < n; l++)
			{
				int jn_l = jn + l;		int ls_i = ls + i;
				if((A[jn_l] != 0) && (B[ls_i] != 0))
					AB[k] += A[jn_l] * B[ls_i];
				ls += s;
			}
		}
		jn += n;		js += s;
	}
}

// ����ת�ó�ԭ���󣬵õ��Գ���,�Գ����Ż��洢
// ������С���˷��������⣬��m >= n����A��ȫ��n��������ʽֵ��ȫΪ��ʱ��ATAΪ����
void DeepVoid::ATAMlt_611(double *A, int m, int n, double *ATA) // ATA[n*n] = AT[n*m]*A[m*n]
{
	int i, j, k;
	int jn = 0;
	for(j = 0; j < n; j++)
	{
		for(i = 0; i < n; i++)
		{
			int jn_i = jn + i;			int kn = 0;
			for(ATA[jn_i] = 0, k = 0; k < m; k++)
			{
				int kn_j = kn + j;	int kn_i = kn + i;
				if((A[kn_j] != 0) && (A[kn_i] != 0))
					ATA[jn_i] += A[kn_j] * A[kn_i];
				kn += n;
			}
		}
		jn += n;
	}
}

// ����ת��
void DeepVoid::MtxTrs_611(double *A, int m, int n, double *AT) // n=0: A[m*m] -> A[m*m]; n!=0: A(m*n) -> AT(n*m)
{
	if(AT != NULL)
	{
		int i, j, jn = 0;
		for(j = 0; j < m; j++)
		{
			{
				int im = 0;
				for(i = 0; i < n; i++)	
				{
					AT[im + j] = A[jn + i];
					im += m;
				}
			}
			jn += n;
		}
	}
	else
	{
		double *tpAT = new double[n * m];
		int i, j, jn = 0;
		for(j = 0; j < m; j++)
		{
			{
				int im = 0;
				for(i = 0; i < n; i++)	
				{
					tpAT[im + j] = A[jn + i];
					im += m;
				}
			}
			jn += n;
		}
		int jm = 0;
		for(j = 0; j < n; j++)
		{
			{
				for(i = 0; i < m; i++)	
				{
					int jm_i = jm + i;
					A[jm_i] = tpAT[jm_i];
				}
			}
			jm += m;
		}
		delete[] tpAT;
	}
}

// ��������
bool DeepVoid::M_Inversion_611(double *MatI, double *MatO, int n) // A(nDim*nDim) -> AI(nDim*nDim)
{
	int t, i, j, k, u, v, n2 = n * n;
	int *is = new int[n];
	int *js = new int[n];
	double d, p, **MatIO = new double*[n];
	for(t = 0; t < n2; t++)	MatO[t] = MatI[t];
	for (MatIO[0] = MatO, t = 1; t < n; t++)	MatIO[t] = MatIO[t - 1] + n;
	for (k = 0; k < n; k++)
	{
		d = 0;
		for (i = k; i < n; i++)
			for (j = k; j < n; j++)
			{
				p = fabs(MatIO[i][j]);
				if (p > d)
				{
					d = p;	is[k] = i;	js[k] = j;
				}
			}
			if (fabs(d) < 1E-15)	break;
			if (is[k] != k)
				for (j = 0; j < n; j++)
				{
					v = is[k];	p = MatIO[k][j];
					if( (v >= n) || (v < 0) )	
					{
						delete[] is;
						delete[] js;
						delete[] MatIO;	
						return false;
					}
					MatIO[k][j]	= MatIO[v][j];	MatIO[v][j]	= p;
				}
				if (js[k] != k)
					for (i = 0; i < n; i++)
					{
						u = js[k];	p = MatIO[i][k];
						if( (u >= n) || (u < 0) )
						{
							delete[] is;
							delete[] js;
							delete[] MatIO;	
							return false;
						}
						MatIO[i][k]	= MatIO[i][u];	MatIO[i][u] = p;
					}
					if(fabs(MatIO[k][k]) > 0)
						MatIO[k][k] = 1 / MatIO[k][k];
					else
						MatIO[k][k] = 999999.0;
					for (j = 0; j < n; j++)	
						if (j != k) 
							MatIO[k][j] *= MatIO[k][k];
					for (i = 0; i < n; i++)
						if (i != k) 
						{
							for (j = 0; j < n; j++) 
								if (j != k) 
									MatIO[i][j] -= MatIO[i][k] * MatIO[k][j];
						}
						for (i = 0; i < n; i++)	
							if (i != k) 
								MatIO[i][k] *= -MatIO[k][k];
	}
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
			for (j = 0; j < n; j++)
			{
				v = js[k];	p = MatIO[k][j];
				if( (v >= n) || (v < 0) )
				{
					delete[] is;
					delete[] js;
					delete[] MatIO;	
					return false;
				}
				MatIO[k][j]	= MatIO[v][j];	MatIO[v][j]	= p;
			}
			if (is[k] != k)
				for (i = 0; i < n; i++)
				{
					u = is[k];	p = MatIO[i][k];
					if( (u >= n) || (u < 0) )
					{
						delete[] is;
						delete[] js;
						delete[] MatIO;	
						return false;
					}
					MatIO[i][k]	= MatIO[i][u];	MatIO[i][u]	= p;
				}
				if(k == 0)
					break;
	}
	delete[] is;
	delete[] js;
	delete[] MatIO;	
	return true;
}

//��������ֵ�ֽ�
int DeepVoid::M_DecompSVD_611(double *a,int m,int n,double *u,double *v,double eps,int ka)
{
	int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,m1,ks;
	double d,dd,t,sm,sm1,em1,sk,ek,b,c,shh,fg[2],cs[2];
	double *s,*e,*w;
	s=(double*)malloc(ka*sizeof(double));
	e=(double*)malloc(ka*sizeof(double));
	w=(double*)malloc(ka*sizeof(double));
	it=60; k=n;
	if (m-1<n) k=m-1;
	l=m;
	if (n-2<m) l=n-2;
	if (l<0) l=0;
	ll=k;
	if (l>k) ll=l;
	if (ll>=1)
	{ for (kk=1; kk<=ll; kk++)
	{ if (kk<=k)
	{ d=0.0;
	for (i=kk; i<=m; i++)
	{ ix=(i-1)*n+kk-1; d=d+a[ix]*a[ix];}
	s[kk-1]=sqrt(d);
	if (s[kk-1]!=0.0)
	{ ix=(kk-1)*n+kk-1;
	if (a[ix]!=0.0)
	{ s[kk-1]=fabs(s[kk-1]);
	if (a[ix]<0.0) s[kk-1]=-s[kk-1];
	}
	for (i=kk; i<=m; i++)
	{ iy=(i-1)*n+kk-1;
	a[iy]=a[iy]/s[kk-1];
	}
	a[ix]=1.0+a[ix];
	}
	s[kk-1]=-s[kk-1];
	}
	if (n>=kk+1)
	{ for (j=kk+1; j<=n; j++)
	{ if ((kk<=k)&&(s[kk-1]!=0.0))
	{ d=0.0;
	for (i=kk; i<=m; i++)
	{ ix=(i-1)*n+kk-1;
	iy=(i-1)*n+j-1;
	d=d+a[ix]*a[iy];
	}
	if(fabs(a[(kk-1)*n+kk-1]) < 0.000000001)  //9��10�ո�
	{
		a[(kk-1)*n+kk-1] = 0.000000001;   //9��10�ո�
	}
	d=-d/a[(kk-1)*n+kk-1];
	for (i=kk; i<=m; i++)
	{ ix=(i-1)*n+j-1;
	iy=(i-1)*n+kk-1;
	a[ix]=a[ix]+d*a[iy];
	}
	}
	e[j-1]=a[(kk-1)*n+j-1];
	}
	}
	if (kk<=k)
	{ for (i=kk; i<=m; i++)
	{ ix=(i-1)*m+kk-1; iy=(i-1)*n+kk-1;
	u[ix]=a[iy];
	}
	}
	if (kk<=l)
	{ d=0.0;
	for (i=kk+1; i<=n; i++)
		d=d+e[i-1]*e[i-1];
	e[kk-1]=sqrt(d);
	if (e[kk-1]!=0.0)
	{ if (e[kk]!=0.0)
	{ e[kk-1]=fabs(e[kk-1]);
	if (e[kk]<0.0) e[kk-1]=-e[kk-1];
	}
	for (i=kk+1; i<=n; i++)
		e[i-1]=e[i-1]/e[kk-1];
	e[kk]=1.0+e[kk];
	}
	e[kk-1]=-e[kk-1];
	if ((kk+1<=m)&&(e[kk-1]!=0.0))
	{ for (i=kk+1; i<=m; i++) w[i-1]=0.0;
	for (j=kk+1; j<=n; j++)
		for (i=kk+1; i<=m; i++)
			w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];
	for (j=kk+1; j<=n; j++)
		for (i=kk+1; i<=m; i++)
		{ ix=(i-1)*n+j-1;
	if(fabs(e[kk]) < 0.000000001) //9��10�ո�
	{
		e[kk] = 0.000000001; //9��10�ո�
	}
	a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];
	}
	}
	for (i=kk+1; i<=n; i++)
		v[(i-1)*n+kk-1]=e[i-1];
	}
	}
	}
	mm=n;
	if (m+1<n) mm=m+1;
	if (k<n) s[k]=a[k*n+k];
	if (m<mm) s[mm-1]=0.0;
	if (l+1<mm) e[l]=a[l*n+mm-1];
	e[mm-1]=0.0;
	nn=m;
	if (m>n) nn=n;
	if (nn>=k+1)
	{ for (j=k+1; j<=nn; j++)
	{ for (i=1; i<=m; i++)
	u[(i-1)*m+j-1]=0.0;
	u[(j-1)*m+j-1]=1.0;
	}
	}
	if (k>=1)
	{ for (ll=1; ll<=k; ll++)
	{ kk=k-ll+1; iz=(kk-1)*m+kk-1;
	if (s[kk-1]!=0.0)
	{ if (nn>=kk+1)
	for (j=kk+1; j<=nn; j++)
	{ d=0.0;
	for (i=kk; i<=m; i++)
	{ ix=(i-1)*m+kk-1;
	iy=(i-1)*m+j-1;
	if(fabs(u[iz]) < 0.000000001)  //9��10�ո�
	{
		u[iz] = 0.000000001;       //9��10�ո�
	}
	d=d+u[ix]*u[iy]/u[iz];
	}
	d=-d;
	for (i=kk; i<=m; i++)
	{ ix=(i-1)*m+j-1;
	iy=(i-1)*m+kk-1;
	u[ix]=u[ix]+d*u[iy];
	}
	}
	for (i=kk; i<=m; i++)
	{ ix=(i-1)*m+kk-1; u[ix]=-u[ix];}
	u[iz]=1.0+u[iz];
	if (kk-1>=1)
		for (i=1; i<=kk-1; i++)
			u[(i-1)*m+kk-1]=0.0;
	}
	else
	{ for (i=1; i<=m; i++)
	u[(i-1)*m+kk-1]=0.0;
	u[(kk-1)*m+kk-1]=1.0;
	}
	}
	}
	for (ll=1; ll<=n; ll++)
	{ kk=n-ll+1; iz=kk*n+kk-1;
	if ((kk<=l)&&(e[kk-1]!=0.0))
	{ for (j=kk+1; j<=n; j++)
	{ d=0.0;
	for (i=kk+1; i<=n; i++)
	{ ix=(i-1)*n+kk-1; iy=(i-1)*n+j-1;
	if(fabs(v[iz]) < 0.000000001) //9��10�ո�
	{
		v[iz] = 0.000000001;   //9��10�ո�
	}
	d=d+v[ix]*v[iy]/v[iz];
	}
	d=-d;
	for (i=kk+1; i<=n; i++)
	{ ix=(i-1)*n+j-1; iy=(i-1)*n+kk-1;
	v[ix]=v[ix]+d*v[iy];
	}
	}
	}
	for (i=1; i<=n; i++)
		v[(i-1)*n+kk-1]=0.0;
	v[iz-n]=1.0;
	}
	for (i=1; i<=m; i++)
		for (j=1; j<=n; j++)
			a[(i-1)*n+j-1]=0.0;
	m1=mm; it=60;
	while (1==1)
	{ if (mm==0)
	{ ppp_611(a,e,s,v,m,n);
	free(s); free(e); free(w); return(1);
	}
	if (it==0)
	{ ppp_611(a,e,s,v,m,n);
	free(s); free(e); free(w); return(-1);
	}
	kk=mm-1;
	while ((kk!=0)&&(fabs(e[kk-1])!=0.0))
	{ d=fabs(s[kk-1])+fabs(s[kk]);
	dd=fabs(e[kk-1]);
	if (dd>eps*d) kk=kk-1;
	else e[kk-1]=0.0;
	}
	if (kk==mm-1)
	{ kk=kk+1;
	if (s[kk-1]<0.0)
	{ s[kk-1]=-s[kk-1];
	for (i=1; i<=n; i++)
	{ ix=(i-1)*n+kk-1; v[ix]=-v[ix];}
	}
	while ((kk!=m1)&&(s[kk-1]<s[kk]))
	{ d=s[kk-1]; s[kk-1]=s[kk]; s[kk]=d;
	if (kk<n)
		for (i=1; i<=n; i++)
		{ ix=(i-1)*n+kk-1; iy=(i-1)*n+kk;
	d=v[ix]; v[ix]=v[iy]; v[iy]=d;
	}
	if (kk<m)
		for (i=1; i<=m; i++)
		{ ix=(i-1)*m+kk-1; iy=(i-1)*m+kk;
	d=u[ix]; u[ix]=u[iy]; u[iy]=d;
	}
	kk=kk+1;
	}
	it=60;
	mm=mm-1;
	}
	else
	{ ks=mm;
	while ((ks>kk)&&(fabs(s[ks-1])!=0.0))
	{ d=0.0;
	if (ks!=mm) d=d+fabs(e[ks-1]);
	if (ks!=kk+1) d=d+fabs(e[ks-2]);
	dd=fabs(s[ks-1]);
	if (dd>eps*d) ks=ks-1;
	else s[ks-1]=0.0;
	}
	if (ks==kk)
	{ kk=kk+1;
	d=fabs(s[mm-1]);
	t=fabs(s[mm-2]);
	if (t>d) d=t;
	t=fabs(e[mm-2]);
	if (t>d) d=t;
	t=fabs(s[kk-1]);
	if (t>d) d=t;
	t=fabs(e[kk-1]);
	if (t>d) d=t;
	if(fabs(d) < 0.000000001)  //9��10�ո�
	{
		d = 0.000000001;   //9��10�ո�
	}
	sm=s[mm-1]/d; sm1=s[mm-2]/d;
	em1=e[mm-2]/d;
	sk=s[kk-1]/d; ek=e[kk-1]/d;
	b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
	c=sm*em1; c=c*c; shh=0.0;
	if ((b!=0.0)||(c!=0.0))
	{ shh=sqrt(b*b+c);
	if (b<0.0) shh=-shh;
	//	shh=c/(b+shh);  //9��10�ո�
	double mm = b+shh;   //9��10�ո�
	if(fabs(mm) < 0.000000001)   //9��10�ո�
	{
		mm = 0.000000001;   //9��10�ո�
	}
	shh=c/mm;  //9��10�ո�
	}
	fg[0]=(sk+sm)*(sk-sm)-shh;
	fg[1]=sk*ek;
	for (i=kk; i<=mm-1; i++)
	{ sss_611(fg,cs);
	if (i!=kk) e[i-2]=fg[0];
	fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
	e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
	fg[1]=cs[1]*s[i];
	s[i]=cs[0]*s[i];
	if ((cs[0]!=1.0)||(cs[1]!=0.0))
		for (j=1; j<=n; j++)
		{ ix=(j-1)*n+i-1;
	iy=(j-1)*n+i;
	d=cs[0]*v[ix]+cs[1]*v[iy];
	v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
	v[ix]=d;
	}
	sss_611(fg,cs);
	s[i-1]=fg[0];
	fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
	s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
	fg[1]=cs[1]*e[i];
	e[i]=cs[0]*e[i];
	if (i<m)
		if ((cs[0]!=1.0)||(cs[1]!=0.0))
			for (j=1; j<=m; j++)
			{ ix=(j-1)*m+i-1;
	iy=(j-1)*m+i;
	d=cs[0]*u[ix]+cs[1]*u[iy];
	u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
	u[ix]=d;
	}
	}
	e[mm-2]=fg[0];
	it=it-1;
	}
	else
	{ if (ks==mm)
	{ kk=kk+1;
	fg[1]=e[mm-2]; e[mm-2]=0.0;
	for (ll=kk; ll<=mm-1; ll++)
	{ i=mm+kk-ll-1;
	fg[0]=s[i-1];
	sss_611(fg,cs);
	s[i-1]=fg[0];
	if (i!=kk)
	{ fg[1]=-cs[1]*e[i-2];
	e[i-2]=cs[0]*e[i-2];
	}
	if ((cs[0]!=1.0)||(cs[1]!=0.0))
		for (j=1; j<=n; j++)
		{ ix=(j-1)*n+i-1;
	iy=(j-1)*n+mm-1;
	d=cs[0]*v[ix]+cs[1]*v[iy];
	v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
	v[ix]=d;
	}
	}
	}
	else
	{ kk=ks+1;
	fg[1]=e[kk-2];
	e[kk-2]=0.0;
	for (i=kk; i<=mm; i++)
	{ fg[0]=s[i-1];
	sss_611(fg,cs);
	s[i-1]=fg[0];
	fg[1]=-cs[1]*e[i-1];
	e[i-1]=cs[0]*e[i-1];
	if ((cs[0]!=1.0)||(cs[1]!=0.0))
		for (j=1; j<=m; j++)
		{ ix=(j-1)*m+i-1;
	iy=(j-1)*m+kk-2;
	d=cs[0]*u[ix]+cs[1]*u[iy];
	u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
	u[ix]=d;
	}
	}
	}
	}
	}
	}
	return(1);
}

void DeepVoid::ppp_611(double *a,double *e,double *s,double *v,int m,int n)
{
	int i,j,p,q;
	double d;
	if (m>=n) i=n;
	else i=m;
	for (j=1; j<=i-1; j++)
	{ a[(j-1)*n+j-1]=s[j-1];
	a[(j-1)*n+j]=e[j-1];
	}
	a[(i-1)*n+i-1]=s[i-1];
	if (m<n) a[(i-1)*n+i]=e[i-1];
	for (i=1; i<=n-1; i++)
		for (j=i+1; j<=n; j++)
		{ p=(i-1)*n+j-1; q=(j-1)*n+i-1;
	d=v[p]; v[p]=v[q]; v[q]=d;
	}
	return;
}

void DeepVoid::sss_611(double *fg,double *cs)
{ double r,d;
if ((fabs(fg[0])+fabs(fg[1]))==0.0)
{ cs[0]=1.0; cs[1]=0.0; d=0.0;}
else 
{ d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
if (fabs(fg[0])>fabs(fg[1]))
{ d=fabs(d);
if (fg[0]<0.0) d=-d;
}
if (fabs(fg[1])>=fabs(fg[0]))
{ d=fabs(d);
if (fg[1]<0.0) d=-d;
}
if(fabs(d) < 0.000000001)  //9��10�ո�
{
	d = 0.000000001;   //9��10�ո�
}
cs[0]=fg[0]/d; cs[1]=fg[1]/d;
}
r=1.0;
if (fabs(fg[0])>fabs(fg[1])) r=cs[1];
else
	if (cs[0]!=0.0) r=1.0/cs[0];
fg[0]=d; fg[1]=r;
return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �� 1 ά�������ð�ݷ����򣬿�����������Ҳ�ɽ������У������ԭ�����Ԫ����������������������е�����
void DeepVoid::Sort_Bubble(double * p,					// ��������������������飬����ִ����������е�Ԫ�ؽ���������߽�������
						   int n,						// ���룺���鳤�ȣ�Ԫ�ظ���
						   int * idx,					// �����������ɺ��Ԫ��ԭ���������ŵ���������������� 0 ��ʼ
						   BOOL bIsAscent /*= TRUE*/	// ���룺�����򣬻��ǽ�������
						   )
{
	int m , k, j, i;
	for( i=0; i<n; i++ )			//Ԥ������������
	{
		idx[i]=i;       
	}

	if(bIsAscent)
	{
		k = 0;
		m = n-1;
		while ( k < m)// k>��m˵���Ѿ��ź�
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{				
				/*if (p[idx[i]] > p[idx[i+1]] )*/
				if (p[i] > p[i+1])
				{ 
					Swap_double(p[i], p[i+1]);        // ����������ֵ
					Swap_int(idx[i], idx[i+1]);    // �������������е�����
					m = i;// m�������������
				}
			}
			j = k + 1;
			k = 0;
			for (i = m; i >= j; i--)
			{
				/*if (p[idx[i-1]] > p[idx[i]])*/
				if (p[i-1] > p[i])
				{ 
					Swap_double(p[i-1], p[i]);
					Swap_int(idx[i], idx[i-1]);
					k = i;// k��ǰ������������
				}
			}
		}
	}
	else
	{
		k = 0;
		m = n-1;
		while ( k < m)
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{
				/*if (p[idx[i]] < p[idx[i+1]] )*/
				if (p[i] < p[i+1])
				{ 
					Swap_double(p[i], p[i+1]);
					Swap_int(idx[i], idx[i+1]);
					m = i; 
				}
			}
			j = k + 1;
			k = 0;
			for (i = m; i >= j; i--)
			{
				/*if (p[idx[i-1]] < p[idx[i]])*/
				if (p[i-1] < p[i])
				{ 
					Swap_double(p[i-1], p[i]);
					Swap_int(idx[i], idx[i-1]); 
					k = i;
				}
			}
		}
	}
}

// �� 1 ά����������������������������
void DeepVoid::Sort_byIndex_double(double * p,	// ��������������������飬����ִ����������е�Ԫ�ؽ��� idx �е�����������������
								   int n,		// ���룺���鳤�ȣ�Ԫ�ظ���
								   int * idx	// ���룺����ֵ���������Ǹ���������������е��������������еĸ�Ԫ�ؽ��������еģ������� 0 ��ʼ
								   )
{
	double * pNew = new double [n];

	int i;
	for (i = 0; i < n; i++)
	{
		pNew[i] = p[idx[i]];
	}

	memcpy(p, pNew, n * sizeof(double));

	delete [] pNew;
}

// �� 1 ά����������������������������
void DeepVoid::Sort_byIndex_int(int * p,	// ��������������������飬����ִ����������е�Ԫ�ؽ��� idx �е�����������������
								int n,		// ���룺���鳤�ȣ�Ԫ�ظ���
								int * idx	// ���룺����ֵ���������Ǹ���������������е��������������еĸ�Ԫ�ؽ��������еģ������� 0 ��ʼ
								)
{
	int * pNew = new int [n];

	int i;
	for (i = 0; i < n; i++)
	{
		pNew[i] = p[idx[i]];
	}

	memcpy(p, pNew, n * sizeof(int));

	delete [] pNew;
}

// �� 2 ά�����ÿһ�а�ĳһ�е����ݽ���������߽�������
void DeepVoid::Sort2_byRow(double ** p,					// ����������������� 2 ά����
						   int w, int h,				// ���룺2 ά����Ŀ��
						   int row /*= 0*/,				// ���룺��ĳһ�е����ݶ������н�������Ĭ��Ϊ�� 1 ��
						   BOOL bIsAscent /*= TRUE*/	// ���룺�������ǽ������У�Ĭ��Ϊ����
						   )
{
	int m , k, j, i, ii;
	
	if(bIsAscent)
	{
		k = 0;
		m = w-1;
		while ( k < m)// k>��m˵���Ѿ��ź�
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{				
				if (p[row][i] > p[row][i+1])
				{ 
					for (ii = 0; ii < h; ii++)
					{
						Swap_double(p[ii][i], p[ii][i+1]);        // ����������ֵ
					}
					m = i;// m�������������
				}
			}
			j = k + 1;
			k = 0;
			for (i = m; i >= j; i--)
			{
				if (p[row][i-1] > p[row][i])
				{
					for (ii = 0; ii < h; ii++)
					{
						Swap_double(p[ii][i-1], p[ii][i]);	
					}
					k = i;// k��ǰ������������
				}
			}
		}
	}
	else
	{
		k = 0;
		m = w-1;
		while ( k < m)
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{
				if (p[row][i] < p[row][i+1])
				{ 
					for (ii = 0; ii < h; ii++)
					{
						Swap_double(p[ii][i], p[ii][i+1]);
					}
					m = i; 
				}
			}
			j = k + 1;
			k = 0;
			for (i = m; i >= j; i--)
			{
				if (p[row][i-1] < p[row][i])
				{ 
					for (ii = 0; ii < h; ii++)
					{
						Swap_double(p[ii][i-1], p[ii][i]);
					}
					k = i;
				}
			}
		}
	}
}

// �� 2 ά�����ÿһ�а�ĳһ�е����ݽ���������߽�������
void DeepVoid::Sort2_byCol(double ** p,					// ����������������� 2 ά����
						   int w, int h,				// ���룺2 ά����Ŀ��
						   int col /*= 0*/,				// ���룺��ĳһ�е����ݶ������н�������Ĭ��Ϊ�� 1 ��
						   BOOL bIsAscent /*= TRUE*/	// ���룺�������ǽ������У�Ĭ��Ϊ����
						   )
{
	int m , k, j, i, ii;
	
	if(bIsAscent)
	{
		k = 0;
		m = h-1;
		while ( k < m)// k>��m˵���Ѿ��ź�
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{				
				if (p[i][col] > p[i+1][col])
				{ 
					for (ii = 0; ii < w; ii++)
					{
						Swap_double(p[i][ii], p[i+1][ii]);        // ����������ֵ	
					}
					m = i;// m�������������
				}
			}
			j = k + 1;
			k = 0;
			for (i = m; i >= j; i--)
			{
				if (p[i-1][col] > p[i][col])
				{ 
					for (ii = 0; ii < w; ii++)
					{
						Swap_double(p[i-1][ii], p[i][ii]);
					}
					k = i;// k��ǰ������������
				}
			}
		}
	}
	else
	{
		k = 0;
		m = h-1;
		while ( k < m)
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{
				if (p[i][col] < p[i+1][col])
				{ 
					for (ii = 0; ii < w; ii++)
					{
						Swap_double(p[i][ii], p[i+1][ii]);
					}
					m = i; 
				}
			}
			j = k + 1;
			k = 0;
			for (i = m; i >= j; i--)
			{
				if (p[i-1][col] < p[i][col])
				{ 
					for (ii = 0; ii < w; ii++)
					{
						Swap_double(p[i-1][ii], p[i][ii]);
					}
					k = i;
				}
			}
		}
	}
}

// �ҵ������� 1 ά�����е���Сֵ���������Сֵ�������е�����
double DeepVoid::FindArrayMin(double * p,						// ���룺������� 1 ά����
							  int n,							// ���룺����ĳ���
							  int & idx,						// ������ҵ���������Сֵ���������� 0 ��ʼ
							  double * pMinThresh /*= NULL*/	// even the minimal value is supposed to be larger than this threshold
							  )
{
	double val_min = p[0];
	idx = 0;
	double val_cur;

	int i;
	for (i = 1; i < n; i++)
	{
		val_cur = p[i];

		if (pMinThresh)
		{
			if (val_min <= *pMinThresh)
			{
				val_min = val_cur;
				continue;
			}

			if (val_cur <= *pMinThresh)
			{
				continue;
			}
		}

		if (val_cur < val_min)
		{
			val_min = val_cur;
			idx = i;
		}
	}

	return val_min;
}

// �ҵ������� 1 ά�����е����ֵ����������ֵ�������е�����
double DeepVoid::FindArrayMax(double * p,	// ���룺������� 1 ά����
							  int n,		// ���룺����ĳ���
							  int & idx		// ������ҵ����������ֵ���������� 0 ��ʼ
							  )
{
	double val_max = p[0];
	idx = 0;
	double val_cur;

	int i;
	for (i = 1; i < n; i++)
	{
		val_cur = p[i];

		if (val_cur > val_max)
		{
			val_max = val_cur;
			idx = i;
		}
	}

	return val_max;
}

// �ҵ������� 1 ά�����е����ֵ����������ֵ�������е�����
BYTE DeepVoid::FindArrayMax_BYTE(BYTE * p,		// ���룺������� 1 ά����
								 int n,			// ���룺����ĳ���
								 int & idx		// ������ҵ����������ֵ���������� 0 ��ʼ
								 )
{
	BYTE val_max = p[0];
	idx = 0;
	BYTE val_cur;

	int i;
	for (i = 1; i < n; i++)
	{
		val_cur = p[i];

		if (val_cur > val_max)
		{
			val_max = val_cur;
			idx = i;
		}
	}

	return val_max;
}

// �ҵ������� 1 ά�����е����ֵ����������ֵ�������е�����
int DeepVoid::FindArrayMax_int(int * p,		// ���룺������� 1 ά����
							   int n,		// ���룺����ĳ���
							   int & idx	// ������ҵ����������ֵ���������� 0 ��ʼ
							   )
{
	int val_max = p[0];
	idx = 0;
	int val_cur;

	int i;
	for (i = 1; i < n; i++)
	{
		val_cur = p[i];

		if (val_cur > val_max)
		{
			val_max = val_cur;
			idx = i;
		}
	}

	return val_max;
}

// ���� double �͵ı���������ֵ
void DeepVoid::Swap_double(double & A, double & B)
{
	double temp = B;
	B = A;
	A = temp;
}

// ���� int �͵ı���������ֵ
void DeepVoid::Swap_int(int & A, int & B)
{
	int temp = B;
	B = A;
	A = temp;
}

// The same procedure with drawing a line, except that this function output
// the points on the line, instead of actually drawing a line.
void DeepVoid::GetLinePts(int x0, int y0,		// input: integers, the coordinates of the starting point
						  int x1, int y1,		// input: integers, the coordinates of the ending point
						  int * px, int * py,	// output:the generated n points on the line
						  int & n				// output:the amount of points in the output
						  )
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int i;

	// if x1 = x0, then the line is vertical
	if (dx == 0)
	{
		if (dy > 0)
		{
			n = dy + 1;
			for (i=0;i<n;i++)
			{
				px[i] = x0;
				py[i] = y0+i;
			}
		} 
		else
		{
			n = -dy + 1;
			for (i=0;i<n;i++)
			{
				px[i] = x0;
				py[i] = y0-i;
			}
		}

		return;
	}

	// if x1 != x0, then go with the simplest way
	double slope = (double)dy/(double)dx;

	if (dx > 0) // x1>x0
	{
		n = dx + 1;
		for (i=0;i<n;i++)
		{
			px[i] = x0+i;
			py[i] = FTOI(slope * i + y0); // round y
		}
	}
	else
	{
		n = -dx + 1;
		for (i=0;i<n;i++)
		{
			px[i] = x0-i;
			py[i] = FTOI(-slope * i + y0); // round y
		}
	}
}

void DeepVoid::SaveCameraData(CString path, const cam_data & cam)
{
	int i,j;

	FILE * file = fopen(path, "w");

	fprintf(file, "Intrinsic Parameters\n%.12f	%.12f	%.12f	%.12f	%.12f\n\n", cam.fx, cam.fy, cam.cx, cam.cy, cam.s);

	CString str, strtmp;


// 	str.Format("Distortion Coefficients\n");
// 	for (i=0;i<5;i++)
// 	{
// 		strtmp.Format("%.12f	", cam.k[i]);
// 		str += strtmp;
// 	}
// 	str += "\n\n";
// 	fprintf(file, str);

	str.Format("Distortion Coefficients\n");
	for (i=0;i<5;i++)
	{
		strtmp.Format("%.12f	", cam.m_dist(i));
		str += strtmp;
	}
	str += "\n\n";
	fprintf(file, str);


// 	str.Format("Rotation Matrix\n");
// 	for (i=0;i<3;i++)
// 	{
// 		for (j=0;j<3;j++)
// 		{
// 			strtmp.Format("%.12f	", cam.R[i*3+j]);
// 			str += strtmp;
// 		}
// 		str += "\n";
// 	}
// 	str += "\n";
// 	fprintf(file, str);

	str.Format("Rotation Matrix\n");
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
//			strtmp.Format("%.12f	", cam.m_R(i*3+j));
			strtmp.Format("%.12f	", cam.m_R(i,j)); // 20160317, CV_StaticAssert(m == 1 || n == 1, "Single index indexation requires matrix to be a column or a row");
			str += strtmp;
		}
		str += "\n";
	}
	str += "\n";
	fprintf(file, str);


// 	str.Format("Translation Vector\n");
// 	for (i=0;i<3;i++)
// 	{
// 		strtmp.Format("%.12f	", cam.t[i]);
// 		str += strtmp;
// 	}
// 	str += "\n\n";
// 	fprintf(file, str);

	str.Format("Translation Vector\n");
	for (i=0;i<3;i++)
	{
		strtmp.Format("%.12f	", cam.m_t(i));
		str += strtmp;
	}
	str += "\n\n";
	fprintf(file, str);


	fclose(file);
}

void DeepVoid::ReadCameraData(CString path, cam_data & cam)
{
	int i,j;

	FILE * file = fopen(path, "r");

	fscanf(file, "Intrinsic Parameters\n");
	fscanf(file, "%lf	%lf	%lf	%lf	%lf\n\n", &cam.fx, &cam.fy, &cam.cx, &cam.cy, &cam.s);

	fscanf(file, "Distortion Coefficients\n");
	for (i=0;i<5;i++)
	{
		fscanf(file, "%lf", &cam.k[i]);
	}
	fscanf(file, "\n\n");

	fscanf(file, "Rotation Matrix\n");
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			fscanf(file, "%lf", &cam.R[i*3+j]);
		}
	}
	fscanf(file, "\n\n");

	fscanf(file, "Translation Vector\n");
	for (i=0;i<3;i++)
	{
		fscanf(file, "%lf", &cam.t[i]);
	}
	fscanf(file, "\n\n");

	fclose(file);
}

void DeepVoid::DetermineInterval(double val_max, double val_min, double val_cur, double radius, double & inter_max, double & inter_min)
{
	if (val_cur >= val_max || val_cur <= val_min)
	{
		inter_max = val_max;
		inter_min = val_min;
		return;
	}

	inter_max = val_cur + radius;
	inter_min = val_cur - radius;

	if (inter_max>val_max)
	{
		inter_max = val_max;
	}

	if (inter_min<val_min)
	{
		inter_min = val_min;
	}
}

// compute exp((-1/2)*(x-miu)^2/sigma^2)
double DeepVoid::exp_miu_sigma(double x, double miu, double sigma)
{
	double dx = x-miu;
	double a = dx/sigma;
	double a2 = a*a;

	return exp(-0.5*a2);
}

// 2020.06.14, instead of drawing all the matches across two images,
// this func trys to draw matches within only the reference/left image
// with a 'line' starting from one feature, and ending at the matching feature.
void DeepVoid::drawMatchesRefImg(const cv::Mat & img1,									// input: the reference/left image
								 const std::vector<cv::KeyPoint> & keypoints1,			// input: keypoints found in the reference/left image
								 const std::vector<cv::KeyPoint> & keypoints2,			// input: keypoints found in the matching image
								 const std::vector<cv::DMatch> & matches1to2,			// input: matches found from ref image to matching image
								 cv::Mat & outImg,										// output: the output image with matches drawn
								 const Scalar & matchColor/* = Scalar(0, 255, 0)*/,		// input: the color of the matched points connecting lines 
								 const Scalar & singlePointColor/* = Scalar(0, 0, 255)*/// input: the color of the unmatched points
								 )
{
	// �ȰѲο�ͼ�����������㻭����
	cv::drawKeypoints(img1, keypoints1, outImg, singlePointColor);

	vector<KeyPoint> keypoints1_FAST_all;

	for (auto iter = keypoints1.begin(); iter != keypoints1.end(); ++iter)
	{
		if (/*fabs(iter->size + 1) < 1.0E-12 && */fabs(iter->angle + 1) < 1.0E-12) // Ŀǰ����ֻ�� fast ������� angle ��Ϊ-1��sift ������Ƕȵ�ȡֵ��ΧΪ0-360��
		{
			keypoints1_FAST_all.push_back(*iter);
		}
	}

	// �ٵ����� FAST �����㻭�������ɫ
	cv::drawKeypoints(outImg, keypoints1_FAST_all, outImg, Scalar(0, 255, 255));

	vector<KeyPoint> matchedKeyPts1; // all the matched keypoints in the reference image
	vector<KeyPoint> matchedFASTPts1;

	for (auto iter = matches1to2.begin(); iter != matches1to2.end(); ++iter)
	{
		const KeyPoint & keypt1 = keypoints1[iter->queryIdx];
		const KeyPoint & keypt2 = keypoints2[iter->trainIdx];

		if (/*fabs(keypt1.size + 1) < 1.0E-12 && */fabs(keypt1.angle + 1) < 1.0E-12) // Ŀǰ����ֻ�� fast ������� angle ��Ϊ-1��sift ������Ƕȵ�ȡֵ��ΧΪ0-360��
		{
			cv::line(outImg, keypt1.pt, keypt2.pt, Scalar(255, 255, 0));

			matchedFASTPts1.push_back(keypt1);
		}
		else
		{
			cv::line(outImg, keypt1.pt, keypt2.pt, matchColor);

			matchedKeyPts1.push_back(keypt1);
		}		
	}

	cv::drawKeypoints(outImg, matchedKeyPts1, outImg, matchColor);
	cv::drawKeypoints(outImg, matchedFASTPts1, outImg, Scalar(255, 255, 0));
}

// 20200717��������Բ�ȷ����ˮƽ��������Ӧ��ɫ��ֵ��RGɫ���򣩣������ڵ��Ʋ�ȷ���ȿ��ӻ�
void DeepVoid::getRGColorforRelativeUncertainty(double uctt, /* input: the given relative uncertainty */ 
												double val_worst, /* input: the worst value for relative uncertainty, which is set to color Red */ 
												double val_best, /* input: the best value for relative uncertainty, which is set to color Green */
												uchar & r, /* output: the computed R */ 
												uchar & g, /* output: the computed G */ 
												uchar & b /* output: the computed B */
												)
{
	double uctt_log = -std::log10(uctt);
	double worst_log = -std::log10(val_worst);
	double best_log = -std::log10(val_best);

	if (uctt_log < worst_log)
	{
		r = 255;
		g = 0;
		b = 0;
	} 
	else if (uctt_log > best_log)
	{
		r = 0;
		g = 255;
		b = 0;
	} 
	else
	{
		// ���� RG ɫ�����ɫ���
		double ang = 90 * (uctt_log - worst_log) / (best_log - worst_log);

		if (ang < 45)
		{
			r = 255;
			g = 255 * DeepVoid::tand(ang);
			b = 0;
		}
		else
		{
			r = 255 / DeepVoid::tand(ang);
			g = 255;
			b = 0;
		}

		//r = 255 * DeepVoid::cosd(ang);
		//g = 255 * DeepVoid::sind(ang);
		//b = 0;
	}	
}

void DeepVoid::getRGColorforRelativeUncertainty(double uctt, double val_worst, double val_best, int & r, int & g, int & b)
{
	uchar r_uchar, g_uchar, b_uchar;

	getRGColorforRelativeUncertainty(uctt, val_worst, val_best, r_uchar, g_uchar, b_uchar);

	r = (int)r_uchar;
	g = (int)g_uchar;
	b = (int)b_uchar;
}

// 20200729
// void DeepVoid::ExtractSiftFeatures(Features & feats,
// 								   const cv::Mat & img,
// 								   int nfeatures /*= 0*/,					// The number of best features to retain. The features are ranked by their scores (measured in SIFT algorithm as the local contrast)
// 								   int nOctaveLayers /*= 3*/,				// The number of layers in each octave. 3 is the value used in D.Lowe paper.The number of octaves is computed automatically from the image resolution.
// 								   double contrastThreshold /*= 0.03*/,		// The contrast threshold used to filter out weak features in semi-uniform (low - contrast) regions.The larger the threshold, the less features are produced by the detector.
// 								   double edgeThreshold /*= 10*/,			// The threshold used to filter out edge-like features. Note that the its meaning is different from the contrastThreshold, i.e.the larger the edgeThreshold, the less features are filtered out(more features are retained).
// 								   double sigma /*= 1.6*/					// The sigma of the Gaussian applied to the input image at the octave \#0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number.
// 								   )
// {
// 	feats.clear(); // �Ȱ�֮ǰ�����
// 
// 	cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
// 
// 	// ��ȡ sift ������
// 	f2d->detect(img, feats.key_points);
// 
// 	// ������ size �Ӵ�С�� sift �������������
// 	sort(feats.key_points.begin(), feats.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.size > b.size; });
// 
// 	// ����������������
// 	f2d->compute(img, feats.key_points, feats.descriptors);
// 
// 	feats.type = Feature_SIFT_SIFT; // sift keypoints + sift descriptors
// 
// 	// ������Ҫ��Ϊ�˽� sift �������ظ�λ�õ�������ͬ���������Ϊͳһ��ȫ�ֱ�ţ�����ÿ�������㴦��ɫ��ֵ��ֵ����
// 	int n = feats.key_points.size();
// 	KeyPoint kpt_pre;
// 	kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
// 	kpt_pre.size = -1000;
// 	int idx_imgPt;
// 	for (int i = 0; i < n; ++i)
// 	{
// 		feats.tracks.push_back(-1);
// 
// 		KeyPoint kpt_cur = feats.key_points[i];
// 		if (fabs(kpt_cur.pt.x - kpt_pre.pt.x) < 1.0E-12 && fabs(kpt_cur.pt.y - kpt_pre.pt.y) < 1.0E-12
// 			&& fabs(kpt_cur.size - kpt_pre.size) < 1.0E-12)
// 		{
// 			// indicating that current keypoint is identical to the previous keypoint
// 			feats.idx_pt.push_back(idx_imgPt);
// 		}
// 		else
// 		{
// 			feats.idx_pt.push_back(i);
// 			idx_imgPt = i;
// 		}
// 
// 		kpt_pre.pt.x = kpt_cur.pt.x;
// 		kpt_pre.pt.y = kpt_cur.pt.y;
// 		kpt_pre.size = kpt_cur.size;
// 
// 		// 20150215, zhaokunz, �Ѹ����������ɫ��Ϣ��ֵ����
// 		uchar r, g, b;
// 		Vec3b rgb;
// 		if (BilinearInterp(img, kpt_cur.pt.x, kpt_cur.pt.y, r, g, b))
// 		{
// 			rgb[0] = b;
// 			rgb[1] = g;
// 			rgb[2] = r;
// 		}
// 		feats.rgbs.push_back(rgb);
// 	}
// }
// 
// // 20200729
// void DeepVoid::ExtractFASTFeatures(Features & feats,
// 								   const cv::Mat & img,
// 								   int thresholdFast /*= 20*/,				// threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.
// 								   bool nonmaxSuppressionFast /*= true*/,		// if true, non-maximum suppression is applied to detected corners (keypoints).
// 								   int typeFast /*= cv::FastFeatureDetector::TYPE_9_16*/,	// one of the three neighborhoods as defined in the paper: FastFeatureDetector::TYPE_9_16, FastFeatureDetector::TYPE_7_12, FastFeatureDetector::TYPE_5_8
// 								   int nfeaturesSift /*= 0*/,					// The number of best features to retain. The features are ranked by their scores (measured in SIFT algorithm as the local contrast)
// 								   int nOctaveLayersSift /*= 3*/,				// The number of layers in each octave. 3 is the value used in D.Lowe paper.The number of octaves is computed automatically from the image resolution.
// 								   double contrastThresholdSift /*= 0.03*/,	// The contrast threshold used to filter out weak features in semi-uniform (low - contrast) regions.The larger the threshold, the less features are produced by the detector.
// 								   double edgeThresholdSift /*= 10*/,			// The threshold used to filter out edge-like features. Note that the its meaning is different from the contrastThreshold, i.e.the larger the edgeThreshold, the less features are filtered out(more features are retained).
// 								   double sigmaSift /*= 1.6*/					// The sigma of the Gaussian applied to the input image at the octave \#0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number.
// 								   )
// {
// 	feats.clear(); // �Ȱ�֮ǰ�����
// 
// 	// ��ȡ fast �ǵ�����
// 	// 20200706���Ȱ�ͼ��ת���ɻҶ�ͼ����ȡFAST��������Ϊopencv fast����ֻ�������ڻҶ�ͼ��sift�ǲ�ɫ�ͻҶȽԿɣ�����lightroom�������ͼƬ����ʱ�ᱻ�Զ�תΪ3ͨ��ͼ���ѹ�fast��ȡ����������ƫ��
// 	if (img.channels() < 3)
// 	{
// 		cv::FAST(img, feats.key_points, thresholdFast, nonmaxSuppressionFast, typeFast);
// 	}
// 	else
// 	{
// 		cv::Mat im_gray;
// 		cv::cvtColor(img, im_gray, CV_RGB2GRAY);
// 		cv::FAST(im_gray, feats.key_points, thresholdFast, nonmaxSuppressionFast, typeFast);
// 	}
// 
// 	// ���� response �Ӵ�С�� fast �������������
// 	sort(feats.key_points.begin(), feats.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });
// 
// 	// ����������������
// 	cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(nfeaturesSift, nOctaveLayersSift, contrastThresholdSift, edgeThresholdSift, sigmaSift);
// 	f2d->compute(img, feats.key_points, feats.descriptors);
// 
// 	feats.type = Feature_FAST_SIFT; // fast keypoints + sift descriptors
// 
// 	// ������Ҫ��Ϊ�˽� sift �������ظ�λ�õ�������ͬ���������Ϊͳһ��ȫ�ֱ�ţ�����ÿ�������㴦��ɫ��ֵ��ֵ����
// 	int n = feats.key_points.size();
// 	for (int i = 0; i < n; ++i)
// 	{
// 		const cv::KeyPoint & keypt = feats.key_points[i];
// 
// 		feats.tracks.push_back(-1);
// 		feats.idx_pt.push_back(i);
// 
// 		// 20150215, zhaokunz, �Ѹ����������ɫ��Ϣ��ֵ����
// 		uchar r, g, b;
// 		Vec3b rgb;
// 		if (BilinearInterp(img, keypt.pt.x, keypt.pt.y, r, g, b))
// 		{
// 			rgb[0] = b;
// 			rgb[1] = g;
// 			rgb[2] = r;
// 		}
// 		feats.rgbs.push_back(rgb);
// 	}
// }
// 
// void DeepVoid::GenSfMFeatures(cam_data & cam, int & nSiftElected, int & nFastElected, int & nManualElected, int nSfMFeatures, int nPrptFeatures)
// {
// 	Features & featsSfM = cam.m_feats;
// 	Features & featsSub = cam.m_subFeats;
// 	const Features & feats_sift = cam.m_featsSIFT;
// 	const Features & feats_fast = cam.m_featsFAST;
// 	const Features & feats_manual = cam.m_featsManual;
// 
// 	int nSift = feats_sift.key_points.size();
// 	int nFast = feats_fast.key_points.size();
// 	int nManual = feats_manual.key_points.size();
// 
// 	// �����
// 	featsSfM.clear();
// 	featsSub.clear();
// 
// 	// ��ʱ�Ⱥϳɸ����
// 	Features featsTmp = feats_sift;
// 	featsTmp.push_back(feats_fast);
// 
// 	// Ȼ���ȡΪ���յģ���¼��
// 	int nSize = featsTmp.key_points.size();
// 	int nSmaller = nSize < nSfMFeatures ? nSize : nSfMFeatures;
// 
// 	featsSfM.key_points.insert(featsSfM.key_points.end(), featsTmp.key_points.begin(), featsTmp.key_points.begin() + nSmaller);
// 	featsSfM.descriptors = featsTmp.descriptors.rowRange(cv::Range(0, nSmaller));
// 	featsSfM.tracks.insert(featsSfM.tracks.end(), featsTmp.tracks.begin(), featsTmp.tracks.begin() + nSmaller);
// 	featsSfM.idx_pt.insert(featsSfM.idx_pt.end(), featsTmp.idx_pt.begin(), featsTmp.idx_pt.begin() + nSmaller);
// 	featsSfM.rgbs.insert(featsSfM.rgbs.end(), featsTmp.rgbs.begin(), featsTmp.rgbs.begin() + nSmaller);
// 
// 	// �ٰ������ӽ���
// 	featsSfM.push_back(feats_manual);
// 
// 	int nFinal = featsSfM.key_points.size();
// 
// 	// ͳ�Ƹ������ѡ���� SfM ������ĸ���
// 	nSiftElected = nSift < nSfMFeatures ? nSift : nSfMFeatures;
// 	nFastElected = nSmaller - nSiftElected;
// 	nManualElected = nManual; // ��ȫ������㶼��ѡ
// 
// 	// ���²��� SfM ���������ͳһ���
// 	for (int i = nSiftElected; i < nFinal; ++i)
// 	{
// 		featsSfM.idx_pt[i] = i;
// 	}
// 
// 	// ������ɡ��ȷ����ˡ��������㼯
// 	if (nPrptFeatures < nFinal)
// 	{
// 		featsSub.key_points.insert(featsSub.key_points.end(), featsSfM.key_points.begin(), featsSfM.key_points.begin() + nPrptFeatures);
// 		featsSub.descriptors = featsSfM.descriptors.rowRange(cv::Range(0, nPrptFeatures));
// 		featsSub.tracks.insert(featsSub.tracks.end(), featsSfM.tracks.begin(), featsSfM.tracks.begin() + nPrptFeatures);
// 		featsSub.idx_pt.insert(featsSub.idx_pt.end(), featsSfM.idx_pt.begin(), featsSfM.idx_pt.begin() + nPrptFeatures);
// 		featsSub.rgbs.insert(featsSub.rgbs.end(), featsSfM.rgbs.begin(), featsSfM.rgbs.begin() + nPrptFeatures);
// 	}
// }
