// ImageListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "ImageListCtrl.h"

#include "ImageDoc.h"
#include "ImageView.h"

#include "MainFrm.h"


// CImageListCtrl

IMPLEMENT_DYNAMIC(CImageListCtrl, CListCtrl)

CImageListCtrl::CImageListCtrl()
{

}

CImageListCtrl::~CImageListCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageListCtrl, CListCtrl)
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CImageListCtrl message handlers

BOOL CImageListCtrl::InitImgListCtrl(int w, int h)
{
	int nCount = GetItemCount();
	
	for (int i = 0; i < nCount; ++i)
	{
		char * pData = (char *)GetItemData(i);

		if (pData)
		{
			free(pData);
		}
	}

	for (int j = 0; j < theApp.m_vPImgCocs.size(); ++j)
	{
		CImageDoc * pDoc = theApp.m_vPImgCocs[j];

		if (pDoc)
		{
			pDoc->OnCloseDocument(); // close all the opened original images
		}		
	}

	DeleteAllItems();
	m_imgList.DeleteImageList();
	theApp.m_vCams.clear();			// delete all corresponding cam data
	theApp.m_imgsOriginal.clear();	// delete all corresponding original images
	theApp.m_imgsProcessed.clear();	// delete all corresponding processed images
	theApp.m_vPImgCocs.clear();
	theApp.m_vPathImgs.clear();		// 20220129，清空所有图像文件路径
	theApp.m_vNameImgs.clear();		// 20220129，清空所有图像纯文件名
	theApp.m_pathDirOut = "";			// 20220129，输出路径置空

	m_w = w;
	m_h = h;

	SetExtendedStyle(LVS_EX_CHECKBOXES);

	if (!m_imgList.Create(m_w, m_h, ILC_COLOR24, 3, 3))
	{
		return FALSE;
	}

	m_imgList.SetBkColor(RGB(125, 125, 0));

	if (!SetImageList(&m_imgList, LVSIL_NORMAL))
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CImageListCtrl::InitImgListCtrl(void)
{
	int nCount = GetItemCount();

	for (int i = 0; i < nCount; ++i)
	{
		char * pData = (char *)GetItemData(i);

		if (pData)
		{
			free(pData);
		}
	}

	for (int j = 0; j < theApp.m_vPImgCocs.size(); ++j)
	{
		CImageDoc * pDoc = theApp.m_vPImgCocs[j];

		if (pDoc)
		{
			pDoc->OnCloseDocument(); // close all the opened original images
		}		
	}

	DeleteAllItems();
	m_imgList.DeleteImageList();
	theApp.m_vCams.clear();		// delete all corresponding cam data
	theApp.m_imgsOriginal.clear(); // delete all corresponding original images
	theApp.m_imgsProcessed.clear(); // delete all corresponding processed images
	theApp.m_vPImgCocs.clear();
	theApp.m_vPathImgs.clear();
	theApp.m_vNameImgs.clear();
	theApp.m_pathDirOut = "";

	SetExtendedStyle(LVS_EX_CHECKBOXES);

	if (!m_imgList.Create(m_w, m_h, ILC_COLOR24, 3, 3))
	{
		return FALSE;
	}

	m_imgList.SetBkColor(RGB(125, 125, 0));

	if (!SetImageList(&m_imgList, LVSIL_NORMAL))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CImageListCtrl::AddOneImage(CString path)
{
	Mat image;

	if (!(image = imread(path.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED)).data)
	{
		return FALSE;
	}

	uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
	BITMAPINFO * bmi = (BITMAPINFO *)buffer;

	int img_w = image.cols;
	int img_h = image.rows;
	int img_c = image.channels();

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
		
		for (int i = 0; i < 256; ++i)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	if ((img_w*img_c) % 4 != 0 && image.isContinuous())
	{
		CvSize size;
		size.width = img_w;	size.height = img_h;
		IplImage * imgTmp = cvCreateImage(size, 8, img_c);
//		image.copyTo(cv::Mat(imgTmp));
		image.copyTo(cv::cvarrToMat(imgTmp)); // 20160316, cv::Mat(const IplImage * img) is removed in opencv 3.x, use cv::cvarrToMat instead.

		Bitmap * pImage = new Bitmap(bmi, imgTmp->imageData);

		if (pImage == NULL)
		{
			cvReleaseImage(&imgTmp);
			return FALSE;
		}
		Bitmap * pThumbnail = (Bitmap*)pImage->GetThumbnailImage(m_w, m_h);
		HBITMAP hBmp;
		pThumbnail->GetHBITMAP(Color(255, 255, 255), &hBmp);
		
		CBitmap bmp;
		bmp.Attach(hBmp);
		int nResult = m_imgList.Add(&bmp, RGB(255, 255, 255));
		bmp.Detach();
	
		int count_items = GetItemCount();
		InsertItem(count_items, GetFileNameOnly(path), nResult);
		SetItemData(count_items, (DWORD_PTR)_strdup(path)/*path.GetBuffer()*/);
		SetCheck(count_items, TRUE);

		RedrawItems(count_items, count_items);
		delete [] pThumbnail;
		delete [] pImage;
	
		cvReleaseImage(&imgTmp);
	} 
	else
	{
		Bitmap * pImage = new Bitmap(bmi, image.data);

		if (pImage == NULL)
		{
			return FALSE;
		}
		Bitmap * pThumbnail = (Bitmap*)pImage->GetThumbnailImage(m_w, m_h);
		HBITMAP hBmp;
		pThumbnail->GetHBITMAP(Color(255, 255, 255), &hBmp);

		CBitmap bmp;
		bmp.Attach(hBmp);
		int nResult = m_imgList.Add(&bmp, RGB(255, 255, 255));
		bmp.Detach();

		int count_items = GetItemCount();
		InsertItem(count_items, GetFileNameOnly(path), nResult);
		SetItemData(count_items, (DWORD_PTR)_strdup(path)/*path.GetBuffer()*/);
		SetCheck(count_items, TRUE);

		RedrawItems(count_items, count_items);
		delete [] pThumbnail;
		delete [] pImage;
	}

	// add a new corresponding cam_data
	cam_data cam;

	// 20220128，这里首先看看图像文件夹下有没有calibration.txt文件
	// 这个文件应包含提前给定的像机内参数fx,fy,cx,cy,s，及像差系数d0-d4
	// 这些内参数可能在后续平差环节中被进一步调整优化，也可能不进一步优化
	CString pathFolder = GetFolderPath(path);

	FILE * fileCalib = fopen(pathFolder + "calibration.txt", "r");

	if (fileCalib)
	{
		double fx, fy, cx, cy, s, k0, k1, k2, k3, k4;
		fscanf(fileCalib, "%lf", &fx);	fscanf(fileCalib, "%lf", &fy);
		fscanf(fileCalib, "%lf", &cx);	fscanf(fileCalib, "%lf", &cy);
		fscanf(fileCalib, "%lf", &s);

		fscanf(fileCalib, "%lf", &k0);	fscanf(fileCalib, "%lf", &k1);
		fscanf(fileCalib, "%lf", &k2);	fscanf(fileCalib, "%lf", &k3);
		fscanf(fileCalib, "%lf", &k4);

		cam.fx = cam.m_K(0, 0) = fx;
		cam.fy = cam.m_K(1, 1) = fy;
		cam.cx = cam.m_K(0, 2) = cx;
		cam.cy = cam.m_K(1, 2) = cy;
		cam.s  = cam.m_K(0, 1) = s;
				 cam.m_K(2, 2) = 1;

		cam.k[0] = cam.m_dist(0) = k0;
		cam.k[1] = cam.m_dist(1) = k1;
		cam.k[2] = cam.m_dist(2) = k2;
		cam.k[3] = cam.m_dist(3) = k3;
		cam.k[4] = cam.m_dist(4) = k4;

		cam.dist_type = 1; // 默认像差类型为 D.C. Brown的

		cam.m_bCalibed = true; // 能到这里说明至少是已经有内参数初值

		fclose(fileCalib);
	}	

	// 20220128，再看看\results结果文件夹中有没有当前图像的参数文件
	// 如果有，那就将图像所有参数均更新为结果文件中的参数
	// 20220129，暂时还是先不读结果文件吧，因为我发现读进来的外参数影响了SfM结果，按说应该不影响的
	// 可能是因为SfM里哪个环节的哪个函数是基于R矩阵里的值来判断图像是否完成了定向，而非基于cam.m_bOriented来判断的，所以
//	CString pathFileParam = pathFolder + "results\\" + GetFileNameNoSuffix(path) + "_param.txt";

//	DeepVoid::ReadCameraData(pathFileParam, cam);

	theApp.m_vCams.push_back(cam);

	// 20200719，把图保留在内存里
	theApp.m_imgsOriginal.push_back(image);
	theApp.m_imgsProcessed.push_back(cv::Mat()/*image.clone()*/); // 20200728，如果不是 clone 的话，两张图就共用一个图像数据块
	theApp.m_vPImgCocs.push_back(NULL);
	theApp.m_vPathImgs.push_back(path);
	theApp.m_vNameImgs.push_back(GetFileNameNoSuffix(path));
	
	return TRUE;
}

void CImageListCtrl::DeleteSelImages(void)
{
	POSITION pos = GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = GetNextSelectedItem(pos);

		char * pData = (char *)GetItemData(nItem);
		if(pData)
		{
			CString str;
			str.Format(_T("%s"), pData);
			str = str.Trim();
			str = GetFileNameOnly(str);
			
			for (int i = 0; i < theApp.m_vPImgCocs.size(); ++i)
			{
				CImageDoc * pDoc = theApp.m_vPImgCocs[i];
				if(str.CompareNoCase(pDoc->GetTitle()) == 0) // if corresponding original image has been opened then close this image first
				{
					pDoc->OnCloseDocument();
					break;
				}
			}
			
			free(pData);
		}

		DeleteItem(nItem);

		theApp.m_vCams.erase(theApp.m_vCams.begin() + nItem); // delete corresponding cam_data
		theApp.m_imgsOriginal.erase(theApp.m_imgsOriginal.begin() + nItem); // delete all corresponding original images
		theApp.m_imgsProcessed.erase(theApp.m_imgsProcessed.begin() + nItem); // delete all corresponding processed images
		theApp.m_vPathImgs.erase(theApp.m_vPathImgs.begin() + nItem);
		theApp.m_vNameImgs.erase(theApp.m_vNameImgs.begin() + nItem);

		pos = GetFirstSelectedItemPosition();
	}
}

void CImageListCtrl::OpenSelImages(void)
{
	POSITION pos = GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = GetNextSelectedItem(pos);

		char * pDir = (char *)GetItemData(nItem);

		CString strDir;
		strDir.Format(_T("%s"), pDir);

		CString strFileName = GetFileNameOnly(strDir);

		auto iter_found = std::find_if(theApp.m_vPImgCocs.begin(), theApp.m_vPImgCocs.end(), [strFileName](const CImageDoc * p) 
		{
			if (p)
			{
				return p->GetTitle() == strFileName;
			}
			else
			{
				return false;
			}
		});

		if (iter_found != theApp.m_vPImgCocs.end())
		{
			continue;
		}

		CImageDoc * pDoc = (CImageDoc *)theApp.CreateDocument(theApp.m_pTemplateImgDoc);

		// 20200719，在 AddOneImage 时已经把图都全读进内存了，这里不再需要读图像文件了。
// 		if (!(pDoc->m_image = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED)).data)
// 		{
// 			pDoc->OnCloseDocument(); // if the image is unreadable, then close this document
// 			continue;
// 		}

//		theApp.m_vPImgCocs.push_back(pDoc);
		theApp.m_vPImgCocs[nItem] = pDoc;

		pDoc->SetTitle(strFileName); // set document title
		pDoc->m_pImgOriginal = &theApp.m_imgsOriginal[nItem];	// 20200719，ImageDoc 类中不再存放图像，图像统一全存在 theApp 图像向量容器中
		pDoc->m_pImgProcessed = &theApp.m_imgsProcessed[nItem];	// 20200719，ImageDoc 类中不再存放图像，图像统一全存在 theApp 图像向量容器中
		if (pDoc->m_pImgView->m_bShowProcessed)
		{
			pDoc->m_pImgView->m_pImage = pDoc->m_pImgProcessed; // 20200719
		} 
		else
		{
			pDoc->m_pImgView->m_pImage = pDoc->m_pImgOriginal;
		}
		pDoc->m_pCam = &theApp.m_vCams[nItem];

		pDoc->m_pImgView->SetImageScrollSize();
		pDoc->m_pImgView->Invalidate(FALSE);
	}
}


void CImageListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar) 
	{
	case VK_DELETE:
//		DeleteSelImages(); // 20200719，不再让随便删除图像了。
		break;

	case VK_RETURN:
		OpenSelImages();
		break;

	default:
		break;
	}

	if ((nChar == 'a' || nChar == 'A') && GetKeyState(VK_CONTROL))
	{
		int count = GetItemCount();
		
		for (int i = 0; i < count; ++i)
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CImageListCtrl::OnDestroy()
{
	int nCount = GetItemCount();
	
	for (int i = 0; i < nCount; ++i)
	{
		char * pData = (char *)GetItemData(i);
		if(pData)
		{
			free(pData);
		}
	}

	CListCtrl::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CImageListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if  (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
