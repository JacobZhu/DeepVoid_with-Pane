// ShowInfoListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "ShowInfoListCtrl.h"


// CShowInfoListCtrl

IMPLEMENT_DYNAMIC(CShowInfoListCtrl, CListCtrl)

CShowInfoListCtrl::CShowInfoListCtrl()
{

}

CShowInfoListCtrl::~CShowInfoListCtrl()
{
}


BEGIN_MESSAGE_MAP(CShowInfoListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CShowInfoListCtrl message handlers


void CShowInfoListCtrl::AddOneInfo(CString info)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strTime;
	strTime.Format("%02u:%02u:%02u:%03u    ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	info = strTime + info;

	InsertItem(0, info);
}