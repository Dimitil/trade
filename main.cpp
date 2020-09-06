#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <exception>
//#include <regex>

void formatStr(std::string &strInput){
    const std::string& delims = "[]\",:";
    for( auto t_iter = strInput.begin(); t_iter != strInput.end(); ++t_iter ){
        if( delims.find(*t_iter) != std::string::npos ){
            *t_iter = ' ';
        }
    }
}

//void formatStr(std::string &strInput){
//    std::regex reg("\\[|\\]|\"|,|:");
//    strInput = std::regex_replace(strInput, reg, " ");
//}


//bool tryGetData( std::istringstream &ist, int &amount, double &price ){
//    char ch = ' ';
//    ist.get(ch);
//    while( isspace(ch) ){
//       ist.get(ch);
//    }
//    ist.putback(ch);
//    if (isdigit(ch)){
//        ist >> price >> amount;
//        return true;
//    }
//    return false;
//}
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

    std::string strInput("");
    while(std::getline(inf, strInput)){
        S.updateSnapshot(strInput);
        std::cout << S;
    }
    inf.close();
    
    return 0;
}
