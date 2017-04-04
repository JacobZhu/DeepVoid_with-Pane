#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "curand_kernel.h"
//#include "helper_cuda.h"
//#include "stdafx.h"
//#include "thrust\sort.h"
//#include "thrust\execution_policy.h"
//#include "device_functions.h"
//#include "math_functions.h"
//#include "thrust 1.8.2\sort.h"


extern "C" void
forCUDA_ShowInfo(const char * info);

extern "C" void
forCUDA_SaveMatAsImage(const char * info,	// input: output path
					   const double * mat,	// input: the mat
					   int w, int h,		// input: the width and height of the mat
					   double valmin,		// input: the minimum value of the mat
					   double valmax		// input: the maximum value of the mat
					   );


//namespace DeepVoid_CUDA
//{

// 两个 double 型的变量互换数值
__host__ __device__ void
Swap_double(double & A, double & B)
{
	double temp = B;
	B = A;
	A = temp;
}

// 两个 int 型的变量互换数值
__host__ __device__ void
Swap_int(int & A, int & B)
{
	int temp = B;
	B = A;
	A = temp;
}

// 20161207, computation of matching cost based on NCC (normalized cross-correlation)
// return 0<=val<=2, or -1 if either a or b or both are flat (all array elements are the same)
// ncc(a,b)=ncc(a,kb) (k>0), ncc(a,b)=-ncc(a,kb) (k<0), ncc(a,b)=ncc(a,k+b)
__host__ __device__ double
matchingcost_ncc(const double * a, const double * b, int n)
{
	double sum_a = 0;
	double sum_b = 0;

	for (int i=0; i<n; ++i)
	{
		sum_a += a[i];
		sum_b += b[i];
	}

	double inv_n = 1.0 / n;

	double mean_a = sum_a * inv_n;
	double mean_b = sum_b * inv_n;

	double sum_ai_a_bi_b = 0;
	double sum_ai_a_ai_a = 0;
	double sum_bi_b_bi_b = 0;

	for (int i=0; i<n; ++i)
	{
		double ai_a = a[i] - mean_a;
		double bi_b = b[i] - mean_b;

		sum_ai_a_bi_b += ai_a*bi_b; // sum(ai-a)(bi-b)
		sum_ai_a_ai_a += ai_a*ai_a; // sum(ai-a)(ai-a), the variance or standard deviation of array a
		sum_bi_b_bi_b += bi_b*bi_b; // sum(bi-b)(bi-b), the variance or standard deviation of array b
	}

	double sum_ab = sum_ai_a_ai_a * sum_bi_b_bi_b;

	double rsqrt_sum_ab = rsqrt(sum_ab);

	if (isinf(rsqrt_sum_ab))
	{
		// sum_ab is zero, which means either array a or b or both are flat (all elements are the same)
		return -1;
	}
	else
	{
		double ncc = 1 - sum_ai_a_bi_b*rsqrt_sum_ab; // 0<=val<=2

		return ncc;
	}
}

// 对 1 维数组进行冒泡法排序，可以升序排列也可降序排列，并输出原数组各元素索引在排完序的新数组中的排列
__host__ __device__ void
Sort_Bubble(double * p,					// 输入兼输出：待排序的数组，函数执行完后，数组中的元素将按升序或者降序排列
			int n,						// 输入：数组长度，元素个数
			int * idx,					// 输出：排序完成后各元素原来的索引号的排列情况，索引从 0 开始
			bool bIsAscent /*= TRUE*/	// 输入：是升序，还是降序排列
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

// compute the matching cost, proposed by S. Birchfield and C. Tomasi, for a single pixel.
// disparity is along x-axis
__host__ __device__ double 
ComputeMatchingCostforOnePixel_x_BT(int x, int y,				// the coordinates in base image of the checked pixel
									int d,						// the disparity to be checked
									int w, int h,				// the width and height of stereo images
									const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
									const unsigned char * imgm	// input: h*w, the rectified grayscale matching image
									) 
{
	// if the point to be checked xm = x-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	// if d>0, then xm is on the left of x, otherwise, on the right of x
	int xm = x-d;

	if (xm <= 0 || xm >= (w-1) || x <= 0 || x >= (w-1))
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	int yw = y * w;

	double fm   = (double)imgm[yw + xm];
	double fm_l = (double)imgm[yw + xm - 1];
	double fm_r = (double)imgm[yw + xm + 1];

	double fm_l_2 = (fm + fm_l) * 0.5;
	double fm_r_2 = (fm + fm_r) * 0.5;

	double pfm[3]; int idx[3];
	pfm[0] = fm; pfm[1] = fm_l_2; pfm[2] = fm_r_2;
//	thrust::sort(thrust::device, pfm, pfm + 3);
	Sort_Bubble(pfm, 3, idx, true);
	double fmmin = pfm[0];
	double fmmax = pfm[2];
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	double fb   = double(imgb[yw + x]);
	double fb_l = double(imgb[yw + x - 1]);
	double fb_r = double(imgb[yw + x + 1]);

	double fb_l_2 = (fb + fb_l) * 0.5;
	double fb_r_2 = (fb + fb_r) * 0.5;

	double pfb[3];
	pfb[0] = fb; pfb[1] = fb_l_2; pfb[2] = fb_r_2;
//	thrust::sort(thrust::device, pfb, pfb + 3);
	Sort_Bubble(pfb, 3, idx, true);
	double fbmin = pfb[0];
	double fbmax = pfb[2];
	//////////////////////////////////////////////////////////////////////////

	// in this way
	pfm[0]=0; pfm[1]=fb-fmmax; pfm[2]=fmmin-fb;
//	thrust::sort(thrust::device, pfm, pfm + 3);
	Sort_Bubble(pfm, 3, idx, true);
	double ffm = pfm[2];

	pfb[0]=0; pfb[1]=fm-fbmax; pfb[2]=fbmin-fm;
//	thrust::sort(thrust::device, pfb, pfb + 3);
	Sort_Bubble(pfb, 3, idx, true);
	double ffb = pfb[2];

	if (ffb <= ffm)
	{
		return ffb;
	}

	return ffm;
}

// compute the matching cost, proposed by S. Birchfield and C. Tomasi, for a single pixel.
// disparity is along y-axis
__host__ __device__ double 
ComputeMatchingCostforOnePixel_y_BT(int x, int y,				// input: the coordinates in base image of the checked pixel
									int d,						// input: the disparity to be checked
									int w, int h,				// input: the width and height of stereo images
									const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
									const unsigned char * imgm	// input: h*w, the rectified grayscale matching image
									)
{
	// if the point to be checked ym = y-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	// if d>0, then ym is on the top of y, otherwise, on the bottom of y
	int ym = y - d;

	if (ym <= 0 || ym >= (h - 1) || y <= 0 || y >= (h - 1))
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	int ymw = ym*w;

	double fm   = (double)imgm[ymw + x];
	double fm_l = (double)imgm[ymw - w + x];
	double fm_r = (double)imgm[ymw + w + x];

	double fm_l_2 = (fm + fm_l) * 0.5;
	double fm_r_2 = (fm + fm_r) * 0.5;

	double pfm[3]; int idx[3];
	pfm[0] = fm; pfm[1] = fm_l_2; pfm[2] = fm_r_2;
//	thrust::sort(/*thrust::device,*/ pfm, pfm + 3);
	Sort_Bubble(pfm, 3, idx, true);
	double fmmin = pfm[0];
	double fmmax = pfm[2];
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	int yw = y*w;

	double fb   = double(imgb[yw + x]);
	double fb_l = double(imgb[yw - w + x]);
	double fb_r = double(imgb[yw + w + x]);

	double fb_l_2 = (fb + fb_l) * 0.5;
	double fb_r_2 = (fb + fb_r) * 0.5;

	double pfb[3];
	pfb[0] = fb; pfb[1] = fb_l_2; pfb[2] = fb_r_2;
//	thrust::sort(/*thrust::device,*/ pfb, pfb + 3);
	Sort_Bubble(pfb, 3, idx, true);
	double fbmin = pfb[0];
	double fbmax = pfb[2];
	//////////////////////////////////////////////////////////////////////////

	// in this way
	pfm[0] = 0; pfm[1] = fb - fmmax; pfm[2] = fmmin - fb;
//	thrust::sort(/*thrust::device,*/ pfm, pfm + 3);
	Sort_Bubble(pfm, 3, idx, true);
	double ffm = pfm[2]; // got to be non-negative

	pfb[0] = 0; pfb[1] = fm - fbmax; pfb[2] = fbmin - fm;
//	thrust::sort(/*thrust::device, */pfb, pfb + 3);
	Sort_Bubble(pfb, 3, idx, true);
	double ffb = pfb[2]; // got to be non-negative

	if (ffb <= ffm)
	{
		return ffb;
	}

	return ffm;
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along x-axis
__host__ __device__ double 
ComputeMatchingCostforOnePixel_x_ncc(int x, int y,				// the coordinates in base image of the checked pixel
									 int d,						// the disparity to be checked
									 int w, int h,				// the width and height of stereo images
									 int hw, int hh,			// the half width and half height of the support window
									 const unsigned char * imgb,// input: h*w, the rectified grayscale base image
									 const unsigned char * imgm	// input: h*w, the rectified grayscale matching image
									 ) 
{
	// if the point to be checked xm = x-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	// if d>0, then xm is on the left of x, otherwise, on the right of x
	int xm = x-d;

	if ((y-hh) < 0 || (y+hh) >= h || (x-hw) < 0 || (x+hw) >= w || (xm-hw) < 0 || (xm+hw) >= w)
	{
		return -1;
	}

	int n = (2 * hh + 1)*(2 * hw + 1);
	double inv_n = 1.0 / n;

	//////////////////////////////////////////////////////////////////////////
	// compute the means of each support windows
	double sum_b = 0;
	double sum_m = 0;

	for (int i = -hh; i <= hh; ++i)
	{
		for (int j = -hw; j <= hw; ++j)
		{
			sum_b += imgb[(y + i)*w + (x + j)];
			sum_m += imgm[(y + i)*w + (xm + j)];
		}
	}

	double mean_b = sum_b*inv_n;
	double mean_m = sum_m*inv_n;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// compute the standard deviation and final ncc value
	double sum_bijmij = 0;
	double sum_bijbij = 0;
	double sum_mijmij = 0;

	for (int i = -hh; i <= hh; ++i)
	{
		for (int j = -hw; j <= hw; ++j)
		{
			double bij = imgb[(y + i)*w + (x + j)] - mean_b;
			double mij = imgm[(y + i)*w + (xm + j)] - mean_m;

			sum_bijmij += bij*mij;
			sum_bijbij += bij*bij;
			sum_mijmij += mij*mij;
		}
	}

	double bbmm = sum_bijbij * sum_mijmij;

	double rsqrt_bbmm = rsqrt(bbmm);

	if (isinf(rsqrt_bbmm))
	{
		// sum_ab is zero, which means either array a or b or both are flat (all elements are the same)
		return -1;
	}
	else
	{
		double ncc = 1 - sum_bijmij*rsqrt_bbmm; // 0<=val<=2

		return ncc;
	}
	//////////////////////////////////////////////////////////////////////////
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along y-axis
__host__ __device__ double 
ComputeMatchingCostforOnePixel_y_ncc(int x, int y,				// input: the coordinates in base image of the checked pixel
									 int d,						// input: the disparity to be checked
									 int w, int h,				// input: the width and height of stereo images
									 int hw, int hh,			// the half width and half height of the support window
									 const unsigned char * imgb,// input: h*w, the rectified grayscale base image
									 const unsigned char * imgm	// input: h*w, the rectified grayscale matching image
									 )
{
	// if the point to be checked ym = y-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	// if d>0, then ym is on the top of y, otherwise, on the bottom of y
	int ym = y - d;

	if ((x - hw) < 0 || (x + hw) >= w || (y - hh) < 0 || (y + hh) >= h || (ym - hh) < 0 || (ym + hh) >= h)
	{
		return -1;
	}

	int n = (2 * hh + 1)*(2 * hw + 1);
	double inv_n = 1.0 / n;

	//////////////////////////////////////////////////////////////////////////
	// compute the means of each support windows
	double sum_b = 0;
	double sum_m = 0;

	for (int i = -hh; i <= hh; ++i)
	{
		for (int j = -hw; j <= hw; ++j)
		{
			sum_b += imgb[(y + i)*w + (x + j)];
			sum_m += imgm[(ym + i)*w + (x + j)];
		}
	}

	double mean_b = sum_b*inv_n;
	double mean_m = sum_m*inv_n;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// compute the standard deviation and final ncc value
	double sum_bijmij = 0;
	double sum_bijbij = 0;
	double sum_mijmij = 0;

	for (int i = -hh; i <= hh; ++i)
	{
		for (int j = -hw; j <= hw; ++j)
		{
			double bij = imgb[(y + i)*w + (x + j)] - mean_b;
			double mij = imgm[(ym + i)*w + (x + j)] - mean_m;

			sum_bijmij += bij*mij;
			sum_bijbij += bij*bij;
			sum_mijmij += mij*mij;
		}
	}

	double bbmm = sum_bijbij * sum_mijmij;

	double rsqrt_bbmm = rsqrt(bbmm);

	if (isinf(rsqrt_bbmm))
	{
		// sum_ab is zero, which means either array a or b or both are flat (all elements are the same)
		return -1;
	}
	else
	{
		double ncc = 1 - sum_bijmij*rsqrt_bbmm; // 0<=val<=2

		return ncc;
	}
	//////////////////////////////////////////////////////////////////////////
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along x-axis
__host__ __device__ double 
ComputeMatchingCostforOnePixel_x_opencvncc(int x, int y,				// the coordinates in base image of the checked pixel
										   int d,						// the disparity to be checked
										   int w, int h,				// the width and height of stereo images
										   int hw, int hh,				// the half width and half height of the support window
										   const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
										   const unsigned char * imgm	// input: h*w, the rectified grayscale matching image
										   )
{
	// if the point to be checked xm = x-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	// if d>0, then xm is on the left of x, otherwise, on the right of x
	int xm = x-d;

	if ((y-hh) < 0 || (y+hh) >= h || (x-hw) < 0 || (x+hw) >= w || (xm-hw) < 0 || (xm+hw) >= w)
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// compute the standard deviation and final ncc value
	double sum_bijmij = 0;
	double sum_bijbij = 0;
	double sum_mijmij = 0;

	for (int i = -hh; i <= hh; ++i)
	{
		for (int j = -hw; j <= hw; ++j)
		{
			double bij = imgb[(y + i)*w + (x + j)];
			double mij = imgm[(y + i)*w + (xm + j)];

			sum_bijmij += bij*mij;
			sum_bijbij += bij*bij;
			sum_mijmij += mij*mij;
		}
	}

	double bbmm = sum_bijbij * sum_mijmij;

	double rsqrt_bbmm = rsqrt(bbmm);

	if (isinf(rsqrt_bbmm))
	{
		// bbmm is zero, which means either array a or b or both are all zeros
		return -1;
	}
	else
	{
		double ncc = 1 - sum_bijmij*rsqrt_bbmm; // 0<=val<=1

		return ncc;
	}
	//////////////////////////////////////////////////////////////////////////
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along y-axis
__host__ __device__ double 
ComputeMatchingCostforOnePixel_y_opencvncc(int x, int y,				// input: the coordinates in base image of the checked pixel
										   int d,						// input: the disparity to be checked
										   int w, int h,				// input: the width and height of stereo images
										   int hw, int hh,				// the half width and half height of the support window
										   const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
										   const unsigned char * imgm	// input: h*w, the rectified grayscale matching image
										   )
{
	// if the point to be checked ym = y-d is out of border of the matching image,
	// the matching cost is designated with an invalid number, e.g. negative number
	// if d>0, then ym is on the top of y, otherwise, on the bottom of y
	int ym = y - d;

	if ((x - hw) < 0 || (x + hw) >= w || (y - hh) < 0 || (y + hh) >= h || (ym - hh) < 0 || (ym + hh) >= h)
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// compute the standard deviation and final ncc value
	double sum_bijmij = 0;
	double sum_bijbij = 0;
	double sum_mijmij = 0;

	for (int i = -hh; i <= hh; ++i)
	{
		for (int j = -hw; j <= hw; ++j)
		{
			double bij = imgb[(y + i)*w + (x + j)];
			double mij = imgm[(ym + i)*w + (x + j)];

			sum_bijmij += bij*mij;
			sum_bijbij += bij*bij;
			sum_mijmij += mij*mij;
		}
	}

	double bbmm = sum_bijbij * sum_mijmij;

	double rsqrt_bbmm = rsqrt(bbmm);

	if (isinf(rsqrt_bbmm))
	{
		// bbmm is zero, which means either array a or b or both are all zeros
		return -1;
	}
	else
	{
		double ncc = 1 - sum_bijmij*rsqrt_bbmm; // 0<=val<=1

		return ncc;
	}
	//////////////////////////////////////////////////////////////////////////
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along y-axis
__host__ __device__ void 
previous_ij(int i, int j,				// input: image indices of current pixel
			int & i_pre, int & j_pre,	// output:image indices of previous pixel
	        int dir						// input: 0:(0,-1), 1:(1,-1), 2:(1,0), 3:(1,1), 4:(0,1), 5:(-1,1), 6:(-1,0), 7:(-1,-1)
			)
{
	i_pre = i;
	j_pre = j;

	if (0 == dir)
	{
		--i_pre;
	}
	else if (1 == dir)
	{
		--i_pre;
		++j_pre;
	}
	else if (2 == dir)
	{
		++j_pre;
	}
	else if (3 == dir)
	{
		++i_pre;
		++j_pre;
	}
	else if (4 == dir)
	{
		++i_pre;
	}
	else if (5 == dir)
	{
		++i_pre;
		--j_pre;
	}
	else if (6 == dir)
	{
		--j_pre;
	}
	else if (7 == dir)
	{
		--i_pre;
		--j_pre;
	}
	else
	{

	}
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along y-axis
__host__ __device__ void 
CostAggregation_OnePixel(int w, int h, int nd,	// input: the width and height of stereo images
						 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						 int i, int j,			// input: image indices of current pixel
						 int i_pre, int j_pre,	// input: image indices of previous pixel
						 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
						 double P2				// input: a larger constant penalty for all larger disparity changes
						 )
{
	if (i<0 || i>=h || j<0 || j>=w || i_pre<0 || i_pre>= h || j_pre<0 || j_pre>=w) // cross the boundary
	{
		return;
	}

	int nPix = w*h; // number of image pixels
	int wi_j_cur = w*i + j;
	int wi_j_pre = w*i_pre + j_pre;

	for (int k = 0; k < nd; ++k)
	{
		double val_cur_k = C[nPix*k + wi_j_cur]; // matching cost of current pixel at disparity level k

		if (val_cur_k < 0)
		{
			continue;
		}
		
		bool bAllNeg = true; // all aggregated costs of previous pixel are negative

		double minPre;  // record the min nonnegative value of aggregated costs of previous pixel
		double minCost; // record the min cost aggregated from all valid disparity levels of previous pixel

		for (int k1 = 0; k1 < nd; ++k1)
		{
			double val_pre_k1 = C[nPix*k1 + wi_j_pre]; // aggregated cost of previous pixel at disparity level k1

			if (val_pre_k1 < 0)
			{
				continue;
			}

			double curCost = val_pre_k1;

			if (k == k1) // the same disparity level
			{
				// do nothing
			}
			else if (abs(k - k1) == 1) // the disparity difference is 1 pixel
			{
				curCost += P1;
			}
			else // the disparity difference is 2 or more pixels
			{
				curCost += P2;
			}

			if (bAllNeg)
			{
				// arriving here means that the first non-negative aggregated cost value of previous pixel is found
				minPre = val_pre_k1;
				minCost = curCost;
				bAllNeg = false;
			}
			else
			{
				if (val_pre_k1 < minPre)
				{
					minPre = val_pre_k1;
				}
				
				if (curCost < minCost)
				{
					minCost = curCost;
				}
			}
		}

		if (bAllNeg) // break outta the loop if all aggregated costs of previous pixel are really negative
		{
			break;
		}

		C[nPix*k + wi_j_cur] += (minCost - minPre);
	}
}

// 20161226, new DSI layout
__host__ __device__ void 
CostAggregation_OnePixel_new(int w, int h, int nd,	// input: the width and height of stereo images
							 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
							 int i, int j,			// input: image indices of current pixel
							 int i_pre, int j_pre,	// input: image indices of previous pixel
							 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
							 double P2				// input: a larger constant penalty for all larger disparity changes
							 )
{
	int offset_cur = (w*i + j)*nd;
	int offset_pre = (w*i_pre + j_pre)*nd;

	for (int k = 0; k < nd; ++k)
	{
		double & val_cur_k = C[offset_cur + k]; // matching cost of current pixel at disparity level k

		if (val_cur_k < 0)
		{
			continue;
		}
		
		bool bAllNeg = true; // all aggregated costs of previous pixel are negative

		double minPre;  // record the min nonnegative value of aggregated costs of previous pixel
		double minCost; // record the min cost aggregated from all valid disparity levels of previous pixel

		for (int k1 = 0; k1 < nd; ++k1)
		{
			double val_pre_k1 = C[offset_pre + k1]; // aggregated cost of previous pixel at disparity level k1

			if (val_pre_k1 < 0)
			{
				continue;
			}

			double curCost = val_pre_k1;

			if (k == k1) // the same disparity level
			{
				// do nothing
			}
			else if (abs(k - k1) == 1) // the disparity difference is 1 pixel
			{
				curCost += P1;
			}
			else // the disparity difference is 2 or more pixels
			{
				curCost += P2;
			}

			if (bAllNeg)
			{
				// arriving here means that the first non-negative aggregated cost value of previous pixel is found
				minPre = val_pre_k1;
				minCost = curCost;
				bAllNeg = false;
			}
			else
			{
				if (val_pre_k1 < minPre)
				{
					minPre = val_pre_k1;
				}
				
				if (curCost < minCost)
				{
					minCost = curCost;
				}
			}
		}

		if (bAllNeg) // break outta the loop if all aggregated costs of previous pixel are really negative
		{
			break;
		}

		val_cur_k += (minCost - minPre);
	}
}

// 20161228, new DSI layout
__host__ __device__ void 
CostAggregation_OnePixel_new(int w, int h, int nd,	// input: the width and height of stereo images
							 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
							 int i, int j, int k,	// input: image indices of current pixel, and index of current disparity level
							 int i_pre, int j_pre,	// input: image indices of previous pixel
							 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
							 double P2				// input: a larger constant penalty for all larger disparity changes
							 )
{
	int offset_cur = (w*i + j)*nd;
	int offset_pre = (w*i_pre + j_pre)*nd;

	double & val_cur_k = C[offset_cur + k]; // matching cost of current pixel at disparity level k

	if (val_cur_k < 0)
	{
		return;
	}
		
	bool bAllNeg = true; // all aggregated costs of previous pixel are negative

	double minPre;  // record the min nonnegative value of aggregated costs of previous pixel
	double minCost; // record the min cost aggregated from all valid disparity levels of previous pixel

	for (int k1 = 0; k1 < nd; ++k1)
	{
		double val_pre_k1 = C[offset_pre + k1]; // aggregated cost of previous pixel at disparity level k1

		if (val_pre_k1 < 0)
		{
			continue;
		}

		double curCost = val_pre_k1;

		if (k == k1) // the same disparity level
		{
			// do nothing
		}
		else if (abs(k - k1) == 1) // the disparity difference is 1 pixel
		{
			curCost += P1;
		}
		else // the disparity difference is 2 or more pixels
		{
			curCost += P2;
		}

		if (bAllNeg)
		{
			// arriving here means that the first non-negative aggregated cost value of previous pixel is found
			minPre = val_pre_k1;
			minCost = curCost;
			bAllNeg = false;
		}
		else
		{
			if (val_pre_k1 < minPre)
			{
				minPre = val_pre_k1;
			}
				
			if (curCost < minCost)
			{
				minCost = curCost;
			}
		}
	}

	if (bAllNeg) // break outta the loop if all aggregated costs of previous pixel are really negative
	{
		return;
	}

	val_cur_k += (minCost - minPre);
}

__host__ __device__ void 
CostAggregation_OnePixel(int w, int h, int nd,	// input: the width and height of stereo images
						 int threadIdx,			// input: the index of current thread within the thread block
						 const double * s_pre,	// input: shared memory of aggregated costs of all previous pixels within the same thread block
						 double * C,			// input & output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						 int i, int j,			// input: image indices of current pixel
						 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
						 double P2				// input: a larger constant penalty for all larger disparity changes
						 )
{
	if (i<0 || i>=h || j<0 || j>=w) // cross the boundary
	{
		return;
	}

	int nPix = w*h; // number of image pixels
	int wi_j_cur = w*i + j;
	int tidxnd = threadIdx*nd;

	for (int k = 0; k < nd; ++k)
	{
		double & val_cur_k = C[nPix*k + wi_j_cur]; // matching cost of current pixel at disparity level k

		if (val_cur_k < 0)
		{
			continue;
		}
		
		bool bAllNeg = true; // all aggregated costs of previous pixel are negative

		double minPre;  // record the min nonnegative value of aggregated costs of previous pixel
		double minCost; // record the min cost aggregated from all valid disparity levels of previous pixel

		for (int k1 = 0; k1 < nd; ++k1)
		{
			double val_pre_k1 = s_pre[tidxnd + k1]; // aggregated cost of previous pixel at disparity level k1

			if (val_pre_k1 < 0)
			{
				continue;
			}

			double curCost = val_pre_k1;

			if (k == k1) // the same disparity level
			{
				// do nothing
			}
			else if (abs(k - k1) == 1) // the disparity difference is 1 pixel
			{
				curCost += P1;
			}
			else // the disparity difference is 2 or more pixels
			{
				curCost += P2;
			}

			if (bAllNeg)
			{
				// arriving here means that the first non-negative aggregated cost value of previous pixel is found
				minPre = val_pre_k1;
				minCost = curCost;
				bAllNeg = false;
			}
			else
			{
				if (val_pre_k1 < minPre)
				{
					minPre = val_pre_k1;
				}
				
				if (curCost < minCost)
				{
					minCost = curCost;
				}
			}
		}

		if (bAllNeg) // break outta the loop if all aggregated costs of previous pixel are really negative
		{
			break;
		}

		val_cur_k += (minCost - minPre);
	}
}

// compute the matching cost based on normalized cross-correlation (ncc).
// disparity is along y-axis
__host__ __device__ void 
CostAggregation_OnePixel(double * cur,			// input & output: it is DSI values as input, aggregated costs as output
						 const double * pre,	// input: it is aggregated costs of the previous pixel
						 int n,					// input: number of disparity levels
						 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
						 double P2				// input: a larger constant penalty for all larger disparity changes
						 )
{
	for (int k = 0; k < n; ++k)
	{
		double & val_cur_k = cur[k]; // matching cost of current pixel at disparity level k

		if (val_cur_k < 0)
		{
			continue;
		}
		
		bool bAllNeg = true; // all aggregated costs of previous pixel are negative

		double minPre;  // record the min nonnegative value of aggregated costs of previous pixel
		double minCost; // record the min cost aggregated from all valid disparity levels of previous pixel

		for (int k1 = 0; k1 < n; ++k1)
		{
			double val_pre_k1 = pre[k1]; // aggregated cost of previous pixel at disparity level k1

			if (val_pre_k1 < 0)
			{
				continue;
			}

			double curCost = val_pre_k1;

			if (k == k1) // the same disparity level
			{
				// do nothing
			}
			else if (abs(k - k1) == 1) // the disparity difference is 1 pixel
			{
				curCost += P1;
			}
			else // the disparity difference is 2 or more pixels
			{
				curCost += P2;
			}

			if (bAllNeg)
			{
				// arriving here means that the first non-negative aggregated cost value of previous pixel is found
				minPre = val_pre_k1;
				minCost = curCost;
				bAllNeg = false;
			}
			else
			{
				if (val_pre_k1 < minPre)
				{
					minPre = val_pre_k1;
				}
				
				if (curCost < minCost)
				{
					minCost = curCost;
				}
			}
		}

		if (bAllNeg) // break outta the loop if all aggregated costs of previous pixel are really negative
		{
			break;
		}

		val_cur_k += (minCost - minPre);
	}
}

// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_l2r_BT(int w, int h,					// input: the width and height of stereo images
						  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						  int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
						  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						  )
{
	// the pixel indices
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= h || j >= w) // over the boundary
	{
		return;
	}

	int nd = dmax - dmin + 1; // number of disparities
	int wh = w*h; // number of pixels in the image
	int iwj = i*w + j;
	
	for (int d = dmin; d <= dmax; ++d)
	{
		int k = d - dmin; // vertical index

		DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_BT(j, i, d, w, h, imgb, imgm); // DSI[k*w*h + i*w + j]
	}
}

// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_r2l_BT(int w, int h,					// input: the width and height of stereo images
						  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						  int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
						  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						  )
{
	// the pixel indices
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= h || j >= w) // over the boundary
	{
		return;
	}

	int nd = dmax - dmin + 1; // number of disparities
	int wh = w*h; // number of pixels in the image
	int iwj = i*w + j;
	
	for (int d = dmin; d <= dmax; ++d)
	{
		int k = d - dmin; // vertical index

		DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_BT(j, i, -d, w, h, imgm, imgb); // DSI[k*w*h + i*w + j]
	}
}

// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_u2b_BT(int w, int h,					// input: the width and height of stereo images
						  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						  int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
						  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						  )
{
	// the pixel indices
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= h || j >= w) // over the boundary
	{
		return;
	}

	int nd = dmax - dmin + 1; // number of disparities
	int wh = w*h; // number of pixels in the image
	int iwj = i*w + j;
	
	for (int d = dmin; d <= dmax; ++d)
	{
		int k = d - dmin; // vertical index

		DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_BT(j, i, d, w, h, imgb, imgm); // DSI[k*w*h + i*w + j]
	}
}

// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_b2u_BT(int w, int h,					// input: the width and height of stereo images
						  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						  int dmax,						// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
						  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						  )
{
	// the pixel indices
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= h || j >= w) // over the boundary
	{
		return;
	}

	int nd = dmax - dmin + 1; // number of disparities
	int wh = w*h; // number of pixels in the image
	int iwj = i*w + j;
	
	for (int d = dmin; d <= dmax; ++d)
	{
		int k = d - dmin; // vertical index

		DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_BT(j, i, -d, w, h, imgm, imgb); // DSI[k*w*h + i*w + j]
	}
}

// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI(int w, int h,				// input: the width and height of stereo images
				   int hw, int hh,				// the half width and half height of the support window
				   const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
				   const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
				   int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
				   int dmax,					// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
				   double * DSI,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
				   int dir,						// input: 0:l2r, 1:r2l, 2:u2b, 3:b2u
				   int costType                 // input: 0:BT,  1:ncc
				   )
{
	// the pixel indices
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= h || j >= w) // over the boundary
	{
		return;
	}

	int nd = dmax - dmin + 1; // number of disparities
	int wh = w*h; // number of pixels in the image
	int iwj = i*w + j;

	if (0==dir) // l2r
	{
		if (0==costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_BT(j, i, d, w, h, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1==costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_ncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
	else if (1==dir) // r2l
	{
		if (0 == costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_BT(j, i, -d, w, h, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1 == costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_ncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
	else if (2==dir) // u2b
	{
		if (0 == costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_BT(j, i, d, w, h, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1 == costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_ncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_opencvncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
	else // b2u
	{
		if (0 == costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_BT(j, i, -d, w, h, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1 == costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_ncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[k*wh + iwj] = ComputeMatchingCostforOnePixel_y_opencvncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
}

// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_new(int w, int h,				// input: the width and height of stereo images
					   int hw, int hh,				// the half width and half height of the support window
					   const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
					   const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
					   int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
					   int dmax,					// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
					   double * DSI,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
					   int dir,						// input: 0:l2r, 1:r2l, 2:u2b, 3:b2u
					   int costType                 // input: 0:BT,  1:ncc
					   )
{
	// the pixel indices
	int i = blockDim.y * blockIdx.y + threadIdx.y;
	int j = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= h || j >= w) // over the boundary
	{
		return;
	}

	int nd = dmax - dmin + 1; // number of disparities
	int offset = (i*w + j)*nd;

	if (0==dir) // l2r
	{
		if (0==costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, d, w, h, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1==costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_x_ncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
	else if (1==dir) // r2l
	{
		if (0 == costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, -d, w, h, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1 == costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_x_ncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
	else if (2==dir) // u2b
	{
		if (0 == costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_y_BT(j, i, d, w, h, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1 == costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_y_ncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_y_opencvncc(j, i, d, w, h, hw, hh, imgb, imgm); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
	else // b2u
	{
		if (0 == costType) // BT
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_y_BT(j, i, -d, w, h, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (1 == costType) // ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_y_ncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else if (2 == costType) // opencv ncc
		{
			for (int d = dmin; d <= dmax; ++d)
			{
				int k = d - dmin; // vertical index

				DSI[offset + k] = ComputeMatchingCostforOnePixel_y_opencvncc(j, i, -d, w, h, hw, hh, imgm, imgb); // DSI[k*w*h + i*w + j]
			}
		}
		else
		{

		}
	}
}

// 20170102, input stereo images are no need to be transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_l2r_BT(int w, int h,	int nd,			// input: the width and height of stereo images
						  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						  )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(i*w + j)*nd + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, d, w, h, imgb, imgm);
	}
}

// 20170102, input stereo images are no need to be transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_l2r_ncc(int w, int h, int nd,		// input: the width and height of stereo images
						   int hw, int hh,				// the half width and half height of the support window
						   const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						   const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						   int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						   double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						   )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y;
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(i*w + j)*nd + k] = ComputeMatchingCostforOnePixel_x_ncc(j, i, d, w, h, hw, hh, imgb, imgm);
	}
}

// 20170102, input stereo images are no need to be transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_l2r_opencvncc(int w, int h, int nd,		// input: the width and height of stereo images
								 int hw, int hh,			// the half width and half height of the support window
								 const unsigned char * imgb,// input: h*w, the rectified grayscale base image
								 const unsigned char * imgm,// input: h*w, the rectified grayscale matching image
								 int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
								 double * DSI				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
								 )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y;
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(i*w + j)*nd + k] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, d, w, h, hw, hh, imgb, imgm);
	}
}

// 20170102, input stereo images are no need to be transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_r2l_BT(int w, int h,	int nd,			// input: the width and height of stereo images
						  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						  )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(i*w + j)*nd + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, -d, w, h, imgm, imgb);
	}
}

// 20170102, input stereo images are no need to be transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_r2l_ncc(int w, int h, int nd,		// input: the width and height of stereo images
						   int hw, int hh,				// the half width and half height of the support window
						   const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
						   const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
						   int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
						   double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
						   )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(i*w + j)*nd + k] = ComputeMatchingCostforOnePixel_x_ncc(j, i, -d, w, h, hw, hh, imgm, imgb);
	}
}

// 20170102, input stereo images are no need to be transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_r2l_opencvncc(int w, int h, int nd,		// input: the width and height of stereo images
								 int hw, int hh,			// the half width and half height of the support window
								 const unsigned char * imgb,// input: h*w, the rectified grayscale base image
								 const unsigned char * imgm,// input: h*w, the rectified grayscale matching image
								 int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
								 double * DSI				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
								 )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(i*w + j)*nd + k] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, -d, w, h, hw, hh, imgm, imgb);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_u2b_BT_transposed(int w, int h, int nd,			// input: the width and height of stereo images
									 const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
									 const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
									 int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
									 double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
									 )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, d, w, h, imgb, imgm);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_u2b_ncc_transposed(int w, int h, int nd,			// input: the width and height of stereo images
									  int hw, int hh,				// the half width and half height of the support window
									  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
									  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
									  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
									  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
									  )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_ncc(j, i, d, w, h, hw, hh, imgb, imgm);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_u2b_opencvncc_transposed(int w, int h, int nd,		// input: the width and height of stereo images
											int hw, int hh,				// the half width and half height of the support window
											const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
											const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
											int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
											double * DSI				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											)
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, d, w, h, hw, hh, imgb, imgm);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_b2u_BT_transposed(int w, int h, int nd,		// input: the width and height of stereo images
									 const unsigned char * imgb,// input: h*w, the rectified grayscale base image
									 const unsigned char * imgm,// input: h*w, the rectified grayscale matching image
									 int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
									 double * DSI				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
									 )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, -d, w, h, imgm, imgb);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_b2u_ncc_transposed(int w, int h, int nd,			// input: the width and height of stereo images
									  int hw, int hh,				// the half width and half height of the support window
									  const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
									  const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
									  int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
									  double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
									  )
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_ncc(j, i, -d, w, h, hw, hh, imgm, imgb);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_GenerateDSI_b2u_opencvncc_transposed(int w, int h, int nd,		// input: the width and height of stereo images
											int hw, int hh,				// the half width and half height of the support window
											const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
											const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
											int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
											double * DSI				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											)
{
	// the indices
	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
	int j = blockDim.y * blockIdx.y + threadIdx.y; 
	int i = blockDim.z * blockIdx.z + threadIdx.z;

	if (k < nd && j < w && i < h)
	{
		int d = dmin + k;

		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_opencvncc(j, i, -d, w, h, hw, hh, imgm, imgb);
	}
}

// 20170102, input stereo images are transposed
// 20161226, DSI with new layout
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
//__global__ void 
//kernel_GenerateDSI_l2r_BT_transposed(int w, int h,	int nd,			// input: the width and height of stereo images
//									 const unsigned char * imgb,	// input: h*w, the rectified grayscale base image
//									 const unsigned char * imgm,	// input: h*w, the rectified grayscale matching image
//									 int dmin,						// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
//									 double * DSI					// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
//									 )
//{
//	// the indices
//	int k = blockDim.x * blockIdx.x + threadIdx.x; // index of disparity level
//	int j = blockDim.y * blockIdx.y + threadIdx.y; 
//	int i = blockDim.z * blockIdx.z + threadIdx.z;
//
//	if (k < nd && j < w && i < h)
//	{
//		int d = dmin + k;
//
//		DSI[(j*h + i)*nd + k] = ComputeMatchingCostforOnePixel_x_BT(j, i, d, w, h, imgb, imgm);
//	}
//}

// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelLine(int w, int h, int nd,	// input: the width and height of stereo images
									   double * C,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
									   double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
									   double P2,				// input: a larger constant penalty for all larger disparity changes
									   int idx1,				// input: the row index of the parallel row when bParallelRow is true, otherwise, is the colomn index of the parallel colomn
									   bool bParallelRow,		// input: true: pixels in a row are processed in parallel, false: pixels in a colomn are processed in parallel
									   int dir					// input: 0:(0,-1), 1:(1,-1), 2:(1,0), 3:(1,1), 4:(0,1), 5:(-1,1), 6:(-1,0), 7:(-1,-1)
									   )
{
	extern __shared__ double s_pre[]; // dynamic shared memory of aggregated costs of all previous pixels within the same thread block

	if (idx1 < 0) // no matter what, idx1 has to be non-negative
	{
		return;
	}

	// the colomn index of current pixel in case bParallelRow is true, otherwise, it's the row index of current pixel in case bParallelRow is false
	int idx2 = blockDim.x * blockIdx.x + threadIdx.x;

	if (bParallelRow) // it's a parallel row
	{
		if (2 == dir || 6 == dir) // horizontal propagation is not applicable in parallel row form
		{
			return;
		}

		if (idx1 >= h || idx2 >= w) // cross the boundary
		{
			return;
		}

		// image indices of current pixel
		int i = idx1;
		int j = idx2;

		// image indices of previous pixel according to the propagation direction
		int i_pre, j_pre;
		previous_ij(i, j, i_pre, j_pre, dir);

		if (i_pre < 0 || i_pre >= h || j_pre < 0 || j_pre >= w) // previous pixel cross the boundary
		{
			return;
		}

//		CostAggregation_OnePixel(w, h, nd, C, i, j, i_pre, j_pre, P1, P2);
		
		///////////////////////////////////////////////////////////////
		/*const int nnn = 100;

		double cur[nnn], pre[nnn];

		int nPix = w*h;
		int wi_j_cur = w*i + j;
		int wi_j_pre = w*i_pre + j_pre;

		for (int k = 0; k < nd; ++k)
		{
			cur[k] = C[nPix*k + wi_j_cur];
			pre[k] = C[nPix*k + wi_j_pre];
		}

		CostAggregation_OnePixel(cur, pre, nd, P1, P2);

		for (int k = 0; k < nd; ++k)
		{
			C[nPix*k + wi_j_cur] = cur[k];
		}*/

		////////////////////////////////////////////////////////////////
		int nPix = w*h;
		int wi_j_pre = w*i_pre + j_pre;
		int inxd = threadIdx.x*nd;

		for (int k = 0; k < nd; ++k)
		{
			s_pre[inxd + k] = C[nPix*k + wi_j_pre];
		}

		CostAggregation_OnePixel(w, h, nd, threadIdx.x, s_pre, C, i, j, P1, P2);
	}
	else // it's a parallel colomn
	{
		if (0 == dir || 4 == dir) // vertical propagation is not applicable in parallel colomn form
		{
			return;
		}

		if (idx1 >= w || idx2 >= h) // cross the boundary
		{
			return;
		}

		// image indices of current pixel
		int i = idx2;
		int j = idx1;

		// image indices of previous pixel according to the propagation direction
		int i_pre, j_pre;
		previous_ij(i, j, i_pre, j_pre, dir);

		if (i_pre < 0 || i_pre >= h || j_pre < 0 || j_pre >= w) // previous pixel cross the boundary
		{
			return;
		}

//		CostAggregation_OnePixel(w, h, nd, C, i, j, i_pre, j_pre, P1, P2);

		////////////////////////////////////////////////////////////////////////
		/*const int nnn = 100;

		double cur[nnn], pre[nnn];

		int nPix = w*h;
		int wi_j_cur = w*i + j;
		int wi_j_pre = w*i_pre + j_pre;

		for (int k = 0; k < nd; ++k)
		{
			cur[k] = C[nPix*k + wi_j_cur];
			pre[k] = C[nPix*k + wi_j_pre];
		}

		CostAggregation_OnePixel(cur, pre, nd, P1, P2);

		for (int k = 0; k < nd; ++k)
		{
			C[nPix*k + wi_j_cur] = cur[k];
		}*/

		///////////////////////////////////////////////////////////////////////////
		int nPix = w*h;
		int wi_j_pre = w*i_pre + j_pre;
		int inxd = threadIdx.x*nd;

		for (int k = 0; k < nd; ++k)
		{
			s_pre[inxd + k] = C[nPix*k + wi_j_pre];
		}

		CostAggregation_OnePixel(w, h, nd, threadIdx.x, s_pre, C, i, j, P1, P2);
	}
}

// 20161226, propagate from up pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelRow_Up(int w, int h, int nd,	// input: the width and height of stereo images
										 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
										 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
										 double P2,				// input: a larger constant penalty for all larger disparity changes
										 int i					// input: the row index of current parallel row, should be 1<=i<h
										 )
{
	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	if (j < w && k < nd) // j should be 0<=j<w
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i - 1, j, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i - 1, j, P1, P2);
	}
}

// 20161226, propagate from up right pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelRow_UpRight(int w, int h, int nd,	// input: the width and height of stereo images
											  double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											  double P1,			// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											  double P2,			// input: a larger constant penalty for all larger disparity changes
											  int i					// input: the row index of current parallel row, should be 1<=i<h
											  )
{
	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	if (j < (w - 1) && k < nd) // j should be 0<=j<(w-1)
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i - 1, j + 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i - 1, j + 1, P1, P2);
	}
}

// 20161226, propagate from up left pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelRow_UpLeft(int w, int h, int nd,	// input: the width and height of stereo images
											 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											 double P2,				// input: a larger constant penalty for all larger disparity changes
											 int i					// input: the row index of current parallel row, should be 1<=i<h
											 )
{
	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	if (j > 0 && j < w && k < nd) // j should be 0<j<w
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i - 1, j - 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i - 1, j - 1, P1, P2);
	}
}

// 20161226, propagate from bottom pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelRow_Bottom(int w, int h, int nd,	// input: the width and height of stereo images
											 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											 double P2,				// input: a larger constant penalty for all larger disparity changes
											 int i					// input: the row index of current parallel row, should be 0<=i<(h-1)
											 )
{
	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	if (j < w && k < nd) // j should be 0<=j<w
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i + 1, j, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i + 1, j, P1, P2);
	}
}

// 20161226, propagate from bottom right pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelRow_BottomRight(int w, int h, int nd,	// input: the width and height of stereo images
												  double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
												  double P1,			// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
												  double P2,			// input: a larger constant penalty for all larger disparity changes
												  int i					// input: the row index of current parallel row, should be 0<=i<(h-1)
												  )
{
	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	if (j < (w - 1) && k < nd) // j should be 0<=j<(w-1)
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i + 1, j + 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i + 1, j + 1, P1, P2);
	}
}

// 20161226, propagate from bottom left pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelRow_BottomLeft(int w, int h, int nd,	// input: the width and height of stereo images
												 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
												 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
												 double P2,				// input: a larger constant penalty for all larger disparity changes
												 int i					// input: the row index of current parallel row, should be 0<=i<(h-1)
												 )
{
	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	if (j > 0 && j < w && k < nd) // j should be 0<j<w
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i + 1, j - 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i + 1, j - 1, P1, P2);
	}
}

// 20161226, propagate from right pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelCol_Right(int w, int h, int nd,	// input: the width and height of stereo images
											double * C,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											double P2,				// input: a larger constant penalty for all larger disparity changes
											int j					// input: the column index of current parallel column, should be 0<=j<(w-1)
											)
{
	// the colomn index of current pixel
//	int i = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int i = blockDim.y * blockIdx.y + threadIdx.y;

	if (i < h && k < nd) // i should be 0<=i<h
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i, j + 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i, j + 1, P1, P2);
	}
}

// 20161226, propagate from up right pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelCol_UpRight(int w, int h, int nd,	// input: the width and height of stereo images
											  double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											  double P1,			// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											  double P2,			// input: a larger constant penalty for all larger disparity changes
											  int j					// input: the column index of current parallel column, should be 0<=j<(w-1)
											  )
{
	// the colomn index of current pixel
//	int i = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int i = blockDim.y * blockIdx.y + threadIdx.y;

	if (i > 0 && i < h && k < nd) // i should be 0<i<h
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i - 1, j + 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i - 1, j + 1, P1, P2);
	}
}

// 20161226, propagate from bottom right pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelCol_BottomRight(int w, int h, int nd,	// input: the width and height of stereo images
												  double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
												  double P1,			// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
												  double P2,			// input: a larger constant penalty for all larger disparity changes
												  int j					// input: the column index of current parallel column, should be 0<=j<(w-1)
												  )
{
	// the colomn index of current pixel
//	int i = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int i = blockDim.y * blockIdx.y + threadIdx.y;

	if (i < (h - 1) && k < nd) // i should be 0<=i<(h-1)
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i + 1, j + 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i + 1, j + 1, P1, P2);
	}
}

// 20161226, propagate from left pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelCol_Left(int w, int h, int nd,// input: the width and height of stereo images
										   double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
										   double P1,			// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
										   double P2,			// input: a larger constant penalty for all larger disparity changes
										   int j				// input: the column index of current parallel column, should be 0<j<w
										   )
{
	// the colomn index of current pixel
//	int i = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int i = blockDim.y * blockIdx.y + threadIdx.y;

	if (i < h && k < nd) // i should be 0<=i<h
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i, j - 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i, j - 1, P1, P2);
	}
}

// 20161226, propagate from up left pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelCol_UpLeft(int w, int h, int nd,	// input: the width and height of stereo images
											 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
											 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
											 double P2,				// input: a larger constant penalty for all larger disparity changes
											 int j					// input: the column index of current parallel column, should be 0<j<w
											 )
{
	// the colomn index of current pixel
//	int i = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int i = blockDim.y * blockIdx.y + threadIdx.y;

	if (i > 0 && i < h && k < nd) // i should be 0<=i<h
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i - 1, j - 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i - 1, j - 1, P1, P2);
	}
}

// 20161226, propagate from bottom left pixel to current pixel
// 20161115, four directions: 0:left to right. 1:right to left. 2:up to bottom. 3:bottom to up
// Generate the Disparity Space Image (DSI) based on the pixelwise matching
// cost proposed in <Depth Discontinuities by Pixel-to-Pixel Stereo> by S. Birchfield and C. Tomasi
__global__ void 
kernel_CostAggregation_OneParallelCol_BottomLeft(int w, int h, int nd,	// input: the width and height of stereo images
												 double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
												 double P1,				// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
												 double P2,				// input: a larger constant penalty for all larger disparity changes
												 int j					// input: the column index of current parallel column, should be 0<j<w
												 )
{
	// the colomn index of current pixel
//	int i = blockDim.x * blockIdx.x + threadIdx.x;

	int k = blockDim.x * blockIdx.x + threadIdx.x;
	int i = blockDim.y * blockIdx.y + threadIdx.y;

	if (i < (h - 1) && k < nd) // i should be 0<=i<h
	{
//		CostAggregation_OnePixel_new(w, h, nd, C, i, j, i + 1, j - 1, P1, P2);
		CostAggregation_OnePixel_new(w, h, nd, C, i, j, k, i + 1, j - 1, P1, P2);
	}
}

// 20161225, Merry Christmas to the world!
// the parallel row moves from up to bottom
// the previous pixel is right above current pixel
//__global__ void 
//kernel_CostAggregation_ParallelRow_up2bottom_0(int w, int h, int nd,// input: the width and height of stereo images
//											   double * C,			// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
//											   double P1,			// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
//											   double P2,			// input: a larger constant penalty for all larger disparity changes
//											   int dir				// input: 0:(0,-1), 1:(1,-1), 2:(1,0), 3:(1,1), 4:(0,1), 5:(-1,1), 6:(-1,0), 7:(-1,-1)
//											   )
//{
//	extern __shared__ double s_pre[]; // dynamic shared memory of aggregated costs of all previous pixels within the same thread block
//
//	// the colomn index of current pixel
//	int j = blockDim.x * blockIdx.x + threadIdx.x;
//
//	if (j < w)
//	{
//		for (int i = 1; i < h; ++i)
//		{
//			// indices of previous pixel on the propagation direction
//			int i_pre = i - 1;
//			int j_pre = j;
//			
//			int nPix = w*h;
//			int wi_j_pre = w*i_pre + j_pre;
//			int inxd = threadIdx.x*nd;
//
//			for (int k = 0; k < nd; ++k)
//			{
//				s_pre[inxd + k] = C[nPix*k + wi_j_pre];
//			}
//
//			CostAggregation_OnePixel(w, h, nd, threadIdx.x, s_pre, C, i, j, P1, P2);
//		}
//
//		// image indices of current pixel
//		int i = idx1;
//		int j = idx2;
//
//		// image indices of previous pixel according to the propagation direction
//		int i_pre, j_pre;
//		previous_ij(i, j, i_pre, j_pre, dir);
//
//		if (i_pre < 0 || i_pre >= h || j_pre < 0 || j_pre >= w) // previous pixel cross the boundary
//		{
//			return;
//		}
//
////		CostAggregation_OnePixel(w, h, nd, C, i, j, i_pre, j_pre, P1, P2);
//		
//		///////////////////////////////////////////////////////////////
//		/*const int nnn = 100;
//
//		double cur[nnn], pre[nnn];
//
//		int nPix = w*h;
//		int wi_j_cur = w*i + j;
//		int wi_j_pre = w*i_pre + j_pre;
//
//		for (int k = 0; k < nd; ++k)
//		{
//			cur[k] = C[nPix*k + wi_j_cur];
//			pre[k] = C[nPix*k + wi_j_pre];
//		}
//
//		CostAggregation_OnePixel(cur, pre, nd, P1, P2);
//
//		for (int k = 0; k < nd; ++k)
//		{
//			C[nPix*k + wi_j_cur] = cur[k];
//		}*/
//
//		////////////////////////////////////////////////////////////////
//		int nPix = w*h;
//		int wi_j_pre = w*i_pre + j_pre;
//		int inxd = threadIdx.x*nd;
//
//		for (int k = 0; k < nd; ++k)
//		{
//			s_pre[inxd + k] = C[nPix*k + wi_j_pre];
//		}
//
//		CostAggregation_OnePixel(w, h, nd, threadIdx.x, s_pre, C, i, j, P1, P2);
//	}
//	else // it's a parallel colomn
//	{
//		if (0 == dir || 4 == dir) // vertical propagation is not applicable in parallel colomn form
//		{
//			return;
//		}
//
//		if (idx1 >= w || idx2 >= h) // cross the boundary
//		{
//			return;
//		}
//
//		// image indices of current pixel
//		int i = idx2;
//		int j = idx1;
//
//		// image indices of previous pixel according to the propagation direction
//		int i_pre, j_pre;
//		previous_ij(i, j, i_pre, j_pre, dir);
//
//		if (i_pre < 0 || i_pre >= h || j_pre < 0 || j_pre >= w) // previous pixel cross the boundary
//		{
//			return;
//		}
//
////		CostAggregation_OnePixel(w, h, nd, C, i, j, i_pre, j_pre, P1, P2);
//
//		////////////////////////////////////////////////////////////////////////
//		/*const int nnn = 100;
//
//		double cur[nnn], pre[nnn];
//
//		int nPix = w*h;
//		int wi_j_cur = w*i + j;
//		int wi_j_pre = w*i_pre + j_pre;
//
//		for (int k = 0; k < nd; ++k)
//		{
//			cur[k] = C[nPix*k + wi_j_cur];
//			pre[k] = C[nPix*k + wi_j_pre];
//		}
//
//		CostAggregation_OnePixel(cur, pre, nd, P1, P2);
//
//		for (int k = 0; k < nd; ++k)
//		{
//			C[nPix*k + wi_j_cur] = cur[k];
//		}*/
//
//		///////////////////////////////////////////////////////////////////////////
//		int nPix = w*h;
//		int wi_j_pre = w*i_pre + j_pre;
//		int inxd = threadIdx.x*nd;
//
//		for (int k = 0; k < nd; ++k)
//		{
//			s_pre[inxd + k] = C[nPix*k + wi_j_pre];
//		}
//
//		CostAggregation_OnePixel(w, h, nd, threadIdx.x, s_pre, C, i, j, P1, P2);
//	}
//}

__global__ void 
kernel_AddVec_double(double * A,			// input & output: A[i] += B[i];
					 const double * B,		// input: 
					 int n					// input: number of elements
					 )
{
	// the colomn index of current pixel in case bParallelRow is true, otherwise, it's the row index of current pixel in case bParallelRow is false
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < n)
	{
		A[i] += B[i];
	}
}

// transpose a 2D array
__global__ void 
kernel_transpose_double(int w, int h,		// input: the width and height of the 2d array
						double * out,		// output:the transposed 2d array
						const double * in	// input: the original 2d array
						)
{
	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		out[j*h + i] = in[i*w + j]; // obviously, reading from in array is coalesced, writing to out array is not.
	}
}

// transpose a 2D array
__global__ void 
kernel_transpose_uchar(int w, int h,			// input: the width and height of the 2d array
					   unsigned char * out,		// output:the transposed 2d array
					   const unsigned char * in	// input: the original 2d array
					   )
{
	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		out[j*h + i] = in[i*w + j]; // obviously, reading from in array is coalesced, writing to out array is not.
	}
}

// transpose a 2D array
// the thread block should be a square otherwise the result will likely be wrong
__global__ void 
kernel_transpose_tile_double(int w, int h,		// input: the width and height of the 2d array
							 double * out,		// output:the transposed 2d array
							 const double * in	// input: the original 2d array
							 )
{
	extern __shared__ double s_tile[];

	int i_in_corner = blockDim.y*blockIdx.y;
	int j_in_corner = blockDim.x*blockIdx.x;

	int i_out_corner = j_in_corner;
	int j_out_corner = i_in_corner;

	int si = threadIdx.y;
	int sj = threadIdx.x;

	int i_in = i_in_corner + si;
	int j_in = j_in_corner + sj;
	
	if (i_in < h && j_in < w)
	{
		s_tile[sj*blockDim.y + si] = in[i_in*w + j_in];
	}
	__syncthreads();

	int i_out = i_out_corner + si;
	int j_out = j_out_corner + sj;

	if (i_out < w && j_out < h)
	{
		out[i_out*h + j_out] = s_tile[si*blockDim.x + sj];
	}
}

// transpose a 2D array
// the thread block should be a square otherwise the result will likely be wrong
__global__ void 
kernel_transpose_tile_uchar(int w, int h,				// input: the width and height of the 2d array
							unsigned char * out,		// output:the transposed 2d array
							const unsigned char * in	// input: the original 2d array
							)
{
	extern __shared__ unsigned char s_square[];

	int i_in_corner = blockDim.y*blockIdx.y;
	int j_in_corner = blockDim.x*blockIdx.x;

	int i_out_corner = j_in_corner;
	int j_out_corner = i_in_corner;

	int si = threadIdx.y;
	int sj = threadIdx.x;

	int i_in = i_in_corner + si;
	int j_in = j_in_corner + sj;
	
	if (i_in < h && j_in < w)
	{
		s_square[sj*blockDim.y + si] = in[i_in*w + j_in];
	}
	__syncthreads();

	int i_out = i_out_corner + si;
	int j_out = j_out_corner + sj;

	if (i_out < w && j_out < h)
	{
		out[i_out*h + j_out] = s_square[si*blockDim.x + sj];
	}
}

// 20170115
// setup the random states for all the elements in the even 2D field
__global__ void 
kernel_setup_randstates_even(curandState * states,		// output:the array of all the random states
							 int w, int h, int wo,		// input: the width, height of the even 2D field and the original width of the original image
							 unsigned long long seed	// input: the seed
							 )
{
	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		int id = i*w + j; // the ID of the element in the even field

		// the original indices in the original 2D field
		int i2 = 2 * i;
		int j2 = 2 * j;

		int ido = i2*wo + j2; // the ID of the element in the original field

		// Each thread gets same seed, a different sequence number, no offset
		curand_init(seed, ido, 0, &states[id]);
	}
}

// 20170115
// setup the random states for all the elements in the odd 2D field
__global__ void 
kernel_setup_randstates_odd(curandState * states,	// output:the array of all the random states
							int w, int h, int wo,	// input: the width, height of the odd 2D field and the original width of the original image
							unsigned long long seed	// input: the seed
							)
{
	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		int id = i*w + j; // the ID of the element in the odd field

		// the original indices in the original 2D field
		int i2 = 2 * i + 1;
		int j2 = 2 * j + 1;

		int ido = i2*wo + j2; // the ID of the element in the original field

		// Each thread gets same seed, a different sequence number, no offset
		curand_init(seed, ido, 0, &states[id]);
	}
}

// 20170119
// setup the random states for all the elements in the 2D field
__global__ void 
kernel_setup_randstates_2d(curandState * states,	// output:the array of all the random states
						   int w, int h,			// input: the width, height of the 2D field
						   unsigned long long seed	// input: the seed
						   )
{
	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		int id = i*w + j; // the ID of the element

		// Each thread gets same seed, a different sequence number, no offset
		curand_init(seed, id, 0, &states[id]);
	}
}

// 20170115
// uniform randomly drawn initialize a 2D array
__global__ void 
kernel_randinit_uniform_double(double * A,			// output:the array
							   curandState * states,// output:the random states
							   int w, int h,		// input: the width and height of the 2D array
							   double valmin,		// input: the minimum value
							   double valmax		// input: the maximum value
							   )
{
	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		int id = i*w + j;

		// Copy state to local memory for efficiency
		curandState localState = states[id];

		double sample = curand_uniform_double(&localState);

		A[id] = (valmax - valmin)*sample + valmin;

		// Copy state back to global memory (update the state)
		states[id] = localState;
	}
}

// 20170115
// initilize the depth maps, alpha maps and beta maps using random numbers
__global__ void 
kernel_PatchMatch_randinit(curandState * states,			// output:the random states
						   double * depth,					// output:the depth map
						   double * alpha,					// output:the alpha map
						   double * beta,					// output:the beta map
						   int w, int h,					// input: the width and height of the maps
						   double min_d, double max_d,		// input: the minimum and maximum depth
						   double min_a, double max_a,		// input: the minimum and maximum alpha
						   double min_b, double max_b		// input: the minimum and maximum beta
						   )
{
	extern __shared__ unsigned char s_rect[]; // 20170319, the shared memory, adjacent pixels in base image share parts in support window

	int i = blockDim.y*blockIdx.y + threadIdx.y;
	int j = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < h && j < w)
	{
		int id = i*w + j;

		// Copy state to local memory for efficiency
		curandState localState = states[id];

		double sample_depth = curand_uniform_double(&localState);
		double sample_alpha = curand_uniform_double(&localState);
		double sample_beta  = curand_uniform_double(&localState);

		depth[id] = (max_d - min_d)*sample_depth + min_d;
		alpha[id] = (max_a - min_a)*sample_alpha + min_a;
		beta[id]  = (max_b - min_b)*sample_beta  + min_b;

		// Copy state back to global memory (update the state)
		states[id] = localState;
	}
}

// 20170115
// initilize the depth maps, alpha maps and beta maps using random numbers
//__global__ void 
//kernel_PatchMatch_randinit(curandState * states,			// output:the random states
//						   double * depth,					// output:the depth map
//						   double * alpha,					// output:the alpha map
//						   double * beta,					// output:the beta map
//						   int w, int h,					// input: the width and height of the maps
//						   double min_d, double max_d,		// input: the minimum and maximum depth
//						   double min_a, double max_a,		// input: the minimum and maximum alpha
//						   double min_b, double max_b		// input: the minimum and maximum beta
//						   )
//{
//	int i = blockDim.y*blockIdx.y + threadIdx.y;
//	int j = blockDim.x*blockIdx.x + threadIdx.x;
//
//	if (i < h && j < w)
//	{
//		int id = i*w + j;
//
//		// Copy state to local memory for efficiency
//		curandState localState = states[id];
//
//		double sample_depth = curand_uniform_double(&localState);
//		double sample_alpha = curand_uniform_double(&localState);
//		double sample_beta  = curand_uniform_double(&localState);
//
//		depth[id] = (max_d - min_d)*sample_depth + min_d;
//		alpha[id] = (max_a - min_a)*sample_alpha + min_a;
//		beta[id]  = (max_b - min_b)*sample_beta  + min_b;
//
//		// Copy state back to global memory (update the state)
//		states[id] = localState;
//	}
//}

extern "C" void
CUDA_GenerateDSI_BT(int w, int h,				// input: the width and height of stereo images
				    const unsigned char * h_imgb,// input: h*w, the rectified grayscale base image
				    const unsigned char * h_imgm,// input: h*w, the rectified grayscale matching image
				    int dmin,					// input: scalar, the minimal disparity, can be negative, but make sure dmin<dmax
				    int dmax,					// input: scalar, the maximal disparity, can be negative, but make sure dmin<dmax
				    double * h_DSI,				// output:h*w*nd, nd = (dmax - dmin + 1), the output Disparity Space Image
				    int w_block,					// input: how many expected threads per row in a thread block
				    int h_block,					// input: how many expected threads per colomn in a thread block
				    int dir						// 0:l2r. 1:r2l. 2:u2b. 3:b2u
				    )
{
	const unsigned int nd = dmax - dmin + 1; // number of disparities

	const unsigned int nPixels = w*h; // number of pixels in total
	const unsigned int sizeMem = sizeof(unsigned char) * nPixels; // number of bytes of an image
	const unsigned int sizeMemDSI = sizeof(double) * nPixels * nd;// number of bytes of the DSI

	// allocate device memory
	unsigned char * d_imgb = NULL;
	unsigned char * d_imgm = NULL;
	double * d_DSI = NULL;
	cudaMalloc((void **)&d_imgb, sizeMem);
	cudaMalloc((void **)&d_imgm, sizeMem);
	cudaMalloc((void **)&d_DSI, sizeMemDSI);

	// copy host memory to device memory
	cudaMemcpy(d_imgb, h_imgb, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_imgm, h_imgm, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_DSI, h_DSI, sizeMemDSI, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % w_block) != 0) ? (w / w_block + 1) : (w / w_block); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % h_block) != 0) ? (h / h_block + 1) : (h / h_block); // number of blocks along y direction in the grid

	// determine the dimension of the grid and a block
	dim3 block(w_block, h_block, 1);
	dim3 grid(nBlocks_x, nBlocks_y, 1);

	/*cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start, 0);*/

	// call cuda kernel
	if (0==dir) // l2r
	{
		kernel_GenerateDSI_l2r_BT<<<grid, block>>>(w, h, d_imgb, d_imgm, dmin, dmax, d_DSI);
	}
	else if (1==dir) // r2l
	{
		kernel_GenerateDSI_r2l_BT<<<grid, block>>>(w, h, d_imgb, d_imgm, dmin, dmax, d_DSI);
	}
	else if (2==dir) // u2b
	{
		kernel_GenerateDSI_u2b_BT<<<grid, block>>>(w, h, d_imgb, d_imgm, dmin, dmax, d_DSI);
	}
	else // b2u
	{
		kernel_GenerateDSI_b2u_BT<<<grid, block>>>(w, h, d_imgb, d_imgm, dmin, dmax, d_DSI);
	}

	/*cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	float elapsedTime = 0.0f;
	cudaEventElapsedTime(&elapsedTime, start, stop);*/

	cudaMemcpy(h_DSI, d_DSI, sizeMemDSI, cudaMemcpyDeviceToHost);

	cudaFree(d_imgb);
	cudaFree(d_imgm);
	cudaFree(d_DSI);
}

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
				 int dir,						// input: 0:l2r. 1:r2l. 2:u2b. 3:b2u
				 int costType                   // input: 0:BT,  1:ncc, 2:opencvncc
				 )
{
	//cudaDeviceProp deviceProp;
	//deviceProp.major = 0;
	//deviceProp.minor = 0;
	//int dev = 0;

	//int argc; char **argv;

	//// This will pick the best possible CUDA capable device
	//// Otherwise pick the device with highest Gflops/s
	//dev = gpuGetMaxGflopsDeviceId();
	//checkCudaErrors(cudaSetDevice(dev));
	//checkCudaErrors(cudaGetDeviceProperties(&deviceProp, dev));
	//printf("GPU Device %d: \"%s\" with compute capability %d.%d\n\n", dev, deviceProp.name, deviceProp.major, deviceProp.minor);

	//checkCudaErrors(cudaGetDeviceProperties(&deviceProp, dev));

	//// Statistics about the GPU device
	//printf("> GPU device has %d Multi-Processors, SM %d.%d compute capabilities\n\n",
	//	deviceProp.multiProcessorCount, deviceProp.major, deviceProp.minor);

	//int version = (deviceProp.major * 0x10 + deviceProp.minor);

	//if (version < 0x20)
	//{
	//	printf("requires a minimum CUDA compute 2.0 capability\n");
	//	exit(EXIT_SUCCESS);
	//}
	/////////////////////////////////////////////////////////////////////////////

	const unsigned int nd = dmax - dmin + 1; // number of disparities

	const unsigned int nPixels = w*h; // number of pixels in total
	const unsigned int sizeMem = sizeof(unsigned char) * nPixels; // number of bytes of an image
	const unsigned int sizeMemDSI = sizeof(double) * nPixels * nd;// number of bytes of the DSI

	// allocate device memory
	unsigned char * d_imgb = NULL;
	unsigned char * d_imgm = NULL;
	double * d_DSI = NULL;
	cudaMalloc((void **)&d_imgb, sizeMem);
	cudaMalloc((void **)&d_imgm, sizeMem);
	cudaMalloc((void **)&d_DSI, sizeMemDSI);

	// copy host memory to device memory
	cudaMemcpy(d_imgb, h_imgb, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_imgm, h_imgm, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_DSI, h_DSI, sizeMemDSI, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % w_block) != 0) ? (w / w_block + 1) : (w / w_block); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % h_block) != 0) ? (h / h_block + 1) : (h / h_block); // number of blocks along y direction in the grid

	// determine the dimension of the grid and a block
	dim3 block(w_block, h_block, 1);
	dim3 grid(nBlocks_x, nBlocks_y, 1);

//	// 20170101, try transpose ////////////////////////////////
//	unsigned char * d_imgb_t = NULL;
//	cudaMalloc((void **)&d_imgb_t, sizeMem);
//	forCUDA_ShowInfo("kernel_transpose starts");
//
//	cudaEvent_t start, stop;
//	cudaEventCreate(&start);
//	cudaEventCreate(&stop);
//
//	cudaEventRecord(start, 0);
//
//	for (int iii = 0; iii < 1000; ++iii)
//	{
////		kernel_transpose_uchar<<<grid, block>>>(w, h, d_imgb_t, d_imgb);
//		kernel_transpose_tile_uchar<<<grid, block, sizeof(unsigned char)*w_block*h_block>>>(w, h, d_imgb_t, d_imgb);
//	}
////	kernel_transpose_uchar<<<grid, block>>>(h, w, d_imgb, d_imgb_t);
//	kernel_transpose_tile_uchar<<<grid, block, sizeof(unsigned char)*w_block*h_block>>>(h, w, d_imgb, d_imgb_t);
//
//	cudaEventRecord(stop, 0);
//	cudaEventSynchronize(stop);
//
//	float elapsedTime = 0.0f;
//	cudaEventElapsedTime(&elapsedTime, start, stop);
//
//	forCUDA_ShowInfo("kernel_transpose ends");
//	cudaFree(d_imgb_t);
//	///////////////////////////////////////////////////////////

	/*cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start, 0);*/

	forCUDA_ShowInfo("kernel_GenerateDSI starts");

	// call cuda kernel
//	kernel_GenerateDSI<<<grid, block>>>(w, h, hw, hh, d_imgb, d_imgm, dmin, dmax, d_DSI, dir, costType);
	kernel_GenerateDSI_new<<<grid, block>>>(w, h, hw, hh, d_imgb, d_imgm, dmin, dmax, d_DSI, dir, costType);

//	getLastCudaError("kernel_GenerateDSI execution failed\n");

	/*cudaError_t err = cudaGetLastError();

	const char * errorString = cudaGetErrorString(err);*/

	//CString strerr(errorString);
	//AfxMessageBox(strerr);
	
	/*if (cudaSuccess != err)
	{
		fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d) %s.\n",
			file, line, errorMessage, (int)err, cudaGetErrorString(err));
		DEVICE_RESET
			exit(EXIT_FAILURE);
	}*/
		
	/*cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	float elapsedTime = 0.0f;
	cudaEventElapsedTime(&elapsedTime, start, stop);*/

	cudaMemcpy(h_DSI, d_DSI, sizeMemDSI, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("kernel_GenerateDSI ends");

	cudaFree(d_imgb);
	cudaFree(d_imgm);
	cudaFree(d_DSI);
}

// 20170102, 3D thread block
extern "C" void
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
					 int dir,						// input: 0:l2r. 1:r2l. 2:u2b. 3:b2u
					 int costType                   // input: 0:BT,  1:ncc, 2:opencvncc
					 )
{
	const unsigned int nd = dmax - dmin + 1; // number of disparities

	const unsigned int nPixels = w*h; // number of pixels in total
	const unsigned int sizeMem = sizeof(unsigned char) * nPixels; // number of bytes of an image
	const unsigned int sizeMemDSI = sizeof(double) * nPixels * nd;// number of bytes of the DSI

	// allocate device memory
	unsigned char * d_imgb = NULL;
	unsigned char * d_imgm = NULL;
	double * d_DSI = NULL;
	cudaMalloc((void **)&d_imgb, sizeMem);
	cudaMalloc((void **)&d_imgm, sizeMem);
	cudaMalloc((void **)&d_DSI, sizeMemDSI);

	// copy host memory to device memory
	cudaMemcpy(d_imgb, h_imgb, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_imgm, h_imgm, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_DSI, h_DSI, sizeMemDSI, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % w_block) != 0) ? (w / w_block + 1) : (w / w_block); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % h_block) != 0) ? (h / h_block + 1) : (h / h_block); // number of blocks along y direction in the grid
	const unsigned int nBlocks_z = ((nd % k_block) != 0) ? (nd / k_block + 1) : (nd / k_block); // number of blocks along z direction in the grid

	const unsigned int nBlocks_tile_x = ((w % nTile) != 0) ? (w / nTile + 1) : (w / nTile); // number of blocks along x direction in the grid
	const unsigned int nBlocks_tile_y = ((h % nTile) != 0) ? (h / nTile + 1) : (h / nTile); // number of blocks along y direction in the grid

	// determine the dimension of the 3D grid and 3D block
	dim3 block(k_block, w_block, h_block);
	dim3 grid(nBlocks_z, nBlocks_x, nBlocks_y);

	// determine the dimension of the 3D grid and 3D block if input stereo images are transposed
	dim3 block_t(k_block, h_block, w_block);
	dim3 grid_t(nBlocks_z, nBlocks_y, nBlocks_x);

	// determine the dimension of the 2D grid and 2D block for transpose kernel
	dim3 blockTile(nTile, nTile, 1);
	dim3 gridTile(nBlocks_tile_x, nBlocks_tile_y, 1);

	int sizeSharedMemTile = sizeof(unsigned char) * nTile * nTile;

	forCUDA_ShowInfo("CUDA_GenerateDSI_new starts");

	// call cuda kernel
	if (0 == dir) // l2r
	{
		if (0 == costType)
		{
			// BT
			kernel_GenerateDSI_l2r_BT<<<grid, block>>>(w, h, nd, d_imgb, d_imgm, dmin, d_DSI);
		}
		else if (1 == costType)
		{
			// ncc
			kernel_GenerateDSI_l2r_ncc<<<grid, block>>>(w, h, nd, hw, hh, d_imgb, d_imgm, dmin, d_DSI);
		}
		else
		{
			// opencv ncc
			kernel_GenerateDSI_l2r_opencvncc<<<grid, block>>>(w, h, nd, hw, hh, d_imgb, d_imgm, dmin, d_DSI);
		}
	}
	else if (1 == dir) // r2l
	{
		if (0 == costType)
		{
			// BT
			kernel_GenerateDSI_r2l_BT<<<grid, block>>>(w, h, nd, d_imgb, d_imgm, dmin, d_DSI);
		}
		else if (1 == costType)
		{
			// ncc
			kernel_GenerateDSI_r2l_ncc<<<grid, block>>>(w, h, nd, hw, hh, d_imgb, d_imgm, dmin, d_DSI);
		}
		else
		{
			// opencv ncc
			kernel_GenerateDSI_r2l_opencvncc<<<grid, block>>>(w, h, nd, hw, hh, d_imgb, d_imgm, dmin, d_DSI);
		}
	}
	else if (2 == dir) // u2b
	{
		unsigned char * d_imgb_t = NULL;
		unsigned char * d_imgm_t = NULL;
		cudaMalloc((void **)&d_imgb_t, sizeMem);
		cudaMalloc((void **)&d_imgm_t, sizeMem);

		// transpose both stereo images first
		kernel_transpose_tile_uchar<<<gridTile, blockTile, sizeSharedMemTile>>>(w, h, d_imgb_t, d_imgb);
		kernel_transpose_tile_uchar<<<gridTile, blockTile, sizeSharedMemTile>>>(w, h, d_imgm_t, d_imgm);

		if (0 == costType)
		{
			// BT
			kernel_GenerateDSI_u2b_BT_transposed<<<grid_t, block_t>>>(h, w, nd, d_imgb_t, d_imgm_t, dmin, d_DSI);
		}
		else if (1 == costType)
		{
			// ncc
			kernel_GenerateDSI_u2b_ncc_transposed<<<grid_t, block_t>>>(h, w, nd, hh, hw, d_imgb_t, d_imgm_t, dmin, d_DSI);
		}
		else
		{
			// opencv ncc
			kernel_GenerateDSI_u2b_opencvncc_transposed<<<grid_t, block_t>>>(h, w, nd, hh, hw, d_imgb_t, d_imgm_t, dmin, d_DSI);
		}

		cudaFree(d_imgb_t);
		cudaFree(d_imgm_t);
	}
	else // b2u
	{
		unsigned char * d_imgb_t = NULL;
		unsigned char * d_imgm_t = NULL;
		cudaMalloc((void **)&d_imgb_t, sizeMem);
		cudaMalloc((void **)&d_imgm_t, sizeMem);

		// transpose both stereo images first
		kernel_transpose_tile_uchar<<<gridTile, blockTile, sizeSharedMemTile>>>(w, h, d_imgb_t, d_imgb);
		kernel_transpose_tile_uchar<<<gridTile, blockTile, sizeSharedMemTile>>>(w, h, d_imgm_t, d_imgm);

		if (0 == costType)
		{
			// BT
			kernel_GenerateDSI_b2u_BT_transposed<<<grid_t, block_t>>>(h, w, nd, d_imgb_t, d_imgm_t, dmin, d_DSI);
		}
		else if (1 == costType)
		{
			// ncc
			kernel_GenerateDSI_b2u_ncc_transposed<<<grid_t, block_t>>>(h, w, nd, hh, hw, d_imgb_t, d_imgm_t, dmin, d_DSI);
		}
		else
		{
			// opencv ncc
			kernel_GenerateDSI_b2u_opencvncc_transposed<<<grid_t, block_t>>>(h, w, nd, hh, hw, d_imgb_t, d_imgm_t, dmin, d_DSI);
		}

		cudaFree(d_imgb_t);
		cudaFree(d_imgm_t);
	}

	cudaMemcpy(h_DSI, d_DSI, sizeMemDSI, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("CUDA_GenerateDSI_new ends");

	cudaFree(d_imgb);
	cudaFree(d_imgm);
	cudaFree(d_DSI);
}

extern "C" void
CUDA_CostAggregation_OneDir(int w, int h, int nd,		// input: the width and height of stereo images, and the number of disparities
							const double * h_DSI,		// input: h*w*nd, the Disparity Space Image
							double * h_C,				// output:h*w*nd, the aggregated cost volume along this direction
							double P1,					// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
							double P2,					// input: a larger constant penalty for all larger disparity changes
							int n_block,				// input: how many expected threads in a thread block
							int dir						// 0:l2r. 1:r2l. 2:u2b. 3:b2u
							)
{
//	cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);

	const unsigned int sizeMemC = sizeof(double) *w * h * nd;// number of bytes of the cost volume

	// allocate device memory
	double * d_C = NULL;
	cudaMalloc((void **)&d_C, sizeMemC);

	// copy host memory to device memory
	cudaMemcpy(d_C, h_DSI, sizeMemC, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % n_block) != 0) ? (w / n_block + 1) : (w / n_block); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % n_block) != 0) ? (h / n_block + 1) : (h / n_block); // number of blocks along y direction in the grid

	int sizeMemPreC_block = sizeof(double) * nd * n_block;

	// determine the dimension of the grid and a block
	//dim3 block(w_block, h_block, 1);
	//dim3 grid(nBlocks_x, nBlocks_y, 1);

	//// call cuda kernel
	//kernel_GenerateDSI<<<grid, block>>>(w, h, hw, hh, d_imgb, d_imgm, dmin, dmax, d_DSI, dir, costType);

	forCUDA_ShowInfo("kernel_CostAggregation_OneParallelLine starts");

	dim3 block(n_block, 1, 1);

	if (0==dir) // up
	{
		dim3 grid(nBlocks_x, 1, 1);
		for (int i = 0; i < h; ++i)
		{
			kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, i, true, 0);
		}
	}
	else if (1==dir) // up right
	{
		if (w >= h) // using parallel row
		{
			dim3 grid(nBlocks_x, 1, 1);
			for (int i = 0; i < h; ++i)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, i, true, 1);
			}
		}
		else // using parallel colomn
		{
			dim3 grid(nBlocks_y, 1, 1);
			for (int j = (w - 1); j >= 0; --j)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, j, false, 1);
			}
		}
	}
	else if (2 == dir) // right
	{
		dim3 grid(nBlocks_y, 1, 1);
		for (int j = (w-1); j >= 0; --j)
		{
			kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, j, false, 2);
		}
	}
	else if (3 == dir) // bottom right
	{
		if (w >= h) // using parallel row
		{
			dim3 grid(nBlocks_x, 1, 1);
			for (int i = (h - 1); i >= 0; --i)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, i, true, 3);
			}
		}
		else // using parallel colomn
		{
			dim3 grid(nBlocks_y, 1, 1);
			for (int j = (w - 1); j >= 0; --j)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, j, false, 3);
			}
		}
	}
	else if (4 == dir) // bottom
	{
		dim3 grid(nBlocks_x, 1, 1);
		for (int i = (h-1); i >= 0; --i)
		{
			kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, i, true, 4);
		}
	}
	else if (5 == dir) // bottom left
	{
		if (w >= h) // using parallel row
		{
			dim3 grid(nBlocks_x, 1, 1);
			for (int i = (h - 1); i >= 0; --i)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, i, true, 5);
			}
		}
		else // using parallel colomn
		{
			dim3 grid(nBlocks_y, 1, 1);
			for (int j = 0; j < w; ++j)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, j, false, 5);
			}
		}
	}
	else if (6 == dir) // left
	{
		dim3 grid(nBlocks_y, 1, 1);
		for (int j = 0; j < w; ++j)
		{
			kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, j, false, 6);
		}
	}
	else if (7 == dir) // up left
	{
		if (w >= h)
		{
			dim3 grid(nBlocks_x, 1, 1);
			for (int i = 0; i < h; ++i)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, i, true, 7);
			}
		}
		else
		{
			dim3 grid(nBlocks_y, 1, 1);
			for (int j = 0; j < w; ++j)
			{
				kernel_CostAggregation_OneParallelLine<<<grid, block, sizeMemPreC_block>>>(w, h, nd, d_C, P1, P2, j, false, 7);
			}
		}
	}
	else
	{

	}

	cudaMemcpy(h_C, d_C, sizeMemC, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("kernel_CostAggregation_OneParallelLine ends");

	cudaFree(d_C);
}

extern "C" void
CUDA_CostAggregation_OneDir_new(int w, int h, int nd,		// input: the width and height of stereo images, and the number of disparities
								const double * h_DSI,		// input: h*w*nd, the Disparity Space Image
								double * h_C,				// output:h*w*nd, the aggregated cost volume along this direction
								double P1,					// input: constant penalty pixels in the neigborhood of (x,y), for which the disparity changes a little bit (that is 1 pixel)
								double P2,					// input: a larger constant penalty for all larger disparity changes
								int n_block,				// input: how many expected threads in a thread block
								int dir						// 0:l2r. 1:r2l. 2:u2b. 3:b2u
								)
{
//	cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);

	const unsigned int sizeMemC = sizeof(double) *w * h * nd;// number of bytes of the cost volume

	// allocate device memory
	double * d_C = NULL;
	cudaMalloc((void **)&d_C, sizeMemC);

	// copy host memory to device memory
	cudaMemcpy(d_C, h_DSI, sizeMemC, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % n_block) != 0) ? (w / n_block + 1) : (w / n_block); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % n_block) != 0) ? (h / n_block + 1) : (h / n_block); // number of blocks along y direction in the grid

	// 20161228
	const unsigned int nBlocks_z = ((nd % n_block) != 0) ? (nd / n_block + 1) : (nd / n_block); // number of blocks along disparity direction in the grid

	forCUDA_ShowInfo("kernel_CostAggregation_OneParallelLine starts");

	// 1D parallelism
//	dim3 block(n_block, 1, 1);
//	dim3 grid_row(nBlocks_x, 1, 1);
//	dim3 grid_col(nBlocks_y, 1, 1);

	// 20161228, 2D parallelism
	dim3 block(n_block, n_block, 1);
	dim3 grid_row(nBlocks_z, nBlocks_x, 1);
	dim3 grid_col(nBlocks_z, nBlocks_y, 1);

	if (0==dir) // up
	{
		for (int i = 1; i < h; ++i)
		{
			kernel_CostAggregation_OneParallelRow_Up<<<grid_row, block>>>(w, h, nd, d_C, P1, P2, i);
		}
	}
	else if (1==dir) // up right
	{
		if (w >= h) // using parallel row
		{
			for (int i = 1; i < h; ++i)
			{
				kernel_CostAggregation_OneParallelRow_UpRight<<<grid_row, block>>>(w, h, nd, d_C, P1, P2, i);
			}
		}
		else // using parallel colomn
		{
			for (int j = (w - 2); j >= 0; --j)
			{
				kernel_CostAggregation_OneParallelCol_UpRight<<<grid_col, block>>>(w, h, nd, d_C, P1, P2, j);
			}
		}
	}
	else if (2 == dir) // right
	{
		for (int j = (w - 2); j >= 0; --j)
		{
			kernel_CostAggregation_OneParallelCol_Right<<<grid_col, block>>>(w, h, nd, d_C, P1, P2, j);
		}
	}
	else if (3 == dir) // bottom right
	{
		if (w >= h) // using parallel row
		{
			for (int i = (h - 2); i >= 0; --i)
			{
				kernel_CostAggregation_OneParallelRow_BottomRight<<<grid_row, block>>>(w, h, nd, d_C, P1, P2, i);
			}
		}
		else // using parallel colomn
		{
			for (int j = (w - 2); j >= 0; --j)
			{
				kernel_CostAggregation_OneParallelCol_BottomRight<<<grid_col, block>>>(w, h, nd, d_C, P1, P2, j);
			}
		}
	}
	else if (4 == dir) // bottom
	{
		for (int i = (h - 2); i >= 0; --i)
		{
			kernel_CostAggregation_OneParallelRow_Bottom<<<grid_row, block>>>(w, h, nd, d_C, P1, P2, i);
		}
	}
	else if (5 == dir) // bottom left
	{
		if (w >= h) // using parallel row
		{
			for (int i = (h - 2); i >= 0; --i)
			{
				kernel_CostAggregation_OneParallelRow_BottomLeft<<<grid_row, block>>>(w, h, nd, d_C, P1, P2, i);
			}
		}
		else // using parallel colomn
		{
			for (int j = 1; j < w; ++j)
			{
				kernel_CostAggregation_OneParallelCol_BottomLeft<<<grid_col, block>>>(w, h, nd, d_C, P1, P2, j);
			}
		}
	}
	else if (6 == dir) // left
	{
		for (int j = 1; j < w; ++j)
		{
			kernel_CostAggregation_OneParallelCol_Left<<<grid_col, block>>>(w, h, nd, d_C, P1, P2, j);
		}
	}
	else if (7 == dir) // up left
	{
		if (w >= h)
		{
			for (int i = 1; i < h; ++i)
			{
				kernel_CostAggregation_OneParallelRow_UpLeft<<<grid_row, block>>>(w, h, nd, d_C, P1, P2, i);
			}
		}
		else
		{
			for (int j = 1; j < w; ++j)
			{
				kernel_CostAggregation_OneParallelCol_UpLeft<<<grid_col, block>>>(w, h, nd, d_C, P1, P2, j);
			}
		}
	}
	else
	{

	}

	cudaMemcpy(h_C, d_C, sizeMemC, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("kernel_CostAggregation_OneParallelLine ends");

	cudaFree(d_C);
}

extern "C" void
CUDA_AddVec_double(double * h_A,		// input & output: A[i] += B[i]
				   const double * h_B,	// input: 
				   int n,				// input: number of elements
				   int nThreads			// input: number of threads in a thread block
				   )
{
	const unsigned int sizeMem = sizeof(double) * n;

	// allocate device memory
	double * d_A = NULL;
	double * d_B = NULL;
	cudaMalloc((void **)&d_A, sizeMem);
	cudaMalloc((void **)&d_B, sizeMem);

	// copy host memory to device memory
	cudaMemcpy(d_A, h_A, sizeMem, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, sizeMem, cudaMemcpyHostToDevice);

	const unsigned int nBlocks = ((n % nThreads) != 0) ? (n / nThreads + 1) : (n / nThreads); // number of blocks along x direction in the grid

	forCUDA_ShowInfo("kernel_AddVec_double starts");

	// determine the dimension of the grid and a block
	dim3 block(nThreads, 1, 1);
	dim3 grid(nBlocks, 1, 1);

	// run cuda kernel
	kernel_AddVec_double<<<grid, block>>>(d_A, d_B, n);

	cudaMemcpy(h_A, d_A, sizeMem, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("kernel_AddVec_double ends");

	cudaFree(d_A);
	cudaFree(d_B);
}

extern "C" void
CUDA_transpose_uchar(unsigned char * h_B,		// output:B = A'
					 const unsigned char * h_A,	// input: A 
					 int w, int h,				// input: width and height of A
					 int nThreads				// input: m, the tile or thread block is m*m
					 )
{
	const unsigned int sizeMem = sizeof(unsigned char) * w * h;

	// allocate device memory
	unsigned char * d_A = NULL;
	unsigned char * d_B = NULL;
	cudaMalloc((void **)&d_A, sizeMem);
	cudaMalloc((void **)&d_B, sizeMem);

	// copy host memory to device memory
	cudaMemcpy(d_A, h_A, sizeMem, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % nThreads) != 0) ? (w / nThreads + 1) : (w / nThreads); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % nThreads) != 0) ? (h / nThreads + 1) : (h / nThreads); // number of blocks along y direction in the grid

	// determine the dimension of the grid and a block
	dim3 block(nThreads, nThreads, 1);
	dim3 grid(nBlocks_x, nBlocks_y, 1);

	forCUDA_ShowInfo("kernel_transpose_uchar starts");
	
	// run cuda kernel
	kernel_transpose_uchar<<<grid, block>>>(w, h, d_B, d_A);

	cudaMemcpy(h_B, d_B, sizeMem, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("kernel_transpose_uchar ends");

	cudaFree(d_A);
	cudaFree(d_B);
}

extern "C" void
CUDA_transpose_tile_uchar(unsigned char * h_B,			// output:B = A'
						  const unsigned char * h_A,	// input: A 
						  int w, int h,					// input: width and height of A
						  int nThreads					// input: m, the tile or thread block is m*m
						  )
{
	const unsigned int sizeMem = sizeof(unsigned char) * w * h;

	// allocate device memory
	unsigned char * d_A = NULL;
	unsigned char * d_B = NULL;
	cudaMalloc((void **)&d_A, sizeMem);
	cudaMalloc((void **)&d_B, sizeMem);

	// copy host memory to device memory
	cudaMemcpy(d_A, h_A, sizeMem, cudaMemcpyHostToDevice);

	const unsigned int nBlocks_x = ((w % nThreads) != 0) ? (w / nThreads + 1) : (w / nThreads); // number of blocks along x direction in the grid
	const unsigned int nBlocks_y = ((h % nThreads) != 0) ? (h / nThreads + 1) : (h / nThreads); // number of blocks along y direction in the grid

	// determine the dimension of the grid and a block
	dim3 block(nThreads, nThreads, 1);
	dim3 grid(nBlocks_x, nBlocks_y, 1);

	forCUDA_ShowInfo("kernel_transpose_tile_uchar starts");
	
	// run cuda kernel
	kernel_transpose_tile_uchar<<<grid, block>>>(w, h, d_B, d_A);

	cudaMemcpy(h_B, d_B, sizeMem, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("kernel_transpose_tile_uchar ends");

	cudaFree(d_A);
	cudaFree(d_B);
}

// 20170104
//extern "C" void
//CUDA_PatchMatch(const unsigned char * h_imgb,	// input: the base gray image
//				const unsigned char * h_imgm,	// input: tha matching gray image
//				int w_b, int h_b,				// input: the width and height of base image
//				int w_m, int h_m,				// input: the width and height of matching image
//				double * h_depth,				// output:the estimated depth map of the base image
//				double * h_alpha,				// output:the estimated surface normal map of the base image
//				double * h_beta,				// output:the estimated surface normal map of the base image
//				int nThreads_w,					// input: the number of threads per row of the thread block
//				int nThreads_h,					// input: the number of threads per column of the thread block
//				unsigned long long randSeed,	// input: the random seed
//				double min_d, double max_d,		// input: the minimum and maximum depth
//				double min_a, double max_a,		// input: the minimum and maximum alpha
//				double min_b, double max_b		// input: the minimum and maximum beta
//				)
//{
//	int w_odd = w_b / 2; // the width of the odd maps
//	int h_odd = h_b / 2; // the height of the odd maps
//
//	int w_even = w_b - w_odd; // the width of the even maps
//	int h_even = h_b - h_odd; // the height of the even maps
//
//	const unsigned int sizeMemB = sizeof(unsigned char) * w_b * h_b;
//	const unsigned int sizeMemM = sizeof(unsigned char) * w_m * h_m;
//
//	const unsigned int sizeMemEven = sizeof(double) * w_even * h_even;
//	const unsigned int sizeMemOdd = sizeof(double) * w_odd * h_odd;
//
//	// allocate device memory
//	unsigned char * d_imgb = NULL;
//	unsigned char * d_imgm = NULL;
////	double * d_depth = NULL;
////	double * d_alpha = NULL;
////	double * d_beta  = NULL;
//	double * d_depth_even = NULL; double * d_depth_odd = NULL;
//	double * d_alpha_even = NULL; double * d_alpha_odd = NULL;
//	double * d_beta_even = NULL;  double * d_beta_odd = NULL;
//	cudaMalloc((void **)&d_imgb, sizeMemB);
//	cudaMalloc((void **)&d_imgm, sizeMemM);
//	cudaMalloc((void **)&d_depth_even, sizeMemEven);
//	cudaMalloc((void **)&d_depth_odd, sizeMemOdd);
//	cudaMalloc((void **)&d_alpha_even, sizeMemEven);
//	cudaMalloc((void **)&d_alpha_odd, sizeMemOdd);
//	cudaMalloc((void **)&d_beta_even, sizeMemEven);
//	cudaMalloc((void **)&d_beta_odd, sizeMemOdd);
//
//	// copy host memory to device memory
//	cudaMemcpy(d_imgb, h_imgb, sizeMemB, cudaMemcpyHostToDevice);
//	cudaMemcpy(d_imgm, h_imgm, sizeMemM, cudaMemcpyHostToDevice);
//
//	curandState * devStatesEven; // the curandStates of the even field
//	curandState * devStatesOdd;  // the curandStates of the odd field
//
//	// allocate the random states
//	cudaMalloc((void **)&devStatesEven, w_even * h_even * sizeof(curandState));
//	cudaMalloc((void **)&devStatesOdd, w_odd * h_odd * sizeof(curandState));
//
//	const unsigned int nBlocks_even_x = ((w_even % nThreads_w) != 0) ? (w_even / nThreads_w + 1) : (w_even / nThreads_w);
//	const unsigned int nBlocks_even_y = ((h_even % nThreads_h) != 0) ? (h_even / nThreads_h + 1) : (h_even / nThreads_h);
//
//	const unsigned int nBlocks_odd_x = ((w_odd % nThreads_w) != 0) ? (w_odd / nThreads_w + 1) : (w_odd / nThreads_w);
//	const unsigned int nBlocks_odd_y = ((h_odd % nThreads_h) != 0) ? (h_odd / nThreads_h + 1) : (h_odd / nThreads_h);
//
//	// determine the dimension of the grid and a block
//	dim3 block(nThreads_w, nThreads_h, 1);
//	dim3 grid_even(nBlocks_even_x, nBlocks_even_y, 1);
//	dim3 grid_odd(nBlocks_odd_x, nBlocks_odd_y, 1);
//
//	//double * h_depth_even = new double[w_even*h_even]; double * h_depth_odd = new double[w_odd * h_odd];
//	//double * h_alpha_even = new double[w_even*h_even]; double * h_alpha_odd = new double[w_odd * h_odd];
//	//double * h_beta_even = new double[w_even*h_even];  double * h_beta_odd = new double[w_odd * h_odd];
//
////	forCUDA_ShowInfo("random starts");
//
//	// setup the random states for the even and odd field
//	kernel_setup_randstates_even<<<grid_even, block>>>(devStatesEven, w_even, h_even, w_b, randSeed);
//	kernel_setup_randstates_odd<<<grid_odd, block>>>(devStatesOdd, w_odd, h_odd, w_b, randSeed);
//
//	// initialize all the parameter maps with uniform random parameters
//	kernel_PatchMatch_randinit<<<grid_even, block>>>(devStatesEven, d_depth_even, d_alpha_even, d_beta_even, w_even, h_even, min_d, max_d, min_a, max_a, min_b, max_b);
//	kernel_PatchMatch_randinit<<<grid_odd,  block>>>(devStatesOdd,  d_depth_odd,  d_alpha_odd,  d_beta_odd,  w_odd,  h_odd,  min_d, max_d, min_a, max_a, min_b, max_b);
//
//	/*kernel_PatchMatch_randinit<<<grid_even, block>>>(devStatesEven, d_depth_even, d_alpha_even, d_beta_even, w_even, h_even, 20, 100, 10, 360, 0, 60);
//	kernel_PatchMatch_randinit<<<grid_odd,  block>>>(devStatesOdd,  d_depth_odd,  d_alpha_odd,  d_beta_odd,  w_odd,  h_odd,  20, 100, 10, 360, 0, 60);
//
//	cudaMemcpy(h_depth_even, d_depth_even, sizeMemEven, cudaMemcpyDeviceToHost);
//	cudaMemcpy(h_alpha_even, d_alpha_even, sizeMemEven, cudaMemcpyDeviceToHost);
//	cudaMemcpy(h_beta_even, d_beta_even, sizeMemEven, cudaMemcpyDeviceToHost);
//	cudaMemcpy(h_depth_odd, d_depth_odd, sizeMemOdd, cudaMemcpyDeviceToHost);
//	cudaMemcpy(h_alpha_odd, d_alpha_odd, sizeMemOdd, cudaMemcpyDeviceToHost);
//	cudaMemcpy(h_beta_odd, d_beta_odd, sizeMemOdd, cudaMemcpyDeviceToHost);
//
//	forCUDA_ShowInfo("random ends");
//
//	forCUDA_SaveMatAsImage("E:\\results\\depth_even.png", h_depth_even, w_even, h_even, 20, 100);
//	forCUDA_SaveMatAsImage("E:\\results\\alpha_even.png", h_alpha_even, w_even, h_even, 10, 360);
//	forCUDA_SaveMatAsImage("E:\\results\\beta_even.png", h_beta_even, w_even, h_even, 0, 60);
//	forCUDA_SaveMatAsImage("E:\\results\\depth_odd.png", h_depth_odd, w_odd, h_odd, 20, 100);
//	forCUDA_SaveMatAsImage("E:\\results\\alpha_odd.png", h_alpha_odd, w_odd, h_odd, 10, 360);
//	forCUDA_SaveMatAsImage("E:\\results\\beta_odd.png", h_beta_odd, w_odd, h_odd, 0, 60);
//
//	delete[] h_depth_even;
//	delete[] h_alpha_even;
//	delete[] h_beta_even;
//	delete[] h_depth_odd;
//	delete[] h_alpha_odd;
//	delete[] h_beta_odd;*/
//
////	forCUDA_ShowInfo("kernel_transpose_tile_uchar starts");
//	
//	// run cuda kernel
////	kernel_transpose_tile_uchar<<<grid, block>>>(w, h, d_B, d_A);
//
////	cudaMemcpy(h_B, d_B, sizeMem, cudaMemcpyDeviceToHost);
//
////	forCUDA_ShowInfo("kernel_transpose_tile_uchar ends");
//
//	cudaFree(d_imgb);
//	cudaFree(d_imgm);
//	cudaFree(d_depth_even);
//	cudaFree(d_depth_odd);
//	cudaFree(d_alpha_even);
//	cudaFree(d_alpha_odd);
//	cudaFree(d_beta_even);
//	cudaFree(d_beta_odd);
//
//	cudaFree(devStatesEven);
//	cudaFree(devStatesOdd);
//}

// 20170119
extern "C" void
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
				)
{
	// determine the width and height of the parallel grid
	int w_grid = ((w_b % 2) != 0) ? ((w_b + 1) / 2) : (w_b / 2);
	int h_grid = h_b;

	int nPix = w_b*h_b;

	int sizeMemB = sizeof(unsigned char) * nPix;
	int sizeMemM = sizeof(unsigned char) * w_m * h_m;

	int sizeMemMaps = sizeof(double) * nPix;

	// allocate device memory
	unsigned char * d_imgb = NULL;
	unsigned char * d_imgm = NULL;
	double * d_depth = NULL;
	double * d_alpha = NULL;
	double * d_beta  = NULL;
	cudaMalloc((void **)&d_imgb, sizeMemB);
	cudaMalloc((void **)&d_imgm, sizeMemM);
	cudaMalloc((void **)&d_depth, sizeMemMaps);
	cudaMalloc((void **)&d_alpha, sizeMemMaps);
	cudaMalloc((void **)&d_beta, sizeMemMaps);

	// copy host memory to device memory
	cudaMemcpy(d_imgb, h_imgb, sizeMemB, cudaMemcpyHostToDevice);
	cudaMemcpy(d_imgm, h_imgm, sizeMemM, cudaMemcpyHostToDevice);

	curandState * devStates; // the curandStates of the even field

	// allocate the random states
	cudaMalloc((void **)&devStates, nPix * sizeof(curandState));

	int nBlocks_ox = ((w_b % nThreads_w) != 0) ? (w_b / nThreads_w + 1) : (w_b / nThreads_w);
	int nBlocks_x = ((w_grid % nThreads_w) != 0) ? (w_grid / nThreads_w + 1) : (w_grid / nThreads_w);
	int nBlocks_y = ((h_grid % nThreads_h) != 0) ? (h_grid / nThreads_h + 1) : (h_grid / nThreads_h);

	// determine the dimension of the grid and a block
	dim3 block(nThreads_w, nThreads_h, 1);
	dim3 grid(nBlocks_x, nBlocks_y, 1);
	dim3 grid_o(nBlocks_ox, nBlocks_y, 1);

	forCUDA_ShowInfo("PatchMatch starts");

	// setup the random states for all pixels
	kernel_setup_randstates_2d<<<grid_o, block>>>(devStates, w_b, h_b, randSeed);

	cudaDeviceSynchronize();
	forCUDA_ShowInfo("1");

	// initialize all the parameter maps with uniform random parameters
	kernel_PatchMatch_randinit<<<grid_o, block>>>(devStates, d_depth, d_alpha, d_beta, w_b, h_b, min_d, max_d, min_a, max_a, min_b, max_b);

	cudaDeviceSynchronize();
	forCUDA_ShowInfo("2");

	cudaMemcpy(h_depth, d_depth, sizeMemMaps, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_alpha, d_alpha, sizeMemMaps, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_beta, d_beta, sizeMemMaps, cudaMemcpyDeviceToHost);

	forCUDA_ShowInfo("PatchMatch ends");

	// save the results as images
	forCUDA_SaveMatAsImage("E:\\results\\depth.png", h_depth, w_b, h_b, min_d, max_d);
	forCUDA_SaveMatAsImage("E:\\results\\alpha.png", h_alpha, w_b, h_b, min_a, max_a);
	forCUDA_SaveMatAsImage("E:\\results\\beta.png", h_beta, w_b, h_b, min_b, max_b);
	
	cudaFree(d_imgb);
	cudaFree(d_imgm);
	cudaFree(d_depth);
	cudaFree(d_alpha);
	cudaFree(d_beta);
	cudaFree(devStates);
}

//}

