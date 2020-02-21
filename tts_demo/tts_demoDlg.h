
// tts_demoDlg.h : 头文件
//

#pragma once

#include <TTS.h>
#include "afxwin.h"

// CTTSDemoDlg 对话框
class CTTSDemoDlg : public CDialog
{
// 构造
public:
	CTTSDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TTS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonTTS();
	DECLARE_MESSAGE_MAP()

private:
	void InitChildCtrls();
	std::string CStringToStdstring(CString cstr);
	std::string getCurExeFilePath(); // 获取当前程序运行路径


private:
	TTS m_tts;
	CString m_strText;
	CComboBox m_cbVoiceName;
	int m_nVolume;
	int m_nSpeed;
	int m_nPitch;
	CString m_strOutputFileName;
};
