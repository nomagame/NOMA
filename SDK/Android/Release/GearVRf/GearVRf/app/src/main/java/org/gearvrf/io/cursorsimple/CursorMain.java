/* Copyright 2015 Samsung Electronics Co., LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.gearvrf.io.cursorsimple;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;

import com.ximmerse.gearvrf.XDeviceModule;

import org.gearvrf.GVRBitmapTexture;
import org.gearvrf.GVRCameraRig;
import org.gearvrf.GVRContext;
import org.gearvrf.GVRDirectLight;
import org.gearvrf.GVRMain;
import org.gearvrf.GVRScene;
import org.gearvrf.GVRSceneObject;
import org.gearvrf.GVRTexture;
import org.gearvrf.GVRTransform;
import org.gearvrf.io.cursor3d.CursorManager;
import org.gearvrf.io.cursor3d.IoDevice;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * This sample can be used with a Laser Cursor as well as an Object Cursor. By default the Object
 * Cursor is enabled. To switch to a Laser Cursor simply rename the "laser_cursor_settings.xml"
 * in the assets directory to "settings.xml"
 */
public class CursorMain extends GVRMain {
    private static final String TAG = CursorMain.class.getSimpleName();
    private GVRScene mainScene;
    private GVRCameraRig cameraRig;
    private CursorManager cursorManager;

    protected XDeviceModule mXDeviceModule;

    @Override
    public void onInit(GVRContext gvrContext) {
        //
        mainScene = gvrContext.getNextMainScene();
        cameraRig=mainScene.getMainCameraRig();
        cameraRig.getLeftCamera().setBackgroundColor(Color.BLACK);
        cameraRig.getRightCamera().setBackgroundColor(Color.BLACK);
        cameraRig.getTransform().setPosition(0,1.6f,0);
        // Build a scene.
        GVRSceneObject theaterSo=null;
        try {
            theaterSo=gvrContext.loadModel("home_theater.fbx");
        } catch (IOException e) {
            e.printStackTrace();
        }
        if(theaterSo!=null) {
            //
            GVRTransform t=theaterSo.getTransform();
            t.setRotationByAxis(180.0f,0f,1f,0f);
            //t.setScale(.01f,.01f,.01f);
            mainScene.addSceneObject(theaterSo);
            // Create a light.
            GVRSceneObject lightSo=new GVRSceneObject(gvrContext);
            t=lightSo.getTransform();
            t.setPosition(2f,2f,-2f);
            t.setRotation(0.8001031f,-0.4619398f,-0.3314136f,-0.1913417f);// eulerAngles(60f,45f,0f) in Unity.
            GVRDirectLight light=new GVRDirectLight(gvrContext,lightSo);
            mainScene.addSceneObject(lightSo);
        }
        //
        List<IoDevice> devices=new ArrayList<IoDevice>();
        // XDevice TODO: Step I: Initialize XDeviceModule before you create CursorManager.
        if(true) {
            mXDeviceModule=new XDeviceModule(gvrContext);
            mXDeviceModule.start(mainScene, null, null);
            // XDevice TODO: Very Important: Set position and rotation of tracker.
            GVRTransform t=mXDeviceModule.getTracker();
            if(t!=null){
                t.setRotationByAxis(180.0f,0f,1f,0f);
                t.setPosition(0f,1.5f,-1.5f);
            }
            // XDevice TODO: Very Important:Set offset between head blob and CameraRig rotation pivot.
            mXDeviceModule.setHeadBlobOffset(0f,.2f,0f);
            // Add all IoDevices in XDeviceModule into device list.
            mXDeviceModule.addDevicesTo(devices);
        }
        //
        cursorManager=new CursorManager(gvrContext, mainScene,devices);
    }

    @Override
    public void onStep() {
        // XDevice TODO: Step II: Update XDeviceModule per frame,in order to dispatch all input events.
        if(mXDeviceModule!=null) {
            mXDeviceModule.tick();
        }
    }

    void close() {
        if (cursorManager != null) {
            cursorManager.close();
        }
        // XDevice TODO: Step III: Finalize XDeviceModule on quit the application
        if(mXDeviceModule!=null){
            mXDeviceModule.stop();
        }
    }

    @Override
    public GVRTexture getSplashTexture(GVRContext gvrContext) {
        Bitmap bitmap = BitmapFactory.decodeResource(
                gvrContext.getContext().getResources(),
                R.mipmap.ic_launcher);
        // return the correct splash screen bitmap
        return new GVRBitmapTexture(gvrContext, bitmap);
    }
}
