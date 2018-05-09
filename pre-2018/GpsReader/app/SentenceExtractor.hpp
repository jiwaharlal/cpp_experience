#pragma once

#include <string>

class SentenceExtractor
{
public:
   void pushBytes(const char* bytes);
   std::string tryPopSentence();
private:
   std::string mBuffer;
};
