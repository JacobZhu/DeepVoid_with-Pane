#include "stdafx.h"

#include "DeepVoid.h"
#include "MainFrm.h"

// 该函数在优化用欧拉角和平移向量表示的视图外参数以及各物点世界坐标时用来计算第 i 个物点在第 j 幅视图上的重投影像点坐标，以供 sba 的函数调用
void DeepVoid::proj_ext_euler_str(int j,			// 当前视图的编号
								  int i,			// 当前点的编号
								  double * aj,		// 当前视图的外参数，前 3 个为视图姿态角 yaw, pitch, roll， 后 3 个为视图的平移向量 [tx, ty, tz]
								  double * bi,		// 当前点的三维坐标，[X, Y, Z]
								  double * xij,     // 当前点在当前视图中的重投影像点坐标，[x, y]
								  void * adata		// 传进来的视图额外的已有信息，这里主要传进来视图的内参数以及像差类型和像差系数，为了计算带像差重投影像点
								  )
{
	vector<cam_data> * pdata;
	pdata =(vector<cam_data> *)adata;
	cam_data cam = (*pdata)[j];

	double yaw = aj[0];
	double pit = aj[1];
	double rol = aj[2];
	double tx  = aj[3];
	double ty  = aj[4];
	double tz  = aj[5];

	CMatrix mRT = GenT(tx, ty, tz) * GenR_Euler(rol, pit, yaw, ZYX);

	CMatrix mK(3, 4, 0);

	mK(1, 1) = cam.fx;	mK(1, 2) = cam.s;	mK(1, 3) = cam.cx;	mK(1, 4) = 0;	
	mK(2, 1) = 0;		mK(2, 2) = cam.fy;	mK(2, 3) = cam.cy;	mK(2, 4) = 0;
	mK(3, 1) = 0;		mK(3, 2) = 0;		mK(3, 3) = 1;		mK(3, 4) = 0;

	CMatrix mWrdPt(4, 1, 1);

	mWrdPt(1) = bi[0];
	mWrdPt(2) = bi[1];
	mWrdPt(3) = bi[2];

	CMatrix mImgPt_reproj = mK * mRT * mWrdPt;
	mImgPt_reproj(1) = mImgPt_reproj(1) / mImgPt_reproj(3);
	mImgPt_reproj(2) = mImgPt_reproj(2) / mImgPt_reproj(3);
	mImgPt_reproj(3) = mImgPt_reproj(3) / mImgPt_reproj(3);

	CMatrix mDist(5, 1);
	mDist(1) = cam.k[0];
	mDist(2) = cam.k[1];
	mDist(3) = cam.k[2];
	mDist(4) = cam.k[3];
	mDist(5) = cam.k[4];

	if (0 == cam.dist_type)
	{
		mImgPt_reproj = CalcuDistedImgPt_Weng(mImgPt_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else if (1 == cam.dist_type)
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}

	xij[0] = mImgPt_reproj(1);
	xij[1] = mImgPt_reproj(2);
}

// 该函数在优化用旋转向量和平移向量表示的视图外参数以及各物点世界坐标时用来计算第 i 个物点在第 j 幅视图上的重投影像点坐标，以供 sba 的函数调用
void DeepVoid::proj_ext_rotvec_str(int j,				// 当前视图的编号
								   int i,				// 当前点的编号
								   double * aj,			// 当前视图的外参数，前 3 个为旋转向量， 后 3 个为视图的平移向量 [tx, ty, tz]
								   double * bi,			// 当前点的三维坐标，[X, Y, Z]
								   double * xij,        // 当前点在当前视图中的重投影像点坐标，[x, y]
								   void * adata			// 传进来的视图额外的已有信息，这里主要传进来视图的内参数以及像差类型和像差系数，为了计算带像差重投影像点
								   )
{
	vector<cam_data> * pdata;
	pdata =(vector<cam_data> *)adata;
	cam_data cam = (*pdata)[j];

	double rv1 = aj[0];
	double rv2 = aj[1];
	double rv3 = aj[2];
	double tx  = aj[3];
	double ty  = aj[4];
	double tz  = aj[5];

	CMatrix mRT = GenT(tx, ty, tz) * GenR_Rodrigues(rv1, rv2, rv3);

	CMatrix mK(3, 4, 0);

	mK(1, 1) = cam.fx;	mK(1, 2) = cam.s;	mK(1, 3) = cam.cx;	mK(1, 4) = 0;	
	mK(2, 1) = 0;		mK(2, 2) = cam.fy;	mK(2, 3) = cam.cy;	mK(2, 4) = 0;
	mK(3, 1) = 0;		mK(3, 2) = 0;		mK(3, 3) = 1;		mK(3, 4) = 0;

	CMatrix mWrdPt(4, 1, 1);

	mWrdPt(1) = bi[0];
	mWrdPt(2) = bi[1];
	mWrdPt(3) = bi[2];

	CMatrix mImgPt_reproj = mK * mRT * mWrdPt;
	double Z_inv = 1 / mImgPt_reproj(3);
	mImgPt_reproj(1) = mImgPt_reproj(1) * Z_inv;
	mImgPt_reproj(2) = mImgPt_reproj(2) * Z_inv;
	mImgPt_reproj(3) = mImgPt_reproj(3) * Z_inv;

	CMatrix mDist(5, 1);
	mDist(1) = cam.k[0];
	mDist(2) = cam.k[1];
	mDist(3) = cam.k[2];
	mDist(4) = cam.k[3];
	mDist(5) = cam.k[4];

	if (0 == cam.dist_type)
	{
		mImgPt_reproj = CalcuDistedImgPt_Weng(mImgPt_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else if (1 == cam.dist_type)
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}

	xij[0] = mImgPt_reproj(1);
	xij[1] = mImgPt_reproj(2);
}

void DeepVoid::jacob_ext_euler_str(int j,				// current index of image
								   int i,				// current index of object point
								   double * aj,			// parameter of current image, [yaw pitch roll tx ty tz]'
								   double * bi,			// current object point coordinates [X Y Z]'
								   double * Aij,		// the bi's projection on image aj derivative with respective to aj
								   double * Bij,		// the bi's projection on image aj derivative with respective to bi
								   void * adata			// extra data
								   )
{
	double yaw = aj[0];
	double pit = aj[1];
	double rol = aj[2];
	double tx  = aj[3];
	double ty  = aj[4];
	double tz  = aj[5];

	double X = bi[0];
	double Y = bi[1];
	double Z = bi[2];
}

void DeepVoid::jacob_ext_rotvec_str(int j,				// current index of image
									int i,				// current index of object point
									double * aj,		// parameter of current image, [rv1 rv2 rv3 tx ty tz]'
									double * bi,		// current object point coordinates [X Y Z]'
									double * Aij,		// the bi's projection on image aj derivative with respective to aj
									double * Bij,		// the bi's projection on image aj derivative with respective to bi
									void * adata		// extra data
									)
{
	vector<cam_data> * pdata;
	pdata =(vector<cam_data> *)adata;
	cam_data cam = (*pdata)[j];

	double rv1 = aj[0];
	double rv2 = aj[1];
	double rv3 = aj[2];
	double tx  = aj[3];
	double ty  = aj[4];
	double tz  = aj[5];

	double X = bi[0];
	double Y = bi[1];
	double Z = bi[2];

	CMatrix mRT = GenT(tx, ty, tz) * GenR_Rodrigues(rv1, rv2, rv3);

	CMatrix mRotVec(3,1);
	mRotVec(1) = rv1;
	mRotVec(2) = rv2;
	mRotVec(3) = rv3;

	CMatrix mWrdPt(4, 1, 1);
	mWrdPt(1) = X;
	mWrdPt(2) = Y;
	mWrdPt(3) = Z;

	CMatrix mK(3, 4, 0);
	mK(1, 1) = cam.fx;	mK(1, 2) = cam.s;	mK(1, 3) = cam.cx;	mK(1, 4) = 0;	
	mK(2, 1) = 0;		mK(2, 2) = cam.fy;	mK(2, 3) = cam.cy;	mK(2, 4) = 0;
	mK(3, 1) = 0;		mK(3, 2) = 0;		mK(3, 3) = 1;		mK(3, 4) = 0;

	CMatrix mImgPt_reproj = mK * mRT * mWrdPt;
	double Z_inv = 1 / mImgPt_reproj(3);
	mImgPt_reproj(1) = mImgPt_reproj(1) * Z_inv;
	mImgPt_reproj(2) = mImgPt_reproj(2) * Z_inv;
	mImgPt_reproj(3) = mImgPt_reproj(3) * Z_inv;

	CMatrix mDist(5, 1);
	mDist(1) = cam.k[0];
	mDist(2) = cam.k[1];
	mDist(3) = cam.k[2];
	mDist(4) = cam.k[3];
	mDist(5) = cam.k[4];

	CMatrix mJacob_xy_RT = Jacobian_xy_RT(mRT, mK, mWrdPt); // 2*12 matrix

	CMatrix mJacob_RT_ExtPara = Jacobian_RT_ExtPara_RotVec(mRotVec); // 12*6 matrix

	CMatrix mJacob_xy_ExtPara = mJacob_xy_RT * mJacob_RT_ExtPara; // 2*6 matrix

	CMatrix mJacob_xy_XYZ = Jacobian_xy_XYZ(X, Y, Z, mK, mRT); // 2*3 matrix

	CMatrix mJacob_distxy_xy, mJacob_distxy_ExtPara, mJacob_distxy_XYZ;

	if (!cam.dist_type)
	{
		mJacob_distxy_xy = Jacobian_distxy_xy_Weng(mImgPt_reproj, mK, mDist); // weng's 像差系数
	} 
	else
	{
		mJacob_distxy_xy = Jacobian_distxy_xy_DCBrown(mImgPt_reproj(1), mImgPt_reproj(2), mK, mDist); // D.C.Brown's 像差系数
	}

	mJacob_distxy_ExtPara = mJacob_distxy_xy * mJacob_xy_ExtPara; // 2*6 matrix
	mJacob_distxy_XYZ = mJacob_distxy_xy * mJacob_xy_XYZ; // 2*3 matrix

// 	if (j!=0)
// 	{
// 		mJacob_distxy_xy.PrintMatrix2Screen();
// 		mJacob_xy_ExtPara.PrintMatrix2Screen();
// 		mJacob_xy_RT.PrintMatrix2Screen();
// 		mJacob_RT_ExtPara.PrintMatrix2Screen();
// 	}
	
	int ii,jj;
	for (ii=0;ii<2;ii++)
	{
		for (jj=0;jj<6;jj++)
		{
			Aij[ii*6+jj] = mJacob_distxy_ExtPara(ii+1,jj+1);
		}
	}

	for (ii=0;ii<2;ii++)
	{
		for (jj=0;jj<3;jj++)
		{
			Bij[ii*3+jj] = mJacob_distxy_XYZ(ii+1,jj+1);
		}
	}
}

// 该函数封装了 Sparse Bundle Adjustment 库的光束法平差函数
// 返回所有重投影像点个数
/*int DeepVoid::optim_sba_levmar_XYZ_ext_euler(vector<CloudPoint> & cloudPts,		// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
											 vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新

											 int itermax,			// 输入：最大迭代次数

											 double * opts,			// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																				// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																				// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																				// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																				// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																				// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

											 double * info 			// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																				// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																				// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																				// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																				// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																				// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																				// info[5]，# iterations，					总迭代次数
																				// info[6]，reason for terminating，		迭代结束原因：
																															// 1. 目标函数对优化参数导数太小
																															// 2. 改正量，即优化参数变化太小
																															// 3. 达到最大迭代次数
																															// 4. 残差相对变化太小
																															// 5. too many attempts to increase damping. Restart with increased mu
																															// 6. 残差太小
																															// 7. stopped by invalid (i.e. NaN or Inf) "func" values. This is a user error
																				// info[7]，# function evaluations，		目标函数调用次数
																				// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																				// info[9]，# number of linear systems solved，求解线性方程组的个数
											 )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = cloudPts.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<cam_data> vCams;	// 参与光束法平差的各视图参数
	
	int i,j,k;
	for (i = 0; i < cams.size(); i++)
	{
		cam_data cam = cams[i];
		if (cam.R[0] < -90) // 由于旋转矩阵 R 肯定是正交矩阵，因此其每一列应该都是单位向量，也就是说每个元素绝对值都不应超过 1，R 的每个元素初始化为 -100，因此若其元素为-100，说明其外参数肯定是没有初值的
		{
			continue;
		}

		vIdxCams.push_back(i);
		vCams.push_back(cam);
	}

	int m = vCams.size();  // 参与光束法平差的视图数目

	// initial parameter vector p0: (a1, ..., am, b1, ..., bn).
	double * p = new double[3 * n + 6 * m];

	for (i = 0; i < m; i++)
	{
		CMatrix mR(3, 3);
		cam_data cam = vCams[i];
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 3; k++)
			{
				mR(j+1, k+1) = cam.R[j*3+k];
			}
		}

		CMatrix mAngs = GetEulerAngleFromR(mR);

		p[i * 6 + 0] = mAngs(1, 1);
		p[i * 6 + 1] = mAngs(2, 1);
		p[i * 6 + 2] = mAngs(3, 1);
		p[i * 6 + 3] = cam.t[0];
		p[i * 6 + 4] = cam.t[1];
		p[i * 6 + 5] = cam.t[2];
	}

	for (i = 0; i < n; i++)
	{
		CloudPoint pt = cloudPts[i];

		p[6 * m + i * 3 + 0] = pt.m_pt.x;
		p[6 * m + i * 3 + 1] = pt.m_pt.y;
		p[6 * m + i * 3 + 2] = pt.m_pt.z;
	}

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	// 生成可视矩阵
	char * vmask = new char[m * n];
	memset(vmask, 0, m*n*sizeof(char));
	for (i = 0; i < n; i++)
	{
		CloudPoint cldpt = cloudPts[i];

		for (j=0;j<m;j++)
		{
			int idxcam = vIdxCams[j];

			for (k=0;k<cldpt.m_vImgInfos.size();k++)
			{
				CloudPoint_ImgInfo imginfo = cldpt.m_vImgInfos[k];
				if (imginfo.m_idxImg == idxcam)
				{
					vmask[i*m+j]=1;
					Point2d imgpt;
					imgpt.x = vCams[j].m_feats.key_points[imginfo.m_idxImgPt].pt.x;
					imgpt.y = vCams[j].m_feats.key_points[imginfo.m_idxImgPt].pt.y;
					vImgPts_vmask.push_back(imgpt);
					break;
				}
			}
		}
	}

	// 生成观测向量
	double * x = new double[vImgPts_vmask.size() * 2];

	for (i=0;i<vImgPts_vmask.size();i++)
	{
		x[2 * i + 0] = vImgPts_vmask[i].x;
		x[2 * i + 1] = vImgPts_vmask[i].y;
	}

	// 调用 sba 库的光束法平差函数
	double opts_default[5];

	// 初次初始化 u 时需要用到的一个控制量，The algorithm is not very sensitive to the choice of t, but as a rule of thumb,
	// one should use a small value, eg t=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use
	// t=1E-3 or even t=1
	opts_default[0] = 1.0E-3;
	opts_default[1] = 1.0E-12;
	opts_default[2] = 1.0E-12;
	opts_default[3] = 1.0E-12;
	opts_default[4] = 0;

	// 如果用户没有输入控制参数，就使用默认的控制参数
	if (!opts)
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_euler_str, NULL, &vCams, itermax, 0, opts_default, info);
	} 
	else // 不为空就使用用户输入的参数
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_euler_str, NULL, &vCams, itermax, 0, opts, info);
	}

	// 下面还要将优化完的参数和点坐标全部赋回去
	for (i = 0; i < m; i++)
	{
		double yaw = p[i * 6 + 0];
		double pit = p[i * 6 + 1];
		double rol = p[i * 6 + 2];
		double tx  = p[i * 6 + 3];
		double ty  = p[i * 6 + 4];
		double tz  = p[i * 6 + 5];

		CMatrix mR = GenR_Euler(rol, pit, yaw, ZYX);

		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 3; k++)
			{
				cams[vIdxCams[i]].R[j*3+k] = mR(j+1, k+1);
			}
		}

		// 接着更新平移向量
		cams[vIdxCams[i]].t[0] = tx;
		cams[vIdxCams[i]].t[1] = ty;
		cams[vIdxCams[i]].t[2] = tz;
	}

	// 把优化后的点坐标赋回去
	for (i=0;i<n;i++)
	{
		cloudPts[i].m_pt.x = p[6 * m + i * 3 + 0];
		cloudPts[i].m_pt.y = p[6 * m + i * 3 + 1];
		cloudPts[i].m_pt.z = p[6 * m + i * 3 + 2];
	}

	// 释放内存
	delete [] p;
	delete [] vmask;
	delete [] x;

	return vImgPts_vmask.size();
}*/

// 该函数封装了 Sparse Bundle Adjustment 库的光束法平差函数
// 返回所有重投影像点个数
/*int DeepVoid::optim_sba_levmar_XYZ_ext_rotvec(vector<CloudPoint> & cloudPts,	// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
											  vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
											  int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

											  int itermax,			// 输入：最大迭代次数

											  double * opts ,			// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																				// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																				// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																				// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																				// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																				// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

											  double * info 			// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																				// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																				// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																				// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																				// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																				// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																				// info[5]，# iterations，					总迭代次数
																				// info[6]，reason for terminating，		迭代结束原因：
																															// 1. 目标函数对优化参数导数太小
																															// 2. 改正量，即优化参数变化太小
																															// 3. 达到最大迭代次数
																															// 4. 残差相对变化太小
																															// 5. 残差太小
																															// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																															//	  definite normal equations matrix. Typically, this indicates a programming error in the
																															//    user-supplied Jacobian.
																															// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																															//    This signals a programming error in the user-supplied projection function func.
																				// info[7]，# function evaluations，		目标函数调用次数
																				// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																				// info[9]，# number of linear systems solved，求解线性方程组的个数
											  )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = cloudPts.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<cam_data> vCams;	// 参与光束法平差的各视图参数

	vIdxCams.push_back(idx_refimg);
	vCams.push_back(cams[idx_refimg]);
	
	int i,j,k;
	for (i = 0; i < cams.size(); i++)
	{
		cam_data cam = cams[i];
		if (cam.R[0] < -90 || i==idx_refimg) // 由于旋转矩阵 R 肯定是正交矩阵，因此其每一列应该都是单位向量，也就是说每个元素绝对值都不应超过 1，R 的每个元素初始化为 -100，因此若其元素为-100，说明其外参数肯定是没有初值的
		{
			continue;
		}

		vIdxCams.push_back(i);
		vCams.push_back(cam);
	}

	int m = vCams.size();  // 参与光束法平差的视图数目

	// initial parameter vector p0: (a1, ..., am, b1, ..., bn).
	double * p = new double[3 * n + 6 * m];

	for (i = 0; i < m; i++)
	{
		CMatrix mR(3, 3);
		cam_data cam = vCams[i];
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 3; k++)
			{
				mR(j+1, k+1) = cam.R[j*3+k];
			}
		}

		CMatrix mRotVec = GetRotVecFromR(mR);

		p[i * 6 + 0] = mRotVec(1);
		p[i * 6 + 1] = mRotVec(2);
		p[i * 6 + 2] = mRotVec(3);
		p[i * 6 + 3] = cam.t[0];
		p[i * 6 + 4] = cam.t[1];
		p[i * 6 + 5] = cam.t[2];
	}

	for (i = 0; i < n; i++)
	{
		CloudPoint pt = cloudPts[i];

		p[6 * m + i * 3 + 0] = pt.m_pt.x;
		p[6 * m + i * 3 + 1] = pt.m_pt.y;
		p[6 * m + i * 3 + 2] = pt.m_pt.z;
	}

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	// 生成可视矩阵
	char * vmask = new char[m * n];
	memset(vmask, 0, m*n*sizeof(char));
	for (i = 0; i < n; i++)
	{
		CloudPoint cldpt = cloudPts[i];

		for (j=0;j<m;j++)
		{
			int idxcam = vIdxCams[j];

			for (k=0;k<cldpt.m_vImgInfos.size();k++)
			{
				CloudPoint_ImgInfo imginfo = cldpt.m_vImgInfos[k];
				if (imginfo.m_idxImg == idxcam)
				{
					vmask[i*m+j]=1;
					Point2d imgpt;
					imgpt.x = vCams[j].m_feats.key_points[imginfo.m_idxImgPt].pt.x;
					imgpt.y = vCams[j].m_feats.key_points[imginfo.m_idxImgPt].pt.y;
					vImgPts_vmask.push_back(imgpt);

					break;
				}
			}
		}
	}

	// 生成观测向量
	double * x = new double[vImgPts_vmask.size() * 2];

	for (i=0;i<vImgPts_vmask.size();i++)
	{
		x[2 * i + 0] = vImgPts_vmask[i].x;
		x[2 * i + 1] = vImgPts_vmask[i].y;
	}

	// 调用 sba 库的光束法平差函数
	double opts_default[5];

	// 初次初始化 u 时需要用到的一个控制量，The algorithm is not very sensitive to the choice of t, but as a rule of thumb,
	// one should use a small value, eg t=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use
	// t=1E-3 or even t=1
	opts_default[0] = 1.0E-3;
	opts_default[1] = 1.0E-12;
	opts_default[2] = 1.0E-12;
	opts_default[3] = 1.0E-12;
	opts_default[4] = 0;

	// 如果用户没有输入控制参数，就使用默认的控制参数
	if (!opts)
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_rotvec_str, &jacob_ext_rotvec_str, &vCams, itermax, 0, opts_default, info);
	} 
	else // 不为空就使用用户输入的参数
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_rotvec_str, &jacob_ext_rotvec_str, &vCams, itermax, 0, opts, info);
	}

	// 下面还要将优化完的参数和点坐标全部赋回去
	for (i = 0; i < m; i++)
	{
		double rv1 = p[i * 6 + 0];
		double rv2 = p[i * 6 + 1];
		double rv3 = p[i * 6 + 2];
		double tx  = p[i * 6 + 3];
		double ty  = p[i * 6 + 4];
		double tz  = p[i * 6 + 5];

		CMatrix mR = GenR_Rodrigues(rv1, rv2, rv3);

		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 3; k++)
			{
				cams[vIdxCams[i]].R[j*3+k] = mR(j+1, k+1);
			}
		}

		// 接着更新平移向量
		cams[vIdxCams[i]].t[0] = tx;
		cams[vIdxCams[i]].t[1] = ty;
		cams[vIdxCams[i]].t[2] = tz;
	}

	// 把优化后的点坐标赋回去
	for (i=0;i<n;i++)
	{
		cloudPts[i].m_pt.x = p[6 * m + i * 3 + 0];
		cloudPts[i].m_pt.y = p[6 * m + i * 3 + 1];
		cloudPts[i].m_pt.z = p[6 * m + i * 3 + 2];
	}

	// 释放内存
	delete [] p;
	delete [] vmask;
	delete [] x;

	return vImgPts_vmask.size();
}*/

// 该函数封装了 Sparse Bundle Adjustment 库的光束法平差函数
// 返回所有重投影像点个数
int DeepVoid::optim_sba_levmar_XYZ_ext_rotvec_own(vector<CloudPoint> & cloudPts,	// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
											  vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
											  int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

											  int itermax /*= 1024*/,			// 输入：最大迭代次数

											  double * opts /*= NULL*/,			// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																				// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																				// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																				// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																				// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																				// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

											  double * info /*= NULL*/			// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																				// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																				// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																				// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																				// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																				// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																				// info[5]，# iterations，					总迭代次数
																				// info[6]，reason for terminating，		迭代结束原因：
																															// 1. 目标函数对优化参数导数太小
																															// 2. 改正量，即优化参数变化太小
																															// 3. 达到最大迭代次数
																															// 4. 残差相对变化太小
																															// 5. 残差太小
																															// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																															//	  definite normal equations matrix. Typically, this indicates a programming error in the
																															//    user-supplied Jacobian.
																															// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																															//    This signals a programming error in the user-supplied projection function func.
																				// info[7]，# function evaluations，		目标函数调用次数
																				// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																				// info[9]，# number of linear systems solved，求解线性方程组的个数
											  )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = cloudPts.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<cam_data> vCams;	// 参与光束法平差的各视图参数

	vIdxCams.push_back(idx_refimg);
	vCams.push_back(cams[idx_refimg]);
	
	for (int i=0;i<cams.size();++i)
	{
		cam_data cam = cams[i];
		if (cam.R[0] < -90 || i==idx_refimg) // 由于旋转矩阵 R 肯定是正交矩阵，因此其每一列应该都是单位向量，也就是说每个元素绝对值都不应超过 1，R 的每个元素初始化为 -100，因此若其元素为-100，说明其外参数肯定是没有初值的
		{
			continue;
		}

		vIdxCams.push_back(i);
		vCams.push_back(cam);
	}

	int m = vCams.size();  // 参与光束法平差的视图数目

	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double,5,1>> dists;
	vector<int> distTypes;

	for (int i=0;i<m;++i)
	{
		Matx33d R;

		cam_data cam = vCams[i];

		for (int j=0; j<3;++j)
		{
			for (int k=0; k<3;++k)
			{
				R(j,k) = cam.R[j*3+k];
			}
		}

		Matx33d K;
		K(0,0) = cam.fx;
		K(1,1) = cam.fy;
		K(0,1) = cam.s;
		K(0,2) = cam.cx;
		K(1,2) = cam.cy;
		K(2,2) = 1;

		Matx31d t;
		for (int j=0;j<3;++j)
		{
			t(j) = cam.t[j];
		}

		Matx<double,5,1> dist;
		for (int j=0;j<5;++j)
		{
			dist(j) = cam.k[j];
		}

		Ks.push_back(K);
		Rs.push_back(R);
		ts.push_back(t);
		dists.push_back(dist);
		distTypes.push_back(cam.dist_type);
	}

	vector<Point3d> XYZs;

	for (int i=0;i<n;++i)
	{
		CloudPoint pt = cloudPts[i];

		Point3d XYZ;
		XYZ.x = pt.m_pt.x;
		XYZ.y = pt.m_pt.y;
		XYZ.z = pt.m_pt.z;

		XYZs.push_back(XYZ);
	}

	int sizes[] = {n, m};
	SparseMat ptrMat(2,sizes,CV_32SC1);

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	vector<Matx31d> nxys(n); // 每个物点关联的参考像点去像差归一化像点坐标
	vector<int> ri_j(n); // 每个物点关联的参考图像

	// 生成可视索引矩阵，ptrMat(i,j) 存放着 Aij、Bij和eij等于其向量中的索引
	// ptrMat 是稀疏矩阵
	srand((unsigned)time(NULL));
	for (int i=0;i<n;++i)
	{
		CloudPoint cldpt = cloudPts[i];

		vector<int> j_visi; // 观测到物点 i 的各图像索引
		vector<Point2d> xys; // 点 i 在各观测图像中的观测像点坐标

		for (int j=0;j<m;++j)
		{
			int idxcam = vIdxCams[j];

			for (int k=0;k<cldpt.m_vImgInfos.size();++k)
			{
				CloudPoint_ImgInfo imginfo = cldpt.m_vImgInfos[k];
				if (imginfo.m_idxImg == idxcam)
				{
					Point2d imgpt;
					imgpt.x = vCams[j].m_feats.key_points[imginfo.m_idxImgPt].pt.x;
					imgpt.y = vCams[j].m_feats.key_points[imginfo.m_idxImgPt].pt.y;
					vImgPts_vmask.push_back(imgpt);

					ptrMat.ref<int>(i,j) = vImgPts_vmask.size()-1;

					j_visi.push_back(j);
					xys.push_back(imgpt);

					break;
				}
			}
		}

		// 随机选取一个观测图像作为该物点 i 的参考图像
		int j_ref = rand()%j_visi.size();

		int j = j_visi[j_ref];
		Point2d xy = xys[j_ref];

		ri_j[i] = j;

		Matx33d K = Ks[j];
		Matx<double,5,1> dist = dists[j];
		int distType = distTypes[j];

		// 去像差得到理想像点坐标
		double ideal_x, ideal_y;
		if (distType==0)
		{
			distortions::remove_weng(K(0,0),K(1,1),K(0,2),K(1,2),K(0,1),
				dist(0),dist(1),dist(2),dist(3),dist(4),xy.x,xy.y,ideal_x,ideal_y);
		} 
		else
		{
			distortions::remove_brown(K(0,0),K(1,1),K(0,2),K(1,2),K(0,1),
				dist(0),dist(1),dist(2),dist(3),dist(4),xy.x,xy.y,ideal_x,ideal_y);
		}

		// 进而得到理想的归一化像点坐标
		Matx31d nxy;
		nxy(0) = ideal_x;
		nxy(1) = ideal_y;
		nxy(2) = 1;

		nxys[i] = calib::invK(K)*nxy;
	}

	int l = vImgPts_vmask.size(); // 所有观测像点的个数

	// j_fixed 和 i_fixed 向量分别存放图像和空间点是否固定的情况
	// j_fixed[j]=1表示图 j 的参数不参与平差调整，i_fixed[i]=1 表示点 i 的坐标不参与平差调整
	vector<uchar> j_fixed(m),i_fixed(n);
	j_fixed[0] = 1;

	// covInv 存放每个观测像点的不确定度（协方差矩阵）的逆矩阵，其实也就是每个观测像点的权值矩阵
	Matx22d cov;
	cov(0,0) = cov(1,1) = 1;
	vector<Matx22d> covInvs;
	for (int k=0;k<l;++k)
	{
		covInvs.push_back(cov);
	}

	double tau  = opts[0]; 
	double eps1 = opts[1];			// input:	threshold
	double eps2 = opts[2];			// input:	threshold

//	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
//	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
//	SBA_ZZK::optim_sparse_lm_wj_cj_XiYiZiWi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
//	SBA_ZZK::optim_sparse_lm_wj_cj_XiYiZi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
//	SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
//	SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_c_fixedBaseline(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,0,1,1,info,tau,itermax,eps1,eps2);
// 	for (int iiii=0;iiii<50;++iiii)
// 	{
// 		vector<Point3d> XYZs_tmp = XYZs;
// 		vector<Matx33d> Rs_tmp = Rs;
// 		vector<Matx31d> ts_tmp = ts;
// 
// 		SBA_ZZK::optim_sparse_lm_wj_tj_di(XYZs_tmp,Ks,Rs_tmp,ts_tmp,dists,distTypes,vImgPts_vmask,covInvs,ri_j,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
// 		SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi(XYZs_tmp,Ks,Rs_tmp,ts_tmp,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
// 	}
//	SBA_ZZK::optim_sparse_lm_wj_tj_di(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,ri_j,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
	SBA_ZZK::optim_sparse_lm_wj_tj_di(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,nxys,ri_j,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
// 	Matx31d leng = -Rs[1]*Rs[0].t()*ts[0] + ts[1];
// 	double dleng = norm(leng);
// 
// 	vector<Matx<double,6,6>> cov_a;
// 	vector<Matx<double,3,3>> cov_b;
// 	Matx<double,1,1> cov_c;
// 	vector<Matx<double,1,6>> cov_ca;
// 	derivatives::covarianceSBA_f_wj_cj_XiYiZi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,cov_a,cov_b,cov_c,cov_ca);
//	derivatives::covarianceSBA_wj_cj_XiYiZi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,cov_a,cov_b);

	// 下面还要将优化完的参数和点坐标全部赋回去
	for (int i=0;i<m;++i)
	{
		Matx33d R = Rs[i];
		Matx31d t = ts[i];

		for (int j=0;j<3;++j)
		{
			for (int k=0;k<3;++k)
			{
				cams[vIdxCams[i]].R[j*3+k] = R(j,k);
			}
		}

		// 接着更新平移向量
		cams[vIdxCams[i]].t[0] = t(0);
		cams[vIdxCams[i]].t[1] = t(1);
		cams[vIdxCams[i]].t[2] = t(2);
	}

	// 把优化后的点坐标赋回去
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		cloudPts[i].m_pt.x = XYZ.x;
		cloudPts[i].m_pt.y = XYZ.y;
		cloudPts[i].m_pt.z = XYZ.z;
	}

	return l;
}

// 20151105，自己基于 OpenCV 编写的 SBA 函数
// 返回所有重投影像点个数
int  DeepVoid::optim_sba_levmar_XYZ_ext_rotvec(SfM_ZZK::PointCloud & map_pointcloud,// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
											   vector<cam_data> & cams,				// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
											   SfM_ZZK::MultiTracks & map_tracks,	// 输入：所有的特征轨迹
											   int idx_refimg,						// input:	the reference image, whose R=I, and t =[0,0,0]'
											   double tc /*= 1.5*/,					// input:	用来计算 Huber 权重的常量
											   int itermax /*= 1024*/,				// 输入：最大迭代次数
											   double * opts /*= NULL*/,			// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																					// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																					// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																					// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																					// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																					// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0
											   double * info /*= NULL*/				// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																					// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																					// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																					// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																					// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																					// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																					// info[5]，# iterations，					总迭代次数
																					// info[6]，reason for terminating，		迭代结束原因：
																																// 1. 目标函数对优化参数导数太小
																																// 2. 改正量，即优化参数变化太小
																																// 3. 达到最大迭代次数
																																// 4. 残差相对变化太小
																																// 5. 残差太小
																																// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																																//	  definite normal equations matrix. Typically, this indicates a programming error in the
																																//    user-supplied Jacobian.
																																// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																																//    This signals a programming error in the user-supplied projection function func.
																					// info[7]，# function evaluations，		目标函数调用次数
																					// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																					// info[9]，# number of linear systems solved，求解线性方程组的个数
											   )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = map_pointcloud.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double,5,1>> dists;
	vector<int> distTypes;

	// 这样做是为了保证参考图排在第一位
	cam_data & cam_ref = cams[idx_refimg];
	vIdxCams.push_back(idx_refimg);
	Ks.push_back(cam_ref.m_K);
	Rs.push_back(cam_ref.m_R);
	ts.push_back(cam_ref.m_t);
	dists.push_back(cam_ref.m_dist);
	distTypes.push_back(cam_ref.dist_type);

	for (int i = 0; i < cams.size(); ++i)
	{
		if (!cams[i].m_bOriented || i == idx_refimg)
		{
			continue;
		}

		cam_data & cam = cams[i];
		vIdxCams.push_back(i);
		Ks.push_back(cam.m_K);
		Rs.push_back(cam.m_R);
		ts.push_back(cam.m_t);
		dists.push_back(cam.m_dist);
		distTypes.push_back(cam.dist_type);
	}

	int m = Ks.size();  // 参与光束法平差的视图数目

	vector<Point3d> XYZs;
	
	// 20200607，n行m列的稀疏矩阵，用于存储各点于各图像上的可见性，其实不单单是可见性，其元素值其实为每个像点在像点坐标向量中的序号
	int sizes[] = { n, m };
	SparseMat ptrMat(2, sizes, CV_32SC1);

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	int i_tmp=0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		XYZs.push_back(iter_objpt->second.m_pt);

		auto iter_found_track = map_tracks.find(trackID);

		const SfM_ZZK::OneTrack & one_track = iter_found_track->second;

		for (int j=0;j<m;++j)
		{
			int idxcam = vIdxCams[j];

			auto iter_found_img = one_track.find(idxcam);

			if (iter_found_img == one_track.end())
			{
				// 在该 track 中没有找到指定已完成定向的观测图像
				continue;
			}

			// 找到了指定图像
			Point2d imgpt;
			imgpt.x = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.x;
			imgpt.y = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.y;
			vImgPts_vmask.push_back(imgpt);

			ptrMat.ref<int>(i_tmp, j) = vImgPts_vmask.size() - 1;
		}

		++i_tmp;
	}

	int l = vImgPts_vmask.size(); // 所有观测像点的个数

	// j_fixed 和 i_fixed 向量分别存放图像和空间点是否固定的情况
	// j_fixed[j]=1表示图 j 的参数不参与平差调整，i_fixed[i]=1 表示点 i 的坐标不参与平差调整
	vector<uchar> j_fixed(m), i_fixed(n);
	j_fixed[0] = 1;

	// covInv 存放每个观测像点的不确定度（协方差矩阵）的逆矩阵，其实也就是每个观测像点的权值矩阵
	Matx22d cov;
	cov(0, 0) = cov(1, 1) = 1;
	vector<Matx22d> covInvs;
	for (int k = 0; k < l; ++k)
	{
		covInvs.push_back(cov);
	}

	double tau  = opts[0]; 
	double eps1 = opts[1];			// input:	threshold
	double eps2 = opts[2];			// input:	threshold

	vector<double> vds(l);

//	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, info, tau, itermax, eps1, eps2);
	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);

	// 20201117，输出每个像点的重投影残差，做数据图写报告用 ////////////////////
// 	FILE * file = fopen("C:\\Users\\DeepV\\Desktop\\vds.txt", "w");
// 	 
// 	for (int i = 0; i < vds.size(); ++i)
// 	{
// 		fprintf(file, "%d	%f\n", i, vds[i]);
// 	}
// 	
// 	fclose(file);
	/////////////////////////////////////////////////////////////////////////
	
	// 下面还要将优化完的参数和点坐标全部赋回去
	for (int i=0;i<m;++i)
	{
		int idxcam = vIdxCams[i];

		cam_data & cam = cams[idxcam];

		cam.m_R = Rs[i];
		cam.m_t = ts[i];

		// 20151224，查看左乘微小旋转矩阵的这种更新方式在经过很多次迭代之后会不会导致旋转矩阵特性退化
// 		Matx33d u,vt;
// 		Matx31d w;
// 
// 		SVD::compute(cam.m_R, w, u, vt);
// 
// 		double detR = determinant(cam.m_R);
// 		double a1 = w(0);
// 		double a2 = w(1);
// 		double a3 = w(2);
// 
// 		if (a>0)
// 		{
// 			mR = u*vt;
// 		} 
// 		else
// 		{
// 			mR = -u*vt;
// 		}
	}

	// 用光束法平差完的新的重投影残差来判断内外点
//	double sigma3 = 3*info[1];
	double sigma3 = tc;

	i_tmp=0;
	for (auto iter_objpt=map_pointcloud.begin();iter_objpt!=map_pointcloud.end();++iter_objpt)
	{
		Point3d XYZ = XYZs[i_tmp];

		iter_objpt->second.m_pt = XYZ;

		auto iter_found_track = map_tracks.find(iter_objpt->first);

		Matx31d mXYZ;
		mXYZ(0) = XYZ.x;
		mXYZ(1) = XYZ.y;
		mXYZ(2) = XYZ.z;

		for (int j=0;j<m;++j)
		{
			const int * ptr = ptrMat.find<int>(i_tmp,j);

			if (NULL==ptr) 
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			int idx_cam = vIdxCams[j];
			double rpjerr = vds[*ptr];

			auto iter_found_img = iter_found_track->second.find(idx_cam);

			if (rpjerr < sigma3)
			{
				// 20151227，重投影残差足够小还不行，还得位于该图像正前方，否则该图像依然会被判为外点
				Matx31d mXYZ_C = Rs[j] * mXYZ + ts[j];

				if (mXYZ_C(2)<=0)
				{
					iter_found_img->second.second = 0; // 物点位于图像后方
				}
				else
				{
					iter_found_img->second.second = 1; // 物点位于图像前方
				}
			} 
			else
			{
				iter_found_img->second.second = 0;
			}
		}

		++i_tmp;
	}

	return l;
}

// 20151105，自己基于 OpenCV 编写的 SBA 函数
// 返回所有重投影像点个数
// 20220202，输出不确定度；改用新数据结构 MultiTracksWithFlags
int  DeepVoid::optim_sba_levmar_XYZ_ext_rotvec_IRLS_Huber(SfM_ZZK::PointCloud & map_pointcloud,			// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
														  vector<cam_data> & cams,						// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
														  SfM_ZZK::MultiTracksWithFlags & map_tracks,	// 输入：所有的特征轨迹
														  double & rltUctt_output,						// 输出：所有物点的综合相对不确定度水平（1倍sigma）
														  int idx_refimg,								// input:	the reference image, whose R=I, and t =[0,0,0]'
														  double tc /*= 1.5*/,							// input:	用来计算 Huber 权重的常量
														  int itermax /*= 1024*/,						// 输入：最大迭代次数
														  double * opts /*= NULL*/,						// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																										// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																										// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																										// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																										// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																										// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0
														  double * info /*= NULL*/						// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																										// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																										// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																										// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																										// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																										// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																										// info[5]，# iterations，					总迭代次数
																										// info[6]，reason for terminating，		迭代结束原因：
																																				// 1. 目标函数对优化参数导数太小
																																				// 2. 改正量，即优化参数变化太小
																																				// 3. 达到最大迭代次数
																																				// 4. 残差相对变化太小
																																				// 5. 残差太小
																																				// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																																				//	  definite normal equations matrix. Typically, this indicates a programming error in the
																																				//    user-supplied Jacobian.
																																				// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																																				//    This signals a programming error in the user-supplied projection function func.
																										// info[7]，# function evaluations，		目标函数调用次数
																										// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																										// info[9]，# number of linear systems solved，求解线性方程组的个数
														  )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = map_pointcloud.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double, 5, 1>> dists;
	vector<int> distTypes;

	// 这样做是为了保证参考图排在第一位
	cam_data & cam_ref = cams[idx_refimg];
	vIdxCams.push_back(idx_refimg);
	Ks.push_back(cam_ref.m_K);
	Rs.push_back(cam_ref.m_R);
	ts.push_back(cam_ref.m_t);
	dists.push_back(cam_ref.m_dist);
	distTypes.push_back(cam_ref.dist_type);

	for (int i = 0; i < cams.size(); ++i)
	{
		if (!cams[i].m_bOriented || i == idx_refimg)
		{
			continue;
		}

		cam_data & cam = cams[i];
		vIdxCams.push_back(i);
		Ks.push_back(cam.m_K);
		Rs.push_back(cam.m_R);
		ts.push_back(cam.m_t);
		dists.push_back(cam.m_dist);
		distTypes.push_back(cam.dist_type);
	}

	int m = Ks.size();  // 参与光束法平差的视图数目

	vector<Point3d> XYZs;

	// 20200607，n行m列的稀疏矩阵，用于存储各点于各图像上的可见性，其实不单单是可见性，其元素值其实为每个像点在像点坐标向量中的序号
	int sizes[] = { n, m };
	SparseMat ptrMat(2, sizes, CV_32SC1);

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	int i_tmp = 0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		XYZs.push_back(iter_objpt->second.m_pt);

		auto iter_found_track = map_tracks.find(trackID);

		const SfM_ZZK::trackWithFlags & one_track = iter_found_track->second;

		for (int j = 0; j < m; ++j)
		{
			int idxcam = vIdxCams[j];

			auto iter_found_img = one_track.find(idxcam);

			if (iter_found_img == one_track.end())
			{
				// 在该 track 中没有找到指定已完成定向的观测图像
				continue;
			}

			// 找到了指定图像
			Point2d imgpt;
			imgpt.x = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.x;
			imgpt.y = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.y;
			vImgPts_vmask.push_back(imgpt);

			ptrMat.ref<int>(i_tmp, j) = vImgPts_vmask.size() - 1;
		}

		++i_tmp;
	}

	int l = vImgPts_vmask.size(); // 所有观测像点的个数

	// j_fixed 和 i_fixed 向量分别存放图像和空间点是否固定的情况
	// j_fixed[j]=1表示图 j 的参数不参与平差调整，i_fixed[i]=1 表示点 i 的坐标不参与平差调整
	vector<uchar> j_fixed(m), i_fixed(n);
	j_fixed[0] = 1;

	// covInv 存放每个观测像点的不确定度（协方差矩阵）的逆矩阵，其实也就是每个观测像点的权值矩阵
	Matx22d cov;
	cov(0, 0) = cov(1, 1) = 1;
	vector<Matx22d> covInvs;
	for (int k = 0; k < l; ++k)
	{
		covInvs.push_back(cov);
	}

	double tau  = opts[0];
	double eps1 = opts[1];	// input:	threshold
	double eps2 = opts[2];	// input:	threshold

	vector<double> vds(l);

	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);

	// 20220202，给出参数估计的协方差阵，也就是不确定度，或者内符合精度 /////////////////////////////////////////////////////////////////////////////////////////////
	vector<Matx<double, 6, 6>> cov_a;		// m个图像独有参数的协方差阵
	vector<Matx<double, 3, 3>> cov_b;		// n个空间点的坐标的协方差阵

	derivatives::covarianceSBA_wj_cj_XiYiZi(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, cov_a, cov_b);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 下面还要将优化完的参数和点坐标全部赋回去
	double sum2_sigmaOptCtr = 0;

	for (int i = 0; i < m; ++i)
	{
		int idxcam = vIdxCams[i];

		cam_data & cam = cams[idxcam];

		cam.m_R = Rs[i];
		cam.m_t = ts[i];

		// 20200712，把光心位置协方差矩阵取出来 ///////////////////////////////////////////
		const Matx<double, 6, 6> & cov_img = cov_a[i];

		Matx33d cov_optctr;

		for (int ii = 0; ii < 3; ++ii)
		{
			int iiplus3 = ii + 3;

			for (int jj = 0; jj < 3; ++jj)
			{
				cov_optctr(ii, jj) = cov_img(iiplus3, jj + 3);
			}
		}

		Matx31d eigenVals;
		Matx33d eigenVecs;

		//	@param eigenvalues output vector of eigenvalues of the same type as src; the eigenvalues are stored in the descending order.
		//	@param eigenvectors output matrix of eigenvectors; it has the same size and type as src;
		//	the eigenvectors are stored as subsequent matrix rows, in the same order as the corresponding eigenvalues.
		cv::eigen(cov_optctr, eigenVals, eigenVecs);
		cv::Scalar sum2 = cv::sum(eigenVals);

		sum2_sigmaOptCtr += sum2.val[0];

		// 输出光心位置的不确定度椭球，方便三维可视化
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx13d vec = std::sqrt(eigenVals(ii))*eigenVecs.row(ii);

			for (int jj = 0; jj < 3; ++jj)
			{
				cam.m_optCtrUncertEllipsoid(ii, jj) = vec(jj);
			}
		}
	}

	double rms_sigmaOptCtr = std::sqrt(sum2_sigmaOptCtr / (m - 1));

	// 用固定阈值来判断内外点
	double sigma3 = tc;
//	double sum2_sigmaObjPt = 0;

	typedef std::pair<int, double> pair_nObsv_ang;
	typedef std::pair<double, pair_nObsv_ang> pair_rltUctt_nObsv_ang;

	vector<pair_rltUctt_nObsv_ang> vData; // 20200715，存储每个物点的平均相对不确定度大小，及其有效被观测次数

	i_tmp = 0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		Point3d XYZ = XYZs[i_tmp];

		iter_objpt->second.m_pt = XYZ;

		// 20200712，把当前物点的协方差阵取出来 ///////////////////////////////////////
		const Matx33d & cov_objpt = cov_b[i_tmp];

		Matx31d eigenVals;
		Matx33d eigenVecs;

		cv::eigen(cov_objpt, eigenVals, eigenVecs);

		cv::Scalar sum2 = cv::sum(eigenVals);
		double sum2_all = sum2.val[0];

//		sum2_sigmaObjPt += sum2_all;

		double rms_sigma = std::sqrt(sum2_all); // sigma = sqrt(sigma1^2 + sigma2^2 + sigma3^2)

		// 输出物点位置的不确定度椭球，方便三维可视化
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx13d vec = std::sqrt(eigenVals(ii))*eigenVecs.row(ii);

			for (int jj = 0; jj < 3; ++jj)
			{
				iter_objpt->second.m_uncertaintyEllipsoid(ii, jj) = vec(jj);
			}
		}
		/////////////////////////////////////////////////////////////////////////////

		auto iter_found_track = map_tracks.find(iter_objpt->first);

		Matx31d mXYZ;
		mXYZ(0) = XYZ.x;
		mXYZ(1) = XYZ.y;
		mXYZ(2) = XYZ.z;

		int nValid = 0; // 当前有多少个合理的观测值
		double sum_d = 0;

		vector<Matx31d> vVecObsers;

		for (int j = 0; j < m; ++j)
		{
			const int * ptr = ptrMat.find<int>(i_tmp, j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			int idx_cam = vIdxCams[j];
			double rpjerr = vds[*ptr];

			auto iter_found_img = iter_found_track->second.find(idx_cam);

			if (rpjerr < sigma3)
			{
				// 20151227，重投影残差足够小还不行，还得位于该图像正前方，否则该图像依然会被判为外点
				const Matx33d & R = Rs[j];
				const Matx31d & t = ts[j];

				Matx31d mXYZ_C = R*mXYZ + t;

				if (mXYZ_C(2) <= 0)
				{
					iter_found_img->second.second[0] = 0; // 物点位于图像后方
				}
				else
				{
					iter_found_img->second.second[0] = 1; // 物点位于图像前方

					// 20200716，记录下当前图像对物点的观测方向（在世界系中给出）
					Matx31d C = -R.t()*t;
					Matx31d vecObser = mXYZ - C;
					vVecObsers.push_back(vecObser);

					// 有效时，算下观测距离
					double d = cv::norm(mXYZ_C, cv::NormTypes::NORM_L2);

					sum_d += d;

					++nValid;
				}
			}
			else
			{
				iter_found_img->second.second[0] = 0;
			}
		}

		if (nValid >= 1) // 至少得有 1 个有效观测就可以算相对不确定度了
		{
			double rltUctt_overall = nValid*rms_sigma / sum_d;

			double maxAng = maxAngleBetween3DVecs(vVecObsers);

			vData.push_back(std::make_pair(rltUctt_overall, std::make_pair(nValid, maxAng)));

			iter_objpt->second.m_rltUctt = rltUctt_overall; // 输出
		}

		++i_tmp;
	}

	// 相对不确定度中值
	int nnn = vData.size();
	int idx_median = 0.5*nnn;
	std::sort(vData.begin(), vData.end(), [](const pair_rltUctt_nObsv_ang & a, const pair_rltUctt_nObsv_ang & b) {return a.first < b.first; });
	rltUctt_output = vData[idx_median].first;

	return l;
}

// 20200607，采用迭代重加权最小二乘 IRLS 机制同时优化 f,XYZW,R,t
// 返回所有重投影像点个数
int DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(SfM_ZZK::PointCloud & map_pointcloud,// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
														   vector<cam_data> & cams,				// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
														   SfM_ZZK::MultiTracks & map_tracks,	// 输入：所有的特征轨迹
														   double & rltUctt_output,				// 输出：所有物点的综合相对不确定度水平（1倍sigma）
														   double & uctt_f,						// 输出：图像共有等效焦距的不确定度（1倍sigma）
														   int idx_refimg,						// input:	the reference image, whose R=I, and t =[0,0,0]'
														   double tc /*= 1.5*/,					// input:	用来计算 Huber 权重的常量
														   int itermax /*= 1024*/,				// 输入：最大迭代次数
														   double * opts /*= NULL*/,			// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																								// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																								// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																								// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																								// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																								// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0
														   double * info /*= NULL*/				// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																								// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																								// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																								// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																								// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																								// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																								// info[5]，# iterations，					总迭代次数
																								// info[6]，reason for terminating，		迭代结束原因：
																								// 1. 目标函数对优化参数导数太小
																								// 2. 改正量，即优化参数变化太小
																								// 3. 达到最大迭代次数
																								// 4. 残差相对变化太小
																								// 5. 残差太小
																								// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																								//	  definite normal equations matrix. Typically, this indicates a programming error in the
																								//    user-supplied Jacobian.
																								// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																								//    This signals a programming error in the user-supplied projection function func.
																								// info[7]，# function evaluations，		目标函数调用次数
																								// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																								// info[9]，# number of linear systems solved，求解线性方程组的个数
														   )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = map_pointcloud.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double, 5, 1>> dists;
	vector<int> distTypes;

	// 这样做是为了保证参考图排在第一位
	cam_data & cam_ref = cams[idx_refimg];
	vIdxCams.push_back(idx_refimg);
	Ks.push_back(cam_ref.m_K);
	Rs.push_back(cam_ref.m_R);
	ts.push_back(cam_ref.m_t);
	dists.push_back(cam_ref.m_dist);
	distTypes.push_back(cam_ref.dist_type);

	for (int i = 0; i < cams.size(); ++i)
	{
		if (!cams[i].m_bOriented || i == idx_refimg)
		{
			continue;
		}

		cam_data & cam = cams[i];
		vIdxCams.push_back(i);
		Ks.push_back(cam.m_K);
		Rs.push_back(cam.m_R);
		ts.push_back(cam.m_t);
		dists.push_back(cam.m_dist);
		distTypes.push_back(cam.dist_type);
	}

	int m = Ks.size();  // 参与光束法平差的视图数目

	vector<Point3d> XYZs;
	
	// 20200607，n行m列的稀疏矩阵，用于存储各点于各图像上的可见性，其实不单单是可见性，其元素值其实为每个像点在像点坐标向量中的序号
	int sizes[] = { n, m };
	SparseMat ptrMat(2, sizes, CV_32SC1);

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	int i_tmp=0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		XYZs.push_back(iter_objpt->second.m_pt);

		auto iter_found_track = map_tracks.find(trackID);

		const SfM_ZZK::OneTrack & one_track = iter_found_track->second;

		for (int j=0;j<m;++j)
		{
			int idxcam = vIdxCams[j];

			auto iter_found_img = one_track.find(idxcam);

			if (iter_found_img == one_track.end())
			{
				// 在该 track 中没有找到指定已完成定向的观测图像
				continue;
			}

			// 找到了指定图像
			Point2d imgpt;
			imgpt.x = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.x;
			imgpt.y = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.y;
			vImgPts_vmask.push_back(imgpt);

			ptrMat.ref<int>(i_tmp, j) = vImgPts_vmask.size() - 1;
		}

		++i_tmp;
	}

	int l = vImgPts_vmask.size(); // 所有观测像点的个数

	// j_fixed 和 i_fixed 向量分别存放图像和空间点是否固定的情况
	// j_fixed[j]=1表示图 j 的参数不参与平差调整，i_fixed[i]=1 表示点 i 的坐标不参与平差调整
	// 20200607 内外参数应该有别，共有参数一般为内参数，不怎么存在参数固定不优化的情形
	// 20200607 22:30，真正跟到计算Jacobian的函数中看，就知道j_fixed只对每幅图像独有的A类参数起作用
	// 即约束Aij是否为空，图像共有参数是C类参数，对应的Jacobian是Cij。
	vector<uchar> j_fixed(m), i_fixed(n);
	j_fixed[0] = 1;

	// covInv 存放每个观测像点的不确定度（协方差矩阵）的逆矩阵，其实也就是每个观测像点的权值矩阵
	Matx22d cov;
	cov(0, 0) = cov(1, 1) = 1;
	vector<Matx22d> covInvs;
	for (int k = 0; k < l; ++k)
	{
		covInvs.push_back(cov);
	}

	double tau  = opts[0]; 
	double eps1 = opts[1];			// input:	threshold
	double eps2 = opts[2];			// input:	threshold

	vector<double> vds(l);

	SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);

	// 20200709，给出参数估计的协方差阵，也就是不确定度，或者内符合精度 /////////////////////////////////////////////////////////////////////////////////////////////
	vector<Matx<double, 6, 6>> cov_a;		// m个图像独有参数的协方差阵
	vector<Matx<double, 3, 3>> cov_b;		// n个空间点的坐标的协方差阵
	Matx<double, 1, 1> cov_c;				// 共参数的协方差阵
	vector<Matx<double, 1, 6>> cov_ca;		// 图像共参数和各图像独有参数之间的协方差阵

	derivatives::covarianceSBA_f_wj_cj_XiYiZi(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, cov_a, cov_b, cov_c, cov_ca);

	uctt_f = std::sqrt(cov_c(0));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 下面还要将优化完的参数和点坐标全部赋回去
	double sum2_sigmaOptCtr = 0;

	for (int i = 0; i < m; ++i)
	{
		int idxcam = vIdxCams[i];

		cam_data & cam = cams[idxcam];

		cam.m_K = Ks[i]; // 20200607,也更新内参数，因为等效焦距 f 也得到了优化
		cam.fx = cam.m_K(0, 0);
		cam.fy = cam.m_K(1, 1);
		cam.cx = cam.m_K(0, 2);
		cam.cy = cam.m_K(1, 2);
		cam.s = cam.m_K(0, 1);
	
		cam.m_R = Rs[i];
		cam.m_t = ts[i];

		// 20200712，把光心位置协方差矩阵取出来 ///////////////////////////////////////////
		const Matx<double, 6, 6> & cov_img = cov_a[i];

		Matx33d cov_optctr;

		for (int ii = 0; ii < 3; ++ii)
		{
			int iiplus3 = ii + 3;

			for (int jj = 0; jj < 3; ++jj)
			{
				cov_optctr(ii, jj) = cov_img(iiplus3, jj + 3);
			}
		}

		Matx31d eigenVals;
		Matx33d eigenVecs;

		//	@param eigenvalues output vector of eigenvalues of the same type as src; the eigenvalues are stored in the descending order.
		//	@param eigenvectors output matrix of eigenvectors; it has the same size and type as src;
		//	the eigenvectors are stored as subsequent matrix rows, in the same order as the corresponding eigenvalues.
		cv::eigen(cov_optctr, eigenVals, eigenVecs);
		cv::Scalar sum2 = cv::sum(eigenVals);

		sum2_sigmaOptCtr += sum2.val[0];

		// 输出光心位置的不确定度椭球，方便三维可视化
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx13d vec = std::sqrt(eigenVals(ii))*eigenVecs.row(ii);

			for (int jj = 0; jj < 3; ++jj)
			{
				cam.m_optCtrUncertEllipsoid(ii, jj) = vec(jj);
			}
		}
	}

	double rms_sigmaOptCtr = std::sqrt(sum2_sigmaOptCtr / (m - 1));

	// 用固定阈值来判断内外点
	double sigma3 = tc;
//	double sum2_sigmaObjPt = 0;

	typedef std::pair<int, double> pair_nObsv_ang;
	typedef std::pair<double, pair_nObsv_ang> pair_rltUctt_nObsv_ang;

//	vector<double> vRltUctt; // 20200715，存储每个物点的平均相对不确定度大小
	vector<pair_rltUctt_nObsv_ang> vData; // 20200715，存储每个物点的平均相对不确定度大小，及其有效被观测次数

	i_tmp = 0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		Point3d XYZ = XYZs[i_tmp];

		iter_objpt->second.m_pt = XYZ;

		// 20200712，把当前物点的协方差阵取出来 ///////////////////////////////////////
		const Matx33d & cov_objpt = cov_b[i_tmp];

		Matx31d eigenVals;
		Matx33d eigenVecs;

		cv::eigen(cov_objpt, eigenVals, eigenVecs);

		cv::Scalar sum2 = cv::sum(eigenVals);
		double sum2_all = sum2.val[0];

//		sum2_sigmaObjPt += sum2_all;

		double rms_sigma = std::sqrt(sum2_all); // sigma = sqrt(sigma1^2 + sigma2^2 + sigma3^2)

		// 输出物点位置的不确定度椭球，方便三维可视化
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx13d vec = std::sqrt(eigenVals(ii))*eigenVecs.row(ii);

			for (int jj = 0; jj < 3; ++jj)
			{
				iter_objpt->second.m_uncertaintyEllipsoid(ii, jj) = vec(jj);
			}
		}
		/////////////////////////////////////////////////////////////////////////////		

		auto iter_found_track = map_tracks.find(iter_objpt->first);

		Matx31d mXYZ;
		mXYZ(0) = XYZ.x;
		mXYZ(1) = XYZ.y;
		mXYZ(2) = XYZ.z;

		int nValid = 0; // 当前有多少个合理的观测值
		double sum_d = 0;

		vector<Matx31d> vVecObsers;

		for (int j = 0; j < m; ++j)
		{
			const int * ptr = ptrMat.find<int>(i_tmp, j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			int idx_cam = vIdxCams[j];
			double rpjerr = vds[*ptr];

			auto iter_found_img = iter_found_track->second.find(idx_cam);

			if (rpjerr < sigma3)
			{
				// 20151227，重投影残差足够小还不行，还得位于该图像正前方，否则该图像依然会被判为外点
				const Matx33d & R = Rs[j];
				const Matx31d & t = ts[j];

				Matx31d mXYZ_C = R*mXYZ + t;

				if (mXYZ_C(2) <= 0)
				{
					iter_found_img->second.second = 0; // 物点位于图像后方
				}
				else
				{
					iter_found_img->second.second = 1; // 物点位于图像前方

					// 20200716，记录下当前图像对物点的观测方向（在世界系中给出）
					Matx31d C = -R.t()*t;
					Matx31d vecObser = mXYZ - C;
					vVecObsers.push_back(vecObser);

					// 有效时，算下观测距离
					double d = cv::norm(mXYZ_C, cv::NormTypes::NORM_L2);

					sum_d += d;

					++nValid;
				}
			} 
			else
			{
				iter_found_img->second.second = 0;
			}
		}

		if (nValid >= 1) // 至少得有 1 个有效观测就可以算相对不确定度了
		{
			double rltUctt_overall = nValid*rms_sigma / sum_d;

			double maxAng = maxAngleBetween3DVecs(vVecObsers);

			vData.push_back(std::make_pair(rltUctt_overall, std::make_pair(nValid, maxAng)));

			iter_objpt->second.m_rltUctt = rltUctt_overall; // 输出
		}
		
		++i_tmp;
	}

	// 相对不确定度中值
	int nnn = vData.size();
	int idx_median = 0.5*nnn;
	std::sort(vData.begin(), vData.end(), [](const pair_rltUctt_nObsv_ang & a, const pair_rltUctt_nObsv_ang & b) {return a.first < b.first; });
	rltUctt_output = vData[idx_median].first;

// 	// 相对不确定度均值
// 	double uctt_mean = std::accumulate(vRltUcttnObsv.begin(), vRltUcttnObsv.end(), 0.0,
// 		[](const double & a, const pair_rltUctt_nObsv & b) {return a + b.first; }) / nnn;
// 
// 	// 相对不确定度的RMS
// 	double sum2_rltUctt = std::accumulate(vRltUctt.begin(), vRltUctt.end(), 0.0, [](const double & a, const double & b) {return a + b*b; });
// 
// 	double uctt_rms = std::sqrt(sum2_rltUctt / nnn);
// 
// 	// 这是用的每个物点的绝对不确定度的综合RMS，没有排除尺度的影响
// 	double rms_sigmaObjPt = std::sqrt(sum2_sigmaObjPt / n);

	return l;
}

// 20200607，采用迭代重加权最小二乘 IRLS 机制同时优化 f,XYZW,R,t
// 返回所有重投影像点个数
// 20220202，采用新数据结构 MultiTracksWithFlags
int  DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(SfM_ZZK::PointCloud & map_pointcloud,	// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
															vector<cam_data> & cams,				// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
															SfM_ZZK::MultiTracksWithFlags & map_tracks,	// 输入：所有的特征轨迹
															double & rltUctt_output,				// 输出：所有物点的综合相对不确定度水平（1倍sigma）
															double & uctt_f,						// 输出：图像共有等效焦距的不确定度（1倍sigma）
															int idx_refimg,							// input:	the reference image, whose R=I, and t =[0,0,0]'
															double tc /*= 1.5*/,					// input:	用来计算 Huber 权重的常量
															int itermax /*= 1024*/,					// 输入：最大迭代次数
															double * opts /*= NULL*/,				// 输入：总共 5 个控制参数，如果为 NULL，则采用默认参数
																									// opts[0]，\mu，							levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																									// opts[1]，||J^T e||_inf，					当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
																									// opts[2]，||dp||_2，						当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
																									// opts[3]，||e||_2，						当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
																									// opts[4]，(||e||_2-||e_new||_2)/||e||_2，	当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0
															double * info /*= NULL*/				// 输出：总共 10 个过程输出量，如果不需要输出，则置为 NULL
																									// info[0]，||e||_2 at initial p，			在初始参数下的残差值，写的误差矢量的 2 范数，其实应该是误差矢量的 2 范数的平方
																									// info[1]，||e||_2 at estimated p，		在最终输出参数下的残差值，同样应该是误差矢量的 2 范数的平方
																									// info[2]，||J^T e||_inf at estimated p，	在最终输出参数下的目标函数对各待优化参数的最大导数
																									// info[3]，||dp||_2 at estimated p，		在最终输出参数下，待优化参数 2 范数的变化量
																									// info[4]，mu/max[J^T J]_ii at estimated p，tau (mu/max(Aii))
																									// info[5]，# iterations，					总迭代次数
																									// info[6]，reason for terminating，		迭代结束原因：
																																			// 1. 目标函数对优化参数导数太小
																																			// 2. 改正量，即优化参数变化太小
																																			// 3. 达到最大迭代次数
																																			// 4. 残差相对变化太小
																																			// 5. 残差太小
																																			// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																																			//	  definite normal equations matrix. Typically, this indicates a programming error in the
																																			//    user-supplied Jacobian.
																																			// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																																			//    This signals a programming error in the user-supplied projection function func.
																									// info[7]，# function evaluations，		目标函数调用次数
																									// info[8]，# jacobian evaluations，		Jacobian 矩阵估计次数
																									// info[9]，# number of linear systems solved，求解线性方程组的个数
												  )
{
	// 首先可以肯定的是输入的标志点的空间坐标肯定都是有效的，即肯定是有 2 以上视图观测到了
	// 并同名像点之间匹配上了，最后通过多目前方交会出来的，因此参与平差的标志点个数就是数组 pts 的长度
	int n = map_pointcloud.size();

	// 而至于参与平差的视图数目就不一定了，能参与平差的视图肯定是外参数已经标定出初值的视图
	// 能通过标志点后方交会得到视图外参数初值就说明视图至少是能观测到 3 个空间坐标已知的标志点
	// 而空间坐标已知的标志点肯定都包含在 pts 中，因此不用再去判断有外参初值的视图是否能至少观测到
	// 3 个 pts 中的点了，因为有外参数初值本身就说明满足了该条件
	vector<int> vIdxCams;	// 用来存放参与光束法平差的视图在数组 cams 中的索引
	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double, 5, 1>> dists;
	vector<int> distTypes;

	// 这样做是为了保证参考图排在第一位
	cam_data & cam_ref = cams[idx_refimg];
	vIdxCams.push_back(idx_refimg);
	Ks.push_back(cam_ref.m_K);
	Rs.push_back(cam_ref.m_R);
	ts.push_back(cam_ref.m_t);
	dists.push_back(cam_ref.m_dist);
	distTypes.push_back(cam_ref.dist_type);

	for (int i = 0; i < cams.size(); ++i)
	{
		if (!cams[i].m_bOriented || i == idx_refimg)
		{
			continue;
		}

		cam_data & cam = cams[i];
		vIdxCams.push_back(i);
		Ks.push_back(cam.m_K);
		Rs.push_back(cam.m_R);
		ts.push_back(cam.m_t);
		dists.push_back(cam.m_dist);
		distTypes.push_back(cam.dist_type);
	}

	int m = Ks.size();  // 参与光束法平差的视图数目

	vector<Point3d> XYZs;

	// 20200607，n行m列的稀疏矩阵，用于存储各点于各图像上的可见性，其实不单单是可见性，其元素值其实为每个像点在像点坐标向量中的序号
	int sizes[] = { n, m };
	SparseMat ptrMat(2, sizes, CV_32SC1);

	// 按可视矩阵按行扫描扫得的各像点坐标
	vector<Point2d> vImgPts_vmask;

	int i_tmp = 0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		XYZs.push_back(iter_objpt->second.m_pt);

		auto iter_found_track = map_tracks.find(trackID);

//		const SfM_ZZK::OneTrack & one_track = iter_found_track->second;
		const SfM_ZZK::trackWithFlags & one_track = iter_found_track->second;

		for (int j = 0; j < m; ++j)
		{
			int idxcam = vIdxCams[j];

			auto iter_found_img = one_track.find(idxcam);

			if (iter_found_img == one_track.end())
			{
				// 在该 track 中没有找到指定已完成定向的观测图像
				continue;
			}

			// 找到了指定图像
			Point2d imgpt;
			imgpt.x = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.x;
			imgpt.y = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.y;
			vImgPts_vmask.push_back(imgpt);

			ptrMat.ref<int>(i_tmp, j) = vImgPts_vmask.size() - 1;
		}

		++i_tmp;
	}

	int l = vImgPts_vmask.size(); // 所有观测像点的个数

	// j_fixed 和 i_fixed 向量分别存放图像和空间点是否固定的情况
	// j_fixed[j]=1表示图 j 的参数不参与平差调整，i_fixed[i]=1 表示点 i 的坐标不参与平差调整
	// 20200607 内外参数应该有别，共有参数一般为内参数，不怎么存在参数固定不优化的情形
	// 20200607 22:30，真正跟到计算Jacobian的函数中看，就知道j_fixed只对每幅图像独有的A类参数起作用
	// 即约束Aij是否为空，图像共有参数是C类参数，对应的Jacobian是Cij。
	vector<uchar> j_fixed(m), i_fixed(n);
	j_fixed[0] = 1;

	// covInv 存放每个观测像点的不确定度（协方差矩阵）的逆矩阵，其实也就是每个观测像点的权值矩阵
	Matx22d cov;
	cov(0, 0) = cov(1, 1) = 1;
	vector<Matx22d> covInvs;
	for (int k = 0; k < l; ++k)
	{
		covInvs.push_back(cov);
	}

	double tau = opts[0];
	double eps1 = opts[1];	// input:	threshold
	double eps2 = opts[2];	// input:	threshold

	vector<double> vds(l);

	SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);

	// 20200709，给出参数估计的协方差阵，也就是不确定度，或者内符合精度 /////////////////////////////////////////////////////////////////////////////////////////////
	vector<Matx<double, 6, 6>> cov_a;		// m个图像独有参数的协方差阵
	vector<Matx<double, 3, 3>> cov_b;		// n个空间点的坐标的协方差阵
	Matx<double, 1, 1> cov_c;				// 共参数的协方差阵
	vector<Matx<double, 1, 6>> cov_ca;		// 图像共参数和各图像独有参数之间的协方差阵

	derivatives::covarianceSBA_f_wj_cj_XiYiZi(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, cov_a, cov_b, cov_c, cov_ca);

	uctt_f = std::sqrt(cov_c(0));
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 下面还要将优化完的参数和点坐标全部赋回去
	double sum2_sigmaOptCtr = 0;

	for (int i = 0; i < m; ++i)
	{
		int idxcam = vIdxCams[i];

		cam_data & cam = cams[idxcam];

		cam.m_K = Ks[i]; // 20200607,也更新内参数，因为等效焦距 f 也得到了优化
		cam.fx = cam.m_K(0, 0);
		cam.fy = cam.m_K(1, 1);
		cam.cx = cam.m_K(0, 2);
		cam.cy = cam.m_K(1, 2);
		cam.s = cam.m_K(0, 1);

		cam.m_R = Rs[i];
		cam.m_t = ts[i];

		// 20200712，把光心位置协方差矩阵取出来 ///////////////////////////////////////////
		const Matx<double, 6, 6> & cov_img = cov_a[i];

		Matx33d cov_optctr;

		for (int ii = 0; ii < 3; ++ii)
		{
			int iiplus3 = ii + 3;

			for (int jj = 0; jj < 3; ++jj)
			{
				cov_optctr(ii, jj) = cov_img(iiplus3, jj + 3);
			}
		}

		Matx31d eigenVals;
		Matx33d eigenVecs;

		//	@param eigenvalues output vector of eigenvalues of the same type as src; the eigenvalues are stored in the descending order.
		//	@param eigenvectors output matrix of eigenvectors; it has the same size and type as src;
		//	the eigenvectors are stored as subsequent matrix rows, in the same order as the corresponding eigenvalues.
		cv::eigen(cov_optctr, eigenVals, eigenVecs);
		cv::Scalar sum2 = cv::sum(eigenVals);

		sum2_sigmaOptCtr += sum2.val[0];

		// 输出光心位置的不确定度椭球，方便三维可视化
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx13d vec = std::sqrt(eigenVals(ii))*eigenVecs.row(ii);

			for (int jj = 0; jj < 3; ++jj)
			{
				cam.m_optCtrUncertEllipsoid(ii, jj) = vec(jj);
			}
		}
	}

	double rms_sigmaOptCtr = std::sqrt(sum2_sigmaOptCtr / (m - 1));

	// 用固定阈值来判断内外点
	double sigma3 = tc;
//	double sum2_sigmaObjPt = 0;

	typedef std::pair<int, double> pair_nObsv_ang;
	typedef std::pair<double, pair_nObsv_ang> pair_rltUctt_nObsv_ang;

//	vector<double> vRltUctt; // 20200715，存储每个物点的平均相对不确定度大小
	vector<pair_rltUctt_nObsv_ang> vData; // 20200715，存储每个物点的平均相对不确定度大小，及其有效被观测次数

	i_tmp = 0;
	for (auto iter_objpt = map_pointcloud.begin(); iter_objpt != map_pointcloud.end(); ++iter_objpt)
	{
		Point3d XYZ = XYZs[i_tmp];

		iter_objpt->second.m_pt = XYZ;

		// 20200712，把当前物点的协方差阵取出来 ///////////////////////////////////////
		const Matx33d & cov_objpt = cov_b[i_tmp];

		Matx31d eigenVals;
		Matx33d eigenVecs;

		cv::eigen(cov_objpt, eigenVals, eigenVecs);

		cv::Scalar sum2 = cv::sum(eigenVals);
		double sum2_all = sum2.val[0];

//		sum2_sigmaObjPt += sum2_all;

		double rms_sigma = std::sqrt(sum2_all); // sigma = sqrt(sigma1^2 + sigma2^2 + sigma3^2)

		// 输出物点位置的不确定度椭球，方便三维可视化
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx13d vec = std::sqrt(eigenVals(ii))*eigenVecs.row(ii);

			for (int jj = 0; jj < 3; ++jj)
			{
				iter_objpt->second.m_uncertaintyEllipsoid(ii, jj) = vec(jj);
			}
		}
		/////////////////////////////////////////////////////////////////////////////		

		auto iter_found_track = map_tracks.find(iter_objpt->first);

		Matx31d mXYZ;
		mXYZ(0) = XYZ.x;
		mXYZ(1) = XYZ.y;
		mXYZ(2) = XYZ.z;

		int nValid = 0; // 当前有多少个合理的观测值
		double sum_d = 0;

		vector<Matx31d> vVecObsers;

		for (int j = 0; j < m; ++j)
		{
			const int * ptr = ptrMat.find<int>(i_tmp, j);

			if (NULL == ptr)
			{
				// 如果 ptr == NULL，说明像点 xij 不存在
				continue;
			}

			int idx_cam = vIdxCams[j];
			double rpjerr = vds[*ptr];

			auto iter_found_img = iter_found_track->second.find(idx_cam);

			if (rpjerr < sigma3)
			{
				// 20151227，重投影残差足够小还不行，还得位于该图像正前方，否则该图像依然会被判为外点
				const Matx33d & R = Rs[j];
				const Matx31d & t = ts[j];

				Matx31d mXYZ_C = R*mXYZ + t;

				if (mXYZ_C(2) <= 0)
				{
					iter_found_img->second.second[0] = 0; // 物点位于图像后方
				}
				else
				{
					iter_found_img->second.second[0] = 1; // 物点位于图像前方

					// 20200716，记录下当前图像对物点的观测方向（在世界系中给出）
					Matx31d C = -R.t()*t;
					Matx31d vecObser = mXYZ - C;
					vVecObsers.push_back(vecObser);

					// 有效时，算下观测距离
					double d = cv::norm(mXYZ_C, cv::NormTypes::NORM_L2);

					sum_d += d;

					++nValid;
				}
			}
			else
			{
				iter_found_img->second.second[0] = 0;
			}
		}

		if (nValid >= 1) // 至少得有 1 个有效观测就可以算相对不确定度了
		{
			double rltUctt_overall = nValid*rms_sigma / sum_d;

			double maxAng = maxAngleBetween3DVecs(vVecObsers);

			vData.push_back(std::make_pair(rltUctt_overall, std::make_pair(nValid, maxAng)));

			iter_objpt->second.m_rltUctt = rltUctt_overall; // 输出
		}

		++i_tmp;
	}

	// 相对不确定度中值
	int nnn = vData.size();
	int idx_median = 0.5*nnn;
	std::sort(vData.begin(), vData.end(), [](const pair_rltUctt_nObsv_ang & a, const pair_rltUctt_nObsv_ang & b) {return a.first < b.first; });
	rltUctt_output = vData[idx_median].first;

// 	// 相对不确定度均值
// 	double uctt_mean = std::accumulate(vRltUcttnObsv.begin(), vRltUcttnObsv.end(), 0.0,
// 		[](const double & a, const pair_rltUctt_nObsv & b) {return a + b.first; }) / nnn;
// 
// 	// 相对不确定度的RMS
// 	double sum2_rltUctt = std::accumulate(vRltUctt.begin(), vRltUctt.end(), 0.0, [](const double & a, const double & b) {return a + b*b; });
// 
// 	double uctt_rms = std::sqrt(sum2_rltUctt / nnn);
// 
// 	// 这是用的每个物点的绝对不确定度的综合RMS，没有排除尺度的影响
// 	double rms_sigmaObjPt = std::sqrt(sum2_sigmaObjPt / n);

	return l;
}

// 20220209，优化像点匹配精度
void DeepVoid::RefineMatchingAccuracy(const SfM_ZZK::PointCloud & map_pointcloud,	// 输入：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
									  SfM_ZZK::MultiTracksWithFlags & map_tracks,	// 输入兼输出：所有的特征轨迹
									  vector<cam_data> & cams,						// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
									  const vector<Mat> & imgs,						// 输入：所有图像
									  int wndSizeMin /*= 5*/,						// 输入：所允许的最小参考图像窗口尺寸
									  int maxIter /*= 128*/,						// input: max iteration
									  double xEps /*= 1.0E-8*/,						// input: threshold
									  double fEps /*= 1.0E-6*/						// input: threshold
									  )
{
	typedef std::pair<int, std::pair<int, Matx31d>> pair_I_i_C; // 20220203，先定义一个下面要用到的数据结构，<I，i，C> 存储当前像点的图像索引号、像点索引号、图像光心坐标

	CString strInfo;

	int k = 0;
	int nObjPts = map_pointcloud.size();

	int rateOld = -100;

	for (auto iterObjPt = map_pointcloud.begin(); iterObjPt != map_pointcloud.end(); ++iterObjPt)
	{
		++k;

		int rate = int((k / (double)nObjPts) * 100);

		if (rate % 10 == 0)
		{
			if (rate != rateOld)
			{
				strInfo.Format("%d%% completed", rate);

				theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

				rateOld = rate;
			}
		}

		const int & ID = iterObjPt->first; // 先取出点的全局 ID 号

		const DeepVoid::CloudPoint & objpt = iterObjPt->second;

		Matx31d X;
		X(0) = objpt.m_pt.x;
		X(1) = objpt.m_pt.y;
		X(2) = objpt.m_pt.z;

		auto track = map_tracks.find(ID); // 再把该点对应的特征轨迹 track 给取出来

		std::vector<pair_I_i_C> pIiCs;

		// 这个循环就是把相关的特征信息给取出来
		for (auto iterImgPt = track->second.begin(); iterImgPt != track->second.end(); ++iterImgPt)
		{
			const int & I = iterImgPt->first;					// 取出特征轨迹中每个像点的图像索引号
			const int & i = iterImgPt->second.first;			// 取出特征轨迹中每个像点于其图像中的像点索引号
			const int & bInlier = iterImgPt->second.second[0];	// 该像点当前是否被判定为内点

			if (!bInlier)	// 如果不是内点就直接 next 了
			{
				continue;
			}

			const cam_data & cam = cams[I];

			Matx31d C = -cam.m_R.t()*cam.m_t; // 取出像点所在图像的光心坐标

			pIiCs.push_back(make_pair(I, make_pair(i, C)));
		}

		// 20220204，虽然我直觉感觉一个物点既然都已经被交会出三维坐标了，那肯定会有至少 1 个内点吧（按说是2个吧，在BA过程中可能有那么1个点残差过大被判定为外点）
		// 但我确实遇到过所有像点都被判定为 outliers 的情况，尤其是这段代码在 final SBA 之前时
		// 按说在 Triangulation_AddOneImg() 之后我应该是把每个可能的物点三维坐标对应的支撑内点集的标志位都置为 1 才对吧？
		// 我进 Triangulation_AddOneImg() 中看了，确实没有刻意置标志位为 1 的操作，也是有它道理的，因为判断内点的工作完全应由 SBA 环节来裁定
		// 其它任何环节也就是提名坐标初值的作用，因此每次 Triangulation_AddOneImg() 之后就应该来一次 SBA 才行，否则很多点的内点标志位是不可信的
		// 我把 image matching refinement 环节挪到 final SBA 之后是完全正确的。
		if (pIiCs.empty())
		{
			continue;
		}

		int nValid = pIiCs.size(); // 到这里 nValid 是有可能为 1 的

		std::vector<std::pair<int, double>> vsumd2;

		for (int i = 0; i < nValid; ++i)
		{
			double sum2 = 0;

			for (int j = 0; j < nValid; ++j)
			{
				if (i == j)
				{
					continue;
				}

				Matx31d dC = pIiCs[i].second.second - pIiCs[j].second.second; // 光心之间的距离

				double d = norm(dC);

				sum2 += d*d;
			}

			vsumd2.push_back(make_pair(i, sum2));
		}

		sort(vsumd2.begin(), vsumd2.end(),
			[](const std::pair<int, double> & a, const std::pair<int, double> & b) {return a.second < b.second; });

		// 把参考像点所在图像索引号和像点索引号取出来
		const int & I_ref = pIiCs[vsumd2[0].first].first;
		const int & i_ref = pIiCs[vsumd2[0].first].second.first;

		auto iterImgPt = track->second.find(I_ref);
		iterImgPt->second.second[1] = 1; // 将参考像点的相关标志位置 1 表明其身份为像点匹配优化环节中的参考像点

		// 20220212，临时加的
		// 相当于只为每个物点指定 1 个参考像点
		// 不再更新参考像点坐标以及任何匹配像点坐标
//		continue;

		cam_data & cam0 = cams[I_ref];
		const Matx33d & mK0 = cam0.m_K;
		const Matx33d & mR0 = cam0.m_R;
		const Matx31d & mt0 = cam0.m_t;
		const Mat & img0 = imgs[I_ref];
		cv::KeyPoint & feat0 = cam0.m_feats.key_points[i_ref];
		Point2f & pt0 = feat0.pt;
		int x0 = FTOI(pt0.x);	// 但凡参考像点，都取成整像素
		int y0 = FTOI(pt0.y);

		int wndSize = int(feat0.size*0.5) * 2 + 1; // 20220209，实际窗口大小按照特征大小来定，确保该值为奇数

		if (wndSize < wndSizeMin)
		{
			wndSize = wndSizeMin;
		}

		Matx31d C0 = -mR0.t()*mt0;

		Matx31d X_C = mR0*X + mt0; // 物点在参考图像中的坐标
		double d_ref = X_C(2); // 物点相对于参考图像的深度值

		int hWndSize = (wndSize - 1)*0.5; // half patch width

		int x0_real, y0_real;
		MakeSureNotOutBorder(x0, y0, x0_real, y0_real, hWndSize, img0.cols, img0.rows);	// 确保参考像点不越界

		pt0.x = x0_real;	// 更新参考像点坐标
		pt0.y = y0_real;

		Matx31d xy1;
		xy1(0) = x0_real;
		xy1(1) = y0_real;
		xy1(2) = 1;

		Matx31d Rtuv1 = mR0.t()*mK0.inv()*xy1;

		Mat mMask(wndSize, wndSize, CV_8UC1, Scalar(1));

		std::vector<Matx33d> vKi, vRi;
		std::vector<Matx31d> vti;
		std::vector<Mat> vImgi;
		std::vector<Mat> vMaski;
		std::vector<int> vNumi;

		// 从排行第 2 个开始就是非参考匹配像点了
		for (int i = 1; i < nValid; ++i)
		{
			const int & I_other = pIiCs[vsumd2[i].first].first;
			const cam_data & cami = cams[I_other];		

			vKi.push_back(cami.m_K);
			vRi.push_back(cami.m_R);
			vti.push_back(cami.m_t);
			vImgi.push_back(imgs[I_other]);
			vMaski.push_back(mMask);
			vNumi.push_back(wndSize*wndSize);
		}

		// 20220209，所有非参考像点一起来进行优化
		double d_init = d_ref;
		double hx_init = 0;
		double hy_init = 0;
		double score_init = 0;
		double d_optim, hx_optim, hy_optim, score_optim;

		bool bSucRefine = optim_gn_drhxhyck_NCCcontrolled_masks(mK0, mR0, mt0, img0, vKi, vRi, vti, vImgi, vMaski, vNumi, x0_real, y0_real, wndSize, wndSize,
			d_init, hx_init, hy_init, score_init, d_optim, hx_optim, hy_optim, score_optim, maxIter, xEps, fEps);

		// 成功的话就更新物点坐标和全部匹配像点坐标
		if (bSucRefine)
		{
			// 先更新物点坐标
			Matx31d X_optim = C0 + d_optim*Rtuv1;

			// 20220209，晚23:05，最终还是决定不更新物点坐标，而只更新匹配像点坐标
			// 尽管更新完物点坐标后，所有点重投影残差立马降到0.05-0.07个像素这么恐怖的程度（SBA经12次迭代后就收敛退出了，残差维持那么多）
			// 但我也发现，更新完物点坐标后的物点平均被观测次数反而降低了一丁点，不确定度也会降低一丁点，
			// 甚至于不知道是不是心理作用，我看着显示的三维点云好像也不及没更新物点坐标时的结果。。。
			// 所以虽然不更新物点坐标仅更新匹配像点坐标时收敛的残差普遍在0.12-0.16个像素的水平，我还是决定不更新物点坐标了，就靠接着的SBA去优化，反正精度肯定是完全够的
// 			objpt.m_pt.x = X_optim(0);
// 			objpt.m_pt.y = X_optim(1);
// 			objpt.m_pt.z = X_optim(2);

			for (int i = 1; i < nValid; ++i)
			{
				const int & I_other = pIiCs[vsumd2[i].first].first;
				const int & i_other = pIiCs[vsumd2[i].first].second.first;

				cam_data & cami = cams[I_other];
				Point2f & pti = cami.m_feats.key_points[i_other].pt;

				Matx31d xyi = cami.m_K*(cami.m_R*X_optim + cami.m_t);

				double x_new = xyi(0) / xyi(2);
				double y_new = xyi(1) / xyi(2);

				double dx = pti.x - x_new;
				double dy = pti.y - y_new;

				pti.x = x_new;
				pti.y = y_new;
			}
		}
	}
}

// iteratively run sba multiple times, until no outliers are detected
/*int DeepVoid::optim_sba_levmar_XYZ_ext_rotvec_iterative(vector<CloudPoint> & cloudPts,		// 输入兼输出：存放所有标志点的空间坐标，平差之后里面的点坐标将被更新
														vector<cam_data> & cams,			// 输入兼输出：存放所有视图的信息，其中包括视图的内参数，外参数，像差系数以及所观测到的标志点像点坐标，平差之后里面能优化的视图外参数将得到更新
														const vector<vector<Point2i>> & allTracks,
														int idx_refimg,					// input:	the reference image, whose R=I, t=[0,0,0]'
														int itermax,				// 输入：最大迭代次数
														double thresh_rpjErr,			// input:	the threshold to determine outliers
														double * opts,				
														double * info
														)
{
	Matx33d mK, mR, mKR;
	Matx31d mt, mKt, mWrdPt, mImgPt;

	mK(2,2) = 1;
	mK(1,0) = mK(2,0) = mK(2,1) = 0;

	int i,j,k,l;
	int nImgPts;

	vector<int> vTrackIdx(1);

	while (vTrackIdx.size()!=0)
	{
		nImgPts = optim_sba_levmar_XYZ_ext_rotvec(cloudPts, cams, idx_refimg, itermax, opts, info);

		vTrackIdx.clear();

		// check every cloud point to see if the reprojection error on some image is too big
		for (i=0;i<cloudPts.size();i++)
		{
			CloudPoint cloudpt = cloudPts[i];

			mWrdPt(0) = cloudpt.m_pt.x;
			mWrdPt(1) = cloudpt.m_pt.y;
			mWrdPt(2) = cloudpt.m_pt.z;

			int nImgs = cloudpt.m_vImgInfos.size();

			// reproject
			for (j=0;j<nImgs;j++)
			{
				int idx_img = cloudpt.m_vImgInfos[j].m_idxImg;
				int idx_imgpt = cloudpt.m_vImgInfos[j].m_idxImgPt;

				cam_data cam = cams[idx_img];

				// observations
				double imgpt_x = cam.m_feats.key_points[idx_imgpt].pt.x;
				double imgpt_y = cam.m_feats.key_points[idx_imgpt].pt.y;

				for (k=0;k<3;k++)
				{
					for (l=0;l<3;l++)
					{
						mR(k,l)=cam.R[k*3+l];
					}
				}

				mt(0) = cam.t[0]; mt(1) = cam.t[1]; mt(2) = cam.t[2];

				mK(0,0) = cam.fx; mK(1,1) = cam.fy;
				mK(0,2) = cam.cx; mK(1,2) = cam.cy;
				mK(0,1) = cam.s;

				mKR = mK*mR;
				mKt = mK*mt;

				mImgPt = mKR * mWrdPt + mKt;

				// linear reprojection
				double imgpt_rpj_linear_x = mImgPt(0)/mImgPt(2);
				double imgpt_rpj_linear_y = mImgPt(1)/mImgPt(2);

				double imgpt_rpj_dist_x, imgpt_rpj_dist_y;

				CalcuDistedImgPt_DCBrown(cam.cx, cam.cy, cam.fx, cam.fy, cam.k,
					imgpt_rpj_linear_x, imgpt_rpj_linear_y, imgpt_rpj_dist_x, imgpt_rpj_dist_y);

				double dx = imgpt_rpj_dist_x - imgpt_x;
				double dy = imgpt_rpj_dist_y - imgpt_y;
				double d = sqrt(dx*dx+dy*dy);

				if (d>thresh_rpjErr)
				{
					vTrackIdx.push_back(cloudpt.m_idx);
					break;
				}
			}
		}

		// delete cloud point and detach features
		for (i=0;i<vTrackIdx.size();i++)
		{
			// get the track index of this outlier
			int idx_track = vTrackIdx[i];

			// first delete the object point from clouds
			vector <CloudPoint>::iterator iter;
			for (iter = cloudPts.begin(); iter != cloudPts.end(); iter++)
			{
				if (iter->m_idx == idx_track)
				{
					cloudPts.erase(iter);
					break;
				}
			}

			// then detach all the features from this track
			vector<Point2i> oneTrack = allTracks[idx_track];
			for (j=0;j<oneTrack.size();j++)
			{
				Point2i indices = oneTrack[j];
				cams[indices.x].m_feats.tracks[indices.y] = -1;
			}
		}
	}

	return nImgPts;
}*/