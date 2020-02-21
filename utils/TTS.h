#ifndef __TTS_H__
#define __TTS_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include <string>

class TTS
{
public:
	TTS();
	~TTS();

public:
	// �����ı�
	void setText(std::string strText);
	// ��������������
	void setVoiceName(std::string strVoiceName);
	// ��������
	void setVolume(int volume);
	// ��������
	void setSpeed(int speed);
	// �������
	void setPitch(int pitch);
	// ���ù���Ŀ¼
	void setWorkDir(std::string workDir);
	// ��ȡ����Ŀ¼
	std::string getWorkDir();

	// ��������ļ�����
	void setOutputFileName(std::string strFileName);

	// ��ȡApp���û���Ϣ
	bool readTTSInfo(std::string strPath);

	// ִ���ı�ת����תMP3
	bool doTTS();

private:
	void updateSessionBeginParams();

	// �ı��ϳ�����
	int text_to_speech(const char* src_text, const char* des_path, const char* params);


private:
	std::string m_strText = "";
	std::string m_strVoiceName = "xiaoyan";
	int m_nVolume = 100;
	int m_nSpeed = 60;
	int m_nPitch = 60;

	std::string m_strFileName = "audio";
	std::string m_strWorkDir = ".";
	
	std::string m_strSessionBeginParams;

	std::string m_strAppId;
	std::string m_strUserName;
	std::string m_strPwd;
};

#endif // __TTS_H__