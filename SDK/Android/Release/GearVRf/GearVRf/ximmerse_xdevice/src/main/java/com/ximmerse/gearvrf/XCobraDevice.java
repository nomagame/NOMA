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

import android.os.Handler;
import android.view.KeyEvent;

import com.ximmerse.input.ControllerInput;
import com.ximmerse.input.IInputEventDispatcher;
import com.ximmerse.sdk.XDeviceApi;
import com.ximmerse.sdk.XDeviceConstants;
import com.ximmerse.utils.ReflectUtil;

import org.gearvrf.GVRContext;
import org.gearvrf.GVRCursorController;
import org.gearvrf.GVRScene;
import org.gearvrf.GVRSceneObject;
import org.gearvrf.GVRTransform;
import org.gearvrf.GVRTransformUtility;
import org.gearvrf.io.cursor3d.IoDevice;
import org.gearvrf.utility.Log;
import org.joml.Quaternionf;

import java.util.HashMap;


/**
 * _VENDOR_TODO_ rename the IoDevice here
 * This class is responsible to deliver position and key events to the 3D cursor library.
 * Feel free to re purpose this class to deliver position and key events for a give IODevice.
 *
 * NOTE: In order to prevent reuse of package name. Make sure you change the package name for
 * this class as well.
 */
public class XCobraDevice extends IoDevice implements IInputEventDispatcher {
    private static final String TAG = XCobraDevice.class.getSimpleName();
    //Change this value to control the maximum depth for the cursor
    protected static final int MAX_DEPTH = 20;
    //_VENDOR_TODO_ enter device details below
    protected static final int VENDOR_ID = 7995;
    protected static final int PRODUCT_ID = 4351;
    protected static final String VENDOR_NAME = "XIMMERSE, LTD.";

    protected static final int ACTION_DOWN = 0;
    protected static final int ACTION_UP = 1;

    protected static final int SET_CONNECTED_DELAY = 1000;

    public static HashMap<Integer,Integer>[] sButtonMap;

    protected static Quaternionf sQuatHelper=new Quaternionf();

    static {
        // TODO: Add button map setting here.
        HashMap<Integer,Integer> bm;
        sButtonMap=new HashMap[2];
        // XCobra-0
        bm=new HashMap<Integer, Integer>();
          // Daydream
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_LEFT_THUMB),KeyEvent.KEYCODE_BUTTON_THUMBL);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_A),KeyEvent.KEYCODE_BUTTON_A);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_BACK),KeyEvent.KEYCODE_BUTTON_SELECT);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_START),KeyEvent.KEYCODE_BUTTON_START);
          // Ximmerse Ex
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_LEFT_SHOULDER),KeyEvent.KEYCODE_BUTTON_L1);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_RIGHT_SHOULDER),KeyEvent.KEYCODE_BUTTON_L1);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_LEFT_TRIGGER),KeyEvent.KEYCODE_BUTTON_L2);
        sButtonMap[0]=bm;
        // XCobra-1
        bm=new HashMap<Integer, Integer>();
        // Daydream
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_LEFT_THUMB),KeyEvent.KEYCODE_BUTTON_THUMBR);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_A),KeyEvent.KEYCODE_BUTTON_X);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_BACK),KeyEvent.KEYCODE_BUTTON_SELECT);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_START),KeyEvent.KEYCODE_BUTTON_START);
        // Ximmerse Ex
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_LEFT_SHOULDER),KeyEvent.KEYCODE_BUTTON_R1);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_RIGHT_SHOULDER),KeyEvent.KEYCODE_BUTTON_R1);
        bm.put(/*ControllerInput.maskToLayer*/(ControllerInput.BUTTON_LEFT_TRIGGER),KeyEvent.KEYCODE_BUTTON_R2);
        sButtonMap[1]=bm;
    }

    //
    protected GVRContext context;
    protected Thread thread;
    protected int mIndex;
    protected HashMap<Integer,Integer> mButtonMap;
    //
    protected GVRCursorController mGvrCursorController;
    protected Object sceneObjectLock=new Object();

    protected Quaternionf rotation=new Quaternionf();

    protected boolean mUseRotation=true;
    protected Quaternionf mRotation=new Quaternionf();
    //protected Vector3f mEulerAngles=new Vector3f();
    protected Quaternionf mYawError=new Quaternionf();

    public XCobraDevice(final GVRContext context, String deviceId, String name, int index) {
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
        // Hack the getGvrCursorController...
        try {
            mGvrCursorController=(GVRCursorController)ReflectUtil.getField(this,"gvrCursorController");
        } catch (Exception e) {
            e.printStackTrace();
        }
        if(!true){
            Log.d(TAG, "Creating a new thread");
            thread = new Thread(threadRunnable);
            thread.start();
        }else {
            initXCobraContext(index);
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
                XCobraDevice.this.setConnected(true);
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
     * This is a convenience wrapper around the {@link #setPosition(float, float, float)} call.
     * This method applies the cameras model matrix to the x, y, z to give relative positions
     * with respect to the camera rig.
     *
     * This call is made from the native layer.
     *
     * @param x
     *            the x value of the position.
     * @param y
     *            the y value of the position.
     * @param z
     *            the z value of the position.
     */
    public void processPosition(float x, float y, float z) {
        this.setPosition(x,y,z);
    }

    /**
     * Modify the tranform's current rotation in quaternion terms
     * X-Cobra's rotation is based on world-space.
     *
     * This call is made from the native layer.
     *
     * @param w
     *            'W' component of the quaternion.
     * @param x
     *            'X' component of the quaternion.
     * @param y
     *            'Y' component of the quaternion.
     * @param z
     *            'Z' component of the quaternion.
     */
    public void processRotation(float w, float x, float y, float z) {
        // Pay attention to Quaternionf's components
        mRotation.set(x, y, z, w);
        // Fix it's yaw...
        mYawError.mul(mRotation,sQuatHelper);
        //sQuatHelper.set(mRotation);
        if(mUseRotation) {
            setRotation(sQuatHelper.w, sQuatHelper.x, sQuatHelper.y, sQuatHelper.z);
        }
    }

    /**
     * Synthesize and forward a KeyEvent to the library.
     *
     * This call is made from the native layer.
     *
     * @param code   id of the button
     * @param action integer representing the action taken on the button
     */
    public void dispatchKeyEvent(int code, int action) {
        //
        int keyCode=0;
        int keyAction=0;
        if (mButtonMap==null||!mButtonMap.containsKey(code)) {
            keyCode=code;
        } else{
            keyCode=mButtonMap.get(code);
        }

        if (action==ACTION_DOWN) {
            keyAction=KeyEvent.ACTION_DOWN;
        } else if (action==ACTION_UP) {
            keyAction=KeyEvent.ACTION_UP;
        }
        // Hack the START/HOME button.
        if(keyAction==KeyEvent.ACTION_DOWN&&keyCode==KeyEvent.KEYCODE_BUTTON_START){
            if(XDeviceModule.instance==null) {
                recenter();
            }else{
                XDeviceModule.instance.recenter();
            }
        }
        //
        KeyEvent keyEvent=new KeyEvent(keyAction, keyCode);
        Log.d(TAG,"dispatchKeyEvent {KeyCode:%d KeyAction:%d}", keyCode, keyAction);
        setKeyEvent(keyEvent);
    }

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

    public void initXCobraContext(int index){
        mIndex=index;
        mButtonMap=sButtonMap[mIndex];
        pluginHandle=XDeviceApi.getInputDeviceHandle(String.format("XCobra-%d",mIndex));
        if(pluginHandle>=0){
            XDeviceApi.addInputEventDispatcher(pluginHandle, this);
        }else {
            Log.e(TAG,"pluginHandle<0 at initXCobraContext(int).");
        }
    }

    public void setConnected2(boolean value){
        boolean valuePrev=isConnected();
        this.setConnected(value);
        if(valuePrev!=value) {
        //if(!valuePrev&&value) {
            recenter();
        }
    }

    /**
     * Recenter,reset X-Cobra's yaw.
     */
    public void recenter(){
        //
        GVRScene scene=context.getMainScene();
        if(scene==null){
            return;
        }
        // TODO: ObjectCursor.ControllerEventListener.lookAt() will cause that the recenter() doesn't work.
        // TODO: GVRTransformUtility.getYaw() fixes the issue the eulerAngles.y==0.0f but yaw!=0.
        float yaw=0f;//*GVRTransformUtility.getYaw(GVRTransformUtility.getRotation(scene.getMainCameraRig().getHeadTransform()));
        yaw=yaw-GVRTransformUtility.getYaw(mRotation);
        mYawError.identity();mYawError=mYawError.rotateAxis(yaw,0f,1f,0f);
        Log.d(TAG,"recenter : Yaw%.2f",yaw*57.29578f);
        // TODO:
        setPosition((mIndex * 2 - 1) * .5f, 1.25f, -1.0f);
    }

    public void triggerVirbation(long duration_ms) {
        // Cancel previous delayed call.
        /*...*/
        //
        XDeviceApi.sendMessage(pluginHandle,XDeviceConstants.kMessage_TriggerVibration,0,0);
        //
        XDeviceApi.sJuggler.delayCall(new Runnable() {
            @Override
            public void run () {
                XDeviceApi.sendMessage(pluginHandle, XDeviceConstants.kMessage_TriggerVibration,-1,0);
            }
        },duration_ms);
    }

    public void enableRotation(boolean value){
        if(mUseRotation&&!value){// Reset the rotation
             recenter();
        }
        mUseRotation=value;
    }


    /**
     * Start a native thread proc.You need call it in a new thread.
     */
    public void startNativeThreadProc(){

    }

    public int pluginHandle=-1;

    public boolean checkPluginHandle(int handle){
        return (handle==pluginHandle);
    }

    @Override
    public void dispatchAxisEvent(int handle, int axis, float value) {
        if(!checkPluginHandle(handle)){
            return;
        }
    }

    @Override
    public void dispatchKeyEvent(int handle,int code, int action) {
        if(!checkPluginHandle(handle)){
            return;
        }
        dispatchKeyEvent(code,action);
    }

    @Override
    public void dispatchPositionEvent(int handle, int node, float x, float y, float z) {
        if(!checkPluginHandle(handle)){
            return;
        }
        switch (node) {
            case 0:
                // We cann't use the position directly.
                if(true){
                    return;
                }
                processPosition(x, y, z);
            break;
        }
    }

    @Override
    public void dispatchRotationEvent(int handle, int node, float x, float y, float z, float w) {
        if(!checkPluginHandle(handle)){
            return;
        }
        switch (node) {
            case 0:
                processRotation(w,x,y,z);
            break;
        }
    }
}


