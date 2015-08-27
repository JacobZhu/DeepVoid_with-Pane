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
	int i;
	for (i=0;i<nCount;i++)
	{
		char * pData = (char *)GetItemData(i);
		if(pData)
		{
			free(pData);
		}
	}

	int j;
	for (j=0;j<theApp.m_vPImgCocs.size();j++)
	{
		CImageDoc * pDoc = theApp.m_vPImgCocs[j];
		pDoc->OnCloseDocument(); // close all the opened original images
	}

	DeleteAllItems();
	m_imgList.DeleteImageList();
	theApp.m_vCams.clear();		// delete all corresponding cam data

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
	int i;
	for (i=0;i<nCount;i++)
	{
		char * pData = (char *)GetItemData(i);
		if(pData)
		{
			free(pData);
		}
	}

	int j;
	for (j=0;j<theApp.m_vPImgCocs.size();j++)
	{
		CImageDoc * pDoc = theApp.m_vPImgCocs[j];
		pDoc->OnCloseDocument(); // close all the opened original images
	}

	DeleteAllItems();
	m_imgList.DeleteImageList();
	theApp.m_vCams.clear();		// delete all corresponding cam data

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
		int i;
		for (i=0;i<256;i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	if ((img_w*img_c)%4 != 0 && image.isContinuous())
	{
		CvSize size;
		size.width = img_w;	size.height = img_h;
		IplImage * imgTmp = cvCreateImage(size, 8, img_c);
		image.copyTo(cv::Mat(imgTmp));

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
	theApp.m_vCams.push_back(cam);
	
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
			int i;
			for (i=0;i<theApp.m_vPImgCocs.size();i++)
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

		theApp.m_vCams.erase(theApp.m_vCams.begin()+nItem); // delete corresponding cam_data

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

		CImageDoc * pDoc = (CImageDoc *)theApp.CreateDocument(theApp.m_pTemplateImgDoc);

		if (!(pDoc->m_image = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED)).data)
		{
			pDoc->OnCloseDocument(); // if the image is unreadable, then close this document
			continue;
		}

		theApp.m_vPImgCocs.push_back(pDoc);

		pDoc->SetTitle(GetFileNameOnly(strDir)); // set document title
		pDoc->m_pImgView->m_pImage = &pDoc->m_image;
		pDoc->m_pFeatures = &theApp.m_vCams[nItem].m_feats; // associate the doc's feature struct pointer with corresponding cam_data's feature struct
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
		DeleteSelImages();
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
		int i;
		for (i=0; i<count; i++)
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CImageListCtrl::OnDestroy()
{
	int nCount = GetItemCount();
	int i;
	for (i=0;i<nCount;i++)
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
