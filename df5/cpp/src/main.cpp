// built-in
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cstring>
#include <assert.h>
#include <tuple>
#include <algorithm>

void panic(const char * const message)
{
    std::cout << message << std::endl;
    exit(1);
}

[[nodiscard]]
std::vector<char*> read_input(const char* const file_path)
{
    std::ifstream file;
    std::vector<char*> output;
    std::string buffer;

    if (!std::filesystem::exists({file_path}) || std::filesystem::is_directory({file_path}))
    {
        panic("arquivo inválido ou não existente");
    }

    file.open(file_path);

    if (file.bad())
    {
        panic("erro ao abrir arquivo");
    }

    constexpr const uint line_size = 10;
    const uint line_count = std::filesystem::file_size({file_path}) / line_size;

    for (uint i = 0; i < line_count; i++)
    {
        output.push_back(new char[11]);
        output[i][10] = 0;
    }
    buffer.reserve(line_size+1);

    uint line = 0;
    while (!std::getline(file, buffer).eof())
    {
        assert(line <= output.size());
        strcpy(output[line], buffer.data());

        buffer.clear();
        buffer.reserve(line_size+1);
        line++;
    }

    return output;
}

uint calc1(const char* const str)
{
    uint row, collum;

    uint start, end;
    char ch;

    start = 0;
    end = 127;
    for (uint i = 0; i < 6; i++)
    {
        ch = *(str+i);

        if (ch == 'F')
        {
            end = ((end + start) >> 1) - 1;
        }
        if (ch == 'B')
        {
            start = ((end + start) >> 1) + 1;
        }
    }
    if (*(str+6) == 'B')
        start++;
    row = start;

    start = 0;
    end = 7;
    for (uint i = 7; i < 9; i++)
    {
        ch = *(str+i);

        if (ch == 'L')
        {
            end = ((end + start) >> 1) - 1;
        }
        if (ch == 'R')
        {
            start = ((end + start) >> 1) + 1;
        }
    }
    if (*(str+9) == 'R')
    {
        start++;
    }
    collum = start;

    // std::cout << "row: " << row << std::endl;
    // std::cout << "collum: " << collum << std::endl;
    // std::cout << "resultado " << str << ": " << (row * 8) + collum << std::endl;
    return (row * 8) + collum;
}

void parte1(const char* const input_path)
{
    std::vector<char*> data = read_input(input_path);

    uint max = 0;

    for (auto& str: data)
    {
        uint res = calc1(str);
        if (res > max)
            max = res;
    }

    std::cout << "resultado: " << max << std::endl;

    for (char*& str: data)
        delete[] str;
}

void parte2(const char* const input_path)
{
    std::vector<char*> data = read_input(input_path);
    std::vector<uint> ids {};

    for (auto& str: data)
    {
        uint res = calc1(str);
        ids.push_back(res);
    }

    std::sort(ids.begin(), ids.end());

    for (uint i= 0; i < ids.size(); i++)
    {
        uint a = ids[i];
        uint b = ids[i + 1];

        if (b - a == 2)
        {
            std::cout << "resultado: " << a+1 << std::endl;
            break;
        }
    }

    for (char*& str: data)
        delete[] str;
}

int main()
{
    parte2("input.txt");
    // calc1("FBFBBFFRLR");
    // calc1("BFFFBBFRRR");
    // calc1("FFFBBBFRRR");
    // calc1("BBFFBBFRLL");
}
