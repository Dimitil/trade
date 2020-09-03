#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // для использования функции exit()
#include <sstream>
#include <map>

struct Snapshot{
    long int m_curTime;
    std::map <double, int> m_asks;
    std::map <double, int> m_bids;

    void print() const{
        std::cout<<'\n'<<m_curTime<<"\n\n";
        for(auto it=m_asks.begin(); it!=m_asks.end(); it++)
        {
            std::cout<<it->first<<'\t'<<it->second;
            std::cout<<'\n';
        }
        std::cout<<'\n';
        for(auto it=m_bids.begin(); it!=m_bids.end(); it++)
        {
            std::cout<<it->first<<'\t'<<it->second;
            std::cout<<'\n';
        }   
    }

    void init(std::string &strInput) {
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
                for(int i=0; i<20; i++){
                ist>>price;
                ist>>amount;
                m_asks[price] = amount;
                }
            }

            if(buf=="bids"){
                for(int i=0; i<20; i++){
                    ist>>price;
                    ist>>amount;
                    m_bids[price]= amount;
                }
            }
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

    void update(std::string strInput){
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
                while(haveData(ist)){
                    ist>>price;
                    ist>>amount;
                        m_asks[price]=amount;
                        if(0==amount){
                            auto it=m_asks.find(price);
                            m_asks.erase(it);
                        }
                    }
                }

            if(buf=="bids"){
               while(haveData(ist)){
                    ist>>price;
                    ist>>amount;
                    m_bids[price]=amount;
                    if(0==amount){
                            auto it=m_bids.find(price);
                            m_bids.erase(it);
                    }
               }
            }
        }
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
    std::getline(inf, strInput);
    formatStr(strInput);
    S.init(strInput);
    S.print();

    for(int i=0; i<524; i++){
        std::getline(inf, strInput);
        formatStr(strInput);
        S.update(strInput);
        S.print();
    }
    


    return 0;
}
