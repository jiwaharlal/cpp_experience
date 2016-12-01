#pragma once

class IDownloadProgressListener
{
   void onProgressUpdate(double total, double current) = 0;
};
