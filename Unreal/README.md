<div align=right><img src="Tools/imgs/xim.png" ></div>

<h1> Unreal 4 Integration </h1>

<h2> Running On PC </h2>
1. Follow the PC setup (here)[https://github.com/Ximmerse/SDK/tree/master/Tools/XDeviceMonitor]
2. Once step 1 is finished, make sure devices are all hooked up to PC before you start the Unreal 4 engine. 
If you plug in devices after starting Unreal 4 engine, it the devices wouldn't work. 
This is due to the nature of plugin life cycle handling in Unreal 4 Engine. 
3. Hit "Play" button. You should be able to see the camera moving in the game scene while moving the Blue LED blob.

<h2> Running on GearVR</h2>
1. Please follow (this guide)[https://github.com/Ximmerse/SDK/tree/master/Tools/AndroidXimService] to make sure Samsung phone is Xim device ready.
2. Make sure you have Osig files ready and copy them into the assets folder. (C:\Program Files (x86)\Epic Games\4.12\Engine\Build\Android\Java\assets) (The path may be different on your machine depending on engine installation location and engine version ).
	<div align = center>
	<img src="../Tools/imgs/copy_osigs.png" width="700">
    </div>
3. Copy the "xdevice-api_library" folder in to "C:\Program Files (x86)\Epic Games\4.12\Engine\Build\Android\Java\JavaLibs" directory(The path may be different on your machine depending on engine installation location and engine version ).
	<div align = center>
	<img src="../Tools/imgs/copy_java_libs.png" width="700">
    </div>
4. Please make sure you have all the 7 permissions in "Advanced Packaging" setting. 
	`android.permission.ACCESS_COARSE_LOCATION`
	`android.permission.INTERNET`
	`android.permission.WAKE_LOCK`
	`android.permission.READ_EXTERNAL_STORAGE`
	`android.permission.BLUETOOTH`
	`android.permission.BLUETOOTH_ADMIN`
	`android.permission.ACCESS_FINE_LOCATION`
5. Once above steps are finsihed, all you need to do is make a build and launch it from your android phone. 

<h2> Sample Project </h2>
Sample project is provided in this directory. 
The project is configured to work out of box. 
All you have to do is make sure you finish the PC setup if you wanna run it on PC, or finish the android phone setup if you wanna test it on GearVR.