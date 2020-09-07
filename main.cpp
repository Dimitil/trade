#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <exception>

void formatStr(std::string &strInput){
    const std::string& delims = "[]\",:";
    for( auto t_iter = strInput.begin(); t_iter != strInput.end(); ++t_iter ){
        if( delims.find(*t_iter) != std::string::npos ){
            *t_iter = ' ';
        }
    }
}


bool tryGetData( std::istringstream &ist, int &amount, double &price ){
    ist >> price >> amount;

    if(ist.good()){ 
        return true;
    }
    ist.clear();
    return false;
}

class Snapshot{
    long int m_curTime;
    std::map <double, int> m_asks;
    std::map <double, int> m_bids;

public:

    void print() const{
        int num=1;
        std::cout << '\n' << m_curTime << "\n\n";
        for(auto it = m_asks.cbegin(); it != m_asks.cend(); it++)
        {
            std::cout << num++ << ')';
            std::cout << it->first << '\t' << it->second;
            std::cout << '\n';
        }
        num = 1;
        std::cout << '\n';
        for(auto it = m_bids.cbegin(); it != m_bids.cend(); it++)
        {
            std::cout << num++<<')';
            std::cout << it->first << '\t' << it->second;
            std::cout << '\n';
        }   
    }

   
    void updateMap( std::istringstream &ist, std::map <double, int> &map)
    {
        int amount = -1;
        double price = -1;
        while( tryGetData(ist, amount, price) ){
            if( 0 == amount ){
                auto it = map.find(price);
                if( it != map.end() ){
                    map.erase(it);
                    continue;
                }
            }
            if( price>0 && amount > 0 ){
            map[price] = amount;
            amount = -1;
            price = -1;
            }
        }
    }

    void updateSnapshot(std::string &strInput){
        formatStr(strInput);
        std::istringstream ist(strInput);
        std::string buf("");
        while( ist >> buf ) {
            if( buf == "time"){
                ist >> m_curTime;
            }

            if( buf == "asks" ){
                updateMap(ist, m_asks);
            }

            if(buf == "bids" ){
                updateMap(ist, m_bids);
            }
        }
    }

   friend std::ostream& operator << (std::ostream &out, const Snapshot &s){
        out << '{' << s.m_curTime << "}, {" << s.m_bids.crbegin()->first << "}, {"
            << s.m_bids.crbegin()->second << "}, {" << s.m_asks.cbegin()->first
            <<
            "}, {" << s.m_asks.cbegin()->second << "}\n";
        return out;
   } 
    
};


int main(int argc, char* argv[])
{
    if(argc<3){
        std::cerr<<"few arguments";
    exit(1);
    }
///home/dmitry/cpp/birja/huobi_dm_depth.log
    std::ifstream inf(argv[1]);
    std::ofstream ofs(argv[2]);
    ofs.precision(7);
    if (!inf)
    {
        std::cerr<< argv[1] << " could not be opened for reading!" << std::endl;
        exit(2);
    }

    Snapshot S{};

    std::string strInput("");
    while(std::getline(inf, strInput)){
        S.updateSnapshot(strInput);
        ofs << S;
    }
    inf.close();
    ofs.close();
    
    return 0;
}

