package com.example.speexandroid.thread;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import com.example.speexandroid.Speex;
import com.example.speexandroid.helper.FileHelper;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

public class AudioTruckThread extends Thread{

    private boolean isRunning;

    private AudioTrack audioTrack;

    private int minBufferSize;
    
    private Context context;

    
    public AudioTruckThread(Context context){
    	this.context = context;
        isRunning = true;
        minBufferSize = AudioTrack.getMinBufferSize(8000,
                AudioFormat.CHANNEL_OUT_MONO,
                AudioFormat.ENCODING_PCM_16BIT);
        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, 8000,
                AudioFormat.CHANNEL_OUT_MONO,
                AudioFormat.ENCODING_PCM_16BIT, minBufferSize,
                AudioTrack.MODE_STREAM);
        audioTrack.setStereoVolume(1.0f, 1.0f);
    }

    @Override
    public void run() {
        try {
        	Speex speex = Speex.getInstance();
            FileInputStream fileInputStream = new FileInputStream(FileHelper.getHelper().getAudi0File());
            audioTrack.play();
            

            
            while(isRunning){
                byte[] bytes = new byte[speex.getDecodeSize()];
                if(fileInputStream.read(bytes,0,speex.getDecodeSize())==-1){
                    isRunning = false;
                    break;
                }
//                byte[] bytes1 = bytes.clone();
                 short[] shorts = speex.decodeSpeex(bytes);
                 if(shorts.length>0){
//                     for(int i = 0;i<shorts.length;i++){
//                    	 System.out.println("shortsstring"+shorts[i]);
//                     }
                     
                	 audioTrack.write(shorts,0,shorts.length);
                 }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
