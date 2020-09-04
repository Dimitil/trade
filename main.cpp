#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

struct Snapshot{
    long int m_curTime;
    std::map <double, int> m_asks;
    std::map <double, int> m_bids;

    void print() const{
        int num=1;
        std::cout<<'\n'<<m_curTime<<"\n\n";
        for(auto it=m_asks.begin(); it!=m_asks.end(); it++)
        {
            std::cout<<num++<<')';
            std::cout<<it->first<<'\t'<<it->second;
            std::cout<<'\n';
        }
        num=1;
        std::cout<<'\n';
        for(auto it=m_bids.begin(); it!=m_bids.end(); it++)
        {
            
            std::cout<<num++<<')';
            std::cout<<it->first<<'\t'<<it->second;
            std::cout<<'\n';
        }   
    }

    bool haveData(std::istringstream &ist){
        char ch=' ';
        ist.get(ch);
        while(isspace(ch)){
            ist.get(ch);
        }
        ist.putback(ch);
        return isdigit(ch);
    }

    void updateMap(std::istringstream &ist, std::map <double, int> &map)
    {
        int amount=-1;
        double price = -1;
         while(haveData(ist)){
             ist>>price;
             ist>>amount;
             map[price]=amount;
             if(0==amount){
                 auto it=map.find(price);
                 map.erase(it);
             }
        }

    }

    void update(const std::string strInput){
        std::istringstream ist(strInput);
        std::string buf("");
        double price=-1;
        int amount=-1;
        while(ist) {
            ist>>buf;
            if(buf=="time"){
                ist>>m_curTime;
            }

            if(buf=="asks"){
                updateMap(ist, m_asks);
            }

            if(buf=="bids"){
                updateMap(ist, m_bids);
            }
        }
    }
    
    void printTrue() const{

        std::cout<<'{'<<m_curTime<<"}, {"<<m_bids.rbegin()->first<<"}, {"<<m_bids.rbegin()->second<<
            "}, {"<<m_asks.begin()->first<<"}, {"<<m_asks.begin()->second<<"}\n";
    }

         
    
};

void formatStr(std::string &strInput){
    const std::string& delims = "[]\",:";
    for(auto t_iter = strInput.begin(); t_iter != strInput.end(); ++t_iter){
        if(delims.find(*t_iter) != std::string::npos){
            *t_iter = ' ';
        }
    }
}

int main()
{
    std::cout.precision(7);
    std::ifstream inf("/home/dmitry/cpp/birja/huobi_dm_depth.log");
    if (!inf)
    {
        std::cerr << "could not be opened for reading!" << std::endl;
        exit(1);
    }

    Snapshot S{};

    std::string strInput;

    while(inf){
        std::getline(inf, strInput);
        formatStr(strInput);
        S.update(strInput);
        S.printTrue();
    }
    


    return 0;
}
