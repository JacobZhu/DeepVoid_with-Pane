#include "DeepVoid_MultiViewStereo.h"

// 20150209, 多视图立体视觉的总函数
void DeepVoid_MVS::MVS_Pyramid(const CString & path_output,						// input:	the path of output file folder
							   const vector<cam_data> & vCams,					// input:	all images' orientations
							   const vector<CString> & vPaths_imgs,				// input:	file paths of all input images
							   const vector<Mat> & vSilhouettes,				// input:	Silhouettes of all images
							   const vector<vector<int>> & vIdxSupports,		// input:	all images' support images' index
							   const vector<CloudPoint> & vClouds,				// input:	all point clouds, one for each input image
							   vector<Mat> & vDepths,							// output:	all generated depths
							   vector<Mat> & vHxs,								// output:	all generated depth gradients
							   vector<Mat> & vHys,								// output:	all generated depth gradients
							   vector<Mat> & vScores,							// output:	all scores, and -1 indicates an invalid estimate
							   vector<Mat> & vVisis,							// output:	all estimated visibilities within corresponding support image set
							   int size /*= 5*/,								// input:	the window size of the image patch, should be odd number
							   double angLimit /*= 80*/,						// input:	the angular range limit of the normal of every object point, in angle, not radian
							   int maxIter /*= 4*/,								// input:	maximum iteration
							   double factor /*= 0.5*/,
							   int nRandSamp /*= 6*/,
							   double thresh_ang /*= 90*/,						// input:	the normal angle constraint, if this value is 360, then no constraint at all
							   double thresh_ncc /*= 0.95*/,					// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
							   double P1 /*= 1.5*/,
							   double P2 /*= 0.1*/,
							   double thresh_imgpt_sigma /*= 1*/,
							   int nLevel /*= 4*/,								// input:	number of pyramid levels
							   int idxOutLevel /*= 0*/,							// input:	output final results at pyramid level idxOutLevel, default is level 0, i.e. the highest resolution level
							   double thresh_ratio /*= 0.001*/,
							   double thresh_WH_Ratio_removeSmall /*= 0.02*/,	// input:	
							   int maxIter_optim /*= 128*/,
							   double xEps /*= 1.0E-8*/,						// input: threshold
							   double fEps /*= 1.0E-6*/,						// input: threshold
							   bool bMPGCFinal /*= true*/						// input: use MPGC optimized or directly Most likely results
							   )
{
	int i,j,k,ii,jj,kk;

	int nImg = vCams.size();

	vDepths.clear(); vHxs.clear(); vHys.clear(); vScores.clear(); vVisis.clear();

	double tana = tan(angLimit*D2R);

	int nPix = pow(2.0,idxOutLevel);
	double nPix_1 = 1.0/nPix;

	CString strInfo;

	// get corresponding orientation matrix //////////////////////////////////////////////////////////////////////////
	vector<Matx33d> vRs,vKs; vector<Matx31d> vts;

	for (k=0;k<nImg;k++)
	{
		Matx33d mR,mK; Matx31d mt;

		for (i=0;i<3;i++)
		{
			for (j=0;j<3;j++)
			{
				mR(i,j) = vCams[k].R[i*3+j];
			}
		}
		mK(0,0) = vCams[k].fx;	mK(0,1) = vCams[k].s;  mK(0,2) = vCams[k].cx;
		mK(1,1) = vCams[k].fy;	mK(1,2) = vCams[k].cy; mK(2,2) = 1;
		mt(0) = vCams[k].t[0];	mt(1) = vCams[k].t[1]; mt(2) = vCams[k].t[2];

		vRs.push_back(mR);
		vKs.push_back(mK);
		vts.push_back(mt);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (k=0;k<nImg;k++)
	{
		int n_spt = vIdxSupports[k].size();

		Matx33d mK0 = vKs[k];
		Matx33d mR0 = vRs[k];
		Matx31d mt0 = vts[k];

		CString path = vPaths_imgs[k];

		Mat img0 = imread(path.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		int h = img0.rows;
		int w = img0.cols;

		vector<Matx33d> vKs_spt, vRs_spt; vector<Matx31d> vts_spt;
		vector<Mat> vImgs_spt;

		for (kk=0;kk<n_spt;kk++)
		{
			int idx_spt = vIdxSupports[k][kk];
			vKs_spt.push_back(vKs[idx_spt]);
			vRs_spt.push_back(vRs[idx_spt]);
			vts_spt.push_back(vts[idx_spt]);

			path = vPaths_imgs[idx_spt];

			Mat img = imread(path.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

			vImgs_spt.push_back(img);
		}

		Mat mDepth_final, mHx_final, mHy_final, mScore_final, mVisi_final, img0_outLevel;
		SurfaceMapEstimation_oneImg_Pyramid(mK0,mR0,mt0,img0,vKs_spt,vRs_spt,vts_spt,vImgs_spt,vClouds,
			path_output,mDepth_final,mHx_final,mHy_final,mScore_final,mVisi_final,img0_outLevel,size,angLimit,maxIter,factor,nRandSamp,
			thresh_ang,thresh_ncc,P1,P2,thresh_imgpt_sigma,nLevel,idxOutLevel,maxIter_optim,xEps,fEps,bMPGCFinal);

		// 输出结果至指定文件夹
		CString str;
		str.Format("%03d depth map at pyramid level %03d.bmp", k, idxOutLevel);
		SaveParaField2Img(path_output+str, mDepth_final);

		str.Format("%03d score map at pyramid level %03d.bmp", k, idxOutLevel);
		SaveScoreField2Img(path_output+str, mScore_final);

		Matx33d mK0_samp;
		mK0_samp(0,0) = mK0(0,0)*nPix_1;
		mK0_samp(1,1) = mK0(1,1)*nPix_1;
		mK0_samp(0,2) = mK0(0,2)*nPix_1;
		mK0_samp(1,2) = mK0(1,2)*nPix_1;
		mK0_samp(0,1) = mK0(0,1);
		mK0_samp(2,2) = 1;

		double fx0l_1 = 1.0/mK0_samp(0,0);
		double fy0l_1 = 1.0/mK0_samp(1,1);

		Mat mNormColor;
		GetNormColorField(mK0_samp,mR0,mt0,fx0l_1,fy0l_1,mDepth_final,mHx_final,mHy_final,mNormColor);
		str.Format("%03d normal map at pyramid level %03d.bmp", k, idxOutLevel);
		imwrite((path_output+str).GetBuffer(), mNormColor);

		str.Format("%03d point cloud at pyramid level %03d.txt", k, idxOutLevel);
		OutputPointCloud(path_output+str,mK0_samp,mR0,mt0,fx0l_1,fy0l_1,img0_outLevel,
			mDepth_final,mHx_final,mHy_final,mScore_final);

		// draw visibility w.r.t each support image
		vector<Mat> vVisi_map;
		for (kk=0;kk<n_spt;kk++)
		{
			Mat mtmp(mVisi_final.rows,mVisi_final.cols,CV_8UC3);
			vVisi_map.push_back(mtmp);
		}

		for (i=0;i<mVisi_final.rows;i++)
		{
			for (j=0;j<mVisi_final.cols;j++)
			{
				uchar visi = mVisi_final.at<uchar>(i,j);

				vector<bool> vbools;
				InterpVisiVector_uchar(visi, vbools);

				for (kk=0;kk<n_spt;kk++)
				{
					if (vbools[kk])
					{
						vVisi_map[kk].at<Vec3b>(i,j).val[0] = img0_outLevel.at<Vec3b>(i,j).val[0];
						vVisi_map[kk].at<Vec3b>(i,j).val[1] = img0_outLevel.at<Vec3b>(i,j).val[1];
						vVisi_map[kk].at<Vec3b>(i,j).val[2] = img0_outLevel.at<Vec3b>(i,j).val[2];
					} 
					else
					{
						vVisi_map[kk].at<Vec3b>(i,j).val[0] = 0;
						vVisi_map[kk].at<Vec3b>(i,j).val[1] = 0;
						vVisi_map[kk].at<Vec3b>(i,j).val[2] = 255;
					}
				}
			}
		}

		for (kk=0;kk<n_spt;kk++)
		{
			str.Format("%03d visi map at pyramid level %03d wrt support image %03d.bmp", k, idxOutLevel, vIdxSupports[k][kk]);
			imwrite((path_output+str).GetBuffer(), vVisi_map[kk]);
		}

		str.Format("%03d image down-sampled at pyramid level %03d.bmp", k, idxOutLevel);
		imwrite((path_output+str).GetBuffer(), img0_outLevel);

		// output to files
		str.Format("%03d depth map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_depth = fopen(path_output+str, "w");
		str.Format("%03d hx map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_hx = fopen(path_output+str, "w");
		str.Format("%03d hy map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_hy = fopen(path_output+str, "w");
		str.Format("%03d score map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_score = fopen(path_output+str, "w");
		str.Format("%03d visi map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_visi = fopen(path_output+str, "w");

		// at the same time evaluate all parameters
		for (i=0;i<img0_outLevel.rows;i++)
		{
			for (j=0;j<img0_outLevel.cols;j++)
			{
				fprintf(file_depth, "%.16f	", mDepth_final.at<float>(i,j));
				fprintf(file_hx, "%.16f	", mHx_final.at<float>(i,j));
				fprintf(file_hy, "%.16f	", mHy_final.at<float>(i,j));
				fprintf(file_score, "%.16f	", mScore_final.at<float>(i,j));
				fprintf(file_visi, "%d	", mVisi_final.at<uchar>(i,j));
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
			fprintf(file_visi, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);
	}

	// depth consistency check
	for (k=0;k<nImg;k++)
	{
		strInfo.Format("check image %03d", k);
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		int n_spt = vIdxSupports[k].size();

		Matx33d mK0 = vKs[k];
		Matx33d mR0 = vRs[k];
		Matx31d mt0 = vts[k];

		Matx33d mK0_samp;
		mK0_samp(0,0) = mK0(0,0)*nPix_1;
		mK0_samp(1,1) = mK0(1,1)*nPix_1;
		mK0_samp(0,2) = mK0(0,2)*nPix_1;
		mK0_samp(1,2) = mK0(1,2)*nPix_1;
		mK0_samp(0,1) = mK0(0,1);
		mK0_samp(2,2) = 1;

		double fx0l_1 = 1.0/mK0_samp(0,0);
		double fy0l_1 = 1.0/mK0_samp(1,1);

		double ratio_disc = 2*tana/(mK0_samp(0,0)+mK0_samp(1,1)); // ratio = tan(beta)/f = tan(beta)/((fx+fy)*0.5) = 2*tan(beta)/(fx+fy)

		CString strFile;
		strFile.Format("%03d image down-sampled at pyramid level %03d.bmp", k, idxOutLevel);

		Mat img0 = imread((path_output+strFile).GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		int h0 = img0.rows;
		int w0 = img0.cols;

		Mat mDepth0(h0,w0,CV_32FC1), mHx0(h0,w0,CV_32FC1), mHy0(h0,w0,CV_32FC1), mScore0(h0,w0,CV_32FC1), mVisi0(h0,w0,CV_8UC1);

		// read-in the reference fields
		strFile.Format("%03d depth map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_depth = fopen(path_output+strFile, "r");
		strFile.Format("%03d hx map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_hx = fopen(path_output+strFile, "r");
		strFile.Format("%03d hy map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_hy = fopen(path_output+strFile, "r");
		strFile.Format("%03d score map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_score = fopen(path_output+strFile, "r");
		strFile.Format("%03d visi map at pyramid level %03d.txt", k, idxOutLevel);
		FILE * file_visi = fopen(path_output+strFile, "r");

		// at the same time evaluate all parameters
		for (i=0;i<h0;i++)
		{
			for (j=0;j<w0;j++)
			{
				double depth,hx,hy,score;
				int tmp;
				fscanf(file_depth, "%lf	", &depth);
				fscanf(file_hx, "%lf	", &hx);
				fscanf(file_hy, "%lf	", &hy);
				fscanf(file_score, "%lf	", &score);

				mDepth0.at<float>(i,j) = depth;
				mHx0.at<float>(i,j) = hx;
				mHy0.at<float>(i,j) = hy;
				mScore0.at<float>(i,j) = score;

				fscanf(file_visi, "%d	", &tmp);
				mVisi0.at<uchar>(i,j) = (uchar)tmp;
			}
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

		vector<Matx33d> vKs_spt, vRs_spt; vector<Matx31d> vts_spt;

		vector<Mat> vDepths_spt, vHxs_spt, vHys_spt;

		for (kk=0;kk<n_spt;kk++)
		{
			int idx_spt = vIdxSupports[k][kk];

			vRs_spt.push_back(vRs[idx_spt]);
			vts_spt.push_back(vts[idx_spt]);

			Matx33d mK_spt = vKs[idx_spt];

			Matx33d mK_samp;
			mK_samp(0,0) = mK_spt(0,0)*nPix_1;
			mK_samp(1,1) = mK_spt(1,1)*nPix_1;
			mK_samp(0,2) = mK_spt(0,2)*nPix_1;
			mK_samp(1,2) = mK_spt(1,2)*nPix_1;
			mK_samp(0,1) = mK_spt(0,1);
			mK_samp(2,2) = 1;

			vKs_spt.push_back(mK_samp);

			strFile.Format("%03d image down-sampled at pyramid level %03d.bmp", idx_spt, idxOutLevel);
			
			Mat img = imread((path_output+strFile).GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

			int h = img.rows;
			int w = img.cols;

			Mat mDepth(h,w,CV_32FC1), mHx(h,w,CV_32FC1), mHy(h,w,CV_32FC1);

			// read-in the support fields
			strFile.Format("%03d depth map at pyramid level %03d.txt", idx_spt, idxOutLevel);
			file_depth = fopen(path_output+strFile, "r");
			strFile.Format("%03d hx map at pyramid level %03d.txt", idx_spt, idxOutLevel);
			file_hx = fopen(path_output+strFile, "r");
			strFile.Format("%03d hy map at pyramid level %03d.txt", idx_spt, idxOutLevel);
			file_hy = fopen(path_output+strFile, "r");

			// at the same time evaluate all parameters
			for (i=0;i<h;i++)
			{
				for (j=0;j<w;j++)
				{
					double depth,hx,hy;
					fscanf(file_depth, "%lf	", &depth);
					fscanf(file_hx, "%lf	", &hx);
					fscanf(file_hy, "%lf	", &hy);

					mDepth.at<float>(i,j) = depth;
					mHx.at<float>(i,j) = hx;
					mHy.at<float>(i,j) = hy;
				}
			}
			fclose(file_depth);
			fclose(file_hx);
			fclose(file_hy);

			vDepths_spt.push_back(mDepth);
			vHxs_spt.push_back(mHx);
			vHys_spt.push_back(mHy);
		}

		DepthConsistencyCheck(mK0_samp,mR0,mt0,mDepth0,mVisi0,mScore0,vKs_spt,vRs_spt,vts_spt,vDepths_spt,vHxs_spt,vHys_spt,thresh_ratio);

		strFile.Format("%03d score map at pyramid level %03d after depth consistency check.bmp", k, idxOutLevel);
		SaveScoreField2Img(path_output+strFile, mScore0);

		// and remove small depth regions
		double area_tmp = w0*h0*thresh_WH_Ratio_removeSmall*thresh_WH_Ratio_removeSmall;
		int area_pix = floor_fast(area_tmp)+1;
		RemoveSmallDepthRegions_4(mK0_samp,mR0,mt0,mDepth0,mScore0,ratio_disc,area_pix);

		strFile.Format("%03d score map at pyramid level %03d after removing small regions.bmp", k, idxOutLevel);
		SaveScoreField2Img(path_output+strFile, mScore0);

		strFile.Format("%03d point cloud at pyramid level %03d final.txt", k, idxOutLevel);
		OutputPointCloud(path_output+strFile,mK0_samp,mR0,mt0,fx0l_1,fy0l_1,img0,mDepth0,mHx0,mHy0,mScore0);


		// further using silhouettes to filter more outliers
// 		VisualHullConstraint(k,vKs,vRs,vts,vSilhouettes,mDepth0,mScore0,2);
// 
// 		strFile.Format("%03d score map after enforcing silhouette constraint.bmp", k);
// 		SaveScoreField2Img(path_output+strFile, mScore0);
// 
// 		strFile.Format("%03d point cloud final after enforcing silhouette constraint.txt", k);
// 		OutputPointCloud(path_output+strFile,mK0,mR0,mt0,fx0_1,fy0_1,img0,mDepth0,mHx0,mHy0,mScore0);
	}
}

// 20150207, zhaokunz, pyramid
// 目标是通过采用金字塔机制（应对若纹理、减少计算量）为后续的多图 MPGC 提供合适的深度和法向的优化初值，以及每个像素相对于每幅支持图像的可见性
// 函数可以输出低于源图像分辨率的深度及法向图，并不一定非得是源尺寸
// 当然如果输出低分辨率的深度及法向图，输出的深度及法向图的内参数也是成比例改变的
void DeepVoid_MVS::SurfaceMapEstimation_oneImg_Pyramid(const Matx33d & mK0,						// input:	interior matrix of the reference image
													   const Matx33d & mR0,						// input:	rotation matrix of the reference image
													   const Matx31d & mt0,						// input:	translation vectors of the reference image
													   const Mat & img0,						// input:	the reference image
													   const vector<Matx33d> & vKs,				// input:	interior matrix of all support images
													   const vector<Matx33d> & vRs,				// input:	rotation matrix of all support images
													   const vector<Matx31d> & vts,				// input:	translation vectors of all support images
													   const vector<Mat> & vImgs,				// input:	all support images
													   const vector<CloudPoint> & clouds,		// input:	the given sparse cloud points
													   CString path_output,						// input:	the path of output file folder
													   Mat & mDepth_final,						// output:	final depth map，为输出层的分辨率
													   Mat & mHx_final,							// output:	final hx map，为输出层的分辨率
													   Mat & mHy_final,							// output:	final hy map，为输出层的分辨率
													   Mat & mScore_final,						// output:	final score map，为输出层的分辨率
													   Mat & mVisi_final,						// output:	final visi map，为输出层的分辨率
													   Mat & img0_outLevel,						// output:	the low resolution reference image in the out level
													   int size /*= 5*/,						// input:	the window size of the image patch, should be odd number
													   double angLimit /*= 80*/,				// input:	the angular range limit of the normal of every object point, in angle, not radian
													   int maxIter /*= 4*/,						// input:	maximum iteration
													   double factor /*= 0.5*/,					// input:	PatchMatch的时候随机采样的半径缩小因子
													   int nRandSamp /*= 6*/,					// input:	PatchMatch中在随机搜索环节进行随机采样的次数
													   double thresh_ang /*= 90*/,				// input:	the normal angle constraint, if this value is 360, then no constraint at all
													   double thresh_ncc /*= 0.95*/,			// input:	the ncc threshold after patchmatch, ncc value is supposed to be very high if it's matched correctly after patchmatch
													   double P1 /*= 1.5*/,						// input:	MRF时候对深度加的惩罚因子
													   double P2 /*= 0.1*/,						// input:	MRF时候对法向加的惩罚因子
													   double thresh_imgpt_sigma /*= 1*/,		// input:	经验的像点匹配精度，拓展可见性时需要用到
													   int nLevel /*= 4*/,						// input:	number of pyramid levels
													   int idxOutLevel /*= 0*/,					// input:	output final results at pyramid level idxOutLevel, default is level 0, i.e. the highest resolution level
													   int maxIter_optim /*= 128*/,				
													   double xEps /*= 1.0E-8*/,				// input: threshold
													   double fEps /*= 1.0E-6*/,				// input: threshold
													   bool bMPGCFinal /*= true*/				// input: use MPGC optimized or directly Most likely results
													   )
{
	int i,j,k,l;

 	int n_spt = vKs.size();

	vector<Matx33d> vK0s;
	vK0s.push_back(mK0);

	vector<Mat> vPyramidImgs;
//	buildPyramid(img0, vPyramidImgs, nLevel-1); // build gaussian pyramid OpenCV
	vPyramidImgs.push_back(img0);

	// 1. 生成当前参考图像的金字塔图集及对应的内参数
	// 第0层为原始图像（最大分辨率），金字塔顶层为分辨率最低的图像
	for (l=1;l<nLevel;l++)
	{
		int nPix = pow(2.0,l);

		Mat img_samp;
		DownSample(img0, img_samp, nPix);
		vPyramidImgs.push_back(img_samp);

		double nPix_1 = 1.0/nPix;

		Matx33d mK0_samp;
		mK0_samp(0,0) = mK0(0,0)*nPix_1;
		mK0_samp(1,1) = mK0(1,1)*nPix_1;
		mK0_samp(0,2) = mK0(0,2)*nPix_1;
		mK0_samp(1,2) = mK0(1,2)*nPix_1;
		mK0_samp(0,1) = mK0(0,1);
		mK0_samp(2,2) = 1;
		vK0s.push_back(mK0_samp);
	}
	//////////////////////////////////////////////////////////////////////////

	// 2. 利用金字塔顶层（最小分辨率）参考图像和其所有N个原始尺寸支持图像来做PatchMatch (PM)，得到N个最小分辨率的参考图像深度图
	vector<Mat> vDepths_l,vHxs_l,vHys_l,vScores_l;

	PatchMatch_Binocular_3DPropagation(vK0s[nLevel-1],mR0,mt0,vPyramidImgs[nLevel-1],vKs,vRs,vts,vImgs,clouds,
		vDepths_l,vHxs_l,vHys_l,vScores_l,size,angLimit,maxIter,factor,nRandSamp);
	//////////////////////////////////////////////////////////////////////////

	// 3. 通过双目MPGC来优化上述PM得到的N个最小分辨率深度图
	for (k=0;k<n_spt;k++)
	{
		MPGC_Binocular_20141215(vK0s[nLevel-1],mR0,mt0,vPyramidImgs[nLevel-1],vKs[k],vRs[k],vts[k],vImgs[k],
			vDepths_l[k],vHxs_l[k],vHys_l[k],vScores_l[k],size,size,maxIter_optim,xEps,fEps);
	}

	vector<vector<Mat>> vDepths_allLevels,vHxs_allLevels,vHys_allLevels,vScores_allLevels;
	//////////////////////////////////////////////////////////////////////////

	// 把该N个最小分辨率深度图存入容器，这些容器里存储的参数场是按倒金字塔排布的，即最小分辨率的参数场位于最底层，高分辨率的位于高层
	vDepths_allLevels.push_back(vDepths_l);
	vHxs_allLevels.push_back(vHxs_l);
	vHys_allLevels.push_back(vHys_l);
	vScores_allLevels.push_back(vScores_l);

	// 4. 低层（分辨率增加）参数场利用上一层估计优化的参数场升采样插值出来的等分辨率参数场作为初值，通过双目MPGC来进一步优化一下，并存入容器
	// 最低到用户指定的输出层
	for (l=(nLevel-2);l>=idxOutLevel;l--)
	{
		for (k=0;k<n_spt;k++)
		{
			AugmentField_Interp(vDepths_allLevels[nLevel-2-l][k],vDepths_l[k],vPyramidImgs[l].cols,vPyramidImgs[l].rows,2);
			AugmentField_Interp(vHxs_allLevels[nLevel-2-l][k],vHxs_l[k],vPyramidImgs[l].cols,vPyramidImgs[l].rows,2,0.5);
			AugmentField_Interp(vHys_allLevels[nLevel-2-l][k],vHys_l[k],vPyramidImgs[l].cols,vPyramidImgs[l].rows,2,0.5);
			AugmentField_Interp(vScores_allLevels[nLevel-2-l][k],vScores_l[k],vPyramidImgs[l].cols,vPyramidImgs[l].rows,2);
		}

// 		PatchMatch_Binocular_3DPropagation_givenInit(vK0s[l],mR0,mt0,vPyramidImgs[l],vKs,vRs,vts,vImgs,clouds,
// 			vDepths_l,vHxs_l,vHys_l,vScores_l,size,angLimit,maxIter,factor,0.01,2);

		for (k=0;k<n_spt;k++)
		{
			MPGC_Binocular_20141215(vK0s[l],mR0,mt0,vPyramidImgs[l],vKs[k],vRs[k],vts[k],vImgs[k],
				vDepths_l[k],vHxs_l[k],vHys_l[k],vScores_l[k],size,size,maxIter_optim,xEps,fEps);
		}

		vDepths_allLevels.push_back(vDepths_l);
		vHxs_allLevels.push_back(vHxs_l);
		vHys_allLevels.push_back(vHys_l);
		vScores_allLevels.push_back(vScores_l);
	}
	//////////////////////////////////////////////////////////////////////////

	int w_out = vPyramidImgs[idxOutLevel].cols;
	int h_out = vPyramidImgs[idxOutLevel].rows;

	// 5. 从各低于输出层分辨率的层传递candidates过来，这里的容器也是按倒金字塔形来排布的
	vector<Mat> vDepths_all, vHxs_all, vHys_all, vScores_all;
	for (l=(nLevel-1);l>idxOutLevel;l--)
	{
		int l_diff = l-idxOutLevel;

		int nPix = pow(2.0, l_diff);

		double nPix_1 = 1.0/nPix;

		for (k=0;k<n_spt;k++)
		{
			Mat mDepth, mHx, mHy, mScore;
			AugmentField_Interp(vDepths_allLevels[nLevel-1-l][k],mDepth,w_out,h_out,nPix);
			AugmentField_Interp(vHxs_allLevels[nLevel-1-l][k],mHx,w_out,h_out,nPix,nPix_1);
			AugmentField_Interp(vHys_allLevels[nLevel-1-l][k],mHy,w_out,h_out,nPix,nPix_1);
			AugmentField_Interp(vScores_allLevels[nLevel-1-l][k],mScore,w_out,h_out,nPix);

			vDepths_all.push_back(mDepth);
			vHxs_all.push_back(mHx);
			vHys_all.push_back(mHy);
			vScores_all.push_back(mScore);
		}
	}
	// 这里是把输出层的参数场存入容器
	for (k=0;k<n_spt;k++)
	{
		vDepths_all.push_back(vDepths_allLevels[nLevel-1-idxOutLevel][k]);
		vHxs_all.push_back(vHxs_allLevels[nLevel-1-idxOutLevel][k]);
		vHys_all.push_back(vHys_allLevels[nLevel-1-idxOutLevel][k]);
		vScores_all.push_back(vScores_allLevels[nLevel-1-idxOutLevel][k]);
	}
	//////////////////////////////////////////////////////////////////////////

	// 6. 把所有候选参数场（共有N*L个候选场，L为从最低分辨率层到输出层的总层数）中法向不合理以及相似性度量过低的候选参数置为无效，使其不能参与后续的MRF融合
	for (k=0;k<vDepths_all.size();k++)
	{
		int idxSpt = k%n_spt;

		// then invalidate those estimates whose normals are not reasonable and whose scores are smaller than certain threshold
		InvalidatePixels_byNormal(vK0s[idxOutLevel],mR0,mt0,vKs[idxSpt],vRs[idxSpt],vts[idxSpt],
			vDepths_all[k],vHxs_all[k],vHys_all[k],vScores_all[k],thresh_ang,thresh_ncc);
	}
	//////////////////////////////////////////////////////////////////////////////////

	Mat mSel;
	Mat mDepth_ML, mHx_ML, mHy_ML, mScore_ML;
	// 7. 基于MRF模型，从所有候选参数中为每个像素寻找一个最似然的参数值
	Extract_MRF_ncc_d_n_DP_withInvalids(vK0s[idxOutLevel],mR0,mt0,vDepths_all,vHxs_all,vHys_all,vScores_all,mSel,mDepth_ML,mHx_ML,mHy_ML,mScore_ML,P1,P2);
	//////////////////////////////////////////////////////////////////////////

	// 8. 通过聚类拓展每个像素相对于各参考图像的可见性
	double mul_factor = pow(2.0, idxOutLevel);
	AugmentVisibility_basedonMostLikelyDepthandNormals_SURE_MultiScales(vK0s[idxOutLevel],mR0,mt0,vKs,vRs,vts,
		vDepths_all,vHxs_all,vHys_all,vScores_all,mSel,mVisi_final,thresh_imgpt_sigma*mul_factor,90);
	//////////////////////////////////////////////////////////////////////////

	// 9. 多图MPGC，像素的所有可见图像（冗余观测）都参与到MPGC当中来优化深度和法向估计
	Mat mDepth_MPGC, mHx_MPGC, mHy_MPGC, mScore_MPGC;
	MPGC_20150207(vK0s[idxOutLevel],mR0,mt0,vPyramidImgs[idxOutLevel],vKs,vRs,vts,vImgs,mDepth_ML,mHx_ML,mHy_ML,mScore_ML,mVisi_final,
		mDepth_MPGC,mHx_MPGC,mHy_MPGC,mScore_MPGC,size,size,maxIter_optim,xEps,fEps);
	//////////////////////////////////////////////////////////////////////////

	// 10. 选择最终可以输出的输出层参数场，到底是MRF融合后的，还是MPGC多图优化后的
	if (bMPGCFinal)
	{
		mDepth_final = mDepth_MPGC.clone();
		mHx_final    = mHx_MPGC.clone();
		mHy_final    = mHy_MPGC.clone();
		mScore_final = mScore_MPGC.clone();
	} 
	else
	{
		mDepth_final = mDepth_ML.clone();
		mHx_final    = mHx_ML.clone();
		mHy_final    = mHy_ML.clone();
		mScore_final = mScore_ML.clone();
	}

	img0_outLevel = vPyramidImgs[idxOutLevel].clone();
}

// Downsampling
void DeepVoid_MVS::DownSample(const Mat & img,	// input:	original image
							  Mat & img_samp,	// output:	down sampled image
							  int n /*= 2*/		// input:	sample one pixel every n pixels
							  )
{
	int i,j;

	int nChannel = img.channels();

	int w_new = img.cols/n;
	int h_new = img.rows/n;

	if (nChannel==1)
	{
		img_samp = Mat(h_new,w_new,CV_8UC1); // graylevel

		for (i=0;i<h_new;i++)
		{
			for (j=0;j<w_new;j++)
			{
				img_samp.at<uchar>(i,j)=img.at<uchar>(i*n,j*n);
			}
		}
	} 
	else
	{
		img_samp = Mat(h_new,w_new,CV_8UC3); // color

		for (i=0;i<h_new;i++)
		{
			for (j=0;j<w_new;j++)
			{
				img_samp.at<Vec3b>(i,j)=img.at<Vec3b>(i*n,j*n);
			}
		}
	}
}

// 20141013, self-contained version, conduct patchmatch for reference image with every support image at each time without view propagation
// with propagation between different layers.
void DeepVoid_MVS::PatchMatch_Binocular_3DPropagation(const Matx33d & mK0,				// input:	interior matrix of the reference image
													  const Matx33d & mR0,				// input:	rotation matrix of the reference image
													  const Matx31d & mt0,				// input:	translation vectors of the reference image
													  const Mat & img0,					// input:	the reference image
													  const vector<Matx33d> & vKs,		// input:	interior matrix of all support images
													  const vector<Matx33d> & vRs,		// input:	rotation matrix of all support images
													  const vector<Matx31d> & vts,		// input:	translation vectors of all support images
													  const vector<Mat> & vImgs,		// input:	all support images
													  const vector<CloudPoint> & clouds,// input:	the given sparse cloud points
													  vector<Mat> & vDepths,			// output:	all the depth maps
													  vector<Mat> & vHxs,				// output:	all the hx maps
													  vector<Mat> & vHys,				// output:	all the hy maps
													  vector<Mat> & vScores,			// output:	all the score maps
													  int size /*= 5*/,					// input:	the window size of the image patch, should be odd number
													  double angLimit /*= 80*/,			// input:	the angular range limit of the normal of every object point, in angle, not radian
													  int maxIter /*= 4*/,				// input:	maximum iteration
													  double factor /*= 0.5*/,
													  int nRandSamp /*= 6*/
													  )
{
	int i,j,k,ii,jj,kk,rk;

	vDepths.clear(); vHxs.clear(); vHys.clear(); vScores.clear();

	int n_spt = vKs.size();
	int n_cloud = clouds.size();

	int imgWidth = img0.cols;
	int imgHeight = img0.rows;
	int wndSizeHalf = (size-1)/2;

	double fx0_1 = 1/mK0(0,0);
	double fy0_1 = 1/mK0(1,1);

	vector<double> vfx_1, vfy_1;
	for (k=0;k<n_spt;k++)
	{
		double fx_1 = 1/vKs[k](0,0);
		double fy_1 = 1/vKs[k](1,1);

		vfx_1.push_back(fx_1);
		vfy_1.push_back(fy_1);
	}

	// determine maximal and minimal parameters //////////////////////////////////////////////////////////////////////////
	double tana = tan(angLimit*D2R);

	double fx0 = mK0(0,0); double fy0 = mK0(1,1);

	double f=(fx0+fy0)*0.5;
	double tana_f = tana/f;

	double depth_min;
	double depth_max;

	for (jj=0;jj<n_cloud;jj++)
	{
		Matx31d mX;

		mX(0) = clouds[jj].m_pt.x;
		mX(1) = clouds[jj].m_pt.y;
		mX(2) = clouds[jj].m_pt.z;

		mX = mR0*mX+mt0;

		double depth = mX(2);

		if (jj == 0)
		{
			depth_min = depth;
			depth_max = depth;
		} 
		else
		{
			if (depth<depth_min)
			{
				depth_min = depth;
			}
			if (depth>depth_max)
			{
				depth_max = depth;
			}
		}
	}

	double ratio_ext = 0.05; // the range of depth is extended on both directions by 5%

	double depth_ext = (depth_max-depth_min)*ratio_ext;

	depth_max += depth_ext;

	double tmp = depth_min - depth_ext;

	if (tmp<0)
	{
		depth_min = 0;
	} 
	else
	{
		depth_min = tmp;
	}

	double h_max = tana_f * depth_max;
	double h_min = -h_max;

	double d_range = depth_max-depth_min;
	double h_range = h_max-h_min;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CString strInfo;
	// initialize all depth, hx, hy maps randomly
	for (k=0;k<n_spt;k++)
	{
		Mat mDepth, mHx, mHy, mScore(imgHeight, imgWidth, CV_32FC1);
		InitRndField(mK0,mR0,mt0,depth_min,depth_max,angLimit,imgWidth,imgHeight,mDepth,mHx,mHy);
		vDepths.push_back(mDepth);
		vHxs.push_back(mHx);
		vHys.push_back(mHy);

		// at the same time evaluate all parameters
		for (i=0;i<imgHeight;i++)
		{
			strInfo.Format("evaluate row %04d with image %02d", i, k);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			for (j=0;j<imgWidth;j++)
			{
				int i_real, j_real, nVisi;
				MakeSureNotOutBorder(j,i,j_real,i_real,wndSizeHalf,imgWidth,imgHeight);

				double depth = vDepths[k].at<float>(i,j);
				double hx = vHxs[k].at<float>(i,j);
				double hy = vHys[k].at<float>(i,j);

				double score, angle;
				CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth,hx,hy,score,angle,size);

				mScore.at<float>(i,j) = score;
			}
		}

		vScores.push_back(mScore);
	}

	// start propagation
	for (ii=0;ii<maxIter;ii++)
	{
		if (ii%2 == 0)
		{
			for (k=0;k<n_spt;k++)
			{
				for (i=0;i<imgHeight;i++)
				{
					strInfo.Format("evaluate row %04d with image %02d in iteration %02d", i, k, ii);
					theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

					for (j=0;j<imgWidth;j++)
					{
						int i_real, j_real, nVisi;
						MakeSureNotOutBorder(j,i,j_real,i_real,wndSizeHalf,imgWidth,imgHeight);

						vector<double> vDepths_candidates, vHx_candidates, vHy_candidates;
						vector<double> vScores_candidates;

						// first evaluate current parameter set for (i,j)
						double depth0 = vDepths[k].at<float>(i,j);
						double hx0 = vHxs[k].at<float>(i,j);
						double hy0 = vHys[k].at<float>(i,j);
						double score0 = vScores[k].at<float>(i,j);

						vScores_candidates.push_back(score0);
						vDepths_candidates.push_back(depth0);
						vHx_candidates.push_back(hx0);
						vHy_candidates.push_back(hy0);

						if (j-1>=0) // means that there is a left neighbor
						{
							double hx_left = vHxs[k].at<float>(i,j-1);
							double hy_left = vHys[k].at<float>(i,j-1);
							double depth_left = vDepths[k].at<float>(i,j-1)/*+hx_left*/; // add the corresponding depth gradient of the left pixel

							double score_left, angle_left;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_left,hx_left,hy_left,score_left,angle_left,size);

							vScores_candidates.push_back(score_left);

							vDepths_candidates.push_back(depth_left);
							vHx_candidates.push_back(hx_left);
							vHy_candidates.push_back(hy_left);
						}

						if (i-1>=0) // means that there is a upper neighbor
						{
							double hx_upper = vHxs[k].at<float>(i-1,j);
							double hy_upper = vHys[k].at<float>(i-1,j);
							double depth_upper = vDepths[k].at<float>(i-1,j)/*+hy_upper*/;

							double score_upper,angle_upper;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_upper,hx_upper,hy_upper,score_upper,angle_upper,size);

							vScores_candidates.push_back(score_upper);

							vDepths_candidates.push_back(depth_upper);
							vHx_candidates.push_back(hx_upper);
							vHy_candidates.push_back(hy_upper);
						}

						for (kk=0;kk<k;kk++)
						{
							double hx_kk = vHxs[kk].at<float>(i,j);
							double hy_kk = vHys[kk].at<float>(i,j);
							double depth_kk = vDepths[kk].at<float>(i,j);

							double score_kk,angle_kk;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_kk,hx_kk,hy_kk,score_kk,angle_kk,size);

							vScores_candidates.push_back(score_kk);

							vDepths_candidates.push_back(depth_kk);
							vHx_candidates.push_back(hx_kk);
							vHy_candidates.push_back(hy_kk);
						}

						vector <double>::iterator iterDouble = max_element(vScores_candidates.begin(), vScores_candidates.end());
						double max_score = *iterDouble;
						int idx_max_score = iterDouble - vScores_candidates.begin();

						double depth_possible = vDepths_candidates[idx_max_score];
						double hx_possible = vHx_candidates[idx_max_score];
						double hy_possible = vHy_candidates[idx_max_score];
						double score_possible = max_score;

						vDepths_candidates.clear(); vHx_candidates.clear(); vHy_candidates.clear(); vScores_candidates.clear();

						vScores_candidates.push_back(score_possible);
						vDepths_candidates.push_back(depth_possible);
						vHx_candidates.push_back(hx_possible);
						vHy_candidates.push_back(hy_possible);

						for (rk=0;rk<nRandSamp;rk++)
						{
							double d_min_k, d_max_k, hx_min_k, hx_max_k, hy_min_k, hy_max_k;
							if (rk==0)
							{
								d_max_k = depth_max;
								d_min_k = depth_min;
								hx_max_k = h_max;
								hx_min_k = h_min;
								hy_max_k = h_max;
								hy_min_k = h_min;
							}
							else
							{
								double factor_k = pow(factor, rk);
								double d_r = factor_k*d_range*0.5;
								double h_r = factor_k*h_range*0.5;
								DetermineInterval(depth_max, depth_min, depth_possible, d_r, d_max_k, d_min_k);
								DetermineInterval(h_max, h_min, hx_possible, h_r, hx_max_k, hx_min_k);
								DetermineInterval(h_max, h_min, hy_possible, h_r, hy_max_k, hy_min_k);
							}

							// generate current parameter set
							double depth_k = rng_initRndField.uniform(d_min_k, d_max_k);
							double hx_k = rng_initRndField.uniform(hx_min_k, hx_max_k);
							double hy_k = rng_initRndField.uniform(hy_min_k, hy_max_k);

							double score_k,angle_k;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_k,hx_k,hy_k,score_k,angle_k,size);

							vScores_candidates.push_back(score_k);

							vDepths_candidates.push_back(depth_k);
							vHx_candidates.push_back(hx_k);
							vHy_candidates.push_back(hy_k);
						}

						iterDouble = max_element(vScores_candidates.begin(), vScores_candidates.end());
						max_score = *iterDouble;
						idx_max_score = iterDouble - vScores_candidates.begin();

						vDepths[k].at<float>(i,j) = vDepths_candidates[idx_max_score];
						vHxs[k].at<float>(i,j) = vHx_candidates[idx_max_score];
						vHys[k].at<float>(i,j) = vHy_candidates[idx_max_score];
						vScores[k].at<float>(i,j) = max_score;
					}
				}

				Mat mDepth_map, mHx_map, mHy_map, mScore_map(imgHeight, imgWidth, CV_8UC3);

				double min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y;

				minMaxIdx(vDepths[k], &min_depth, &max_depth);
				minMaxIdx(vHxs[k], &min_incre_x, &max_incre_x);
				minMaxIdx(vHys[k], &min_incre_y, &max_incre_y);

				vDepths[k].convertTo(mDepth_map, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
				vHxs[k].convertTo(mHx_map, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
				vHys[k].convertTo(mHy_map, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

				for (i=0;i<imgHeight;i++)
				{
					for (j=0;j<imgWidth;j++)
					{
						if (vScores[k].at<float>(i,j)<0)
						{
							mScore_map.at<Vec3b>(i,j).val[0] = 0;
							mScore_map.at<Vec3b>(i,j).val[1] = 0;
							mScore_map.at<Vec3b>(i,j).val[2] = 255;
						}
						else
						{
							mScore_map.at<Vec3b>(i,j).val[0] = 0;
							mScore_map.at<Vec3b>(i,j).val[1] = FTOI(255*vScores[k].at<float>(i,j));
							mScore_map.at<Vec3b>(i,j).val[2] = 0;
						}
					}
				}

				strInfo.Format("D:\\all\\depth map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mDepth_map);
				strInfo.Format("D:\\all\\hx map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mHx_map);
				strInfo.Format("D:\\all\\hy map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mHy_map);
				strInfo.Format("D:\\all\\score map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mScore_map);

				Mat mNormColor;
				GetNormColorField(mK0, mR0, mt0, fx0_1, fy0_1, vDepths[k], vHxs[k], vHys[k], mNormColor);
				strInfo.Format("D:\\all\\normcolor map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mNormColor);

				strInfo.Format("D:\\all\\cloud points iteration %02d with image %02d.txt", ii, k);
				OutputPointCloud(strInfo,mK0,mR0,mt0,fx0_1,fy0_1,img0,vDepths[k],vHxs[k],vHys[k],vScores[k]);
			}
		} 
		else
		{
			for (k=(n_spt-1);k>=0;k--)
			{
				for (i=imgHeight-1;i>=0;i--)
				{
					strInfo.Format("evaluate row %04d with image %02d in iteration %02d", i, k, ii);
					theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

					for (j=imgWidth-1;j>=0;j--)
					{
						int i_real, j_real, nVisi;
						MakeSureNotOutBorder(j,i,j_real,i_real,wndSizeHalf,imgWidth,imgHeight);

						vector<double> vDepths_candidates, vHx_candidates, vHy_candidates;
						vector<double> vScores_candidates;

						// first evaluate current parameter set for (i,j)
						double depth0 = vDepths[k].at<float>(i,j);
						double hx0 = vHxs[k].at<float>(i,j);
						double hy0 = vHys[k].at<float>(i,j);
						double score0 = vScores[k].at<float>(i,j);

						vScores_candidates.push_back(score0);
						vDepths_candidates.push_back(depth0);
						vHx_candidates.push_back(hx0);
						vHy_candidates.push_back(hy0);

						if (j+1<imgWidth) // means that there is a right neighbor
						{
							double hx_right = vHxs[k].at<float>(i,j+1);
							double hy_right = vHys[k].at<float>(i,j+1);
							double depth_right = vDepths[k].at<float>(i,j+1)/*-hx_right*/;

							double score_right,angle_right;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_right,hx_right,hy_right,score_right,angle_right,size);

							vScores_candidates.push_back(score_right);

							vDepths_candidates.push_back(depth_right);
							vHx_candidates.push_back(hx_right);
							vHy_candidates.push_back(hy_right);
						}

						if (i+1<imgHeight) // means that there is a lower neighbor
						{
							double hx_lower = vHxs[k].at<float>(i+1,j);
							double hy_lower = vHys[k].at<float>(i+1,j);
							double depth_lower = vDepths[k].at<float>(i+1,j)/*-hy_lower*/;

							double score_lower,angle_lower;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_lower,hx_lower,hy_lower,score_lower,angle_lower,size);

							vScores_candidates.push_back(score_lower);

							vDepths_candidates.push_back(depth_lower);
							vHx_candidates.push_back(hx_lower);
							vHy_candidates.push_back(hy_lower);
						}

						for (kk=(n_spt-1);kk>k;kk--)
						{
							double hx_kk = vHxs[kk].at<float>(i,j);
							double hy_kk = vHys[kk].at<float>(i,j);
							double depth_kk = vDepths[kk].at<float>(i,j);

							double score_kk,angle_kk;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_kk,hx_kk,hy_kk,score_kk,angle_kk,size);

							vScores_candidates.push_back(score_kk);

							vDepths_candidates.push_back(depth_kk);
							vHx_candidates.push_back(hx_kk);
							vHy_candidates.push_back(hy_kk);
						}

						vector <double>::iterator iterDouble = max_element(vScores_candidates.begin(), vScores_candidates.end());
						double max_score = *iterDouble;
						int idx_max_score = iterDouble - vScores_candidates.begin();

						double depth_possible = vDepths_candidates[idx_max_score];
						double hx_possible = vHx_candidates[idx_max_score];
						double hy_possible = vHy_candidates[idx_max_score];
						double score_possible = max_score;

						vDepths_candidates.clear(); vHx_candidates.clear(); vHy_candidates.clear(); vScores_candidates.clear();

						vScores_candidates.push_back(score_possible);
						vDepths_candidates.push_back(depth_possible);
						vHx_candidates.push_back(hx_possible);
						vHy_candidates.push_back(hy_possible);

						for (rk=0;rk<nRandSamp;rk++)
						{
							double d_min_k, d_max_k, hx_min_k, hx_max_k, hy_min_k, hy_max_k;
							if (rk==0)
							{
								d_max_k = depth_max;
								d_min_k = depth_min;
								hx_max_k = h_max;
								hx_min_k = h_min;
								hy_max_k = h_max;
								hy_min_k = h_min;
							}
							else
							{
								double factor_k = pow(factor, rk);
								double d_r = factor_k*d_range*0.5;
								double h_r = factor_k*h_range*0.5;
								DetermineInterval(depth_max, depth_min, depth_possible, d_r, d_max_k, d_min_k);
								DetermineInterval(h_max, h_min, hx_possible, h_r, hx_max_k, hx_min_k);
								DetermineInterval(h_max, h_min, hy_possible, h_r, hy_max_k, hy_min_k);
							}

							// generate current parameter set
							double depth_k = rng_initRndField.uniform(d_min_k, d_max_k);
							double hx_k = rng_initRndField.uniform(hx_min_k, hx_max_k);
							double hy_k = rng_initRndField.uniform(hy_min_k, hy_max_k);

							double score_k,angle_k;
							CheckOnePixel_givenOneParamSet_oneImg(mK0,mR0,mt0,img0,fx0_1,fy0_1,vKs[k],vRs[k],vts[k],vImgs[k],vfx_1[k],vfy_1[k],j_real,i_real,depth_k,hx_k,hy_k,score_k,angle_k,size);

							vScores_candidates.push_back(score_k);

							vDepths_candidates.push_back(depth_k);
							vHx_candidates.push_back(hx_k);
							vHy_candidates.push_back(hy_k);
						}

						iterDouble = max_element(vScores_candidates.begin(), vScores_candidates.end());
						max_score = *iterDouble;
						idx_max_score = iterDouble - vScores_candidates.begin();

						vDepths[k].at<float>(i,j) = vDepths_candidates[idx_max_score];
						vHxs[k].at<float>(i,j) = vHx_candidates[idx_max_score];
						vHys[k].at<float>(i,j) = vHy_candidates[idx_max_score];
						vScores[k].at<float>(i,j) = max_score;
					}
				}

				Mat mDepth_map, mHx_map, mHy_map, mScore_map(imgHeight, imgWidth, CV_8UC3);

				double min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y;

				minMaxIdx(vDepths[k], &min_depth, &max_depth);
				minMaxIdx(vHxs[k], &min_incre_x, &max_incre_x);
				minMaxIdx(vHys[k], &min_incre_y, &max_incre_y);

				vDepths[k].convertTo(mDepth_map, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
				vHxs[k].convertTo(mHx_map, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
				vHys[k].convertTo(mHy_map, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

				for (i=0;i<imgHeight;i++)
				{
					for (j=0;j<imgWidth;j++)
					{
						if (vScores[k].at<float>(i,j)<0)
						{
							mScore_map.at<Vec3b>(i,j).val[0] = 0;
							mScore_map.at<Vec3b>(i,j).val[1] = 0;
							mScore_map.at<Vec3b>(i,j).val[2] = 255;
						}
						else
						{
							mScore_map.at<Vec3b>(i,j).val[0] = 0;
							mScore_map.at<Vec3b>(i,j).val[1] = FTOI(255*vScores[k].at<float>(i,j));
							mScore_map.at<Vec3b>(i,j).val[2] = 0;
						}
					}
				}

				strInfo.Format("D:\\all\\depth map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mDepth_map);
				strInfo.Format("D:\\all\\hx map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mHx_map);
				strInfo.Format("D:\\all\\hy map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mHy_map);
				strInfo.Format("D:\\all\\score map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mScore_map);

				Mat mNormColor;
				GetNormColorField(mK0, mR0, mt0, fx0_1, fy0_1, vDepths[k], vHxs[k], vHys[k], mNormColor);
				strInfo.Format("D:\\all\\normcolor map iteration %02d with image %02d.bmp", ii, k);
				imwrite(strInfo.GetBuffer(), mNormColor);

				strInfo.Format("D:\\all\\cloud points iteration %02d with image %02d.txt", ii, k);
				OutputPointCloud(strInfo,mK0,mR0,mt0,fx0_1,fy0_1,img0,vDepths[k],vHxs[k],vHys[k],vScores[k]);
			}
		}
	}
}

RNG rng_initRndField(0xffffffff);
// 20140914, self-contained version
void DeepVoid_MVS::InitRndField(const Matx33d & mK,		// input:	the camera matrix
								const Matx33d & mR,		// input:	the rotation matrix
								const Matx31d & mt,		// input:	the translation vector
								double depth_min,		// input:	the minimal depth
								double depth_max,		// input:	the maximal depth
								double angLimit,		// input:	the angular range limit of the normal of every object point, in angle, not radian
								int width, int height,	// input:	the size of the image
								Mat & mDepth,			// output:	the 32FC1 matrix with the same size of the image, the initial depth
								Mat & mHx,				// output:	the 32FC1 matrix with the same size of the image, the initial hx
								Mat & mHy				// output:	the 32FC1 matrix with the same size of the image, the initial hy
								)
{
	int i,j;

	double f=(mK(0,0)+mK(1,1))*0.5;
	double tana = tan(angLimit*D2R);
	double tana_f = tana/f;

	// initialize the random depth field first
	mDepth = Mat(height, width, CV_32FC1);
	mHx = Mat(height, width, CV_32FC1);
	mHy = Mat(height, width, CV_32FC1);

	rng_initRndField.fill(mDepth, RNG::UNIFORM, depth_min, depth_max);

	// then
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			double Z = mDepth.at<float>(i,j);
			double dX_incre = tana_f*Z;
			mHx.at<float>(i,j) = rng_initRndField.uniform(-dX_incre, dX_incre);
			mHy.at<float>(i,j) = rng_initRndField.uniform(-dX_incre, dX_incre);
		}
	}
}

void DeepVoid_MVS::MakeSureNotOutBorder(int x, int y,				// input:	original center of rect
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

// 20140914, self-contained version, check one parameter set wrt one particular image not all support images
void DeepVoid_MVS::CheckOnePixel_givenOneParamSet_oneImg(const Matx33d & mK0, 	// input: mK0 reference image
														 const Matx33d & mR0, 	// input: mR0 reference image
														 const Matx31d & mt0, 	// input: mt0 reference image
														 const Mat & img0,		// input: the reference image
														 double fx0_1,			// input: fx0_1 = 1/fx0
														 double fy0_1, 			// input: fy0_1 = 1/fy0
														 const Matx33d & mK,	// input: mK one specific support image
														 const Matx33d & mR,	// input: mR one specific support image
														 const Matx31d & mt,	// input: mt one specific support image
														 const Mat & img,		// input: the support image
														 double fx_1,			// input: fx_1 = 1/fx
														 double fy_1,			// input: fy_1 = 1/fy
														 int x, int y,			// input:	the indices of the pixel to be checked
														 double depth,			// input:	the depth assigned to this pixel
														 double hx, double hy,	// input:	the depth incremental factor along x and y axis assigned to this pixel
														 double & score,		// output:	evaluation score of current parameter set
														 double & angle,		// output:	the angle between current patch normal estimate and the sight line wrt support image
														 int size /*= 5*/		// input:	the window size of the image patch
														 )
{
	int i, j, ii,jj;

	int half_s = (size-1)*0.5;

	vector<Point3d> vWrdPts;

	int nChannel = img0.channels();

	Mat imgsmall0, imgsmalli;

	if (nChannel==1) // gray level image
	{
		imgsmall0 = Mat(size, size, CV_32FC1); imgsmalli = Mat(size, size, CV_32FC1);

		for (i=-half_s;i<=half_s;i++)
		{
			for (j=-half_s;j<=half_s;j++)
			{
				Matx31d XYZ = GetXYZ_givenDepth(mK0, mR0, mt0, x+j, y+i, depth+hx*j+hy*i);
				Point3d pt3d;
				pt3d.x = XYZ(0);
				pt3d.y = XYZ(1);
				pt3d.z = XYZ(2);
				vWrdPts.push_back(pt3d);

				imgsmall0.at<float>(i+half_s, j+half_s) = img0.at<uchar>(y+i,x+j);
				imgsmall0.at<float>(i+half_s, j+half_s) = img0.at<uchar>(y+i,x+j);
				imgsmall0.at<float>(i+half_s, j+half_s) = img0.at<uchar>(y+i,x+j);
			}
		}
	} 
	else // color image
	{
		imgsmall0 = Mat(size, size, CV_32FC3); imgsmalli = Mat(size, size, CV_32FC3);

		for (i=-half_s;i<=half_s;i++)
		{
			for (j=-half_s;j<=half_s;j++)
			{
				Matx31d XYZ = GetXYZ_givenDepth(mK0, mR0, mt0, x+j, y+i, depth+hx*j+hy*i);
				Point3d pt3d;
				pt3d.x = XYZ(0);
				pt3d.y = XYZ(1);
				pt3d.z = XYZ(2);
				vWrdPts.push_back(pt3d);

				imgsmall0.at<Vec3f>(i+half_s, j+half_s).val[0] = img0.at<Vec3b>(y+i,x+j).val[0];
				imgsmall0.at<Vec3f>(i+half_s, j+half_s).val[1] = img0.at<Vec3b>(y+i,x+j).val[1];
				imgsmall0.at<Vec3f>(i+half_s, j+half_s).val[2] = img0.at<Vec3b>(y+i,x+j).val[2];
			}
		}
	}

	// first, compute the normal corresponds to current parameter set
	double fx0 = mK0(0,0); double fy0 = mK0(1,1);
	double cx0 = mK0(0,2); double cy0 = mK0(1,2);
	double nimgx = (x-cx0)*fx0_1;
	double nimgy = (y-cy0)*fy0_1;
	Matx31d mn0; mn0(2)=1;
	get_normal_givendrhxhy(fx0, fy0, nimgx, nimgy, depth, hx, hy, mn0(0), mn0(1));
	Matx31d mnw = -mR0.t()*mn0; // convert the normal into world coordinate system
	double nmnw = norm(mnw);

	//////////////////////////////////////////////////////////////////////////
	double x_k, y_k;

	bool bOutBorder = false;

	for (ii=0;ii<size;ii++)
	{
		for (jj=0;jj<size;jj++)
		{
			double r,g,b, imgpt_x, imgpt_y;
			if (BilinearInterp(mK,mR,mt,img,vWrdPts[ii*size+jj].x, vWrdPts[ii*size+jj].y, vWrdPts[ii*size+jj].z, r,g,b, &imgpt_x, &imgpt_y))
			{
				if (nChannel==1)
				{
					imgsmalli.at<float>(ii, jj) = b;
				} 
				else
				{
					imgsmalli.at<Vec3f>(ii, jj).val[0] = b;
					imgsmalli.at<Vec3f>(ii, jj).val[1] = g;
					imgsmalli.at<Vec3f>(ii, jj).val[2] = r;
				}
			} 
			else
			{
				bOutBorder = true;
				break;
			}

			if (ii==half_s&&jj==half_s)
			{
				x_k = imgpt_x;
				y_k = imgpt_y;
			}
		}
		if (bOutBorder)
		{
			break;
		}
	}

	if (bOutBorder)
	{
		score = -1;
		angle = -1;
		return;
	}

	// current line of sight
	Matx31d msightvec; // from image point to optical center
	msightvec(0) = -(x_k-mK(0,2))*fx_1;
	msightvec(1) = -(y_k-mK(1,2))*fy_1;
	msightvec(2) = -1;
	Matx31d mnk = mR*mnw;
	Matx<double, 1, 1> mcosa = mnk.t()*msightvec;
	double cosa = mcosa(0)/(nmnw*norm(msightvec));
	double ang = acos(cosa)*R2D;
	angle = ang;
	//	if (ang>=90){score=-1;return;} // ang is supposed to be smaller than 90 if it is visible


	// Create the result matrix
	int result_cols = imgsmalli.cols - imgsmall0.cols + 1;
	int result_rows = imgsmalli.rows - imgsmall0.rows + 1;
	Mat result(result_rows, result_cols, CV_32FC1);
	matchTemplate(imgsmall0, imgsmalli, result, /*CV_TM_SQDIFF_NORMED*/CV_TM_CCORR_NORMED);
	score = result.at<float>(0);
}

// return the computed XYZ given the depth
Matx31d DeepVoid_MVS::GetXYZ_givenDepth(const Matx33d & mK,					// input:	3*3, camera matrix
										const Matx33d & mR,					// input:	3*3, rotation matrix
										const Matx31d & mt,					// input:	3*1, translation vector
										double img_x, double img_y,			// input:	the distortion free image coordinates of the point
										double depth						// input:	the depth of this point relative to this camera
										)
{
	Matx31d mImgPt;
	mImgPt(0) = img_x;
	mImgPt(1) = img_y;
	mImgPt(2) = 1;

	Matx31d mDir = mK.inv()*mImgPt;

	return mR.t()*(depth*mDir-mt); // Xc=RXw+t -> Xw=R'(Xc-t)
}

// return the computed XYZ given the depth
Matx31d DeepVoid_MVS::GetXYZ_givenDepth(const Matx33d & mR,					// input:	3*3, rotation matrix
										const Matx31d & mt,					// input:	3*1, translation vector
										double nimgx, double nimgy,			// input:	the distortion free normalized image coordinates of the point
										double depth							// input:	the depth of this point relative to this camera
										)
{
	Matx31d mDir;
	mDir(0) = nimgx;
	mDir(1) = nimgy;
	mDir(2) = 1;

	return mR.t()*(depth*mDir-mt); // Xc=RXw+t -> Xw=R'(Xc-t)
}

// the normal n(a,b,1) is the opposite normal, the actual normal is -n(-a,-b,-1)
void DeepVoid_MVS::get_normal_givendrhxhy(double fx, double fy,			// input:	equivalent focal length
										  double nimgx, double nimgy,	// input:	the normalized image point
										  double dr,					// input:	the depth of given pixel
										  double hx, double hy,			// input:	the depth gradient
										  double & a, double & b		// output:	the normal is n(a,b,1)
										  )
{
	double fxhx = fx*hx;
	double fyhy = fy*hy;
	double dd = fxhx*nimgx+fyhy*nimgy+dr;

	double dd_1 = 1.0/dd;

	a = -fxhx*dd_1;
	b = -fyhy*dd_1;
}

// Implementation of bilinear interpolation
double DeepVoid_MVS::BilinearInterp(double x, double y,	// the coordinates of position to be interpolated
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

bool DeepVoid_MVS::BilinearInterp(const Mat & img,		// input:	the image data
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

bool DeepVoid_MVS::BilinearInterp(const Matx33d & mK,			// input:	the camera matrix
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

void DeepVoid_MVS::DetermineInterval(double val_max, double val_min, double val_cur, double radius, double & inter_max, double & inter_min)
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

void DeepVoid_MVS::GetNormColorField(const Matx33d & mK,	// input:	the camera parameters
									 const Matx33d & mR,
									 const Matx31d & mt,
									 double fx_1,
									 double fy_1,
									 const Mat & mDepth,	// input:	given depth map relative to the given image
									 const Mat & mHx,
									 const Mat & mHy,
									 Mat & mNormColor		// output:	3 channel matrix
									 )
{
	int i,j,ii,jj;

	int imgWidth = mDepth.cols;
	int imgHeight = mDepth.rows;

	double max_norm = 1.0;
	double min_norm = -1.0;
	double max_min_1 = 1/(max_norm - min_norm);

	double fx = mK(0,0);
	double fy = mK(1,1);
	double cx = mK(0,2);
	double cy = mK(1,2);

	mNormColor = Mat(imgHeight, imgWidth, CV_8UC3);

	for (i=0;i<imgHeight;i++)
	{
		double nimgy = (i-cy)*fy_1;

		for (j=0;j<imgWidth;j++)
		{
			double nimgx = (j-cx)*fx_1;

			double depth = mDepth.at<float>(i,j);
			double hx = mHx.at<float>(i,j);
			double hy = mHy.at<float>(i,j);

			Matx31d XYZ = GetXYZ_givenDepth(mK, mR, mt, j, i, depth);

			// get the normal of current obtained parameters
			Matx31d mn0; mn0(2)=1;
			get_normal_givendrhxhy(fx, fy, nimgx, nimgy, depth, hx, hy, mn0(0), mn0(1));
			Matx31d mnw = -mR.t()*mn0; // convert the normal into world coordinate system
			double normN = norm(mnw);
			mnw(0)/=normN;mnw(1)/=normN;mnw(2)/=normN;

			uchar r = 255 * (mnw(0)-min_norm) * max_min_1;
			uchar g = 255 * (mnw(1)-min_norm) * max_min_1;
			uchar b = 255 * (mnw(2)-min_norm) * max_min_1;

			mNormColor.at<Vec3b>(i,j)[0] = b;
			mNormColor.at<Vec3b>(i,j)[1] = g;
			mNormColor.at<Vec3b>(i,j)[2] = r;
		}
	}
}

void DeepVoid_MVS::OutputPointCloud(CString strFile,	// input:	the file to write
									const Matx33d & mK,	// input:	the camera parameters
									const Matx33d & mR,
									const Matx31d & mt,
									double fx_1,
									double fy_1,
									const Mat & img,	// input:	the image
									const Mat & mDepth,	// input:	given depth map relative to the given image
									const Mat & mHx,
									const Mat & mHy,
									const Mat & mScores	// input:	the confidence map of the given depth map, if -1 means the corresponding depth is invalid
									)
{
	int i,j,ii,jj;

	int imgWidth = img.cols;
	int imgHeight = img.rows;

	double fx = mK(0,0);
	double fy = mK(1,1);
	double cx = mK(0,2);
	double cy = mK(1,2);

	FILE * file = fopen(strFile, "w");
	for (i=0;i<imgHeight;i++)
	{
		double nimgy = (i-cy)*fy_1;

		for (j=0;j<imgWidth;j++)
		{
			if (mScores.at<float>(i,j)<0)
			{
				continue;
			}

			double nimgx = (j-cx)*fx_1;

			double depth = mDepth.at<float>(i,j);
			double hx = mHx.at<float>(i,j);
			double hy = mHy.at<float>(i,j);

			Matx31d XYZ = GetXYZ_givenDepth(mR, mt, nimgx, nimgy, depth);

			// get the normal of current obtained parameters
			Matx31d mn0; mn0(2)=1;
			get_normal_givendrhxhy(fx, fy, nimgx, nimgy, depth, hx, hy, mn0(0), mn0(1));
			Matx31d mnw = -mR.t()*mn0; // convert the normal into world coordinate system
			double normN = norm(mnw);
			mnw(0)/=normN;mnw(1)/=normN;mnw(2)/=normN;

			int rgbRed,rgbGreen,rgbBlue;

			if (1 == img.channels())
			{
				Scalar pix = img.at<uchar>(i,j);
				rgbRed = rgbGreen = rgbBlue = pix.val[0];
			} 
			else
			{
				Vec3b pix = img.at<Vec3b>(i,j);
				rgbBlue = pix.val[0]; rgbGreen = pix.val[1]; rgbRed = pix.val[2];
			}

			fprintf(file, "%.12f;%.12f;%.12f;%d;%d;%d;%.12f;%.12f;%.12f\n", XYZ(0), XYZ(1), XYZ(2), rgbRed, rgbGreen, rgbBlue, mnw(0), mnw(1), mnw(2));
		}
	}
	fclose(file);
}

// 20141215, self-contained, do mpgc with only one support image
void DeepVoid_MVS::MPGC_Binocular_20141215(const Matx33d & mK0,		// input:	interior matrix of reference image
										   const Matx33d & mR0,		// input:	rotation matrix of reference image
										   const Matx31d & mt0,		// input:	translation vector of reference image
										   const Mat & img0,		// input:	reference image
										   const Matx33d & mK,		// input:	interior matrix of the support image
										   const Matx33d & mR,		// input:	rotation matrix of the support image
										   const Matx31d & mt,		// input:	translation vectors of the support image
										   const Mat & img,			// input:	the support image
										   Mat & mDepth,			// in&output:	initial and optimized depth
										   Mat & mHx,				// in&output:	initial and optimized hx
										   Mat & mHy,				// in&output:	initial and optimized hy
										   Mat & mScore,			// in&output:	initial and optimized score
										   int pw,					// input:	image patch width
										   int ph,					// input:	image patch height
										   int maxIter /*= 128*/,
										   double xEps /*= 1.0E-8*/,// input: threshold
										   double fEps /*= 1.0E-6*/	// input: threshold
										   )
{
	int i,j,k,ii,jj;

	int w = mDepth.cols;
	int h = mDepth.rows;

	int hpw = (pw-1)*0.5; // half patch width
	int hph = (ph-1)*0.5; // half patch height

	double min_depth, max_depth;
	minMaxIdx(mDepth, &min_depth, &max_depth);

	Mat mMask(ph, pw, CV_8UC1, Scalar(1));

	vector<Matx33d> vKs_visi,vRs_visi;
	vector<Matx31d> vts_visi;
	vector<Mat> vImgs_visi, vMasks_visi;
	vector<int> vNum_visi;

	vKs_visi.push_back(mK); vRs_visi.push_back(mR); vts_visi.push_back(mt);
	vImgs_visi.push_back(img); vMasks_visi.push_back(mMask);
	vNum_visi.push_back(pw*ph);

	CString strInfo;

	for (i=0;i<h;i++)
	{
		strInfo.Format("Binocular MPGC-optimizing row %04d", i);
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		for (j=0;j<w;j++)
		{
			double depth_init = mDepth.at<float>(i,j);
			double hx_init = mHx.at<float>(i,j);
			double hy_init = mHy.at<float>(i,j);
			double score_init = mScore.at<float>(i,j);

			if (score_init<=0)
			{
				continue;
			}

			int i_real, j_real;
			MakeSureNotOutBorder(j,i,j_real,i_real,hpw,w,h);

			double depth_optim, hx_optim, hy_optim, score_optim;

			if (optim_gn_drhxhyck_NCCcontrolled_masks(mK0,mR0,mt0,img0,vKs_visi,vRs_visi,vts_visi,vImgs_visi,vMasks_visi,vNum_visi,j_real,i_real,pw,ph,
				depth_init,hx_init,hy_init,score_init,depth_optim,hx_optim,hy_optim,score_optim,maxIter,xEps,fEps))
			{
				// the optimized depth must be within certain range, or all parameters are mean values
				if (depth_optim>min_depth && depth_optim<max_depth)
				{
					mDepth.at<float>(i,j) = depth_optim;
					mHx.at<float>(i,j) = hx_optim;
					mHy.at<float>(i,j) = hy_optim;
					mScore.at<float>(i,j) = score_optim;
				}
			}
		}
	}
}

// 20140913, self-contained, with masks to ensure unimodal MPGC
bool DeepVoid_MVS::optim_gn_drhxhyck_NCCcontrolled_masks(const Matx33d & mK0,				// input:	interior matrix of reference image
														 const Matx33d & mR0,				// input:	rotation matrix of reference image
														 const Matx31d & mt0,				// input:	translation vector of reference image
														 const Mat & img0,					// input:	reference image
														 const vector<Matx33d> & vKs,		// input:	interior matrix of all visible support images
														 const vector<Matx33d> & vRs,		// input:	rotation matrix of all visible support images
														 const vector<Matx31d> & vts,		// input:	translation vectors of all visible support images
														 const vector<Mat> & vImgs,			// input:	images of all visible support images
														 const vector<Mat> & vMasks,		// input:	pre-determined masks, one for each visible image
														 const vector<int> & vNum,			// input:	number of valid pixels in each mask, one for each visible image
														 int x, int y,						// input:	the indices of the pixel to be checked
														 int patchWidth, int patchHeight,	// input:	the patch size
														 double depth_init,					// input:	initial depth of this pixel
														 double hx_init,					// input:	initial normal of this pixel
														 double hy_init,					// input:	initial normal of this pixel
														 double score_init,					// input:	initial mean ncc value
														 double & depth_optim,				// output:	optimized depth of this pixel
														 double & hx_optim,					// output:	optimized normal of this pixel
														 double & hy_optim,					// output:	optimized normal of this pixel
														 double & score_optim,				// output:	optimized mean ncc value
														 int maxIter /*= 128*/,				// input: max iteration
														 double xEps /*= 1.0E-8*/,			// input: threshold
														 double fEps /*= 1.0E-6*/,			// input: threshold
														 int * iterNum /*= NULL*/			// output:iteration number when quiting
														 )
{
	int i,j,k,ii;

	int npts_patch = patchWidth*patchHeight;

	int n_visi = vKs.size(); // number of visible support images

	int nChannel = img0.channels();

	int halfpatch_w = (patchWidth-1)*0.5;
	int halfpatch_h = (patchHeight-1)*0.5;

	vector<Matx33d> vKRs;
	vector<Matx31d> vKts;
	vector<double> vfx_1,vfy_1;

	for (k=0;k<n_visi;k++)
	{
		vKRs.push_back(vKs[k]*vRs[k]);
		vKts.push_back(vKs[k]*vts[k]);

		double fx_1 = 1/vKs[k](0,0);
		double fy_1 = 1/vKs[k](1,1);

		vfx_1.push_back(fx_1);
		vfy_1.push_back(fy_1);
	}

	Matx31d mC0 = -mR0.t() * mt0;

	Matx33d mR0_t = mR0.t();

	double fx0 = mK0(0,0);
	double fy0 = mK0(1,1);
	double fx0_1 = 1/fx0;
	double fy0_1 = 1/fy0;
	double cx0 = mK0(0,2);
	double cy0 = mK0(1,2);

	// normalized image point
	double nimgx0 = (x-cx0)*fx0_1;
	double nimgy0 = (y-cy0)*fy0_1;

	vector<Matx31d> vRtxs_ref; // the normalized image points or directions of every patch points
	for (i=-halfpatch_h;i<=halfpatch_h;i++)
	{
		double nimgy = (y+i-cy0)*fy0_1;

		for (j=-halfpatch_w;j<=halfpatch_w;j++)
		{
			double nimgx = (x+j-cx0)*fx0_1;

			Matx31d xyz;
			xyz(0) = nimgx;
			xyz(1) = nimgy;
			xyz(2) = 1;

			Matx31d nxy = mR0_t*xyz; // R'x(u,v) transpose of R * normalized image points of (u,v)

			vRtxs_ref.push_back(nxy);
		}
	}

	Mat patch_ref;
	if (nChannel==1) // gray level image
	{
		patch_ref = Mat(patchHeight, patchWidth, CV_32FC1);

		for (i=-halfpatch_h;i<=halfpatch_h;i++)
		{
			for (j=-halfpatch_w;j<=halfpatch_w;j++)
			{
				patch_ref.at<float>(i+halfpatch_h, j+halfpatch_w) = img0.at<uchar>(y+i,x+j);
			}
		}
	} 
	else // color image
	{
		patch_ref = Mat(patchHeight, patchWidth, CV_32FC3);

		for (i=-halfpatch_h;i<=halfpatch_h;i++)
		{
			for (j=-halfpatch_w;j<=halfpatch_w;j++)
			{
				patch_ref.at<Vec3f>(i+halfpatch_h, j+halfpatch_w).val[0] = img0.at<Vec3b>(y+i,x+j).val[0];
				patch_ref.at<Vec3f>(i+halfpatch_h, j+halfpatch_w).val[1] = img0.at<Vec3b>(y+i,x+j).val[1];
				patch_ref.at<Vec3f>(i+halfpatch_h, j+halfpatch_w).val[2] = img0.at<Vec3b>(y+i,x+j).val[2];
			}
		}
	}

	double depth = depth_init;
	double hx = hx_init;
	double hy = hy_init;
	vector<double> vck(n_visi, 1);

	// record runtime info of each iteration
	vector<double> vDepths, vHxs, vHys, vScores, vF2;

	double fVal_old = 0; double ncc_mean_old = 0;
	double fVal_new; double ncc_mean_new;
	double hVal;

	for (k=0;k<maxIter;k++)
	{
		// first get the 3d patch points corresponding to current parameter set
		vector<Matx31d> vWrdPts;

		for (i=-halfpatch_h;i<=halfpatch_h;i++)
		{
			for (j=-halfpatch_w;j<=halfpatch_w;j++)
			{
				Matx31d mDir;
				mDir(0) = vRtxs_ref[(i+halfpatch_h)*patchWidth+j+halfpatch_w](0);
				mDir(1) = vRtxs_ref[(i+halfpatch_h)*patchWidth+j+halfpatch_w](1);
				mDir(2) = vRtxs_ref[(i+halfpatch_h)*patchWidth+j+halfpatch_w](2);

				Matx31d XYZ = mC0 + (depth+hx*j+hy*i) * mDir;

				vWrdPts.push_back(XYZ);
			}
		}

		// current window normal
		Matx31d mn0; mn0(2)=1;
		get_normal_givendrhxhy(fx0,fy0,nimgx0,nimgy0,depth,hx,hy,mn0(0),mn0(1));
		// convert the normal into world coordinate system
		// the reason why is not R'n here is because the normal output by get_normal_givendrhxhy() is actually -n
		Matx31d mnw = -mR0_t*mn0; 

		vector<Mat> vJacobs, vFs;
		vector<int> vIdxValid;

		int nAllRows = 0;

		Mat mJacob_k, mF_k, imgPatchResamp; vector<double> vnccs; double sum_ncc = 0; int sum_num = 0; bool bInvalid = false;
		for (ii=0;ii<n_visi;ii++)
		{
			double ncc; bool bOppo;

			if (!f_jacob_fk_drhxhyck_patchref_float_20140911(patch_ref,vImgs[ii],vMasks[ii],vNum[ii],vKs[ii],vRs[ii],
				vKRs[ii],vKts[ii],vfx_1[ii],vfy_1[ii],mnw,vWrdPts,vRtxs_ref,vck[ii],mJacob_k,mF_k,ncc,bOppo))
			{
				continue;
			}

			if (!bInvalid && bOppo)
			{
				bInvalid = true;
			}

			vnccs.push_back(ncc);
			sum_ncc += ncc*vNum[ii];
			sum_num += vNum[ii];

			nAllRows += mJacob_k.rows;

			vJacobs.push_back(mJacob_k);
			vFs.push_back(mF_k);
			vIdxValid.push_back(ii);
		}

		int nValid = vJacobs.size();
		int nAllCols = 3+nValid;

		if (nValid == 0)
		{
			// if no valid observations exist, then all parameters stay unchanged
			depth_optim = depth_init;
			hx_optim = hx_init;
			hy_optim = hy_init;
			score_optim = score_init;
			return false;
		}

		Mat mJ, mF;

		mJ = Mat(nAllRows, nAllCols, CV_32FC1, Scalar(0)); 
		mF = Mat(nAllRows, 1, CV_32FC1, Scalar(0));

		int ntmp = 0;
		for (i=0;i<nValid;i++)
		{
			for (j=0;j<vJacobs[i].rows;j++)
			{
				mJ.at<float>(ntmp+j,0) = vJacobs[i].at<float>(j,0);
				mJ.at<float>(ntmp+j,1) = vJacobs[i].at<float>(j,1);
				mJ.at<float>(ntmp+j,2) = vJacobs[i].at<float>(j,2);
				mJ.at<float>(ntmp+j,3+i) = vJacobs[i].at<float>(j,3);
				mF.at<float>(ntmp+j) = vFs[i].at<float>(j);
			}

			ntmp += vJacobs[i].rows;
		}

		// 解方程组 (J'J) h = -J'f 得到的 h 即为待优化参数的改正量，下次迭代的 x(k + 1) = x(k) + h
		Mat mA = mJ.t() * mJ;
		Mat mb = -mJ.t() * mF;
		Mat mA_1 = mA.inv(DECOMP_CHOLESKY);

		Mat mh = mA_1*mb;

		fVal_new = norm(mF)*norm(mF);
		hVal     = norm(mh);
		ncc_mean_new = sum_ncc/sum_num;

		// record this iteration
		vDepths.push_back(depth);
		vHxs.push_back(hx);
		vHys.push_back(hy);
		if (bInvalid) // this means current parameter voilated the normal requirement
		{
			vScores.push_back(-1);
		} 
		else
		{
			vScores.push_back(ncc_mean_new);
		}
		vF2.push_back(fVal_new);

		double s0 = sqrt(fVal_new/(mJ.rows-mJ.cols));

		vector<double> vStds;
		for (i=0;i<mA_1.rows;i++)
		{
			double stdev = s0 * sqrt(mA_1.at<float>(i,i));
			vStds.push_back(stdev);
		}

		double df2 = fVal_new - fVal_old;
		double dncc = ncc_mean_new - ncc_mean_old;

		// 当目标函数值的平方和变化量小于一定阈值或者改正量的范数值小于一定阈值时认为迭代收敛，退出迭代
		if ((fabs(df2) < fEps) || (hVal < xEps))
		{
			break;
		}

		fVal_old = fVal_new;
		ncc_mean_old = ncc_mean_new;

		depth += mh.at<float>(0);
		hx    += mh.at<float>(1);
		hy    += mh.at<float>(2);

		for (i=0;i<nValid;i++)
		{
			vck[vIdxValid[i]] += mh.at<float>(3+i);
		}
	}

	if (iterNum)
	{
		*iterNum = k;
	}

	vector <double>::const_iterator iterDouble = max_element(vScores.begin(), vScores.end());
	double max_score = *iterDouble;

	if (max_score < 0) // this means all parameter candidates have inreasonable normals
	{
		// if no valid observations exist, then all parameters stay unchanged
		depth_optim = depth_init;
		hx_optim = hx_init;
		hy_optim = hy_init;
		score_optim = score_init;
		return false;
	}

	int idx_max_score = iterDouble - vScores.begin();

//	iterDouble = min_element(vF2.begin(), vF2.end());
	double min_F2/* = *iterDouble*/;
	int idx_min_F2/* = iterDouble - vF2.begin()*/;

// 	bool bFirst = true;
// 	for (i=0;i<vF2.size();i++)
// 	{
// 		if (vScores[i]<0)
// 		{
// 			continue;
// 		}
// 
// 		if (bFirst)
// 		{
// 			min_F2 = vF2[i];
// 			idx_min_F2 = i;
// 			bFirst = false;
// 			continue;
// 		}
// 
// 		if (vF2[i]<min_F2)
// 		{
// 			min_F2 = vF2[i];
// 			idx_min_F2 = i;
// 		}
// 	}

	int idx_final = idx_max_score;
//	int idx_final = idx_min_F2;

	depth_optim = vDepths[idx_final];
	hx_optim = vHxs[idx_final];
	hy_optim = vHys[idx_final];
	score_optim = vScores[idx_final];

	return true;
}

// 20140911 return the objective vector and jacobian matrix of kth image
bool DeepVoid_MVS::f_jacob_fk_drhxhyck_patchref_float_20140911(const Mat & patch_ref,				// input:	the patch image in reference image
															   const Mat & img,						// input:	the kth image, supposed to be distortion free
															   const Mat & mask,					// input:	mask
															   int nValid,							// input:	number of valid pixels in mask
															   const Matx33d & mK,
															   const Matx33d & mR,
															   const Matx33d & mKR,					// input:	the kth image's intrinsic parameters
															   const Matx31d & mKt,					// input:	the kth image's rotation matrix
															   double fx_1, double fy_1,
															   const Matx31d & mnw,					// input:	the normal vector of reference patch in world coordinate
															   const vector<Matx31d> & vXYZs,		// input:	the spatial coordinates of every points in the patch
															   const vector<Matx31d> & vRtxs_ref,	// input:	the normalized image coordinates of every points in the patch in reference image
															   double ck,							// input:	current estimate of the photometric factor
															   Mat & mJacob,						// output:	the jacobian matrix
															   Mat & mF,							// output:	the objective vector 
															   double & ncc,						// output:	the ncc
															   bool & bOppo							// output:	whether if normal is opposite or not
															   )
{
	int i,j,k;

	if (nValid<3) // at least 3 valid pixels
	{
		return false;
	}

	int nChannel = img.channels();

	int n_pts = vXYZs.size();

	int w = patch_ref.cols;
	int h = patch_ref.rows;

	int hw = (w-1)/2;
	int hh = (h-1)/2;

	double cx = mK(0,2);
	double cy = mK(1,2);

	double imgx_mid, imgy_mid;

	vector<Point2d> vImgPts;
	for (i=0;i<n_pts;i++)
	{
		Matx31d mImg = mKR*vXYZs[i]+mKt;

		double z_1 = 1/mImg(2);
		Point2d pt2d;
		pt2d.x = mImg(0)*z_1;
		pt2d.y = mImg(1)*z_1;
		vImgPts.push_back(pt2d);

		if (i==(hh*w+hw))
		{
			imgx_mid = pt2d.x;
			imgy_mid = pt2d.y;
		}
	}

	Mat mGx, mGy, mPatch_rsmp;

	if (!MPGC_get_patch_gx_gy_roberts(img, vImgPts, w, h, mPatch_rsmp, mGx, mGy))
	{
		return false;
	}

	mJacob = Mat(nChannel*nValid, 4, CV_32FC1, Scalar(0));
	mF = Mat(nChannel*nValid, 1, CV_32FC1, Scalar(0));

	Mat mCkIk, mIr;
	if (nChannel==1) // gray image
	{
		mCkIk = Mat(nValid, 1, CV_32FC1, Scalar(0));
		mIr   = Mat(nValid, 1, CV_32FC1, Scalar(0));
	} 
	else // color image
	{
		mCkIk = Mat(nValid, 1, CV_32FC3, Scalar(0));
		mIr   = Mat(nValid, 1, CV_32FC3, Scalar(0));
	}


	int idx_cur = 0; // number of currently been considered valid pixels

	for (i=0;i<h;i++)
	{
		int v = i-hh;
		for (j=0;j<w;j++)
		{
			int u = j-hw;

			uchar val_mask = mask.at<uchar>(i,j);
			if (val_mask==0) // this means that this pixel is not valid, it should not be considered in MPGC
			{
				continue;
			}

			// dXYZ/ddrhxhy
			Matx33d dXYZ_ddrhxhy = Jacobian_XYZ_drhxhy(vRtxs_ref[i*w+j],u,v);

			// dxy/dXYZ
			double imgx,imgy;
			Matx23d dxy_dXYZ = Jacobian_xy_XYZ(mKR,mKt,vXYZs[i*w+j](0),vXYZs[i*w+j](1),vXYZs[i*w+j](2),imgx,imgy);

			// dxy/ddrhxhy
			Matx23d dxy_ddrhxhy = dxy_dXYZ*dXYZ_ddrhxhy;

			if (nChannel == 1)
			{
				Matx12d dI_dxy;
				dI_dxy(0) = mGx.at<float>(i,j);
				dI_dxy(1) = mGy.at<float>(i,j);

				Matx13d df_ddrhxhy = ck * dI_dxy * dxy_ddrhxhy;

				int idxrow = idx_cur;
				mJacob.at<float>(idxrow, 0) = df_ddrhxhy(0);
				mJacob.at<float>(idxrow, 1) = df_ddrhxhy(1);
				mJacob.at<float>(idxrow, 2) = df_ddrhxhy(2);
				mJacob.at<float>(idxrow, 3) = mPatch_rsmp.at<float>(i,j);
				double ckIk = mPatch_rsmp.at<float>(i,j) * ck;
				double Ir = patch_ref.at<float>(i,j);
				mF.at<float>(idxrow) = ckIk - Ir; // df = ckIk - Ir

				mCkIk.at<float>(idx_cur) = ckIk;
				mIr.at<float>(idx_cur) = Ir;
			}
			else
			{
				Matx32d dI_dxy;
				for (k=0;k<3;k++)
				{
					dI_dxy(k,0) = mGx.at<Vec3f>(i,j).val[k];
					dI_dxy(k,1) = mGy.at<Vec3f>(i,j).val[k];
				}

				Matx33d df_ddrhxhy = ck * dI_dxy * dxy_ddrhxhy;

				for (k=0;k<3;k++)
				{
					int idxrow = (idx_cur)*3+k;
					mJacob.at<float>(idxrow, 0) = df_ddrhxhy(k,0);
					mJacob.at<float>(idxrow, 1) = df_ddrhxhy(k,1);
					mJacob.at<float>(idxrow, 2) = df_ddrhxhy(k,2);
					mJacob.at<float>(idxrow, 3) = mPatch_rsmp.at<Vec3f>(i,j).val[k];
					double ckIk = mPatch_rsmp.at<Vec3f>(i,j).val[k] * ck;
					double Ir = patch_ref.at<Vec3f>(i,j).val[k];
					mF.at<float>(idxrow) = ckIk - Ir; // df = ckIk - Ir

					mCkIk.at<Vec3f>(idx_cur).val[k] = ckIk;
					mIr.at<Vec3f>(idx_cur).val[k] = Ir;
				}
			}

			++idx_cur;
		}
	}

	Mat mNCC(1, 1, CV_32FC1);
	matchTemplate(mIr, mCkIk, mNCC, CV_TM_CCORR_NORMED); // compute NCC
	ncc = mNCC.at<float>(0);

	// compute the angle between normal and the line of sight
	// current line of sight
	Matx31d msightvec; // from image point to optical center
	msightvec(0) = -(imgx_mid-cx)*fx_1;
	msightvec(1) = -(imgy_mid-cy)*fy_1;
	msightvec(2) = -1;
	Matx31d mnk = mR*mnw;
	double cosa = ComputeCosa(mnk, msightvec);
	if (cosa<=0) // ang is supposed to be smaller than 90 if it is visible
	{
		bOppo = true;
	}
	else
	{
		bOppo = false;
	}

// 	matchTemplate(patch_ref, mPatch_rsmp, mNCC, CV_TM_CCORR_NORMED);
// 	double ncc_old = mNCC.at<float>(0);

	return true;
}

bool DeepVoid_MVS::MPGC_get_patch_gx_gy_roberts(const Mat & img,					// input:	the image data to be resampled
												const vector<Point2d> & vImgPts,	// input:	the image coordinates of all the samples
												int patchWidth, int patchHeight,	// input:	the size of the image patch
												Mat & mPatch,						// output:	the resampled image patch
												Mat & mGx,							// output:	the dI/dx matrix of every pixel in image patch
												Mat & mGy							// output:	the dI/dy matrix of every pixel in image patch
												)
{
	int i,j,k;
	int imgWidth = img.cols;
	int imgHeight = img.rows;
	int nChannel = img.channels();

	if (nChannel == 1)
	{
		mPatch = Mat(patchHeight, patchWidth, CV_32FC1);
		mGx = Mat(patchHeight, patchWidth, CV_32FC1);
		mGy = Mat(patchHeight, patchWidth, CV_32FC1);

		for (i=0;i<patchHeight;i++)
		{
			for (j=0;j<patchWidth;j++)
			{
				double img_x = vImgPts[i*patchWidth+j].x;
				double img_y = vImgPts[i*patchWidth+j].y;

				double r,g,b;
				double rl,gl,bl; // rgb of the I(x-1,y)
				double rr,gr,br; // rgb of the I(x+1,y)
				double ru,gu,bu; // rgb of the I(x,y-1)
				double rb,gb,bb; // rgb of the I(x,y+1)
				if (BilinearInterp(img, img_x, img_y, r, g, b) &&
					BilinearInterp(img, img_x-1, img_y, rl, gl, bl) && BilinearInterp(img, img_x+1, img_y, rr, gr, br) && 
					BilinearInterp(img, img_x, img_y-1, ru, gu, bu) && BilinearInterp(img, img_x, img_y+1, rb, gb, bb))
				{
					mPatch.at<float>(i,j) = r;
					mGx.at<float>(i,j) = (rr-rl)*0.5; // dI/dx = (I(x+1,y)-I(x-1,y))/2
					mGy.at<float>(i,j) = (rb-ru)*0.5; // dI/dy = (I(x,y+1)-I(x,y-1))/2
				} 
				else
				{
					return false;
				}
			}
		}

// 		for (i=0;i<patchHeight;i++)
// 		{
// 			for (j=0;j<patchWidth;j++)
// 			{
// 				if (j==0)
// 				{
// 					mGx.at<float>(i,j)=mPatch.at<float>(i,j+1)-mPatch.at<float>(i,j);
// 				}
// 				else if (j==(patchWidth-1))
// 				{
// 					mGx.at<float>(i,j)=mPatch.at<float>(i,j)-mPatch.at<float>(i,j-1);
// 				}
// 				else
// 				{
// 					mGx.at<float>(i,j)=(mPatch.at<float>(i,j+1)-mPatch.at<float>(i,j-1))*0.5;
// 				}
// 
// 				if (i==0)
// 				{
// 					mGy.at<float>(i,j)=mPatch.at<float>(i+1,j)-mPatch.at<float>(i,j);
// 				} 
// 				else if (i==(patchHeight-1))
// 				{
// 					mGy.at<float>(i,j)=mPatch.at<float>(i,j)-mPatch.at<float>(i-1,j);
// 				}
// 				else
// 				{
// 					mGy.at<float>(i,j)=(mPatch.at<float>(i+1,j)-mPatch.at<float>(i-1,j))*0.5;
// 				}
// 			}
// 		}
	}
	else
	{
		mPatch = Mat(patchHeight, patchWidth, CV_32FC3);
		mGx = Mat(patchHeight, patchWidth, CV_32FC3);
		mGy = Mat(patchHeight, patchWidth, CV_32FC3);

		for (i=0;i<patchHeight;i++)
		{
			for (j=0;j<patchWidth;j++)
			{
				double img_x = vImgPts[i*patchWidth+j].x;
				double img_y = vImgPts[i*patchWidth+j].y;

				double r,g,b;
				double rl,gl,bl; // rgb of the I(x-1,y)
				double rr,gr,br; // rgb of the I(x+1,y)
				double ru,gu,bu; // rgb of the I(x,y-1)
				double rb,gb,bb; // rgb of the I(x,y+1)
				if (BilinearInterp(img, img_x, img_y, r, g, b) &&
					BilinearInterp(img, img_x-1, img_y, rl, gl, bl) && BilinearInterp(img, img_x+1, img_y, rr, gr, br) && 
					BilinearInterp(img, img_x, img_y-1, ru, gu, bu) && BilinearInterp(img, img_x, img_y+1, rb, gb, bb))
				{
					mPatch.at<Vec3f>(i,j).val[0] = b;
					mPatch.at<Vec3f>(i,j).val[1] = g;
					mPatch.at<Vec3f>(i,j).val[2] = r;

					mGx.at<Vec3f>(i,j).val[0] = (br-bl)*0.5; // dI/dx = (I(x+1,y)-I(x-1,y))/2
					mGx.at<Vec3f>(i,j).val[1] = (gr-gl)*0.5; // dI/dx = (I(x+1,y)-I(x-1,y))/2
					mGx.at<Vec3f>(i,j).val[2] = (rr-rl)*0.5; // dI/dx = (I(x+1,y)-I(x-1,y))/2

					mGy.at<Vec3f>(i,j).val[0] = (bb-bu)*0.5; // dI/dy = (I(x,y+1)-I(x,y-1))/2
					mGy.at<Vec3f>(i,j).val[1] = (gb-gu)*0.5; // dI/dy = (I(x,y+1)-I(x,y-1))/2
					mGy.at<Vec3f>(i,j).val[2] = (rb-ru)*0.5; // dI/dy = (I(x,y+1)-I(x,y-1))/2
				} 
				else
				{
					return false;
				}
			}
		}

// 		for (i=0;i<patchHeight;i++)
// 		{
// 			for (j=0;j<patchWidth;j++)
// 			{
// 				if (j==0)
// 				{
// 					for (k=0;k<3;k++)
// 					{
// 						mGx.at<Vec3f>(i,j).val[k]=mPatch.at<Vec3f>(i,j+1).val[k]-mPatch.at<Vec3f>(i,j).val[k];
// 					}
// 				}
// 				else if (j==(patchWidth-1))
// 				{
// 					for (k=0;k<3;k++)
// 					{
// 						mGx.at<Vec3f>(i,j).val[k]=mPatch.at<Vec3f>(i,j).val[k]-mPatch.at<Vec3f>(i,j-1).val[k];
// 					}
// 				}
// 				else
// 				{
// 					for (k=0;k<3;k++)
// 					{
// 						mGx.at<Vec3f>(i,j).val[k]=(mPatch.at<Vec3f>(i,j+1).val[k]-mPatch.at<Vec3f>(i,j-1).val[k])*0.5;
// 					}
// 				}
// 
// 				if (i==0)
// 				{
// 					for (k=0;k<3;k++)
// 					{
// 						mGy.at<Vec3f>(i,j).val[k]=mPatch.at<Vec3f>(i+1,j).val[k]-mPatch.at<Vec3f>(i,j).val[k];
// 					}
// 				} 
// 				else if (i==(patchHeight-1))
// 				{
// 					for (k=0;k<3;k++)
// 					{
// 						mGy.at<Vec3f>(i,j).val[k]=mPatch.at<Vec3f>(i,j).val[k]-mPatch.at<Vec3f>(i-1,j).val[k];
// 					}
// 				}
// 				else
// 				{
// 					for (k=0;k<3;k++)
// 					{
// 						mGy.at<Vec3f>(i,j).val[k]=(mPatch.at<Vec3f>(i+1,j).val[k]-mPatch.at<Vec3f>(i-1,j).val[k])*0.5;
// 					}
// 				}
// 			}
// 		}
	}

	return true;
}

// support window中某像素表征的三维点世界坐标对该support window的dr hx hy求导
Matx33d DeepVoid_MVS::Jacobian_XYZ_drhxhy(const Matx31d & mRtx,			// input:	R'x(u,v), transpose of R * normalized image points of (u,v)
										  double u, double v				// input:	local offset coordinates of this pixel w.r.t central pixel
										  )
{
	Matx13d m1uv;
	m1uv(0) = 1;
	m1uv(1) = u;
	m1uv(2) = v;

	return mRtx*m1uv; // R'x[1 u v]
}

// 投影像点坐标对对应的空间点世界坐标求导
Matx23d DeepVoid_MVS::Jacobian_xy_XYZ(const Matx33d & mKR,			
									  const Matx31d & mKt,
									  double X, double Y, double Z,
									  double & imgx, double & imgy
									  )
{
	Matx31d XYZ;
	XYZ(0) = X;
	XYZ(1) = Y;
	XYZ(2) = Z;

	Matx31d Proj = mKR*XYZ+mKt;

	double f1 = Proj(0);
	double f2 = Proj(1);
	double g =  Proj(2);
	double g_1  = 1.0/g;
	double g2_1 = g_1*g_1;

	imgx = f1*g_1;
	imgy = f2*g_1;

	Matx23d der;

	der(0,0) = (mKR(0,0)*g-mKR(2,0)*f1)*g2_1; // dx/dX
	der(0,1) = (mKR(0,1)*g-mKR(2,1)*f1)*g2_1; // dx/dY
	der(0,2) = (mKR(0,2)*g-mKR(2,2)*f1)*g2_1; // dx/dZ

	der(1,0) = (mKR(1,0)*g-mKR(2,0)*f2)*g2_1; // dy/dX
	der(1,1) = (mKR(1,1)*g-mKR(2,1)*f2)*g2_1; // dy/dY
	der(1,2) = (mKR(1,2)*g-mKR(2,2)*f2)*g2_1; // dy/dZ

	return der;
}

// 计算两向量间夹角的余弦 [-1, 1]
double DeepVoid_MVS::ComputeCosa(const Matx31d & vec1, const Matx31d & vec2)
{
	Matx<double, 1, 1> mcosa = vec1.t()*vec2;        // v1'v2
	double cosa = mcosa(0)/(norm(vec1)*norm(vec2));  // cosa = v1'v2/|v1||v2|

	if (cosa>1)
	{
		cosa=1;
	}
	if (cosa<-1)
	{
		cosa=-1;
	}

	return cosa;
}

// augment the value field of a lower resolution image into a higher resolution image
void DeepVoid_MVS::AugmentField_Interp(const Mat & field,		// input:	lower resolution image to be augmented
									   Mat & field_aug,			// output:	the augmented field
									   int width, int height,	// input:	size of augmented field
									   int n /*= 2*/,			// input:	replicate value of one pixel for n pixels
									   double factor /*= 1*/	// input:	optional input, factor
									   )
{
	int i,j;

	field_aug = Mat(height,width,CV_32FC1);

	double n_1 = 1.0/n;

	for (i=0;i<height;i++)
	{
		double i0=i*n_1; // the double row index in the lower resolution field

		int y1 = floor_fast(i0);// the y coordinate of the two top points
		if (y1>=(field.rows-1))
		{
			y1=field.rows-2;
		}
		int y2 = y1+1;			// the y coordinate of the two bottom points

		for (j=0;j<width;j++)
		{
			double j0=j*n_1; // the double col index in the lower resolution field

			int x1 = floor_fast(j0);// the x coordinate of the two left points
			if (x1>=(field.cols-1))
			{
				x1=field.cols-2;
			}
			int x2 = x1+1;			// the x coordinate of the two right points

			double f1 = field.at<float>(y1,x1)*factor; // topleft
			double f2 = field.at<float>(y1,x2)*factor; // topright
			double f3 = field.at<float>(y2,x1)*factor; // bottomleft
			double f4 = field.at<float>(y2,x2)*factor; // bottomright

			double f = BilinearInterp(j0, i0, x1, x2, y1, y2, f1, f2, f3, f4);

			field_aug.at<float>(i,j) = f;
		}
	}
}

// 20140914, self-contained version, invalidate those pixels whose normal estimate is obviously wrong by setting its score to -1
void DeepVoid_MVS::InvalidatePixels_byNormal(const Matx33d & mK0, 		// input:	mK0 reference image
											 const Matx33d & mR0, 		// input:	mR0 reference image
											 const Matx31d & mt0, 		// input:	mt0 reference image
											 const Matx33d & mK,		// input:	mK one specific support image
											 const Matx33d & mR,		// input:	mR one specific support image
											 const Matx31d & mt,		// input:	mt one specific support image
											 const Mat & mDepth,
											 const Mat & mHx,
											 const Mat & mHy,
											 Mat & mScore,
											 double thresh_ang /*= 90*/,
											 double thresh_ncc/* = 1.0E-4*/
											)
{
	int i,j;

	int w = mDepth.cols;
	int h = mDepth.rows;

	double cos_thresh_ang = cosd(thresh_ang);

	double fx0 = mK0(0,0); double fy0 = mK0(1,1);
	double fx0_1 = 1/fx0;  double fy0_1 = 1/fy0;
	double cx0 = mK0(0,2); double cy0 = mK0(1,2);

	double fx = mK(0,0); double fy = mK(1,1);
	double fx_1 = 1/fx;  double fy_1 = 1/fy;
	double cx = mK(0,2); double cy = mK(1,2);

	Matx33d mKR = mK*mR;
	Matx31d mKt = mK*mt;

	for (i=0;i<h;i++)
	{
		double nimgy0 = (i-cy0)*fy0_1;

		for (j=0;j<w;j++)
		{
			double score = mScore.at<float>(i,j);

			if (score < thresh_ncc)
			{
				mScore.at<float>(i,j) = -1; // if score is 0, it is also set to -1
				continue;
			}

			double nimgx0 = (j-cx0)*fx0_1;

			double depth  = mDepth.at<float>(i,j);
			double hx = mHx.at<float>(i,j);
			double hy = mHy.at<float>(i,j);

			// compute corresponding 3d point
			Matx31d XYZ = GetXYZ_givenDepth(mR0, mt0, nimgx0, nimgy0, depth);

			// compute corresponding normal
			Matx31d mn0; mn0(2)=1;
			get_normal_givendrhxhy(fx0, fy0, nimgx0, nimgy0, depth, hx, hy, mn0(0), mn0(1));
			Matx31d mnw = -mR0.t()*mn0; // convert the normal into world coordinate system
			double nmnw = norm(mnw);

			// project
			Matx31d mx = mKR*XYZ + mKt;

			double z_1 = 1/mx(2);

			double img_x = mx(0)*z_1;
			double img_y = mx(1)*z_1;

			// current line of sight
			Matx31d msightvec; // from image point to optical center
			msightvec(0) = -(img_x-cx)*fx_1;
			msightvec(1) = -(img_y-cy)*fy_1;
			msightvec(2) = -1;
			Matx31d mnk = mR*mnw;
			Matx<double, 1, 1> mcosa = mnk.t()*msightvec;
			double cosa = mcosa(0)/(nmnw*norm(msightvec));

			if (cosa<=cos_thresh_ang)
			{
				mScore.at<float>(i,j) = -1; // ang is supposed to be smaller than 90 if it is visible
			} 
		}
	}
}

// 计算角度的cos值，输入为角度
double DeepVoid_MVS::cosd(double angle)
{
	double radian = angle * CV_PI / 180;

	return cos(radian);
}

// 20140914, self-contained version, complete MRF model, data term is considered, and smoothness term is used on both depth and normals
void DeepVoid_MVS::Extract_MRF_ncc_d_n_DP_withInvalids(const Matx33d & mK0,			// input:	mK0 reference image
													   const Matx33d & mR0,			// input:	mR0 reference image
													   const Matx31d & mt0,			// input:	mt0 reference image
													   const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
													   const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
													   const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
													   const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
													   Mat & mSel,					// output:	selected best support image index for each pixel who generates the most likely depth with reference image
													   Mat & mDepth,				// output:	selected best depth map
													   Mat & mHx,					// output:	selected best hx map
													   Mat & mHy,					// output:	selected best hy map
													   Mat & mScore,				// output:	selected best score map
													   double P1,					// input:
													   double P2					// input:
													   )
{
	double pi_1 = 1.0/CV_PI; // 1/pi

	int i,j,k;

	int nCam = vDepths.size();

	int w = vDepths[0].cols;
	int h = vDepths[0].rows;

	double fx0_1 = 1/mK0(0,0);
	double fy0_1 = 1/mK0(1,1);

	vector<Mat> C_dir, S_dirs;

	for (i=0;i<nCam;i++)
	{
		Mat mtmp1(h, w, CV_32FC1, Scalar(0)), mtmp2(h, w, CV_32FC1, Scalar(0));
		C_dir.push_back(mtmp1);
		S_dirs.push_back(mtmp2); // cannot push the same Mat into two vectors, otherwise C_dir[k] will have the same memory location as S_dirs[k]
	}

	vector<double> Lr_1(nCam), Lr(nCam);


	/*------------ 01 direction --------------------------------------------------------------------------*/
	// dir = [1, 0] i.e. along positive direction of x-axis or the 0 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 01 starts");
	for (i=0;i<h;i++) // left image border
	{
		for (k=0;k<nCam;k++)
		{
			double score = vScores[k].at<float>(i,0);
			if (score<0)
			{
				C_dir[k].at<float>(i,0) = -1;
			}
			else
			{
				C_dir[k].at<float>(i,0) = 1-score;
			}
		}
	}

	for (j=1;j<w;j++)
	{
		for (i=0;i<h;i++)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i,j-1);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j-1, i, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 02 direction --------------------------------------------------------------------------*/
	// dir = [-1, 0] i.e. along negative direction of x-axis or the 180 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 02 starts");
	for (i=0;i<h;i++) // right image border
	{
		for (k=0;k<nCam;k++)
		{
			double score = vScores[k].at<float>(i,w-1);
			if (score<0)
			{
				C_dir[k].at<float>(i,w-1) = -1;
			} 
			else
			{
				C_dir[k].at<float>(i,w-1) = 1-score;
			}
		}
	}

	for (j=w-2;j>-1;j--)
	{
		for (i=0;i<h;i++)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i,j+1);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j+1, i, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 03 direction --------------------------------------------------------------------------*/
	// dir = [0, 1] i.e. along positive direction of y-axis or the -90 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 03 starts");
	for (k=0;k<nCam;k++) // top image border
	{
		for (j=0;j<w;j++)
		{
			double score = vScores[k].at<float>(0,j);
			if (score<0)
			{
				C_dir[k].at<float>(0,j) = -1;
			} 
			else
			{
				C_dir[k].at<float>(0,j) = 1-score;
			}
		}
	}

	for (i=1;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i-1,j);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j, i-1, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 04 direction --------------------------------------------------------------------------*/
	// dir = [0, -1] i.e. along negative direction of y-axis or the 90 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 04 starts");
	for (k=0;k<nCam;k++) // bottom image border
	{
		for (j=0;j<w;j++)
		{
			double score = vScores[k].at<float>(h-1,j);
			if (score<0)
			{
				C_dir[k].at<float>(h-1,j) = -1;
			} 
			else
			{
				C_dir[k].at<float>(h-1,j) = 1-score;
			}
		}
	}

	for (i=h-2;i>-1;i--)
	{
		for (j=0;j<w;j++)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i+1,j);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j, i+1, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 05 direction --------------------------------------------------------------------------*/
	// dir = [1, 1] i.e. along the -45 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 05 starts");
	for (i=0;i<h;i++) // left image border
	{
		for (k=0;k<nCam;k++)
		{
			double score = vScores[k].at<float>(i,0);
			if (score<0)
			{
				C_dir[k].at<float>(i,0) = -1;
			} 
			else
			{
				C_dir[k].at<float>(i,0) = 1-score;
			}
		}
	}
	for (k=0;k<nCam;k++) // top image border
	{
		for (j=1;j<w;j++)
		{
			double score = vScores[k].at<float>(0,j);
			if (score<0)
			{
				C_dir[k].at<float>(0,j) = -1;
			} 
			else
			{
				C_dir[k].at<float>(0,j) = 1-score;
			}
		}
	}

	for (i=1;i<h;i++)
	{
		for (j=1;j<w;j++)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i-1,j-1);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j-1, i-1, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 06 direction --------------------------------------------------------------------------*/
	// dir = [-1, -1] i.e. along the 135 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 06 starts");
	for (i=0;i<h;i++) // right image border
	{
		for (k=0;k<nCam;k++)
		{
			double score = vScores[k].at<float>(i,w-1);
			if (score<0)
			{
				C_dir[k].at<float>(i,w-1) = -1;
			} 
			else
			{
				C_dir[k].at<float>(i,w-1) = 1-score;
			}
		}
	}
	for (k=0;k<nCam;k++) // bottom image border
	{
		for (j=0;j<w-1;j++)
		{
			double score = vScores[k].at<float>(h-1,j);
			if (score<0)
			{
				C_dir[k].at<float>(h-1,j) = -1;
			} 
			else
			{
				C_dir[k].at<float>(h-1,j) = 1-score;
			}	
		}
	}

	for (i=h-2;i>-1;i--)
	{
		for (j=w-2;j>-1;j--)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i+1,j+1);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j+1, i+1, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 07 direction --------------------------------------------------------------------------*/
	// dir = [1, -1] i.e. along the 45 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 07 starts");
	for (i=0;i<h;i++) // left image border
	{
		for (k=0;k<nCam;k++)
		{
			double score = vScores[k].at<float>(i,0);
			if (score<0)
			{
				C_dir[k].at<float>(i,0) = -1;
			} 
			else
			{
				C_dir[k].at<float>(i,0) = 1-score;
			}
		}
	}
	for (k=0;k<nCam;k++) // bottom image border
	{
		for (j=1;j<w;j++)
		{
			double score = vScores[k].at<float>(h-1,j);
			if (score<0)
			{
				C_dir[k].at<float>(h-1,j) = -1;
			} 
			else
			{
				C_dir[k].at<float>(h-1,j) = 1-score;
			}
		}
	}

	for (i=h-2;i>-1;i--)
	{
		for (j=1;j<w;j++)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i+1,j-1);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j-1, i+1, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	/*------------ 08 direction --------------------------------------------------------------------------*/
	// dir = [-1, 1] i.e. along the -135 direction
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("scan direction 08 starts");
	for (i=0;i<h;i++) // right image border
	{
		for (k=0;k<nCam;k++)
		{
			double score = vScores[k].at<float>(i,w-1);
			if (score<0)
			{
				C_dir[k].at<float>(i,w-1) = -1;
			} 
			else
			{
				C_dir[k].at<float>(i,w-1) = 1-score;
			}
		}
	}
	for (k=0;k<nCam;k++) // top image border
	{
		for (j=0;j<w-1;j++)
		{
			double score = vScores[k].at<float>(0,j);
			if (score<0)
			{
				C_dir[k].at<float>(0,j) = -1;
			} 
			else
			{
				C_dir[k].at<float>(0,j) = 1-score;
			}
		}
	}

	for (i=1;i<h;i++)
	{
		for (j=w-2;j>-1;j--)
		{
			for (k=0;k<nCam;k++)
			{
				Lr_1[k] = C_dir[k].at<float>(i-1,j+1);
			}
			DP_step_withInvalids(mK0, mR0, mt0, fx0_1, fy0_1, j+1, i-1, j, i, vDepths, vHxs, vHys, vScores, pi_1, P1, P2, Lr_1, Lr);
			for (k=0;k<nCam;k++)
			{
				C_dir[k].at<float>(i,j) = Lr[k];
			}
		}
	}
	// add the 3D aggregated cost to the sum
	for (k=0;k<nCam;k++)
	{
		S_dirs[k] += C_dir[k];
	}

	//////////////////////////////////////////////////////////////////////////
	// get the 
	mSel = Mat(h, w, CV_8UC1);
	mDepth = Mat(h, w, CV_32FC1);
	mHx = Mat(h, w, CV_32FC1);
	mHy = Mat(h, w, CV_32FC1);
	mScore = Mat(h, w, CV_32FC1);

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			vector<double> vss;
			vector<int> vss_idx;
			for (k=0;k<nCam;k++)
			{
				double s = S_dirs[k].at<float>(i,j);
				if (s>0)
				{
					vss.push_back(s);
					vss_idx.push_back(k);
				}
			}

			if (vss.size()==0)
			{
				mDepth.at<float>(i,j) = vDepths[0].at<float>(i,j);
				mHx.at<float>(i,j) = vHxs[0].at<float>(i,j);
				mHy.at<float>(i,j) = vHys[0].at<float>(i,j);
				mScore.at<float>(i,j) = vScores[0].at<float>(i,j);
				mSel.at<uchar>(i,j) = 0;
			} 
			else
			{
				vector <double>::iterator iterDouble = min_element(vss.begin(), vss.end());
				int idx_min_score = iterDouble - vss.begin();

				mDepth.at<float>(i,j) = vDepths[vss_idx[idx_min_score]].at<float>(i,j);
				mHx.at<float>(i,j) = vHxs[vss_idx[idx_min_score]].at<float>(i,j);
				mHy.at<float>(i,j) = vHys[vss_idx[idx_min_score]].at<float>(i,j);
				mScore.at<float>(i,j) = vScores[vss_idx[idx_min_score]].at<float>(i,j);
				mSel.at<uchar>(i,j) = (uchar)vss_idx[idx_min_score];
			}
		}
	}
}

// DP used in complete MRF model, including data term, smoothness terms on both depth and normals
void DeepVoid_MVS::DP_step_withInvalids(const Matx33d & mK,			// input:	mK0 reference image
										const Matx33d & mR,			// input:	mR0 reference image
										const Matx31d & mt,			// input:	mt0 reference image
										double fx_1, 				// input:	fx0_1 = 1/fx0
										double fy_1,				// input:	fy0_1 = 1/fy0
										int xr_1, int yr_1,			// input:	previous pixel
										int xr, int yr,				// input:	current pixel
										const vector<Mat> & vDepths,// input:	all depth candidates
										const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
										const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
										const vector<Mat> & vScores,
										double pi_1,				// input:	1/π
										double P1,					// input:
										double P2,					// input:
										const vector<double> & Lr_1,
										vector<double> & Lr
										)
{
//	double pi_2_1 = 2*pi_1; // 1/(0.5*pi)

	int i,j;

	int nCam = vDepths.size();

	vector<int> idxValid;
	int nValid = 0;
	double min_Lr_1;

	// find all the valid depths in Lr_1, whose aggregated cost are not negative
	for (i=0;i<nCam;i++)
	{
		if (Lr_1[i]<0)
		{
			continue;
		}

		// find the minimal nonnegative value at the same time
		if (!nValid)
		{
			min_Lr_1 = Lr_1[i];
		} 
		else
		{
			if (Lr_1[i] < min_Lr_1)
			{
				min_Lr_1 = Lr_1[i];
			}
		}

		idxValid.push_back(i);
		nValid++;
	}

	// if nValid=0, it means that previous pixels definitly have no matches at all
	// then we take the right pixel next to it to be the starting pixel for cost aggregation
	if (!nValid)
	{
		for (i=0;i<nCam;i++)
		{
			double score = vScores[i].at<float>(yr,xr);
			if (score<0)
			{
				Lr[i] = -1;
			} 
			else
			{
				Lr[i] = 1-score; // the true matching cost is 1-ncc, which means the bigger ncc is, the smaller the cost is
			}
		}

		return;
	}

	// compute corresponding normalized image point of previous pixel and current pixel, i.e. the sight direction vector
	double fx = mK(0,0); double fy = mK(1,1);
	double cx = mK(0,2); double cy = mK(1,2);
	double nimgx0 = (xr_1-cx)*fx_1; double nimgy0 = (yr_1-cy)*fy_1;
	double nimgx  = (xr  -cx)*fx_1; double nimgy  = (yr  -cy)*fy_1;
// 	Matx31d dir0; dir0(0) = nimgx0; dir0(1) = nimgy0; dir0(2) = 1; // direction of previous pixel
// 	Matx31d dir;   dir(0) = nimgx;   dir(1) = nimgy;   dir(2) = 1; // direction of current pixel

	vector <double>::const_iterator iterDouble;

	for (i=0;i<nCam;i++)
	{
		// if the matching cost at depth Cp(i) is negative i.e. invalid, it means that the matching cost between p and p-d is enormous
		// then there is no need to check this depth
		double score = vScores[i].at<float>(yr,xr);
		if (score<0)
		{
			Lr[i] = -1;
			continue;
		}

		double di = vDepths[i].at<float>(yr,xr);
		double hxi = vHxs[i].at<float>(yr,xr);
		double hyi = vHys[i].at<float>(yr,xr);
		
// 		Matx31d veci = di*dir; // the sight line vector of current pixel with depth di
// 		double nvi = norm(veci);

		// normal vec
		Matx31d faxi; faxi(2)=1;
		get_normal_givendrhxhy(fx, fy, nimgx, nimgy, di, hxi, hyi, faxi(0), faxi(1));
		double nfaxi = norm(faxi);
		Matx13d faxi_t = faxi.t();

		vector<double> vCost;

		for (j=0;j<nValid;j++)
		{
			double dj = vDepths[idxValid[j]].at<float>(yr_1,xr_1);
			double hxj = vHxs[idxValid[j]].at<float>(yr_1,xr_1);
			double hyj = vHys[idxValid[j]].at<float>(yr_1,xr_1);

			// 1. depth smoothness cost by converting depth difference into an angle
// 			Matx31d vecj = dj*dir0; // the sight line vector of previous pixel with depth dj
// 			Matx31d dv = vecj-veci;
// 			double ndv = norm(dv);
// 			Matx<double,1,1> cosa = veci.t() * dv;
// 			double sinb = fabs(cosa(0)/(nvi*ndv)); // sin(beta) = |cos(alpha)| see my notebook
// 			double beta = asin(sinb);    // 0-90
//			double cost_d = beta*pi_2_1; // normalize the cost

			// 1. depth smoothness cost by converting depth difference into relative difference
			double cost_d = 2*fabs((dj-di)/(di+dj));

			// 2. normal smoothness cost
			Matx31d faxj; faxj(2)=1;
			get_normal_givendrhxhy(fx,fy,nimgx0,nimgy0,dj,hxj,hyj,faxj(0),faxj(1));
			double nfaxj = norm(faxj);
			Matx<double,1,1> cosa = faxi_t*faxj;
			double tmp = cosa(0)/(nfaxi*nfaxj);
			if (tmp>1)
			{
				tmp=1;
			}
			if (tmp<-1)
			{
				tmp=-1;
			}
			double rad = acos(tmp); // 0-180
			double cost_n = rad*pi_1;

			// compute cost in total
			double cost = Lr_1[idxValid[j]] + P1*cost_d + P2*cost_n;
			vCost.push_back(cost);
		}

		iterDouble = min_element(vCost.begin(), vCost.end());
		double minCost = *iterDouble;

		// 3. the data term cost is finally summed
		Lr[i] = (1-score)+minCost-min_Lr_1;
	}
}

// 20141218, self-contained version, take into account not only the depth but also the normal
// one support image produce multiple candidates from different scale
// given dij denoting depth map generated from ith pyramid level with jth support image
// the depth maps in input vDepths are arranged as [d00 | d01 | d10 | d11 | d20 | d21 | d30 | d31]
void DeepVoid_MVS::AugmentVisibility_basedonMostLikelyDepthandNormals_SURE_MultiScales(const Matx33d & mK0,			// input:	interior matrix of the reference image
																					   const Matx33d & mR0,			// input:	rotation matrix of the reference image
																					   const Matx31d & mt0,			// input:	translation vector of the reference image
																					   const vector<Matx33d> & vKs,	// input:	interior matrix of all support images
																					   const vector<Matx33d> & vRs,	// input:	rotation matrix of all support images
																					   const vector<Matx31d> & vts,	// input:	translation vectors of all support images
																					   const vector<Mat> & vDepths,	// input:	all depth map relative to reference wrt each support image
																					   const vector<Mat> & vHxs,	// input:	all hx map relative to reference wrt each support image
																					   const vector<Mat> & vHys,	// input:	all hy map relative to reference wrt each support image
																					   const vector<Mat> & vScores,	// input:	all score map relative to reference wrt each support image
																					   const Mat & mSel,			// input:	selected best support image index for each pixel who generates the most likely depth with reference image
																					   Mat & mVisi,					// output:	augmented visibility within support image set based on selected most likely depth
																					   double thresh_sigma /*= 1*/,	// input:	image coordinate threshold for grouping depths with the most likely depth
																					   double thresh_ang /*= 90*/	// input:
																					   )
{
	int i,j,k;

	double cos_thresh_ang = cosd(thresh_ang);

	int n_spt = vKs.size(); // total number of support images

	int n_candidates = vDepths.size(); // total number of candidates

	int n_level = n_candidates/n_spt; // total number of scales

	int w = vDepths[0].cols;
	int h = vDepths[0].rows;

	double fx0 = mK0(0,0);
	double fy0 = mK0(1,1);
	double fx0_1 = 1/fx0;
	double fy0_1 = 1/fy0;
	double cx0 = mK0(0,2);
	double cy0 = mK0(1,2);

	Mat mVisiN(h, w, CV_8UC1);
	mVisi = Mat(h, w, CV_8UC1);

	for (i=0;i<h;i++)
	{
		double nimgy0 = (i-cy0)*fy0_1;
		for (j=0;j<w;j++)
		{
			uchar bestImg = mSel.at<uchar>(i,j);
			double score_bst = vScores[bestImg].at<float>(i,j);
			if (score_bst<0)
			{
				// if the chosen best depth is invalid, then no need to augment visibility
				// because there are definately no other valid depths available
				mVisi.at<uchar>(i,j) = 0;
				mVisiN.at<uchar>(i,j) = 0;
				continue;
			}

			double nimgx0 = (j-cx0)*fx0_1;

			Matx31d vNImg; // the normalized image point i.e. the direction of the line of sight
			vNImg(0) = nimgx0;
			vNImg(1) = nimgy0;
			vNImg(2) = 1;
			Matx31d vRrNImg = mR0.t()*vNImg;  // Rr'nx

			vector<Matx31d> vNormals; vector<double> vfax_norm_1;
			vector<double> vdmin, vdmax;

			// we first compute corresponding image points of every depth values in its corresponding image
			for (k=0;k<n_candidates;k++)
			{
				int idxSpt = k%n_spt;				

				double dk = vDepths[k].at<float>(i,j);
				double hxk = vHxs[k].at<float>(i,j);
				double hyk = vHys[k].at<float>(i,j);

				Matx31d XYZ = GetXYZ_givenDepth(mR0,mt0,nimgx0,nimgy0,dk);
				// project
				Matx33d mKR = vKs[idxSpt]*vRs[idxSpt];
				Matx31d mKt = vKs[idxSpt]*vts[idxSpt];
				Matx31d mx = mKR*XYZ + mKt;
				double z_1 = 1/mx(2);

				Matx31d a = mKR*vRrNImg;

				Point2d pt2d;
				pt2d.x = mx(0)*z_1;
				pt2d.y = mx(1)*z_1;

				double ddmin, ddmax;
				ComputeDepthUncertainBoundary(pt2d.x, pt2d.y, mx(2), a(0), a(1), a(2), ddmin, ddmax, thresh_sigma);
				double dmin = dk+ddmin;
				double dmax = dk+ddmax;
				vdmin.push_back(dmin);
				vdmax.push_back(dmax);

				// 20140908
				// compute the normal of the best estimate
				Matx31d faxk; faxk(2)=1;
				get_normal_givendrhxhy(fx0, fy0, nimgx0, nimgy0, dk, hxk, hyk, faxk(0), faxk(1));
				double nfaxk_1 = 1/norm(faxk);
				vNormals.push_back(faxk);
				vfax_norm_1.push_back(nfaxk_1);
			}

			Matx13d best_norm_t = vNormals[bestImg].t(); // n'

			vector<bool> vbvisi(n_spt);
			for (k=0;k<n_spt;k++){vbvisi[k] = false;}
			int idxSpt = bestImg%n_spt;
			vbvisi[idxSpt] = true;

			uchar visiN = 1;

			for (k=0;k<n_candidates;k++)
			{
				// if current checked image is the best image itself
				// or if it is a invalid depth, then continue, no need to cluster this depth
				if (k==bestImg){continue;}
				if (vScores[k].at<float>(i,j)<0){continue;}

				// we first check if current normal estimate is consistent with the best estimate or not
				Matx<double,1,1> cosa = best_norm_t * vNormals[k];
				double cosa_d = cosa(0)*vfax_norm_1[bestImg]*vfax_norm_1[k];
				if (cosa_d<cos_thresh_ang) // this means the angle between the best normal and current normal is bigger than certain threshold
				{
					continue;
				}
				//////////////////////////////////////////////////////////////////////////

				// then we check if kth uncertainty range overlap the most likely uncertainty range or not
				if (vdmin[bestImg]>vdmax[k] || vdmin[k]>vdmax[bestImg])
				{
					continue; // no overlap
				} 
				else
				{
					idxSpt = k%n_spt; // index of currently checked support image
					if (!vbvisi[idxSpt])
					{
						vbvisi[idxSpt] = true; // overlaps
						visiN++;
					}
				}
			}

			uchar visi = GetVisibilityVector_uchar(vbvisi);

			mVisi.at<uchar>(i,j) = visi;
			mVisiN.at<uchar>(i,j) = visiN;
		}
	}
}

// 20141013
void DeepVoid_MVS::ComputeDepthUncertainBoundary(double imgxj0, double imgyj0,		// input:	the matching image point in support image
												 double dj0,						// input:	the depth of the reconstructed object point w.r.t the support image
												 double a1, double a2, double a3,	// input:	[a1 a2 a3]' = KjRjRr'nx
												 double & ddmin,					// output:	the depth decrement of the lower boundary w.r.t the reconstructed depth
												 double & ddmax,					// output:	the depth decrement of the upper boundary w.r.t the reconstructed depth
												 double thresh_imgpt_sigma /*= 1*/	// input:	the uncertainty of the matching image point 		
												 )
{
	double sigma2 = thresh_imgpt_sigma*thresh_imgpt_sigma;

	double tmp1 = a1 - a3*imgxj0;
	double tmp2 = a2 - a3*imgyj0;

	double a = tmp1*tmp1 + tmp2*tmp2 - sigma2*a3*a3;
	double b = -2*sigma2*a3*dj0;
	double c = -sigma2*dj0*dj0;

	double tmp3 = sqrt(b*b - 4*a*c);
	double tmp4 = 0.5/a;

	ddmin = (-b - tmp3) * tmp4;
	ddmax = (-b + tmp3) * tmp4;
}

uchar DeepVoid_MVS::GetVisibilityVector_uchar(const vector<bool> & vbools)
{
	int i;

	int n = vbools.size();

	uchar visi = 0;

	for (i=n-1;i>=0;i--)
	{
		visi<<=1;

		if (vbools[i])
		{
			++visi;
		}
	}

	return visi;
}

// 20150207, self-contained, visibility determined, use the most likely values of depth,hx and hy as initial values to optimize final depth and normal by MPGC
void DeepVoid_MVS::MPGC_20150207(const Matx33d & mK0,					// input:	interior matrix of reference image
								 const Matx33d & mR0,					// input:	rotation matrix of reference image
								 const Matx31d & mt0,					// input:	translation vector of reference image
								 const Mat & img0,						// input:	reference image
								 const vector<Matx33d> & vKs,			// input:	interior matrix of all support images
								 const vector<Matx33d> & vRs,			// input:	rotation matrix of all support images
								 const vector<Matx31d> & vts,			// input:	translation vectors of all support images
								 const vector<Mat> & vImgs,				// input:	images of all support images
								 const Mat & mDepth_ML,
								 const Mat & mHx_ML,
								 const Mat & mHy_ML,
								 const Mat & mScore_ML,
								 const Mat & mVisi,						// input:	augmented visibility within support image set based on selected most likely depth
								 Mat & mDepth,							// output:	fused depths
								 Mat & mHx,								// output:	fused hxs
								 Mat & mHy,								// output:	fused hys
								 Mat & mScore,							// output:	fused scores
								 int pw,								// input:	image patch width
								 int ph,								// input:	image patch height
								 int maxIter /*= 128*/,
								 double xEps /*= 1.0E-8*/,				// input: threshold
								 double fEps /*= 1.0E-6*/				// input: threshold
								 )
{
	int i,j,k,ii,jj;

	int w = mDepth_ML.cols;
	int h = mDepth_ML.rows;

	int n_spt = vKs.size(); // number of support images

	int hpw = (pw-1)*0.5; // half patch width
	int hph = (ph-1)*0.5; // half patch height

	Mat mMask(ph, pw, CV_8UC1, Scalar(1));

	double min_depth, max_depth;
	minMaxIdx(mDepth_ML, &min_depth, &max_depth);

	mDepth = Mat(h, w, CV_32FC1);
	mHx = Mat(h, w, CV_32FC1);
	mHy = Mat(h, w, CV_32FC1);
	mScore = Mat(h, w, CV_32FC1);

	CString strInfo;

	for (i=0;i<h;i++)
	{
		strInfo.Format("Multi-View MPGC-optimizing row %04d", i);
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		for (j=0;j<w;j++)
		{
			uchar visi = mVisi.at<uchar>(i,j);
			vector<bool> vbools; int nVisi;
			InterpVisiVector_uchar(visi, vbools, &nVisi);

			if (nVisi<1) // there got be at least 1 observation to start MPGC optimization
			{
				mDepth.at<float>(i,j) = mDepth_ML.at<float>(i,j);
				mHx.at<float>(i,j) = mHx_ML.at<float>(i,j);
				mHy.at<float>(i,j) = mHy_ML.at<float>(i,j);
				mScore.at<float>(i,j) = mScore_ML.at<float>(i,j);
				continue;
			}

			double depth_init = mDepth_ML.at<float>(i,j);	// initial depth is the most likely value
			double hx_init = mHx_ML.at<float>(i,j);			// initial hx is the most likely value
			double hy_init = mHy_ML.at<float>(i,j);			// initial hy is the most likely value
			double score_init = mScore_ML.at<float>(i,j);	// initial ncc value is the most likely value

			int i_real, j_real;
			MakeSureNotOutBorder(j,i,j_real,i_real,hpw,w,h);

			//////////////////////////////////////////////////////////////////////////
			// generate mask
			vector<Mat> vMasks_visi; vector<int> vNum_visi;
			vector<Matx33d> vKs_visi, vRs_visi; vector<Matx31d> vts_visi; vector<Mat> vImgs_visi;
			for (k=0;k<n_spt;k++)
			{
				if (vbools[k])
				{
					vMasks_visi.push_back(mMask); // 掩膜的值全为1，说明掩膜没起作用，区域内的所有像素都参与MPGC
					vNum_visi.push_back(pw*ph);
					vKs_visi.push_back(vKs[k]);
					vRs_visi.push_back(vRs[k]);
					vts_visi.push_back(vts[k]);
					vImgs_visi.push_back(vImgs[k]);
				}
			}
			//////////////////////////////////////////////////////////////////////////

			double depth_optim, hx_optim, hy_optim, score_optim;

			if (optim_gn_drhxhyck_NCCcontrolled_masks(mK0,mR0,mt0,img0,vKs_visi,vRs_visi,vts_visi,vImgs_visi,vMasks_visi,vNum_visi,j_real,i_real,pw,ph,
				depth_init,hx_init,hy_init,score_init,depth_optim,hx_optim,hy_optim,score_optim,maxIter,xEps,fEps))
			{
				// the optimized depth must be within certain range, or all parameters are mean values
				if (depth_optim>min_depth && depth_optim<max_depth)
				{
					mDepth.at<float>(i,j) = depth_optim;
					mHx.at<float>(i,j) = hx_optim;
					mHy.at<float>(i,j) = hy_optim;
					mScore.at<float>(i,j) = score_optim;
					continue;
				}
			}

			mDepth.at<float>(i,j) = depth_init;
			mHx.at<float>(i,j) = hx_init;
			mHy.at<float>(i,j) = hy_init;
			mScore.at<float>(i,j) = score_init;
		}
	}
}

// if visi=0001 0010, then vec=[f t f f t f f f]
void DeepVoid_MVS::InterpVisiVector_uchar(uchar visi, vector<bool> & vec, int * nValid /*= NULL*/)
{
	vec.clear();

	int nnn = 0;

	int i;
	for (i=0;i<8;i++)
	{
		uchar bit = pow(2.0,i);

		if (bit&visi)
		{
			vec.push_back(true);
			++nnn;
		} 
		else
		{
			vec.push_back(false);
		}
	}

	if (nValid)
	{
		*nValid = nnn;
	}
}

void DeepVoid_MVS::SaveParaField2Img(CString & path, const Mat & field)
{
	Mat mField_map;

	double min_val, max_val;

	minMaxIdx(field, &min_val, &max_val);

	field.convertTo(mField_map, CV_8UC1, 255/(max_val-min_val), -255*min_val/(max_val-min_val));

	imwrite(path.GetBuffer(), mField_map);
}

void DeepVoid_MVS::SaveScoreField2Img(CString & path, const Mat & mScore)
{
	int i,j;

	int h = mScore.rows;
	int w = mScore.cols;

	Mat mScore_map(h, w, CV_8UC3);

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			if (mScore.at<float>(i,j)<0)
			{
				mScore_map.at<Vec3b>(i,j).val[0] = 0;
				mScore_map.at<Vec3b>(i,j).val[1] = 0;
				mScore_map.at<Vec3b>(i,j).val[2] = 255;
			}
			else
			{
				mScore_map.at<Vec3b>(i,j).val[0] = 0;
				mScore_map.at<Vec3b>(i,j).val[1] = FTOI(255*mScore.at<float>(i,j));
				mScore_map.at<Vec3b>(i,j).val[2] = 0;
			}
		}
	}

	imwrite(path.GetBuffer(), mScore_map);
}

// 20140913, self-contained version
void DeepVoid_MVS::DepthConsistencyCheck(const Matx33d & mK0,				// input:	interior matrix of reference image
										 const Matx33d & mR0,				// input:	rotation matrix of reference image
										 const Matx31d & mt0,				// input:	translation vector of reference image
										 const Mat & mDepth0,				// input:	depth map of reference image
										 const Mat & mVisi0,				// input:	visibility map of reference image
										 Mat & mScore0,						// output:	pixels do not pass check are set to invalids
										 const vector<Matx33d> & vKs,		// input:	interior matrix of all support images
										 const vector<Matx33d> & vRs,		// input:	rotation matrix of all support images
										 const vector<Matx31d> & vts,		// input:	translation vectors of all support images
										 const vector<Mat> & vDepths,		// input:	depth maps of all support images
										 const vector<Mat> & vHxs,			// input:	hx maps of all support images
										 const vector<Mat> & vHys,			// input:	hy maps of all support images
										 double thresh_ratio /*= 0.001*/
										 )
{
	int i,j,k;

	int w = mDepth0.cols;
	int h = mDepth0.rows;

	int n_spt = vKs.size(); // number of support images

	double fx0_1 = 1/mK0(0,0);
	double fy0_1 = 1/mK0(1,1);
	double cx0 = mK0(0,2);
	double cy0 = mK0(1,2);

	for (i=0;i<h;i++)
	{
		double nimgy0 = (i-cy0)*fy0_1;
		for (j=0;j<w;j++)
		{
			double score0 = mScore0.at<float>(i,j);
			if (score0<0){continue;} // this pixel is invalid already

			double nimgx0 = (j-cx0)*fx0_1;

			double depth0 = mDepth0.at<float>(i,j);
			Matx31d XYZ = GetXYZ_givenDepth(mR0,mt0,nimgx0,nimgy0,depth0);

			uchar visi = mVisi0.at<uchar>(i,j);

			vector<bool> vbools;
			InterpVisiVector_uchar(visi, vbools);

			bool bfailed = true;

			for (k=0;k<n_spt;k++)
			{
				if (!vbools[k]){continue;}

				Matx31d XYZ_k = vRs[k]*XYZ+vts[k];

				Matx31d imgpt = vKs[k]*XYZ_k;
				double zk_1 = 1/imgpt(2);
				double xk = imgpt(0)*zk_1;
				double yk = imgpt(1)*zk_1;

				if (xk<0 || xk>w-1 || yk<0 || yk>h-1)
				{
					continue;
				}

				int xk_int = int(xk+0.5);
				int yk_int = int(yk+0.5);

				double u = xk-xk_int;
				double v = yk-yk_int;

				double dk = vDepths[k].at<float>(yk_int, xk_int);
				double hxk = vHxs[k].at<float>(yk_int, xk_int);
				double hyk = vHys[k].at<float>(yk_int, xk_int);

				double ddd = dk + u*hxk+v*hyk;

				double ratio = fabs((XYZ_k(2)-dk)/XYZ_k(2)); // does not take into account depth gradient estimate in k

				if (ratio<thresh_ratio)
				{
					bfailed = false;
					break;
				}
			}

			if (bfailed)
			{
				mScore0.at<float>(i,j) = -1;
			}
		}
	}
}

// 20140913, self-contained version
void DeepVoid_MVS::RemoveSmallDepthRegions_4(const Matx33d & mK,			// input:	reference image
											 const Matx33d & mR,			// input:	reference image
											 const Matx31d & mt,			// input:	reference image
											 const Mat & mDepth,
											 Mat & mScore,
											 double thresh_ratio /*= 0.01*/,
											 int thresh_area /*= 15*/
											 )
{
	int i,j,k;

	int w = mDepth.cols;
	int h = mDepth.rows;

	Mat mStatus(h, w, CV_8UC1, Scalar(0)); // if value is 0 it is unchecked, if 1 it is checked

	for (i=0;i<h;i++)
	{
		for (j=0;j<w;j++)
		{
			double score = mScore.at<float>(i,j);
			uchar status = mStatus.at<uchar>(i,j);

			if (status==1){continue;}

			if (score<0){mStatus.at<uchar>(i,j)=1;continue;} // this pixel is already a invalid pixel or it's been checked to be a valid one

			// Breadth-first searching and clustering
			// create a queue FIFO (first in first out)
			list<Point2i> queue;
			vector<Point2i> cluster;

			Point2i pt2d;
			pt2d.x = j; pt2d.y = i;

			queue.push_back(pt2d);
			cluster.push_back(pt2d);
			mStatus.at<uchar>(i,j)=1;

			while (queue.size() > 0)
			{
				Point2i pt2d0 = queue.front(); // can not be int & t = queue.front() here, since pop_front() will delete the first element and t will point nowhere if it is a reference to the first element
				queue.pop_front();

				int i0 = pt2d0.y; int j0 = pt2d0.x;

				double d0 = mDepth.at<float>(i0,j0);

				int l = j0-1; int r = j0+1; int u = i0-1; int b = i0+1;

				// first, check the left pixel if there is one
				if (l>=0 && mScore.at<float>(i0,l)>0 && mStatus.at<uchar>(i0,l)==0)
				{
					double d = mDepth.at<float>(i0,l);
					
					double val = 0.5*thresh_ratio*(d+d0);

					if (fabs(d-d0)<val)
					{
						Point2i pt2d_new;
						pt2d_new.x = l; pt2d_new.y = i0;
						queue.push_back(pt2d_new);
						cluster.push_back(pt2d_new);
						mStatus.at<uchar>(i0,l)=1;
					}
				}

				// second, check the right pixel if there is one
				if (r<w && mScore.at<float>(i0,r)>0 && mStatus.at<uchar>(i0,r)==0)
				{
					double d = mDepth.at<float>(i0,r);
					
					double val = 0.5*thresh_ratio*(d+d0);

					if (fabs(d-d0)<val)
					{
						Point2i pt2d_new;
						pt2d_new.x = r; pt2d_new.y = i0;
						queue.push_back(pt2d_new);
						cluster.push_back(pt2d_new);
						mStatus.at<uchar>(i0,r)=1;
					}
				}

				// third, check the upper pixel if there is one
				if (u>=0 && mScore.at<float>(u,j0)>0 && mStatus.at<uchar>(u,j0)==0)
				{
					double d = mDepth.at<float>(u,j0);
					
					double val = 0.5*thresh_ratio*(d+d0);

					if (fabs(d-d0)<val)
					{
						Point2i pt2d_new;
						pt2d_new.x = j0; pt2d_new.y = u;
						queue.push_back(pt2d_new);
						cluster.push_back(pt2d_new);
						mStatus.at<uchar>(u,j0)=1;
					}
				}

				// last, check the bottom pixel if there is one
				if (b<h && mScore.at<float>(b,j0)>0 && mStatus.at<uchar>(b,j0)==0)
				{
					double d = mDepth.at<float>(b,j0);
					
					double val = 0.5*thresh_ratio*(d+d0);

					if (fabs(d-d0)<val)
					{
						Point2i pt2d_new;
						pt2d_new.x = j0; pt2d_new.y = b;
						queue.push_back(pt2d_new);
						cluster.push_back(pt2d_new);
						mStatus.at<uchar>(b,j0)=1;
					}
				}
			}

			if (cluster.size()<thresh_area)
			{
				for (k=0;k<cluster.size();k++)
				{
					Point2i pt2d_tmp = cluster[k];
					mScore.at<float>(pt2d_tmp.y, pt2d_tmp.x) = -1;
				}
			}
		}
	}
}

// 20150204, zhaokunz, try to score all other oriented images for dense matching with designated reference image based on certain measure
void DeepVoid_MVS::ScoreMatchingImages(const vector<cam_data> & allCams,			// input:	all images
									   const vector<CloudPoint> & clouds,			// input:	all reconstructed tracks
									   const vector<vector<DMatch>> & allMatches,	// input:	all pair-wise matches
									   int idxRefImg,								// input:	the index of the reference image
									   vector<Point2d> & scores,					// output:	scores of all other images, with x being the index, and y being the score, in descending order
									   double ang_desired /*= 15*/					// input:	the desired triangulation angle
									   )
{
	int i,j;

	int nCam = allCams.size();

	double ang_sigma = ang_desired/3.0;

	scores.clear();

	if (allCams[idxRefImg].R[0]<-90)
	{
		// the reference image has not been oriented yet
		return;
	}

	// first, compute the optical centers of all images
	vector<Matx31d> vCs;
	for (auto iter=allCams.begin();iter!=allCams.end();++iter)
	{
		if (iter->R[0]<-90)
		{
			Matx31d C;
			vCs.push_back(C);
			continue;
		}

		Matx33d mR;
		for (i=0;i<3;i++)
		{
			for (j=0;j<3;j++)
			{
				mR(i,j)=iter->R[i*3+j];
			}
		}

		Matx31d mt;
		mt(0) = iter->t[0];
		mt(1) = iter->t[1];
		mt(2) = iter->t[2];

		Matx31d mC = -mR.t()*mt;

		vCs.push_back(mC);
	}

	for (i=0;i<nCam;i++)
	{
		if (allCams[i].R[0]<-90||i==idxRefImg)
		{
			Point2d pt;
			pt.x = i;
			pt.y = -1;
			scores.push_back(pt);
			continue;
		}

		double sum_w = 0;

		vector<DMatch> matches = allMatches[idxRefImg*nCam+i];

		for (auto iter_match=matches.begin();iter_match!=matches.end();++iter_match)
		{
			int idxTrack = allCams[idxRefImg].m_feats.tracks[iter_match->queryIdx];

			auto iter_find_cloud = find_if(clouds.begin(),clouds.end(),[idxTrack](const CloudPoint & a){return a.m_idx==idxTrack;});

			if (iter_find_cloud==clouds.end())
			{
				// this track has not been reconstructed yet
				continue;
			}

			auto iter_find_imgpt_ref = find_if(iter_find_cloud->m_vImgInfos.begin(),iter_find_cloud->m_vImgInfos.end(),
				[idxRefImg](const CloudPoint_ImgInfo & a){return a.m_idxImg==idxRefImg;});
			auto iter_find_imgpt_i   = find_if(iter_find_cloud->m_vImgInfos.begin(),iter_find_cloud->m_vImgInfos.end(),
				[i](const CloudPoint_ImgInfo & a){return a.m_idxImg==i;});

			if (iter_find_imgpt_ref==iter_find_cloud->m_vImgInfos.end()||iter_find_imgpt_i==iter_find_cloud->m_vImgInfos.end())
			{
				// at least one of the matching features is not associated with the reconstructed cloud point
				continue;
			}

			// here, we confirm that we found a valid reconstructed matching feature
			Matx31d XYZ;
			XYZ(0) = iter_find_cloud->m_pt.x;
			XYZ(1) = iter_find_cloud->m_pt.y;
			XYZ(2) = iter_find_cloud->m_pt.z;

			Matx31d vec_ref = XYZ-vCs[idxRefImg];
			double nVec_ref = norm(vec_ref);

			Matx31d vec_i = XYZ-vCs[i];
			double nVec_i = norm(vec_i);

			Matx<double,1,1> cosa = vec_ref.t() * vec_i;
			double tmp = cosa(0)/(nVec_ref*nVec_i);
			if (tmp>1)
			{
				tmp=1;
			}
			if (tmp<-1)
			{
				tmp=-1;
			}
			double ang = acos(tmp)*R2D; // 0-180

			double w = exp_miu_sigma(ang,ang_desired,ang_sigma); // compute the weight

			sum_w+=w;
		}

		Point2d pt;
		pt.x = i;
		pt.y = sum_w;

		scores.push_back(pt);
	}

	sort(scores.begin(), scores.end(), [](const Point2d & a, const Point2d & b){return a.y>b.y;});
}

// compute exp((-1/2)*(x-miu)^2/sigma^2)
double DeepVoid_MVS::exp_miu_sigma(double x, double miu, double sigma)
{
	double dx = x-miu;
	double a = dx/sigma;
	double a2 = a*a;

	return exp(-0.5*a2);
}