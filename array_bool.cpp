#include <iostream>
#include <stdexcept>
#include <vector>
#include <stdint.h>

template <typename IntType>
struct ArrayBool
{
   enum { bitSizeof = sizeof(IntType) * 8 };

   struct AssignHelper
   {
      AssignHelper(IntType& bitSet, size_t bitIndex)
         : mBitSet(bitSet)
         , mBitIndex(bitIndex)
      {
      }

      void operator =(bool value)
      {
         IntType mask = ~(1 << mBitIndex);
         IntType content = (value ? 1 : 0) << mBitIndex;
         mBitSet &= mask;
         mBitSet |= content;
      }

      operator bool() const
      {
         IntType mask = 1 << mBitIndex;

         return mBitSet & mask;
      }

      IntType& mBitSet;
      size_t mBitIndex;
   };

   AssignHelper operator [](size_t index)
   {
      //std::cout << "operator []" << std::endl;

      size_t bitSetIndex = index / bitSizeof;
      size_t bitIndex = index % bitSizeof;

      if (bitSetIndex >= mStorage.size())
      {
         throw std::out_of_range("ArrayBool::operator[] out of range");
      }

      return AssignHelper(mStorage[bitSetIndex], bitIndex);
   }

   bool operator [](size_t index) const
   {
      //std::cout << "operator [] const" << std::endl;

      size_t bitSetIndex = index / bitSizeof;
      size_t bitIndex = index % bitSizeof;

      if (bitSetIndex >= mStorage.size())
      {
         return false;
      }

      IntType mask = 1 << bitIndex;
      return mStorage[bitSetIndex] & mask;
   }

   size_t size() const
   {
      return mStorage.size() * sizeof(IntType) * 8;
   }

   void resize(size_t newSize)
   {
      mStorage.resize(newSize / (sizeof(IntType) * 8) + 1);
   }

   std::vector<IntType> mStorage;
};

template <typename IntType>
std::ostream& operator <<(std::ostream& out, const ArrayBool<IntType>& bitSet)
{
   for (size_t i = 0, limit = bitSet.size(); i < limit; ++i)
   {
      out << bitSet[i] << ", ";
   }

   return out;
}

int main()
{
   std::cout << sizeof(uint8_t) << std::endl;

   ArrayBool<uint8_t> ab;
   ab.resize(5);
   ab[5] = true;

   std::cout << ab << std::endl;

   return 0;
}
