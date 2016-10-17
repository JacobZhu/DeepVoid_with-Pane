#include "stdafx.h"

// ����Ż�����������й��Ż����̵ĸ���Ϣ��
CString DeepVoid::InterpretOptimInfo(double * info,				// 10 ά����������������Ż����̵������Ϣ
									 const CMatrix & mCovar		// ���Ż��������Ƶ�Э�������
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

	// ����������Ĳ�������Э�������Ϊ Null ��ֱ��ֻ���������ַ���
	if (mCovar.IsNull())
	{
		return str1 + str3 + str2;
	}
	
	CString str4;	// ���ַ���¼������Ʋ����ı�׼�� sigma
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

// �ú������Ż��� 2 ����ͼ����ڵ� 1 ����ͼ��������Լ��ռ���ڵ� 1 ����ͼ����ϵ�е�����ʱ�����������пռ����������ͼ�е�������ͶӰ�������
void DeepVoid::proj_relative_XYZ_ext_euler(double * p,			// ���Ż��Ĳ�����Ϊ m ά�ģ��ڸú����д��Ż����� 2 ����ͼ����� 1 ����ͼ�� 3 ��ŷ���Ǻ� 3 ��ƽ���������Լ����� a ����� 3 ά�ռ�����
										   double * hx,			// ���ݴ��Ż������γɵ���ͶӰ�۲�����Ϊ n ά������
										   int m,				// ���Ż�������ά����Ҫ���� a ���ռ�㣬�� m = 3*a+6
										   int n,				// �۲�������ά����Ҫ���� a ���ռ�㣬���ܹ��Ĺ۲�����ά���� n = 4*a
										   void * adata			// ��������������Ϣ����Ը���ͶӰ������˵�Ļ�����Ҫ����������Ϣ��Ҫ��������ڲ������Լ����ϵ��
										   )
{
	vector<cam_data> * pdata;
	pdata =(vector<cam_data> *)adata;
	// ��������ͼ�Ĳ����ṹ�ֱ�ȡ����
	cam_data cam1 = (*pdata)[0];
	cam_data cam2 = (*pdata)[1];

	// �ռ�ͬ����־�����
	int n_pts = (m - 6) / 3;


	/*-----------------------------------------------ÿ�������᲻ͬ�Ĳ���-------------------------------------------------------------*/
	// �� 2 ����ͼ -------------------------------------------------------------------------------------------------------------------
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

	// �� 1 ����ͼ -------------------------------------------------------------------------------------------------------------------
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

	// �� 1 ����ͼ������ͶӰ
	CMatrix mImgPts1_reproj = Proj2IdealImgPt_3Dto2D(mWrdPts, mK1 * mRT1);

	if (0 == cam1.dist_type)
	{
		mImgPts1_reproj = CalcuDistedImgPt_Weng(mImgPts1_reproj, mK1, mDist1); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else if (1 == cam1.dist_type)
	{
		mImgPts1_reproj = CalcuDistedImgPt_DCBrown(mImgPts1_reproj, mK1, mDist1); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else
	{
		mImgPts1_reproj = CalcuDistedImgPt_DCBrown(mImgPts1_reproj, mK1, mDist1); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}

	// �� 2 ����ͼ������ͶӰ
	CMatrix mImgPts2_reproj = Proj2IdealImgPt_3Dto2D(mWrdPts, mK2 * mRT2);

	if (0 == cam2.dist_type)
	{
		mImgPts2_reproj = CalcuDistedImgPt_Weng(mImgPts2_reproj, mK2, mDist2); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else if (1 == cam2.dist_type)
	{
		mImgPts2_reproj = CalcuDistedImgPt_DCBrown(mImgPts2_reproj, mK2, mDist2); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else
	{
		mImgPts2_reproj = CalcuDistedImgPt_DCBrown(mImgPts2_reproj, mK2, mDist2); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}

	for (i = 0; i < n_pts; i++)
	{
		hx[4*i+0] = mImgPts1_reproj(1, i+1);
		hx[4*i+1] = mImgPts1_reproj(2, i+1);
		hx[4*i+2] = mImgPts2_reproj(1, i+1);
		hx[4*i+3] = mImgPts2_reproj(2, i+1);
	}
}

// �ú������Ż���ŷ���Ǻ�ƽ��������ʾ����ͼ�����ʱ����������ռ������ͼ�ϵ���ͶӰ������꣬�Թ� dlevmar_dif ��������
void DeepVoid::proj_ext_rotvec(double * p,			// ���Ż��Ĳ�����Ϊ m ά�ģ��ڸú����д��Ż����� 3 ����ת������ 3 ��ƽ����������� m = 6
							   double * hx,			// ���ݴ��Ż������γɵ���ͶӰ�۲�����Ϊ n ά������
							   int m,				// ���Ż�������ά��
							   int n,				// �۲�������ά��
							   void * adata			// ��������������Ϣ����Ը���ͶӰ������˵�Ļ�����Ҫ����������Ϣ��Ҫ��������ڲ������Լ����ռ���ά�������
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
		mImgPts_reproj = CalcuDistedImgPt_Weng(mImgPts_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else if (1 == cam.dist_type)
	{
		mImgPts_reproj = CalcuDistedImgPt_DCBrown(mImgPts_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else
	{
		mImgPts_reproj = CalcuDistedImgPt_DCBrown(mImgPts_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
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
			mJacob_distxy_xy = Jacobian_distxy_xy_Weng(mImgPt_reproj, mK, mDist); // weng's ���ϵ��
		} 
		else
		{
			mJacob_distxy_xy = Jacobian_distxy_xy_DCBrown(mImgPt_reproj(1), mImgPt_reproj(2), mK, mDist); // D.C.Brown's ���ϵ��
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

// �Ż��� 2 ����ͼ����ڵ� 1 ����ͼ��������Լ��ռ���ڵ� 1 ����ͼ����ϵ�е�����
/*void DeepVoid::optim_levmar_relative_XYZ_ext_euler(CMatrix & mWrdPts,					// ����������4��n�ľ��󣬿��Ƶ����ά���� 
												   const CMatrix & mImgPts1,			// ���룺3��n��2��n�ľ��󣬿ռ���� 1 ����ͼ�е�ͼ������
												   const CMatrix & mImgPts2,			// ���룺3��n��2��n�ľ��󣬿ռ���� 2 ����ͼ�е�ͼ������
												   const CMatrix & mK1,					// ���룺3��3��3��4�ľ���1 ����ͼ���ڲ�������
												   const CMatrix & mK2,					// ���룺3��3��3��4�ľ���2 ����ͼ���ڲ�������
												   CMatrix & mRT,						// ����������4��4�ľ���2 ����ͼ����� 1 ����ͼ�ĳ�ʼ���������
												   const CMatrix & mDist1,				// ���룺5��1�� 1 ����ͼ�����ϵ��
												   const CMatrix & mDist2,				// ���룺5��1�� 2 ����ͼ�����ϵ��
												   int   distType1,						// ���룺1 ����ͼ��������ͣ�ѡ���� Weng's (0) ���� D.C.Brown's (1)������ 5 �����ϵ��
												   int   distType2,						// ���룺2 ����ͼ��������ͣ�ѡ���� Weng's (0) ���� D.C.Brown's (1)������ 5 �����ϵ��
												   int   maxIter,						// ���룺����������
												   double * opts ,			// ���룺5 ά�����������������Ż����̵Ĳ������������ NULL �����Ĭ�ϲ���������
																						// opts[0]��levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																						// opts[1]����Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																						// opts[2]�������Ż����� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																						// opts[3]�������ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																						// opts[4]���������޲�ֹ����Ÿ�Ⱦ����ʱ��Ҫ�õ��Ĳ������������ò���С�� 0 ʱ���Ÿ�Ⱦ���ʽ�������Ĳ�������Ƶģ�������׼ȷ���������Щ��Ĭ��Ϊ��������£��ǲ���ǰ���ֹ��Ƶģ�Ĭ��Ϊ 1.0E-06
												   double * info ,			// �����10 ά����������������Ż����̵������Ϣ������ÿ�����Ԫ�صĽ��Ϳ��Բο���ص���Ϣ�������
												   CMatrix & mCovar ,	// ����������Ʋ���֮���Э�������
												   int * iterNum 				// �������������
												   )
{
	int n = mWrdPts.m_nCol; // �ռ�����

	int m = 3 * n + 6;		// ���Ż�����������ά��

	double * p = new double [m];
	double * x = new double [4*n];

	CMatrix mAngs = GetEulerAngleFromR(mRT);	

	// ��ֵ�Ż����������͹۲�����
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

		// �۲�������ֵ
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

	double * covar = new double[m * m];		// �Ż���� m��m �ĸ����Ż�������Э�������

	// ��ʼ���� levmar �Ż�����
	int n_iter = dlevmar_dif(&proj_relative_XYZ_ext_euler, p, x, m, 4*n, maxIter, opts, info, NULL, covar, &vCams);

	if (iterNum)
	{
		*iterNum = n_iter;
	}

	// ����������������
	mRT = GenT(p[3], p[4], p[5]) * GenR_Euler(p[2], p[1], p[0], ZYX);

	// ���¿ռ���� 1 ����ͼ����ϵ�е�����
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
}*/

// ��֪����ڲ��������ϵ�������� levmar �����ĺ����Ż����������� ��̬����ת������ʾ�� tx, ty, tz��Ҳ����˵ͬʱ�Ż������̬�Ǻ͹���λ�ã���6�����Ż�����
// Ҫ�� 3 �����Ͽ��Ƶ�����Ż�
/*void DeepVoid::optim_levmar_ext_rotvec(const vector<Point3d> & objPts,		// ���룺4��n�ľ��󣬿��Ƶ����ά���� 
									   const vector<Point2d> & imgPts,		// ���룺3��n��2��n�ľ��󣬿��Ƶ��ͼ������
									   const Matx33d & mK,					// ���룺3��3��3��4�ľ�������ĳ�ʼ�ڲ�������
									   Matx33d & mR,						// ����������4��4�������ʼ���������
									   Matx31d & mt,						// ����������ƽ������
									   const CMatrix & mDist,				// ���룺5��1�������ʼ���ϵ��
									   int   distType,						// ���룺������ͣ�ѡ���� Weng's (0) ���� D.C.Brown's (1)������ 5 �����ϵ��
									   int   maxIter,						// ���룺����������
									   double * opts ,			// ���룺5 ά�����������������Ż����̵Ĳ������������ NULL �����Ĭ�ϲ���������
																			// opts[0]��levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																			// opts[1]����Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																			// opts[2]�������Ż����� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																			// opts[3]�������ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																			// opts[4]���������޲�ֹ����Ÿ�Ⱦ����ʱ��Ҫ�õ��Ĳ������������ò���С�� 0 ʱ���Ÿ�Ⱦ���ʽ�������Ĳ�������Ƶģ�������׼ȷ���������Щ��Ĭ��Ϊ��������£��ǲ���ǰ���ֹ��Ƶģ�Ĭ��Ϊ 1.0E-06
									   double * info ,			// �����10 ά����������������Ż����̵������Ϣ������ÿ�����Ԫ�صĽ��Ϳ��Բο���ص���Ϣ�������
									   CMatrix & mCovar ,   // ����������Ʋ���֮���Э�������
									   int * iterNum 				// �������������
									   )
{
	// �ȷֽ�� 3 ��ŷ���ǳ�ֵ
	Matx31d mRotVec_init = GetRotVecFromR(mR);

	// ���Ż���������
	int m = 6;

	double * p = new double[m];

	// �����Ż�������������ֵ
	p[0] = mRotVec_init(0);
	p[1] = mRotVec_init(1);
	p[2] = mRotVec_init(2);
	p[3] = mt(0);
	p[4] = mt(1);
	p[5] = mt(2);

	// �۲�������
	int n = imgPts.size();

	// �۲�����
	double * x = new double[2 * n];

	// ���۲�������ֵ
	int i, j;
	for (i = 0; i < n; i++)
	{
		x[2*i]   = imgPts[i].x;
		x[2*i+1] = imgPts[i].y;
	}

	// ���Ÿ�Ҫ���뵽�Ż�������ĸ��ӱ궨��Ϣ�ṹ��ֵ
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

	double * covar = new double[m * m];		// �Ż���� m��m �ĸ����Ż�������Э�������

	// ��ʼ���� levmar �Ż�����
	int n_iter = dlevmar_der(&proj_ext_rotvec, &j_ext_rotvec, p, x, m, 2*n, maxIter, opts, info, NULL, covar, &adata);

	if (iterNum)
	{
		*iterNum = n_iter;
	}

	// �������������
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

	// �ͷ��ڴ�
	delete [] p;
	delete [] x;
	delete [] covar;
}*/

/*void DeepVoid::PIRO_Y_Z_AXAYAZ_levmar(const CMatrix & mImgPts1,				// input: 3*n matrix, the distortion free normalized image coordinates on reference image
									  const CMatrix & mImgPts2,				// input: 3*n matrix, the distortion free normalized image coordinates on the other image
							          CMatrix & mRT,						// output:4*4 matrix, the output relative orientation
									  double bx ,					// input: default fixed value for X coordinate of the other image's optical center in reference
									  int   maxIter,				// input��max iteration
									  double * opts,				// ���룺4 ά�����������������Ż����̵Ĳ������������ NULL �����Ĭ�ϲ���������
																			// opts[0]��levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																			// opts[1]����Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																			// opts[2]�������Ż����� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
																			// opts[3]�������ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-17
									  double * info ,				// �����10 ά����������������Ż����̵������Ϣ������ÿ�����Ԫ�صĽ��Ϳ��Բο���ص���Ϣ�������
									  CMatrix & mCovar ,	// ����������Ʋ���֮���Э�������
									  int * iterNum 				// �������������
									  )
{
	int i,j;

	int n = mImgPts1.m_nCol; // number of matches

	int m = 5;		// dimension of parameters

	double * p = new double [m];
	double * x = new double [n];

	memset(x,0,n*sizeof(double)); // the functions are supposed to be all zeros

	// ��ֵ�Ż���������
	p[0] = 0;
	p[1] = 0;
	p[2] = 0;
	p[3] = 0;
	p[4] = 0;

	double * covar = new double[m * m];		// �Ż���� m��m �ĸ����Ż�������Э�������

	levmar_mImgPts1_mImgPts2_bx data;
	data.mImgPts1 = mImgPts1.Clone();
	data.mImgPts2 = mImgPts2.Clone();
	data.bx = bx;

	// ��ʼ���� levmar �Ż�����
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
}*/