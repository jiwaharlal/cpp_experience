#define _USE_MATH_DEFINES
#include <boost/algorithm/clamp.hpp>
#include <cmath>
#include <cstdint>
#include <ctgmath>
#include <iostream>
#include <math.h>
#include <stdint.h>

static const int32_t sMaxTileLevel = 22;
static const int32_t sTileResolution = 256;

namespace NNavigationData
{

struct CGeoCoordinate
{
   CGeoCoordinate(double longitude, double latitude)
      : mLongitude(longitude)
      , mLatitude(latitude)
   {}

   double getLongitude() const { return mLongitude; }
   double getLatitude() const { return mLatitude; }

   double mLongitude;
   double mLatitude;
};

} // NNavigationData

struct Point
{
   int x;
   int y;
};

double log(double base, double x)
{
   return log(x) / log(base);
}

uint32_t getTileLevelForScale(double pixelsPerDegree)
{
   double n = pixelsPerDegree * 360. / sTileResolution;
   return boost::algorithm::clamp(floor(log(2, n)), 1, 22);
}

   //typedef std::pair<int32_t, double> tResolutionPair;
   //typedef std::vector<tResolutionPair> tResolutionList;

   //tResolutionList resolutionList;
   //for (int32_t i = 1; i <= sMaxTileLevel; ++i)
   //{
      //double tileResolution = sTileResolution / (360. / pow(2, i));
      //resolutionList.push_back(std::make_pair(i, tileResolution));
   //}

   //tResolutionList::const_iterator it = boost::lower_bound(
         //resolutionList,
         //pixelsPerDegree,
         //boost::bind(&tResolutionPair::second, _1) < _2);

   //return it->first;
//}

Point tileForCoord(const NNavigationData::CGeoCoordinate& coord, uint32_t level)
{
   Point tile;

   tile.x = floor(((coord.getLongitude() + 180) / 360) * pow(2, level));

   double n = sin(coord.getLatitude() * M_PI/180);
   tile.y = floor((.5 - log((1 + n) / (1 - n)) / (4 * M_PI)) * pow(2, level));

   return tile;
}

double tile2lon(uint32_t x, uint32_t level)
{
   return x / pow(2, level) * 360 - 180;
}

double tile2lat(uint32_t y, uint32_t level)
{
   double n = M_PI - 2 * M_PI * y / pow(2, level);
   return 180 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

NNavigationData::CGeoCoordinate tileLeftTop(uint32_t x, uint32_t y, uint32_t level)
{
   return NNavigationData::CGeoCoordinate(
         tile2lon(y, level),
         tile2lat(x, level));
}

int main()
{
   std::cout << getTileLevelForScale(1000.) << std::endl;
   std::cout << "Tile origin lon, lat: " << tile2lon(4299, 13) << ", " << tile2lat(2821, 13) << std::endl;

   Point p = tileForCoord(NNavigationData::CGeoCoordinate(8.92, 48.75), 10);
   std::cout << "Tile for coord " << p.x << ", " << p.y << std::endl;

   return 0;
}
