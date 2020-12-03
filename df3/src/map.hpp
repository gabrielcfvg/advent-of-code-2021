#ifndef DF3_MAP
#define DF3_MAP

// built-in
#include <filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <assert.h>

// extern
#include "lib/core.hpp"

// local
#include "panic.hpp"

using namespace boar;

class Map
{
    private:

        std::vector<std::vector<uint8_t>> data;

    public:

        Vector2ui dim {0, 0};

        Map(const char* const file_path)
        {
            std::ifstream file;
            std::vector<uint8_t> buffer;
            buffer.reserve(128);
            this->data.reserve(350);

            if (!std::filesystem::exists({file_path}) || std::filesystem::is_directory({file_path}))
            {
                panic("arquivo inválido ou não existente");
            }

            file.open(file_path, std::ios::in);

            if (file.bad())
            {
                panic("erro ao abrir arquivo");
            }

            char ch;
            while (true)
            {
                ch = file.get();
                if (ch == std::char_traits<char>::eof())
                {
                    break;
                }
                if (ch == '\n')
                {
                    this->data.push_back({buffer});
                    buffer.clear();
                    buffer.reserve(this->data.back().size());
                }
                else
                {
                    buffer.push_back(((ch == '.') ? 0 : 1));
                }      
            }    

            this->dim = Vector2ui{(uint)this->data.back().size(), (uint)this->data.size()};
        }

        void debugfn_print(uint lncnt)
        {
            assert(lncnt >= this->data.size());

            for (uint i = 0; i < lncnt; i++)
            {
                for (auto& ch: this->data[i])
                    std::cout << ch;
                std::cout << std::endl;
            }
        }

        bool at(Vector2ui pos) const
        {
            return this->data.at(pos.y).at(pos.x);
        }

        ~Map()
        {

        };
};


#endif