#include <curl/curl.h>
#include "CurlRequest.h"

CurlRequest::CurlRequest(
          const std::string& url,
          std::ostream& out,
          tProgressCallback progressUpdateCallback)
   : mUrl(url)
   , mProgressCallback(progressUpdateCallback)
   , mOut(out)
{

}

int CurlRequest::curlProgressCallback(void* requestPtr, double dltotal, double dlnow, double, double)
{
   return reinterpret_cast<CurlRequest*>(requestPtr)->mProgressCallback(dltotal, dlnow);
}

size_t CurlRequest::curlWriteCallback(void *contents, size_t size, size_t nmemb, void *receiverPtr)
{
   reinterpret_cast<std::ostream*>(receiverPtr)->write(reinterpret_cast<char*>(contents), size * nmemb);
   return size * nmemb;
}

bool CurlRequest::run()
{
   CURL *curl;
   CURLcode res;
   std::string readBuffer;

   curl = curl_easy_init();
   if(curl)
   {
      curl_easy_setopt(curl, CURLOPT_URL, mUrl.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlWriteCallback);
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
      curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, &curlProgressCallback);
      curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mOut);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);

      return res == CURLE_OK;
   }

   return false;
}
