#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <boost/phoenix.hpp>

#include "curlCpp/CurlRequest.h"

int progressReport(double total, double current)
{
   std::cout << "\rProgress: " << current << "/" << total;
   return 0;
}

int main(int argc, char** argv)
{
   // https://maps.wikimedia.org/osm-intl/11/327/792.png

   using namespace cv;
   using namespace boost::phoenix;

   if (argc < 2)
   {
      std::cout << "Usage: curlDownloader <url>" << std::endl;
      return 1;
   }

   std::string url(argv[1]);

   std::cout << "Requested: " << url << std::endl;

   std::stringstream requestResult;
   //CurlRequest request(url, requestResult, &progressReport);
   CurlRequest request(url, requestResult, val(0));
   request.run();

   Mat image;

   if (argc == 3)
   {
      std::ofstream fout(argv[2]);
      fout << requestResult.str();
      fout.close();

      Mat image = imread(argv[2]);
   }
   else
   {
      int    size = requestResult.str().size();       // Size of buffer
      std::vector<char> buf(requestResult.str().begin(), requestResult.str().end());

      // Create a Size(1, nSize) Mat object of 8-bit, single-byte elements
      cv::Mat rawData  =  cv::Mat(1, &size, CV_8UC1, reinterpret_cast<void*>(&buf[0]));

      Mat image = imdecode(rawData, CV_LOAD_IMAGE_COLOR);
   }

   if(!image.data)                              // Check for invalid input
   {
      std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
   }

   namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
   imshow( "Display window", image );                   // Show our image inside it.

   waitKey(0);                                          // Wait for a keystroke in the window
   return 0;
}
