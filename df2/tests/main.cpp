#ifdef PRIOR_MEMORY
#undef PRIOR_MEMORY
#endif

// built-in
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cstring>
#include <tuple>

// local
#include "../src/job.hpp"

// types
typedef std::tuple<uint8_t, uint8_t, char, char*> RawJob;

inline RawJob createrj(uint8_t a, uint8_t b, char c, char* d)
{
    return std::make_tuple(a, b, c, d);
}
inline uint8_t get_min(const RawJob& tp)
{
    return std::get<0>(tp);
}
inline uint8_t get_max(const RawJob& tp)
{
    return std::get<1>(tp);
}
inline char get_ch(const RawJob& tp)
{
    return std::get<2>(tp);
}
inline char* get_pswd(const RawJob& tp)
{
    return std::get<3>(tp);
}

Job get_copy(Job& j)
{
    return Job{j};
}


[[nodiscard]]
std::vector<RawJob> gen_data(uint16_t count,
                             uint16_t min_min, uint16_t min_max,
                             uint16_t max_min, uint16_t max_max,
                             uint16_t str_min, uint16_t str_max)
{
    static std::random_device random_device;
    static std::mt19937 random_engine(random_device());
    static std::uniform_int_distribution<char> char_gen {32, 126};

    std::uniform_int_distribution<int> dist_min {min_min, min_max};
    std::uniform_int_distribution<int> dist_max {max_min, max_max};
    std::uniform_int_distribution<int> dist_str {str_min, str_max};

    static const auto gen_str = [](uint size)
    {
        char* str = new char[size+1];
        str[size] = 0;
        for (uint i = 0; i < size; i++)
        {
            str[i] = char_gen(random_engine);
        }
        return str;
    };

    std::vector<RawJob> output;
    output.reserve(count);

    for (uint i = 0; i < count; i++)
    {
        output.push_back(
            createrj(
                dist_min(random_engine), 
                dist_max(random_engine),
                char_gen(random_engine),
                gen_str(dist_str(random_engine))
            )
        );
    }

    return output;
}


[[nodiscard]]
bool default_constructor_test(const std::vector<RawJob>& data)
{
    for (auto& rj: data)
    {
        Job tmp {get_min(rj), get_max(rj), get_ch(rj), get_pswd(rj)};

        // verifica se os valores são iguais aos usados na construção
        if ((tmp.min != get_min(rj))                     ||
            (tmp.max != get_max(rj))                     || 
            (tmp.ch != get_ch(rj))                       || 
            (strcmp(tmp.password, get_pswd(rj)) != 0)    ||
            (strlen(tmp.password) != strlen(get_pswd(rj)))) 
        {
            return false;
        }
    }
    return true;
}

[[nodiscard]]
bool lvalue_constructor_test(const std::vector<RawJob>& data)
{
    for (auto& rj: data)
    {
        Job tmp1 {get_min(rj), get_max(rj), get_ch(rj), get_pswd(rj)};
        Job tmp2 {tmp1};

        // verifica se todos os valores são identicos
        if ((tmp2.min != tmp1.min)                         ||
            (tmp2.max != tmp1.max)                         || 
            (tmp2.ch != tmp1.ch)                           || 
            (strcmp(tmp1.password, tmp2.password) != 0)    ||
            (strlen(tmp1.password) != strlen(tmp2.password))) 
        {
            return false;
        }

        // verifica se os dois ponteiros apontam pra lugares diferentes
        if (tmp1.password == tmp2.password)
        {
            return false;
        }

    }
    return true;
}

[[nodiscard]]
bool lvalue_assignment_test()
{
    // esse teste não recebe dados externos visto que
    // precisa de dados especificos para testar o sistema
    // de cópia com e sem o "PRIOR_MEMORY"

    std::vector<RawJob> data1 = gen_data(200, 1, 5, 6, 10, 15, 25);
    std::vector<RawJob> data2 = gen_data(200, 1, 5, 6, 10, 5, 14);

    for (uint i = 0; i < 200; i++)
    {
        Job tmp1 {get_min(data1[i]), get_max(data1[i]), get_ch(data1[i]), get_pswd(data1[i])};
        Job tmp2 {get_min(data2[i]), get_max(data2[i]), get_ch(data2[i]), get_pswd(data2[i])};

        char* origptr = tmp1.password;
        tmp1 = tmp2;

        // verifica se todos os valores são identicos
        if ((tmp2.min != tmp1.min)                    ||
            (tmp2.max != tmp1.max)                    || 
            (tmp2.ch != tmp1.ch)                      || 
            (strcmp(tmp1.password, tmp2.password) != 0))
        {
            return false;
        }

        // confirma se o ponteiro original se mantém
        if (tmp1.password != origptr)
        {
            return false;
        }

        // verifica se os dois ponteiros apontam pra lugares diferentes
        if (tmp1.password == tmp2.password)
        {
            return false;
        }
    }

    for (uint i = 0; i < 200; i++)
    {
        Job tmp1 {get_min(data1[i]), get_max(data1[i]), get_ch(data1[i]), get_pswd(data1[i])};
        Job tmp2 {get_min(data2[i]), get_max(data2[i]), get_ch(data2[i]), get_pswd(data2[i])};

        char* origptr = tmp2.password;
        tmp2 = tmp1;

        // verifica se todos os valores são identicos
        if ((tmp2.min != tmp1.min)                    ||
            (tmp2.max != tmp1.max)                    || 
            (tmp2.ch != tmp1.ch)                      || 
            (strcmp(tmp1.password, tmp2.password) != 0))
        {
            return false;
        }

        // confirma se o ponteiro original foi alterado
        if (tmp1.password == origptr)
        {
            return false;
        }

        // verifica se os dois ponteiros apontam pra lugares diferentes
        if (tmp1.password == tmp2.password)
        {
            return false;
        }
    }

    for (auto& rj: data1)
    {
        delete[] get_pswd(rj);
    }
    for (auto& rj: data2)
    {
        delete[] get_pswd(rj);
    }

    return true;

}

[[nodiscard]]
bool rvalue_constructor_test(const std::vector<RawJob>& data)
{
    std::vector<RawJob> data2 = gen_data(data.size(), 1, 5, 6, 10, 5, 25);

    for (uint i = 0; i < data.size(); i++)
    {
        Job tmp2 {get_min(data2[i]), get_max(data2[i]), get_ch(data2[i]), get_pswd(data2[i])};
        Job tmp1 {std::move(tmp2)};

        // verifica se os valores são iguais aos usados na construção
        if ((tmp1.min != get_min(data2[i]))                     ||
            (tmp1.max != get_max(data2[i]))                     || 
            (tmp1.ch != get_ch(data2[i]))                       || 
            (strcmp(tmp1.password, get_pswd(data2[i])) != 0)    ||
            (strlen(tmp1.password) != strlen(get_pswd(data2[i])))) 
        {
            return false;
        }
    }
    for (auto& rj: data2)
    {
        delete[] get_pswd(rj);
    }
    return true;
}

[[nodiscard]]
bool rvalue_assignment_test(const std::vector<RawJob>& data)
{
    std::vector<RawJob> data2 = gen_data(data.size(), 1, 5, 6, 10, 5, 25);

    for (uint i = 0; i < data.size(); i++)
    {
        Job tmp2 {get_min(data2[i]), get_max(data2[i]), get_ch(data2[i]), get_pswd(data2[i])};
        Job tmp1 {get_min(data[i]), get_max(data[i]), get_ch(data[i]), get_pswd(data[i])};

        tmp1 = std::move(tmp2);

        // verifica se os valores são iguais aos usados na construção
        if ((tmp1.min != get_min(data2[i]))                     ||
            (tmp1.max != get_max(data2[i]))                     || 
            (tmp1.ch != get_ch(data2[i]))                       || 
            (strcmp(tmp1.password, get_pswd(data2[i])) != 0)    ||
            (strlen(tmp1.password) != strlen(get_pswd(data2[i])))) 
        {
            return false;
        }
    }
    for (auto& rj: data2)
    {
        delete[] get_pswd(rj);
    }
    return true;
}


int main()
{

    std::vector<RawJob> data = gen_data(200, 1, 5, 6, 10, 5, 25);

    if (default_constructor_test(data) == false)
    {
        std::cout << "erro default_construct_test" << std::endl;
    }
    if (lvalue_constructor_test(data) == false)
    {
        std::cout << "erro lvalue_construct_test" << std::endl;
    }
    if (lvalue_assignment_test() == false)
    {
        std::cout << "erro lvalue_assignment_test" << std::endl;
    }
    if (rvalue_constructor_test(data) == false)
    {
        std::cout << "erro rvalue_construct_test" << std::endl;
    }
    if (rvalue_assignment_test(data) == false)
    {
        std::cout << "erro rvalue_assignment_test" << std::endl;
    }

    for (auto& rj: data)
    {
        delete[] get_pswd(rj);
    }
}