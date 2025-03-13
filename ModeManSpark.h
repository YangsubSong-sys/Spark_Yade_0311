#if !defined(AFX_MODEMANSPARK_H__B2B790C6_1631_4C38_95E4_7792ED2298F5__INCLUDED_)
#define AFX_MODEMANSPARK_H__B2B790C6_1631_4C38_95E4_7792ED2298F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeManSpark.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeManSpark dialog

#define MAX_SPARK_CAL_NET	10


class CModeManSpark : public CDialog
{
// Construction
public:
	CModeManSpark(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeManSpark)
	enum { IDD = IDD_MODE_MAN_SPARK };
	int		m_nSelectSparkVolt;
	int		m_nSelectSparkPin;
	double	m_dSparkRef[MAX_SPARK_CAL_NET];
	double	m_dSparkRead[MAX_SPARK_CAL_NET];
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeManSpark)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	CComboBox	m_ctrlSparkVoltSet;
	CComboBox	m_ctrlSparkPinSel;
//	CButton		m_ctrlSparkRadioBtn[MAX_SPARK_CAL_NET];

	double m_dR1[MAX_SPARK_CAL_NET];
	int		m_nMode;
	FILE*   m_fp;
	char	m_szFile[128];


	int OnSparkCal();
	int Initialize();
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeManSpark)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSparkRead();
	afx_msg void OnButtonSparkReadApply();
	afx_msg void OnOk();
	afx_msg void OnButtonSparkRefLoad();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMANSPARK_H__B2B790C6_1631_4C38_95E4_7792ED2298F5__INCLUDED_)
