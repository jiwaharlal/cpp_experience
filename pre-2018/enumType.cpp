#include <iostream>
#include <map>
#include <typeinfo>

template <typename EnumType1, typename EnumType2>
class TEnumConverter
{
   public:
      TEnumConverter(EnumType1 defaultValue1, EnumType2 defaultValue2);

      void addMatch(EnumType1 value1, EnumType2 value2);
      EnumType1 convert(EnumType2 value);
      EnumType2 convert(EnumType1 value);
   private:
      typedef std::map<EnumType1, EnumType2> tMapType1to2;
      typedef std::map<EnumType2, EnumType1> tMapType2to1;
      tMapType1to2 mMap1to2;
      tMapType2to1 mMap2to1;
      EnumType1 mDefaultValue1;
      EnumType2 mDefaultValue2;
};

template <typename EnumType1, typename EnumType2>
TEnumConverter<EnumType1, EnumType2>::TEnumConverter(EnumType1 defaultValue1, EnumType2 defaultValue2)
   : mDefaultValue1(defaultValue1)
   , mDefaultValue2(defaultValue2)
{
   addMatch(defaultValue1, defaultValue2);
}

template <typename EnumType1, typename EnumType2>
void TEnumConverter<EnumType1, EnumType2>::addMatch(EnumType1 value1, EnumType2 value2)
{
   mMap1to2[value1] = value2;
   mMap2to1[value2] = value1;
}

template <typename EnumType1, typename EnumType2>
EnumType1 TEnumConverter<EnumType1, EnumType2>::convert(EnumType2 value)
{
   std::cout << "Converting type " << typeid(EnumType2).name() << " to " << typeid(EnumType1).name() << std::endl;
   typename tMapType2to1::const_iterator it = mMap2to1.find(value);
   if (it == mMap2to1.end())
   {
      return mDefaultValue1;
   }
   return it->second;
}

template <typename EnumType1, typename EnumType2>
EnumType2 TEnumConverter<EnumType1, EnumType2>::convert(EnumType1 value)
{
   std::cout << "Converting type " << typeid(EnumType1).name() << " to " << typeid(EnumType2).name() << std::endl;
   typename tMapType1to2::const_iterator it = mMap1to2.find(value);
   if (it == mMap1to2.end())
   {
      return mDefaultValue2;
   }
   return it->second;
}

enum GpsFixType {
  GpsFixType_NoFix = 0,
  GpsFixType_FixTime = 1,
  GpsFixType_Fix2D = 2,
  GpsFixType_Fix3D = 3
};

struct ChannelGpsFixType
{
   enum EType
   {
      GpsNoFix,      // No GPS fix, also named as 0D-Fix.
      GpsFixTime,    // A GPS TIme fix is available. The time member of the GPS dataset are valid. Sometimes also named as 1D-Fix.
      GpsFix2D,      // A 2D-GPS Fix are available. The height member of GPS is constant, the position could be unstable.
      GpsFix3D       // A fully 3D-Fix are available. All normal position member of the GPS dataset are valid.
   };

   ChannelGpsFixType() : value(GpsFix2D) {}
   operator EType() { return value; }

   EType value;
};

struct GpsQuality
{
   int numberOfSatellites;
   ChannelGpsFixType fixType;
};

TEnumConverter<GpsFixType, ChannelGpsFixType::EType> createFixTypeConverter()
{
   TEnumConverter<GpsFixType, ChannelGpsFixType::EType> converter(GpsFixType_NoFix, ChannelGpsFixType::GpsNoFix);
   converter.addMatch(GpsFixType_Fix2D, ChannelGpsFixType::GpsFix2D);
   converter.addMatch(GpsFixType_Fix3D, ChannelGpsFixType::GpsFix3D);
   converter.addMatch(GpsFixType_FixTime, ChannelGpsFixType::GpsFixTime);

   return converter;
}

TEnumConverter<GpsFixType, std::string> createGpsTypeToStringConverter()
{
   TEnumConverter<GpsFixType, std::string> converter(GpsFixType_NoFix, "No Fix");
   converter.addMatch(GpsFixType_FixTime, "FixTime");
   converter.addMatch(GpsFixType_Fix2D, "Fix2D");
   converter.addMatch(GpsFixType_Fix3D, "Fix3D");

   return converter;
}

TEnumConverter<ChannelGpsFixType::EType, std::string>  createChannelToStringConverter()
{
   TEnumConverter<ChannelGpsFixType::EType, std::string> converter(ChannelGpsFixType::GpsNoFix, "Channel no fix");
   converter.addMatch(ChannelGpsFixType::GpsFixTime, "fix time");
   converter.addMatch(ChannelGpsFixType::GpsFix2D, "fix 2d");
   converter.addMatch(ChannelGpsFixType::GpsFix3D, "fix 3d");

   return converter;
}

static TEnumConverter<GpsFixType, ChannelGpsFixType::EType> sFixTypeConverter = createFixTypeConverter();
static TEnumConverter<GpsFixType, std::string> sSimpleTypeToStr = createGpsTypeToStringConverter();
static TEnumConverter<ChannelGpsFixType::EType, std::string> sEmbeddedTypeToStr = createChannelToStringConverter();

std::string toStr(GpsFixType value)
{
   return sSimpleTypeToStr.convert(value);
}

std::string toStr(ChannelGpsFixType::EType value)
{
   return sEmbeddedTypeToStr.convert(value);
}

template <GpsFixType EnumValue1, ChannelGpsFixType::EType EnumValue2>
struct ValueConverter
{
   GpsFixType convert(ChannelGpsFixType::EType)
   {
      return EnumValue1;
   }

   ChannelGpsFixType::EType convert(GpsFixType)
   {
      return EnumValue2;
   }
};

int main(int, char**)
{
   GpsFixType ft;
   std::cout << "Simple enum value " << ft << std::endl;

   //ChannelGpsFixType fixType;
   GpsQuality q = {0};
   std::cout << sEmbeddedTypeToStr.convert(q.fixType) << std::endl;

   std::cout << toStr(sFixTypeConverter.convert(GpsFixType_Fix2D)) << std::endl;
   std::cout << toStr(sFixTypeConverter.convert(ChannelGpsFixType::GpsFix3D)) << std::endl;

   std::cout << "sizeof enum " << sizeof(ChannelGpsFixType::EType) << std::endl;
   std::cout << "sizeof ChannelGpsFixType " << sizeof(ChannelGpsFixType) << std::endl;

   return 0;
}
