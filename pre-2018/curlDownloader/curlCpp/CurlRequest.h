#ifndef __CURL_REQUEST_H__
#define __CURL_REQUEST_H__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <ostream>

class IDownloadProgressListener;

class CurlRequest
{
public:
   typedef boost::function<bool(double, double)> tProgressCallback;

public:
    CurlRequest(
          const std::string& url,
          std::ostream& out,
          tProgressCallback progressUpdateCallback);

    bool run();

private:
    static int curlProgressCallback(
          void* requestPtr,
          double dltotal,
          double dlnow,
          double ultotal,
          double ulnow);

    static size_t curlWriteCallback(
          void *contents,
          size_t size,
          size_t nmemb,
          void *requestPtr);

private:
    const std::string mUrl;
    const tProgressCallback mProgressCallback;
    std::ostream& mOut;
};

#endif // __CURL_REQUEST_H__
