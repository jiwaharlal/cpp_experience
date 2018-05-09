#include "SentenceExtractor.hpp"

void SentenceExtractor::pushBytes(const char* bytes)
{
   mBuffer += bytes;
   if (mBuffer.find("$") == std::string::npos)
   {
      mBuffer.clear();
   }
}

std::string SentenceExtractor::tryPopSentence()
{
   std::string result;

   size_t sentenceStart = mBuffer.find("$");
   if (sentenceStart == std::string::npos)
   {
      return std::string();
   }
   mBuffer = mBuffer.substr(sentenceStart);

   size_t sentenceEnd = mBuffer.find("\n");
   if (sentenceEnd == std::string::npos)
   {
      return std::string();
   }
   result = mBuffer.substr(0, sentenceEnd);
   mBuffer = mBuffer.substr(sentenceEnd);

   return result;
}
