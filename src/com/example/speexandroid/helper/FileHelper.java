package com.example.speexandroid.helper;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.os.Environment;

public class FileHelper {

    private final static String FILES = "AUDIOS";

    private final static String FILNAME = "audio";

    private File file2;

    private static FileHelper helper;

    public static synchronized FileHelper getHelper(){
        if(null == helper)
            helper = new FileHelper();
        return helper;
    }

    private FileHelper() {
        creatFiles();
    }

    private void creatFiles() {
        File file = new File(Environment.getExternalStorageDirectory(), FILES);
        if (!file.exists()) {
            file.mkdirs();
        }
        file2 = new File(file, FILNAME);
    }

    public File getAudi0File() {
        return file2;
    }

    public void writeFile(byte[] buffer) throws IOException {
        if (file2.exists()) {
            file2.delete();
        }
        if (buffer == null) {
            return;
        }
        if (file2.createNewFile()) {
            FileOutputStream fos = new FileOutputStream(file2);
            fos.write(buffer);
            fos.close();
        }
    }

}
