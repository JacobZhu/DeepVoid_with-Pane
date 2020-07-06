#include "stdafx.h"

// �ú������Ż���ŷ���Ǻ�ƽ��������ʾ����ͼ������Լ��������������ʱ��������� i ������ڵ� j ����ͼ�ϵ���ͶӰ������꣬�Թ� sba �ĺ�������
void DeepVoid::proj_ext_euler_str(int j,			// ��ǰ��ͼ�ı��
								  int i,			// ��ǰ��ı��
								  double * aj,		// ��ǰ��ͼ���������ǰ 3 ��Ϊ��ͼ��̬�� yaw, pitch, roll�� �� 3 ��Ϊ��ͼ��ƽ������ [tx, ty, tz]
								  double * bi,		// ��ǰ�����ά���꣬[X, Y, Z]
								  double * xij,     // ��ǰ���ڵ�ǰ��ͼ�е���ͶӰ������꣬[x, y]
								  void * adata		// ����������ͼ�����������Ϣ��������Ҫ��������ͼ���ڲ����Լ�������ͺ����ϵ����Ϊ�˼���������ͶӰ���
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
		mImgPt_reproj = CalcuDistedImgPt_Weng(mImgPt_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else if (1 == cam.dist_type)
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}

	xij[0] = mImgPt_reproj(1);
	xij[1] = mImgPt_reproj(2);
}

// �ú������Ż�����ת������ƽ��������ʾ����ͼ������Լ��������������ʱ��������� i ������ڵ� j ����ͼ�ϵ���ͶӰ������꣬�Թ� sba �ĺ�������
void DeepVoid::proj_ext_rotvec_str(int j,				// ��ǰ��ͼ�ı��
								   int i,				// ��ǰ��ı��
								   double * aj,			// ��ǰ��ͼ���������ǰ 3 ��Ϊ��ת������ �� 3 ��Ϊ��ͼ��ƽ������ [tx, ty, tz]
								   double * bi,			// ��ǰ�����ά���꣬[X, Y, Z]
								   double * xij,        // ��ǰ���ڵ�ǰ��ͼ�е���ͶӰ������꣬[x, y]
								   void * adata			// ����������ͼ�����������Ϣ��������Ҫ��������ͼ���ڲ����Լ�������ͺ����ϵ����Ϊ�˼���������ͶӰ���
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
		mImgPt_reproj = CalcuDistedImgPt_Weng(mImgPt_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else if (1 == cam.dist_type)
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
	}
	else
	{
		mImgPt_reproj = CalcuDistedImgPt_DCBrown(mImgPt_reproj, mK, mDist); // ��һ��֮��mImgPt_reproj��ŵ��Ǵ�������ͶӰ�������
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
		mJacob_distxy_xy = Jacobian_distxy_xy_Weng(mImgPt_reproj, mK, mDist); // weng's ���ϵ��
	} 
	else
	{
		mJacob_distxy_xy = Jacobian_distxy_xy_DCBrown(mImgPt_reproj(1), mImgPt_reproj(2), mK, mDist); // D.C.Brown's ���ϵ��
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

// �ú�����װ�� Sparse Bundle Adjustment ��Ĺ�����ƽ���
// ����������ͶӰ������
/*int DeepVoid::optim_sba_levmar_XYZ_ext_euler(vector<CloudPoint> & cloudPts,		// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
											 vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����

											 int itermax,			// ���룺����������

											 double * opts,			// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																				// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																				// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0

											 double * info 			// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
																				// info[0]��||e||_2 at initial p��			�ڳ�ʼ�����µĲв�ֵ��д�����ʸ���� 2 ��������ʵӦ�������ʸ���� 2 ������ƽ��
																				// info[1]��||e||_2 at estimated p��		��������������µĲв�ֵ��ͬ��Ӧ�������ʸ���� 2 ������ƽ��
																				// info[2]��||J^T e||_inf at estimated p��	��������������µ�Ŀ�꺯���Ը����Ż������������
																				// info[3]��||dp||_2 at estimated p��		��������������£����Ż����� 2 �����ı仯��
																				// info[4]��mu/max[J^T J]_ii at estimated p��tau (mu/max(Aii))
																				// info[5]��# iterations��					�ܵ�������
																				// info[6]��reason for terminating��		��������ԭ��
																															// 1. Ŀ�꺯�����Ż���������̫С
																															// 2. �����������Ż������仯̫С
																															// 3. �ﵽ����������
																															// 4. �в���Ա仯̫С
																															// 5. too many attempts to increase damping. Restart with increased mu
																															// 6. �в�̫С
																															// 7. stopped by invalid (i.e. NaN or Inf) "func" values. This is a user error
																				// info[7]��# function evaluations��		Ŀ�꺯�����ô���
																				// info[8]��# jacobian evaluations��		Jacobian ������ƴ���
																				// info[9]��# number of linear systems solved��������Է�����ĸ���
											 )
{
	// ���ȿ��Կ϶���������ı�־��Ŀռ�����϶�������Ч�ģ����϶����� 2 ������ͼ�۲⵽��
	// ��ͬ�����֮��ƥ�����ˣ����ͨ����Ŀǰ����������ģ���˲���ƽ��ı�־������������� pts �ĳ���
	int n = cloudPts.size();

	// �����ڲ���ƽ�����ͼ��Ŀ�Ͳ�һ���ˣ��ܲ���ƽ�����ͼ�϶���������Ѿ��궨����ֵ����ͼ
	// ��ͨ����־��󷽽���õ���ͼ�������ֵ��˵����ͼ�������ܹ۲⵽ 3 ���ռ�������֪�ı�־��
	// ���ռ�������֪�ı�־��϶��������� pts �У���˲�����ȥ�ж�����γ�ֵ����ͼ�Ƿ������ٹ۲⵽
	// 3 �� pts �еĵ��ˣ���Ϊ���������ֵ�����˵�������˸�����
	vector<int> vIdxCams;	// ������Ų��������ƽ�����ͼ������ cams �е�����
	vector<cam_data> vCams;	// ���������ƽ��ĸ���ͼ����
	
	int i,j,k;
	for (i = 0; i < cams.size(); i++)
	{
		cam_data cam = cams[i];
		if (cam.R[0] < -90) // ������ת���� R �϶����������������ÿһ��Ӧ�ö��ǵ�λ������Ҳ����˵ÿ��Ԫ�ؾ���ֵ����Ӧ���� 1��R ��ÿ��Ԫ�س�ʼ��Ϊ -100���������Ԫ��Ϊ-100��˵����������϶���û�г�ֵ��
		{
			continue;
		}

		vIdxCams.push_back(i);
		vCams.push_back(cam);
	}

	int m = vCams.size();  // ���������ƽ�����ͼ��Ŀ

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

	// �����Ӿ�����ɨ��ɨ�õĸ��������
	vector<Point2d> vImgPts_vmask;

	// ���ɿ��Ӿ���
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

	// ���ɹ۲�����
	double * x = new double[vImgPts_vmask.size() * 2];

	for (i=0;i<vImgPts_vmask.size();i++)
	{
		x[2 * i + 0] = vImgPts_vmask[i].x;
		x[2 * i + 1] = vImgPts_vmask[i].y;
	}

	// ���� sba ��Ĺ�����ƽ���
	double opts_default[5];

	// ���γ�ʼ�� u ʱ��Ҫ�õ���һ����������The algorithm is not very sensitive to the choice of t, but as a rule of thumb,
	// one should use a small value, eg t=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use
	// t=1E-3 or even t=1
	opts_default[0] = 1.0E-3;
	opts_default[1] = 1.0E-12;
	opts_default[2] = 1.0E-12;
	opts_default[3] = 1.0E-12;
	opts_default[4] = 0;

	// ����û�û��������Ʋ�������ʹ��Ĭ�ϵĿ��Ʋ���
	if (!opts)
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_euler_str, NULL, &vCams, itermax, 0, opts_default, info);
	} 
	else // ��Ϊ�վ�ʹ���û�����Ĳ���
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_euler_str, NULL, &vCams, itermax, 0, opts, info);
	}

	// ���滹Ҫ���Ż���Ĳ����͵�����ȫ������ȥ
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

		// ���Ÿ���ƽ������
		cams[vIdxCams[i]].t[0] = tx;
		cams[vIdxCams[i]].t[1] = ty;
		cams[vIdxCams[i]].t[2] = tz;
	}

	// ���Ż���ĵ����긳��ȥ
	for (i=0;i<n;i++)
	{
		cloudPts[i].m_pt.x = p[6 * m + i * 3 + 0];
		cloudPts[i].m_pt.y = p[6 * m + i * 3 + 1];
		cloudPts[i].m_pt.z = p[6 * m + i * 3 + 2];
	}

	// �ͷ��ڴ�
	delete [] p;
	delete [] vmask;
	delete [] x;

	return vImgPts_vmask.size();
}*/

// �ú�����װ�� Sparse Bundle Adjustment ��Ĺ�����ƽ���
// ����������ͶӰ������
/*int DeepVoid::optim_sba_levmar_XYZ_ext_rotvec(vector<CloudPoint> & cloudPts,	// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
											  vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
											  int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

											  int itermax,			// ���룺����������

											  double * opts ,			// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																				// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																				// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0

											  double * info 			// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
																				// info[0]��||e||_2 at initial p��			�ڳ�ʼ�����µĲв�ֵ��д�����ʸ���� 2 ��������ʵӦ�������ʸ���� 2 ������ƽ��
																				// info[1]��||e||_2 at estimated p��		��������������µĲв�ֵ��ͬ��Ӧ�������ʸ���� 2 ������ƽ��
																				// info[2]��||J^T e||_inf at estimated p��	��������������µ�Ŀ�꺯���Ը����Ż������������
																				// info[3]��||dp||_2 at estimated p��		��������������£����Ż����� 2 �����ı仯��
																				// info[4]��mu/max[J^T J]_ii at estimated p��tau (mu/max(Aii))
																				// info[5]��# iterations��					�ܵ�������
																				// info[6]��reason for terminating��		��������ԭ��
																															// 1. Ŀ�꺯�����Ż���������̫С
																															// 2. �����������Ż������仯̫С
																															// 3. �ﵽ����������
																															// 4. �в���Ա仯̫С
																															// 5. �в�̫С
																															// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																															//	  definite normal equations matrix. Typically, this indicates a programming error in the
																															//    user-supplied Jacobian.
																															// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																															//    This signals a programming error in the user-supplied projection function func.
																				// info[7]��# function evaluations��		Ŀ�꺯�����ô���
																				// info[8]��# jacobian evaluations��		Jacobian ������ƴ���
																				// info[9]��# number of linear systems solved��������Է�����ĸ���
											  )
{
	// ���ȿ��Կ϶���������ı�־��Ŀռ�����϶�������Ч�ģ����϶����� 2 ������ͼ�۲⵽��
	// ��ͬ�����֮��ƥ�����ˣ����ͨ����Ŀǰ����������ģ���˲���ƽ��ı�־������������� pts �ĳ���
	int n = cloudPts.size();

	// �����ڲ���ƽ�����ͼ��Ŀ�Ͳ�һ���ˣ��ܲ���ƽ�����ͼ�϶���������Ѿ��궨����ֵ����ͼ
	// ��ͨ����־��󷽽���õ���ͼ�������ֵ��˵����ͼ�������ܹ۲⵽ 3 ���ռ�������֪�ı�־��
	// ���ռ�������֪�ı�־��϶��������� pts �У���˲�����ȥ�ж�����γ�ֵ����ͼ�Ƿ������ٹ۲⵽
	// 3 �� pts �еĵ��ˣ���Ϊ���������ֵ�����˵�������˸�����
	vector<int> vIdxCams;	// ������Ų��������ƽ�����ͼ������ cams �е�����
	vector<cam_data> vCams;	// ���������ƽ��ĸ���ͼ����

	vIdxCams.push_back(idx_refimg);
	vCams.push_back(cams[idx_refimg]);
	
	int i,j,k;
	for (i = 0; i < cams.size(); i++)
	{
		cam_data cam = cams[i];
		if (cam.R[0] < -90 || i==idx_refimg) // ������ת���� R �϶����������������ÿһ��Ӧ�ö��ǵ�λ������Ҳ����˵ÿ��Ԫ�ؾ���ֵ����Ӧ���� 1��R ��ÿ��Ԫ�س�ʼ��Ϊ -100���������Ԫ��Ϊ-100��˵����������϶���û�г�ֵ��
		{
			continue;
		}

		vIdxCams.push_back(i);
		vCams.push_back(cam);
	}

	int m = vCams.size();  // ���������ƽ�����ͼ��Ŀ

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

	// �����Ӿ�����ɨ��ɨ�õĸ��������
	vector<Point2d> vImgPts_vmask;

	// ���ɿ��Ӿ���
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

	// ���ɹ۲�����
	double * x = new double[vImgPts_vmask.size() * 2];

	for (i=0;i<vImgPts_vmask.size();i++)
	{
		x[2 * i + 0] = vImgPts_vmask[i].x;
		x[2 * i + 1] = vImgPts_vmask[i].y;
	}

	// ���� sba ��Ĺ�����ƽ���
	double opts_default[5];

	// ���γ�ʼ�� u ʱ��Ҫ�õ���һ����������The algorithm is not very sensitive to the choice of t, but as a rule of thumb,
	// one should use a small value, eg t=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use
	// t=1E-3 or even t=1
	opts_default[0] = 1.0E-3;
	opts_default[1] = 1.0E-12;
	opts_default[2] = 1.0E-12;
	opts_default[3] = 1.0E-12;
	opts_default[4] = 0;

	// ����û�û��������Ʋ�������ʹ��Ĭ�ϵĿ��Ʋ���
	if (!opts)
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_rotvec_str, &jacob_ext_rotvec_str, &vCams, itermax, 0, opts_default, info);
	} 
	else // ��Ϊ�վ�ʹ���û�����Ĳ���
	{
		sba_motstr_levmar(n, 0, m, 1, vmask, p, 6, 3, x, NULL, 2, &proj_ext_rotvec_str, &jacob_ext_rotvec_str, &vCams, itermax, 0, opts, info);
	}

	// ���滹Ҫ���Ż���Ĳ����͵�����ȫ������ȥ
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

		// ���Ÿ���ƽ������
		cams[vIdxCams[i]].t[0] = tx;
		cams[vIdxCams[i]].t[1] = ty;
		cams[vIdxCams[i]].t[2] = tz;
	}

	// ���Ż���ĵ����긳��ȥ
	for (i=0;i<n;i++)
	{
		cloudPts[i].m_pt.x = p[6 * m + i * 3 + 0];
		cloudPts[i].m_pt.y = p[6 * m + i * 3 + 1];
		cloudPts[i].m_pt.z = p[6 * m + i * 3 + 2];
	}

	// �ͷ��ڴ�
	delete [] p;
	delete [] vmask;
	delete [] x;

	return vImgPts_vmask.size();
}*/

// �ú�����װ�� Sparse Bundle Adjustment ��Ĺ�����ƽ���
// ����������ͶӰ������
int DeepVoid::optim_sba_levmar_XYZ_ext_rotvec_own(vector<CloudPoint> & cloudPts,	// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
											  vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
											  int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

											  int itermax /*= 1024*/,			// ���룺����������

											  double * opts /*= NULL*/,			// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																				// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																				// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																				// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0

											  double * info /*= NULL*/			// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
																				// info[0]��||e||_2 at initial p��			�ڳ�ʼ�����µĲв�ֵ��д�����ʸ���� 2 ��������ʵӦ�������ʸ���� 2 ������ƽ��
																				// info[1]��||e||_2 at estimated p��		��������������µĲв�ֵ��ͬ��Ӧ�������ʸ���� 2 ������ƽ��
																				// info[2]��||J^T e||_inf at estimated p��	��������������µ�Ŀ�꺯���Ը����Ż������������
																				// info[3]��||dp||_2 at estimated p��		��������������£����Ż����� 2 �����ı仯��
																				// info[4]��mu/max[J^T J]_ii at estimated p��tau (mu/max(Aii))
																				// info[5]��# iterations��					�ܵ�������
																				// info[6]��reason for terminating��		��������ԭ��
																															// 1. Ŀ�꺯�����Ż���������̫С
																															// 2. �����������Ż������仯̫С
																															// 3. �ﵽ����������
																															// 4. �в���Ա仯̫С
																															// 5. �в�̫С
																															// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																															//	  definite normal equations matrix. Typically, this indicates a programming error in the
																															//    user-supplied Jacobian.
																															// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																															//    This signals a programming error in the user-supplied projection function func.
																				// info[7]��# function evaluations��		Ŀ�꺯�����ô���
																				// info[8]��# jacobian evaluations��		Jacobian ������ƴ���
																				// info[9]��# number of linear systems solved��������Է�����ĸ���
											  )
{
	// ���ȿ��Կ϶���������ı�־��Ŀռ�����϶�������Ч�ģ����϶����� 2 ������ͼ�۲⵽��
	// ��ͬ�����֮��ƥ�����ˣ����ͨ����Ŀǰ����������ģ���˲���ƽ��ı�־������������� pts �ĳ���
	int n = cloudPts.size();

	// �����ڲ���ƽ�����ͼ��Ŀ�Ͳ�һ���ˣ��ܲ���ƽ�����ͼ�϶���������Ѿ��궨����ֵ����ͼ
	// ��ͨ����־��󷽽���õ���ͼ�������ֵ��˵����ͼ�������ܹ۲⵽ 3 ���ռ�������֪�ı�־��
	// ���ռ�������֪�ı�־��϶��������� pts �У���˲�����ȥ�ж�����γ�ֵ����ͼ�Ƿ������ٹ۲⵽
	// 3 �� pts �еĵ��ˣ���Ϊ���������ֵ�����˵�������˸�����
	vector<int> vIdxCams;	// ������Ų��������ƽ�����ͼ������ cams �е�����
	vector<cam_data> vCams;	// ���������ƽ��ĸ���ͼ����

	vIdxCams.push_back(idx_refimg);
	vCams.push_back(cams[idx_refimg]);
	
	for (int i=0;i<cams.size();++i)
	{
		cam_data cam = cams[i];
		if (cam.R[0] < -90 || i==idx_refimg) // ������ת���� R �϶����������������ÿһ��Ӧ�ö��ǵ�λ������Ҳ����˵ÿ��Ԫ�ؾ���ֵ����Ӧ���� 1��R ��ÿ��Ԫ�س�ʼ��Ϊ -100���������Ԫ��Ϊ-100��˵����������϶���û�г�ֵ��
		{
			continue;
		}

		vIdxCams.push_back(i);
		vCams.push_back(cam);
	}

	int m = vCams.size();  // ���������ƽ�����ͼ��Ŀ

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

	// �����Ӿ�����ɨ��ɨ�õĸ��������
	vector<Point2d> vImgPts_vmask;

	vector<Matx31d> nxys(n); // ÿ���������Ĳο����ȥ����һ���������
	vector<int> ri_j(n); // ÿ���������Ĳο�ͼ��

	// ���ɿ�����������ptrMat(i,j) ����� Aij��Bij��eij�����������е�����
	// ptrMat ��ϡ�����
	srand((unsigned)time(NULL));
	for (int i=0;i<n;++i)
	{
		CloudPoint cldpt = cloudPts[i];

		vector<int> j_visi; // �۲⵽��� i �ĸ�ͼ������
		vector<Point2d> xys; // �� i �ڸ��۲�ͼ���еĹ۲��������

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

		// ���ѡȡһ���۲�ͼ����Ϊ����� i �Ĳο�ͼ��
		int j_ref = rand()%j_visi.size();

		int j = j_visi[j_ref];
		Point2d xy = xys[j_ref];

		ri_j[i] = j;

		Matx33d K = Ks[j];
		Matx<double,5,1> dist = dists[j];
		int distType = distTypes[j];

		// ȥ���õ������������
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

		// �����õ�����Ĺ�һ���������
		Matx31d nxy;
		nxy(0) = ideal_x;
		nxy(1) = ideal_y;
		nxy(2) = 1;

		nxys[i] = calib::invK(K)*nxy;
	}

	int l = vImgPts_vmask.size(); // ���й۲����ĸ���

	// j_fixed �� i_fixed �����ֱ���ͼ��Ϳռ���Ƿ�̶������
	// j_fixed[j]=1��ʾͼ j �Ĳ���������ƽ�������i_fixed[i]=1 ��ʾ�� i �����겻����ƽ�����
	vector<uchar> j_fixed(m),i_fixed(n);
	j_fixed[0] = 1;

	// covInv ���ÿ���۲����Ĳ�ȷ���ȣ�Э������󣩵��������ʵҲ����ÿ���۲�����Ȩֵ����
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

	// ���滹Ҫ���Ż���Ĳ����͵�����ȫ������ȥ
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

		// ���Ÿ���ƽ������
		cams[vIdxCams[i]].t[0] = t(0);
		cams[vIdxCams[i]].t[1] = t(1);
		cams[vIdxCams[i]].t[2] = t(2);
	}

	// ���Ż���ĵ����긳��ȥ
	for (int i=0;i<n;++i)
	{
		Point3d XYZ = XYZs[i];

		cloudPts[i].m_pt.x = XYZ.x;
		cloudPts[i].m_pt.y = XYZ.y;
		cloudPts[i].m_pt.z = XYZ.z;
	}

	return l;
}

// 20151105���Լ����� OpenCV ��д�� SBA ����
// ����������ͶӰ������
int  DeepVoid::optim_sba_levmar_XYZ_ext_rotvec(SfM_ZZK::PointCloud & map_pointcloud,// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
											   vector<cam_data> & cams,				// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
											   SfM_ZZK::MultiTracks & map_tracks,	// ���룺���е������켣
											   int idx_refimg,						// input:	the reference image, whose R=I, and t =[0,0,0]'
											   double tc /*= 1.5*/,					// input:	�������� Huber Ȩ�صĳ���
											   int itermax /*= 1024*/,				// ���룺����������
											   double * opts /*= NULL*/,			// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																					// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																					// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																					// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																					// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																					// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0
											   double * info /*= NULL*/				// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
																					// info[0]��||e||_2 at initial p��			�ڳ�ʼ�����µĲв�ֵ��д�����ʸ���� 2 ��������ʵӦ�������ʸ���� 2 ������ƽ��
																					// info[1]��||e||_2 at estimated p��		��������������µĲв�ֵ��ͬ��Ӧ�������ʸ���� 2 ������ƽ��
																					// info[2]��||J^T e||_inf at estimated p��	��������������µ�Ŀ�꺯���Ը����Ż������������
																					// info[3]��||dp||_2 at estimated p��		��������������£����Ż����� 2 �����ı仯��
																					// info[4]��mu/max[J^T J]_ii at estimated p��tau (mu/max(Aii))
																					// info[5]��# iterations��					�ܵ�������
																					// info[6]��reason for terminating��		��������ԭ��
																																// 1. Ŀ�꺯�����Ż���������̫С
																																// 2. �����������Ż������仯̫С
																																// 3. �ﵽ����������
																																// 4. �в���Ա仯̫С
																																// 5. �в�̫С
																																// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																																//	  definite normal equations matrix. Typically, this indicates a programming error in the
																																//    user-supplied Jacobian.
																																// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																																//    This signals a programming error in the user-supplied projection function func.
																					// info[7]��# function evaluations��		Ŀ�꺯�����ô���
																					// info[8]��# jacobian evaluations��		Jacobian ������ƴ���
																					// info[9]��# number of linear systems solved��������Է�����ĸ���
											   )
{
	// ���ȿ��Կ϶���������ı�־��Ŀռ�����϶�������Ч�ģ����϶����� 2 ������ͼ�۲⵽��
	// ��ͬ�����֮��ƥ�����ˣ����ͨ����Ŀǰ����������ģ���˲���ƽ��ı�־������������� pts �ĳ���
	int n = map_pointcloud.size();

	// �����ڲ���ƽ�����ͼ��Ŀ�Ͳ�һ���ˣ��ܲ���ƽ�����ͼ�϶���������Ѿ��궨����ֵ����ͼ
	// ��ͨ����־��󷽽���õ���ͼ�������ֵ��˵����ͼ�������ܹ۲⵽ 3 ���ռ�������֪�ı�־��
	// ���ռ�������֪�ı�־��϶��������� pts �У���˲�����ȥ�ж�����γ�ֵ����ͼ�Ƿ������ٹ۲⵽
	// 3 �� pts �еĵ��ˣ���Ϊ���������ֵ�����˵�������˸�����
	vector<int> vIdxCams;	// ������Ų��������ƽ�����ͼ������ cams �е�����
	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double,5,1>> dists;
	vector<int> distTypes;

	// ��������Ϊ�˱�֤�ο�ͼ���ڵ�һλ
	cam_data & cam_ref = cams[idx_refimg];
	vIdxCams.push_back(idx_refimg);
	Ks.push_back(cam_ref.m_K);
	Rs.push_back(cam_ref.m_R);
	ts.push_back(cam_ref.m_t);
	dists.push_back(cam_ref.m_dist);
	distTypes.push_back(cam_ref.dist_type);

	for (int i=0;i<cams.size();++i)
	{
		if (!cams[i].m_bOriented || i==idx_refimg)
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

	int m = Ks.size();  // ���������ƽ�����ͼ��Ŀ

	vector<Point3d> XYZs;
	
	int sizes[] = {n, m};
	SparseMat ptrMat(2,sizes,CV_32SC1);

	// �����Ӿ�����ɨ��ɨ�õĸ��������
	vector<Point2d> vImgPts_vmask;

	int i_tmp=0;
	for (auto iter_objpt=map_pointcloud.begin();iter_objpt!=map_pointcloud.end();++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		XYZs.push_back(iter_objpt->second.m_pt);

		auto iter_found_track = map_tracks.find(trackID);

		const SfM_ZZK::OneTrack & one_track = iter_found_track->second;

		for (int j=0;j<m;++j)
		{
			int idxcam = vIdxCams[j];

			auto iter_found_img = one_track.find(idxcam);

			if (iter_found_img==one_track.end())
			{
				// �ڸ� track ��û���ҵ�ָ������ɶ���Ĺ۲�ͼ��
				continue;
			}

			// �ҵ���ָ��ͼ��
			Point2d imgpt;
// 			imgpt.x = vCams[j].m_feats.key_points[iter_found_img->second].pt.x;
// 			imgpt.y = vCams[j].m_feats.key_points[iter_found_img->second].pt.y;
			imgpt.x = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.x;
			imgpt.y = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.y;
			vImgPts_vmask.push_back(imgpt);

			ptrMat.ref<int>(i_tmp,j) = vImgPts_vmask.size()-1;
		}

		++i_tmp;
	}

	int l = vImgPts_vmask.size(); // ���й۲����ĸ���

	// j_fixed �� i_fixed �����ֱ���ͼ��Ϳռ���Ƿ�̶������
	// j_fixed[j]=1��ʾͼ j �Ĳ���������ƽ�������i_fixed[i]=1 ��ʾ�� i �����겻����ƽ�����
	vector<uchar> j_fixed(m),i_fixed(n);
	j_fixed[0] = 1;

	// covInv ���ÿ���۲����Ĳ�ȷ���ȣ�Э������󣩵��������ʵҲ����ÿ���۲�����Ȩֵ����
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

	vector<double> vds(l);

//	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi(XYZs,Ks,Rs,ts,dists,distTypes,vImgPts_vmask,covInvs,j_fixed,i_fixed,ptrMat,info,tau,itermax,eps1,eps2);
	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);
	
	// ���滹Ҫ���Ż���Ĳ����͵�����ȫ������ȥ
	for (int i=0;i<m;++i)
	{
		int idxcam = vIdxCams[i];

		cam_data & cam = cams[idxcam];

		cam.m_R = Rs[i];
		cam.m_t = ts[i];

		// 20151224���鿴���΢С��ת��������ָ��·�ʽ�ھ����ܶ�ε���֮��᲻�ᵼ����ת���������˻�
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

	// �ù�����ƽ������µ���ͶӰ�в����ж������
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
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			int idx_cam = vIdxCams[j];
			double rpjerr = vds[*ptr];

			auto iter_found_img = iter_found_track->second.find(idx_cam);

			if (rpjerr < sigma3)
			{
				// 20151227����ͶӰ�в��㹻С�����У�����λ�ڸ�ͼ����ǰ���������ͼ����Ȼ�ᱻ��Ϊ���
				Matx31d mXYZ_C = Rs[j] * mXYZ + ts[j];

				if (mXYZ_C(2)<=0)
				{
					iter_found_img->second.second = 0; // ���λ��ͼ���
				}
				else
				{
					iter_found_img->second.second = 1; // ���λ��ͼ��ǰ��
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

// 20200607�����õ����ؼ�Ȩ��С���� IRLS ����ͬʱ�Ż� f,XYZW,R,t
// ����������ͶӰ������
int DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(SfM_ZZK::PointCloud & map_pointcloud,// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
														   vector<cam_data> & cams,				// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
														   SfM_ZZK::MultiTracks & map_tracks,	// ���룺���е������켣
														   int idx_refimg,						// input:	the reference image, whose R=I, and t =[0,0,0]'
														   double tc /*= 1.5*/,					// input:	�������� Huber Ȩ�صĳ���
														   int itermax /*= 1024*/,				// ���룺����������
														   double * opts /*= NULL*/,			// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																								// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																								// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																								// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																								// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																								// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0
														   double * info /*= NULL*/				// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
																								// info[0]��||e||_2 at initial p��			�ڳ�ʼ�����µĲв�ֵ��д�����ʸ���� 2 ��������ʵӦ�������ʸ���� 2 ������ƽ��
																								// info[1]��||e||_2 at estimated p��		��������������µĲв�ֵ��ͬ��Ӧ�������ʸ���� 2 ������ƽ��
																								// info[2]��||J^T e||_inf at estimated p��	��������������µ�Ŀ�꺯���Ը����Ż������������
																								// info[3]��||dp||_2 at estimated p��		��������������£����Ż����� 2 �����ı仯��
																								// info[4]��mu/max[J^T J]_ii at estimated p��tau (mu/max(Aii))
																								// info[5]��# iterations��					�ܵ�������
																								// info[6]��reason for terminating��		��������ԭ��
																								// 1. Ŀ�꺯�����Ż���������̫С
																								// 2. �����������Ż������仯̫С
																								// 3. �ﵽ����������
																								// 4. �в���Ա仯̫С
																								// 5. �в�̫С
																								// 6. stopped due to excessive failed attempts to increase damping for getting a positive
																								//	  definite normal equations matrix. Typically, this indicates a programming error in the
																								//    user-supplied Jacobian.
																								// 7. stopped due to infinite values in the coordinates of the set of predicted projections.
																								//    This signals a programming error in the user-supplied projection function func.
																								// info[7]��# function evaluations��		Ŀ�꺯�����ô���
																								// info[8]��# jacobian evaluations��		Jacobian ������ƴ���
																								// info[9]��# number of linear systems solved��������Է�����ĸ���
														   )
{
	// ���ȿ��Կ϶���������ı�־��Ŀռ�����϶�������Ч�ģ����϶����� 2 ������ͼ�۲⵽��
	// ��ͬ�����֮��ƥ�����ˣ����ͨ����Ŀǰ����������ģ���˲���ƽ��ı�־������������� pts �ĳ���
	int n = map_pointcloud.size();

	// �����ڲ���ƽ�����ͼ��Ŀ�Ͳ�һ���ˣ��ܲ���ƽ�����ͼ�϶���������Ѿ��궨����ֵ����ͼ
	// ��ͨ����־��󷽽���õ���ͼ�������ֵ��˵����ͼ�������ܹ۲⵽ 3 ���ռ�������֪�ı�־��
	// ���ռ�������֪�ı�־��϶��������� pts �У���˲�����ȥ�ж�����γ�ֵ����ͼ�Ƿ������ٹ۲⵽
	// 3 �� pts �еĵ��ˣ���Ϊ���������ֵ�����˵�������˸�����
	vector<int> vIdxCams;	// ������Ų��������ƽ�����ͼ������ cams �е�����
	vector<Matx33d> Ks;
	vector<Matx33d> Rs;
	vector<Matx31d> ts;
	vector<Matx<double,5,1>> dists;
	vector<int> distTypes;

	// ��������Ϊ�˱�֤�ο�ͼ���ڵ�һλ
	cam_data & cam_ref = cams[idx_refimg];
	vIdxCams.push_back(idx_refimg);
	Ks.push_back(cam_ref.m_K);
	Rs.push_back(cam_ref.m_R);
	ts.push_back(cam_ref.m_t);
	dists.push_back(cam_ref.m_dist);
	distTypes.push_back(cam_ref.dist_type);

	for (int i=0;i<cams.size();++i)
	{
		if (!cams[i].m_bOriented || i==idx_refimg)
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

	int m = Ks.size();  // ���������ƽ�����ͼ��Ŀ

	vector<Point3d> XYZs;
	
	// 20200607��n��m�е�ϡ��������ڴ洢�����ڸ�ͼ���ϵĿɼ��ԣ���ʵ�������ǿɼ��ԣ���Ԫ��ֵ��ʵΪÿ�������������������е����
	int sizes[] = {n, m};
	SparseMat ptrMat(2,sizes,CV_32SC1);

	// �����Ӿ�����ɨ��ɨ�õĸ��������
	vector<Point2d> vImgPts_vmask;

	int i_tmp=0;
	for (auto iter_objpt=map_pointcloud.begin();iter_objpt!=map_pointcloud.end();++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		XYZs.push_back(iter_objpt->second.m_pt);

		auto iter_found_track = map_tracks.find(trackID);

		const SfM_ZZK::OneTrack & one_track = iter_found_track->second;

		for (int j=0;j<m;++j)
		{
			int idxcam = vIdxCams[j];

			auto iter_found_img = one_track.find(idxcam);

			if (iter_found_img==one_track.end())
			{
				// �ڸ� track ��û���ҵ�ָ������ɶ���Ĺ۲�ͼ��
				continue;
			}

			// �ҵ���ָ��ͼ��
			Point2d imgpt;
			imgpt.x = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.x;
			imgpt.y = cams[idxcam].m_feats.key_points[iter_found_img->second.first].pt.y;
			vImgPts_vmask.push_back(imgpt);

			ptrMat.ref<int>(i_tmp,j) = vImgPts_vmask.size()-1;
		}

		++i_tmp;
	}

	int l = vImgPts_vmask.size(); // ���й۲����ĸ���

	// j_fixed �� i_fixed �����ֱ���ͼ��Ϳռ���Ƿ�̶������
	// j_fixed[j]=1��ʾͼ j �Ĳ���������ƽ�������i_fixed[i]=1 ��ʾ�� i �����겻����ƽ�����
	// 20200607 �������Ӧ���б𣬹��в���һ��Ϊ�ڲ���������ô���ڲ����̶����Ż�������
	// 20200607 22:30��������������Jacobian�ĺ����п�����֪��j_fixedֻ��ÿ��ͼ����е�A�����������
	// ��Լ��Aij�Ƿ�Ϊ�գ�ͼ���в�����C���������Ӧ��Jacobian��Cij��
	vector<uchar> j_fixed(m),i_fixed(n);
	j_fixed[0] = 1;

	// covInv ���ÿ���۲����Ĳ�ȷ���ȣ�Э������󣩵��������ʵҲ����ÿ���۲�����Ȩֵ����
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

	vector<double> vds(l);

//	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);
	SBA_ZZK::optim_sparse_lm_f_wj_tj_XiYiZiWi_IRLS_Huber(XYZs, Ks, Rs, ts, dists, distTypes, vImgPts_vmask, covInvs, j_fixed, i_fixed, ptrMat, vds, tc, info, tau, itermax, eps1, eps2);
	
	// ���滹Ҫ���Ż���Ĳ����͵�����ȫ������ȥ
	for (int i=0;i<m;++i)
	{
		int idxcam = vIdxCams[i];

		cam_data & cam = cams[idxcam];

		cam.m_K = Ks[i]; // 20200607,Ҳ�����ڲ�������Ϊ��Ч���� f Ҳ�õ����Ż�
		cam.fx = cam.m_K(0, 0);
		cam.fy = cam.m_K(1, 1);
		cam.cx = cam.m_K(0, 2);
		cam.cy = cam.m_K(1, 2);
		cam.s = cam.m_K(0, 1);
	
		cam.m_R = Rs[i];
		cam.m_t = ts[i];		
	}

	// �ù̶���ֵ���ж������
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
				// ��� ptr == NULL��˵����� xij ������
				continue;
			}

			int idx_cam = vIdxCams[j];
			double rpjerr = vds[*ptr];

			auto iter_found_img = iter_found_track->second.find(idx_cam);

			if (rpjerr < sigma3)
			{
				// 20151227����ͶӰ�в��㹻С�����У�����λ�ڸ�ͼ����ǰ���������ͼ����Ȼ�ᱻ��Ϊ���
				Matx31d mXYZ_C = Rs[j] * mXYZ + ts[j];

				if (mXYZ_C(2)<=0)
				{
					iter_found_img->second.second = 0; // ���λ��ͼ���
				}
				else
				{
					iter_found_img->second.second = 1; // ���λ��ͼ��ǰ��
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

// iteratively run sba multiple times, until no outliers are detected
/*int DeepVoid::optim_sba_levmar_XYZ_ext_rotvec_iterative(vector<CloudPoint> & cloudPts,		// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
														vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
														const vector<vector<Point2i>> & allTracks,
														int idx_refimg,					// input:	the reference image, whose R=I, t=[0,0,0]'
														int itermax,				// ���룺����������
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