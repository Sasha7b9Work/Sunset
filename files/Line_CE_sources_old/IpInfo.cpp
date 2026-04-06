// IpInfo.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "IpInfo.h"

#pragma        comment (lib, "iphlpapi")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIpInfo dialog


CIpInfo::CIpInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CIpInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpInfo)
	m_edt_tmp = _T("");
	//}}AFX_DATA_INIT
}


void CIpInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpInfo)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_IP, m_edt_ip);
	DDX_Text(pDX, IDC_EDIT_NEW_IP, m_edt_tmp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIpInfo, CDialog)
	//{{AFX_MSG_MAP(CIpInfo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnChangeIP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpInfo message handlers

BOOL CIpInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString str;
	if (GetIPAddressPCM(str))
	m_edt_ip.SetWindowText(str);
	else
		m_edt_ip.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CIpInfo::GetIPAddressPCM(CString &strIP)
{
	char*  buf;
    PIP_ADAPTER_INFO   pAdaptersInfo;  
	PIP_ADDR_STRING  pAddr;
    DWORD  dwSize = 0;

    if(GetAdaptersInfo(NULL, &dwSize) != ERROR_BUFFER_OVERFLOW) return FALSE;
    buf = new char[dwSize];
    if (!buf) return FALSE;
    pAdaptersInfo = reinterpret_cast<PIP_ADAPTER_INFO>(buf);
    if (GetAdaptersInfo (pAdaptersInfo, &dwSize) == ERROR_SUCCESS)
		{
		   while (pAdaptersInfo)
			{
				pAddr = &pAdaptersInfo->IpAddressList;
				while (pAddr){
					strIP=pAddr->IpAddress.String;
					pAddr = pAddr->Next;
				}
				pAdaptersInfo = pAdaptersInfo->Next;
			}	
		}
    delete[] buf;
	return TRUE;
}

HBRUSH CIpInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_IP)
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CIpInfo::OnChangeIP() 
{
	// TODO: Add your control notification handler code here
	if  (CheckIP())
	{ if (SetIPAddressPCM(m_edt_tmp))
		{
		 AfxMessageBox(_T("Изменения вступят в силу после перезапуска прибора."));
  		 CDialog::OnOK();
		}
		else
		{ CDialog::OnCancel();//Ошибка реестра
		}
	}
	else
	{
		 AfxMessageBox(_T("Неверно введен IP адрес."));
	}
	
}


BOOL CIpInfo::CheckIP()
{  UpdateData();
	char* lpsz=new char[m_edt_tmp.GetLength()+1];
	for (int i=0;i<m_edt_tmp.GetLength();i++)
	{
	lpsz[i]=(char)m_edt_tmp.GetAt(i);
	}
	lpsz[i]=0;

	if (inet_addr(lpsz)==INADDR_NONE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CIpInfo::SetIPAddressPCM(CString strIP)
{
	HKEY hkKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"Comm\\PCI\\E100CE1\\Parms\\TcpIp",
		0,
		KEY_ALL_ACCESS,
		&hkKey)!=ERROR_SUCCESS)
	{
		AfxMessageBox(L"Ошибка 1 реестра!");
		return FALSE;
	}

	//strIP=L"192.168.55.104";
	//strIP=L"192.168.1.145";
	int len=strIP.GetLength()*2+2;
	if (RegSetValueEx(hkKey,L"IpAddress",0,REG_MULTI_SZ,(CONST BYTE *)(LPCTSTR)strIP,len)!=ERROR_SUCCESS)
	{
		AfxMessageBox(L"Ошибка 2 реестра!");
		return FALSE;
	}
	CString strMask=L"255.255.255.0";
	len=strMask.GetLength()*2+2;
	if (RegSetValueEx(hkKey,L"Subnetmask",0,REG_MULTI_SZ,(CONST BYTE *)(LPCTSTR)strMask,len)!=ERROR_SUCCESS)
	{
		AfxMessageBox(L"Ошибка 3 реестра!");
		return FALSE;
	}
	if (hkKey)
	RegCloseKey(hkKey);
	return TRUE;
}

