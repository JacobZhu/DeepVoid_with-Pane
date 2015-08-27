#include "stdafx.h"

// 解读优化函数输出的有关优化过程的各信息量
CString DeepVoid::InterpretOptimInfo(double * info,				// 10 维的向量，用来输出优化过程的相关信息
									 const CMatrix & mCovar		// 各优化参数估计的协方差矩阵
									 )
{
	CString str1;
	str1.Format("Residue before:		%e\nResidue after:		%e\nMaximal gradient:	%e\nUpdate vector norm:	%e\ntau (mu/max(Aii)):	%e\nIterations:		%d\n",
		info[0], info[1], info[2], info[3], info[4], int(info[5]));

	CString str2;

	if (fabs(info[6] - 1) < 1.0E-6)
	{
		str2 = "Reason for terminating:	Stopped by small gradient";
	}
	else if (fabs(info[6] - 2) < 1.0E-6)
	{
		str2 = "Reason for terminating:	Stopped by small update vector";
	}
	else if (fabs(info[6] - 3) < 1.0E-6)
	{
		str2 = "Reason for terminating:	Stopped by max iterations";
	}
	else if (fabs(info[6] - 4) < 1.0E-6)
	{
		str2 = "Reason for terminating:	Singular matrix. Restart from current p with increased mu";
	}
	else if (fabs(info[6] - 5) < 1.0E-6)
	{
		str2 = "Reason for terminating:	No further error reduction is possible. Restart with increased mu";
	}
	else if (fabs(info[6] - 6) < 1.0E-6)
	{
		str2 = "Reason for terminating:	Stopped by small residue";
	}
	else if (fabs(info[6] - 7) < 1.0E-6)
	{
		str2 = "Reason for terminating:	Stopped by invalid (i.e. NaN or Inf) func values, a user error";
	}
	else
	{

	}

	CString str3;
	str3.Format("Function evaluations:	%d\nJacobian evaluations:	%d\nLinear systems solved:	%d\n", int(info[7]), int(info[8]), int(info[9]));

	// 如果传进来的参数估计协方差矩阵为 Null 则直接只返回以下字符串
	if (mCovar.IsNull())
	{
		return str1 + str3 + str2;
	}
	
	CString str4;	// 该字符串录入各估计参数的标准差 sigma
	str4.Format("\n");
	int m = mCovar.m_nCol;
	int i;
	for (i = 0; i < m; i++)
	{
		CString strTmp;
		strTmp.Format("\nSTDEV of parameter %d:	%lf", i+1, sqrt(mCovar(i+1, i+1)));
		str4 += strTmp;
	}

	return str1 + str3 + str2 + str4;
}

// 该函数在优化第 2 幅视图相对于第 1 幅视图的外参数以及空间点在第 1 幅视图坐标系中的坐标时用来计算所有空间点在两幅视图中的所有重投影像点坐标
void DeepVoid::proj_relative_XYZ_ext_euler(double * p,			// 待优化的参数，为 m 维的，在该函数中待优化的是 2 号视图相对于 1 号视图的 3 个欧拉角和 3 个平移向量，以及所有 a 个点的 3 维空间坐标
										   double * hx,			// 根据待优化参数形成的重投影观测量，为 n 维的向量
										   int m,				// 待优化参数的维数，要是有 a 个空间点，则 m = 3*a+6
										   int n,				// 观测向量的维数，要是有 a 个空间点，则总共的观测向量维数是 n = 4*a
										   void * adata			// 传进来的已有信息，针对该重投影函数来说的话，需要传进来的信息主要有像机的内参数，以及像差系数
										   )
{
	vector<cam_data> * pdata;
	pdata =(vector<cam_data> *)adata;
	// 把两个视图的参数结构分别取出来
	cam_data cam1 = (*pdata)[0];
	cam_data cam2 = (*pdata)[1];

	// 空间同名标志点个数
	int n_pts = (m - 6) / 3;


	/*-----------------------------------------------每个函数会不同的部分-------------------------------------------------------------*/
	// 第 2 个视图 -------------------------------------------------------------------------------------------------------------------
	double yaw = p[0];
	double pit = p[1];
	double rol = p[2];
	double tx  = p[3];
	double ty  = p[4];
	double tz  = p[5];

	CMatrix mRT2 = GenT(tx, ty, tz) * GenR_Euler(rol, pit, yaw, ZYX);

	CMatrix mK2(3, 4, 0);

	mK2(1, 1) = cam2.fx;		mK2(1, 2) = cam2.s;			mK2(1, 3) = cam2.cx;		mK2(1, 4) = 0;	
	mK2(2, 1) = 0;				mK2(2, 2) = cam2.fy;		mK2(2, 3) = cam2.cy;		mK2(2, 4) = 0;
	mK2(3, 1) = 0;				mK2(3, 2) = 0;				mK2(3, 3) = 1;				mK2(3, 4) = 0;

	CMatrix mDist2(5, 1);
	mDist2(1) = cam2.k[0];
	mDist2(2) = cam2.k[1];
	mDist2(3) = cam2.k[2];
	mDist2(4) = cam2.k[3];
	mDist2(5) = cam2.k[4];

	// 第 1 个视图 -------------------------------------------------------------------------------------------------------------------
	CMatrix mRT1 = GenT(0, 0, 0) * GenR_Euler(0, 0, 0, ZYX);

	CMatrix mK1(3, 4, 0);

	mK1(1, 1) = cam1.fx;		mK1(1, 2) = cam1.s;			mK1(1, 3) = cam1.cx;		mK1(1, 4) = 0;	
	mK1(2, 1) = 0;				mK1(2, 2) = cam1.fy;		mK1(2, 3) = cam1.cy;		mK1(2, 4) = 0;
	mK1(3, 1) = 0;				mK1(3, 2) = 0;				mK1(3, 3) = 1;				mK1(3, 4) = 0;

	CMatrix mDist1(5, 1);
	mDist1(1) = cam1.k[0];
	mDist1(2) = cam1.k[1];
	mDist1(3) = cam1.k[2];
	mDist1(4) = cam1.k[3];
	mDist1(5) = cam1.k[4];
	/*--------------------------------------------------------------------------------------------------------------------------------*/

	CMatrix mWrdPts(4, n_pts, 1);

	int i;
	for (i = 0; i < n_pts; i++)
	{
		mWrdPts(1, i+1) = p[i*3+6];
		mWrdPts(2, i+1) = p[i*3+7];
		mWrdPts(3, i+1) = p[i*3+8];
	}

	// 在 1 号视图上做重投影
	CMatrix mImgPts1_reproj = Proj2IdealImgPt_3Dto2D(mWrdPts, mK1 * mRT1);

	if (0 == cam1.dist_type)
	{
		mImgPts1_reproj = CalcuDistedImgPt_Weng(mImgPts1_reproj, mK1, mDist1); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else if (1 == cam1.dist_type)
	{
		mImgPts1_reproj = CalcuDistedImgPt_DCBrown(mImgPts1_reproj, mK1, mDist1); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else
	{
		mImgPts1_reproj = CalcuDistedImgPt_DCBrown(mImgPts1_reproj, mK1, mDist1); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}

	// 在 2 号视图上做重投影
	CMatrix mImgPts2_reproj = Proj2IdealImgPt_3Dto2D(mWrdPts, mK2 * mRT2);

	if (0 == cam2.dist_type)
	{
		mImgPts2_reproj = CalcuDistedImgPt_Weng(mImgPts2_reproj, mK2, mDist2); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else if (1 == cam2.dist_type)
	{
		mImgPts2_reproj = CalcuDistedImgPt_DCBrown(mImgPts2_reproj, mK2, mDist2); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else
	{
		mImgPts2_reproj = CalcuDistedImgPt_DCBrown(mImgPts2_reproj, mK2, mDist2); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}

	for (i = 0; i < n_pts; i++)
	{
		hx[4*i+0] = mImgPts1_reproj(1, i+1);
		hx[4*i+1] = mImgPts1_reproj(2, i+1);
		hx[4*i+2] = mImgPts2_reproj(1, i+1);
		hx[4*i+3] = mImgPts2_reproj(2, i+1);
	}
}

// 该函数在优化用欧拉角和平移向量表示的视图外参数时用来计算各空间点在视图上的重投影像点坐标，以供 dlevmar_dif 函数调用
void DeepVoid::proj_ext_rotvec(double * p,			// 待优化的参数，为 m 维的，在该函数中待优化的是 3 个旋转向量和 3 个平移向量，因此 m = 6
							   double * hx,			// 根据待优化参数形成的重投影观测量，为 n 维的向量
							   int m,				// 待优化参数的维数
							   int n,				// 观测向量的维数
							   void * adata			// 传进来的已有信息，针对该重投影函数来说的话，需要传进来的信息主要有像机的内参数，以及各空间三维点的坐标
							   )
{
	levmar_calib_data * pdata;
	pdata =(levmar_calib_data *)adata;

	int n_pts = pdata->objPts.size();

	cam_data cam = pdata->camData;

	double rv1 = p[0];
	double rv2 = p[1];
	double rv3 = p[2];
	double tx  = p[3];
	double ty  = p[4];
	double tz  = p[5];

	CMatrix mRT = GenT(tx, ty, tz) * GenR_Rodrigues(rv1, rv2, rv3);

	CMatrix mK(3, 4, 0);

	mK(1, 1) = cam.fx;	mK(1, 2) = cam.s;	mK(1, 3) = cam.cx;	mK(1, 4) = 0;	
	mK(2, 1) = 0;		mK(2, 2) = cam.fy;	mK(2, 3) = cam.cy;	mK(2, 4) = 0;
	mK(3, 1) = 0;		mK(3, 2) = 0;		mK(3, 3) = 1;		mK(3, 4) = 0;

	CMatrix mWrdPts(4, n_pts, 1);

	int i;
	for (i = 0; i < n_pts; i++)
	{
		mWrdPts(1, i+1) = pdata->objPts[i].x;
		mWrdPts(2, i+1) = pdata->objPts[i].y;
		mWrdPts(3, i+1) = pdata->objPts[i].z;
	}

	CMatrix mImgPts_reproj = Proj2IdealImgPt_3Dto2D(mWrdPts, mK * mRT);

	CMatrix mDist(5, 1);
	mDist(1) = cam.k[0];
	mDist(2) = cam.k[1];
	mDist(3) = cam.k[2];
	mDist(4) = cam.k[3];
	mDist(5) = cam.k[4];

	if (0 == cam.dist_type)
	{
		mImgPts_reproj = CalcuDistedImgPt_Weng(mImgPts_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else if (1 == cam.dist_type)
	{
		mImgPts_reproj = CalcuDistedImgPt_DCBrown(mImgPts_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}
	else
	{
		mImgPts_reproj = CalcuDistedImgPt_DCBrown(mImgPts_reproj, mK, mDist); // 这一步之后mImgPt_reproj存放的是带像差的重投影像点坐标
	}

	for (i = 0; i < n_pts; i++)
	{
		hx[2*i]   = mImgPts_reproj(1, i+1);
		hx[2*i+1] = mImgPts_reproj(2, i+1);
	}
}

void DeepVoid::f_PIRO_Y_Z_AXAYAZ(double * p,		// Y,Z,AX,AY,AZ
								 double * hx,		// f=det(b,r1,r2)
								 int m,				// dimension of parameters, m=5
								 int n,				// dimension of observation equations i.e. number of f
								 void * adata		// containing distortion-free normalized image points i.e. CMatrix mImgPts1 and CMatrix mImgPts2
								 )
{
	int i;

	levmar_mImgPts1_mImgPts2_bx * pdata;
	pdata =(levmar_mImgPts1_mImgPts2_bx *)adata;

	double Y  = p[0];
	double Z  = p[1];
	double AX = p[2];
	double AY = p[3];
	double AZ = p[4];
	
	CMatrix mR = GenR_Euler_Radian(AX, AY, AZ, ZYX);
	mR = mR.GetRect(1,1,3,3);
	
	CMatrix mImgPts2_in1 = mR.Transpose()*pdata->mImgPts2; // should be 3*n

	CMatrix mF(3, 3);
	mF(1,1)= pdata->bx;
	mF(2,1)= Y;
	mF(3,1)= Z;
	mF(3,2)=1;
	
	for (i = 0; i < n; i++)
	{
		// compute f
		mF(1,2)=pdata->mImgPts1(1,i+1);	mF(1,3)= mImgPts2_in1(1,i+1);
		mF(2,2)=pdata->mImgPts1(2,i+1);	mF(2,3)= mImgPts2_in1(2,i+1);
										mF(3,3)= mImgPts2_in1(3,i+1);
		hx[i] = mF.Det();
	}
}

void DeepVoid::j_PIRO_Y_Z_AXAYAZ(double * p,			// Y,Z,AX,AY,AZ
								 double * j,			// f=det(b,r1,r2)
								 int m,					// dimension of parameters
								 int n,					// dimension of observation equations i.e. number of f
								 void * adata			// containing distortion-free normalized image points i.e. CMatrix mImgPts1 and CMatrix mImgPts2
								 )
{
	int i;

	levmar_mImgPts1_mImgPts2_bx * pdata;
	pdata =(levmar_mImgPts1_mImgPts2_bx *)adata;

	double Y  = p[0];
	double Z  = p[1];
	double AX = p[2];
	double AY = p[3];
	double AZ = p[4];

	double df_dY, df_dZ, df_dAX, df_dAY, df_dAZ, f, df_dnx1, df_dny1, df_dnx2, df_dny2;

	for (i = 0; i < n; i++)
	{
		der_f_PIRO_Y_Z(pdata->mImgPts1(1,i+1), pdata->mImgPts1(2,i+1), pdata->mImgPts2(1,i+1), pdata->mImgPts2(2,i+1),
		Y, Z, AX, AY, AZ, df_dY, df_dZ, df_dAX, df_dAY, df_dAZ, f, df_dnx1, df_dny1, df_dnx2, df_dny2, pdata->bx);

		j[5*i+0] = df_dY;
		j[5*i+1] = df_dZ;
		j[5*i+2] = df_dAX;
		j[5*i+3] = df_dAY;
		j[5*i+4] = df_dAZ;
	}
}

void DeepVoid::j_ext_rotvec(double * p,			// rv1, rv2, rv3 and tx, ty, tz
							double * j,			// jacobian
							int m,				// dimension of parameters
							int n,				// dimension of observation equations
							void * adata		// 
							)
{
	int ii,jj,kk;

	levmar_calib_data * pdata;
	pdata =(levmar_calib_data *)adata;

	int n_pts = pdata->objPts.size();

	cam_data cam = pdata->camData;

	double rv1 = p[0];
	double rv2 = p[1];
	double rv3 = p[2];
	double tx  = p[3];
	double ty  = p[4];
	double tz  = p[5];

	CMatrix mRotVec(3,1);
	mRotVec(1) = rv1;
	mRotVec(2) = rv2;
	mRotVec(3) = rv3;

	CMatrix mRT = GenT(tx, ty, tz) * GenR_Rodrigues(mRotVec);

	CMatrix mK(3, 4, 0);

	mK(1, 1) = cam.fx;	mK(1, 2) = cam.s;	mK(1, 3) = cam.cx;	mK(1, 4) = 0;	
	mK(2, 1) = 0;		mK(2, 2) = cam.fy;	mK(2, 3) = cam.cy;	mK(2, 4) = 0;
	mK(3, 1) = 0;		mK(3, 2) = 0;		mK(3, 3) = 1;		mK(3, 4) = 0;

	CMatrix mDist(5, 1);
	mDist(1) = cam.k[0];mDist(2) = cam.k[1];mDist(3) = cam.k[2];mDist(4) = cam.k[3];mDist(5) = cam.k[4];

	CMatrix mWrdPt(4, 1, 1), mImgPt_reproj, mJacob_xy_RT, mJacob_xy_ExtPara, mJacob_distxy_xy, mJacob_distxy_ExtPara;
	CMatrix mJacob_RT_ExtPara = Jacobian_RT_ExtPara_RotVec(mRotVec); // 12*6 matrix

	for (ii=0;ii<n_pts;ii++)
	{
		mWrdPt(1) = pdata->objPts[ii].x;
		mWrdPt(2) = pdata->objPts[ii].y;
		mWrdPt(3) = pdata->objPts[ii].z;

		mImgPt_reproj = mK * mRT * mWrdPt;
		double Z_inv = 1 / mImgPt_reproj(3);
		mImgPt_reproj(1) = mImgPt_reproj(1) * Z_inv;
		mImgPt_reproj(2) = mImgPt_reproj(2) * Z_inv;
		mImgPt_reproj(3) = mImgPt_reproj(3) * Z_inv;

		mJacob_xy_RT = Jacobian_xy_RT(mRT, mK, mWrdPt); // 2*12 matrix

		mJacob_xy_ExtPara = mJacob_xy_RT * mJacob_RT_ExtPara; // 2*6 matrix

		if (!cam.dist_type)
		{
			mJacob_distxy_xy = Jacobian_distxy_xy_Weng(mImgPt_reproj, mK, mDist); // weng's 像差系数
		} 
		else
		{
			mJacob_distxy_xy = Jacobian_distxy_xy_DCBrown(mImgPt_reproj(1), mImgPt_reproj(2), mK, mDist); // D.C.Brown's 像差系数
		}

		mJacob_distxy_ExtPara = mJacob_distxy_xy * mJacob_xy_ExtPara; // 2*6 matrix

		for (jj=0;jj<2;jj++)
		{
			for (kk=0;kk<6;kk++)
			{
				j[12*ii+jj*6+kk] = mJacob_distxy_ExtPara(jj+1,kk+1);
			}
		}
	}
}

// 优化第 2 幅视图相对于第 1 幅视图的外参数以及空间点在第 1 幅视图坐标系中的坐标
void DeepVoid::optim_levmar_relative_XYZ_ext_euler(CMatrix & mWrdPts,					// 输入兼输出：4×n的矩阵，控制点的三维坐标 
												   const CMatrix & mImgPts1,			// 输入：3×n或2×n的矩阵，空间点在 1 号视图中的图像坐标
												   const CMatrix & mImgPts2,			// 输入：3×n或2×n的矩阵，空间点在 2 号视图中的图像坐标
												   const CMatrix & mK1,					// 输入：3×3或3×4的矩阵，1 号视图的内参数矩阵
												   const CMatrix & mK2,					// 输入：3×3或3×4的矩阵，2 号视图的内参数矩阵
												   CMatrix & mRT,						// 输入兼输出：4×4的矩阵，2 号视图相对于 1 号视图的初始外参数矩阵
												   const CMatrix & mDist1,				// 输入：5×1的 1 号视图的像差系数
												   const CMatrix & mDist2,				// 输入：5×1的 2 号视图的像差系数
												   int   distType1,						// 输入：1 号视图的像差类型，选择是 Weng's (0) 还是 D.C.Brown's (1)，都是 5 个像差系数
												   int   distType2,						// 输入：2 号视图的像差类型，选择是 Weng's (0) 还是 D.C.Brown's (1)，都是 5 个像差系数
												   int   maxIter,						// 输入：最大迭代次数
												   double * opts /*= NULL*/,			// 输入：5 维的向量，用来控制优化过程的参数，如果输入 NULL 则采用默认参数，其中
																						// opts[0]，levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																						// opts[1]，当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-17
																						// opts[2]，当待优化参数 2 范数的相对变化量小于该阈值时优化结束，默认为 1.0E-17
																						// opts[3]，当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-17
																						// opts[4]，采用有限差分估计雅阁比矩阵的时候要用到的步长参数，当该参数小于 0 时，雅阁比矩阵式采用中心差分来估计的，这样更准确，但会更慢些，默认为正的情况下，是采用前向差分估计的，默认为 1.0E-06
												   double * info /*= NULL*/,			// 输出：10 维的向量，用来输出优化过程的相关信息，具体每个输出元素的解释可以参考相关的信息解读函数
												   CMatrix & mCovar /*= CMatrix(0)*/,	// 输出：各估计参数之间的协方差矩阵
												   int * iterNum /*= NULL*/				// 输出：迭代次数
												   )
{
	int n = mWrdPts.m_nCol; // 空间点个数

	int m = 3 * n + 6;		// 待优化参数向量的维数

	double * p = new double [m];
	double * x = new double [4*n];

	CMatrix mAngs = GetEulerAngleFromR(mRT);	

	// 赋值优化参数向量和观测向量
	p[0] = mAngs(1, 1);
	p[1] = mAngs(2, 1);
	p[2] = mAngs(3, 1);
	p[3] = mRT(1, 4);
	p[4] = mRT(2, 4);
	p[5] = mRT(3, 4);

	int i,j;
	for (i = 0; i < n; i++)
	{
		p[3*i+6] = mWrdPts(1, i+1);
		p[3*i+7] = mWrdPts(2, i+1);
		p[3*i+8] = mWrdPts(3, i+1);

		// 观测向量赋值
		x[4*i+0] = mImgPts1(1, i+1);
		x[4*i+1] = mImgPts1(2, i+1);
		x[4*i+2] = mImgPts2(1, i+1);
		x[4*i+3] = mImgPts2(2, i+1);
	}

	cam_data cam1, cam2;
	cam1.fx = mK1(1, 1);	cam1.fy = mK1(2, 2);	
	cam1.cx = mK1(1, 3);	cam1.cy = mK1(2, 3);	
	cam1.s  = mK1(1, 2);
	cam1.k[0] = mDist1(1);
	cam1.k[1] = mDist1(2);
	cam1.k[2] = mDist1(3);
	cam1.k[3] = mDist1(4);
	cam1.k[4] = mDist1(5);
	cam1.dist_type = distType1;

	cam2.fx = mK2(1, 1);	cam2.fy = mK2(2, 2);	
	cam2.cx = mK2(1, 3);	cam2.cy = mK2(2, 3);	
	cam2.s  = mK2(1, 2);
	cam2.k[0] = mDist2(1);
	cam2.k[1] = mDist2(2);
	cam2.k[2] = mDist2(3);
	cam2.k[3] = mDist2(4);
	cam2.k[4] = mDist2(5);
	cam2.dist_type = distType2;

	vector<cam_data> vCams;
	vCams.push_back(cam1);
	vCams.push_back(cam2);

	double * covar = new double[m * m];		// 优化后的 m×m 的各待优化参数的协方差矩阵

	// 开始调用 levmar 优化函数
	int n_iter = dlevmar_dif(&proj_relative_XYZ_ext_euler, p, x, m, 4*n, maxIter, opts, info, NULL, covar, &vCams);

	if (iterNum)
	{
		*iterNum = n_iter;
	}

	// 更新相对外参数矩阵
	mRT = GenT(p[3], p[4], p[5]) * GenR_Euler(p[2], p[1], p[0], ZYX);

	// 更新空间点在 1 号视图坐标系中的坐标
	for (i = 0; i < n; i++)
	{
		mWrdPts(1, i+1) = p[3*i+6];
		mWrdPts(2, i+1) = p[3*i+7];
		mWrdPts(3, i+1) = p[3*i+8];
	}

	if (!mCovar.IsNull())
	{
		mCovar = CMatrix(m, m);

		for (i = 0; i < m; i++)
		{
			for (j = 0; j < m; j++)
			{
				mCovar(i+1, j+1) = covar[i*m+j];
			}
		}
	}

	delete [] p;
	delete [] x;
	delete [] covar;
}

// 已知像机内参数及像差系数，利用 levmar 代码库的函数优化像机的外参数 姿态由旋转向量表示， tx, ty, tz，也就是说同时优化像机姿态角和光心位置，共6个待优化参数
// 要求 3 个以上控制点参与优化
void DeepVoid::optim_levmar_ext_rotvec(const vector<Point3d> & objPts,		// 输入：4×n的矩阵，控制点的三维坐标 
									   const vector<Point2d> & imgPts,		// 输入：3×n或2×n的矩阵，控制点的图像坐标
									   const Matx33d & mK,					// 输入：3×3或3×4的矩阵，像机的初始内参数矩阵
									   Matx33d & mR,						// 输入兼输出：4×4的像机初始外参数矩阵
									   Matx31d & mt,						// 输入兼输出：平移向量
									   const CMatrix & mDist,				// 输入：5×1的像机初始像差系数
									   int   distType,						// 输入：像差类型，选择是 Weng's (0) 还是 D.C.Brown's (1)，都是 5 个像差系数
									   int   maxIter,						// 输入：最大迭代次数
									   double * opts /*= NULL*/,			// 输入：5 维的向量，用来控制优化过程的参数，如果输入 NULL 则采用默认参数，其中
																			// opts[0]，levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																			// opts[1]，当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-17
																			// opts[2]，当待优化参数 2 范数的相对变化量小于该阈值时优化结束，默认为 1.0E-17
																			// opts[3]，当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-17
																			// opts[4]，采用有限差分估计雅阁比矩阵的时候要用到的步长参数，当该参数小于 0 时，雅阁比矩阵式采用中心差分来估计的，这样更准确，但会更慢些，默认为正的情况下，是采用前向差分估计的，默认为 1.0E-06
									   double * info /*= NULL*/,			// 输出：10 维的向量，用来输出优化过程的相关信息，具体每个输出元素的解释可以参考相关的信息解读函数
									   CMatrix & mCovar /*= CMatrix(0)*/,   // 输出：各估计参数之间的协方差矩阵
									   int * iterNum /*= NULL*/				// 输出：迭代次数
									   )
{
	// 先分解出 3 个欧拉角初值
	Matx31d mRotVec_init = GetRotVecFromR(mR);

	// 待优化参数个数
	int m = 6;

	double * p = new double[m];

	// 给待优化参数向量赋初值
	p[0] = mRotVec_init(0);
	p[1] = mRotVec_init(1);
	p[2] = mRotVec_init(2);
	p[3] = mt(0);
	p[4] = mt(1);
	p[5] = mt(2);

	// 观测像点个数
	int n = imgPts.size();

	// 观测向量
	double * x = new double[2 * n];

	// 给观测向量赋值
	int i, j;
	for (i = 0; i < n; i++)
	{
		x[2*i]   = imgPts[i].x;
		x[2*i+1] = imgPts[i].y;
	}

	// 接着给要输入到优化函数里的附加标定信息结构赋值
	levmar_calib_data adata;
	adata.camData.fx = mK(0,0);
	adata.camData.fy = mK(1,1);
	adata.camData.s  = mK(0,1);
	adata.camData.cx = mK(0,2);
	adata.camData.cy = mK(1,2);

	adata.camData.k[0] = mDist(1);
	adata.camData.k[1] = mDist(2);
	adata.camData.k[2] = mDist(3);
	adata.camData.k[3] = mDist(4);
	adata.camData.k[4] = mDist(5);

	adata.camData.dist_type = distType;

	adata.objPts = objPts;

	double * covar = new double[m * m];		// 优化后的 m×m 的各待优化参数的协方差矩阵

	// 开始调用 levmar 优化函数
	int n_iter = dlevmar_der(&proj_ext_rotvec, &j_ext_rotvec, p, x, m, 2*n, maxIter, opts, info, NULL, covar, &adata);

	if (iterNum)
	{
		*iterNum = n_iter;
	}

	// 更新外参数矩阵
	mt(0) = p[3]; mt(1) = p[4]; mt(2) = p[5];
	CMatrix cmR = GenR_Rodrigues(p[0], p[1], p[2]);
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			mR(i,j) = cmR(i+1,j+1);
		}
	}

	if (!mCovar.IsNull())
	{
		mCovar = CMatrix(m, m);

		for (i = 0; i < m; i++)
		{
			for (j = 0; j < m; j++)
			{
				mCovar(i+1, j+1) = covar[i*m+j];
			}
		}
	}

	// 释放内存
	delete [] p;
	delete [] x;
	delete [] covar;
}

void DeepVoid::PIRO_Y_Z_AXAYAZ_levmar(const CMatrix & mImgPts1,				// input: 3*n matrix, the distortion free normalized image coordinates on reference image
									  const CMatrix & mImgPts2,				// input: 3*n matrix, the distortion free normalized image coordinates on the other image
							          CMatrix & mRT,						// output:4*4 matrix, the output relative orientation
									  double bx /*= 1*/,					// input: default fixed value for X coordinate of the other image's optical center in reference
									  int   maxIter/*= 128*/,				// input：max iteration
									  double * opts/* = NULL*/,				// 输入：4 维的向量，用来控制优化过程的参数，如果输入 NULL 则采用默认参数，其中
																			// opts[0]，levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
																			// opts[1]，当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-17
																			// opts[2]，当待优化参数 2 范数的相对变化量小于该阈值时优化结束，默认为 1.0E-17
																			// opts[3]，当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-17
									  double * info /*= NULL*/,				// 输出：10 维的向量，用来输出优化过程的相关信息，具体每个输出元素的解释可以参考相关的信息解读函数
									  CMatrix & mCovar /*= CMatrix(0)*/,	// 输出：各估计参数之间的协方差矩阵
									  int * iterNum /*= NULL*/				// 输出：迭代次数
									  )
{
	int i,j;

	int n = mImgPts1.m_nCol; // number of matches

	int m = 5;		// dimension of parameters

	double * p = new double [m];
	double * x = new double [n];

	memset(x,0,n*sizeof(double)); // the functions are supposed to be all zeros

	// 赋值优化参数向量
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	p[3] = 0;
	p[4] = 0;

	double * covar = new double[m * m];		// 优化后的 m×m 的各待优化参数的协方差矩阵

	levmar_mImgPts1_mImgPts2_bx data;
	data.mImgPts1 = mImgPts1.Clone();
	data.mImgPts2 = mImgPts2.Clone();
	data.bx = bx;

	// 开始调用 levmar 优化函数
	int n_iter = dlevmar_der(&f_PIRO_Y_Z_AXAYAZ, &j_PIRO_Y_Z_AXAYAZ, p, x, m, n, maxIter, opts, info, NULL, covar, &data);

	if (iterNum)
	{
		*iterNum = n_iter;
	}

	mRT = GenR_Euler_Radian(p[2], p[3], p[4], ZYX);

	CMatrix mOptCnt(3,1);
	mOptCnt(1) = bx;
	mOptCnt(2) = p[0];
	mOptCnt(3) = p[1];

	CMatrix mT = -mRT.GetRect(1,1,3,3)*mOptCnt; // t=-RC

	mRT(1,4) = mT(1);
	mRT(2,4) = mT(2);
	mRT(3,4) = mT(3);

	if (!mCovar.IsNull())
	{
		mCovar = CMatrix(m, m);

		for (i = 0; i < m; i++)
		{
			for (j = 0; j < m; j++)
			{
				mCovar(i+1, j+1) = covar[i*m+j];
			}
		}

		// change to dev_angle^2
		mCovar(3,3) = mCovar(3,3)*180*180/(CV_PI*CV_PI);
		mCovar(4,4) = mCovar(4,4)*180*180/(CV_PI*CV_PI);
		mCovar(5,5) = mCovar(5,5)*180*180/(CV_PI*CV_PI);
	}

	delete [] p;
	delete [] x;
	delete [] covar;
}