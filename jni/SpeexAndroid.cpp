
extern "C"
{
#include <jni.h>
#include<string.h>
#include<unistd.h>
#include<speex/speex.h>
#include<speex/speex_preprocess.h>

//判断是否开启
static int code_open = 0;

//编解码数据结构
static SpeexBits ebits, dbits;

//编解码 状态
void *enc_state, *dnc_state;

//编解码帧大小
static int dec_frame_size, enc_frame_size;

static SpeexPreprocessState * m_st;

JNIEXPORT void Java_com_example_speexandroid_Speex_intSpeex(JNIEnv *env,jobject thiz,jint quality)
{
	//判断是否开启，避免重复开启
	if(code_open)
	{
		return;
	}
	code_open++;
	//初始化
	speex_bits_init(&ebits);
	speex_bits_init(&dbits);

	//初始化状态 speex_wb_mode 宽带编码 speex_nb_mode:窄带 speex_uwb_mode表示超宽带模式
	enc_state = speex_encoder_init(&speex_nb_mode);
	dnc_state = speex_decoder_init(&speex_nb_mode);

	//设置压缩质量
	int tmp=0;
	/**
	 * 参数state表示编码器的状态；参数request表示要定义的参数类型，如SPEEX_ GET_ FRAME_SIZE表示设置帧大小，
	 * SPEEX_ SET_QUALITY表示量化大小，这决定了编码的质量；参数ptr表示要设定的值。
	 */
	speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &tmp);
	speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &tmp);
	speex_encoder_ctl(enc_state, SPEEX_SET_VBR_QUALITY, &quality);
	speex_encoder_ctl(enc_state, SPEEX_GET_FRAME_SIZE, &enc_frame_size);
	speex_decoder_ctl(dnc_state, SPEEX_GET_FRAME_SIZE, &dec_frame_size);

	/**
	 * 默认的帧长度是160，对应默认的8kHz的采样率，那么一帧对应的时间是20ms，
	 * 采样频率比它的高时，因此帧长度也要相应增加，其时间长度也对应20ms
	 */
	m_st = speex_preprocess_state_init(160,8000);
	int denoise = 1;
	int noiseSuppress = -25;
	speex_preprocess_ctl(m_st,SPEEX_PREPROCESS_SET_DENOISE,&denoise);//降噪
	speex_preprocess_ctl(m_st,SPEEX_PREPROCESS_GET_NOISE_SUPPRESS,&noiseSuppress);//设置噪声的dB

	int agc = 1;
	int q=24000;
	//actually default is 8000(0,32768),here make it louder for voice is not loudy enough by default. 8000
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC, &agc);//增益
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL,&q);
	int vad = 1;
	int vadProbStart = 80;
	int vadProbContinue = 65;
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart); //Set probability required for the VAD to go from silence to voice
	speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //Set probability required for the VAD to stay

}

//编码
JNIEXPORT jbyteArray Java_com_example_speexandroid_Speex_encodeSpeex(JNIEnv *env,jobject thiz,jshortArray lin)
{
	if(!enc_state)
	{
		return 0;
	}

	jshort buffer[enc_frame_size];
	jbyte output_buffer[enc_frame_size];

	//再次设定SpeexBits
	speex_bits_reset(&ebits);
	//jshortArray 转换成 jshort数组
	env->GetShortArrayRegion(lin,0,enc_frame_size,buffer);

	speex_preprocess_run(m_st,buffer);

	speex_encode_int(enc_state, buffer, &ebits);
	int tot_bytes = speex_bits_write(&ebits, (char *) output_buffer,enc_frame_size);
	jbyteArray encoded = env->NewByteArray(enc_frame_size);
	env->SetByteArrayRegion(encoded,0,tot_bytes,output_buffer);

	//释放 资源
//	env->ReleaseShortArrayElements(lin,buffer,NULL);
	return encoded;
}

JNIEXPORT jshortArray Java_com_example_speexandroid_Speex_decodeSpeex(JNIEnv *env,jobject thiz,jbyteArray encoded)
{

	if(!code_open)
		return 0;
	jbyte buffer[dec_frame_size];
	jshort output_buffer[dec_frame_size];

	//再次设定SpeexBits
	speex_bits_reset(&dbits);
	//jbyteArray 转 jbyte数组
	env->GetByteArrayRegion(encoded,0,enc_frame_size,buffer);
	//读入原始数据
	speex_bits_read_from(&dbits,(char*)buffer,dec_frame_size);
	//解码
	speex_decode_int(dnc_state,&dbits,output_buffer);
	//创建 jshortArray
	jshortArray lin = env->NewShortArray(dec_frame_size);
	//将 short数组buffer的数据写入jshortArray中
	env->SetShortArrayRegion(lin,0,dec_frame_size,output_buffer);

	//释放内存
//	env->ReleaseByteArrayElements(encoded,buffer,NULL);
	return lin;
}

JNIEXPORT jint Java_com_example_speexandroid_Speex_getEncodeSize(JNIEnv *env,jobject thiz)
{
	if(!code_open)
	{
		return 0;
	}

	return enc_frame_size;
}

JNIEXPORT jint Java_com_example_speexandroid_Speex_getDecodeSize(JNIEnv *env,jobject thiz)
{
	if(!code_open)
	{
		return 0;
	}
	return dec_frame_size;
}

JNIEXPORT void Java_com_example_speexandroid_Speex_closeSpeex(JNIEnv *env,jobject thiz)
{
	if(!code_open)
	{
		return;
	}

	speex_encoder_destroy(&enc_state);
	speex_bits_destroy(&ebits);

	speex_decoder_destroy(&dnc_state);
	speex_bits_destroy(&dbits);

}

}


