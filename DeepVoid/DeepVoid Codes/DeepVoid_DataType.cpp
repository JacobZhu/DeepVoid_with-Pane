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
		img.copyTo(cv::Mat(imgTmp));

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

// 计算角度的cos值，输入为角度
double DeepVoid::cosd(double angle)
{
	double radian = angle * CV_PI / 180;

	return cos(radian);
}

// 计算角度的sin值，输入为角度
double DeepVoid::sind(double angle)
{
	double radian = angle * CV_PI / 180;

	return sin(radian);
}

// 计算角度的tan值，输入为角度
double DeepVoid::tand(double angle)
{
	double radian = angle * CV_PI / 180;

	return tan(radian);
}

// 计算输入数值的arccosine值，返回值范围为0 - 180°
double DeepVoid::acosd(double x)
{
	double radian = acos(x);

	return radian * 180 / CV_PI;
}

// 计算输入数值的arcsine值，返回值范围为-90 - 90°
double DeepVoid::asind(double x)
{
	double radian = asin(x);

	return radian * 180 / CV_PI;
}

// 计算输入数值的arctangent值，返回值范围为-90 - 90°
double DeepVoid::atand(double x)
{
	double radian = atan(x);

	return radian * 180 / CV_PI;
}

// 计算(y / x)的arctangent值，由于可以综合考虑y和x的符号，因此返回值的范围拓展到四个象限，即-180 - 180°
double DeepVoid::atand(double y, double x)
{
	double radian = atan2(y, x);

	return radian * 180 / CV_PI;
}

// 判断输入 x 的符号，为正则返回 1，为负则返回 -1
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
		AfxMessageBox("参与加法运算的两个矩阵大小不一致！");
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
		AfxMessageBox("参与减法运算的两个矩阵大小不一致！");
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
		AfxMessageBox("参与乘法运算的两个矩阵大小不匹配！");
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

// 不一定非要是方阵
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
	// 进行Cholesky分解，若成功CMatrix的m_pMat不为空，矩阵就是Symmetric positive definite，否则m_pMat为空，说明矩阵不是Symmetric positive definite
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
		AfxMessageBox("矩阵行索引越界！");
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
		AfxMessageBox("拷贝源行矩阵与本矩阵尺寸不匹配！");
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
		AfxMessageBox("矩阵列索引越界！");
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
		AfxMessageBox("拷贝源列矩阵与本矩阵尺寸不匹配！");
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
		AfxMessageBox("矩阵矩形块越界！");
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
		AfxMessageBox("拷贝源矩阵矩形块越界！");
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
		// 优先判断是否是对称正定矩阵，对称正定矩阵一定可逆
		if (IsSymmPosDef())
		{
			invCond = cvInvert(m_pMat, mInv.m_pMat, CV_SVD_SYM);
			return mInv;
		}

		// 若不是对称正定矩阵，而是对称非正定，或者是更一般的非对称方阵时，先判断是否可逆，若可逆，则采用全选主元Gaussian-Jordan法([A|E] -> [E|inv(A)])求其逆
		// 若不可逆，则继续下面的SVD法，给出其pseudo-inverse，所有的非方阵都只有pseudo-inverse
		if (cvInvert(m_pMat, mInv.m_pMat, CV_LU) != 0)
		{
			return mInv;
		}
	} 

	// 当矩阵是非方阵，或者是除对称正定矩阵之外的方阵中不可逆的方阵时，采用SVD法给出其pseudo-inverse
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
			a = (*this)(i, j); // 这个索引还是不能直接用 (i, j)，只能 (*this)(i, j)
			b = atand(a);
			mAngle(i, j) = b;
		}
	}

	return mAngle;
}

CMatrix DeepVoid::Solve(const CMatrix & mA, const CMatrix & mB)
{
	// 判断系数矩阵mA和方程右边阵mB的行数是否一致，不一致就返回空矩阵
	if (mA.m_nRow != mB.m_nRow)
	{
		AfxMessageBox("Row number of A and B don't match when solving Ax = B !");
		return CMatrix(0);
	}

	int i;
	int m = mA.m_nRow;
	int n = mA.m_nCol;

	CMatrix mX(mA.m_nCol, mB.m_nCol);

	// 判断是求齐次方程组Ax=0还是非齐次方程组Ax=B
	if (!mB.IsZero())
	{
		// 先判断是不是对称正定阵，是的话就用针对对称正定阵的解法解方程，不是的话或者求解失败就继续尝试LU法
		if (mA.IsSymmPosDef())
		{
			if (1 == cvSolve(mA.m_pMat, mB.m_pMat, mX.m_pMat, CV_SVD_SYM))
			{
				return mX;
			}
		}

		// 用LU方法解方程的基本思想是，先化A=LU，LUx=B，再先后运用forward substitution和back substitution解y=L\B和x=U\y
		// LU方法只在A列满秩的时候才有效，在列不满秩时会返回0值，求解失败。另外OpenCV的LU解方程函数只接受m>=n的A，若m<n，则直接出错
		// 当方程无精确解时，此LU方法能得到最小二乘解，且耗时少

		// 2011.04.14屏蔽，原因是CV_LU方法运算正确的前提就是A列满秩，可是通过测试发现，OpenCV此LU函数并不能很好的判断矩阵A是否降秩，输入
		// 一个明显的6×4的秩为3的矩阵，此函数未判断出其降秩还继续后面步骤，导致求解出来的x明显是错误的，只有当输入一个后3行全为0的秩是3的
		// 矩阵时此函数才判断出其降秩，从而跳出。出于对精度的高要求，特此屏蔽此函数，统一只用SVD来求解方程组
		// 		if (m >= n)
		// 		{
		// 			if (1 == cvSolve(mA.m_pMat, mB.m_pMat, mX.m_pMat, CV_LU))
		// 			{
		// 				return mX;
		// 			}
		// 		}

		// 在前述方法皆失效的情况下采用SVD的方法求解方程，在矩阵A非列满秩的情况下，要是方程存在精确解此法给出||x||最小的精确解
		// 要是方程不存在精确解此法给出||x||最小的最小二乘解；A列满秩情况下，若方程存在精确解，则此法给出唯一的精确解，若方程不存在
		// 精确解，此法给出唯一的最小二乘解
		if (1 == cvSolve(mA.m_pMat, mB.m_pMat, mX.m_pMat, CV_SVD))
		{
			return mX;
		} 
		else
		{
			return CMatrix(0);
		}
	} 

	// 针对求解Ax=0，利用SVD化A=UDV'，使||Ax||最小化，且||x||=1的解x就是V的最后一列
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
		// cvCrossProduct这个函数只接受列向量作为输入，返回的也是列向量
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

// 对3×3的矩阵进行RQ分解：A = RQ，其中R为对角线元素全为正的上三角矩阵，Q则为正交矩阵
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

// 方法基于《Matrix Computations 3rd Edition》一书P.210中的Algorithm 5.1.1 (Householder Vector)
// Given x∈Rn，this function computes v∈Rn with v(1) = 1 and β∈R such that P = In - βvv' is orthogonal and Px = |x|e1
// 要求输入mX为列向量
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

// 方法基于《Matrix Computations 3rd Edition》一书P.216中的Algorithm 5.1.3 (Computes Givens c and s)
// 根据输入的[a; b]'计算使得[c s; -s c]' * [a; b]' = [r; 0]'成立的cos和sin
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

// 生成n×n的单位阵
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

// 根据输入的三维向量v生成其对应的叉乘矩阵[v]
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
			AfxMessageBox("待拼接的矩阵列数与本矩阵不匹配！");
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
			AfxMessageBox("待拼接的矩阵列数与本矩阵不匹配！");
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
			AfxMessageBox("待拼接的矩阵行数与本矩阵不匹配！");
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
			AfxMessageBox("待拼接的矩阵行数与本矩阵不匹配！");
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

// 基于SVD的求秩，和MATLAB方法一致
int DeepVoid::CMatrix::Rank(double eps /*= 1.0E-10*/) const
{
	// 利用SVD分解得到A=UDV'中的D矩阵，对角线上的奇异值按降序排列
	CMatrix mDiag(m_nRow, m_nCol);
	cvSVD(m_pMat, mDiag.m_pMat);

	// 统计大于阈值eps的奇异值的个数即为矩阵的秩
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

// 计算矩阵条件数的倒数，即最小奇异值与最大奇异值的比值
// 越接近于1越好，越接近0说明矩阵越接近奇异
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
		AfxMessageBox("参与矩阵对应元素相乘运算的两个矩阵大小不一致！");
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
		AfxMessageBox("参与矩阵对应元素相除运算的两个矩阵大小不一致！");
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
	CMatrix mExtended = ExtendRows(mMat, row); // 先按行拓展

	mExtended = ExtendCols(mExtended, col); // 再按列拓展

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
		// 打开文件出错则提示出错原因并返回BOOL
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		CString tmp;
		tmp.Format("无法打开欲读入矩阵的文件: %s", szError);
		AfxMessageBox(tmp);
		/*_tprintf_s(_T("无法打开欲读入矩阵的文件: %1024s"), szError);*/
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


// 对m×n的矩阵A进行QR分解：A = QR，其中Q为m×m的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
// 方法基于《Matrix Computations 3rd Edition》一书P.224中的Algorithm 5.2.1 (Householder QR)
// Householder QR 和 Givens QR 对同一个矩阵 A 进行分解时都能得到相同的上三角矩阵 R，但不一定保证分解得到
// 的正交矩阵 Q1 和 Q2 完全相同，除非上三角矩阵 R 为行满秩才能保证 Q1 和 Q2 完全相同，即 R'(Q1' - Q2') = 0
// 对于 (Q1' - Q2') 只有0解，另外 Householder QR 分解得到的 Q 和 MATLAB 分解的结果始终一致，说明 MATLAB 的
// QR 分解函数采用的也是 Householder 方法
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

	BOOL lastNegative = FALSE; // 上三角矩阵第n行第n列的元素是否为负，默认是否

	if (mA(n, n) < 0.0)
	{
		lastNegative = TRUE;

		mTmp = -1 * mA.GetRect(n, n, m_nCol - n + 1, 1); // 若为负，则整行换号，保证对角线元素全为正，并置标志位
		mA.SetRect(n, n, mTmp); 
	}

	mR = CMatrix(m_nRow, m_nCol);

	mBeta = mBeta.GetRect(2, 1, 1, mBeta.m_nRow - 1);

	for (j = 1; j <= n; j++)
	{
		mR.SetRect(j, j, mA.GetRect(j, j, m_nCol - j + 1, 1));
	}


	// 若需得到正交矩阵Q才计算Q，不需要时就不计算
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

// 对m×n的矩阵A进行QR分解：A = QR，其中Q为m×m的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
// 方法基于《Matrix Computations 3rd Edition》一书P.227中的Algorithm 5.2.2 (Givens QR)
// Householder QR 和 Givens QR 对同一个矩阵 A 进行分解时都能得到相同的上三角矩阵 R，但不一定保证分解得到
// 的正交矩阵 Q1 和 Q2 完全相同，除非上三角矩阵 R 为行满秩才能保证 Q1 和 Q2 完全相同，即 R'(Q1' - Q2') = 0
// 对于 (Q1' - Q2') 只有0解，另外 Householder QR 分解得到的 Q 和 MATLAB 分解的结果始终一致，说明 MATLAB 的
// QR 分解函数采用的也是 Householder 方法
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
	CMatrix mCSs(4, 1), mCS(4, 1); // mCS记录每次计算出来的cos、sin、i、k以及是否换符号
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


			// 当来考虑每一列的对角线元素时，按一开始的[c -s; s c]若得到的对角线上元素是负值时
			// 则正交矩阵取为[-c s; s c]，这时仍能保证下面这个元素为0的同时，还能保证分解得到的
			// 上三角矩阵对角线上的元素全为正值
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

	BOOL lastNegative = FALSE; // 上三角矩阵第n行第n列的元素是否为负，默认是否

	if (mA(n, n) < 0.0)
	{
		lastNegative = TRUE;

		mTmp = -1 * mA.GetRect(n, n, m_nCol - n + 1, 1); // 若为负，则整行换号，保证对角线元素全为正，并置标志位
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


	// 若需得到正交矩阵Q才计算Q，不需要时就不计算
	if (!mQ.IsNull())
	{
		mQ = GenI(m_nRow);

		if (!mCSs.IsNull())
		{
			int r = mCSs.m_nCol; // 共有多少组正交矩阵

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

// 对m×n的矩阵A进行RQ分解（要求m<=n）：A = RQ，其中Q为n×n的正交矩阵，R为m×n的，且主对角线元素皆为正的上三角矩阵
// 利用的是Givens旋转方法实现的，至于要求m<=n的原因在笔记本内有详细解释
void DeepVoid::CMatrix::RQ_Givens(CMatrix & mR, CMatrix & mQ /* = CMatrix */) const
{
	if (m_nRow > m_nCol)
	{
		AfxMessageBox("进行RQ分解的矩阵的行数必须小于等于列数！");

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
	CMatrix mCSs(4, 1), mCS(4, 1); // mCS记录每次计算出来的cos、sin、i、k以及是否换符号
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

	BOOL lastNegative = FALSE; // 上三角矩阵第1行第1列的元素是否为负，默认是否

	if (mA(1, 1) < 0.0)
	{
		lastNegative = TRUE;

		mA(1, 1) = - mA(1, 1); // 若为负，则反号变成正数，并置标志位
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

	// 若需得到正交矩阵Q才计算Q，不需要时就不计算
	if (!mQ.IsNull())
	{
		mQ = GenI(m_nCol);

		if (lastNegative)
		{
			mQ(1, 1) = -1;
		}

		if (!mCSs.IsNull())
		{
			int r = mCSs.m_nCol; // 共有多少组正交矩阵

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

// 方法基于《Matrix Computations 3rd Edition》一书P.144中的Algorithm 4.2.1 (Cholesky: Gaxpy Version)
// 对对称正定矩阵实施Cholesky分解 A = LL'，返回对角线上元素全为正的下三角矩阵L，此函数也用来判断对称矩阵是否正定，分解成功则返回的矩阵对象m_pMat不为NULL，否则为NULL
CMatrix DeepVoid::CMatrix::Chol_Lower(double eps /* = EPSILON_ZHU */) const
{
	// 首先判断矩阵是否对称
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

	// 将L的上三角部分置0
	for (i = 1; i < mL.m_nRow; i++)
	{
		for (j = i + 1; j < mL.m_nCol + 1; j++)
		{
			mL(i, j) = 0.0;
		}
	}

	return mL;
}


// 对对称正定矩阵实施Cholesky分解 A = UU'，返回对角线上元素全为正的上三角矩阵U，此函数也用来判断对称矩阵是否正定，分解成功则返回的矩阵对象m_pMat不为NULL，否则为NULL
CMatrix DeepVoid::CMatrix::Chol_Upper(double eps /* = EPSILON_ZHU */) const
{
	CMatrix mL = Chol_Lower(eps);

	// 若mL为Null说明Cholesky分解失败，返回CMatrix(0)
	if (mL.IsNull())
	{
		return mL;
	}

	// 若mL不为Null说明 A = LL'分解成功，接着利用RQ分解将L分解成 L = UQ，U为上三角矩阵，Q为正交矩阵
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
// 尚总的解方程及矩阵操作相关 /////////////////////////////////////////////////////////////////////////////////
// 求解超定线性方程组(利用伪逆求最小二乘解)

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

// 对称正定阵的求逆（结果还是对称正定阵）-- 变量循环重新编号法,对称阵不优化存储
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

// 一般矩阵相乘
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

// 矩阵转置乘原矩阵，得到对称阵,对称阵不优化存储
// 对于最小二乘法方程问题，当m >= n，当A的全部n阶子行列式值不全为零时，ATA为正定
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

// 矩阵转置
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

// 矩阵求逆
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

//矩阵奇异值分解
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
	if(fabs(a[(kk-1)*n+kk-1]) < 0.000000001)  //9月10日改
	{
		a[(kk-1)*n+kk-1] = 0.000000001;   //9月10日改
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
	if(fabs(e[kk]) < 0.000000001) //9月10日改
	{
		e[kk] = 0.000000001; //9月10日改
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
	if(fabs(u[iz]) < 0.000000001)  //9月10日改
	{
		u[iz] = 0.000000001;       //9月10日改
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
	if(fabs(v[iz]) < 0.000000001) //9月10日改
	{
		v[iz] = 0.000000001;   //9月10日改
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
	if(fabs(d) < 0.000000001)  //9月10日改
	{
		d = 0.000000001;   //9月10日改
	}
	sm=s[mm-1]/d; sm1=s[mm-2]/d;
	em1=e[mm-2]/d;
	sk=s[kk-1]/d; ek=e[kk-1]/d;
	b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
	c=sm*em1; c=c*c; shh=0.0;
	if ((b!=0.0)||(c!=0.0))
	{ shh=sqrt(b*b+c);
	if (b<0.0) shh=-shh;
	//	shh=c/(b+shh);  //9月10日改
	double mm = b+shh;   //9月10日改
	if(fabs(mm) < 0.000000001)   //9月10日改
	{
		mm = 0.000000001;   //9月10日改
	}
	shh=c/mm;  //9月10日改
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
if(fabs(d) < 0.000000001)  //9月10日改
{
	d = 0.000000001;   //9月10日改
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

// 对 1 维数组进行冒泡法排序，可以升序排列也可降序排列，并输出原数组各元素索引在排完序的新数组中的排列
void DeepVoid::Sort_Bubble(double * p,					// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按升序或者降序排列
						   int n,						// 输入：数组长度，元素个数
						   int * idx,					// 输出：排序完成后各元素原来的索引号的排列情况，索引从 0 开始
						   BOOL bIsAscent /*= TRUE*/	// 输入：是升序，还是降序排列
						   )
{
	int m , k, j, i;
	for( i=0; i<n; i++ )			//预定义索引数组
	{
		idx[i]=i;       
	}

	if(bIsAscent)
	{
		k = 0;
		m = n-1;
		while ( k < m)// k>＝m说明已经排好
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{				
				/*if (p[idx[i]] > p[idx[i+1]] )*/
				if (p[i] > p[i+1])
				{ 
					Swap_double(p[i], p[i+1]);        // 交换数组中值
					Swap_int(idx[i], idx[i+1]);    // 交换索引数组中的索引
					m = i;// m向后都是升序排列
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
					k = i;// k向前都是升序排列
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

// 对 1 维数组根据输入的索引来进行重排列
void DeepVoid::Sort_byIndex_double(double * p,	// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按 idx 中的索引来进行重排列
								   int n,		// 输入：数组长度，元素个数
								   int * idx	// 输入：索引值，函数就是根据这个索引参数中的索引来对数组中的各元素进行重排列的，索引从 0 开始
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

// 对 1 维数组根据输入的索引来进行重排列
void DeepVoid::Sort_byIndex_int(int * p,	// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按 idx 中的索引来进行重排列
								int n,		// 输入：数组长度，元素个数
								int * idx	// 输入：索引值，函数就是根据这个索引参数中的索引来对数组中的各元素进行重排列的，索引从 0 开始
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

// 对 2 维数组的每一列按某一行的数据进行升序或者降序排列
void DeepVoid::Sort2_byRow(double ** p,					// 输入兼输出：待排序的 2 维数组
						   int w, int h,				// 输入：2 维数组的宽高
						   int row /*= 0*/,				// 输入：按某一行的数据对所有列进行排序，默认为第 1 行
						   BOOL bIsAscent /*= TRUE*/	// 输入：按升序还是降序排列，默认为升序
						   )
{
	int m , k, j, i, ii;
	
	if(bIsAscent)
	{
		k = 0;
		m = w-1;
		while ( k < m)// k>＝m说明已经排好
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{				
				if (p[row][i] > p[row][i+1])
				{ 
					for (ii = 0; ii < h; ii++)
					{
						Swap_double(p[ii][i], p[ii][i+1]);        // 交换数组中值
					}
					m = i;// m向后都是升序排列
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
					k = i;// k向前都是升序排列
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

// 对 2 维数组的每一行按某一列的数据进行升序或者降序排列
void DeepVoid::Sort2_byCol(double ** p,					// 输入兼输出：待排序的 2 维数组
						   int w, int h,				// 输入：2 维数组的宽高
						   int col /*= 0*/,				// 输入：按某一列的数据对所有行进行排序，默认为第 1 行
						   BOOL bIsAscent /*= TRUE*/	// 输入：按升序还是降序排列，默认为升序
						   )
{
	int m , k, j, i, ii;
	
	if(bIsAscent)
	{
		k = 0;
		m = h-1;
		while ( k < m)// k>＝m说明已经排好
		{ 	
			j = m-1; 
			m = 0;
			for ( i = k; i <= j; i++)
			{				
				if (p[i][col] > p[i+1][col])
				{ 
					for (ii = 0; ii < w; ii++)
					{
						Swap_double(p[i][ii], p[i+1][ii]);        // 交换数组中值	
					}
					m = i;// m向后都是升序排列
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
					k = i;// k向前都是升序排列
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

// 找到并返回 1 维数组中的最小值，并输出最小值在数组中的索引
double DeepVoid::FindArrayMin(double * p,						// 输入：待考察的 1 维数组
							  int n,							// 输入：数组的长度
							  int & idx,						// 输出：找到的数组最小值的索引，从 0 开始
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

// 找到并返回 1 维数组中的最大值，并输出最大值在数组中的索引
double DeepVoid::FindArrayMax(double * p,	// 输入：待考察的 1 维数组
							  int n,		// 输入：数组的长度
							  int & idx		// 输出：找到的数组最大值的索引，从 0 开始
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

// 找到并返回 1 维数组中的最大值，并输出最大值在数组中的索引
BYTE DeepVoid::FindArrayMax_BYTE(BYTE * p,		// 输入：待考察的 1 维数组
								 int n,			// 输入：数组的长度
								 int & idx		// 输出：找到的数组最大值的索引，从 0 开始
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

// 找到并返回 1 维数组中的最大值，并输出最大值在数组中的索引
int DeepVoid::FindArrayMax_int(int * p,		// 输入：待考察的 1 维数组
							   int n,		// 输入：数组的长度
							   int & idx	// 输出：找到的数组最大值的索引，从 0 开始
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

// 两个 double 型的变量互换数值
void DeepVoid::Swap_double(double & A, double & B)
{
	double temp = B;
	B = A;
	A = temp;
}

// 两个 int 型的变量互换数值
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


	str.Format("Distortion Coefficients\n");
	for (i=0;i<5;i++)
	{
		strtmp.Format("%.12f	", cam.k[i]);
		str += strtmp;
	}
	str += "\n\n";
	fprintf(file, str);


	str.Format("Rotation Matrix\n");
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			strtmp.Format("%.12f	", cam.R[i*3+j]);
			str += strtmp;
		}
		str += "\n";
	}
	str += "\n";
	fprintf(file, str);


	str.Format("Translation Vector\n");
	for (i=0;i<3;i++)
	{
		strtmp.Format("%.12f	", cam.t[i]);
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