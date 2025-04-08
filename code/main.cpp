#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <parse.hpp>
#include <render.hpp>

void display_full_map(std::vector<std::string> map, int height, int width) {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            std::cout << map[row][col];
        }

        std::cout << "\n";
    }
}

int main() {
    std::string file_directory = "..\\maps\\map1.txt";

    std::ifstream file(file_directory);
    std::string my_text;
    std::vector<std::string> map;

    int width = 0;
    int height = 0;

    while (std::getline(file, my_text)) {
        map.push_back(my_text);
        
        width = my_text.length();
        height += 1;

        std::cout << my_text << "\n";
    }

    std::cout << "\n";

    bool is_valid = is_map_valid(map);

    file.close();
    if (!is_valid) {
        std::cout << "WRONG!11!1" << "\n";
        return -1;
    } else {
        std::cout << "VALID!!1!1" << "\n";
    }

    render(map, width, height);
    return 0;
}
