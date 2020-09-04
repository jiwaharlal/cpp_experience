#include <iostream>
#include <sstream>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <boost/phoenix.hpp>
#include <boost/thread.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "curlCpp/CurlRequest.h"

namespace io = boost::iostreams;

int progressReport(double total, double current)
{
   std::cout << "\rProgress: " << current << "/" << total;
   return 0;
}

void load(const std::string& url, std::ostream& out)
{
   CurlRequest request(url, out, &progressReport);
   if (!request.run())
   {
      std::cout << "Error loading data" << std::endl;
   }
   out.flush();
}

int main(int argc, char** argv)
{
   // https://maps.wikimedia.org/osm-intl/11/327/792.png

   //using namespace cv;
   using namespace boost::phoenix;

   if (argc < 2)
   {
      std::cout << "Usage: curlDownloader <url>" << std::endl;
      return 1;
   }

   std::string url(argv[1]);

   std::cout << "Requested: " << url << std::endl;

   std::vector<char> data;
   io::filtering_ostream dataOstream(io::back_inserter(data));

   //CurlRequest request(url, dataOstream, boost::phoenix::val(0));
   //if (!request.run())
   //{
      //std::cout << "Error loading data" << std::endl;
      //return -1;
   //}
   //dataOstream.flush();

   boost::thread loadThread(boost::bind(&load, url, boost::ref(dataOstream)));
   loadThread.join();

   std::cout << std::endl << "Data loaded : "  << data.size() << " bytes" << std::endl;

   boost::this_thread::sleep(boost::posix_time::milliseconds(500));

   //Mat image;

   //if (argc == 3)
   //{
      //std::ofstream fout(argv[2]);
      //fout.write(&data[0], data.size());
      //fout.close();

      //image = imread(argv[2]);
   //}
   //else
   //{
      //int    size = data.size();       // Size of buffer

      //// Create a Size(1, nSize) Mat object of 8-bit, single-byte elements
      //cv::Mat rawData  =  cv::Mat(1, &size, CV_8UC1, &data[0]);
      ////Mat rawData = imgbuf(Size(256, 256), CV_8UC3, data);
      //if (!rawData.data)
      //{
         //std::cout << "Wrong raw data" << std::endl;
      //}

      //image = imdecode(rawData, CV_LOAD_IMAGE_COLOR);
   //}

   //if(!image.data)                              // Check for invalid input
   //{
      //std::cout <<  "Could not open or find the image" << std::endl ;
      //return -1;
   //}

   //namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
   //imshow( "Display window", image );                   // Show our image inside it.

   //waitKey(0);                                          // Wait for a keystroke in the window
   return 0;
}
