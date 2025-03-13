#if !defined(AFX_MODESET11_H__7617EF3D_1E05_4678_9741_638C215F33C4__INCLUDED_)
#define AFX_MODESET11_H__7617EF3D_1E05_4678_9741_638C215F33C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet11 dialog

class CModeSet11 : public CDialog
{
// Construction
public:
	CModeSet11(CWnd* pParent = NULL);   // standard constructor


	CLabelControl  m_Label101;
	CLabelControl  m_Label102; 
	CCommandButton m_button101;
	CCommandButton m_button102;

	CEdit	m_edit101;
	CEdit	m_edit102;
	CEdit	m_edit103;
	CEdit	m_edit104;
	 
  	void  OnButton101_Test(); 
	void  OnButton102_Stop();
   
	void  OnTest1_Set1();
    int  OnTest1_Run1_LV();//sylee230815
	int  OnTest1_Run1_HV();//sylee230815
    int  AHV_ADCI_Read1_Avg3();  //sylee230815 

	CListBox	m_list1;//sylee230815
    int nLVNG1;//sylee230815
    int nBlockNo;//sylee230815
  
	int nStartPin,nEndPin,nCount,nDelay,nProPer1,nFlag_Stop,nMaxpin_Half,nMohm,nMohmADC1;
	char szText[100];
	CString str;
 	int k9Err,nDe1Co1;
	double Rval1;
	short nDe1[5001][3];

// Dialog Data
	//{{AFX_DATA(CModeSet11)
	enum { IDD = IDD_MODE_SET11_SELF1 };
//	CButton	m_check1;
		// NOTE: the ClassWizard will add data members here
 
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet11)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation


protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet11)
		// NOTE: the ClassWizard will add member functions here

 	afx_msg void OnClickOk();

	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

 

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESET11_H__7617EF3D_1E05_4678_9741_638C215F33C4__INCLUDED_)
