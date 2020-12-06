// built-in
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <filesystem>
#include <utility>
#include <unordered_set>
#include <assert.h>

using std::string;
void panic(const char * const message)
{
    std::cout << message << std::endl;
    exit(1);
}


bool validate_pass(const string& passk, const string& passv)
{
    if (passk == "byr")
    {
        try
        {
            int num = std::stoi(passv);
            return num >= 1920 && num <= 2002;
        }
        catch(std::invalid_argument&)
        {
            puts("erro stoi");
            return false;
        }
    }
    else if (passk == "iyr")
    {
        try
        {
            int num = std::stoi(passv);
            return num >= 2010 && num <= 2020;
        }
        catch(std::invalid_argument&)
        {
            puts("erro stoi");
            return false;
        }
    }
    else if (passk == "eyr")
    {
        try
        {
            int num = std::stoi(passv);
            return num >= 2020 && num <= 2030;
        }
        catch(std::invalid_argument&)
        {
            puts("erro stoi");
            return false;
        }
    } 
    else if (passk == "hgt")
    {
        int num;

        char* ptr_back = (char*)passv.data() + passv.size();
        if (!((*(ptr_back-1) == 'n' || *(ptr_back-1) == 'm') && (*(ptr_back-2) == 'i' || *(ptr_back-2) == 'c')))
        {
            return false;
        }

        try
        {
            string tmpstr {passv.data(), passv.data() + passv.size() - 2};
            num = std::stoi(tmpstr);
        }
        catch(std::invalid_argument&)
        {
            puts("erro stoi");
            return false;
        }
        
        if (passv.back() == 'm')
        {
            if (num < 150 || num > 193)
            {
                return false;
            }
        }
        else
        {
            if (num < 59 || num > 76)
            {
                return false;
            }            
        }
        return true; 
    }
    else if (passk == "hcl")
    {
        char* ptr = const_cast<char*>(passv.data());

        if (*ptr != '#')
            return false;
        
        // while (*ptr++ != 0)
        // {
        //     if (!((*ptr >= 48 && *ptr <= 57) || (*ptr >= 97|| *ptr <= 102)))
        //     {
        //         return false;
        //     }
        // }

        for (auto ptr = passv.begin()+1; ptr != passv.end(); ptr++)
        {
            if (!((*ptr >= 48 && *ptr <= 57) || (*ptr >= 97 && *ptr <= 102)))
            {
                return false;
            }
        }
        return passv.size() == 7;   
    }
    else if (passk == "ecl")
    {
        static const std::unordered_set<string> set {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

        if (set.find(passv) == set.end())
        {
            return false;
        }
        return true;
    }
    else if (passk == "pid")
    {
        uint len = 0;

        for (char ptr: passv)
        {
            if (!(ptr >= 48 && ptr <= 57))
            {
                return false;
            }
            len++;
        }
        return len == 9;
    }
    panic("error");
}


int main()
{
    assert(validate_pass("byr", "2002") == true);
    assert(validate_pass("byr", "2003") == false);
    assert(validate_pass("hgt", "60in") == true);
    assert(validate_pass("hgt", "190cm") == true);
    assert(validate_pass("hgt", "190in") == false);
    assert(validate_pass("hgt", "190") == false);
    assert(validate_pass("hcl", "#123abc") == true);
    assert(validate_pass("hcl", "#123abz") == false);
    assert(validate_pass("hcl", "123abc") == false);
    assert(validate_pass("ecl", "brn") == true);
    assert(validate_pass("ecl", "wat") == false);
    assert(validate_pass("pid", "000000001") == true);
    assert(validate_pass("pid", "0123456789") == false);

    while (true)
    {
        string k;
        string v;
        std::cin >> k >> v;
        std::cout << (validate_pass(k, v)? "sim" : "não") << std::endl;
    }
}