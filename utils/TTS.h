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
	// 设置文本
	void setText(std::string strText);
	// 设置语音库名称
	void setVoiceName(std::string strVoiceName);
	// 设置音量
	void setVolume(int volume);
	// 设置语速
	void setSpeed(int speed);
	// 设置语调
	void setPitch(int pitch);
	// 设置工作目录
	void setWorkDir(std::string workDir);
	// 获取工作目录
	std::string getWorkDir();

	// 设置输出文件名称
	void setOutputFileName(std::string strFileName);

	// 读取App和用户信息
	bool readTTSInfo(std::string strPath);

	// 执行文本转语音转MP3
	bool doTTS();

private:
	void updateSessionBeginParams();

	// 文本合成语音
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