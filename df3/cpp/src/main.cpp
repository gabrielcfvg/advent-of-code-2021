// built-in
#include <thread>

// extern
#include "lib/CLI11.hpp"

// local
#include "map.hpp"

uint process(Vector2ui dir, const Map* map, uint* total)
{

    Vector2ui act {0, 0};
    Vector2ui dim = map->dim;
    uint t = 0;
    while (true)
    {
        act += dir;
        act.x %= dim.x;

        if (act.y >= dim.y)
        {
            break;
        }

        if (map->at(act))
        {
            t++;
        }
    }

    *total = t;
    return t;
}

void parte1(const char* const input_path)
{
    const Map map {input_path};
    uint total;

    process({3, 1}, &map, &total);

}

void parte2(const char* const input_path)
{
    
    const Map map {input_path};
    uint total = 0;
    uint b;
    
    total =  process({1, 1}, &map, &b);
    total *= process({3, 1}, &map, &b);
    total *= process({5, 1}, &map, &b);
    total *= process({7, 1}, &map, &b);
    total *= process({1, 2}, &map, &b);

    std::cout << "resultado: " << total << std::endl;
}


int main(int argc, char** argv)
{
    CLI::App app {"adventofcode 3"};
    app.require_subcommand();

    std::string path {"input.txt"};

    CLI::App* sub1 = app.add_subcommand("part1");
    sub1->add_option("input", path, "input path");
    sub1->callback([path](){
        parte1(path.data());
    });

    CLI::App* sub2 = app.add_subcommand("part2");
    sub2->add_option("input", path, "input path");
    sub2->callback([path](){
        parte2(path.data());
    });

    CLI11_PARSE(app, argc, argv);
}