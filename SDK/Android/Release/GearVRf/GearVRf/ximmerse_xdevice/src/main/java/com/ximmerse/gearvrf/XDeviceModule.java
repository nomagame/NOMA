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

import com.ximmerse.sdk.DeviceVersion;
import com.ximmerse.sdk.LaunchMode;
import com.ximmerse.sdk.XDeviceApi;

import org.gearvrf.GVRCameraRig;
import org.gearvrf.GVRContext;
import org.gearvrf.GVRScene;
import org.gearvrf.GVRSceneObject;
import org.gearvrf.GVRTransform;
import org.gearvrf.io.cursor3d.IoDevice;
import org.gearvrf.utility.Log;

import java.util.List;

public class XDeviceModule {

    private static final String TAG = XDeviceModule.class.getSimpleName();
    public static XDeviceModule instance;

    protected GVRContext mContext;

    protected XHawkDevice mXHawk;
    protected XCobraDevice[] mXCobras;

    public XDeviceModule(final GVRContext context){
        if(instance==null){
            instance=this;
        }else if(instance!=this){
            Log.e(TAG,"instance!=this at ctor()");
            return;
        }
        mContext=context;
    }

    public void start(GVRScene scene, GVRCameraRig cameraRig, GVRTransform tracker){
        if(mContext==null){
            return;
        }
        //
        if(mXHawk==null) {
            XDeviceApi.init(mContext.getContext(), DeviceVersion.DEVELOPMENT_KIT_4, LaunchMode.GEARVR_FRAMEWORK);
            mXHawk=new XHawkDevice(mContext,"X-Hawk 0","Tracker");
            mXCobras=new XCobraDevice[2];
            mXCobras[0]=new XCobraDevice(mContext,"X-Cobra 0","Left controller",0);
            mXCobras[1]=new XCobraDevice(mContext,"X-Cobra 1","Right controller",1);
        }
        //
        if(tracker==null){
            GVRSceneObject so=new GVRSceneObject(mContext);
            tracker=so.getTransform();
            scene.addSceneObject(so);
        }
        if(cameraRig==null){
            cameraRig=scene.getMainCameraRig();
        }
        mXHawk.setSceneObjects(tracker,cameraRig,mXCobras[0],mXCobras[1]);
        //
        mXCobras[0].setConnected2(true);
        mXCobras[1].setConnected2(true);
        //
        recenter();
    }

    public void stop(){
        XDeviceApi.exit();
    }

    public void tick(){
        //XDeviceApi.dispatchAllInputEvents();
    }

    public void addDevicesTo(List<IoDevice> devices){
        if(mXCobras!=null){
            devices.add(mXCobras[0]);
            devices.add(mXCobras[1]);
        }
    }

    public void recenter(){
        //
        if(mXHawk!=null){
            mXHawk.recenter();
        }
        //
        if(mXCobras!=null){
            mXCobras[0].recenter();
            mXCobras[1].recenter();
        }
    }

    public void triggerVirbation(int index, long duration_ms){
        if(mXCobras!=null){
            mXCobras[index].triggerVirbation(duration_ms);
        }
    }

    // Properties.
    
    public GVRCameraRig getCameraRig(){
        if(mXHawk!=null){
            return mXHawk.getCameraRig();
        }
        return null;
    }
    
    public void setCameraRig(GVRCameraRig cameraRig){
        if(mXHawk!=null){
            mXHawk.setCameraRig(cameraRig);
        }
    }
    
    public GVRTransform getTracker(){
        if(mXHawk!=null){
            return mXHawk.getTracker();
        }
        return null;
    }
    
    public void setTracker(GVRTransform tracker){
        if(mXHawk!=null){
            mXHawk.setTracker(tracker);
        }
    }

    public void setHeadBlobOffset(float x, float y, float z){
        if(mXHawk!=null){
            mXHawk.headBlobOffset.set(x, y, z);
        }

    }

}
