#pragma once
#include <fstream>
#include <string>
#include <set>
#include "console.h"

//TODO change records to singletone
//TODO array of info
//TODO support to upgrade app
//TODO change file when change array
class records
{
private:
    std::string infoE, infoN, infoH, infoC; //info about records in difficults easy, normal, hard && custom equivalent
public:
    //constructors
    records() {
        infoE = infoN = infoH = infoC = "";
    }
    records(console&);
    records(const records& rec) = delete;
    records& operator=(const records& rec) = delete;
    records(records&& rec) noexcept = delete;
    records& operator=(records&& rec) noexcept = delete;

    //destructor
    ~records()
    {
        std::ofstream out("save.txt");
        out << infoE << '\n' << infoN << '\n' << infoH << '\n' << infoC;
        out.close();
        infoE = infoN = infoH = infoC = "";
    }

    //functions
    void put(console&, char, int);

    //overloads
    friend console& operator<<(console&, const records&);
    
 };