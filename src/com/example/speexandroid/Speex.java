package com.example.speexandroid;

public class Speex {
	
	
	static{
		System.loadLibrary("SpeexAndroid");
	}
	
	private static Speex speex;
	
	public static synchronized Speex getInstance(){
		if(null == speex)
			speex = new Speex();
		return speex;
	}
	
	private Speex(){
		intSpeex(4);
	}
	
	/**
	 * 初始化 speex
	 * @param quality speex 质量
	 */
	public native void intSpeex(int quality);
	
	/**
	 * 语音数据编码
	 * @param encode 编码数据内容
	 * @return 编码后的数据
	 */
	public native byte[] encodeSpeex(short[] encode);
	
	/**
	 * 语音数据解码
	 * @param decode 需要解码的数据
	 * @return 解码后的数据
	 */
	public native short[] decodeSpeex(byte[] decode);
	
	/**
	 * 获取能编码的数据长度
	 * @return 数据长度
	 */
	public native int getEncodeSize();
	
	/**
	 * 获取能解码的数据长度
	 * @return 数据长度
	 */
	public native int getDecodeSize();
	
	/**
	 * 释放内存
	 */
	public native void closeSpeex();

}
