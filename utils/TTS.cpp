#include "TTS.h"

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

#include "inih\IniParser.h"


/* wav音频头部格式 */
typedef struct _wave_pcm_hdr
{
	char            riff[4];                // = "RIFF"
	int				size_8;                 // = FileSize - 8
	char            wave[4];                // = "WAVE"
	char            fmt[4];                 // = "fmt "
	int				fmt_size;				// = 下一个结构体的大小 : 16

	short int       format_tag;             // = PCM : 1
	short int       channels;               // = 通道数 : 1
	int				samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
	int				avg_bytes_per_sec;      // = 每秒字节数 : samples_per_sec * bits_per_sample / 8
	short int       block_align;            // = 每采样点字节数 : wBitsPerSample / 8
	short int       bits_per_sample;        // = 量化比特数: 8 | 16

	char            data[4];                // = "data";
	int				data_size;              // = 纯数据长度 : FileSize - 44 
} wave_pcm_hdr;

/* 默认wav音频头部数据 */
wave_pcm_hdr default_wav_hdr =
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{ 'W', 'A', 'V', 'E' },
	{ 'f', 'm', 't', ' ' },
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{ 'd', 'a', 't', 'a' },
	0
};


TTS::TTS()
	: m_strSessionBeginParams("engine_type = local, voice_name = xiaoyan, text_encoding = GB2312, tts_res_path = fo|res\\tts\\xiaoyan.jet;fo|res\\tts\\common.jet, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2")
{
}


TTS::~TTS()
{
}

void TTS::setText(std::string strText)
{
	m_strText = strText;
}

void TTS::setVoiceName(std::string strVoiceName)
{
	m_strVoiceName = strVoiceName;
}

void TTS::setVolume(int volume)
{
	m_nVolume = volume;
}

void TTS::setSpeed(int speed)
{
	m_nSpeed = speed;
}

void TTS::setPitch(int pitch)
{
	m_nPitch = pitch;
}

void TTS::setWorkDir(std::string workDir)
{
	m_strWorkDir = workDir;
}

std::string TTS::getWorkDir()
{
	return m_strWorkDir;
}

void TTS::setOutputFileName(std::string strFileName)
{
	m_strFileName = strFileName;
}

bool TTS::readTTSInfo(std::string strPath)
{
	IniParser parser(strPath + "tts.dat");

	int error = parser.GetErrorCode();
	if (error < 0)
		return false;

	m_strAppId = parser.GetIniKeyString("tts", "appid", "");
	m_strUserName = parser.GetIniKeyString("tts", "name", "");
	m_strPwd = parser.GetIniKeyString("tts", "pwd", "");
	return true;
}

bool TTS::doTTS()
{
	int ret = MSP_SUCCESS;
	std::string strLoginParams = "appid = ";
	strLoginParams += m_strAppId;
	strLoginParams += ", work_dir = ";
	strLoginParams += m_strWorkDir;
	//const char* login_params = "appid = 5c9f13db, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动

	/* 用户登录 */
	ret = MSPLogin(m_strUserName.c_str(), m_strPwd.c_str(), strLoginParams.c_str()); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
	if (MSP_SUCCESS != ret)
	{
		printf("MSPLogin failed, error code: %d.\n", ret);
		MSPLogout(); //退出登录
		return false;
	}

	/* 文本合成 */
	printf("开始合成 ...\n");
	std::string strWavFile = m_strWorkDir + m_strFileName + ".wav";
	updateSessionBeginParams();
	ret = text_to_speech(m_strText.c_str(), strWavFile.c_str(), m_strSessionBeginParams.c_str());
	if (MSP_SUCCESS != ret)
	{
		printf("text_to_speech failed, error code: %d.\n", ret);
		MSPLogout(); //退出登录
		return false;
	}

	printf("转成mp3\n");
	std::string strMp3File = m_strWorkDir + m_strFileName + ".mp3";
	std::string strCmd = m_strWorkDir + "lame.exe " + strWavFile + " " + strMp3File + " & del " + strWavFile;
	//system("lame.exe tts_sample.wav tts_sample.mp3 & del tts_sample.wav");
	system(strCmd.c_str());

	printf("合成完毕\n");

	MSPLogout(); //退出登录
	return true;	
}

void TTS::updateSessionBeginParams()
{
	m_strSessionBeginParams = "engine_type = local, ";
	m_strSessionBeginParams += ("voice_name = " + m_strVoiceName + ", ");
	m_strSessionBeginParams += ("text_encoding = GB2312, ");
	m_strSessionBeginParams += ("tts_res_path = fo|res\\tts\\" + m_strVoiceName + ".jet;fo|res\\tts\\common.jet, ");
	m_strSessionBeginParams += ("sample_rate = 16000, ");
	m_strSessionBeginParams += ("speed = " + std::to_string(m_nSpeed) + ", ");
	m_strSessionBeginParams += ("volume = " + std::to_string(m_nVolume) + ", ");
	m_strSessionBeginParams += ("pitch = " + std::to_string(m_nPitch) + ", ");
	m_strSessionBeginParams += ("rdn = 2");
}

int TTS::text_to_speech(const char* src_text, const char* des_path, const char* params)
{
	int          ret = -1;
	FILE*        fp = NULL;
	const char*  sessionID = NULL;
	unsigned int audio_len = 0;
	wave_pcm_hdr wav_hdr = default_wav_hdr;
	int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

	if (NULL == src_text || NULL == des_path)
	{
		printf("params is error!\n");
		return ret;
	}
	fp = fopen(des_path, "wb");
	if (NULL == fp)
	{
		printf("open %s error.\n", des_path);
		return ret;
	}
	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionBegin failed, error code: %d.\n", ret);
		fclose(fp);
		return ret;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSTextPut failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		fclose(fp);
		return ret;
	}
	printf("正在合成 ...\n");
	fwrite(&wav_hdr, sizeof(wav_hdr), 1, fp); //添加wav音频头，使用采样率为16000
	while (1)
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (NULL != data)
		{
			fwrite(data, audio_len, 1, fp);
			wav_hdr.data_size += audio_len; //计算data_size大小
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
	}
	printf("\n");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSAudioGet failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		fclose(fp);
		return ret;
	}
	/* 修正wav文件头数据的大小 */
	wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

	/* 将修正过的数据写回文件头部,音频文件为wav格式 */
	fseek(fp, 4, 0);
	fwrite(&wav_hdr.size_8, sizeof(wav_hdr.size_8), 1, fp); //写入size_8的值
	fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
	fwrite(&wav_hdr.data_size, sizeof(wav_hdr.data_size), 1, fp); //写入data_size的值
	fclose(fp);
	fp = NULL;
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionEnd failed, error code: %d.\n", ret);
	}

	return ret;
}
