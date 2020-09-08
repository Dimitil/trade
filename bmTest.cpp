#include <benchmark/benchmark.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <list>


const size_t ar_size = 500; //less or equal 153k

void updateMap( double *ar_price, int *ar_amount, std::map <double, int> &map)
{
       for(int i=0; i<ar_size; i++){ 
            if( 0 == ar_amount[i] ){
                auto it = map.find(ar_price[i]);
                if( it != map.end() ){
                    map.erase(it);
                    continue;
                }
            }
            if( ar_price[i]>0 && ar_amount[i] > 0 ){
                map[ar_price[i]] = ar_amount[i];
            }
        }
}


void updateVector(double* ar_price, int* ar_amount, std::vector <std::pair<double, int>> &v){
    for(int i=0; i<ar_size; i++){ 
        if (ar_price[i] > 0 && ar_amount[i] >= 0){
            auto it = v.begin(); 
            for(; it != v.end(); it++){
                if(it->first == ar_price[i]) break;
            }
            if( it != v.end()){
                if(ar_amount[i] == 0){
                   v.erase(std::remove(v.begin(), v.end(), *it), v.end());
                   continue;
                }
                it->second = ar_amount[i];
            }
            else {
                v.push_back(std::make_pair(ar_price[i], ar_amount[i]));
            }
        }
    }
}

void updateList(double* ar_price, int* ar_amount, std::list <std::pair<double, int>> &l){
    for(int i=0; i<ar_size; i++){ 
        if (ar_price[i] > 0 && ar_amount[i] >= 0){
            auto it = l.begin(); 
            for(; it != l.end(); it++){
                if(it->first==ar_price[i]) break;
                }
            if( it != l.end()){
                if(ar_amount[i] == 0){
                   l.erase(it);
                   continue;
                }
                it->second = ar_amount[i];
            }
            else {
                l.push_back(std::make_pair(ar_price[i], ar_amount[i]));
            }
        }
    }
}

void BM_MapUpdate(benchmark::State& state) {
    std::map <double, int> map;
    std::ifstream ifs("test");
    double ar_price[ar_size];
    int ar_amount[ar_size];
    for(int i=0; i<ar_size; i++){
       ifs >> ar_price[i];
       ifs >> ar_amount[i];
    }
    ifs.close();

    for(auto _ : state) {
        updateMap(ar_price, ar_amount, map);
    }
}
BENCHMARK(BM_MapUpdate);//->Unit(benchmark::kMillisecond);


void BM_VectorUpdate(benchmark::State& state) {
    std::vector <std::pair <double, int>> vec;
    std::ifstream ifs("test");
    double ar_price[ar_size];
    int ar_amount[ar_size];
    for(int i=0; i<ar_size; i++){
       ifs >> ar_price[i];
       ifs >> ar_amount[i];
       if(!ifs.good()) {
           std::cerr<<"\nERROR 1\n";
           exit(1);
       }
    }
    ifs.close();

    for(auto _ : state){
        updateVector(ar_price, ar_amount, vec);
    }
}
BENCHMARK(BM_VectorUpdate);//->Unit(benchmark::kMillisecond);

void BM_ListUpdate(benchmark::State& state) {
    std::list <std::pair <double, int>> l;
    std::ifstream ifs("test");
    double ar_price[ar_size];
    int ar_amount[ar_size];
    for(int i=0; i<ar_size; i++){
       ifs >> ar_price[i];
       ifs >> ar_amount[i];
       if(!ifs.good()) {
           std::cerr<<"\nERROR 1\n";
           exit(1);
       }
    }
    ifs.close();

    for(auto _ : state){
        updateList(ar_price, ar_amount, l);
    }
}
BENCHMARK(BM_ListUpdate);//->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
