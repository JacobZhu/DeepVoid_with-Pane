#include "stdafx.h"

#include "DeepVoid.h"
#include "MainFrm.h"

// Quadratic curve fitting
void DeepVoid::QuadCurveFit(double * x, double * f,					// 输入：所有采样点的 x 和 f 值
							int n,									// 输入：采样点个数
							double & a0, double & a1, double & a2	// 输出：拟合出来的二次曲线的系数 f = a0 + a1x + a2x^2
							)
{
	double * pA = new double [n * 3];
	double * pb = new double [n];
	double X[3];

	double xx,xx2,ff;

	// 生成 AX=b 的 A 及 b 矩阵
	int i;
	for (i = 0; i < n; i++)
	{
		ff = f[i];

		xx = x[i];
		xx2 = xx*xx;

		pA[i * 3]     = 1;
		pA[i * 3 + 1] = xx;
		pA[i * 3 + 2] = xx2;

		pb[i]         = ff;
	}

	// 解 AX = B 非齐次线性方程组
	EquSlvPdI_611(pA, pb, n, 3, X);  // M[m*n]*S[n]=B[m]

	a0 = X[0];
	a1 = X[1];
	a2 = X[2];

	delete [] pA;
	delete [] pb;
}

// compute the matching cost, proposed by S. Birchfield and C. Tomasi, for a single pixel.
double DeepVoid::ComputeMatchingCostforOnePixel_BT(int x, int y,	// the coordinates in base image of the checked pixel
												   int d,			// the disparity to be checked
												   int w, int h,	// the width and height of stereo images
												   BYTE ** imgb,	// h*w, the rectified grayscale base image
											 	   BYTE ** imgm		// h*w, the rectified grayscale matching image
												   ) 
{
	// if the point to be checked xm = x-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	int xm = x-d;

	if (xm <= 0 || xm >= (w-1) || x <= 0 || x >= (w-1))
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	double fm   = (double)imgm[y][xm];
	double fm_l = (double)imgm[y][xm-1];
	double fm_r = (double)imgm[y][xm+1];

	double fm_l_2 = (fm+fm_l)/2;
	double fm_r_2 = (fm+fm_r)/2;

	double pfm[3]; int idx[3];
	pfm[0] = fm; pfm[1] = fm_l_2; pfm[2] = fm_r_2;  
	Sort_Bubble(pfm, 3, idx);
	double fmmin = pfm[0];
	double fmmax = pfm[2];
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	double fb   = double(imgb[y][x]);
	double fb_l = double(imgb[y][x-1]);
	double fb_r = double(imgb[y][x+1]);

	double fb_l_2 = (fb+fb_l)/2;
	double fb_r_2 = (fb+fb_r)/2;

	double pfb[3];
	pfb[0] = fb; pfb[1] = fb_l_2; pfb[2] = fb_r_2;
	Sort_Bubble(pfb, 3, idx);
	double fbmin = pfb[0];
	double fbmax = pfb[2];
	//////////////////////////////////////////////////////////////////////////

	// in this way
	pfm[0]=0; pfm[1]=fb-fmmax; pfm[2]=fmmin-fb;
	Sort_Bubble(pfm, 3, idx);
	double ffm = pfm[2];

	pfb[0]=0; pfb[1]=fm-fbmax; pfb[2]=fbmin-fm;
	Sort_Bubble(pfb, 3, idx);
	double ffb = pfb[2];

	// or the other way
// 	pfm[0]=fabs(fb-fm); pfm[1]=fabs(fb-fm_l_2); pfm[2]=fabs(fb-fm_r_2);
// 	Sort_Bubble(pfm, 3, idx);
// 	double ffm = pfm[0];
// 
// 	pfb[0]=fabs(fm-fb); pfb[1]=fabs(fm-fb_l_2); pfb[2]=fabs(fm-fb_r_2);
// 	Sort_Bubble(pfb, 3, idx);
// 	double ffb = pfb[0];

	if (ffb <= ffm)
	{
		return ffb;
	}

	return ffm;
}

// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
void DeepVoid::GenerateDSI_BT(int w, int h,			// input: the width and height of stereo images
							  BYTE ** imgb,			// input: h*w, the rectified grayscale base image
							  BYTE ** imgm,			// input: h*w, the rectified grayscale matching image
							  int dmin,				// input: scalar, the minimal disparity
							  int dmax,				// input: scalar, the maximal disparity
							  double *** DSI,		// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
							  bool bMB /*= false*/	// input: is matching in the reverse direction, i.e. matching image is treated as base image and vice versa
							  )
{
	int i,j,k;

	// normal direction, base image -> matching image
	if (!bMB)
	{
		for (i=0;i<h;i++)
		{
			CString strInfo;
			strInfo.Format("generate DSI for row %04d", i);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
			for (j=0;j<w;j++)
			{
				for (k=dmin;k<=dmax;k++)
				{
					DSI[k-dmin][i][j] = ComputeMatchingCostforOnePixel_BT(j, i, k, w, h, imgb, imgm);
				}
			}
		}
	} 
	else // reverse direction, matching image -> base image
	{
		for (i=0;i<h;i++)
		{
			CString strInfo;
			strInfo.Format("generate DSI for row %04d", i);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
			for (j=0;j<w;j++)
			{
				for (k=dmin;k<=dmax;k++)
				{
					DSI[k-dmin][i][j] = ComputeMatchingCostforOnePixel_BT(j, i, -k, w, h, imgb, imgm);
				}
			}
		}
	}
}

// Cost aggregation along one path for one given pixel based on scanline
// optimizaiton or dynamic programming?
void DeepVoid::CostAggalongOnePathforOnePixel(int x, int y,			// input: the coordinates in base image of the checked pixel
											  double ang,			// input: the angle between scanline and positive x-axis, starting from positive x-axis
											  int w, int h, int nd,	// input: the dimensions of the Disparity Space Image
											  double *** DSI,		// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
											  double P1,			// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											  double P2,			// input: scalar, a larger constant penalty for all larger disparity changes
											  double * Lr			// output:nd-vector,the aggregated cost vector along one path for given pixel (x, y)
											  )
{
	int x_start, y_start;

	double tanAng = tand(ang);

	// first, determine the intersection between image border and the path
	if (fabs(ang) <= 1.0E-6 || fabs(ang-360) <= 1.0E-6) // ang=0 or ang=360
	{
		x_start = w-1;
		y_start = y;
	} 
	else if (ang > 0 && ang < 90) // 0<ang<90
	{
		int yy = FTOI(y - tanAng * (w-1-x));
		if (yy>=0)
		{
			x_start = w-1;
			y_start = yy;
		} 
		else
		{
			x_start = FTOI(y/tanAng + x);
			y_start = 0;
		}
	}
	else if (fabs(ang-90) <= 1.0E-6) // ang=90
	{
		x_start = x;
		y_start = 0;
	}
	else if (ang>90 && ang<180) // 90<ang<180
	{
		int yy = FTOI(y + tanAng * x);
		if (yy>=0)
		{
			x_start = 0;
			y_start = yy;
		} 
		else
		{
			x_start = FTOI(y/tanAng + x);
			y_start = 0;
		}
	}
	else if (fabs(ang-180) <= 1.0E-6) // ang=180
	{
		x_start = 0;
		y_start = y;
	}
	else if (ang>180 && ang<270) // 180<ang<270
	{
		int yy = FTOI(y + tanAng * x);
		if (yy<h)
		{
			x_start = 0;
			y_start = yy;
		} 
		else
		{
			x_start = FTOI((y-h+1)/tanAng + x);
			y_start = h-1;
		}
	}
	else if (fabs(ang-270) <= 1.0E-6) // ang=270
	{
		x_start = x;
		y_start = h-1;
	}
	else // 270<ang<360
	{
		int yy = FTOI(y - tanAng * (w-1-x));
		if (yy<h)
		{
			x_start = w-1;
			y_start = yy;
		} 
		else
		{
			x_start = FTOI((y-h+1)/tanAng + x);
			y_start = h-1;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// compute all the points on the path
	int n_maxDim, n_ptsLine;
	if (w>=h)
	{
		n_maxDim = w;
	} 
	else
	{
		n_maxDim = h;
	}
	int * px = new int [n_maxDim];
	int * py = new int [n_maxDim];

	GetLinePts(x_start, y_start, x, y, px, py, n_ptsLine);

	// start cost aggregation along the path
	int i;
	for (i=0;i<nd;i++)
	{
		Lr[i] = DSI[i][py[0]][px[0]];
	}
	for (i=1;i<n_ptsLine;i++)
	{
		CostAggalongOnePathforOnePixel_Step(px[i], py[i], nd, DSI, Lr, P1, P2);
	}

	// release memory
	delete [] px;
	delete [] py;
}

// Critical step in cost aggregation along one path for one given pixel based on scanline optimizaiton or dynamic programming?
void DeepVoid::CostAggalongOnePathforOnePixel_Step(int x, int y,				// input: the coordinates in base image of the checked pixel
												   int nd,						// input: disparity range
												   double *** DSI,				// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
												   double * Lr,					// input & output: nd_vector, as input it's the recursively aggregated cost up to the previous pixel before current one along the path
																				// as output it's the recursively aggregated cost up to the current pixel along the path
												   double P1,					// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
												   double P2,					// input: scalar, a larger constant penalty for all larger disparity changes
												   BYTE * pGrayDiff /*= NULL*/	// input: the difference between the gray level of currently checked pixel and the previous one
												   )
{
	int i,j;

	// copy the input Lr into Lr0
	double * Lr0 = new double [nd];
	memcpy(Lr0, Lr, nd * sizeof(double));

	// find all the valid disparities in Lr0, whose aggregated cost are not negative
	int * idxValid = new int [nd];
	double * Lr0Valid = new double [nd];
	int nValid = 0;
	double min_Lr0;

	for (i=0;i<nd;i++)
	{
		if (Lr0[i]<0)
		{
			continue;
		}

		// find the minimal nonnegative value at the same time
		if (!nValid)
		{
			min_Lr0 = Lr0[i];
		} 
		else
		{
			if (Lr0[i] < min_Lr0)
			{
				min_Lr0 = Lr0[i];
			}
		}

		idxValid[nValid]=i;
		Lr0Valid[nValid]=Lr0[i];
		nValid++;
	}

	// if nValid=0, it means that previous pixels definitly have no matches at all, this situation often occurs at the base image's left border
	// then we take the right pixel next to it to be the starting pixel for cost aggregation
	if (!nValid)
	{
		for (i=0;i<nd;i++)
		{
			Lr[i] = DSI[i][y][x];
		}
		
		// release memory
		delete [] Lr0;
		delete [] idxValid;
		delete [] Lr0Valid;
		return;
	}

	double minCost, curCost;

	// adaptive P2 to preserve depth discontinuity
	if (pGrayDiff)
	{
		double tmp = P2/abs(*(pGrayDiff));
		if (tmp > P1)
		{
			P2 = tmp;
		}
	}

	// start to compute the Lr
	for (i=0;i<nd;i++) // check every disparity of current pixel
	{
		// if the matching cost at disparity Cp(i) is negative i.e. invalid, it means that the matching cost between p and p-d is enormous
		// then there is no need to check this disparity
		if (DSI[i][y][x]<0)
		{
			Lr[i] = DSI[i][y][x];
			continue;
		}

		// check every disparity with previously aggregated positive cost
		for (j=0;j<nValid;j++)
		{
			if (idxValid[j] == i)
			{
				curCost = Lr0[idxValid[j]];
			}
			else if (abs(idxValid[j] - i) == 1)
			{
				curCost = Lr0[idxValid[j]] + P1;
			}
			else
			{
				curCost = Lr0[idxValid[j]] + P2;
			}

			if (0 == j)
			{
				minCost = curCost;
			} 
			else
			{
				if (curCost < minCost)
				{
					minCost = curCost;
				}
			}
		}

		Lr[i] = DSI[i][y][x] + minCost - min_Lr0;
	}

	// release memory
	delete [] Lr0;
	delete [] idxValid;
	delete [] Lr0Valid;
}

// Critical step in cost aggregation along one path for one given pixel based on scanline optimizaiton or dynamic programming?
void DeepVoid::CostAgg_Step(int x, int y,		// input: the coordinates in base image of the checked pixel i.e. Lr
							int nd,				// input: disparity range
							double *** DSI,		// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
							int x_0, int y_0,	// input: the coordinates in base image of the previous pixel i.e.Lr_1 
							double *** C_dir,	// input&output: h*w*nd, the 3D aggregated costs along each direction
							double P1,			// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
							double P2			// input: scalar, a larger constant penalty for all larger disparity changes
							)
{
	int i,j;

	double * Lr0 = new double [nd];

	// copy the input Lr into Lr0
	for (i=0;i<nd;i++)
	{
		Lr0[i] = C_dir[i][y_0][x_0];
	}

	// find all the valid disparities in Lr0, whose aggregated cost are not negative
	int * idxValid = new int [nd];
	double * Lr0Valid = new double [nd];
	int nValid = 0;
	double min_Lr0;

	for (i=0;i<nd;i++)
	{
		if (Lr0[i]<0)
		{
			continue;
		}

		// find the minimal nonnegative value at the same time
		if (!nValid)
		{
			min_Lr0 = Lr0[i];
		} 
		else
		{
			if (Lr0[i] < min_Lr0)
			{
				min_Lr0 = Lr0[i];
			}
		}

		idxValid[nValid]=i;
		Lr0Valid[nValid]=Lr0[i];
		nValid++;
	}

	// if nValid=0, it means that previous pixels definitly have no matches at all, this situation often occurs at the base image's left border
	// then we take the right pixel next to it to be the starting pixel for cost aggregation
	if (!nValid)
	{
		for (i=0;i<nd;i++)
		{
			C_dir[i][y][x] = DSI[i][y][x];
		}

		// release memory
		delete [] Lr0;
		delete [] idxValid;
		delete [] Lr0Valid;
		return;
	}

	double minCost, curCost;

	// start to compute the Lr
	for (i=0;i<nd;i++) // check every disparity of current pixel
	{
		// if the matching cost at disparity Cp(i) is negative i.e. invalid, it means that the matching cost between p and p-d is enormous
		// then there is no need to check this disparity
		if (DSI[i][y][x]<0)
		{
			C_dir[i][y][x] = DSI[i][y][x];
			continue;
		}

		// check every disparity with previously aggregated positive cost
		for (j=0;j<nValid;j++)
		{
			if (idxValid[j] == i)
			{
				curCost = Lr0[idxValid[j]];
			}
			else if (abs(idxValid[j] - i) == 1)
			{
				curCost = Lr0[idxValid[j]] + P1;
			}
			else
			{
				curCost = Lr0[idxValid[j]] + P2;
			}

			if (0 == j)
			{
				minCost = curCost;
			} 
			else
			{
				if (curCost < minCost)
				{
					minCost = curCost;
				}
			}
		}

		C_dir[i][y][x] = DSI[i][y][x] + minCost - min_Lr0;
	}

	// release memory
	delete [] Lr0;
	delete [] idxValid;
	delete [] Lr0Valid;
}

// Cost aggregation along multiple paths for one given pixel based on scanline
// optimizaiton or dynamic programming?
void DeepVoid::CostAggalongMultiPathsforOnePixel(int x, int y,			// input: the coordinates in base image of the checked pixel
												 int nPaths,			// input: the amount of scanlines
												 double * angs,			// input: contains angles of each path
												 int w, int h, int nd,	// input: the dimensions of the Disparity Space Image
												 double *** DSI,		// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
												 double P1,				// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
												 double P2,				// input: scalar, a larger constant penalty for all larger disparity changes
												 double * S				// output:nd-vector, the aggregated cost vector along multiple paths for given pixel (x, y)
												 )
{
	// reset the values in S into 0
	memset(S, 0, nd * sizeof(double));

	// create memory for aggregated cost along each path
	double * Lr_each = new double [nd];

	int i,j;
	for (i=0;i<nPaths;i++)
	{
		CostAggalongOnePathforOnePixel(x, y, angs[i], w, h, nd, DSI, P1, P2, Lr_each);
		
		for (j=0;j<nd;j++)
		{
			S[j] += Lr_each[j];
		}
	}

	// release memory
	delete [] Lr_each;
}

// Semi global matching algorithm, implemented based on <Stereo Processing by Semiglobal Matching and Mutual Information> 2008
void DeepVoid::SemiGlobalMatching(int w, int h,				// input: the width and height of stereo images
								  BYTE ** imgb,				// input: h*w, the rectified grayscale base image
								  BYTE ** imgm,				// input: h*w, the rectified grayscale matching image
								  int dmin,					// input: scalar, the minimal disparity
								  int dmax,					// input: scalar, the maximal disparity
								  double P1,				// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
								  double P2,				// input: scalar, a larger constant penalty for all larger disparity changes
								  double ** DI,				// output:h*w, the disparity image
								  SGM_PATHS nDir,			// input: the amount of scanlines
								  int constcCheck /*= -1*/,	// input: if matching is conducted reversely, i.e. treat the match image as a base image and vice versa.
								  bool bSubPix /*= true*/	// input: if sub pixel interpolation for sub pixel disparities is applied
								  )
{
	int i,j,k;

	int nd = dmax - dmin + 1;

	// initialize DSI matrix, which is h*w*nd
	int imgSize = w*h;
	double *** DSI = new double ** [nd];
	for (i = 0; i < nd; i++)
	{
		DSI[i] = new double * [h];
		DSI[i][0] = new double [imgSize];
		memset(DSI[i][0], 0, imgSize * sizeof(double));
		for (j = 1; j < h; j++)
		{
			DSI[i][j] = DSI[i][j - 1] + w;
		}
	}

	// generate the DSI based on some certain matching cost measure
	GenerateDSI_BT(w, h, imgb, imgm, dmin, dmax, DSI);

	SemiGlobalMatching_givenDSI(w, h, imgb, imgm, dmin, dmax, DSI, P1, P2, DI, nDir, bSubPix);

	// if this var is nonnegative, then uniqueness constraint is enforced by consistency check to determine occlusions and false matches
	if (constcCheck >= 0) 
	{
		// a new disparity image for matching image
		double ** DI_m = new double * [h];
		DI_m[0] = new double [imgSize];
		memset(DI_m[0], 0, imgSize * sizeof(double));
		for (i = 1; i < h; i++)
		{
			DI_m[i] = DI_m[i - 1] + w;
		}

		// generate the reverse matching cost DSI
		GenerateDSI_BT(w, h, imgm, imgb, dmin, dmax, DSI, true);

		// run semi global matching again for matching image
		SemiGlobalMatching_givenDSI(w, h, imgm, imgb, dmin, dmax, DSI, P1, P2, DI_m, nDir, bSubPix);

		bool bOccluded;
		int dp, q;
		for (i=0;i<h;i++)
		{
			for (j=0;j<w;j++)
			{
				dp = int(DI[i][j]+0.5); // round the subpixel disparity
				if (dp<=0) // disparity is supposed to be positive
				{
					continue;
				}
				q = j-dp; // the corresponding point in matching image
				if (q<0 || abs(dp - int(DI_m[i][q]+0.5)) > constcCheck) // if this is satisfied means that (i,j) is a occluded point or a mismatched point
				/*if (q<0 || fabs(DI[i][j] - DI_m[i][q]) > constcCheck)*/
				{
					bOccluded = true;
					for (k=dmin;k<=dmax;k++)
					{
						if (j-k<0) // cross the left image border
						{
							break;
						}
						double d_tmp = DI_m[i][j-k];
						if (fabs(d_tmp-k)<1.0E-6)
						{
							// according to Hirshmuller, if the epipolar line of a mismatched point intersect the disparity function Dm 
							// whereas the epipolar line of a occluded point does not intersect the disparity function Dm
							bOccluded = false; 
							break;
						}
					}
					if (bOccluded)
					{
						DI[i][j] = -1; // occluded
					} 
					else
					{
						DI[i][j] = -2; // mismatched
					}
				}
			}
		}

		delete [] DI_m[0];
		delete [] DI_m;
	}

	// release
	for (i = 0; i < nd; i++)
	{
		delete [] DSI[i][0];
		delete [] DSI[i];
	}
	delete [] DSI;
}

// Semi global matching algorithm, implemented based on <Stereo Processing by Semiglobal Matching and Mutual Information> 2008
void DeepVoid::SemiGlobalMatching_givenDSI(int w, int h,					// input: the width and height of stereo images
										   BYTE ** imgb,					// input: h*w, the rectified grayscale base image
										   BYTE ** imgm,					// input: h*w, the rectified grayscale matching image
										   int dmin,						// input: scalar, the minimal disparity
										   int dmax,						// input: scalar, the maximal disparity
										   double *** DSI,					// input: h*w*nd, the disparity space image containing all the matching cost for all pixels
										   double P1,						// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
										   double P2,						// input: scalar, a larger constant penalty for all larger disparity changes
										   double ** DI,					// output:h*w, the disparity image
										   SGM_PATHS nDir /*= SGM_PATHS_8*/,// input: the amount of scanlines*/
										   bool bSubPix /*= true*/			// input: if sub pixel interpolation for sub pixel disparities is applied
										   )
{
	int i,j,k;

	// compute all disparity levels
	int nd = dmax - dmin + 1;
	int * alld = new int [nd];
	for (i=0;i<nd;i++)
	{
		alld[i]=dmin+i;
	}

	// initialize C_dir matrix, which is h*w*nd, it's the 3D aggregated costs along each direction 
	// initialize S_dirs matrix, which is h*w*nd, it's the sum of all the 3D aggregated costs along all direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("apply new memory");
	int imgSize = w*h;
	double *** C_dir = new double ** [nd];
	double *** S_dirs = new double ** [nd];
	for (i = 0; i < nd; i++)
	{
		C_dir[i] = new double * [h];
		S_dirs[i] = new double * [h];

		C_dir[i][0] = new double [imgSize];
		S_dirs[i][0] = new double [imgSize];

		memset(C_dir[i][0], 0, imgSize * sizeof(double));
		memset(S_dirs[i][0], 0, imgSize * sizeof(double));

		for (j = 1; j < h; j++)
		{
			C_dir[i][j] = C_dir[i][j - 1] + w;
			S_dirs[i][j] = S_dirs[i][j - 1] + w;
		}
	}

	// Lr_1
	double * Lr_1 = new double [nd];
	BYTE grayDiff;

	// aggregate along 8 directions or 16 directions
	if (SGM_PATHS_8 == nDir)
	{
		/*------------ 01 direction --------------------------------------------------------------------------*/
		// dir = [1, 0] i.e. along positive direction of x-axis or the 0 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 01 starts");
		for (i=0;i<h;i++) // left image border
		{
			for (k=0;k<nd;k++)
			{
				C_dir[k][i][0] = DSI[k][i][0];
			}
		}

		for (j=1;j<w;j++)
		{
			for (i=0;i<h;i++)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i][j-1];
				}
				grayDiff = imgm[i][j]-imgm[i][j-1];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 02 direction --------------------------------------------------------------------------*/
		// dir = [-1, 0] i.e. along negative direction of x-axis or the 180 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 02 starts");
		for (i=0;i<h;i++) // right image border
		{
			for (k=0;k<nd;k++)
			{
				C_dir[k][i][w-1] = DSI[k][i][w-1];
			}
		}

		for (j=w-2;j>-1;j--)
		{
			for (i=0;i<h;i++)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i][j+1];
				}
				grayDiff = imgm[i][j]-imgm[i][j+1];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 03 direction --------------------------------------------------------------------------*/
		// dir = [0, 1] i.e. along positive direction of y-axis or the -90 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 03 starts");
		for (k=0;k<nd;k++) // top image border
		{
			for (j=0;j<w;j++)
			{
				C_dir[k][0][j] = DSI[k][0][j];
			}
		}

		for (i=1;i<h;i++)
		{
			for (j=0;j<w;j++)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i-1][j];
				}
				grayDiff = imgm[i][j]-imgm[i-1][j];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 04 direction --------------------------------------------------------------------------*/
		// dir = [0, -1] i.e. along negative direction of y-axis or the 90 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 04 starts");
		for (k=0;k<nd;k++) // bottom image border
		{
			for (j=0;j<w;j++)
			{
				C_dir[k][h-1][j] = DSI[k][h-1][j];
			}
		}

		for (i=h-2;i>-1;i--)
		{
			for (j=0;j<w;j++)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i+1][j];
				}
				grayDiff = imgm[i][j]-imgm[i+1][j];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 05 direction --------------------------------------------------------------------------*/
		// dir = [1, 1] i.e. along the -45 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 05 starts");
		for (i=0;i<h;i++) // left image border
		{
			for (k=0;k<nd;k++)
			{
				C_dir[k][i][0] = DSI[k][i][0];
			}
		}
		for (k=0;k<nd;k++) // top image border
		{
			for (j=1;j<w;j++)
			{
				C_dir[k][0][j] = DSI[k][0][j];
			}
		}

		for (i=1;i<h;i++)
		{
			for (j=1;j<w;j++)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i-1][j-1];
				}
				grayDiff = imgm[i][j]-imgm[i-1][j-1];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 06 direction --------------------------------------------------------------------------*/
		// dir = [-1, -1] i.e. along the 135 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 06 starts");
		for (i=0;i<h;i++) // right image border
		{
			for (k=0;k<nd;k++)
			{
				C_dir[k][i][w-1] = DSI[k][i][w-1];
			}
		}
		for (k=0;k<nd;k++) // bottom image border
		{
			for (j=0;j<w-1;j++)
			{
				C_dir[k][h-1][j] = DSI[k][h-1][j];
			}
		}

		for (i=h-2;i>-1;i--)
		{
			for (j=w-2;j>-1;j--)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i+1][j+1];
				}
				grayDiff = imgm[i][j]-imgm[i+1][j+1];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 07 direction --------------------------------------------------------------------------*/
		// dir = [1, -1] i.e. along the 45 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 07 starts");
		for (i=0;i<h;i++) // left image border
		{
			for (k=0;k<nd;k++)
			{
				C_dir[k][i][0] = DSI[k][i][0];
			}
		}
		for (k=0;k<nd;k++) // bottom image border
		{
			for (j=1;j<w;j++)
			{
				C_dir[k][h-1][j] = DSI[k][h-1][j];
			}
		}

		for (i=h-2;i>-1;i--)
		{
			for (j=1;j<w;j++)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i+1][j-1];
				}
				grayDiff = imgm[i][j]-imgm[i+1][j-1];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);

		/*------------ 08 direction --------------------------------------------------------------------------*/
		// dir = [-1, 1] i.e. along the -135 direction
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 08 starts");
		for (i=0;i<h;i++) // right image border
		{
			for (k=0;k<nd;k++)
			{
				C_dir[k][i][w-1] = DSI[k][i][w-1];
			}
		}
		for (k=0;k<nd;k++) // top image border
		{
			for (j=0;j<w-1;j++)
			{
				C_dir[k][0][j] = DSI[k][0][j];
			}
		}

		for (i=1;i<h;i++)
		{
			for (j=w-2;j>-1;j--)
			{
				for (k=0;k<nd;k++)
				{
					Lr_1[k] = C_dir[k][i-1][j+1];
				}
				grayDiff = imgm[i][j]-imgm[i-1][j+1];
				/*CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2, &grayDiff);*/
				CostAggalongOnePathforOnePixel_Step(j, i, nd, DSI, Lr_1, P1, P2);
				for (k=0;k<nd;k++)
				{
					C_dir[k][i][j] = Lr_1[k];
				}
			}
		}

		// add the 3D aggregated cost to the sum
		Add3DArrays_double(w, h, nd, S_dirs, C_dir);
	} 
	else
	{

	}

	BOOL bFoundNonneg = FALSE; // if the nonnegative value is found
	double min_S_p;
	int minIdx_S_p;

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			bFoundNonneg = FALSE;
			for (k=0;k<nd;k++)
			{
				if (S_dirs[k][i][j]<0)
				{
					continue;
				}

				if (!bFoundNonneg)
				{
					min_S_p = S_dirs[k][i][j];
					minIdx_S_p = k;
					bFoundNonneg = TRUE;
				}
				else
				{
					if (S_dirs[k][i][j] < min_S_p)
					{
						min_S_p = S_dirs[k][i][j];
						minIdx_S_p = k;
					}
				}
			}

			if (!bFoundNonneg)
			{
				DI[i][j] = -1;
				continue;
			}

			if (!bSubPix || minIdx_S_p == 0 || minIdx_S_p == nd-1 || S_dirs[minIdx_S_p-1][i][j]<0 || S_dirs[minIdx_S_p+1][i][j]<0)
			{
				DI[i][j] = alld[minIdx_S_p];
			} 
			else // interpolate the subpixel position of the disparity using quadratic curve fitting
			{
				double x[3], f[3];
				x[0] = alld[minIdx_S_p-1]; f[0] = S_dirs[minIdx_S_p-1][i][j];
				x[1] = alld[minIdx_S_p];   f[1] = S_dirs[minIdx_S_p][i][j];
				x[2] = alld[minIdx_S_p+1]; f[2] = S_dirs[minIdx_S_p+1][i][j];

				double a0, a1, a2;
				QuadCurveFit(x, f, 3, a0, a1, a2);

				double d_sub = -0.5*a1/a2;

				DI[i][j] = d_sub;
			}
		}
	}

	// release memory
	delete [] alld;

	for (i = 0; i < nd; i++)
	{
		delete [] C_dir[i][0];
		delete [] C_dir[i];

		delete [] S_dirs[i][0];
		delete [] S_dirs[i];
	}
	delete [] C_dir;
	delete [] S_dirs;

	delete [] Lr_1;
}

// convert a int matrix to a image
void DeepVoid::ConvertMtx2Image_int(int w, int h,	// input: the width and height of stereo images
									int ** mtx,		// input: the matrix to be converted
									BYTE ** img		// output:the output image
									)
{
	int minVal = mtx[0][0];
	int maxVal = mtx[0][0];

	int i,j;
	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			if (mtx[i][j] < minVal)
			{
				minVal = mtx[i][j];
				continue;
			} 
			
			if (mtx[i][j] > maxVal)
			{
				maxVal = mtx[i][j];
			}
		}
	}

	int dd = maxVal - minVal;

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			img[i][j] = (BYTE)FTOI((mtx[i][j]-minVal)*255/(double)dd);
		}
	}
}

// convert a int matrix to a image
void DeepVoid::ConvertMtx2Image_double(int w, int h,	// input: the width and height of stereo images
									   double ** mtx,	// input: the matrix to be converted
									   BYTE ** img		// output:the output image
									   )
{
	double minVal = mtx[0][0];
	double maxVal = mtx[0][0];

	int i,j;
	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			if (mtx[i][j] < minVal)
			{
				minVal = mtx[i][j];
				continue;
			} 

			if (mtx[i][j] > maxVal)
			{
				maxVal = mtx[i][j];
			}
		}
	}

	double dd = maxVal - minVal;

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			if (fabs(mtx[i][j]+1)<1.0E-6)
			{
				img[i][j] = 0; // occluded
			}
			else if (fabs(mtx[i][j]+2)<1.0E-6)
			{
				img[i][j] = 255; // mismatched
			} 
			else
			{
				img[i][j] = (BYTE)FTOI((mtx[i][j]-minVal)*255/dd);
			}
		}
	}
}

// A = A + B
void DeepVoid::Add3DArrays_double(int w, int h, int d,
								  double *** A,
								  double *** B
								  )
{
	int i,j,k;

	for (k=0;k<d;k++)
	{
		for (i=0;i<h;i++)
		{
			for (j=0;j<w;j++)
			{
				A[k][i][j] += B[k][i][j];
			}
		}
	}
}

/*------------------------------------------------------------------------------------*/
/* Graph related ---------------------------------------------------------------------*/

DeepVoid::CEdge::CEdge()
{
}

DeepVoid::CEdge::~CEdge()
{
}

DeepVoid::CEdge::CEdge(const CEdge & edge)
{
	*this = edge;
}

CEdge & DeepVoid::CEdge::operator = (const CEdge & otherEdge)
{
	from = otherEdge.from;
	to = otherEdge.to;
	weight = otherEdge.weight;
	return *this;
}

DeepVoid::CGraph::CGraph()
{
	n_v = 0;
	v = NULL;
	adjMat = NULL;
}

DeepVoid::CGraph::CGraph(int n)
{
	int i;

	n_v = n;

	int size = n_v * n_v;

	v = new CVertex [n_v];

	adjMat = new bool * [n_v];
	adjMat[0] = new bool [size];
	memset(adjMat[0], 0, size * sizeof(bool));
	for (i = 1; i < n_v; i++)
	{
		adjMat[i] = adjMat[i - 1] + n_v;
	}
}

DeepVoid::CGraph::CGraph(const CGraph & graph)
{
	n_v = 0;
	v = NULL;
	adjMat = NULL;

	*this = graph;
}

DeepVoid::CGraph::~CGraph()
{
	release();
}

void DeepVoid::CGraph::release(void)
{
	if (v)
	{
		delete [] v;
		v = NULL;
	}

	if (adjMat)
	{
		delete [] adjMat[0];
		delete [] adjMat;
		adjMat = NULL;
	}

	n_v = 0;
}

void DeepVoid::CGraph::create(int n)
{
	// release all resources before recreation a new graph
	release();

	// recreate a new graph and apply for new resources
	int i;

	n_v = n;

	int size = n_v * n_v;

	v = new CVertex [n_v];

	adjMat = new bool * [n_v];
	adjMat[0] = new bool [size];
	memset(adjMat[0], 0, size * sizeof(bool));
	for (i = 1; i < n_v; i++)
	{
		adjMat[i] = adjMat[i - 1] + n_v;
	}
}

// implementation of operator =, graph = othergraph
CGraph & DeepVoid::CGraph::operator = (const CGraph & otherGraph)
{
	if (this == &otherGraph)
	{
		return *this;
	} 
	else
	{
		// create a new graph
		create(otherGraph.n_v);

		// and copy every thing from otherGraph
		int i,j;
		for (i=0;i<n_v;i++)
		{
			v[i].in_v = otherGraph.v[i].in_v;
			v[i].key  = otherGraph.v[i].key;
			v[i].out_e= otherGraph.v[i].out_e;

			for (j=0;j<n_v;j++)
			{
				adjMat[i][j] = otherGraph.adjMat[i][j];
			}
		}

		return *this;
	}
}

void DeepVoid::CGraph::addEdge(int from, int to, double weight)
{
	// if this edge already exists, only change the weight of this edge and return
	if (adjMat[from][to])
	{
		int i;
		for (i=0; i<v[from].out_e.size(); i++)
		{
			if (v[from].out_e[i].to == to)
			{
				v[from].out_e[i].weight = weight;
				break;
			}
		}
		return;
	}

	// if this edge does not exist, add this edge
	CEdge e;
	e.from = from;
	e.to = to;
	e.weight = weight;

	v[from].out_e.push_back(e); // add the edge to vertex[from], because the edge starts at vertex[from]
	v[to].in_v.push_back(from); // add the vertex[from] to vertex[to], because the edge ends at vertex[to]
	adjMat[from][to] = true;	// update the adjacency matrix
}

void DeepVoid::CGraph::deleteEdge(int from, int to)
{
	// if this edge does not exist just return
	if (!adjMat[from][to])
	{
		return;
	}

	// if this edge exists delete this edge
	vector <CEdge>::iterator e_iter;
	for (e_iter = v[from].out_e.begin(); e_iter != v[from].out_e.end(); e_iter++)
	{
		if (e_iter->to == to)
		{
			v[from].out_e.erase(e_iter);
			break;
		}
	}
	
	// and delete the vertex[from] from the incident vertices list of vertex[to]
	vector <int>::iterator v_iter;
	for (v_iter = v[to].in_v.begin(); v_iter != v[to].in_v.end(); v_iter++)
	{
		if (*v_iter == from)
		{
			v[to].in_v.erase(v_iter);
			break;
		}
	}

	// update the adjacency matrix
	adjMat[from][to] = false;
}

BOOL DeepVoid::CGraph::isEdge(int from, int to) const
{
	return adjMat[from][to];
}

BOOL DeepVoid::CGraph::getEdgeValue(int from, int to, double & w) const
{
	if (!isEdge(from, to))
	{
		return FALSE;
	}

	vector <CEdge>::const_iterator iter;
	for (iter = v[from].out_e.begin(); iter != v[from].out_e.end(); iter++)
	{
		if (iter->to == to)
		{
			w = iter->weight;
			break;
		}
	}

	return TRUE;
}

BOOL DeepVoid::CGraph::setEdgeValue(int from, int to, double w)
{
	if (!isEdge(from, to))
	{
		return FALSE;
	}

	vector <CEdge>::iterator iter;
	for (iter = v[from].out_e.begin(); iter != v[from].out_e.end(); iter++)
	{
		if (iter->to == to)
		{
			iter->weight = w;
			break;
		}
	}

	return TRUE;
}

BOOL DeepVoid::CGraph::addEdgeValue(int from, int to, double w)		// add the weight to the designated edge, if the edge exists return TRUE, otherwise FALSE
{
	if (!isEdge(from, to))
	{
		return FALSE;
	}

	vector <CEdge>::iterator iter;
	for (iter = v[from].out_e.begin(); iter != v[from].out_e.end(); iter++)
	{
		if (iter->to == to)
		{
			iter->weight += w;
			break;
		}
	}

	return TRUE;
}

// The Breadth-first search algorithm finds the least number of edges between designated vertex a and b, neglecting the weight of each edge
// this function return the number of least edges found between vertex a and b if there is this path, otherwise return -1
int DeepVoid::BFS(const CGraph & graph,			// input:  the graph to be searched in
				  int v_a, int v_b,				// input:  indices of vertices a and b
				  vector <int> & path,			// output: the shortest path, with indices of all vertices in this output if there is a path from a to b;
												//		   if there is no path from a to b this var indicates which vertices are visited from a which are not
				  double * thresh_w/* = NULL*/,	// input:  threshold, if the edge weight between two vertices <= this threshold, then these two vertices will still not be considered connected
				  double * min_w /*= NULL*/		// output: if there is this shortest path, then return the smallest weight
				  )
{
	path.clear(); // empty path vector

	// create a queue FIFO (first in first out)
	list<int> queue;

	// the array indicating which vertex has been visited
	BOOL * bVisited = new BOOL [graph.n_v];
	memset(bVisited, 0, graph.n_v * sizeof(BOOL));

	// create a array in order to reconstruct the shortest path once the it is found
	int * p = new int [graph.n_v];

	// put vertex a into the queue
	queue.push_back(v_a);
	bVisited[v_a] = TRUE;

	BOOL bFound = FALSE;

	vector <CEdge>::iterator iter;

	while (queue.size() > 0) // keep doing until queue is empty
	{
		int t = queue.front(); // can not be int & t = queue.front() here, since pop_front() will delete the first element and t will point nowhere if it is a reference to the first element
		queue.pop_front();

		if (t == v_b) // found the vertex b
		{
			bFound = TRUE;
			break;
		}

		for (iter = graph.v[t].out_e.begin(); iter != graph.v[t].out_e.end(); iter++)
		{
			int u = iter->to;

			if (!bVisited[u])
			{
				if (thresh_w && iter->weight <= *thresh_w)
				{
					continue;
				}

				queue.push_back(u);
				bVisited[u] = TRUE;
				p[u] = t;
			}
		}
	}

	// if found the shortest path between a and b, then reconstruct this path
	if (bFound)
	{
		// reconstruct shortest path based on array p
		int pre = v_b;
		path.push_back(pre);
		
		while (pre != v_a)
		{
			pre = p[pre];
			path.push_back(pre);
		}

		// the actual path sequence from vertex a to vertex b is the reversed one
		std::reverse(path.begin(),path.end());

		// find the smallest edge weight
		if (min_w)
		{
			graph.getEdgeValue(path[0], path[1], *min_w);
			int j = 1;
			int i;
			for (i=2;i<path.size();i++)
			{
				double tmp;
				graph.getEdgeValue(path[j], path[i], tmp);
				if (*min_w > tmp)
				{
					*min_w = tmp;
				}
				j++;
			}
		}

		// release
		delete [] bVisited;
		delete [] p;

		return (path.size()-1);
	}
	else // if there is no path from a to b path will contain the visited vertices from a
	{
		int i;
		for (i=0;i<graph.n_v;i++)
		{
			if (bVisited[i])
			{
				path.push_back(1);
			}
			else
			{
				path.push_back(0);
			}
		}

		// release
		delete [] bVisited;
		delete [] p;

		return -1;
	}
}

// Classical maxflow/mincut algorithm that finds the maximum flow from source s to sink t
// The algorithm is identical to the Ford-Fulkerson algorithm,
// except that the search order when finding the augmenting path is defined.
// The path found must be a shortest path that has available capacity.
// This can be found by a breadth-first search, as we let edges have unit length.
// Complexity is O(VE^2)
double DeepVoid::MaxFlowMinCut_EdmondsKarp(CGraph & graph,		// input & output: the residual graph, with all reversed paths already constructed
										   int s, int t,		// input: the indices of the source s and sink t
										   vector <int> & segs	// output: n-vector, n is the number of vertices in residual graph, this is the segmentation obtained when max flow is reached,
																//         segs[i] = 1 means that vertex i is segmented to Set S, segs[i] = 0 means that vertex j is segmented to Set T.
										   )
{
	int i;

	vector <int> augPath;

	// the capacity of each edge in a valid augmenting path should be positive
	double thresh_w = 0;

	// the minimum edge weight of a augmenting path
	double min_w;

	// find the augmenting path for the very first time using breadth-first search
	int n_e = BFS(graph, s, t, augPath, &thresh_w, &min_w);

	double f = 0;

	while (n_e > 0) // there is a path
	{
		for (i=0;i<augPath.size()-1;i++)
		{
			graph.addEdgeValue(augPath[i], augPath[i+1], -min_w); // all the edges weights along this augmenting path in residual graph subtract the minimum weight
			graph.addEdgeValue(augPath[i+1], augPath[i],  min_w); // all the edges weights along reverse augmenting path in residual graph add the minimum weight
		}
		
		f += min_w;

		n_e = BFS(graph, s, t, augPath, &thresh_w, &min_w);
	}

	segs = augPath; // segmentation result, vertices are segmented to Set S and T, which contains source s and sink t respectively

	return f;
}

/*------------------------------------------------------------------------------------*/


uchar DeepVoid::GetCensusTransform_BitString_33(const Mat & img,	// input: the original graylevel image
												int x, int y		// input: the pixel to be checked
												)
{
	int i,j;

	uchar val0 = img.at<uchar>(y,x);

	int l = x-1;
	int t = y-1;

	uchar census = 0;

	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			if (i==1&&j==1)
			{
				continue;
			}

		 	census<<=1; // shift 1 bit to left

			uchar val = img.at<uchar>(t+i, l+j);

			if (val < val0)
			{
				++census;
			}
		}
	}

	return census;
}

// 5*5 bit string of census transform, 24 bits = 3 Bytes, so int type is used, which is 4 Bytes
UINT DeepVoid::GetCensusTransform_BitString_55(const Mat & img,	// input: the original graylevel image
											   int x, int y		// input: the pixel to be checked
											   )
{
	int i,j;

	int w = 5;
	int h = 5;
	int half_w = 2;
	int half_h = 2;

	uchar val0 = img.at<uchar>(y,x);

	int l = x-half_w;
	int t = y-half_h;

	UINT census = 0;

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			if (i==half_h&&j==half_w)
			{
				continue;
			}

			census<<=1; // shift 1 bit to left

			uchar val = img.at<uchar>(t+i, l+j);

			if (val < val0)
			{
				++census;
			}
		}
	}

	return census;
}

// 9*7 bit string of census transform, 62 bits = 7.75 Bytes, so double type is used, which is 8 Bytes
uint64 DeepVoid::GetCensusTransform_BitString_97(const Mat & img,	// input: the original graylevel image
												 int x, int y		// input: the pixel to be checked
												 )
{
	int i,j;

	int w = 9;
	int h = 7;
	int half_w = 4;
	int half_h = 3;

	uchar val0 = img.at<uchar>(y,x);

	int l = x-half_w;
	int t = y-half_h;

	uint64 census = 0;

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			if (i==half_h&&j==half_w)
			{
				continue;
			}

			census<<=1; // shift 1 bit to left

			uchar val = img.at<uchar>(t+i, l+j);

			if (val < val0)
			{
				++census;
			}
		}
	}

	return census;
}

uchar DeepVoid::hamdist(uchar x, uchar y)
{
	uchar dist = 0, val = x^y; // XOR

	// Count the number of set bits
	while(val)
	{
		++dist; 
		val &= val-1;
	}

	return dist;
}

UINT DeepVoid::hamdist(UINT x, UINT y)
{
	UINT dist = 0, val = x^y; // XOR

	// Count the number of set bits
	while(val)
	{
		++dist; 
		val &= val-1;
	}

	return dist;
}

uint64 DeepVoid::hamdist(uint64 x, uint64 y)
{
	uint64 dist = 0, val = x^y; // XOR

	// Count the number of set bits
	while(val)
	{
		++dist; 
		val &= val-1;
	}

	return dist;
}

// Semi global matching algorithm, implemented based on <Stereo Processing by Semiglobal Matching and Mutual Information> 2008
// 20150317, input the cv::Mat
void DeepVoid::SemiGlobalMatching(const Mat & mImgB,				// input: the epipolar-rectified base image, should be graylevel
								  const Mat & mImgM,				// input: the epipolar-rectified matching image, should be graylevel
								  int dmin,							// input: scalar, the minimal disparity
								  int dmax,							// input: scalar, the maximal disparity
								  double P1,						// input: scalar, constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
								  double P2,						// input: scalar, a larger constant penalty for all larger disparity changes
								  Mat & mDI,						// output:the disparity image
								  bool bVertical /*= false*/,		// input: whether the layout is vertical or horizontal 
								  SGM_PATHS nDir /*= SGM_PATHS_8*/,	// input: the amount of scanlines*/
								  int constcCheck /*= -1*/,			// input: if matching is conducted reversely, i.e. treat the match image as a base image and vice versa.
								  bool bSubPix /*= true*/			// input: if sub pixel interpolation for sub pixel disparities is applied
								  )
{
	mDI = Mat(mImgB.rows, mImgB.cols, CV_32FC1);

	int imgWidth, imgHeight;

	if (!bVertical)
	{
		imgHeight = mImgB.rows;
		imgWidth = mImgB.cols;
	} 
	else // if the stereo layout is vertical then transpose the stereo images
	{
		imgHeight = mImgB.cols;
		imgWidth = mImgB.rows;
	}

	int imgSize = imgHeight * imgWidth;

	BYTE ** pImgB = new BYTE * [imgHeight];
	pImgB[0] = new BYTE [imgSize];
	memset(pImgB[0], 0, imgSize * sizeof(BYTE));
	int i, j;
	for (i = 1; i < imgHeight; i++)
	{
		pImgB[i] = pImgB[i - 1] + imgWidth;
	}

	BYTE ** pImgM = new BYTE * [imgHeight];
	pImgM[0] = new BYTE [imgSize];
	memset(pImgM[0], 0, imgSize * sizeof(BYTE));
	for (i = 1; i < imgHeight; i++)
	{
		pImgM[i] = pImgM[i - 1] + imgWidth;
	}

	double ** pDI = new double * [imgHeight];
	pDI[0] = new double [imgSize];
	memset(pDI[0], 0, imgSize * sizeof(double));
	for (i = 1; i < imgHeight; i++)
	{
		pDI[i] = pDI[i - 1] + imgWidth;
	}

	if (!bVertical)
	{
		for (i=0;i<imgHeight;i++)
		{
			for (j=0;j<imgWidth;j++)
			{
				uchar gray = mImgB.at<uchar>(i,j);
				pImgB[i][j] = gray;

				gray = mImgM.at<uchar>(i,j);
				pImgM[i][j] = gray;
			}
		}
	} 
	else
	{
		for (i=0;i<imgHeight;i++)
		{
			for (j=0;j<imgWidth;j++)
			{
				uchar gray = mImgB.at<uchar>(j,i);
				pImgB[i][j] = gray;

				gray = mImgM.at<uchar>(j,i);
				pImgM[i][j] = gray;
			}
		}
	}
	
	// Start SGM for the base image
	SemiGlobalMatching(imgWidth, imgHeight, pImgB, pImgM, dmin, dmax, P1, P2, pDI, nDir, constcCheck, bSubPix);

	Mat mDisparity(mImgB.rows, mImgB.cols, CV_8UC3);

	double factor = 255.0/(dmax-dmin);

	if (!bVertical)
	{
		for (i=0;i<imgHeight;i++)
		{
			for (j=0;j<imgWidth;j++)
			{
				double d = pDI[i][j];
				mDI.at<float>(i,j) = d;

				if (d<-1.5) // d=-2, mismatched
				{
					mDisparity.at<Vec3b>(i,j)[0] = 0; mDisparity.at<Vec3b>(i,j)[1] = 0; mDisparity.at<Vec3b>(i,j)[2] = 255;
				} 
				else if (d<=0) // d=-1, occluded
				{
					mDisparity.at<Vec3b>(i,j)[0] = 255; mDisparity.at<Vec3b>(i,j)[1] = 0; mDisparity.at<Vec3b>(i,j)[2] = 0;
				}
				else // valid disparity
				{
					mDisparity.at<Vec3b>(i,j)[0] = mDisparity.at<Vec3b>(i,j)[1] = mDisparity.at<Vec3b>(i,j)[2] = FTOI((d-dmin)*factor);
				}
			}
		}
	} 
	else
	{
		for (i=0;i<imgHeight;i++)
		{
			for (j=0;j<imgWidth;j++)
			{
				double d = pDI[i][j];
				mDI.at<float>(j,i) = d;

				if (d<-1.5) // d=-2, mismatched
				{
					mDisparity.at<Vec3b>(j,i)[0] = 0; mDisparity.at<Vec3b>(j,i)[1] = 0; mDisparity.at<Vec3b>(j,i)[2] = 255;
				} 
				else if (d<=0) // d=-1, occluded
				{
					mDisparity.at<Vec3b>(j,i)[0] = 255; mDisparity.at<Vec3b>(j,i)[1] = 0; mDisparity.at<Vec3b>(j,i)[2] = 0;
				}
				else // valid disparity
				{
					mDisparity.at<Vec3b>(j,i)[0] = mDisparity.at<Vec3b>(j,i)[1] = mDisparity.at<Vec3b>(j,i)[2] = FTOI((d-dmin)*factor);
				}
			}
		}
	}
	
//	imwrite("C:\\Users\\DeepVoid\\Desktop\\disparity by SGM (ZZK).bmp", mDisparity);
	imwrite("D:\\stereo\\disparity by SGM (ZZK).bmp", mDisparity);

	// set the baseline and focal length
// 	double b = 0.283401295273638;
// 	double ff = 1855/n_sampPix;
// 
// 	double cx = 1296/n_sampPix;
// 	double cy = 1026.98084691563/n_sampPix;
// 
// 	FILE * pfile = fopen("C:\\Users\\DeepVoid\\Desktop\\cloud.txt", "w");
// 	for (i=0;i<imgHeight;i++)
// 	{
// 		for (j=0;j<imgWidth;j++)
// 		{
// 			if (pDI[i][j] <= 0)
// 			{
// 				continue;
// 			}
// 
// 			double b_d = b/pDI[i][j];
// 			double Z = b_d * ff;
// 			double X = b_d * (j - cx);
// 			double Y = b_d * (i - cy);
// 
// 			fprintf(pfile, "%lf;%lf;%lf;%d;%d;%d\n",
// 				X, Y, Z, UCHAR(pImgB[i][j]), UCHAR(pImgB[i][j]), UCHAR(pImgB[i][j]));
// 		}
// 	}
// 	fclose(pfile);
// 
// 	pfile = fopen("C:\\Users\\DeepVoid\\Desktop\\depth map.txt", "w");
// 	for (i=0;i<imgHeight;i++)
// 	{
// 		for (j=0;j<imgWidth;j++)
// 		{
// 			fprintf(pfile, "%lf	", pDI[i][j]);
// 		}
// 		fprintf(pfile, "\n");
// 	}
// 	fclose(pfile);

	// release memory
	delete [] pImgB[0];
	delete [] pImgB;

	delete [] pImgM[0];
	delete [] pImgM;

	delete [] pDI[0];
	delete [] pDI;
}
