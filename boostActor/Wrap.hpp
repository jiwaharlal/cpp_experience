template <class T>
struct Wrap{};

template <typename FunctorType>
struct FunctorWrapper: public FunctorType
{
   template <typename T1>
   FunctorWrapper(T1 t1)
      : FunctorType(t1)
   {}

   template <typename T1, typename T2>
   FunctorWrapper(T1 t1, T2 t2)
      : FunctorType(t1, t2)
   {}

   template <typename T>
   void operator ()(Wrap<T>&)
   {
      FunctorType::template operator ()<T>();
   }
};
