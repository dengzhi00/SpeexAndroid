package com.example.speexandroid.view;

import com.example.speexandroid.R;

import android.content.Context;
import android.graphics.drawable.AnimationDrawable;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class RecordAnimationView extends RelativeLayout implements View.OnTouchListener{

    private ImageView animation_imag;

    private TextView textView;

    private AnimationDrawable animation;

    public RecordAnimationView(Context context) {
        super(context);
        init();
    }

    public RecordAnimationView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public RecordAnimationView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context,attrs,defStyleAttr);
        init();
    }

    private void init() {
        LayoutInflater.from(getContext()).inflate(R.layout.view_record, this, true);
        animation_imag = (ImageView) findViewById(R.id.animation_imag);
        textView = (TextView) findViewById(R.id.text_record);

        animation = (AnimationDrawable) animation_imag.getDrawable();

        setBackgroundColor(getContext().getResources().getColor(R.color.audio));
        this.setVisibility(View.GONE);
        setOnTouchListener(this);
    }


    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {
        return true;
    }

    public void startAnimation(){
        this.setVisibility(View.VISIBLE);
        textView.setText("录音");
        animation.start();
    }

    public void stopAnimation(){
        this.setVisibility(View.GONE);
        animation.stop();
    }

}
