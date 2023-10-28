#include "records.h"
#include "console.h"

records::records(console& con)
{
    std::ifstream savein("save.txt");
    std::ofstream saveout;

    if (!savein.is_open())
    {
        con << "Can't find save file. Creating new.";
        con.sleep(750);
        con << '.';
        con.sleep(750);
        con << ".\n";

        saveout.open("save.txt");
        saveout << "\n\n\n";
        saveout.close();
        savein.open("save.txt");
    }

    getline(savein, infoE);
    getline(savein, infoN);
    getline(savein, infoH);
    getline(savein, infoC);

    savein.close();
}

void fill_table(std::string&, std::multiset<std::pair<int, std::string>>&);
void enter_to_records(console&, std::multiset<std::pair<int, std::string>>&, int);
void change(std::multiset<std::pair<int, std::string>>&, std::string&);

void work(console& con, std::string& info, int time)
{
    std::multiset <std::pair <int, std::string> > table_of_records;
    std::multiset <std::pair <int, std::string> >::reverse_iterator it;
    
    fill_table(info, table_of_records);
    it = table_of_records.rbegin();
    if (table_of_records.size() < 10 || time < it->first)
        enter_to_records(con, table_of_records, time);

    change(table_of_records, info);
}
void records::put(console& con, char difficult, int time)
{
    switch (difficult)
    {
    case 'E':
        work(con, infoE, time);
        break;
    case 'N':
        work(con, infoN, time);
        break;
    case 'H':
        work(con, infoH, time);
        break;
    case 'C':
        work(con, infoC, time);
        break;
    }

    
}
void fill_table(std::string& info, std::multiset<std::pair<int, std::string>>& table_of_records)
{
    std::string word = "", name = "";
    int bTime = 0, info_size = info.size();

    for (int i = 0; i < info_size; i++)
    {
        if (info[i] == ' ')
        {
            for (int j = 0; j < word.size() - 4; j++)
                name += word[j];
            
            for (int j = word.size() - 4; j < word.size(); j++)
            {
                bTime *= 10;
                bTime += word[j] - '0';
            }

            table_of_records.insert({ bTime, name });
            word = "";
            name = "";
            bTime = 0;
            continue;
        }
        word += info[i];
    }
}

void enter_to_records(console& con, std::multiset<std::pair<int, std::string>> &table_of_records, int Time)
{
    std::string name;
    con << "Congrats! You reached new record. Enter your name type XXXX:\n";
    con >> name;

    while (name.size() != 4)
    {
        con << "Error. Please, try again: ";
        con >> name;
    }

    int bTime = Time > 5999 ? 5999 : Time;
    table_of_records.insert({bTime, name});
}
void change(std::multiset<std::pair<int, std::string>>& table_of_records, std::string& info)
{
    info = "";
    auto it = table_of_records.begin();
    for (int i = 0; i < 10 && it != table_of_records.end(); i++, it++)
    {
        std::string name = it->second;
        char* b_numb = new char[1024];
        int x = it->first > 5999 ? 5999 : it->first;
        _itoa_s(x, b_numb, 32, 10);
        std::string numb = b_numb;
        while (numb.size() < 4)
            numb = '0' + numb;
        info += name + numb + ' ';
    }
}

console& operator<<(console& con, const records& rec)
{
    std::string info;

    con << "Enter difficult: \nEasy - E, Normal - N, Hard - H, Custom - C.\n";
    char symb;
    do
    {
        symb = con.getch();
        
        auto rechange = [](char& symb)
        {
            if (symb >= 'a' && symb <= 'z') symb += 'A' - 'a';
            return symb;
        };

        rechange(symb);
    } while (symb != 'E' && symb != 'N' && symb != 'H' && symb != 'C');

    switch (symb)
    {
    case 'E':
        info = rec.infoE;
        break;
    case 'N':
        info = rec.infoN;
        break;
    case 'H':
        info = rec.infoH;
        break;
    case 'C':
        info = rec.infoC;
        break;
    }

    std::multiset<std::pair<int, std::string>> table_of_records;
    fill_table(info, table_of_records);

    if (table_of_records.empty()) 
        con << "Cann't find records.\n";
    else
    {
        con << "Name Time\n";
        for (auto& i : table_of_records)
        {
            int bTime = i.first;
            con << i.second << ' ';

            if (bTime / 60 < 10)
                con << 0;
            con << bTime / 60 << ':';

            int x = bTime - (bTime / 60) * 60;
            if (x % 10 == x) 
                con << 0;
            con << x << '\n';
        }
    }
    con.getch();
    con.clear();

    return con;
}
