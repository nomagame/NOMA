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

package org.gearvrf;

import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import org.gearvrf.scene_objects.GVRViewSceneObject;
import org.gearvrf.scene_objects.view.GVRTextView;
import org.gearvrf.scene_objects.view.GVRView;
import org.gearvrf.scene_objects.view.GVRWebView;
import org.joml.Quaternionf;

public class GVRViewUtility {

    protected static Quaternionf sQuatHelper=new Quaternionf();

    public static GVRTextView createTextView(
        GVRActivity gvrActivity,String text,
        int textSize,int textColor,int backgroundColor,
        int width,int height,int gravity
    ) {
        /*return null;*/

        GVRTextView textView = new GVRTextView(gvrActivity, width, height);
        textView.setTextSize(textSize);
        textView.setTextColor(textColor);
        //setTextViewProperties(textView, resources);
        textView.setBackgroundColor(backgroundColor);
        textView.setGravity(gravity);
        //
        return textView;
    }

    public static GVRWebView createWebView(
        GVRActivity gvrActivity,String url,
        int width,int height,int scale
    ) {
        GVRWebView webView = new GVRWebView(gvrActivity);
        webView.setInitialScale(scale);
        webView.measure( width, height);
        webView.layout(0, 0, width, height);
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webView.loadUrl(url);
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });
        return webView;
    }

    public static GVRViewSceneObject createGVRViewSceneObject(
            GVRContext gvrContext, GVRScene mainScene,
            GVRView view, int renderingOrder,
            float x, float y, float z,
            float width, float height,
            float eulerX, float eulerY, float eulerZ
    ) {
        /*return null;*/

        GVRViewSceneObject text = new GVRViewSceneObject(gvrContext, view,
                gvrContext.createQuad( width, height));
        text.getTransform().setPosition( x, y, z);
        //
        sQuatHelper.identity();
        sQuatHelper.rotationXYZ(eulerX,eulerY,eulerZ);
        text.getTransform().setRotation(sQuatHelper.w, sQuatHelper.x, sQuatHelper.y, sQuatHelper.z);
        text.getRenderData().setRenderingOrder(renderingOrder);
        mainScene.addSceneObject(text);
        //
        return  text;
    }


}
