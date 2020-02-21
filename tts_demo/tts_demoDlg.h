
// tts_demoDlg.h : ͷ�ļ�
//

#pragma once

#include <TTS.h>
#include "afxwin.h"

// CTTSDemoDlg �Ի���
class CTTSDemoDlg : public CDialog
{
// ����
public:
	CTTSDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TTS_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonTTS();
	DECLARE_MESSAGE_MAP()

private:
	void InitChildCtrls();
	std::string CStringToStdstring(CString cstr);
	std::string getCurExeFilePath(); // ��ȡ��ǰ��������·��


private:
	TTS m_tts;
	CString m_strText;
	CComboBox m_cbVoiceName;
	int m_nVolume;
	int m_nSpeed;
	int m_nPitch;
	CString m_strOutputFileName;
};
