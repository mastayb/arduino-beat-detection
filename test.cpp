#include <cstdlib>
#include <iostream>
#include "StatsBuffer.h"

using namespace std;

int main() {
   StatsBuffer<int, 5, float> buf;
   
   int r[5];
   
   cout<<buf.mean()<<endl;
   cout<<buf.variance()<<endl;
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);

   cout<<buf.mean()<<endl;
   cout<<buf.variance()<<endl;


      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);

   for (auto i:{0,1,2,3,4})
      cout<<buf[i]<<endl;

   cout<<buf.mean()<<endl;
   cout<<buf.variance()<<endl;

      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);

   for (auto i:{0,1,2,3,4})
      cout<<buf[i]<<endl;

   cout<<buf.mean()<<endl;
   cout<<buf.variance()<<endl;


      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);
      buf.push(rand() % 10);

   for (auto i:{0,1,2,3,4})
      cout<<buf[i]<<endl;

   cout<<buf.mean()<<endl;
   cout<<buf.variance()<<endl;

}


