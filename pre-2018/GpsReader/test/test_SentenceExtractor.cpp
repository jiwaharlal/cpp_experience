#include "SentenceExtractor.hpp"
#include "catch.hpp"

TEST_CASE("Sentence extracted from buffer", "[sentenceExtractor]")
{
   SentenceExtractor se;

   se.pushBytes("566$123");
   std::string sentence = se.tryPopSentence();
   REQUIRE(sentence.empty());

   se.pushBytes("456\n234");
   sentence = se.tryPopSentence();
   REQUIRE(sentence == "$123456");
}
