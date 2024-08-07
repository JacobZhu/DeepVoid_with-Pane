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

CString DeepVoid::GetFolderPath(CString filePath)
{
	CString pathFolder = filePath.Left(filePath.ReverseFind('\\')) + '\\';

	return pathFolder;
}

bool DeepVoid::ReadinEPFLCamera(CString filePath, Matx33d & K, Matx31d & dist, Matx33d & R, Matx31d & t)
{
	FILE * file = fopen(filePath, "r");

	if (file)
	{
		K = Matx33d::zeros();
		R = Matx33d::zeros();
		t = Matx31d::zeros();
		dist = Matx31d::zeros();

		fscanf(file, "%lf", &K(0, 0)); fscanf(file, "%lf", &K(0, 1)); fscanf(file, "%lf", &K(0, 2));
		fscanf(file, "%lf", &K(1, 0)); fscanf(file, "%lf", &K(1, 1)); fscanf(file, "%lf", &K(1, 2));
		fscanf(file, "%lf", &K(2, 0)); fscanf(file, "%lf", &K(2, 1)); fscanf(file, "%lf", &K(2, 2));

		fscanf(file, "%lf", &dist(0)); fscanf(file, "%lf", &dist(1)); fscanf(file, "%lf", &dist(2));

		fscanf(file, "%lf", &R(0, 0)); fscanf(file, "%lf", &R(0, 1)); fscanf(file, "%lf", &R(0, 2));
		fscanf(file, "%lf", &R(1, 0)); fscanf(file, "%lf", &R(1, 1)); fscanf(file, "%lf", &R(1, 2));
		fscanf(file, "%lf", &R(2, 0)); fscanf(file, "%lf", &R(2, 1)); fscanf(file, "%lf", &R(2, 2));

		fscanf(file, "%lf", &t(0)); fscanf(file, "%lf", &t(1)); fscanf(file, "%lf", &t(2));

		R = R.t();
		t = -R*t;

		fclose(file);

		return true;
	}
	else
	{
		return false;
	}
}

CString DeepVoid::GetFileNameNoSuffix(CString filePath)
{
	// 先把带尾缀的文件名给提取出来
	CString fileNameWithSuffix = GetFileNameOnly(filePath);

	// 再把尾缀给去掉
//	int index = fileNameWithSuffix.GetLength() - fileNameWithSuffix.ReverseFind('.') - 1;

	CString fileNameNoSuffix = fileNameWithSuffix.Left(fileNameWithSuffix.ReverseFind('.'));
	
	return fileNameNoSuffix;
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

// 20220207
bool DeepVoid::optim_lm_hi_ai_bi(const vector<Point2d> & xys,			// 输入：参考图像中各参考像素的坐标
								 const vector<Vec3d> & RGBs,			// 输入：参考图像中各参考像素的RBG值，double型，[0]:R，[1]:G，[2]:B
								 const Mat & img,						// 输入：匹配图像
								 Matx<double, 8, 1> & x,				// 输入兼输出：最小二乘图像匹配参数
								 int & code,							// 输出：迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出
								 int IRLS /*= 0*/,						// 输入：是否进行迭代重加权 0：否；1：Huber；2：...
								 double e_Huber /*= 50*/,				// 输入：Huber IRLS 的阈值
								 int maxIter /*= 64*/,					// 输入：最大迭代次数
								 double tau /*= 1.0E-3*/,				// 输入：The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								 double eps1 /*= 1.0E-8*/,				// 输入：梯度收敛阈值
								 double eps2 /*= 1.0E-12*/				// 输入：改正量收敛阈值
								 )
{
	int k = 0;			// 迭代次数索引
	int v = 2;			// 更新 u 时需要用到的一个控制量      
	double u;			// LM 优化算法中最关键的阻尼系数 (J'WJ + uI)h = -J'Wf = -g
	double r;			// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;		// 梯度的模
	double dx_norm;		// 改正量的模
	double dx_thresh;	// 改正量收敛判断阈值 eps2*(norm(x)+eps2)
	double F, F_new;	// 目标函数值 0.5*ft*covInv*f 或者 0.5*f'Wf
	double x_norm;		// 当前待优化参数向量的模，即2范数L2，||x||2
	double L0_Lh;		// 泰勒展开式的函数值下降量

	double ratio_1_3 = 1.0 / 3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	code = 2;			// 迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出

	Matx<double, 8, 1> g, g_new, x_new;
	Matx<double, 8, 8> H, H_new;

	vector<Matx31d> fs;

	if (!derivatives::H_g_hi_ai_bi(xys, RGBs, img, x, H, g, F, fs, IRLS, e_Huber))
	{
		code = 3;
		return false;	// 大概率是有像素越界了，直接退出
	}
	
	x_norm = norm(x);	// 当前参数向量 2 范数
	g_norm = norm(g, NORM_INF);	// 目标函数值 F 在当前参数向量处梯度向量的 INF 范数

	// 梯度收敛，说明已在平坦区域，直接退出
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
		return true;
	}


	// 确定一个合理的初始阻尼系数 u ////////////////////////////////////////////
	vector<double> Aii(8);

	for (int i = 0; i < 8; ++i)
	{
		Aii[i] = H(i, i);
	}

	auto iter = max_element(Aii.begin(), Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu
	//////////////////////////////////////////////////////////////////////////


	while (!found && k < maxIter)
	{
		++k;

		Matx<double, 8, 8> H_uI = H;

		// 加阻尼系数
		for (int i = 0; i < 8; ++i)
		{
			H_uI(i, i) += u;
		}

		Matx<double, 8, 1> dx;

		// 解方程 (JWJ + uI)*dx = -g 得到 P 的改正量
		solve(H_uI, -g, dx, DECOMP_CHOLESKY);

		dx_norm = norm(dx);

		dx_thresh = eps2*(x_norm + eps2);	// 根据当前待优化参数向量的模来确定改正量大小是否满足收敛条件

		if (dx_norm < dx_thresh)
		{
			found = true;
			code = 1;
		}
		else
		{
			x_new = x + dx;	// 暂时先更新一下，录不录用还得

			if (!derivatives::H_g_hi_ai_bi(xys, RGBs, img, x_new, H_new, g_new, F_new, fs, IRLS, e_Huber))
			{
				code = 3;
				return false;	// 大概率是有像素越界了，直接退出
			}

			Matx<double, 1, 1> tmp = 0.5*dx.t()*(u*dx - g);
			L0_Lh = tmp(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (F - F_new) / L0_Lh;

			if (r > 0)
			{
				// 采纳新参数
				x = x_new;

				// 一并采纳新参数处的 Hessian 矩阵和梯度向量
				H = H_new;
				g = g_new;

				// 还采纳新参数处的目标函数值
				F = F_new;

				// 采纳已经计算出来的新参数向量的模
				x_norm = norm(x);
				g_norm = norm(g, NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			}
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	return true;
}

// 20220207
bool DeepVoid::optim_gn_hi_ai_bi(const vector<Point2d> & xys,				// 输入：参考图像中各参考像素的坐标
							     const vector<Vec3d> & RGBs,				// 输入：参考图像中各参考像素的RBG值，double型，[0]:R，[1]:G，[2]:B
							     const Mat & img,							// 输入：匹配图像
							     Matx<double, 8, 1> & x,					// 输入兼输出：最小二乘图像匹配参数
							     int IRLS /*= 0*/,							// 输入：是否进行迭代重加权 0：否；1：Huber；2：...
							     double e_Huber /*= 30*/,					// 输入：Huber IRLS 的阈值
							     int maxIter /*= 128*/,						// input:	max iteration
							     double xEps /*= 1.0E-12*/,					// input:	threshold
							     double fEps /*= 1.0E-12*/					// input:	threshold
							     )
{
	Matx<double, 8, 8> H;
	Matx<double, 8, 1> g, dx;
	double F;
	vector<Matx31d> fs;
	double F_old = 0;

	for (int k = 0; k < maxIter; ++k)
	{
		if (!derivatives::H_g_hi_ai_bi(xys, RGBs, img, x, H, g, F, fs, IRLS, e_Huber))
		{
			return false;	// 大概率是有像素越界了，直接退出
		}

		// 解方程 (JWJ + uI)*dx = -g 得到 P 的改正量
		solve(H, -g, dx, DECOMP_CHOLESKY);

		double df2 = F - F_old;

		if ((fabs(df2) < fEps) || (norm(dx) < xEps))
		{
			break;
		}

		F_old = F;

		x += dx;
	}

	return true;
}

// 20220207
void DeepVoid::optim_lm_a_w_t(const vector<Point3d> & pt3d0s,		// 输入：参考图像中各参考像素的坐标
							  const vector<Point3d> & pt3d1s,		// 输入：参考图像中各参考像素的RBG值，double型，[0]:R，[1]:G，[2]:B
							  double & a,
							  Matx33d & R,
							  Matx31d & t,
							  double * info /*= NULL*/,				//int & code,	// 输出：迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出										// int & code,// 输出：迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出
							  int maxIter /*= 128*/,				// 输入：最大迭代次数
							  double tau /*= 1.0E-6*/,				// 输入：The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  double eps1 /*= 1.0E-8*/,				// 输入：梯度收敛阈值
							  double eps2 /*= 1.0E-12*/				// 输入：改正量收敛阈值
						      )
{
	int n = pt3d0s.size();

	int k = 0;			// 迭代次数索引
	int v = 2;			// 更新 u 时需要用到的一个控制量      
	double u;			// LM 优化算法中最关键的阻尼系数 (J'WJ + uI)h = -J'Wf = -g
	double r;			// gain ratio, 增益率，用来衡量近似展开式的好坏
	double g_norm;		// 梯度的模
	double dx_norm;		// 改正量的模
//	double dx_thresh;	// 改正量收敛判断阈值 eps2*(norm(x)+eps2)
	double F, F_new;	// 目标函数值 0.5*ft*covInv*f 或者 0.5*f'Wf
//	double x_norm;		// 当前待优化参数向量的模，即2范数L2，||x||2
	double L0_Lh;		// 泰勒展开式的函数值下降量

	double ratio_1_3 = 1.0 / 3.0;

	bool found = false; // 标识是否已经满足迭代收敛条件
	int code = 2;			// 迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出

	Matx<double, 7, 1> g, g_new, x_new;
	Matx<double, 7, 7> H, H_new;

	vector<Matx31d> fs;

	vector<Matx31d> X0s, X1s;
	for (int i = 0; i < n; ++i)
	{
		Point3d pt0 = pt3d0s[i];
		Point3d pt1 = pt3d1s[i];

		Matx31d X0, X1;

		X0(0) = pt0.x;
		X0(1) = pt0.y;
		X0(2) = pt0.z;

		X1(0) = pt1.x;
		X1(1) = pt1.y;
		X1(2) = pt1.z;

		X0s.push_back(X0);
		X1s.push_back(X1);
	}

	derivatives::H_g_a_w_t(X0s, X1s, a, R, t, H, g, F, fs);

	double err_rpj_init = sqrt(2 * F / n);

	g_norm = norm(g, NORM_INF);	// 目标函数值 F 在当前参数向量处梯度向量的 INF 范数

	// 梯度收敛，说明已在平坦区域，直接退出
	if (g_norm < eps1)
	{
		found = true;
		code = 0;
	}

	// 确定一个合理的初始阻尼系数 u ////////////////////////////////////////////
	vector<double> Aii(7);

	for (int i = 0; i < 7; ++i)
	{
		Aii[i] = H(i, i);
	}

	auto iter = max_element(Aii.begin(), Aii.end());
	double max_Aii = *iter;

	u = tau * max_Aii; // initial miu
	//////////////////////////////////////////////////////////////////////////


	while (!found && k < maxIter)
	{
		++k;

		Matx<double, 7, 7> H_uI = H;

		// 加阻尼系数
		for (int i = 0; i < 7; ++i)
		{
			H_uI(i, i) += u;
		}

		Matx<double, 7, 1> dx;

		// 解方程 (JWJ + uI)*dx = -g 得到 P 的改正量
		solve(H_uI, -g, dx, DECOMP_CHOLESKY);

		dx_norm = norm(dx);

		if (dx_norm < eps2)
		{
			found = true;
			code = 1;
		}
		else
		{
			double da = dx(0);
			double dw0 = dx(1);
			double dw1 = dx(2);
			double dw2 = dx(3);
			double dtX = dx(4);
			double dtY = dx(5);
			double dtZ = dx(6);

			double a_new = a + da;

			Matx33d dR = calib::converse_rotvec_R(dw0, dw1, dw2);

			Matx33d R_new = dR*R;

			Matx31d t_new;

			t_new(0) = t(0) + dtX;
			t_new(1) = t(1) + dtY;
			t_new(2) = t(2) + dtZ;

			derivatives::H_g_a_w_t(X0s, X1s, a_new, R_new, t_new, H_new, g_new, F_new, fs);

			Matx<double, 1, 1> tmp = 0.5*dx.t()*(u*dx - g);
			L0_Lh = tmp(0); // 在当前参数处利用梯度和改正量预估的期望目标函数下降量

			r = (F - F_new) / L0_Lh;

			if (r > 0)
			{
				// 采纳新参数
				a = a_new;
				R = R_new;
				t = t_new;

				// 一并采纳新参数处的 Hessian 矩阵和梯度向量
				H = H_new;
				g = g_new;

				// 还采纳新参数处的目标函数值
				F = F_new;

				// 采纳已经计算出来的新参数向量的模
				g_norm = norm(g, NORM_INF);

				if (g_norm < eps1) // 梯度收敛，说明抵达平坦区域
				{
					found = true;
					code = 0;
				}

				double tmp_db = std::max(ratio_1_3, 1 - pow(2 * r - 1, 3));
				u *= tmp_db;
				v = 2;
			}
			else
			{
				u *= v;
				v *= 2;
			}
		}
	}

	double err_rpj_final = sqrt(2 * F / n);

	if (info)
	{
		info[0] = err_rpj_init;
		info[1] = err_rpj_final;
		info[2] = g_norm;
		info[3] = k;
		info[4] = code;
	}
}

// 20220207，最小二乘图像匹配优化
bool DeepVoid::LSM(int x0, int y0,				// 输入：参考像点坐标
				   const Mat & img0,			// 输入：参考图像
				   double & x, double & y,		// 输入兼输出：匹配像点坐标
				   const Mat & img,				// 输入：匹配图像
				   int wndSize,					// 输入：窗口大小
				   int & code,					// 输出：迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出
				   int method /*= 0*/,			// 输入：0:LM；1:GN
				   int IRLS /*= 0*/,			// 输入：是否进行迭代重加权 0：否；1：Huber；2：...
				   double e_Huber /*= 50*/,		// 输入：Huber IRLS 的阈值
				   int maxIter /*= 64*/,		// 输入：最大迭代次数
				   double tau /*= 1.0E-3*/,		// 输入：The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
				   double eps1 /*= 1.0E-8*/,	// 输入：梯度收敛阈值
				   double eps2 /*= 1.0E-12*/,	// 输入：改正量收敛阈值
				   double xEps /*= 1.0E-12*/,	// 输入：GN法迭代退出阈值
				   double fEps /*= 1.0E-12*/	// 输入：GN法迭代退出阈值
				   )
{
	int nc = img0.channels();

	int hSize = (wndSize - 1)*0.5;

	if (nc == 3) // 彩色图像
	{
		vector<Point2d> xys;
		vector<Vec3d> RGBs;

		for (int i = -hSize; i <= hSize; ++i)
		{
			for (int j = -hSize; j <= hSize; ++j)
			{
				int xx = x0 + j;
				int yy = y0 + i;

				Point2d xy;
				xy.x = xx;
				xy.y = yy;

				Vec3b pix = img0.at<Vec3b>(yy, xx);
				uchar B = pix.val[0];
				uchar G = pix.val[1];
				uchar R = pix.val[2];

				Vec3d I;
				I[0] = R;
				I[1] = G;
				I[2] = B;

				xys.push_back(xy);
				RGBs.push_back(I);
			}
		}

		Matx<double, 8, 1> params;
		params(0) = 0;		// h0 = 0
		params(1) = 1;		// h1 = 1
		params(2) = x - x0;	// a0 = x' - x
		params(3) = 1;		// a1 = 1
		params(4) = 0;		// a2 = 0
		params(5) = y - y0;	// b0 = y' - y
		params(6) = 0;		// b1 = 0
		params(7) = 1;		// b2 = 1

		if (method == 0)
		{
			if (!optim_lm_hi_ai_bi(xys, RGBs, img, params, code, IRLS, e_Huber, maxIter, tau, eps1, eps2))
			{
				return false;
			}
		} 
		else
		{
			if (!optim_gn_hi_ai_bi(xys, RGBs, img, params, IRLS, e_Huber, maxIter, xEps, fEps))
			{
				return false;
			}
		}

		double a0 = params(2);
		double a1 = params(3);
		double a2 = params(4);

		double b0 = params(5);
		double b1 = params(6);
		double b2 = params(7);

		// 更新匹配像点坐标
		x = a0 + a1*x0 + a2*y0;
		y = b0 + b1*x0 + b2*y0;

		return true;		
	} 
	else // 灰度图像
	{
		return false; // 针对灰度图像还没实现
	}
}

// 20220208，最小二乘图像匹配优化
bool DeepVoid::LSM(int x0, int y0,				// 输入：参考像点坐标
				   const Mat & img0,			// 输入：参考图像
				   const Matx33d & K0,			// 输入：参考图像的内参数矩阵
				   const Matx33d & R0,			// 输入：参考图像的旋转矩阵
				   const Matx31d & t0,			// 输入：参考图像的平移向量
				   double & x1, double & y1,	// 输入兼输出：匹配像点坐标
				   const Mat & img1,			// 输入：匹配图像
				   const Matx33d & K1,			// 输入：参考图像的内参数矩阵
				   const Matx33d & R1,			// 输入：参考图像的旋转矩阵
				   const Matx31d & t1,			// 输入：参考图像的平移向量
				   int wndSize,					// 输入：窗口大小
				   int & code,					// 输出：迭代终止条件: 0:梯度收敛; 1:改正量大小收敛；2:超过最大迭代次数；3:遭遇重大问题（像素越界）导致迭代直接终止退出
				   int method /*= 0*/,			// 输入：0:LM；1:GN
				   int IRLS /*= 0*/,			// 输入：是否进行迭代重加权 0：否；1：Huber；2：...
				   double e_Huber /*= 50*/,		// 输入：Huber IRLS 的阈值
				   int maxIter /*= 64*/,		// 输入：最大迭代次数
				   double tau /*= 1.0E-3*/,		// 输入：The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
				   double eps1 /*= 1.0E-8*/,	// 输入：梯度收敛阈值
				   double eps2 /*= 1.0E-12*/,	// 输入：改正量收敛阈值
				   double xEps /*= 1.0E-12*/,	// 输入：GN法迭代退出阈值
				   double fEps /*= 1.0E-12*/	// 输入：GN法迭代退出阈值
				   )
{
	// 先利用参考像点和初始匹配像点前向交会出物点坐标 /////////////////////////////////////////////////
	double d;
	double hx = 0;
	double hy = 0;

	Matx31d C0;	// 参考图像光心坐标

	{
		vector<Point2d> imgpts0, imgpts1, errs;
		vector<Point3d> wrdpts;
		Point2d pt0, pt1;
		pt0.x = x0;
		pt0.y = y0;
		pt1.x = x1;
		pt1.y = y1;
		imgpts0.push_back(pt0);
		imgpts1.push_back(pt1);

		double rpj_err = Triangulate_Optimal(imgpts0, K0, R0, t0, imgpts1, K1, R1, t1, wrdpts, errs);

		Point3d pt3d = wrdpts[0];

		Matx31d X;
		X(0) = pt3d.x;
		X(1) = pt3d.y;
		X(2) = pt3d.z;

		C0 = -R0.t()*t0;

		Matx31d X_C = R0*X + t0;	// 物点在参考图像中的坐标
		d = X_C(2);			// 物点相对于参考图像的深度值
	}
	///////////////////////////////////////////////////////////////////////////////////////////////


	int nc = img0.channels();

	int hSize = (wndSize - 1)*0.5;

	if (nc == 3) // 彩色图像
	{
		vector<Point2d> pts0, pts1;
		vector<Vec3d> RGBs;

		Matx33d R0tK0Inv = R0.t()*K0.inv();
		Matx33d K1R1 = K1*R1;
		Matx31d K1t1 = K1*t1;

		for (int i = -hSize; i <= hSize; ++i)
		{
			for (int j = -hSize; j <= hSize; ++j)
			{
				int xx = x0 + j;
				int yy = y0 + i;

				Point2d pt0;
				pt0.x = xx;
				pt0.y = yy;

				Vec3b pix = img0.at<Vec3b>(yy, xx);
				uchar B = pix.val[0];
				uchar G = pix.val[1];
				uchar R = pix.val[2];

				Vec3d I;
				I[0] = R;
				I[1] = G;
				I[2] = B;

				pts0.push_back(pt0);
				RGBs.push_back(I);

				Matx31d xy1;
				xy1(0) = xx;
				xy1(1) = yy;
				xy1(2) = 1;

				Matx31d dir = R0tK0Inv*xy1;

				Matx31d X = C0 + (d + hx*j + hy*i) * dir;

				Matx31d xyw = K1R1*X + K1t1;

				Point2d pt1;
				pt1.x = xyw(0) / xyw(2);
				pt1.y = xyw(1) / xyw(2);

				pts1.push_back(pt1);
			}
		}

		double a0, a1, a2, b0, b1, b2;

		derivatives::compute_affine_2D(pts0, pts1, a0, a1, a2, b0, b1, b2);

		Matx<double, 8, 1> params;
		params(0) = 0;		// h0 = 0
		params(1) = 1;		// h1 = 1
		params(2) = a0/*x - x0*/;	// a0 = x' - x
		params(3) = a1/*1*/;		// a1 = 1
		params(4) = a2/*0*/;		// a2 = 0
		params(5) = b0/*y - y0*/;	// b0 = y' - y
		params(6) = b1/*0*/;		// b1 = 0
		params(7) = b2/*1*/;		// b2 = 1

		if (method == 0)
		{
			if (!optim_lm_hi_ai_bi(pts0, RGBs, img1, params, code, IRLS, e_Huber, maxIter, tau, eps1, eps2))
			{
				return false;
			}
		}
		else
		{
			if (!optim_gn_hi_ai_bi(pts0, RGBs, img1, params, IRLS, e_Huber, maxIter, xEps, fEps))
			{
				return false;
			}
		}

		a0 = params(2);
		a1 = params(3);
		a2 = params(4);

		b0 = params(5);
		b1 = params(6);
		b2 = params(7);

		// 更新匹配像点坐标
		x1 = a0 + a1*x0 + a2*y0;
		y1 = b0 + b1*x0 + b2*y0;

		return true;
	}
	else // 灰度图像
	{
		return false; // 针对灰度图像还没实现
	}
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

	// 确定当前像点所在的具体像素坐标
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

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			int I = img.at<uchar>(i, j);

			m00 += I;	// m00 = sum(I)
			m10 += /*j*/dj*I; // m10 = sum(xI)
			m01 += /*i*/di*I; // m01 = sum(yI)
		}
	}

	// 区域内的图像灰度全黑，即全为 0，则有可能出现 m00 仍为 0 的情况
	if (m00 == 0)
	{
		return false;
	}

	double m00_1 = 1.0 / m00;

	xIC = m00_1*m10;
	yIC = m00_1*m01;

	return true;
}

// 20200824，计算一圆形支持区域内的图像灰度质心，通过相对于中心像素坐标偏移的方式表示
bool DeepVoid::IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
											    int ix, int iy,					// input: the center of the region
											    int r,							// input: the radius of the circular region
											    double & dxIC, double & dyIC,	// output:the location of the calculated intensity centroid (in terms of offsets)
												double * pm00_1/* = NULL*/		// output:1/m00
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

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			int I = img.at<uchar>(i, j);

			m00 += I;		// m00 = sum(I)
			m10 += dj*I;	// m10 = sum(xI)
			m01 += di*I;	// m01 = sum(yI)
		}
	}

	// 区域内的图像灰度全黑，即全为 0，则有可能出现 m00 仍为 0 的情况
	if (m00 == 0)
	{
		return false;
	}

	double m00_1 = 1.0 / m00;

	dxIC = m00_1*m10;
	dyIC = m00_1*m01;

	if (pm00_1)
	{
		(*pm00_1) = m00_1;
	}

	return true;
}

// 20200824，计算一圆形支持区域内的图像灰度质心，通过相对于中心像素坐标偏移的方式表示
// 20231018，输出图像矩moments: m00, m10, m01
bool DeepVoid::IntensityCentroid_CircularRegion_moments(const cv::Mat & img,						// input: the input gray scale image
														int ix, int iy,								// input: the center of the region
														int r,										// input: the radius of the circular region
														double & dxIC, double & dyIC,				// output:the location of the calculated intensity centroid (in terms of offsets)
														double & m00, double & m10, double & m01	// output:1/m00
														)
{
	int w = img.cols;
	int h = img.rows;

	m00 = 0;
	m10 = 0;
	m01 = 0;

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

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			int I = img.at<uchar>(i, j);

			m00 += I;		// m00 = sum(I)
			m10 += dj*I;	// m10 = sum(xI)
			m01 += di*I;	// m01 = sum(yI)
		}
	}

	// 区域内的图像灰度全黑，即全为 0，则有可能出现 m00 仍为 0 的情况
	if (m00 == 0)
	{
		return false;
	}

	double m00_1 = 1.0 / m00;

	dxIC = m00_1*m10;
	dyIC = m00_1*m01;

	return true;
}

// 20200824，计算一圆形支持区域内的图像灰度质心，通过相对于中心像素坐标偏移的方式表示
// 20201205，给定图像灰度高斯随机噪声的标准差，输出计算得到的灰度质心坐标不确定度标准差
bool DeepVoid::IntensityCentroid_CircularRegion(const cv::Mat & img,			// input: the input gray scale image
											    int ix, int iy,					// input: the center of the region
											    int r,							// input: the radius of the circular region
											    double & dxIC, double & dyIC,	// output:the location of the calculated intensity centroid (in terms of offsets)
											    double sigma_I,					// input: the standard deviation of the Gaussian random noise of image intensity
											    double & sigma_xIC, double & sigma_yIC	// output: the standard deviation of the x y coordinates of the intensity centroid propagated by the random noise of intensity
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

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			int I = img.at<uchar>(i, j);

			m00 += I;		// m00 = sum(I)
			m10 += dj*I;	// m10 = sum(xI)
			m01 += di*I;	// m01 = sum(yI)
		}
	}

	// 区域内的图像灰度全黑，即全为 0，则有可能出现 m00 仍为 0 的情况
	if (m00 == 0)
	{
		return false;
	}

	double m00_1 = 1.0 / m00;

	dxIC = m00_1*m10;
	dyIC = m00_1*m01;

	// 20201205，开始计算误差传递
	double dxIC_1 = 1.0 / dxIC;
	double z = dyIC*dxIC_1; // z=dy/dx
	double dang_dz = 1.0 / (1 + z*z); // da/dz=1/(1+z^2)
	double sigma2_I = sigma_I*sigma_I;
	double sigma2_xIC = 0;
	double sigma2_yIC = 0;
	double sigma2_ang = 0;
	for (int di = -r; di <= r; ++di)
	{
		int i = iy + di;		

		if (i < 0 || i >= h)
		{
			continue;
		}

		int di2 = di*di;

		double ayc_aIk = (di - dyIC)*m00_1;
		double yk2 = ayc_aIk*ayc_aIk;

		for (int dj = -r; dj <= r; ++dj)
		{
			int j = ix + dj;

			if (j < 0 || j >= w)
			{
				continue;
			}

			int dj2 = dj*dj;

			double axc_aIk = (dj - dxIC)*m00_1;
			double xk2 = axc_aIk*axc_aIk;

			double rr = std::sqrt(di2 + dj2);

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			sigma2_xIC += xk2*sigma2_I;
			sigma2_yIC += yk2*sigma2_I;

			double dang_dIk = dang_dz*dxIC_1*(ayc_aIk - axc_aIk*z);

			sigma2_ang += dang_dIk*dang_dIk*sigma2_I;
		}
	}

	sigma_xIC = std::sqrt(sigma2_xIC);
	sigma_yIC = std::sqrt(sigma2_yIC);
	double sigma_IC = std::sqrt(sigma2_xIC + sigma2_yIC);

	double sigma_ang = std::sqrt(sigma2_ang)*R2D;

	return true;
}

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
bool DeepVoid::CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
							  int ix, int iy,		// input: the center of the region
							  int r,				// input: the radius of the circular region
							  double & angle		// output:the location of the calculated intensity centroid (in terms of offsets)
							  )
{
	double dx, dy;

	double sigma_xIC, sigma_yIC;

//	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy, 5, sigma_xIC, sigma_yIC)) // 说明图像区域内灰度值全为 0，即全黑
	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy)) // 说明图像区域内灰度值全为 0，即全黑
	{
		return false;
	}

	double radian = std::atan2(dy, dx); // [-π; +π]

	// 以下情况基本不会出现
// 	if (isnan(radian) || isinf(radian))
// 	{
// 		return false;
// 	}
	
	angle = radian*R2D;

	return true;
}

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 20201206，给定图像灰度高斯随机噪声的标准差，输出计算得到的特征方向的不确定度标准差
bool DeepVoid::CornerAngle_IC(const cv::Mat & img,		// input: the input gray scale image
							  int ix, int iy,			// input: the center of the region
							  int r,					// input: the radius of the circular region
							  double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
							  double & sigma_angle,		// output:the standard deviation of the corner angle propagated by the random noise of intensity
							  double sigma_I /*= 5.0*/	// input: the standard deviation of the Gaussian random noise of image intensity					
							  )
{
	double dx, dy;

	double sigma_xIC, sigma_yIC;

	double m00_1;

	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy, &m00_1)) // 说明图像区域内灰度值全为 0，即全黑
	{
		return false;
	}

	double radian = std::atan2(dy, dx); // [-π; +π]


	//////////////////////////////////////////////////////////////////////////
// 	if (fabs(dx) < 0.0000001)
// 	{
// 		double shitreallyhappens = 1;
// 	}
	//////////////////////////////////////////////////////////////////////////

	
	angle = radian*R2D; // 特征方向角度

	// 20201205，开始计算误差传递
	int w = img.cols;
	int h = img.rows;
	double dx_1 = 1.0 / dx;
	double z = dy*dx_1; // z=dy/dx
	double dang_dz = 1.0 / (1 + z*z); // da/dz=1/(1+z^2)
	double sigma2_I = sigma_I*sigma_I;
	double sigma2_radian = 0;

	for (int di = -r; di <= r; ++di)
	{
		int i = iy + di;

		if (i < 0 || i >= h)
		{
			continue;
		}

		int di2 = di*di;

		double ayc_aIk = (di - dy)*m00_1;

		for (int dj = -r; dj <= r; ++dj)
		{
			int j = ix + dj;

			if (j < 0 || j >= w)
			{
				continue;
			}

			int dj2 = dj*dj;

			double rr = std::sqrt(di2 + dj2);

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			double axc_aIk = (dj - dx)*m00_1;

			double dang_dIk = dang_dz*dx_1*(ayc_aIk - axc_aIk*z);

			sigma2_radian += dang_dIk*dang_dIk*sigma2_I;
		}
	}
	
	sigma_angle = std::sqrt(sigma2_radian)*R2D; // 特征方向角度不确定度标准差

	return true;
}

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 20201206，给定图像灰度高斯随机噪声的标准差，输出计算得到的特征方向的不确定度标准差
// 20231018，z=yc/xc=m01/m10，这样就不涉及到90°和270°时xc=0没法除的问题了，不用显式的求解xc和yc，只求m01和m10即可
bool DeepVoid::CornerAngle_IC_moments(const cv::Mat & img,		// input: the input gray scale image
									  int ix, int iy,			// input: the center of the region
									  int r,					// input: the radius of the circular region
									  double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
									  double & sigma_angle,		// output:the standard deviation of the corner angle propagated by the random noise of intensity
									  double sigma_I /*= 5.0*/	// input: the standard deviation of the Gaussian random noise of image intensity					
									  )
{
	double dx, dy;

//	double sigma_xIC, sigma_yIC;

//	double m00_1;
	double m00, m10, m01;

//	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy, &m00_1)) // 说明图像区域内灰度值全为 0，即全黑
	if (!IntensityCentroid_CircularRegion_moments(img, ix, iy, r, dx, dy, m00, m10, m01)) // 说明图像区域内灰度值全为 0，即全黑
	{
		return false;
	}

	double radian = std::atan2(dy, dx); // [-π; +π]
	angle = radian*R2D; // 特征方向角度

	double m10m01_1 = 1.0 / (m10*m10 + m01*m01); // 1 / (m10^2 + m01^2)

	//////////////////////////////////////////////////////////////////////////
// 	if (fabs(dx) < 0.0000001)
// 	{
// 		double shitreallyhappens = 1;
// 	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	if (isinf(m10m01_1)) // 20231019，能到这一步，说明区域内灰度并非全黑，但是区域内灰度关于中心完全对称，得到的灰度质心就严格位于中心位置，这样得到的方向0是不可靠的，为其赋一个极大的不确定度后退出
	{
//		double shitreallyhappens = 1;
		sigma_angle = 10000; // 这里附上一极大的不确定度值，以表示centroid就位于区域中心，没有所谓方向可言
		return true; // 但这里依然是返回了 true，相当于返回 false 单纯就表示 m00 为0，即区域内全黑，返回 true的话，具体什么情况就得看角度不确定度的情况了
	}
	//////////////////////////////////////////////////////////////////////////

	// 20201205，开始计算误差传递
	int w = img.cols;
	int h = img.rows;
//	double m10_1 = 1.0 / m10;
//	double z = m01*m10_1; // z = m01/m10
//	double dang_dz = 1.0 / (1 + z*z); // da/dz=1/(1+z^2)
	double sigma2_I = sigma_I*sigma_I;
	double sigma2_radian = 0;

	for (int di = -r; di <= r; ++di)
	{
		int i = iy + di;

		if (i < 0 || i >= h)
		{
			continue;
		}

		int di2 = di*di;

//		double ayc_aIk = (di - dy)*m00_1;
//		double am01_aIk = di; // am01/aIi = yi

		for (int dj = -r; dj <= r; ++dj)
		{
			int j = ix + dj;

			if (j < 0 || j >= w)
			{
				continue;
			}

			int dj2 = dj*dj;

			double rr = std::sqrt(di2 + dj2);

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

//			double axc_aIk = (dj - dx)*m00_1;
//			double am10_aIk = dj; // am10/aIi = xi

//			double dang_dIk = dang_dz*dx_1*(ayc_aIk - axc_aIk*z);
//			double dang_dIk = dang_dz*m10_1*(am01_aIk - am10_aIk*z);
			double dang_dIk = (m10*di - m01*dj)*m10m01_1;

			sigma2_radian += dang_dIk*dang_dIk*sigma2_I;
		}
	}
	
	sigma_angle = std::sqrt(sigma2_radian)*R2D; // 特征方向角度不确定度标准差

	return true;
}

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 20201206，给定图像灰度高斯随机噪声的标准差，输出计算得到的特征方向的不确定度标准差
// 20230703，采用质心点位置误差椭球相对于区域中心的张角作为方向角不确定度
bool DeepVoid::CornerAngle_IC_geometry(const cv::Mat & img,		// input: the input gray scale image
									   int ix, int iy,			// input: the center of the region
									   int r,					// input: the radius of the circular region
									   double & angle,			// output:the location of the calculated intensity centroid (in terms of offsets)
									   double & sigma_angle,	// output:the standard deviation of the corner angle propagated by the random noise of intensity
									   double sigma_I /*= 5.0*/	// input: the standard deviation of the Gaussian random noise of image intensity					
									   )
{
	double dx, dy;

	double sigma_xIC, sigma_yIC;

	double m00_1;

	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy, &m00_1)) // 说明图像区域内灰度值全为 0，即全黑
	{
		return false;
	}

	double radian = std::atan2(dy, dx); // [-π; +π]


	//////////////////////////////////////////////////////////////////////////
// 	if (fabs(dx) < 0.0000001)
// 	{
// 		double shitreallyhappens = 1;
// 	}
	//////////////////////////////////////////////////////////////////////////


	angle = radian*R2D; // 特征方向角度

	// 20201205，开始计算误差传递
	int w = img.cols;
	int h = img.rows;
	double dx_1 = 1.0 / dx;
	double z = dy*dx_1; // z=dy/dx
	double dang_dz = 1.0 / (1 + z*z); // da/dz=1/(1+z^2)
	double sigma2_I = sigma_I*sigma_I;
	double sigma2_radian = 0;

	// 20230703 //////////////////////////////////////////////////////////////
	double sigma2_yc = 0;
	double sigma2_xc = 0;
	double sigma_xcyc = 0;
	//////////////////////////////////////////////////////////////////////////

	for (int di = -r; di <= r; ++di)
	{
		int i = iy + di;

		if (i < 0 || i >= h)
		{
			continue;
		}

		int di2 = di*di;

		double ayc_aIk = (di - dy)*m00_1;

		// 20230703 //////////////////////////////////////////////////////////////
		double ayc_aIk_sigmaIk = ayc_aIk*sigma_I;
		double ayc_aIk_sigmaIk2 = ayc_aIk_sigmaIk*ayc_aIk_sigmaIk;
		//////////////////////////////////////////////////////////////////////////

		for (int dj = -r; dj <= r; ++dj)
		{
			int j = ix + dj;

			if (j < 0 || j >= w)
			{
				continue;
			}

			int dj2 = dj*dj;

			double rr = std::sqrt(di2 + dj2);

			if (rr > r) // 确保圆形区域
			{
				continue;
			}

			double axc_aIk = (dj - dx)*m00_1;

			double dang_dIk = dang_dz*dx_1*(ayc_aIk - axc_aIk*z);

			sigma2_radian += dang_dIk*dang_dIk*sigma2_I;

			// 20230703 //////////////////////////////////////////////////////////////
			double axc_aIk_sigmaIk = axc_aIk*sigma_I;

			sigma2_xc += axc_aIk_sigmaIk*axc_aIk_sigmaIk;
			sigma_xcyc += axc_aIk_sigmaIk*ayc_aIk_sigmaIk;
			sigma2_yc += ayc_aIk_sigmaIk2;
			//////////////////////////////////////////////////////////////////////////
		}
	}

	sigma_angle = std::sqrt(sigma2_radian)*R2D; // 特征方向角度不确定度标准差

	Matx22d cov;
	cov(0, 0) = sigma2_xc;
	cov(1, 1) = sigma2_yc;
	cov(0, 1) = cov(1, 0) = sigma_xcyc;

	Matx21d eigenVals;
	Matx22d eigenVecs;

	//	@param eigenvalues output vector of eigenvalues of the same type as src; the eigenvalues are stored in the descending order.
	//	@param eigenvectors output matrix of eigenvectors; it has the same size and type as src;
	//	the eigenvectors are stored as subsequent matrix rows, in the same order as the corresponding eigenvalues.
	cv::eigen(cov, eigenVals, eigenVecs);

	double axisLength_long = std::sqrt(eigenVals(0))/**0.5*/;													// 误差椭球的长轴长度
	double axisLength_short = std::sqrt(eigenVals(1))/**0.5*/;													// 误差椭球的短轴长度
	double axisLength_rms = std::sqrt(axisLength_long*axisLength_long + axisLength_short*axisLength_short);	// 由上两个长度平方和开根号得到的合成长度
	double axisLength_mean = (axisLength_long + axisLength_short)*0.5;										// 由上两个长度取均值得到的合成长度

	double ratioNoncircle = axisLength_short / axisLength_long;

// 	if (ratioNoncircle < 0.50)
// 	{
// 		double shithappens = 1.0;
// 	}

	double dist = std::sqrt(dx*dx + dy*dy); // 质心至图像区域中心的距离

// 	double sigma_angle_new1 = 2 * asind(axisLength_rms / dist);
// 	double sigma_angle_new2 = 2 * asind(axisLength_mean / dist);

	if (axisLength_mean > dist)
	{
		sigma_angle = 180;
	} 
	else
	{
		sigma_angle = 2 * asind(axisLength_mean / dist);
	}

	return true;
}

// 20200825，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
bool DeepVoid::CornerAngle_IC(const cv::Mat & img,	// input: the input gray scale image
							  double x, double y,	// input: the center of the region
							  int r,				// input: the radius of the circular region
							  double & angle		// output:the location of the calculated intensity centroid (in terms of offsets)
							  )
{
	// 确定当前像点所在的具体像素坐标
	int ix = (int)x;
	int iy = (int)y;

	return CornerAngle_IC(img, ix, iy, r, angle);
}

// 20200901，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 特征的区域半径由可以可靠确定特征方向角度时的区域大小来确定
// 角度范围为[0,360)
bool DeepVoid::FeatureRadiusAngle(const cv::Mat & img,			// input: the input gray scale image
								  int ix, int iy,				// input: the center of the region
								  int & r,						// output:the estimated radius of the circular region
								  double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
								  double threshOffset /*= 1.0*/,// input: 当灰度质心偏移量能超过该阈值像素数时才认为可靠确定特征方向，由此定下特征尺度大小
								  int r_max /*= 100*/			// input: 允许的最大邻域半径值
								  )
{
	double thd2 = threshOffset*threshOffset;

	r = 2; // 定一个起始的邻域半径

	double dx, dy;

	// 直接先算一次，看下质心偏移量，另外看看区域内是否为全黑，如果全黑也没必要进行后续的计算了
	if (!IntensityCentroid_CircularRegion(img, ix, iy, r, dx, dy)) // 说明图像区域内灰度值全为 0，即全黑
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

	double radian = std::atan2(dy, dx); // [-π; +π]

	angle = radian*R2D;

	if (angle < 0) // 确保最终的角度范围符合 opencv keypoint::angle 的取值范围，即 [0,360)
	{
		angle += 360;
	}

	return true;
}

bool DeepVoid::FeatureRadiusAngle(const cv::Mat & img,			// input: the input gray scale image
								  double x, double y,			// input: the center of the region
								  int & r,						// output:the estimated radius of the circular region
								  double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
								  double threshOffset /*= 1.0*/,// input: 当灰度质心偏移量能超过该阈值像素数时才认为可靠确定特征方向，由此定下特征尺度大小
								  int r_max /*= 100*/			// input: 允许的最大邻域半径值
								  )
{
	// 确定当前像点所在的具体像素坐标
	int ix = (int)x;
	int iy = (int)y;

	return FeatureRadiusAngle(img, ix, iy, r, angle, threshOffset, r_max);
}

// 20200901，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 特征的区域半径由可以可靠确定特征方向角度时的区域大小来确定
// 角度范围为[0,360)
bool DeepVoid::FeatureRadiusAngle_dAng(const cv::Mat & img,			// input: the input gray scale image
									   int ix, int iy,				// input: the center of the region
									   int & r,						// output:the estimated radius of the circular region
									   double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
									   double thresh_dAng /*= 5.0*/,// input: 当当前特征方向相对于上一次迭代的改变量小于该阈值时才认为可靠确定特征方向，由此定下特征尺度大小
									   int r_max /*= 100*/			// input: 允许的最大邻域半径值
									   )
{
	r = 2;

	double preAng;
	double dAng = 10000000;

	double sigma_angle;

	if (!CornerAngle_IC(img, ix, iy, r, preAng))
	{
		return false;
	}

	while (dAng >= thresh_dAng)
	{
		++r;

		if (r > r_max)
		{
			return false;
		}

		CornerAngle_IC(img, ix, iy, r, angle); // [-360; +360]

		dAng = std::abs(angle - preAng);

		if (dAng > 180)
		{
			dAng = 360 - dAng;
		}

		preAng = angle;
	}

	if (angle < 0) // 确保最终的角度范围符合 opencv keypoint::angle 的取值范围，即 [0,360)
	{
		angle += 360;
	}

	return true;
}

bool DeepVoid::FeatureRadiusAngle_dAng(const cv::Mat & img,			// input: the input gray scale image
									   double x, double y,			// input: the center of the region
									   int & r,						// output:the estimated radius of the circular region
									   double & angle,				// output:the location of the calculated intensity centroid (in terms of offsets)
									   double thresh_dAng /*= 5.0*/,// input: 当当前特征方向相对于上一次迭代的改变量小于该阈值时才认为可靠确定特征方向，由此定下特征尺度大小
									   int r_max /*= 100*/			// input: 允许的最大邻域半径值
									   )
{
	// 确定当前像点所在的具体像素坐标
	int ix = (int)x;
	int iy = (int)y;

	return FeatureRadiusAngle_dAng(img, ix, iy, r, angle, thresh_dAng, r_max);
}


// 20201207，通过计算一圆形支持区域内图像灰度质心偏移量的方式计算该角点特征的方向
// 特征的区域半径由可以可靠确定特征方向角度时的区域大小来确定
// 角度范围为[0,360)
bool DeepVoid::FeatureRadiusAngle_sigmaAng(const cv::Mat & img,				// input: the input gray scale image
										   int ix, int iy,					// input: the center of the region
										   int & r,							// output:the estimated radius of the circular region
										   double & angle,					// output:the location of the calculated intensity centroid (in terms of offsets)
										   double sigma_I /*= 5.0*/,		// input: the standard deviation of the Gaussian random noise of image intensity
										   double thresh_sigmaAng /*= 1.0*/,// input: 当当前特征方向角度不确定度标准差小于该阈值时才认为其为可靠的特征方向，并由此定下特征尺度大小
										   int r_max /*= 100*/				// input: 允许的最大邻域半径值
										   )
{
	r = 1; // 20201207，改为从 1 开始

	double sigma_angle, dSigma;
	double sigma_angle_pre = 10000000; // 20230817，记录前一次迭代的方向角不确定度

	std::vector<double> vSigmas; // 20230817，用于记录每一次迭代的方向角不确定度

//	if (!CornerAngle_IC(img, ix, iy, r, angle, sigma_angle, sigma_I)) // 老代数方式，z=yc/xc
	if (!CornerAngle_IC_moments(img, ix, iy, r, angle, sigma_angle, sigma_I)) // 新代数方式，z=m01/m10
//	if (!CornerAngle_IC_geometry(img, ix, iy, r, angle, sigma_angle, sigma_I)) // 几何方式，直接求质心圆相对特征的张角
	{
		return false;
	}

	vSigmas.push_back(sigma_angle);

//	dSigma = sigma_angle - sigma_angle_pre; // 20231029，看看变化量

//	while (sigma_angle >= thresh_sigmaAng)
	while (fabs(sigma_angle) >= /*0.3*/thresh_sigmaAng) // 20231019，加个绝对值，这样即便为 -inf 负无穷，那肯定也不会直接退出
//	while (fabs(dSigma) >= 0.01/*thresh_sigmaAng*/) // 20231029，退出的控制量变成角度不确定度的变化量的绝对值得小于指定阈值
	{
		++r;

		sigma_angle_pre = sigma_angle; // 20230817

		if (r > r_max)
		{
			return false;
		}

//		double ang_old, ang_geo, sigmaAng_old, sigmaAng_geo;

//		CornerAngle_IC(img, ix, iy, r, /*angle*/ang_old, /*sigma_angle*/sigmaAng_old, sigma_I); // [-360; +360]
		CornerAngle_IC_moments(img, ix, iy, r, angle, sigma_angle, sigma_I); // [-360; +360]
//		CornerAngle_IC_geometry(img, ix, iy, r, /*angle*/ang_geo, /*sigma_angle*/sigmaAng_geo, sigma_I); // [-360; +360]

		vSigmas.push_back(sigma_angle);

		dSigma = sigma_angle - sigma_angle_pre; // 20230817，看看方向角不确定度的变化量
	}

	if (angle < 0) // 确保最终的角度范围符合 opencv keypoint::angle 的取值范围，即 [0,360)
	{
		angle += 360;
	}

	return true;
}

bool DeepVoid::FeatureRadiusAngle_sigmaAng(const cv::Mat & img,				// input: the input gray scale image
										   double x, double y,				// input: the center of the region
										   int & r,							// output:the estimated radius of the circular region
										   double & angle,					// output:the location of the calculated intensity centroid (in terms of offsets)
										   double sigma_I /*= 5.0*/,		// input: the standard deviation of the Gaussian random noise of image intensity
										   double thresh_sigmaAng /*= 1.0*/,// input: 当当前特征方向角度不确定度标准差小于该阈值时才认为其为可靠的特征方向，并由此定下特征尺度大小
										   int r_max /*= 100*/				// input: 允许的最大邻域半径值
										   )
{
	// 确定当前像点所在的具体像素坐标
	int ix = (int)x;
	int iy = (int)y;

	return FeatureRadiusAngle_sigmaAng(img, ix, iy, r, angle, sigma_I, thresh_sigmaAng, r_max);
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
	FILE * file = fopen(path, "r");

	if (file)
	{
		// 先读入内参数 ///////////////////////////////////////////////////////////
		fscanf(file, "Intrinsic Parameters\n");
		fscanf(file, "%lf	%lf	%lf	%lf	%lf\n\n", &cam.fx, &cam.fy, &cam.cx, &cam.cy, &cam.s);
		cam.m_K(0, 0) = cam.fx;
		cam.m_K(1, 1) = cam.fy;
		cam.m_K(0, 2) = cam.cx;
		cam.m_K(1, 2) = cam.cy;
		cam.m_K(0, 1) = cam.s;
		cam.m_K(2, 2) = 1;
		//////////////////////////////////////////////////////////////////////////


		// 再读入像差系数 /////////////////////////////////////////////////////////
		fscanf(file, "Distortion Coefficients\n");
		for (int i = 0; i < 5; i++)
		{
			fscanf(file, "%lf", &cam.k[i]);
			
			cam.m_dist(i) = cam.k[i];
		}
		cam.dist_type = 1; // 默认为 D.C. Brown 的像差系数类型
		fscanf(file, "\n\n");
		//////////////////////////////////////////////////////////////////////////


		// 再读入旋转矩阵 /////////////////////////////////////////////////////////
		fscanf(file, "Rotation Matrix\n");
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				fscanf(file, "%lf", &cam.R[i * 3 + j]);

				cam.m_R(i, j) = cam.R[i * 3 + j];
			}
		}
		fscanf(file, "\n\n");
		//////////////////////////////////////////////////////////////////////////


		// 最后读入平移向量 ///////////////////////////////////////////////////////
		fscanf(file, "Translation Vector\n");
		for (int i = 0; i < 3; i++)
		{
			fscanf(file, "%lf", &cam.t[i]);

			cam.m_t(i) = cam.t[i];
		}
		fscanf(file, "\n\n");
		//////////////////////////////////////////////////////////////////////////

		cam.m_bCalibed = true; // 20220128，这里只默认图像是内标定完了的，即使是已有了有效外参数

		fclose(file);
	}
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
	// 先把参考图中所有特征点画出来
	cv::drawKeypoints(img1, keypoints1, outImg, singlePointColor);

	vector<KeyPoint> keypoints1_FAST_all;

	for (auto iter = keypoints1.begin(); iter != keypoints1.end(); ++iter)
	{
		if (/*fabs(iter->size + 1) < 1.0E-12 && */fabs(iter->angle + 1) < 1.0E-12) // 目前来看只有 fast 特征点的 angle 恒为-1，sift 特征点角度的取值范围为0-360°
		{
			keypoints1_FAST_all.push_back(*iter);
		}
	}

	// 再单独把 FAST 特征点画个别的颜色
	cv::drawKeypoints(outImg, keypoints1_FAST_all, outImg, Scalar(0, 255, 255));

	vector<KeyPoint> matchedKeyPts1; // all the matched keypoints in the reference image
	vector<KeyPoint> matchedFASTPts1;

	for (auto iter = matches1to2.begin(); iter != matches1to2.end(); ++iter)
	{
		const KeyPoint & keypt1 = keypoints1[iter->queryIdx];
		const KeyPoint & keypt2 = keypoints2[iter->trainIdx];

		if (/*fabs(keypt1.size + 1) < 1.0E-12 && */fabs(keypt1.angle + 1) < 1.0E-12) // 目前来看只有 fast 特征点的 angle 恒为-1，sift 特征点角度的取值范围为0-360°
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

// 20200717，给定相对不确定度水平，给出相应的色彩值（RG色彩域），服务于点云不确定度可视化
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
		// 计算 RG 色彩域的色相角
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
// 	feats.clear(); // 先把之前的清掉
// 
// 	cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma);
// 
// 	// 提取 sift 特征点
// 	f2d->detect(img, feats.key_points);
// 
// 	// 按特征 size 从大到小对 sift 特征点进行排序
// 	sort(feats.key_points.begin(), feats.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.size > b.size; });
// 
// 	// 生成特征描述向量
// 	f2d->compute(img, feats.key_points, feats.descriptors);
// 
// 	feats.type = Feature_SIFT_SIFT; // sift keypoints + sift descriptors
// 
// 	// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
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
// 		// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
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
// 	feats.clear(); // 先把之前的清掉
// 
// 	// 提取 fast 角点特征
// 	// 20200706，先把图像转换成灰度图再提取FAST特征，因为opencv fast算子只能适用于灰度图（sift是彩色和灰度皆可），而lightroom处理完的图片导出时会被自动转为3通道图，难怪fast提取的特征总有偏移
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
// 	// 按照 response 从大到小对 fast 特征点进行排序
// 	sort(feats.key_points.begin(), feats.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });
// 
// 	// 生成特征描述向量
// 	cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(nfeaturesSift, nOctaveLayersSift, contrastThresholdSift, edgeThresholdSift, sigmaSift);
// 	f2d->compute(img, feats.key_points, feats.descriptors);
// 
// 	feats.type = Feature_FAST_SIFT; // fast keypoints + sift descriptors
// 
// 	// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
// 	int n = feats.key_points.size();
// 	for (int i = 0; i < n; ++i)
// 	{
// 		const cv::KeyPoint & keypt = feats.key_points[i];
// 
// 		feats.tracks.push_back(-1);
// 		feats.idx_pt.push_back(i);
// 
// 		// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
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
// 	// 先清空
// 	featsSfM.clear();
// 	featsSub.clear();
// 
// 	// 暂时先合成个大的
// 	Features featsTmp = feats_sift;
// 	featsTmp.push_back(feats_fast);
// 
// 	// 然后截取为最终的，并录入
// 	int nSize = featsTmp.key_points.size();
// 	int nSmaller = nSize < nSfMFeatures ? nSize : nSfMFeatures;
// 
// 	featsSfM.key_points.insert(featsSfM.key_points.end(), featsTmp.key_points.begin(), featsTmp.key_points.begin() + nSmaller);
// 	featsSfM.descriptors = featsTmp.descriptors.rowRange(cv::Range(0, nSmaller));
// 	featsSfM.tracks.insert(featsSfM.tracks.end(), featsTmp.tracks.begin(), featsTmp.tracks.begin() + nSmaller);
// 	featsSfM.idx_pt.insert(featsSfM.idx_pt.end(), featsTmp.idx_pt.begin(), featsTmp.idx_pt.begin() + nSmaller);
// 	featsSfM.rgbs.insert(featsSfM.rgbs.end(), featsTmp.rgbs.begin(), featsTmp.rgbs.begin() + nSmaller);
// 
// 	// 再把手提点加进来
// 	featsSfM.push_back(feats_manual);
// 
// 	int nFinal = featsSfM.key_points.size();
// 
// 	// 统计各类点入选参与 SfM 特征点的个数
// 	nSiftElected = nSift < nSfMFeatures ? nSift : nSfMFeatures;
// 	nFastElected = nSmaller - nSiftElected;
// 	nManualElected = nManual; // 即全部手提点都入选
// 
// 	// 更新参与 SfM 的特征点的统一编号
// 	for (int i = nSiftElected; i < nFinal; ++i)
// 	{
// 		featsSfM.idx_pt[i] = i;
// 	}
// 
// 	// 最后生成“先发制人”的特征点集
// 	if (nPrptFeatures < nFinal)
// 	{
// 		featsSub.key_points.insert(featsSub.key_points.end(), featsSfM.key_points.begin(), featsSfM.key_points.begin() + nPrptFeatures);
// 		featsSub.descriptors = featsSfM.descriptors.rowRange(cv::Range(0, nPrptFeatures));
// 		featsSub.tracks.insert(featsSub.tracks.end(), featsSfM.tracks.begin(), featsSfM.tracks.begin() + nPrptFeatures);
// 		featsSub.idx_pt.insert(featsSub.idx_pt.end(), featsSfM.idx_pt.begin(), featsSfM.idx_pt.begin() + nPrptFeatures);
// 		featsSub.rgbs.insert(featsSub.rgbs.end(), featsSfM.rgbs.begin(), featsSfM.rgbs.begin() + nPrptFeatures);
// 	}
// }
