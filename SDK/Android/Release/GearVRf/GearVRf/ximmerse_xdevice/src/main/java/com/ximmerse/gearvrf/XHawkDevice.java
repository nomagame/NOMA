/*
 * Copyright (c) 2016. Samsung Electronics Co., LTD
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.ximmerse.gearvrf;

import android.opengl.Matrix;
import android.os.Handler;

import com.ximmerse.input.IInputEventDispatcher;
import com.ximmerse.sdk.XDeviceApi;

import org.gearvrf.GVRCameraRig;
import org.gearvrf.GVRContext;
import org.gearvrf.GVRTransform;
import org.gearvrf.GVRTransformUtility;
import org.gearvrf.io.cursor3d.IoDevice;
import org.gearvrf.utility.Log;
import org.joml.Quaternionf;
import org.joml.Vector3f;

/**
 * _VENDOR_TODO_ rename the IoDevice here
 * This class is responsible to deliver position and key events to the 3D cursor library.
 * Feel free to re purpose this class to deliver position and key events for a give IODevice.
 *
 * NOTE: In order to prevent reuse of package name. Make sure you change the package name for
 * this class as well.
 */
public class XHawkDevice extends IoDevice implements IInputEventDispatcher {


    private static final String TAG = XHawkDevice.class.getSimpleName();
    //Change this value to control the maximum depth for the cursor
    private static final int MAX_DEPTH = 20;
    //_VENDOR_TODO_ enter device details below
    private static final int VENDOR_ID = 7995;
    private static final int PRODUCT_ID = 4351;
    private static final String VENDOR_NAME = "XIMMERSE, LTD.";

    private static final int SET_CONNECTED_DELAY = 1000;

    protected static float[] sRawFloatsHelper=new float[4],sFloatsHelper=new float[4];
    protected static Vector3f sVector3Helper=new Vector3f();

    protected GVRContext context;
    protected Thread thread;

    public XHawkDevice(GVRContext context, String deviceId, String name) {
        /** The last param (boolean) denotes that the device is not ready when this constructor
         * is called. We will use the setConnected call to let the framework know that the device
         * is ready. */
        super(deviceId, VENDOR_ID, PRODUCT_ID, name, VENDOR_NAME, false);
        this.context = context;

        // Set the initial position for the cursor here
        setPosition(0.0f, 0.0f, -1.0f);

        /**
         * It is important that we create a new thread so that the GL and the Main Android threads
         * are not impacted by the events generated with by the IO device.
         *
         * We are intentionally creating a thread in Java since it is easier to maintain. Threads
         * created in C++ need to be explicitly registered with the JVM. Here is more information
         * on creating threads in C++ if you wish to do so:
         * http://android.wooyd.org/JNIExample/
         */
        if(!true){
            Log.d(TAG, "Creating a new thread");
            thread = new Thread(threadRunnable);
            thread.start();
        }else {
            initXHawkContext();
        }

        if(!true){

        /**
         * Simulate startup sequence using a delayed handler, use the {@link #setConnected
         * (boolean)} call to let the framework know that the device is ready.
         */
        Handler handler = new Handler(context.getActivity().getMainLooper());
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "Send out setConnected");
                XHawkDevice.this.setConnected(true);
            }
        }, SET_CONNECTED_DELAY);
    }}

    private Runnable threadRunnable = new Runnable() {
        @Override
        public void run() {
            // call into native to initialize the thread
            // TODO : The main thread will run on XHawkApi...
            startNativeThreadProc();
        }
    };

    /**
     * Perform cleanup using this method.
     */
    public void close() {
        Log.d(TAG, "Interrupting thread");
        // Destroy the native thread.
    }

    @Override
    public void setEnable(boolean enable) {
        // When set to disable (i.e. enable == false) the calls to setPosition and setKeyEvents
        // are ignored by the framework. It is recommended that the events processing be put on
        // hold.
        super.setEnable(enable);
    }

    public void initXHawkContext(){
        pluginHandle = XDeviceApi.getInputDeviceHandle("XHawk-0");
        if (pluginHandle >= 0) {
            XDeviceApi.addInputEventDispatcher(pluginHandle, this);
        } else {
            Log.e(TAG, "pluginHandle<0 at initXHawkContext().");
        }
    }

    /**
     * Start a native thread proc.You need call it in a new thread.
     */
    public void startNativeThreadProc(){

    }

    protected GVRTransform mTracker;
    protected GVRCameraRig mCameraRig;
    protected XCobraDevice[] mHands=new XCobraDevice[2];

    // For recenter
    protected Vector3f mCameraRigStart=new Vector3f();

    //
    public  Vector3f headBlobOffset=new Vector3f();
    protected Quaternionf mCameraRigRot=new Quaternionf();
    protected Vector3f mBlobToHead=new Vector3f();

    public void setSceneObjects(GVRTransform tracker, GVRCameraRig cameraRig,XCobraDevice l,XCobraDevice r){
        mTracker=tracker;
        mCameraRig=cameraRig;
        mHands[0]=l;
        mHands[1]=r;
        //
        GVRTransformUtility.getPosition(mCameraRig.getTransform(),mCameraRigStart);
    }

    public void recenter(){
        if(mCameraRig!=null) {
            mCameraRig.resetYaw();
            GVRTransformUtility.setPosition(mCameraRig.getTransform(), mCameraRigStart);
        }
    }

    public GVRCameraRig getCameraRig(){return mCameraRig;}
    public void setCameraRig(GVRCameraRig cameraRig){mCameraRig=cameraRig;}

    public GVRTransform getTracker(){return mTracker;}
    public void setTracker(GVRTransform tracker){mTracker=tracker;}

    public int pluginHandle=-1;
    public boolean checkPluginHandle(int handle){
        return (handle==pluginHandle);
    }

    @Override
    public void dispatchAxisEvent(int handle, int axis, float value) {

    }

    @Override
    public void dispatchKeyEvent(int handle,int code, int action) {

    }

    @Override
    public void dispatchPositionEvent(int handle, int node, float x, float y, float z) {
        if(!checkPluginHandle(handle)){
            return;
        }

        float[] viewMatrix=((mTracker==null)?mCameraRig.getHeadTransform():mTracker).getModelMatrix();
        int i=0;
        sRawFloatsHelper[i++]=x;
        sRawFloatsHelper[i++]=y;
        sRawFloatsHelper[i++]=z;
        sRawFloatsHelper[i++]=1.0f;
        Matrix.multiplyMV(sFloatsHelper,0,viewMatrix,0,sRawFloatsHelper,0);
        switch (node){
            case 0:
            case 1:
                i=0;
                // We apply the calculated position to motion controller.
                mHands[node].processPosition(
                    sFloatsHelper[i++],
                    sFloatsHelper[i++],
                    sFloatsHelper[i++]
                );
            break;
            case 2:
                GVRTransformUtility.getRotation(mCameraRig.getTransform(),mCameraRigRot);
                mCameraRigRot.transform(headBlobOffset,mBlobToHead);
                mCameraRig.getTransform().setPosition(
                    sFloatsHelper[0]+mBlobToHead.get(0),
                    sFloatsHelper[1]+mBlobToHead.get(1),
                    sFloatsHelper[2]+mBlobToHead.get(2)
				);
            break;
        }
    }

    @Override
    public void dispatchRotationEvent(int handle, int node, float x, float y, float z, float w) {

    }

}


