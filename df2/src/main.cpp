// built-in
#include <thread>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <mutex>
#include <assert.h>
#include <cstring>

// extern
#include "../lib/CLI11.hpp"        // ---> https://github.com/CLIUtils/CLI11

// local
#include "job.hpp"

using std::endl;

inline void panic(const char* const message)
{
    std::cout << message << endl;
    exit(1);
}

[[nodiscard]]
std::vector<Job> read_input(const char* const file_path)
{
    std::vector<Job> out;
    std::ifstream file;
    std::string buffer;
    buffer.reserve(2048);
    out.reserve(1000);

    if (!std::filesystem::exists({file_path}) || std::filesystem::is_directory({file_path}))
    {
        panic("arquivo inválido ou não existente");
    }

    file.open(file_path, std::ios::in);


    std::string min;
    std::string max;
    char ch;
    std::string password;

    while (!std::getline(file, buffer).eof())
    {
        uint min_end = buffer.find('-');
        min = buffer.substr(0, min_end);

        uint max_end = buffer.find(' ', min_end+1);
        max = buffer.substr(min_end+1, max_end - (min_end + 1));

        ch = buffer[max_end+1];

        uint pswd_start = buffer.find(' ', max_end+1);
        password = buffer.substr(pswd_start);
    
        out.push_back({(uint8_t)std::stoi(min), (uint8_t)std::stoi(max), ch, password.data()});
    }

    file.close();
    return out;
}

static std::mutex mut;
static uint total = 0;

void process1(const std::vector<Job>* jobs)
{
    uint int_total = 0;

    for (auto& job: *jobs)
    {
        size_t str_len = strlen(job.password); 
        if (str_len < job.min)
            return;

        char* ptr = job.password;
        uint count = 0;

        while (*ptr)
        {
            if (*ptr == job.ch)
                count++;
            ptr++;
        }

        if (count >= job.min && count <= job.max)
            int_total++;
    }

    mut.lock();
    total += int_total;
    mut.unlock();
    
    delete jobs;
    return;
}

void process2(const std::vector<Job>* jobs)
{
    uint int_total = 0;

    for (auto& job: *jobs)
    {
        if ((*(job.password + job.min) == job.ch) != (*(job.password + job.max) == job.ch))
            int_total++;
    }

    mut.lock();
    total += int_total;
    mut.unlock();
    
    delete jobs;
    return;
}


void parte(const char* const file_path, void (process(const std::vector<Job>*)))
{
    std::vector<Job> data = read_input(file_path);

    uint thread_number = std::thread::hardware_concurrency();
    uint* count = new uint[thread_number];

    for (uint i = 0; i < thread_number; i++)
        count[i] = ((uint)data.size() / thread_number);

    
    if (std::accumulate(count, count + thread_number, 0) != (int)data.size())
        count[0]++;

    if (!(std::accumulate(count, count + thread_number, 0) == (int)data.size()))
    {
        panic("erro ao dividir jobs entre o número de threads");
    }

    for (uint i = 1; i < thread_number; i++)
    {
        count[i] = count[i] + count[i-1];
    }

    std::thread** threads = new std::thread*[thread_number];

    const std::vector<Job>* tdata = new std::vector<Job>{data.begin(), data.begin() + count[0]}; 
    threads[0] = new std::thread{process, tdata};
    for (uint i = 1; i < thread_number; i++)
    {
        tdata = new std::vector<Job>{data.begin() + count[i-1], data.begin() + count[i]};
        threads[i] = new std::thread{process, tdata};
    }
    
    for (uint i = 0; i < thread_number; i++)
    {
        (*threads[i]).join();
        delete threads[i];
    }

    std::cout << "resultado: " << total << endl;
}

int main(int argc, char** argv)
{
    CLI::App app {"adventofcode 2"};
    app.require_subcommand();

    std::string path {"input.txt"};

    CLI::App* sub1 = app.add_subcommand("part1", "verificação de quantidade");
    sub1->add_option("input", path, "input path");
    sub1->callback([path](){
        parte(path.data(), process1);
    });

    CLI::App* sub2 = app.add_subcommand("part2", "verificação de posição");
    sub2->add_option("input", path, "input path");
    sub2->callback([path](){
        parte(path.data(), process2);
    });

    CLI11_PARSE(app, argc, argv);
}