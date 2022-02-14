// �������е��󵼺���

#pragma once

#include "DeepVoid Codes/DeepVoid_DataType.h"

// 20150525
struct Point4d
{
	double x, y, z, w; // the point coordinates

	Point4d()
	{
		x=y=z=w=0;
	};

	Point4d(double _x, double _y, double _z, double _w)
	{
		x=_x;
		y=_y;
		z=_z;
		w=_w;
	};

	Point4d & operator = (const Point4d & otherPt)
	{
		if (this != &otherPt)
		{
			x=otherPt.x;
			y=otherPt.y;
			z=otherPt.z;
			w=otherPt.w;
		}

		return *this;
	};

	Point4d(const Point4d & pt)
	{
		*this = pt;
	};
};

namespace derivatives
{

// 20151107, ���� Huber Ȩ��
// Ĭ�ϲв� x �϶�����ֵ��������ͶӰ�в�����
double weight_Huber(double x, double c);

// ���Գ���������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> der_xy_fxfycxcys(const Matx33d & mR,	// input:	current rotation matrix
								  const Matx31d & mt,	// input:	current translation vector
								  const Matx31d & mX	// input:	current object coordinates
								  );

// ���Գ���������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> der_xy_fxfycxcys(double u, double v);

// ���Գ���������fx,fy,cx,cy��4���ڲ�����
Matx<double,2,4> der_xy_fxfycxcy(double u, double v);

// ���Գ���������f,cx,cy��3���ڲ�����
Matx<double,2,3> der_xy_fcxcy(double u, double v);

// ���Գ���������f��1���ڲ�����
Matx<double,2,1> der_xy_f(double u, double v);

// ���Գ����Ĺ�һ���������������Գ����������
Matx22d der_uv_xy(double fx, double fy, double s);

// ���Գ������������һ�����������
Matx22d der_xy_uv(double fx, double fy, double s);

// ��һ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> der_uv_fxfycxcys(double x, double y, double fx, double fy, double cx, double cy, double s);

// ��һ�����������Թ�һ�����������
Matx22d der_dudv_uv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5);

// ��һ�����������Թ�һ�����������
Matx22d der_dudv_uv_brown(double u, double v, double k1, double k2, double k3, double k4, double k5);

// ���������Թ�һ������������
Matx22d der_dxdy_dudv(double fx, double fy, double s);

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> der_dxdy_fxfycxcys(double du, double dv);

// ����������fx,fy,cx,cy,��4���ڲ�����
Matx<double,2,4> der_dxdy_fxfycxcy(double du, double dv);

// ����������f,cx,cy,��3���ڲ�����
Matx<double,2,3> der_dxdy_fcxcy(double du, double dv);

// ����������f��1���ڲ�����
Matx<double,2,1> der_dxdy_f(double du, double dv);

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> der_dxdy_fxfycxcys_weng(double u, double v, double k1, double k2, double k3, double k4, double k5);

// ����������fx,fy,cx,cy��4���ڲ�����
Matx<double,2,4> der_dxdy_fxfycxcy_weng(double u, double v, double k1, double k2, double k3, double k4, double k5);

// ����������fx,fy,cx,cy,s��5���ڲ�����
Matx<double,2,5> der_dxdy_fxfycxcys_brown(double u, double v, double k1, double k2, double k3, double k4, double k5);

// ����������fx,fy,cx,cy,s��4���ڲ�����
Matx<double,2,4> der_dxdy_fxfycxcy_brown(double u, double v, double k1, double k2, double k3, double k4, double k5);

// ���Գ�����������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
Matx<double,2,12> der_xy_Rt(double fx, double fy, double s, const Matx33d & mR, const Matx31d & mt, const Matx31d & mX);

// ��һ������������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
Matx<double,2,12> der_uv_Rt(const Matx33d & mR, const Matx31d & mt, const Matx31d & mX);

// ��һ������������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
Matx<double,2,12> der_uv_Rt(const Matx33d & mR, const Matx31d & mt, double X, double Y, double Z, double W);

// ��һ������������ת���� R ��Ԫ���Լ�ƽ������ t ��Ԫ����
// ���� v1 = r11 X + r12 Y + r13 Z + tx W
//		v2 = r21 X + r22 Y + r23 Z + ty W
//		v3 = r31 X + r32 Y + r33 Z + tz W
Matx<double,2,12> der_uv_Rt(double v1, double v2, double v3, double X, double Y, double Z, double W);

// ��һ������������ת���� R ��Ԫ���Լ��������� c ��
// ���� v1 = r11 (X-Cx) + r12 (Y-Cy) + r13 (Z-Cz)
//		v2 = r21 (X-Cx) + r22 (Y-Cy) + r23 (Z-Cz)
//		v3 = r31 (X-Cx) + r32 (Y-Cy) + r33 (Z-Cz)
// Matx<double,2,12> der_uv_Rc(const Matx33d & R, double u, double v, double v1, double v2, double v3, double X, double Y, double Z, double Cx, double Cy, double Cz);

// ��һ������������ת���� R ��Ԫ���Լ��������� c ��
// ���� v1 = r11 (X-WCx) + r12 (Y-WCy) + r13 (Z-WCz)
//		v2 = r21 (X-WCx) + r22 (Y-WCy) + r23 (Z-WCz)
//		v3 = r31 (X-WCx) + r32 (Y-WCy) + r33 (Z-WCz)
Matx<double,2,12> der_uv_Rc(const Matx33d & R, double u, double v, double v1, double v2, double v3, double X, double Y, double Z, double W, double Cx, double Cy, double Cz);

// ��һ���������Կռ���������XYZW��
// ���� v1 = r11 X + r12 Y + r13 Z + tx W
//		v2 = r21 X + r22 Y + r23 Z + ty W
//		v3 = r31 X + r32 Y + r33 Z + tz W
Matx<double,2,4> der_uv_XYZW(const Matx33d & R, const Matx31d & t, double v1, double v2, double v3);

// 20170808, for sparse LM of fundamental matrix
// ���Գ��������Կռ���������XYZW��
// ���� f1 = P11 X + P12 Y + P13 Z + P14 W
//      f2 = P21 X + P22 Y + P23 Z + P24 W
//      g  = P31 X + P32 Y + P33 Z + P34 W
Matx<double, 2, 4> der_xy_XYZW(const Matx34d & P, double f1, double f2, double g);

// 20170812, for sparse LM of fundamental matrix
// ���Գ��������Կռ���������XYW�󵼣�Z����̶�Ϊ1��������Ϊ 0
// ���� f1 = P11 X + P12 Y + P13 Z + P14 W
//      f2 = P21 X + P22 Y + P23 Z + P24 W
//      g  = P31 X + P32 Y + P33 Z + P34 W
Matx<double, 2, 3> der_xy_XYW(const Matx34d & P, double f1, double f2, double g);

// 20170808, for sparse LM of fundamental matrix
// ���Գ���������ͶӰ�����12��Ԫ����
// ���� f1 = P11 X + P12 Y + P13 Z + P14 W
//      f2 = P21 X + P22 Y + P23 Z + P24 W
//      g  = P31 X + P32 Y + P33 Z + P34 W
Matx<double, 2, 12> der_xy_P(double X, double Y, double Z, double W, double f1, double f2, double g);

// ��һ���������Կռ������XYZ��
// ���� v1 = r11 X + r12 Y + r13 Z + tx
//		v2 = r21 X + r22 Y + r23 Z + ty
//		v3 = r31 X + r32 Y + r33 Z + tz
Matx<double,2,3> der_uv_XYZ(const Matx33d & R, double v1, double v2, double v3);

// ��ת����R��Ԫ�ض���ת����w��Ԫ���󵼣�����R=dR*R0��dR=I+[w]�ķ�ʽ��õ�
Matx<double,9,3> der_R_w(const Matx33d & mR);

// ����任����[R|t;0 1]��Ԫ�ض���ת����w��Ԫ���Լ�ƽ��������Ԫ���󵼣�����R=dR*R0��dR=I+[w]�ķ�ʽ��õ�
Matx<double,12,6> der_Rt_wt(const Matx33d & mR);

// ��ת���� R �Լ��������� c ����ת���� w ��Ԫ���Լ��������� c ��Ԫ���󵼣�����R=dR*R0��dR=I+[w]�ķ�ʽ��õ�
Matx<double,12,6> der_Rc_wc(const Matx33d & mR);

// ��һ������������weng's���ϵ����
Matx<double,2,5> der_dudv_ki_weng(double u, double v);

// ��һ������������brown's���ϵ����
Matx<double,2,5> der_dudv_ki_brown(double u, double v);


// ����Լ�����̶���������� /////////////////////////////////////////////////////////////////////////////

// constraint: �̶���ǰͼ��ĳͼ֮��Ļ��߳�Ϊĳֵ: ||Rjck+tj||^2 - d^2 = 0
// ��Լ�����̶Ե�ǰͼ����ת�����Լ�ƽ����������
Matx<double,1,12> der_c_fixedBaseline_Rt(double Xkw,double Ykw, double Zkw,			// ͼ k �Ĺ���������ϵ�е�����
										 double Xkj,double Ykj, double Zkj			// ͼ k �Ĺ����ڵ�ǰͼ��ϵ�е�����
										 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////


Matx<double,2,16> j_f_fxfycxcys_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
											   double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
											   const Matx33d & mK,				// ��ǰ���ڲ�������
											   const Matx33d & mR,				// ��ǰ����̬����
											   const Matx31d & mt,				// ��ǰ��ƽ����������
											   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
											   int distType,					// ������ͣ�0 weng's, 1 brown's
											   double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
											   );

Matx<double,2,15> j_f_fxfycxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,		// �ÿ��Ƶ������
											  double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
											  const Matx33d & mK,				// ��ǰ���ڲ�������
											  const Matx33d & mR,				// ��ǰ����̬����
											  const Matx31d & mt,				// ��ǰ��ƽ����������
											  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
											  int distType,						// ������ͣ�0 weng's, 1 brown's
											  double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
											  );

Matx<double,2,14> j_f_fcxcy_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
										   double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
										   const Matx33d & mK,				// ��ǰ���ڲ�������
										   const Matx33d & mR,				// ��ǰ����̬����
										   const Matx31d & mt,				// ��ǰ��ƽ����������
										   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
										   int distType,					// ������ͣ�0 weng's, 1 brown's
										   double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
										   );

Matx<double,2,12> j_f_f_w_t_k1k2k3k4k5(double X, double Y, double Z,	// �ÿ��Ƶ������
									   double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
									   const Matx33d & mK,				// ��ǰ���ڲ�������
									   const Matx33d & mR,				// ��ǰ����̬����
									   const Matx31d & mt,				// ��ǰ��ƽ����������
									   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
									   int distType,					// ������ͣ�0 weng's, 1 brown's
									   double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
									   );

Matx<double,2,11> j_f_fxfycxcy_w_t_k1(double X, double Y, double Z,		// �ÿ��Ƶ������
									  double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
									  const Matx33d & mK,				// ��ǰ���ڲ�������
									  const Matx33d & mR,				// ��ǰ����̬����
									  const Matx31d & mt,				// ��ǰ��ƽ����������
									  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
									  int distType,						// ������ͣ�0 weng's, 1 brown's
									  double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
									  );

Matx<double,2,10> j_f_fcxcy_w_t_k1(double X, double Y, double Z,	// �ÿ��Ƶ������
								   double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
								   const Matx33d & mK,				// ��ǰ���ڲ�������
								   const Matx33d & mR,				// ��ǰ����̬����
								   const Matx31d & mt,				// ��ǰ��ƽ����������
								   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
								   int distType,					// ������ͣ�0 weng's, 1 brown's
								   double & dx, double & dy			// ��ǰ�����µ���ͶӰ�в�
								   );

Matx<double,2,12> j_f_fxfycxcy_w_t_k1k2(double X, double Y, double Z,	// �ÿ��Ƶ������
										double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
										const Matx33d & mK,				// ��ǰ���ڲ�������
										const Matx33d & mR,				// ��ǰ����̬����
										const Matx31d & mt,				// ��ǰ��ƽ����������
										double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
										int distType,					// ������ͣ�0 weng's, 1 brown's
										double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
										);

Matx<double,2,11> j_f_fcxcy_w_t_k1k2(double X, double Y, double Z,	// �ÿ��Ƶ������
								     double x, double y,			// �ÿ��Ƶ��ʵ�ʹ۲��������
								     const Matx33d & mK,			// ��ǰ���ڲ�������
								     const Matx33d & mR,			// ��ǰ����̬����
								     const Matx31d & mt,			// ��ǰ��ƽ����������
								     double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
								     int distType,					// ������ͣ�0 weng's, 1 brown's
								     double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
								     );

// Bundle adjustment ��ص��󵼺����ۺ��� ////////////////////////////////
void j_f_w_t_XYZW(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
				  double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
				  const Matx33d & mK,						// ��ǰ���ڲ�������
				  const Matx33d & mR,						// ��ǰ����̬����
				  const Matx31d & mt,						// ��ǰ��ƽ����������
				  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
				  int distType,								// ������ͣ�0 weng's, 1 brown's
				  Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
				  Matx<double,2,4> & B,						// ��ͶӰ�������Կռ������XYZW��ĵ�
				  double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
				  );

void j_f_w_t_XYZ(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
				 double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
				 const Matx33d & mK,						// ��ǰ���ڲ�������
				 const Matx33d & mR,						// ��ǰ����̬����
				 const Matx31d & mt,						// ��ǰ��ƽ����������
				 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
				 int distType,								// ������ͣ�0 weng's, 1 brown's
				 Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
				 Matx<double,2,3> & B,						// ��ͶӰ�������Կռ������ XYZ ��ĵ�
				 double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
				 );

void j_f_w_c_XYZW(double X, double Y, double Z, double W,	// �ÿ��Ƶ���������
				  double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
				  const Matx33d & mK,						// ��ǰ���ڲ�������
				  const Matx33d & mR,						// ��ǰ����̬����
				  const Matx31d & mc,						// ��ǰ�Ĺ����������
				  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
				  int distType,								// ������ͣ�0 weng's, 1 brown's
				  Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
				  Matx<double,2,4> & B,						// ��ͶӰ�������Կռ������ XYZW ��ĵ�
				  double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
				  );

void j_f_w_c_XYZ(double X, double Y, double Z, double W,	// �ÿ��Ƶ��������꣬�˴� W ӦΪ 1
				 double x, double y,						// �ÿ��Ƶ��ʵ�ʹ۲��������
				 const Matx33d & mK,						// ��ǰ���ڲ�������
				 const Matx33d & mR,						// ��ǰ����̬����
				 const Matx31d & mc,						// ��ǰ�Ĺ����������
				 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
				 int distType,								// ������ͣ�0 weng's, 1 brown's
				 Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
				 Matx<double,2,3> & B,						// ��ͶӰ�������Կռ������ XYZ ��ĵ�
				 double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
				 );

void j_f_f_w_t_XYZW(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
				    double x, double y,							// �ÿ��Ƶ��ʵ�ʹ۲��������
					const Matx33d & mK,							// ��ǰ���ڲ�������
					const Matx33d & mR,							// ��ǰ����̬����
					const Matx31d & mt,							// ��ǰ��ƽ����������
					double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
					int distType,								// ������ͣ�0 weng's, 1 brown's
					Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
					Matx<double,2,4> & B,						// ��ͶӰ�������Կռ������XYZW��ĵ�
					Matx<double,2,1> & C,						// ��ͶӰ��������һ����Ч���� f ��
					double & dx, double & dy					// ��ǰ�����µ���ͶӰ�в�
					);

void j_f_f_w_c_XYZ(double X, double Y, double Z,	double W,	// �ÿ��Ƶ���������
				   double x, double y,							// �ÿ��Ƶ��ʵ�ʹ۲��������
				   const Matx33d & mK,							// ��ǰ���ڲ�������
				   const Matx33d & mR,							// ��ǰ����̬����
				   const Matx31d & mc,							// ��ǰ�Ĺ����������
				   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
				   int distType,								// ������ͣ�0 weng's, 1 brown's
				   Matx<double,2,6> & A,						// ��ͶӰ�����������������ĵ�
				   Matx<double,2,3> & B,						// ��ͶӰ�������Կռ������XYZ��ĵ�
				   Matx<double,2,1> & C,						// ��ͶӰ��������һ����Ч���� f ��
				   double & dx, double & dy						// ��ǰ�����µ���ͶӰ�в�
				   );

// ������ȵ� BA �����У�һ���ɵ���������� xij �������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void j_f_w_t_w0_t0_d0(double d0,				// ���룺��ǰ������������ο�ͼ�����ȹ���
					  double x, double y,		// ���룺������ڵ�ǰ����ͼ���еĹ۲��������
					  const Matx33d & mK,		// ���룺��ǰ����ͼ����ڲ�������
					  const Matx33d & mR,		// ���룺��ǰ����ͼ�����̬����
					  const Matx31d & mt,		// ���룺��ǰ����ͼ���ƽ����������
					  double k1,double k2,double k3, double k4, double k5,	// ���룺��ǰ����ͼ������ϵ������
					  int distType,				// ���룺��ǰ����ͼ���������ͣ�0 weng's, 1 brown's
					  const Matx31d & nx0,		// ���룺���������ο�ͼ��ϵ�еĹ�һ��������꣬��������������߷���
					  const Matx33d & mR0,		// ���룺��ǰ�ο�ͼ�����̬����
					  const Matx31d & mt0,		// ���룺��ǰ�ο�ͼ���ƽ����������
					  Matx<double,2,6> & A,		// �������ͶӰ�������Գ���ͼ�������ĵ�
					  Matx<double,2,1> & B,		// �������ͶӰ���������������ĵ�
					  Matx<double,2,6> & A0,	// �������ͶӰ�������Բο�ͼ�������ĵ�
					  double & dx, double & dy	// �������ǰ�����µ���ͶӰ�в�
					  );

// 20220206�������������ꡢ�Ҷ�ֵ���Լ���ǰ����С����ͼ��ƥ���������{h0,h1,a0,a1,a2,b0,b1,b2}
// �����ǰ��ƥ����� f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
// �Լ� f ������ 8 ��ƥ������ĵ���
// ���� x' = a0 + a1*x + a2*y
//      y' = b0 + b1*x + b2*y
bool j_f_hi_ai_bi(double x, double y,				// ���룺�����ο�ͼ���е���������
				  double I,							// ���룺�ο�ͼ��(x,y)�㴦��ͼ��Ҷ�ֵ
				  const Mat & img,					// ���룺ƥ��ͼ��
				  double h0, double h1,				// ���룺��С����ͼ��ƥ�����
				  double a0, double a1, double a2,	// ���룺��С����ͼ��ƥ�����
				  double b0, double b1, double b2,	// ���룺��С����ͼ��ƥ�����
				  double & f,						// �����f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
				  Matx<double, 1, 8> & J			// �����f ������ 8 ��ƥ������ĵ���
			      );

// 20220206�������������ꡢRGBֵ���Լ���ǰ����С����ͼ��ƥ���������{h0,h1,a0,a1,a2,b0,b1,b2}
// �����ǰ��ƥ����� f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
// �Լ� f ������ 8 ��ƥ������ĵ���
// ���� x' = a0 + a1*x + a2*y
//      y' = b0 + b1*x + b2*y
bool j_f_hi_ai_bi(double x, double y,				// ���룺�����ο�ͼ���е���������
				  double R, double G, double B,		// ���룺�ο�ͼ��(x,y)�㴦��RGBֵ
				  const Mat & img,					// ���룺ƥ��ͼ��
				  double h0, double h1,				// ���룺��С����ͼ��ƥ�����
				  double a0, double a1, double a2,	// ���룺��С����ͼ��ƥ�����
				  double b0, double b1, double b2,	// ���룺��С����ͼ��ƥ�����
				  Matx31d & f,						// �����f = h0 + h1*I'(x'(a0,a1,a2), y'(b0,b1,b2)) - I
				  Matx<double, 3, 8> & J			// �����f ������ 8 ��ƥ������ĵ���
			      );

// 20220214
// ������ǰ����任���� a�����ű�����, R, t��X1 = aR*X0 + t
// ������ǰ�ĸ���任�в� f = aR*X0 + t - X1
// ������ǰ�в������Ա任������ Jacobian ����
void j_f_a_w_t(const Matx31d & X0,					// ���룺X1 = aRX0 + t
			   const Matx31d & X1,					// ���룺X1 = aRX0 + t
			   double a,							// ���룺��ǰ�߶ȹ��� // const Matx<double, 7, 1> & params,	// ���룺��ǰ����任���� a�����ű�����, angX, angY, angZ, tx, ty, tz
			   const Matx33d & R,					// ���룺��ǰ��ת�������
			   const Matx31d & t,					// ���룺��ǰƽ����������
			   Matx31d & f,							// �����f = aRX0 + t - X1
			   Matx<double, 3, 7> & J				// �����df/dparams
			   );

// 20220214
// ������ǰ����任���� a�����ű�����, R, t��X1 = aR*X0 + t
// ������ǰ�ĸ���任�в� f = aR*X0 + t - X1
// ������ǰ�в������Ա任������ Jacobian ����
void H_g_a_w_t(const vector<Matx31d> & X0s,			// ���룺X1 = aRX0 + t
			   const vector<Matx31d> & X1s,			// ���룺X1 = aRX0 + t
			   double a,							// ���룺��ǰ�߶ȹ��� // const Matx<double, 7, 1> & params,	// ���룺��ǰ����任���� a�����ű�����, angX, angY, angZ, tx, ty, tz
			   const Matx33d & R,					// ���룺��ǰ��ת�������
			   const Matx31d & t,					// ���룺��ǰƽ����������
			   Matx<double, 7, 7> & H,				// �����Hessian ���� H = J'WJ
			   Matx<double, 7, 1> & g,				// ��������� 8 ���������ݶ����� g = J'Wf
			   double & F,							// �������Ŀ�꺯��ֵ F = 0.5*f'Wf
			   vector<Matx31d> & fs					// �����f = aRX0 + t - X1
			   );

// 20220207����С����ͼ��ƥ���Ż��м��� Hessian ���� H �Ͳ����ݶ����� g
bool H_g_hi_ai_bi(const vector<Point2d> & xys,		// ���룺�ο�ͼ���и��ο����ص�����
				  const vector<Vec3d> & RGBs,		// ���룺�ο�ͼ���и��ο����ص�RBGֵ��double�ͣ�[0]:R��[1]:G��[2]:B
				  const Mat & img,					// ���룺ƥ��ͼ��
				  const Matx<double, 8, 1> & x,		// ���룺��ǰ��С����ͼ��ƥ�� 8 ���������ƣ�h0,h1,a0,a1,a2,b0,b1,b2
				  Matx<double, 8, 8> & H,			// �����Hessian ���� H = J'WJ
				  Matx<double, 8, 1> & g,			// ��������� 8 ���������ݶ����� g = J'Wf
				  double & F,						// �������Ŀ�꺯��ֵ F = 0.5*f'Wf
				  vector<Matx31d> & fs,				// ������ο�ͼ����ÿ���ο����ص�Ԥ�����
				  int IRLS = 0,						// ���룺�Ƿ���е����ؼ�Ȩ 0����1��Huber��2��...
				  double e_Huber = 50				// ���룺Huber IRLS ����ֵ
			      );

// 20220208������һ����������Ӧ��������� 2D ����任���� a0,a1,a2,b0,b1,b2
// x1 = a0 + a1x0 + a2y0
// y1 = b0 + b1x0 + b2y0
void compute_affine_2D(const vector<Point2d> & xys0,			// ���룺���вο������е���������
					   const vector<Point2d> & xys1,			// ���룺�ο�����������������ƥ��ͼ���е�ͶӰ���ص�����
					   double & a0, double & a1, double & a2,	// ������������� 2D ����任����
					   double & b0, double & b1, double & b2	// ������������� 2D ����任����
					   );

// void j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
// 				  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
// 				  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
// 				  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
// 				  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
// 				  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
// 				  const vector<Point2d> & xys,				// ���룺����ͼ���ϵ�������꣬������Ϊ m*n ��
// 				  const vector<int> & j_fixed,				// ���룺��Щͼ��Ĳ����ǹ̶��ģ����ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
// 				  const vector<int> & i_fixed,				// ���룺��Щ�ռ�������ǹ̶��ģ������ i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
// 				  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
// 				  vector<Matx<double,2,6>> & A,				// ��������е�Aij���������ų�һ������
// 				  vector<Matx<double,2,4>> & B,				// ��������е�Bij���������ų�һ������
// 				  vector<Matx21d> & e						// ��������е� eij = xij - Q(aj,bi) ���������ų�һ������
// 				  );

void j_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
				  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
				  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
				  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
				  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
				  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
				  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
				  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
				  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
				  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
				  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				  vector<Matx<double,6,6>> & U,				// �����m��Uj���󣬽���ͼ������й�
				  vector<Matx<double,4,4>> & V,				// �����n��Vi���󣬽����ռ�������й�
				  vector<Matx<double,6,4>> & W,				// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
				  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
				  vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
				  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
				  Mat & g,									// �����6*m+4*nά�Ĳ����ݶ�
				  vector<double> & vds						// �����l��������ͶӰ�в���
				  );

// 20151107, iteratively reweighted least square IRLS
// �����ؼ�Ȩģʽ��Ϊ��Ӧ�� outliers
// ���� Huber Ȩ��
void j_f_w_t_XYZW_IRLS_Huber(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
						     const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
						     const vector<Matx33d> & Rs,			// ���룺m��ͼ����ת����
							 const vector<Matx31d> & ts,			// ���룺m��ͼ��ƽ������
							 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
							 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							 vector<Matx22d> & covInvs,				// �����l�������������Э�������������Ҳ�����Խ���Ԫ�� (i)=wi*wi����ÿ���۲���Ȩ�ص�ƽ����Ȩ�����ɹ۲����������ͶӰ�в���������
							 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							 vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
							 vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
							 vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							 vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							 vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
							 Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ��׼ȷ��˵Ӧ���� wi*fi����Ӧ�ó���Ȩ��ֵ
							 Mat & g,								// �����6*m+4*nά�Ĳ����ݶ�
							 vector<double> & vds,					// �����l��������ͶӰ�в���
							 double tc = 3.0						// ���룺������ͶӰ�в� d С�� tc �Ĺ۲���Ȩ��ȫ��Ϊ1������Ȩ��Ϊ tc/d
							 );

// 20170811��һ��ͼ��ԣ���ͼ��ͶӰ����̶�Ϊ[I|0]����һ��ΪP�����Ż�������
// ������һϵ������Ӱ�ռ����ؽ������Ĵ��Ż���������꣬���ǵ� Z ����̶�Ϊ1����Ϊ����ͼ��������ע��Z���겻����Ϊ0�������4ά���������˻�Ϊ��XYW��ά�����ʾ
// �ú�����Ҫ���ڷ����Ե����Ż���ͼ��Ļ������� F
void j_f_P_XYW(const Matx34d & P,				// ���룺��ǰ��ͼͶӰ���� P �Ĺ���
			   double X, double Y, double W,	// ���룺��ǰ������ X Y W �������
			   double x0, double y0,			// ���룺���������ο�ͼ�еĵ�ʵ�ʹ۲��������
			   double x1, double y1,			// ���룺���������ͼ�еĵ�ʵ�ʹ۲��������
			   Matx<double,2,12> & A1,			// ��������������ͼ�е���ͶӰ����������ͶӰ���� P ��Ԫ����ĵ���
			   Matx<double,2,3> & B1,			// ��������������ͼ�е���ͶӰ���������� X Y W ������ĵ���
			   double & dx0, double & dy0,		// �������ǰ�����£����������ο�ͼ�е���ͶӰ�в�
			   double & dx1, double & dy1		// �������ǰ�����£����������ͼ�е���ͶӰ�в�
			   );

// 20170809��һ��ͼ��ԣ���ͼ��ͶӰ����̶�Ϊ[I|0]����һ��ΪP�����Ż�������
// ������һϵ������Ӱ�ռ����ؽ������Ĵ��Ż���������꣬���ǵ� Z ����̶�Ϊ1����Ϊ����ͼ��������ע��Z���겻����Ϊ0�������4ά���������˻�Ϊ��XYW��ά�����ʾ
// �ú�����Ҫ���ڷ����Ե����Ż���ͼ��Ļ������� F
void j_f_P_XYW(const Matx34d & P,									// ���룺��ǰ���Ƶ���ͼ����Ӱ�ռ��е�ͶӰ����
	           const vector<Point3d> & XYWs,						// ���룺��ǰ���Ƶ� n ���ռ��� XYW ����
			   const vector<Point2d> & xysL,						// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������
			   const vector<Point2d> & xysR,						// ���룺n ���ռ������ͼ�й۲��������
			   const vector<Matx22d> & covInvsL,					// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������Э�������������
			   const vector<Matx22d> & covInvsR,					// ���룺n ���ռ������ͼ�й۲��������Э�������������
			   Matx<double, 12, 12> & U,							// �����1 �� U ���󣬽��� P �й�
			   vector<Matx<double, 3, 3>> & V,						// �����n �� Vi ���󣬽��Ϳռ�������й�
			   vector<Matx<double, 12, 3>> & W,						// �����n �� Wi ����ͬʱ�� P �Լ��ռ�������й�
			   Matx<double, 12, 1> & ea,							// �����1 �� ea �в�����
			   vector<Matx<double, 3, 1>> & eb,						// �����n �� ebi �в�����
			   double & F,											// �������ǰ��Ŀ�꺯��ֵ 0.5*ft*covinv*f
			   double & x_norm,										// �������ǰ���Ż�����������ģ����2����L2��|x|2
			   Mat & g,												// �����12+3*nά�Ĳ����ݶ�
			   vector<Point2d> & vds								// �����n ���ռ������������ͼ�е���ͶӰ�в���
			   );

// 20170816��ϡ��LM�Ż����������е�ϡ��������淽�̵ĺ���
void solve_sparseLM_F(double u,								// ���룺����ϵ��
					  const Matx<double,12,12> & U,			// ���룺1��U���󣬽�����ͼ��ͶӰ����P�й�
					  const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
					  const vector<Matx<double,12,3>> & W,	// ���룺n��Wi����ͬʱ���ռ�㼰P�й�
					  const Matx<double,12,1> & ea,			// ���룺1��ea�в�����������P�й�
					  const vector<Matx<double,3,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
					  Matx<double,12,1> & da,				// �����1��P�ĸ�����
					  vector<Matx<double,3,1>> & db,		// �����n���ռ������ĸ�����
					  Mat & h								// �����12+3*nά�Ĳ���������
					  );

void j_f_w_t_XYZ(const vector<Point3d> & XYZs,			// ���룺n���ռ��XYZ����
				 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
				 const vector<Matx33d> & Rs,			// ���룺m��ͼ����ת����
				 const vector<Matx31d> & ts,			// ���룺m��ͼ��ƽ������
				 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
				 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
				 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
				 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
				 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
				 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
				 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				 vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
				 vector<Matx<double,3,3>> & V,			// �����n��Vi���󣬽����ռ�������й�
				 vector<Matx<double,6,3>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
				 vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
				 vector<Matx<double,3,1>> & eb,			// �����n��ebi�в������������ռ�������й�
				 Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
				 Mat & g,								// �����6*m+3*nά�Ĳ����ݶ�
				 vector<double> & vds					// �����l��������ͶӰ�в���
				 );

void j_f_w_c_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
				  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
				  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
				  const vector<Matx31d> & cs,				// ���룺m��ͼ���������
				  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
				  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
				  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
				  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
				  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
				  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
				  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				  vector<Matx<double,6,6>> & U,				// �����m��Uj���󣬽���ͼ������й�
				  vector<Matx<double,4,4>> & V,				// �����n��Vi���󣬽����ռ�������й�
				  vector<Matx<double,6,4>> & W,				// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
				  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
				  vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
				  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
				  Mat & g,									// �����6*m+4*nά�Ĳ����ݶ�
				  vector<double> & vds						// �����l��������ͶӰ�в���
				  );

void j_f_w_c_XYZ(const vector<Point3d> & XYZs,			// ���룺n���ռ��XYZ����
				 const vector<Matx33d> & Ks,			// ���룺m��ͼ���ڲ�������
				 const vector<Matx33d> & Rs,			// ���룺m��ͼ����ת����
				 const vector<Matx31d> & cs,			// ���룺m��ͼ���������
				 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
				 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
				 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
				 const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
				 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
				 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
				 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				 vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
				 vector<Matx<double,3,3>> & V,			// �����n��Vi���󣬽����ռ�������й�
				 vector<Matx<double,6,3>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
				 vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
				 vector<Matx<double,3,1>> & eb,			// �����n��ebi�в������������ռ�������й�
				 Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
				 Mat & g,								// �����6*m+3*nά�Ĳ����ݶ�
				 vector<double> & vds					// �����l��������ͶӰ�в���
				 );

void j_f_f_w_t_XYZW(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
					const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
				    const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
				    const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
				    const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
				    const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
				    const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
				    const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
				    const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
				    const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
				    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				    vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
				    vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
				    vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
					Matx<double,1,1> & Q,					// �����Ψһһ��Q����ֻ���������й�
					vector<Matx<double,1,6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
					vector<Matx<double,1,4>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
				    vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
				    vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
					Matx<double,1,1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
				    Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
				    Mat & g,								// �����1+6*m+4*nά�Ĳ����ݶ�
				    vector<double> & vds					// �����l��������ͶӰ�в���
				    );

// 20200604, iteratively reweighted least square, IRLS
// �����ؼ�Ȩģʽ��Ϊ��Ӧ�� outliers
// ���� Huber Ȩ��
void j_f_f_w_t_XYZW_IRLS_Huber(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
							   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
							   const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
							   const vector<Matx<double, 5, 1>> & dists,// ���룺m��ͼ�����ϵ��
							   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							   vector<Matx22d> & covInvs,				// �����l�������������Э�������������Ҳ�����Խ���Ԫ�� (i)=wi*wi����ÿ���۲���Ȩ�ص�ƽ����Ȩ�����ɹ۲����������ͶӰ�в���������
							   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
							   vector<Matx<double, 6, 6>> & U,			// �����m��Uj���󣬽���ͼ������й�
							   vector<Matx<double, 4, 4>> & V,			// �����n��Vi���󣬽����ռ�������й�
							   vector<Matx<double, 6, 4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
							   Matx<double, 1, 1> & Q,					// �����Ψһһ��Q����ֻ���������й�
							   vector<Matx<double, 1, 6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
							   vector<Matx<double, 1, 4>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
							   vector<Matx<double, 6, 1>> & ea,			// �����m��eaj�в�����������ͼ������й�
							   vector<Matx<double, 4, 1>> & eb,			// �����n��ebi�в������������ռ�������й�
							   Matx<double, 1, 1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
							   Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
							   Mat & g,									// �����1+6*m+4*nά�Ĳ����ݶ�
							   vector<double> & vds,					// �����l��������ͶӰ�в���
							   double tc = 3.0							// ���룺������ͶӰ�в� d С�� tc �Ĺ۲���Ȩ��ȫ��Ϊ1������Ȩ��Ϊ tc/d
							   );

void j_f_f_w_c_XYZ(const vector<Point3d> & XYZs,			// ���룺n���ռ��XYZ����
				   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
				   const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
				   const vector<Matx31d> & cs,				// ���룺m��ͼ���������
				   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
				   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
				   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
				   const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
				   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
				   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
				   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				   vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
				   vector<Matx<double,3,3>> & V,			// �����n��Vi���󣬽����ռ�������й�
				   vector<Matx<double,6,3>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
				   Matx<double,1,1> & Q,					// �����Ψһһ��Q����ֻ���������й�
				   vector<Matx<double,1,6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
				   vector<Matx<double,1,3>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
				   vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
				   vector<Matx<double,3,1>> & eb,			// �����n��ebi�в������������ռ�������й�
				   Matx<double,1,1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
				   Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
				   Mat & g,									// �����1+6*m+3*nά�Ĳ����ݶ�
				   vector<double> & vds						// �����l��������ͶӰ�в���
				   );

void j_f_f_w_t_XYZW_c_fixedBaseline(const vector<Point4d> & XYZWs,			// ���룺n���ռ��XYZW����
									const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
									const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
									const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
									const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
									const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
									const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
									const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
									const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
									const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
									const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									int k_fixedBaseLine,					// ���룺������ͼ��֮��Ļ��߹̶�������k �ǲ�������һ��
									int j_fixedBaseLine,					// ���룺������ͼ��֮��Ļ��߹̶�������j �ǵ�����һ��
									double d_fixedBaseLine,					// ���룺����ͼ��֮��Ĺ̶����߳���
									vector<Matx<double,6,6>> & U,			// �����m��Uj���󣬽���ͼ������й�
									vector<Matx<double,4,4>> & V,			// �����n��Vi���󣬽����ռ�������й�
									vector<Matx<double,6,4>> & W,			// �����l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
									Matx<double,1,1> & Q,					// �����Ψһһ��Q����ֻ���������й�
									vector<Matx<double,1,6>> & G,			// �����m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
									vector<Matx<double,1,4>> & H,			// �����n��Hi����ͬʱ�͹���ͼ������Լ�����й�
									vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
									vector<Matx<double,4,1>> & eb,			// �����n��ebi�в������������ռ�������й�
									Matx<double,1,1> & ec,					// �����Ψһһ��ec�в���������������ͼ������й�
									Mat & f,								// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
									Mat & g,								// �����1+6*m+4*nά�Ĳ����ݶ�
									vector<double> & vds,					// �����l��������ͶӰ�в���
									Matx<double,1,6> & Daj,					// �����Լ�����̶Թ̶����ߵ� j ���������
									double & h								// �����Լ�����̵�ǰ��ֵ
									);

// ������ȵ� BA �����У����пɵ���������� xij �������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void j_f_w_t_w0_t0_d0(const vector<double> & ds,				// ���룺n���ռ����������ֵ
					  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
					  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
					  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
					  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
					  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
					  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵĿɵ���������꣬������Ϊ m*n ��
					  const vector<Matx22d> & covInvs,			// ���룺l�����пɵ����������Э�������������
					  const vector<Matx31d> & nxys,				// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ����һ���������
					  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
					  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
					  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ������ǹ̶��ģ�i_fixed[i]=1��������� i ��ȹ̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
					  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
					  vector<Matx<double,6,6>> & U,				// ������ܹ�(m+1)m/2��Ujk���󣬽���ͼ������й�
					  vector<double> & V,						// �����n��Vi���󣬽�������й�
					  vector<Matx<double,6,1>> & W,				// �����l��Wij����ͬʱ����ȼ���۲�ͼ���й�
					  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
					  vector<double> & eb,						// �����n��ebi�в������������ռ�������й�
					  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
					  Mat & g,									// �����6*m+nά�Ĳ����ݶ�
					  vector<double> & vds						// �����l��������ͶӰ�в���
					  );

// ������ȵ� BA �����У������������ xij �������ɵ����ķǲο�����Լ����ɵ����Ĳο���㣩�������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void j_f_w_t_w0_t0_d0_new(const vector<double> & ds,			// ���룺n���ռ����������ֵ
					  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
					  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
					  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
					  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
					  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
					  const vector<Point2d> & xys,				// ���룺l���۲�������꣬���Ϊ m*n ��
					  const vector<Matx22d> & covInvs,			// ���룺l���۲��������Э�������������
					  const vector<Matx31d> & nxys,				// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ����һ���������
					  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
					  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
					  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ������ǹ̶��ģ�i_fixed[i]=1��������� i ��ȹ̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
					  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
					  vector<Matx<double,6,6>> & U,				// ������ܹ�(m+1)m/2��Ujk���󣬽���ͼ������й�
					  vector<double> & V,						// �����n��Vi���󣬽�������й�
					  vector<Matx<double,6,1>> & W,				// �����l��Wij����ͬʱ����ȼ���۲�ͼ���й�
					  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
					  vector<double> & eb,						// �����n��ebi�в������������ռ�������й�
					  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
					  Mat & g,									// �����6*m+nά�Ĳ����ݶ�
					  vector<double> & vds						// �����l��������ͶӰ�в���
					  );

// ������ȵ� BA �����У������������ xij �������ɵ����ķǲο�����Լ����ɵ����Ĳο���㣩�������ͼ������� w t ��
// ��Ҫ����ο�ͼ������� w0 t0 ��
// ���Ҫ�Ը��������ο�ͼ�е���� d0 ������
void j_f_w_t_w0_t0_d0_new2(const vector<double> & ds,			// ���룺n���ռ����������ֵ
					  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
					  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
					  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
					  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
					  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
					  const vector<Point2d> & xys,				// ���룺l���۲�������꣬���Ϊ m*n ��
					  const vector<Matx22d> & covInvs,			// ���룺l���۲��������Э�������������
					  const vector<Matx31d> & nxys,				// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ����һ���������
					  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
					  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
					  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ������ǹ̶��ģ�i_fixed[i]=1��������� i ��ȹ̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
					  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
					  vector<Matx<double,6,6>> & U,				// ������ܹ�(m+1)m/2��Ujk���󣬽���ͼ������й�
					  vector<Matx<double,1,1>> & V,				// �����n��Vi���󣬽�������й�
					  vector<Matx<double,6,1>> & W,				// �����l��Wij����ͬʱ����ȼ���۲�ͼ���й�
					  vector<Matx<double,6,1>> & ea,			// �����m��eaj�в�����������ͼ������й�
					  vector<Matx<double,1,1>> & eb,			// �����n��ebi�в������������ռ�������й�
					  Mat & f,									// �����2*l�����в�������ʵҲ�������۵�Ŀ�꺯��ֵ
					  Mat & g,									// �����6*m+nά�Ĳ����ݶ�
					  vector<double> & vds						// �����l��������ͶӰ�в���
					  );

// ��� 0 �������������6 ��������в����� 4 �������в����� Sparse Bundle Adjustment ������������淽��
void solveSBA_0_6_4(double u,								// ���룺����ϵ��
					const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
					const vector<Matx<double,6,6>> & U,		// ���룺m��Uj���󣬽���ͼ������й�
					const vector<Matx<double,4,4>> & V,		// ���룺n��Vi���󣬽����ռ�������й�
					const vector<Matx<double,6,4>> & W,		// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
					const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
					const vector<Matx<double,4,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
					vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
					vector<Matx<double,4,1>> & db,			// �����n���ռ������ĸ�����
					Mat & h									// �����6*m+4*nά�Ĳ���������
					);

void solveSBA_0_6_3(double u,								// ���룺����ϵ��
				    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
				    const vector<Matx<double,6,6>> & U,		// ���룺m��Uj���󣬽���ͼ������й�
				    const vector<Matx<double,3,3>> & V,		// ���룺n��Vi���󣬽����ռ�������й�
				    const vector<Matx<double,6,3>> & W,		// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
				    const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
				    const vector<Matx<double,3,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
				    vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
				    vector<Matx<double,3,1>> & db,			// �����n���ռ������ĸ�����
				    Mat & h									// �����6*m+3*nά�Ĳ���������
				    );

void solveSBA_1_6_4(double u,								// ���룺����ϵ��
					const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
					const vector<Matx<double,6,6>> & U,		// ���룺m��Uj���󣬽���ͼ������й�
					const vector<Matx<double,4,4>> & V,		// ���룺n��Vi���󣬽����ռ�������й�
					const vector<Matx<double,6,4>> & W,		// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
					const Matx<double,1,1> & Q,				// ���룺Ψһһ��Q����ֻ���������й�
					const vector<Matx<double,1,6>> & G,		// ���룺m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
					const vector<Matx<double,1,4>> & H,		// ���룺n��Hi����ͬʱ�͹���ͼ������Լ�����й�
					const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
					const vector<Matx<double,4,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
					const Matx<double,1,1> & ec,			// ���룺Ψһһ��ec�в���������������ͼ������й�
					vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
					vector<Matx<double,4,1>> & db,			// �����n���ռ������ĸ�����
					Matx<double,1,1> & dc,					// ���������ͼ������ĸ�����
					Mat & h									// �����1+6*m+4*nά�Ĳ���������
					);

void solveSBA_1_6_4_Daj(double u,								// ���룺����ϵ��
						const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
						const vector<Matx<double,6,6>> & U,		// ���룺m��Uj���󣬽���ͼ������й�
						const vector<Matx<double,4,4>> & V,		// ���룺n��Vi���󣬽����ռ�������й�
						const vector<Matx<double,6,4>> & W,		// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
						const Matx<double,1,1> & Q,				// ���룺Ψһһ��Q����ֻ���������й�
						const vector<Matx<double,1,6>> & G,		// ���룺m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
						const vector<Matx<double,1,4>> & H,		// ���룺n��Hi����ͬʱ�͹���ͼ������Լ�����й�
						const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
						const vector<Matx<double,4,1>> & eb,	// ���룺n��ebi�в������������ռ�������й�
						const Matx<double,1,1> & ec,			// ���룺Ψһһ��ec�в���������������ͼ������й�
						int j_constrained,						// ���룺��Լ����ͼ�������
						const vector<Matx<double,1,6>> & Daj,	// ���룺ÿ��Լ�����̶Ե� j ��ͼ����в����ĵ���
						const vector<double> & hs,				// ���룺ÿ��Լ�������ڵ�ǰ�����µ�ֵ
						vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
						vector<Matx<double,4,1>> & db,			// �����n���ռ������ĸ�����
						Matx<double,1,1> & dc,					// ���������ͼ������ĸ�����
						Mat & h,								// �����1+6*m+4*nά�Ĳ���������
						double & val							// �����0.5*dx'C'lamda
						);

// 20150721,�������һά���������ʾ
void solveSBA_0_6_1(double u,								// ���룺����ϵ��
				    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ������
				    const vector<Matx<double,6,6>> & U,		// ���룺�ܹ�(m+1)m/2�� Ujk ���󣬽���ͼ������й�
				    const vector<Matx<double,1,1>> & V,		// ���룺n��Vi���󣬽����ռ������й�
				    const vector<Matx<double,6,1>> & W,		// ���룺l��Wij����ͬʱ���ռ����ȼ���۲�ͼ���й�
				    const vector<Matx<double,6,1>> & ea,	// ���룺m��eaj�в�����������ͼ������й�
				    const vector<Matx<double,1,1>> & eb,	// ���룺n��ebi�в������������ռ������й�
				    vector<Matx<double,6,1>> & da,			// �����m��ͼ������ĸ�����
				    vector<Matx<double,1,1>> & db,			// �����n���ռ����ȵĸ�����
				    Mat & h									// �����6*m+nά�Ĳ���������
				    );

// void covarianceSBA_0_6_4(const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
// 						 const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
// 						 const vector<Matx<double,4,4>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
// 						 const vector<Matx<double,6,4>> & W		// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
// 						 );

void covarianceSBA_0_6_3(const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
						 const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
						 const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
						 const vector<Matx<double,6,3>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
						 vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э�������
						 vector<Matx<double,3,3>> & cov_b		// �����n���ռ��������Э�������
						 );

void covarianceSBA_1_6_3(const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
						 const vector<Matx<double,6,6>> & U,	// ���룺m��Uj���󣬽���ͼ������й�
						 const vector<Matx<double,3,3>> & V,	// ���룺n��Vi���󣬽����ռ�������й�
						 const vector<Matx<double,6,3>> & W,	// ���룺l��Wij����ͬʱ���ռ�㼰��۲�ͼ���й�
						 const Matx<double,1,1> & Q,			// ���룺Ψһһ��Q����ֻ���������й�
						 const vector<Matx<double,1,6>> & G,	// ���룺m��Gj����ͬʱ�Ͷ��м�����ͼ������й�
						 const vector<Matx<double,1,3>> & H,	// ���룺n��Hi����ͬʱ�͹���ͼ������Լ�����й�
						 vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э������
						 vector<Matx<double,3,3>> & cov_b,		// �����n���ռ��������Э������
						 Matx<double,1,1> & cov_c,				// �������������Э������
						 vector<Matx<double,1,6>> & cov_ca		// �����ͼ�񹲲����͸�ͼ����в���֮���Э������
						 );

void covarianceSBA_wj_cj_XiYiZi(const vector<Point3d> & XYZs,			// ���룺n���ռ������
								const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
								const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
								const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
								const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								vector<Matx<double,6,6>> & cov_a,		// �����m��ͼ����в�����Э������
								vector<Matx<double,3,3>> & cov_b		// �����n���ռ��������Э������
								);

void covarianceSBA_f_wj_cj_XiYiZi(const vector<Point3d> & XYZs,				// ���룺n���ռ������
								  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								  const vector<Matx33d> & Rs,				// ���룺m��ͼ����ת����
								  const vector<Matx31d> & ts,				// ���룺m��ͼ��ƽ������
								  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
								  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								  vector<Matx<double,6,6>> & cov_a,			// �����m��ͼ����в�����Э������
								  vector<Matx<double,3,3>> & cov_b,			// �����n���ռ��������Э������
								  Matx<double,1,1> & cov_c,					// �������������Э������
								  vector<Matx<double,1,6>> & cov_ca			// �����ͼ�񹲲����͸�ͼ����в���֮���Э������
								  );

//////////////////////////////////////////////////////////////////////////

void j_f_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
								  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
								  const Matx33d & mK,									// ��ǰ���ڲ�������
								  const Matx33d & mR,									// ��ǰ����̬����
								  const Matx31d & mt,									// ��ǰ��ƽ����������
								  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
								  int distType,											// ������ͣ�0 weng's, 1 brown's
								  Mat & J,												// �����	Jacobian����
								  Mat & f,												// �����	������Ŀ�꺯��ֵ
								  vector<double> & vds									// 
								  );

void j_f_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
								 const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
								 const Matx33d & mK,									// ��ǰ���ڲ�������
								 const Matx33d & mR,									// ��ǰ����̬����
								 const Matx31d & mt,									// ��ǰ��ƽ����������
								 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
								 int distType,											// ������ͣ�0 weng's, 1 brown's
								 Mat & J,												// �����	Jacobian����
								 Mat & f,												// �����	������Ŀ�꺯��ֵ
								 vector<double> & vds									// 
								 );

void j_f_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
							  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
							  const Matx33d & mK,									// ��ǰ���ڲ�������
							  const Matx33d & mR,									// ��ǰ����̬����
							  const Matx31d & mt,									// ��ǰ��ƽ����������
							  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
							  int distType,											// ������ͣ�0 weng's, 1 brown's
							  Mat & J,												// �����	Jacobian����
							  Mat & f,												// �����	������Ŀ�꺯��ֵ
							  vector<double> & vds									// 
							  );

void j_f_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
						  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
						  const Matx33d & mK,									// ��ǰ���ڲ�������
						  const Matx33d & mR,									// ��ǰ����̬����
						  const Matx31d & mt,									// ��ǰ��ƽ����������
						  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
						  int distType,											// ������ͣ�0 weng's, 1 brown's
						  Mat & J,												// �����	Jacobian����
						  Mat & f,												// �����	������Ŀ�꺯��ֵ
						  vector<double> & vds									// 
						  );

void j_f_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
						 const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
						 const Matx33d & mK,									// ��ǰ���ڲ�������
						 const Matx33d & mR,									// ��ǰ����̬����
						 const Matx31d & mt,									// ��ǰ��ƽ����������
						 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
						 int distType,											// ������ͣ�0 weng's, 1 brown's
						 Mat & J,												// �����	Jacobian����
						 Mat & f,												// �����	������Ŀ�꺯��ֵ
						 vector<double> & vds									// 
						 );

void j_f_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
					  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
					  const Matx33d & mK,									// ��ǰ���ڲ�������
					  const Matx33d & mR,									// ��ǰ����̬����
					  const Matx31d & mt,									// ��ǰ��ƽ����������
					  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
					  int distType,											// ������ͣ�0 weng's, 1 brown's
					  Mat & J,												// �����	Jacobian����
					  Mat & f,												// �����	������Ŀ�꺯��ֵ
					  vector<double> & vds									// 
					  );

void j_f_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
						   const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
						   const Matx33d & mK,									// ��ǰ���ڲ�������
						   const Matx33d & mR,									// ��ǰ����̬����
						   const Matx31d & mt,									// ��ǰ��ƽ����������
						   double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
						   int distType,										// ������ͣ�0 weng's, 1 brown's
						   Mat & J,												// �����	Jacobian����
						   Mat & f,												// �����	������Ŀ�꺯��ֵ
						   vector<double> & vds									// 
						   );

void j_f_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,					// ���룺		���Ƶ�����
						const vector<Point2d> & vImgPts,					// ���룺		���Ƶ�۲��������
						const Matx33d & mK,									// ��ǰ���ڲ�������
						const Matx33d & mR,									// ��ǰ����̬����
						const Matx31d & mt,									// ��ǰ��ƽ����������
						double k1,double k2,double k3, double k4, double k5,// ��ǰ�����ϵ������
						int distType,										// ������ͣ�0 weng's, 1 brown's
						Mat & J,											// �����	Jacobian����
						Mat & f,											// �����	������Ŀ�꺯��ֵ
						vector<double> & vds								// 
						);

// Matx<double,2,16> j_f_fxfycxcys_w_t_k1k2k3k4k5_weng(double X, double Y, double Z,	// �ÿ��Ƶ������
// 													double x, double y,				// �ÿ��Ƶ��ʵ�ʹ۲��������
// 													const Matx33d & mK,				// ��ǰ���ڲ�������
// 													const Matx33d & mR,				// ��ǰ����̬����
// 													const Matx31d & mt,				// ��ǰ��ƽ����������
// 													double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
// 													double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
// 													);

// Matx<double,2,16> j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(double X, double Y, double Z,	// �ÿ��Ƶ������
// 													 double x, double y,			// �ÿ��Ƶ��ʵ�ʹ۲��������
// 													 const Matx33d & mK,			// ��ǰ���ڲ�������
// 													 const Matx33d & mR,			// ��ǰ����̬����
// 													 const Matx31d & mt,			// ��ǰ��ƽ����������
// 													 double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
// 													 double & dx, double & dy		// ��ǰ�����µ���ͶӰ�в�
// 													 );

// void j_f_fxfycxcy_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,						// ���룺		���Ƶ�����
// 									  const vector<Point2d> & vImgPts,						// ���룺		���Ƶ�۲��������
// 									  const Matx33d & mK,									// ��ǰ���ڲ�������
// 									  const Matx33d & mR,									// ��ǰ����̬����
// 									  const Matx31d & mt,									// ��ǰ��ƽ����������
// 									  double k1,double k2,double k3, double k4, double k5,	// ��ǰ�����ϵ������
// 									  Mat & J,												// �����	Jacobian����
// 									  Mat & f,												// �����	������Ŀ�꺯��ֵ
// 									  vector<double> & vds									// 
// 									  );
// 
// void j_f_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,					// ���룺		���Ƶ�����
// 									    const vector<Point2d> & vImgPts,					// ���룺		���Ƶ�۲��������
// 									    const Matx33d & mK,									// ��ǰ���ڲ�������
// 									    const Matx33d & mR,									// ��ǰ����̬����
// 									    const Matx31d & mt,									// ��ǰ��ƽ����������
// 									    double k1,double k2,double k3, double k4, double k5,// ��ǰ�����ϵ������
// 									    Mat & J,											// �����	Jacobian����
// 									    Mat & f,											// �����	������Ŀ�꺯��ֵ
// 									    vector<double> & vds								// 
// 									    );

}

namespace distortions
{

// �����һ����������
void dudv_weng(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv);

// �����һ����������
// examined on 2015.04.22
void dudv_brown(double u, double v, double k1, double k2, double k3, double k4, double k5, double & du, double & dv);

// ������������
void dxdy_weng(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy);

// ������������
// examined on 2015.04.22
void dxdy_brown(double fx, double fy, double s, double u, double v, double k1, double k2, double k3, double k4, double k5, double & dx, double & dy);

// �Դ�������������ȥ����
void remove_weng(double fx, double fy, double cx, double cy, double s,
				 double k1, double k2, double k3, double k4, double k5,
	             double dist_x, double dist_y,
				 double & ideal_x, double & ideal_y,
				 int maxIter = 32, 
				 double eps = 1e-10
				 );

// �Դ�������������ȥ����
void remove_brown(double fx, double fy, double cx, double cy, double s,
				  double k1, double k2, double k3, double k4, double k5,
	              double dist_x, double dist_y,
				  double & ideal_x, double & ideal_y,
				  int maxIter = 32, 
				  double eps = 1e-10
				  );

}

namespace calib
{

const double R2D = 180.0/CV_PI;
const double D2R = CV_PI/180.0;

// �������ڡ�Matrix Computations 3rd Edition��һ��P.216�е�Algorithm 5.1.3 (Computes Givens c and s)
// ���������[a; b]'����ʹ��[c s; -s c]' * [a; b]' = [r; 0]'������cos��sin
void Givens(double a, double b, double & c, double & s);

// ��m��n�ľ���A����RQ�ֽ⣨Ҫ��m<=n����A = RQ������QΪn��n����������RΪm��n�ģ������Խ���Ԫ�ؽ�Ϊ���������Ǿ���
// ���õ���Givens��ת����ʵ�ֵģ�����Ҫ��m<=n��ԭ���ڱʼǱ�������ϸ����
void RQ_Givens(const Mat & A, Mat & R, Mat & Q);

// ��3��3�ľ���ʵʩRQ�ֽ�
void RQ3x3_Givens(const Matx33d & A, Matx33d & R, Matx33d & Q);

// �����������ת���󣬷���һ�� 3 ά����������Ϊ ��|v|������ v Ϊ��ת��ĵ�λ�������� Ϊ��ת����
Matx31d converse_R_rotvec(const Matx33d & mR);

// ����ת����ת��Ϊ��ת����
Matx33d converse_rotvec_R(double rv1, double rv2, double rv3);
Matx33d converse_rotvec_R(const Matx31d & v);
Matx33d converse_rotvec_R(const Matx13d & v);

// ������Ԫ����������ת����
Matx33d converse_quaternion_R(double a, double b, double c, double d);

// given the rotational radian by axis-X, compute the rotation matrix
Matx33d converse_radX_R(double rad);
// given the rotational angle by axis-X, compute the rotation matrix
Matx33d converse_angX_R(double ang);

// given the rotational radian by axis-Y, compute the rotation matrix
Matx33d converse_radY_R(double rad);
// given the rotational angle by axis-Y, compute the rotation matrix
Matx33d converse_angY_R(double ang);

// given the rotational radian by axis-Z, compute the rotation matrix
Matx33d converse_radZ_R(double rad);
// given the rotational angle by axis-Z, compute the rotation matrix
Matx33d converse_angZ_R(double ang);

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
Matx33d normalize_2d(const vector<Point2f> & pts, vector<Point2f> & pts_t);

// zhaokunz, 20150107, <multiple view geometry in computer vision 2nd edition> p.107
// Normalizing transformations based on isotropic scaling
// return 3*3 transformation matrix
// tested on 2015.04.22
Matx33d normalize_2d(const vector<Point2d> & pts, vector<Point2d> & pts_t);

// 20150421�������пռ��ƽ������������ϵ�������е㵽���ĵ�ƽ������Ϊ3^0.5
Matx44d normalize_3d(const vector<Point3f> & pts, vector<Point3f> & pts_t);

// 20150421�������пռ��ƽ������������ϵ�������е㵽���ĵ�ƽ������Ϊ3^0.5
// tested on 2015.04.22
Matx44d normalize_3d(const vector<Point3d> & pts, vector<Point3d> & pts_t);

Matx33d cross_mat(const Matx31d & v);
Matx33d cross_mat(const Matx13d & v);

// �������K����������
// tested on 2015.04.22
Matx33d invK(const Matx33d & mK);
Matx33d invK(double fx, double fy, double cx, double cy, double s);

// wx=PX,����PΪ3��4�ľ���XΪ4��1������
Matx34d dlt_34(const Mat & mX,	// ���룺	4��n �ľ���
			   const Mat & mx	// ���룺	2��n �ľ���
			   );

// wx=HX,����HΪ3��3�ľ���XΪ3��1������
Matx33d dlt_33(const Mat & mX,	// ���룺	3��n �ľ���
			   const Mat & mx	// ���룺	2��n �ľ���
			   );

// �����Ӧ�����Խ�aH�зֽ������ڲκ���ת���󣬲����س߶�����a
double decomp_KR(const Matx33d & mKR,	// ���룺	3��3 ��aKR����Ҳ�������Ӧ
				 Matx33d & mK,			// �����	3��3 ������ڲξ���
				 Matx33d & mR			// �����	3��3 �������ת����
				 );

// ��ͶӰ��������Խ�aP�зֽ������������
void decomp_P(const Matx34d & mP,	// ���룺	3��4 ��ͶӰ����aP
			  Matx33d & mK,			// �����	3��3 ������ڲξ���
			  Matx33d & mR,			// �����	3��3 �������ת����
			  Matx31d & mt			// �����	3��1 �����ƽ������
			  );

// �����Խ�a[R|t]�зֽ����������
void decomp_R_t(const Matx34d & mR_t,	// ���룺	3��4 ��a[R|t]
				Matx33d & mR,			// �����	3��3 �������ת����
				Matx31d & mt			// �����	3��1 �����ƽ������
				);

// �����Խ�aK[r1 r2 t]�зֽ����������
void decomp_H(const Matx33d & mH,	// ���룺	3��3 ��aK[r1 r2 t]
			  const Matx33d & mK,	// ���룺	��֪�� 3��3 ���ڲξ���
			  Matx33d & mR,			// �����	3��3 �������ת����
			  Matx31d & mt			// �����	3��1 �����ƽ������
			  );

// ����6�����Ͽ��Ƶ����Ա궨���ͶӰ����
Matx34d calib_P(const vector<Point3d> & pts3d, const vector<Point2d> & pts2d);

// ����4���Ͽ��Ƶ�͹���λ�����Ա궨��������ӦH=KR
Matx33d calib_KR(const vector<Point3d> & pts3d, const vector<Point2d> & pts2d, const Point3d & c);

}

namespace optim
{
void optim_lm_fxfycxcys_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
									   const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
									   Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
									   Matx33d & mR,					// ����������	���Ż����Ż������ת����
									   Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
									   double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
									   int distType = 0,				// ���룺	0 weng's, 1 brown's
									   double * info = NULL,			// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									   int maxIter = 15,				// input:	the maximum number of iterations
									   double eps1 = 1.0E-8,			// input:	threshold
									   double eps2 = 1.0E-12			// input:	threshold
									   );

void optim_lm_fxfycxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
									  const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
									  Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
									  Matx33d & mR,						// ����������	���Ż����Ż������ת����
									  Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
									  double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
									  int distType = 0,					// ���룺	0 weng's, 1 brown's
									  double * info = NULL,				// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									  double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									  int maxIter = 15,					// input:	the maximum number of iterations
									  double eps1 = 1.0E-8,				// input:	threshold
									  double eps2 = 1.0E-12				// input:	threshold
									  );

void optim_lm_fcxcy_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
								   const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
								   Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
								   Matx33d & mR,					// ����������	���Ż����Ż������ת����
								   Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
								   double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
								   int distType = 0,				// ���룺	0 weng's, 1 brown's
								   double * info = NULL,			// output:	runtime info, 5-vector
																	// info[0]:	the initial reprojection error
																	// info[1]:	the final reprojection error
																	// info[2]: final max gradient
																	// info[3]: the number of iterations
																	// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								   int maxIter = 15,				// input:	the maximum number of iterations
								   double eps1 = 1.0E-8,			// input:	threshold
								   double eps2 = 1.0E-12			// input:	threshold
								   );

void optim_lm_f_w_t_k1k2k3k4k5(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
							   const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
							   Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
							   Matx33d & mR,					// ����������	���Ż����Ż������ת����
							   Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
							   double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
							   int distType = 0,				// ���룺	0 weng's, 1 brown's
							   double * info = NULL,			// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							   int maxIter = 15,				// input:	the maximum number of iterations
							   double eps1 = 1.0E-8,			// input:	threshold
							   double eps2 = 1.0E-12			// input:	threshold
							   );

void optim_lm_fxfycxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
							  const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
							  Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
							  Matx33d & mR,						// ����������	���Ż����Ż������ת����
							  Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
							  double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
							  int distType = 0,					// ���룺	0 weng's, 1 brown's
							  double * info = NULL,				// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 15,					// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,				// input:	threshold
							  double eps2 = 1.0E-12				// input:	threshold
							  );

void optim_lm_fcxcy_w_t_k1(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
						   const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
						   Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
						   Matx33d & mR,					// ����������	���Ż����Ż������ת����
						   Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
						   double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
						   int distType = 0,				// ���룺	0 weng's, 1 brown's
						   double * info = NULL,			// output:	runtime info, 5-vector
															// info[0]:	the initial reprojection error
															// info[1]:	the final reprojection error
															// info[2]: final max gradient
															// info[3]: the number of iterations
															// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
						   double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
						   int maxIter = 15,				// input:	the maximum number of iterations
						   double eps1 = 1.0E-8,			// input:	threshold
						   double eps2 = 1.0E-12			// input:	threshold
						   );

void optim_lm_fxfycxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,// ���룺		���Ƶ�����
								const vector<Point2d> & vImgPts,// ���룺		���Ƶ�۲��������
								Matx33d & mK,					// ����������	���Ż����Ż�����ڲ�������
								Matx33d & mR,					// ����������	���Ż����Ż������ת����
								Matx31d & mt,					// ����������	���Ż����Ż����ƽ������
								double * dist,					// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
								int distType = 0,				// ���룺	0 weng's, 1 brown's
								double * info = NULL,			// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								double tau = 1.0E-3,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								int maxIter = 15,				// input:	the maximum number of iterations
								double eps1 = 1.0E-8,			// input:	threshold
								double eps2 = 1.0E-12			// input:	threshold
								);

void optim_lm_fcxcy_w_t_k1k2(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
							 const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
						     Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
						     Matx33d & mR,						// ����������	���Ż����Ż������ת����
						     Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
						     double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
						     int distType = 0,					// ���룺	0 weng's, 1 brown's
						     double * info = NULL,				// output:	runtime info, 5-vector
																// info[0]:	the initial reprojection error
																// info[1]:	the final reprojection error
																// info[2]: final max gradient
																// info[3]: the number of iterations
																// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
						     double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
						     int maxIter = 15,					// input:	the maximum number of iterations
						     double eps1 = 1.0E-8,				// input:	threshold
						     double eps2 = 1.0E-12				// input:	threshold
						     );

// void optim_lm_fxfycxcys_w_t_k1k2k3k4k5_weng(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
// 											const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
// 											Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
// 											Matx33d & mR,						// ����������	���Ż����Ż������ת����
// 											Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
// 											double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
// 											double * info = NULL,				// output:	runtime info, 5-vector
// 																				// info[0]:	the initial reprojection error
// 																				// info[1]:	the final reprojection error
// 																				// info[2]: final max gradient
// 																				// info[3]: the number of iterations
// 																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
// 											double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
// 											int maxIter = 15,					// input:	the maximum number of iterations
// 											double eps1 = 1.0E-8,				// input:	threshold
// 											double eps2 = 1.0E-12				// input:	threshold
// 											);

// void optim_lm_fxfycxcys_w_t_k1k2k3k4k5_brown(const vector<Point3d> & vWrdPts,	// ���룺		���Ƶ�����
// 											 const vector<Point2d> & vImgPts,	// ���룺		���Ƶ�۲��������
// 											 Matx33d & mK,						// ����������	���Ż����Ż�����ڲ�������
// 											 Matx33d & mR,						// ����������	���Ż����Ż������ת����
// 											 Matx31d & mt,						// ����������	���Ż����Ż����ƽ������
// 											 double * dist,						// ����������	double dist[5]�����Ż����Ż����weng's���ϵ��
// 											 double * info = NULL,				// output:	runtime info, 5-vector
// 																				// info[0]:	the initial reprojection error
// 																				// info[1]:	the final reprojection error
// 																				// info[2]: final max gradient
// 																				// info[3]: the number of iterations
// 																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
// 											 double tau = 1.0E-3,				// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
// 											 int maxIter = 15,					// input:	the maximum number of iterations
// 											 double eps1 = 1.0E-8,				// input:	threshold
// 											 double eps2 = 1.0E-12				// input:	threshold
// 											 );
}

namespace SfM_ZZK
{

// 2015.11.03, a pair structure representing a 2D index
typedef std::pair<int,int> pair_ij;

// 2015.11.03, a pair structure representing a 2D index with a int value, often used in std::vector
typedef std::pair<pair_ij,int> pair_ij_k;

// 2015.11.03, a pair structure representing a 2D index with a double value, often used in std::vector
typedef std::pair<pair_ij,double> pair_ij_a;

// 2020.06.11, a pair structure representing the fundamental matrix and feature matches found between two images
typedef std::pair<cv::Matx33d, std::vector<DMatch>> pair_F_matches;

// 2020.06.21, a pair structure representing the fundamental matrix and feature matches found between two images
// and also the projective reconstruction world coordinates of those matched features
typedef std::pair<pair_F_matches, std::vector<Point3d>> pair_F_matches_pWrdPts;

// 2015.09.20, a map container contains all the pairwise matches between each image pair: {<image i,image j>, matches}, named after openMVG convention.
typedef std::map<std::pair<int, int>, std::vector<DMatch>> PairWiseMatches;

// 2020.06.11, a map container contains the fundamental matrix and all the pairwise matches found between each image pair: {<img i, img j>, F, matches}.
typedef std::map<std::pair<int, int>, pair_F_matches> PairWise_F_Matches;

// 2020.06.21, a map container contains the fundamental matrix, all the pairwise matches 
// and projective reconstruction world coordinates of those matches found between each image pair: {<img i, img j>, <<F, matches>, wrdpts>}
// the found matches and estimated wrdpts are supposed to be consistent with the estimated fundamental matrix F
typedef std::map<std::pair<int, int>, pair_F_matches_pWrdPts> PairWise_F_matches_pWrdPts;

// 2015.10.08, a map container to store a track: collection of {ImageId,FeatureId}
// The corresponding image points with their imageId and FeatureId.
// the reason why map instead of set is used here is that one track is supposed to contain only one feature for one image
// setting ImageId the first input of map can make it unique, whereas (0,1) and (0,2) are two different keys to set
// typedef std::map<int,int> OneTrack;
// 20151108 ���һλ���Ӧ���������ڵ㣨1��������㣨0��
typedef std::map<int,std::pair<int,int>> OneTrack;
// 20151128, �ϵ������켣�ṹ��û�б�ʾ outlier �� value
typedef std::map<int,int> OneTrack_old; 

// 20220201��<ImgID, <FeatID, [flag1, flag2, flag3,...]>>
// ֮ǰ��OneTrack��ÿ��������ֻ��1����־λ��ͨ�������������������ڸ�����ͶӰ�в���ֵ��������Ƿ��ж�Ϊ�ڵ㣨1��������㣨0��
// ����ÿ��������ӵ����һ����־λ���У���������������ӱ�־λ���������ٸı�����µ����ݽṹ��
// ���Ǵ�����Ҫ��ÿ����������һ����־λ�����Σ��ȷ�˵����Ҫָ��track��һ������Ƿ��ڵ��⣬����Ҫ��־λָ��track��һ������Ƿ�����С����ƥ��Ĳο����
typedef std::map<int, std::pair<int, std::vector<int>>> trackWithFlags;

// 2015.10.08, a map container to store all tracks: collection of {trackId, onetrack}
typedef std::map<int, OneTrack> MultiTracks;
// 20151128, �ϵ��������ƽṹ��û�б�ʾ outlier �� value
typedef std::map<int, OneTrack_old> MultiTracks_old;

// 20220201���µ����ݽṹ�������ÿ��track���ÿ����㶼����ӵ�ж����־λ��std::vector<int>��ʽ��
typedef std::map<int, trackWithFlags> MultiTracksWithFlags;

// 2015.11.04, a map container to store all the coordinates of the reconstructed tracks
typedef std::map<int, DeepVoid::CloudPoint> PointCloud;

// 2015.10.08, find all tracks based on Carl Olsson's algorithm in <Stable structure from motion for unordered image collections>
// global minimum weight version
// 2015.10.22, �ѱ���Ȩ�ص�{<I,J>, weight}�ṹ�� map �ĳ� vector���Ӷ������ڽ��й켣�ں�֮ǰ�ȶ�ͼ��԰�ƥ������������map������ֻ�ܰ�key����
// ֮��Ͱ���ͼ���ȥ�����ںϼ��ɣ�����Ҫ��ÿ�ζ���map�а�ֵ��������ͼ��ԣ����������ɾ��
void FindAllTracks_Olsson(const PairWiseMatches & map_matches,	// input:	all pairwise matches
						  MultiTracks & map_tracks				// output:	all the found tracks
						  );
// 20151128���ϵ��������ƽṹ
void FindAllTracks_Olsson(const PairWiseMatches & map_matches,	// input:	all pairwise matches
						  MultiTracks_old & map_tracks			// output:	all the found tracks
						  );

// 20200622�������µ����ݽṹ
void FindAllTracks_Olsson(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise fundamental matrix F, matches and projective reconstruction
						  MultiTracks & map_tracks									// output:	all the found tracks
						  );

// 20220201�������µ����ݽṹ
void FindAllTracks_Olsson(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise fundamental matrix F, matches and projective reconstruction
						  MultiTracksWithFlags & map_tracks,						// output:	all the found tracks
						  int nFlagImgpt = 1										// input:	�����켣�е�ÿ�����Ԥ����ٸ���־λ
						  );

// 2015.10.08, find all tracks based on Carl Olsson's algorithm in <Stable structure from motion for unordered image collections>
// original version ie local minimum weight version with random starting image
void FindAllTracks_Olsson_Original(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								   MultiTracks & map_tracks				// output:	all the found tracks
								   );
// 20151128���ϵ��������ƽṹ
void FindAllTracks_Olsson_Original(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								   MultiTracks_old & map_tracks				// output:	all the found tracks
								   );

// 2015.10.21, find all tracks that are connected components, and do not contain any conflict at all
// this method finds the ever least tracks
void FindAllTracks_Least(const PairWiseMatches & map_matches,	// input:	all pairwise matches
						 MultiTracks & map_tracks				// output:	all the found tracks
						 );
// 20151128���ϵ��������ƽṹ
void FindAllTracks_Least(const PairWiseMatches & map_matches,	// input:	all pairwise matches
						 MultiTracks_old & map_tracks				// output:	all the found tracks
						 );

// 2015.10.08, build the track length histogram
// 2015.10.21, and return the average track length
double BuildTrackLengthHistogram(const MultiTracks & map_tracks,	// input:	all the tracks
								 std::map<int,int> & hist			// output:	the histogram
							     );
// 20151128�������ϵĽṹ
double BuildTrackLengthHistogram(const MultiTracks_old & map_tracks,	// input:	all the tracks
								 std::map<int,int> & hist				// output:	the histogram
							     );

// 2015.10.08, build the track length histogram
// 2015.10.21, and return the average track length
double BuildTrackLengthHistogram(const vector<vector<Point2i>> & allTracks,	// input:	all the tracks
							     std::map<int,int> & hist						// output:	the histogram
							     );

// 2015.10.08, build the track length histogram
// 2015.10.21, and return the average track length
// 20220201�������½ṹ
double BuildTrackLengthHistogram(const MultiTracksWithFlags & map_tracks,	// input:	all the tracks
								 std::map<int,int> & hist					// output:	the histogram
							     );

// 20151103, zhaokunz, find image pair good for RO
// rank all image pairs according to the sum of track lengths
void RankImagePairs_TrackLengthSum(const PairWiseMatches & map_matches,	// input:	all pairwise matches
								   const MultiTracks & map_tracks,		// input:	all the tracks
								   vector<pair_ij_k> & pairs			// output:	pairs in descending order
								   );

// 20200622, zhaokunz, �����µ����ݽṹ
void RankImagePairs_TrackLengthSum(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise matches
								   const MultiTracks & map_tracks,								// input:	all the tracks
								   vector<pair_ij_k> & pairs									// output:	pairs in descending order
								   );

// 20220201��zhaokunz�������µ����ݽṹ
void RankImagePairs_TrackLengthSum(const PairWise_F_matches_pWrdPts & map_F_matches_pWrdPts,	// input:	all pairwise matches
								   const MultiTracksWithFlags & map_tracks,						// input:	all the tracks
								   vector<pair_ij_k> & pairs									// output:	pairs in descending order
								   );

// 20151108���¼���һ��ͼ���Ҫǰ�������µĵ�
void Triangulation_AddOneImg(PointCloud & map_pointcloud,				// output:	����
							 const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
							 const MultiTracks & map_tracks,			// input:	���е������켣
							 int idx_newImg,							// input:	����ɶ���ͼ���������
							 double thresh_inlier = 1.5					// input:	�����ж��ڵ����ͶӰ�в���ֵ
							 );

// 20151108���¼���һ��ͼ���Ҫǰ�������µĵ�
// 20220202�������½ṹMultiTracksWithFlags
void Triangulation_AddOneImg(PointCloud & map_pointcloud,				// output:	����
							 const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
							 const MultiTracksWithFlags & map_tracks,	// input:	���е������켣
							 int idx_newImg,							// input:	����ɶ���ͼ���������
							 double thresh_inlier = 1.5					// input:	�����ж��ڵ����ͶӰ�в���ֵ
							 );

// 20151111�����������켣�е�ǰ������ɶ����ͼ��ǰ�����ᣬÿ��������õ�һ�����꣬����ȡ֧�ּ������������
int Triangulation_AllImgs(PointCloud & map_pointcloud,				// output:	����
						   const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
						   const MultiTracks & map_tracks,			// input:	���е������켣
						   double thresh_inlier = 1.5				// input:	�����ж��ڵ����ͶӰ�в���ֵ
						   );

// 20151109�������ǰ����
void OutputPointCloud(CString strFile,							// input:	����ļ�·��
					  const PointCloud & map_pointcloud,		// input:	����
					  const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
					  const MultiTracks & map_tracks,			// input:	���е������켣
					  vector<DeepVoid::CloudPoint> & cloud,		// output:	�ϵĵ��ƽṹ��
					  int n_minInilier = 2						// input:	���ٵ��иø���ͼ��۲⵽�õ�
					  );

// 20220201�������ǰ����
void OutputPointCloud(CString strFile,							// input:	����ļ�·��
					  const PointCloud & map_pointcloud,		// input:	����
					  const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ��
					  const MultiTracksWithFlags & map_tracks,	// input:	���е������켣
					  vector<DeepVoid::CloudPoint> & cloud,		// output:	�ϵĵ��ƽṹ��
					  int n_minInilier = 2						// input:	���ٵ��иø���ͼ��۲⵽�õ�
					  );

// 20160405���ѵ�ǰ�ĵ��ƺ�ͼ���������Viz3d������
void Draw3DScene(viz::Viz3d & wnd3d,						// output:	3D��ʾ�Ĵ���
				 viz::WCloud & cld,
				 const PointCloud & map_pointcloud,		    // input:	����
				 const vector<DeepVoid::cam_data> & cams,	// input:	����ͼ�����
				 const MultiTracks & map_tracks,			// input:	���е������켣
				 int n_minInilier = 2						// input:	���ٵ��иø���ͼ��۲⵽�õ�
				 );

// 20151112,ͳ��ÿ�����Ƶ㱻�۲������ֱ��ͼ
double BuildCloudPointInlierHistogram(const PointCloud & map_pointcloud,	// output:	����
									  const MultiTracks & map_tracks,		// input:	���е������켣
									  std::map<int,int> & hist				// output:	the histogram
									  );

// 20151112,ͳ��ÿ�����Ƶ㱻�۲������ֱ��ͼ
// 20220202�������µ����ݽṹ
double BuildCloudPointInlierHistogram(const PointCloud & map_pointcloud,		// output:	����
									  const MultiTracksWithFlags & map_tracks,	// input:	���е������켣
									  std::map<int,int> & hist					// output:	the histogram
									  );

// optimize Ri based on Rotation Averaging using Newton-Raphson method
// ��ϸ�ο� Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> �е� Algorithm A2
void optim_nr_Ri_Govindu04(const vector<Matx33d> & vRijs,	// ���룺		���й۲�������ת����
						   const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
						   vector<Matx33d> & vRis,			// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
						   int idx_ref,						// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
						   int * nIter = NULL,				// �����		ʵ�ʵ�������
						   double * residue = NULL,			// �����		���յĲв��С
						   int max_iter = 32,				// ���룺		����������
						   double xEps = 1.0E-12,			// ���룺		��ֹ����
						   double fEps = 1.0E-12			// ���룺		��ֹ����
						   );

// optimize Ri based on Rotation Averaging using Newton-Raphson method
// ��ϸ�ο� Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> �е� Algorithm A2
void optim_nr_Ri(const vector<Matx33d> & vRijs,	// ���룺		���й۲�������ת����
				 const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
				 vector<Matx33d> & vRis,		// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
				 int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
				 int * nIter = NULL,			// �����		ʵ�ʵ�������
				 double * residue = NULL,		// �����		���յĲв��С
				 int max_iter = 32,				// ���룺		����������
				 double xEps = 1.0E-12,			// ���룺		��ֹ����
				 double fEps = 1.0E-12			// ���룺		��ֹ����
				 );

// optimize Ri based on Rotation Averaging using Gauss-Newton method
// ��ϸ�ο� Govindu 04 <Lie-algebraic averaging for globally consistent motion estimation> �е� Algorithm A2
void optim_gn_Ri(const vector<Matx33d> & vRijs,	// ���룺		���й۲�������ת����
				 const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
				 vector<Matx33d> & vRis,		// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
				 int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
				 int * nIter = NULL,			// �����		ʵ�ʵ�������
				 double * residue = NULL,		// �����		���յĲв��С
				 int max_iter = 32,				// ���룺		����������
				 double xEps = 1.0E-12,			// ���룺		��ֹ����
				 double fEps = 1.0E-12			// ���룺		��ֹ����
				 );

void optim_lm_Ri(const vector<Matx33d> & vRijs,	// ���룺		���й۲�������ת����
				 const vector<Point2i> & vijs,	// ���룺		ÿ�������ת�����ij�����ţ���0��ʼ
				 vector<Matx33d> & vRis,		// ����&�����	����ͼ��ĳ�ʼ��ת�����Լ��Ż������ת����
				 int idx_ref,					// ���룺		�����Ĳο�ͼ������������û�в����Ĳο�ͼ�񣬾����������Ϊ-1
				 double * info = NULL,			// output:	runtime info, 5-vector
												// info[0]:	the initial reprojection error
												// info[1]:	the final reprojection error
												// info[2]: final max gradient
												// info[3]: the number of iterations
												// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
				 double tau = 1.0E-3,			// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
				 int maxIter = 64,				// input:	the maximum number of iterations
				 double eps1 = 1.0E-8,			// input:	threshold
				 double eps2 = 1.0E-12			// input:	threshold
				 );

}

namespace SBA_ZZK
{

void optim_sparse_lm_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,					// ����������n���ռ������
								    const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								    vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
								    vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
								    const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								    const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								    const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								    const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
								    const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								    const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									vector<double> & vds,					// �����ÿ��������ͶӰ�в�
									double * info = NULL,					// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									double tau = 1.0E-3,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									int maxIter = 64,						// input:	the maximum number of iterations
									double eps1 = 1.0E-8,					// input:	threshold
									double eps2 = 1.0E-12					// input:	threshold
								    );

// 20151107��iteratively reweighted least squares
// �����ؼ�Ȩ�汾������ Huber Ȩ��
void optim_sparse_lm_wj_tj_XiYiZiWi_IRLS_Huber(vector<Point3d> & XYZs,					// ����������n���ռ������
											   const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
											   vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
											   vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
											   const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
											   const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
											   const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
											   vector<Matx22d> & covInvs,				// �����l�������������Э�������������(i)=wi*wi
											   const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
											   const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
											   const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
											   vector<double> & vds,					// �����ÿ��������ͶӰ�в�
											   double tc = 3.0,							// ���룺���� Huber Ȩ��ʱ�õ��ĳ���
											   double * info = NULL,					// output:	runtime info, 5-vector
																						// info[0]:	the initial reprojection error
																						// info[1]:	the final reprojection error
																						// info[2]: final max gradient
																						// info[3]: the number of iterations
																						// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
											   double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
											   int maxIter = 64,						// input:	the maximum number of iterations
											   double eps1 = 1.0E-8,					// input:	threshold
											   double eps2 = 1.0E-12					// input:	threshold
											   );

// 20170820��˫Ŀ�Ӿ��У��̶���ͼ��ͶӰ����Ϊ[I|0]��ϡ���Ż���ͼ��ͶӰ���� P
// �Լ���Ӱ�ؽ������� n ��������� [X Y 1 W]������ Z ����̶�Ϊ 1
// �ú�����ҪĿ���Ǵ��Ż���� P �зֽ�����м�����������Ļ������� F ��
void optim_sparse_lm_P_XiYiWi(Matx34d & P,									// ������������ǰ���Ƶ���ͼ����Ӱ�ռ��е�ͶӰ����
							  vector<Point3d> & XYWs,						// ����������n������XYW���꣬Z����Ĭ��Ϊ1
							  const vector<Point2d> & xysL,					// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������
							  const vector<Point2d> & xysR,					// ���룺n ���ռ������ͼ�й۲��������
							  const vector<Matx22d> & covInvsL,				// ���룺n ���ռ������ͼ���ο�ͼ���й۲��������Э�������������
							  const vector<Matx22d> & covInvsR,				// ���룺n ���ռ������ͼ�й۲��������Э�������������
							  vector<Point2d> & vds,						// �����n ���ռ������������ͼ�е���ͶӰ�в���
							  double * info = NULL,							// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,							// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 64,								// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,							// input:	threshold
							  double eps2 = 1.0E-12							// input:	threshold
							  );

void optim_sparse_lm_wj_tj_XiYiZi(vector<Point3d> & XYZs,					// ����������n���ռ������
								  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								  vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
								  vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
								  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
								  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								  double * info = NULL,						// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								  int maxIter = 64,							// input:	the maximum number of iterations
								  double eps1 = 1.0E-8,						// input:	threshold
								  double eps2 = 1.0E-12						// input:	threshold
								  );

void optim_sparse_lm_wj_cj_XiYiZiWi(vector<Point3d> & XYZs,					// ����������n���ռ������
								    const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								    vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
								    vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
								    const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								    const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								    const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								    const vector<Matx22d> & covInvs,		// ���룺l�������������Э�������������
								    const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								    const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								    const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									double * info = NULL,					// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									double tau = 1.0E-3,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									int maxIter = 64,						// input:	the maximum number of iterations
									double eps1 = 1.0E-8,					// input:	threshold
									double eps2 = 1.0E-12					// input:	threshold
								    );

void optim_sparse_lm_wj_cj_XiYiZi(vector<Point3d> & XYZs,					// ����������n���ռ������
								  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
								  vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
								  vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
								  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
								  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
								  double * info = NULL,						// output:	runtime info, 5-vector
																			// info[0]:	the initial reprojection error
																			// info[1]:	the final reprojection error
																			// info[2]: final max gradient
																			// info[3]: the number of iterations
																			// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
								  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
								  int maxIter = 64,							// input:	the maximum number of iterations
								  double eps1 = 1.0E-8,						// input:	threshold
								  double eps2 = 1.0E-12						// input:	threshold
								  );

void optim_sparse_lm_f_wj_tj_XiYiZiWi(vector<Point3d> & XYZs,					// ����������n���ռ������
									  vector<Matx33d> & Ks,						// ����������m��ͼ���ڲ�������
								      vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
								      vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
								      const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
								      const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
								      const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
								      const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
								      const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
								      const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
								      const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
									  double * info = NULL,						// output:	runtime info, 5-vector
																				// info[0]:	the initial reprojection error
																				// info[1]:	the final reprojection error
																				// info[2]: final max gradient
																				// info[3]: the number of iterations
																				// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
									  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
									  int maxIter = 64,							// input:	the maximum number of iterations
									  double eps1 = 1.0E-8,						// input:	threshold
									  double eps2 = 1.0E-12						// input:	threshold
								      );

// 20200607��iteratively reweighted least squares
// �����ؼ�Ȩ�汾������ Huber Ȩ��
void optim_sparse_lm_f_wj_tj_XiYiZiWi_IRLS_Huber(vector<Point3d> & XYZs,				// ����������n���ռ������
												 vector<Matx33d> & Ks,					// ����������m��ͼ���ڲ�������
												 vector<Matx33d> & Rs,					// ����������m��ͼ����ת����
												 vector<Matx31d> & ts,					// ����������m��ͼ��ƽ������
												 const vector<Matx<double,5,1>> & dists,// ���룺m��ͼ�����ϵ��
												 const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
												 const vector<Point2d> & xys,			// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
												 vector<Matx22d> & covInvs,				// �����l�������������Э�������������(i)=wi*wi
												 const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
												 const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
												 const SparseMat & ptrMat,				// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
												 vector<double> & vds,					// �����ÿ��������ͶӰ�в�
												 double tc = 3.0,						// ���룺���� Huber Ȩ��ʱ�õ��ĳ���
												 double * info = NULL,					// output:	runtime info, 5-vector
																						// info[0]:	the initial reprojection error
																						// info[1]:	the final reprojection error
																						// info[2]: final max gradient
																						// info[3]: the number of iterations
																						// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
												 double tau = 1.0E-3,					// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
												 int maxIter = 64,						// input:	the maximum number of iterations
												 double eps1 = 1.0E-8,					// input:	threshold
												 double eps2 = 1.0E-12					// input:	threshold
												 );

void optim_sparse_lm_f_wj_tj_XiYiZiWi_c_fixedBaseline(vector<Point3d> & XYZs,					// ����������n���ռ������
													  vector<Matx33d> & Ks,						// ����������m��ͼ���ڲ�������
													  vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
													  vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
													  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
													  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
													  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
													  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
													  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
													  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
													  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Aij��Bij��eij�ڸ��������д洢��λ������
													  int k_fixedBaseLine,						// ���룺������ͼ��֮��Ļ��߹̶�������k �ǲ�������һ��
													  int j_fixedBaseLine,						// ���룺������ͼ��֮��Ļ��߹̶�������j �ǵ�����һ��
													  double d_fixedBaseLine,					// ���룺����ͼ��֮��Ĺ̶����߳���
													  double * info = NULL,						// output:	runtime info, 5-vector
																								// info[0]:	the initial reprojection error
																								// info[1]:	the final reprojection error
																								// info[2]: final max gradient
																								// info[3]: the number of iterations
																								// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
													  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
													  int maxIter = 64,							// input:	the maximum number of iterations
													  double eps1 = 1.0E-8,						// input:	threshold
													  double eps2 = 1.0E-12						// input:	threshold
													  );

// 20150722��ÿ��������������ο�ͼ�����ֵ������
void optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// ����������n���ռ������
							  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							  vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
							  vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
							  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
							  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
							  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Wij�ڸ��������д洢��λ������
							  double * info = NULL,						// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 64,							// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,						// input:	threshold
							  double eps2 = 1.0E-12						// input:	threshold
							  );

// 20150728��ÿ��������������ο�ͼ�����ֵ������
// ÿ����������ȥ����һ�������������Ϊ�������룬�����ں����ڲ�����
void optim_sparse_lm_wj_tj_di(vector<Point3d> & XYZs,					// ����������n���ռ������
							  const vector<Matx33d> & Ks,				// ���룺m��ͼ���ڲ�������
							  vector<Matx33d> & Rs,						// ����������m��ͼ����ת����
							  vector<Matx31d> & ts,						// ����������m��ͼ��ƽ������
							  const vector<Matx<double,5,1>> & dists,	// ���룺m��ͼ�����ϵ��
							  const vector<int> & distTypes,			// ���룺m��ͼ������ϵ������
							  const vector<Point2d> & xys,				// ���룺l������ͼ���ϵ�������꣬������Ϊ m*n ��
							  const vector<Matx22d> & covInvs,			// ���룺l�������������Э�������������
							  const vector<Matx31d> & nxys,				// ���룺n��������������ο�ͼ��ϵ�еĹ۲����߷���Ҳ��ȥ����һ���������
							  const vector<int> & ri_j,					// ���룺n���������Ĳο�ͼ���������һ�� i ����Ӧһ�� j
							  const vector<uchar> & j_fixed,			// ���룺mά��������Щͼ��Ĳ����ǹ̶��ģ�j_fixed[j]=1�������ͼ�� j �����̶�����ô Aij = 0 �����κ����еĹ۲�� i ������
							  const vector<uchar> & i_fixed,			// ���룺nά��������Щ�ռ�������ǹ̶��ģ�i_fixed[i]=1��������� i ����̶�����ô Bij = 0 �����κι۲⵽�õ��ͼ�� j ������
							  const SparseMat & ptrMat,					// ���룺��һά�洢�����Ŀ��Ӿ���ptrMat(i,j)��������xij��xys�����д洢��λ���������Լ�Wij�ڸ��������д洢��λ������
							  double * info = NULL,						// output:	runtime info, 5-vector
																		// info[0]:	the initial reprojection error
																		// info[1]:	the final reprojection error
																		// info[2]: final max gradient
																		// info[3]: the number of iterations
																		// info[4]: the termination code, 0: small gradient; 1: small correction; 2: max iteration 
							  double tau = 1.0E-3,						// input:	The algorithm is not very sensitive to the choice of tau, but as a rule of thumb, one should use a small value, eg tau=1E-6 if x0 is believed to be a good approximation to real value, otherwise, use tau=1E-3 or even tau=1
							  int maxIter = 64,							// input:	the maximum number of iterations
							  double eps1 = 1.0E-8,						// input:	threshold
							  double eps2 = 1.0E-12						// input:	threshold
							  );

}