#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // для использования функции exit()
#include <vector>
#include <sstream>

struct Snapshot{
    long int m_curTime;
    std::pair<double, int> m_asks[20];
    std::pair<double, int> m_bids[20];

    void print() const{
        std::cout<<'\n'<<m_curTime<<"\n\n";
        for(int i=0; i<20; i++)
        {
            std::cout<<m_asks[i].first<<'\t'<<m_asks[i].second;
            std::cout<<'\n';
        }
        std::cout<<'\n';
        for(int i=0; i<20; i++)
        {
            std::cout<<m_bids[i].first<<'\t'<<m_bids[i].second;
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
                m_asks[i]=std::make_pair(price, amount);
                }
            }

            if(buf=="bids"){
                for(int i=0; i<20; i++){
                    ist>>price;
                    ist>>amount;
                    m_bids[i]=std::make_pair(price, amount);
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

            if(buf=="asks" && haveData(ist)){
                    ist>>price;
                    ist>>amount;
                    std::cout<<price<<' '<<amount;}
                else {
                    std::cout<<"no asks\n";
                }

            if(buf=="bids" && haveData(ist)){
                    ist>>price;
                    ist>>amount;
                    std::cout<<price<<' '<<amount;}
                else {
                    std::cout<<"no bids\n";
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

    std::getline(inf, strInput);
    formatStr(strInput);
    std::cout<<strInput;
    S.update(strInput);


    return 0;
}
