#include <benchmark/benchmark.h>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <list>

void updateMap( std::ifstream &ifs, std::map <double, int> &map)
{
        while(ifs){
            int amount = -1;
            double price = -1;
            ifs >> price >> amount;
            if(ifs.good()) {
                if( 0 == amount ){
                    auto it = map.find(price);
                    if( it != map.end() ){
                        map.erase(it);
                        continue;
                    }
                }
                if( price>0 && amount > 0 ){
                    map[price] = amount;
                }
            }
        }
}


void updateVector(std::ifstream &ifs, std::vector <std::pair<double, int>> &v){
    while(ifs){
        int amount = -1;
        double price = -1;
        ifs >> price >> amount;
        if(ifs.good()){
            if ( price > 0 && amount >= 0){
               auto it = v.begin(); 
                for(; it != v.end(); it++){
                    if(it->first==price) break;
                }
                if( it != v.end()){
                    if(amount == 0){
                        v.erase(std::remove(v.begin(), v.end(), *it), v.end());
                        continue;
                    }
                    it->second = amount;
                }
                else {
                    v.push_back(std::make_pair(price, amount));
                }
            }
        }
    }
}

void updateList(std::ifstream &ifs, std::list <std::pair<double, int>> &l){
    while(ifs){
        int amount = -1;
        double price = -1;
        ifs >> price >> amount;
        if(ifs.good()){
            if ( price > 0 && amount >= 0){
               auto it = l.begin(); 
                for(; it != l.end(); it++){
                    if(it->first==price) break;
                }
                if( it != l.end()){
                    if(amount == 0){
                        l.erase(it);
                        continue;
                    }
                    it->second = amount;
                }
                else {
                    l.push_back(std::make_pair(price, amount));
                }
            }
        }
    }
}

void BM_MapUpdate(benchmark::State& state) {
    std::map <double, int> map;
    for(auto _ : state) {
        std::ifstream ifs("test");
        updateMap(ifs, map);
        ifs.close();
    }
}
BENCHMARK(BM_MapUpdate)->Unit(benchmark::kMillisecond);


void BM_VectorUpdate(benchmark::State& state) {
    std::vector <std::pair <double, int>> vec;
    for(auto _ : state){
        std::ifstream ifs("test");
        updateVector(ifs, vec);
        ifs.close();
    }
}
BENCHMARK(BM_VectorUpdate)->Unit(benchmark::kMillisecond);

void BM_ListUpdate(benchmark::State& state) {
    std::list <std::pair <double, int>> l;
    for(auto _ : state){
        std::ifstream ifs("test");
        updateList(ifs, l);
        ifs.close();
    }
}
BENCHMARK(BM_ListUpdate)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
