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

// extern
#include "lib/CLI11.hpp"

using std::string;

static const std::unordered_set<string> karr {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

void panic(const char * const message)
{
    std::cout << message << std::endl;
    exit(1);
}

[[nodiscard]]
std::vector<std::vector<std::pair<string, string>>> read_input(const char* const file_path)
{
    std::ifstream file;
    std::vector<std::vector<std::pair<string, string>>> out;
    
    std::vector<std::pair<string, string>>* vec_buffer;
    std::string buffer;
    std::string pair;

    out.reserve(300);

    if (!std::filesystem::exists({file_path}) || std::filesystem::is_directory({file_path}))
    {
        panic("arquivo inválido ou não existente");
    }

    file.open(file_path);

    if (file.bad())
    {
        panic("erro ao abrir arquivo");
    }

    vec_buffer = new std::vector<std::pair<string, string>>{};
    vec_buffer->reserve(8);
    while (!std::getline(file, buffer).eof())
    {
        if (buffer.size() == 0)
        {
            out.push_back(std::move(*vec_buffer));
            delete vec_buffer;
            vec_buffer = new std::vector<std::pair<string, string>>{};
            vec_buffer->reserve(8);
            continue;
        }

        int start = 0;
        int end;
        size_t fpos;
        std::string k;

        while (true)
        {
            fpos = buffer.find(' ', start);
            end = (fpos != buffer.npos) ? fpos : (buffer.size());
            
            if (start >= end)
                break;

            int len = end - (start + 0);
            pair = buffer.substr(start, len);

            k = {
                pair.data(),
                pair.data() + 3
            };
            
            if (k == "cid")
            {
                start = end + 1;
                continue;
            }
            
            vec_buffer->push_back({
                k, 
                {
                    pair.data() + 4,
                    pair.data() + pair.size()
                }
            });

            start = end + 1;
        }
    }
    
    out.push_back(std::move(*vec_buffer));
    delete vec_buffer;
    file.close();
    return out;
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

        if (passv.front() != '#')
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

void parte(const char* const input_path)
{
    std::vector<std::vector<std::pair<string, string>>> data = read_input(input_path);

    uint total = 0;
    uint total2 = 0;

    for (auto& pass: data)
    {
        if (pass.size() >= 7)
            total++;

        bool all = true;
        for (auto& kv: pass)
        {
            if (!validate_pass(kv.first, kv.second))
            {
                all = false;
            }
        }
        if (all)
            total2++;
    }
    std::cout << "resultado parte 1: " << total << std::endl;
    std::cout << "resultado parte 2: " << total2 << std::endl;
}


int main()
{
    parte("input.txt");
}