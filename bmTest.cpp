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
           // for(auto it = map.begin(); it != map.end(); ++it){
           //     std::cout<<it->first<<' '<<it->second<<'\n';
           // }
           // std::cout<<'\n';
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
    //for(auto i=v.begin(); i!=v.end(); i++){
    //    std::cout << i->first << ' ' << i->second << '\n';
    //}
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
   
   //for(auto i=l.begin(); i!=l.end(); i++){
   //     std::cout << i->first << ' ' << i->second << '\n';
   // }
}

static void BM_MapUpdate(benchmark::State& state) {
    std::map <double, int> map;
    std::ifstream ifs("test");
    for(auto _ : state){
    updateMap(ifs, map);
    //map.clear();
    }
}
BENCHMARK(BM_MapUpdate);


static void BM_VectorUpdate(benchmark::State& state) {
    std::vector <std::pair <double, int>> vec;
    std::ifstream ifs("test");
    for(auto _ : state){
    updateVector(ifs, vec);
    //vec.clear();
    }
}
BENCHMARK(BM_VectorUpdate);

static void BM_ListUpdate(benchmark::State& state) {
    std::list <std::pair <double, int>> l;
    std::ifstream ifs("test");
    for(auto _ : state){
    updateList(ifs, l);
    //l.clear();
    }
}
BENCHMARK(BM_ListUpdate);


BENCHMARK_MAIN();
