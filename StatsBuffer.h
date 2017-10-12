
template<typename T, size_t N, typename stats_type>
class StatsBuffer {
   static_assert(N != 0);
   T data[N] = {};
   size_t index = 0;
   stats_type _mean = 0;
   stats_type variance_sum = 0; 
   
public:
   void push(const T item);
   const stats_type mean() {return _mean;}
   const stats_type variance() {return variance_sum/N;}

   T& operator[](size_t i) {return data[i];}
};


   
template<typename T, size_t N, typename stats_type>
void StatsBuffer<T,N,stats_type>::push(const T item) {
   index = (index + 1) % N;

   _mean = _mean + (item)/N - data[index]/N;

   data[index] = item;
}



