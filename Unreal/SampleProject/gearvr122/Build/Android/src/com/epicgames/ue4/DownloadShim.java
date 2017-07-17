package com.epicgames.ue4;

import com.xim.gearvrtest.OBBDownloaderService;
import com.xim.gearvrtest.DownloaderActivity;


public class DownloadShim
{
	public static OBBDownloaderService DownloaderService;
	public static DownloaderActivity DownloadActivity;
	public static Class<DownloaderActivity> GetDownloaderType() { return DownloaderActivity.class; }
}

