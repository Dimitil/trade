#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // для использования функции exit()
#include <vector>
#include <sstream>

int main()
{

    std::vector<std::pair<double, int>> asks;
    asks.resize(20);
    std::vector<std::pair<double, int>> bids;
    bids.resize(20);
    std::ifstream inf("/home/dmitry/cpp/birja/huobi_dm_depth.log");

    if (!inf)
    {
        std::cerr << "could not be opened for reading!" << std::endl;
        exit(1);
    }


    const std::string& delims = "[]\",:";
    std::string strInput;
    std::getline(inf, strInput);

    for(auto t_iter = strInput.begin(); t_iter != strInput.end(); ++t_iter){
                               //Если в разделителях найден текущий символ - меняем его на пробел
        if(delims.find(*t_iter) != std::string::npos){
            *t_iter = ' ';
        }
    }
    std::istringstream ist(strInput);
    std::string buf;
    double price=0;
    int amount=0;
    int curTime=0;
    while(ist) {
        ist>>buf;
        //std::cout<<buf<<'\n';
        if(buf=="time"){
            ist>>curTime;
        }

        }
        if(buf=="asks")
        {
            for(int i=0; i<20; i++){
            ist>>price;
            ist>>amount;
            asks[i]=std::make_pair(price, amount);
            }
        }

        if(buf=="bids"){
            for(int i=0; i<20; i++){
            ist>>price;
            ist>>amount;
            bids[i]=std::make_pair(price, amount);
            }
        }



    for(int i=0; i<20; i++)
    {
        std::cout<<asks[i].first<<'\t'<<asks[i].second;
        std::cout<<'\n';
    }

    std::cout<<'\n';

    for(int i=0; i<20; i++)
    {
        std::cout<<bids[i].first<<'\t'<<bids[i].second;
        std::cout<<'\n';
    }

    return 0;
}
