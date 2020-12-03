// built-in
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <utility>

// extern
#include "lib/CLI11.hpp"        // ---> https://github.com/CLIUtils/CLI11

using std::endl;

inline void panic(const char* const message)
{
    std::cout << message << endl;
    exit(1);
}

[[nodiscard]]
inline std::vector<uint16_t> read_input(const char* const file_path) noexcept
{
    std::vector<uint16_t> out;
    std::ifstream file;
    std::string buffer;
    buffer.reserve(2048);
    out.reserve(200);

    if (!std::filesystem::exists({file_path}) || std::filesystem::is_directory({file_path}))
    {
        panic("arquivo inválido ou não existente");
    }

    file.open(file_path, std::ios::in);

    while (!std::getline(file, buffer).eof())
    {
        out.push_back(std::stoi(buffer));
    }

    file.close();
    return out;
}

int parte1(const std::string& path_file)
{
    const std::vector<uint16_t> data = read_input(path_file.data());
    const uint16_t data_size = data.size();

    if (data_size < 2)
    {
        panic("dados insuficientes");
    }

    constexpr const uint16_t half = 2020 / 2;
    
    std::vector<uint16_t> bth;
    std::vector<uint16_t> sth;
    bool eq;

    bth.reserve(data_size);
    sth.reserve(data_size);

    for (auto& i: data)
    {
        if (i > half)
        {
            bth.push_back(i);
        }
        else if (i < half)
        {
            sth.push_back(i);
        }
        else if (i == half)
        {
            if (eq)
            {
                std::cout << "resultado: " << (i << 1) << endl;
                return 0;
            }
            else
            {
                eq = true;
            }
        }
    }

    std::vector<uint16_t>* l;

    for (auto& i: data)
    {
        if (i > half)
        {
            l = &sth;
        }
        else if (i < half)
        {
            l = &bth;
        }

        for (auto& ot: *l)
        {
            if (i + ot == 2020)
            {
                std::cout << "resultado: " << (i * ot) << endl;
                return 0;
            }
        }    
    }

    return 1;
}

int parte2(const std::string& path_file)
{
    const std::vector<uint16_t> data = read_input(path_file.data());
    const uint16_t data_size = data.size();

    if (data_size < 2)
    {
        panic("dados insuficientes");
    }

    constexpr const uint16_t half = 2020 / 2;
    
    std::vector<uint16_t> bth;
    std::vector<uint16_t> sth;

    bth.reserve(data_size);
    sth.reserve(data_size);

    static const auto get_sum =
    [](const std::vector<uint16_t>& vec, uint16_t x, uint16_t sum_res) -> uint32_t
    {
        uint vec_size = vec.size();
        for (uint i = 0; i < vec_size; i++)
            for (uint j = i + 1; i < vec_size; i++)
            {
                if (vec[i] + vec[j] == sum_res)
                {
                    return x * vec[i] * vec[j];
                }
            }
        return 0;
    };
    


    for (auto& i: data)
    {
        if (i >= half)
        {
            bth.push_back(i);
        }
        else if (i < half)
        {
            sth.push_back(i);
        }
    }

    int ret;
    for (auto& i: data)
    {
        if (i >= half)
        {
            if ((ret = get_sum(sth, i, 2020-i)))
            {
                std::cout << "resultado: " << ret << endl;
                return 0;
            }
        }
        else if (i < half)
        {
            if ((ret = get_sum(data, i, 2020-i)))
            {
                std::cout << "resultado: " << ret << endl;
                return 0;
            }
        }
    }

    return 1;
}

int main(int argc, char** argv)
{

    CLI::App app {"adventofcode 1"};
    app.require_subcommand();

    std::string path {"input.txt"};

    CLI::App* sub1 = app.add_subcommand("double", "busca dupla");
    sub1->add_option("input", path, "input path");
    sub1->callback([path](){
        parte1(path);
    });

    CLI::App* sub2 = app.add_subcommand("triple", "busca tripla");
    sub2->add_option("input", path, "input path");
    sub2->callback([path](){
        parte2(path);
    });

    CLI11_PARSE(app, argc, argv);
}
