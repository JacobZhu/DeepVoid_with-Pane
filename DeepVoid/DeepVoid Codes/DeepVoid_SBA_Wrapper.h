/****************************************************************************************************************************/
/* Johnny Zhu's own library, trying to wrap the functions from Sparse Bundle Adjustment lib, and make it for my own purpose.*/
/* Author: Johnny Zhu (������)                                                                             Date:2013.08.15  */
/****************************************************************************************************************************/

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

namespace DeepVoid
{

// �ú������Ż���ŷ���Ǻ�ƽ��������ʾ����ͼ������Լ��������������ʱ��������� i ������ڵ� j ����ͼ�ϵ���ͶӰ������꣬�Թ� sba �ĺ�������
void proj_ext_euler_str(int j,					// ��ǰ��ͼ�ı��
						int i,					// ��ǰ��ı��
						double * aj,			// ��ǰ��ͼ���������ǰ 3 ��Ϊ��ͼ��̬�� yaw, pitch, roll�� �� 3 ��Ϊ��ͼ��ƽ������ [tx, ty, tz]
						double * bi,			// ��ǰ�����ά���꣬[X, Y, Z]
						double * xij,           // ��ǰ���ڵ�ǰ��ͼ�е���ͶӰ������꣬[x, y]
						void * adata			// ����������ͼ�����������Ϣ��������Ҫ��������ͼ���ڲ����Լ�������ͺ����ϵ����Ϊ�˼���������ͶӰ���
						);

// �ú������Ż�����ת������ƽ��������ʾ����ͼ������Լ��������������ʱ��������� i ������ڵ� j ����ͼ�ϵ���ͶӰ������꣬�Թ� sba �ĺ�������
void proj_ext_rotvec_str(int j,					// ��ǰ��ͼ�ı��
						 int i,					// ��ǰ��ı��
						 double * aj,			// ��ǰ��ͼ���������ǰ 3 ��Ϊ��ת������ �� 3 ��Ϊ��ͼ��ƽ������ [tx, ty, tz]
						 double * bi,			// ��ǰ�����ά���꣬[X, Y, Z]
						 double * xij,          // ��ǰ���ڵ�ǰ��ͼ�е���ͶӰ������꣬[x, y]
						 void * adata			// ����������ͼ�����������Ϣ��������Ҫ��������ͼ���ڲ����Լ�������ͺ����ϵ����Ϊ�˼���������ͶӰ���
						 );

void jacob_ext_euler_str(int j,					// current index of image
						 int i,					// current index of object point
						 double * aj,			// parameter of current image, [yaw pitch roll tx ty tz]'
						 double * bi,			// current object point coordinates [X Y Z]'
						 double * Aij,			// the bi's projection on image aj derivative with respective to aj
						 double * Bij,			// the bi's projection on image aj derivative with respective to bi
						 void * adata			// extra data
						 );

void jacob_ext_rotvec_str(int j,				// current index of image
						  int i,				// current index of object point
						  double * aj,			// parameter of current image, [rv1 rv2 rv3 tx ty tz]'
						  double * bi,			// current object point coordinates [X Y Z]'
						  double * Aij,			// the bi's projection on image aj derivative with respective to aj
						  double * Bij,			// the bi's projection on image aj derivative with respective to bi
						  void * adata			// extra data
						  );

// �ú�����װ�� Sparse Bundle Adjustment ��Ĺ�����ƽ���
// ����������ͶӰ������
/*int  optim_sba_levmar_XYZ_ext_euler(vector<CloudPoint> & cloudPts,		// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
									vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����

									int itermax = 1024,					// ���룺����������

									double * opts = NULL,				// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																		// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																		// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0

									double * info = NULL				// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
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
									);*/

// �ú�����װ�� Sparse Bundle Adjustment ��Ĺ�����ƽ���
// ����������ͶӰ������
/*int  optim_sba_levmar_XYZ_ext_rotvec(vector<CloudPoint> & cloudPts,		// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
									 vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
									 int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

									 int itermax = 1024,				// ���룺����������

									 double * opts = NULL,				// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																		// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																		// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0

									 double * info = NULL				// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
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
									 );*/

// �ú�����װ�� Sparse Bundle Adjustment ��Ĺ�����ƽ���
// ����������ͶӰ������
int  optim_sba_levmar_XYZ_ext_rotvec_own(vector<CloudPoint> & cloudPts,		// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
									 vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
									 int idx_refimg,					// input:	the reference image, whose R=I, and t =[0,0,0]'

									 int itermax = 1024,				// ���룺����������

									 double * opts = NULL,				// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																		// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																		// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																		// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0

									 double * info = NULL				// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
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
									 );

// 20151105���Լ����� OpenCV ��д�� SBA ����
// ����������ͶӰ������
int  optim_sba_levmar_XYZ_ext_rotvec(SfM_ZZK::PointCloud & map_pointcloud,	// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
									 vector<cam_data> & cams,				// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
									 SfM_ZZK::MultiTracks & map_tracks,		// ���룺���е������켣
									 int idx_refimg,						// input:	the reference image, whose R=I, and t =[0,0,0]'
									 double tc = 1.5,						// input:	�������� Huber Ȩ�صĳ���
									 int itermax = 1024,					// ���룺����������
									 double * opts = NULL,					// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																			// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																			// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																			// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																			// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																			// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0
									 double * info = NULL					// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
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
									 );

// 20200607�����õ����ؼ�Ȩ��С���� IRLS ����ͬʱ�Ż� f,XYZW,R,t
// ����������ͶӰ������
int  optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(SfM_ZZK::PointCloud & map_pointcloud,	// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
												  vector<cam_data> & cams,				// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
												  SfM_ZZK::MultiTracks & map_tracks,	// ���룺���е������켣
												  double & rltUctt_output,				// ��������������ۺ���Բ�ȷ����ˮƽ��1��sigma��
												  double & uctt_f,						// �����ͼ���е�Ч����Ĳ�ȷ���ȣ�1��sigma��
												  int idx_refimg,						// input:	the reference image, whose R=I, and t =[0,0,0]'
												  double tc = 1.5,						// input:	�������� Huber Ȩ�صĳ���
												  int itermax = 1024,					// ���룺����������
												  double * opts = NULL,					// ���룺�ܹ� 5 �����Ʋ��������Ϊ NULL�������Ĭ�ϲ���
																						// opts[0]��\mu��							levmar �Ż�������Ҫ�õ��Ĳ��� u �ĳ�ʼ�߶����ӣ�Ĭ��Ϊ 1.0E-3
																						// opts[1]��||J^T e||_inf��					��Ŀ�꺯���Ը����Ż������������С�ڵ��ڸ�ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																						// opts[2]��||dp||_2��						�����Ż����� 2 �����ı仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																						// opts[3]��||e||_2��						�����ʸ���� 2 ����С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 1.0E-12
																						// opts[4]��(||e||_2-||e_new||_2)/||e||_2��	�����ʸ���� 2 ��������Ա仯��С�ڸ���ֵʱ�Ż�������Ĭ��Ϊ 0
												  double * info = NULL					// ������ܹ� 10 ��������������������Ҫ���������Ϊ NULL
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
												  );

// iteratively run sba multiple times, until no outliers are detected
/*int optim_sba_levmar_XYZ_ext_rotvec_iterative(vector<CloudPoint> & cloudPts,		// ����������������б�־��Ŀռ����꣬ƽ��֮������ĵ����꽫������
											  vector<cam_data> & cams,			// �������������������ͼ����Ϣ�����а�����ͼ���ڲ���������������ϵ���Լ����۲⵽�ı�־��������꣬ƽ��֮���������Ż�����ͼ��������õ�����
											  const vector<vector<Point2i>> & allTracks,
											  int idx_refimg,					// input:	the reference image, whose R=I, t=[0,0,0]'
											  int itermax = 1024,				// ���룺����������
											  double thresh_rpjErr = 1,			// input:	the threshold to determine outliers
											  double * opts = NULL,				
											  double * info = NULL
											  );*/
}