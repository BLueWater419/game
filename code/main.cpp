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
        // return -1;
    } else {
        std::cout << "VALID!!1!1" << "\n";
    }

    render(map, width, height);

    return 0;
}

/*
bool map_has_correct_paths_loop(std::vector<std::string> map, std::pair<int, int> hw_pair) {
    std::vector<std::pair<int, int>> directions = {
        std::make_pair(0, 1), std::make_pair(1, 0),
        std::make_pair(0, -1), std::make_pair(-1, 0)
    };

    std::vector<std::pair<int, int>> important_indices = get_important_indices(map, hw_pair.second);
    std::pair<int, int> starting_pos = get_starting_position(map, hw_pair.second);

    for (std::pair<int, int> indices : important_indices) {
        std::deque<std::pair<int, int>> queue;
        std::vector<std::pair<int, int>> explored;

        bool found = false;

        queue.push_back(starting_pos);
        explored.push_back(starting_pos);

        while (!queue.empty()) {
            std::pair<int, int> v = queue.front();
            queue.pop_front();

            for (std::pair<int, int> pair : directions) {
                int next_x = (v.second + pair.second) % hw_pair.second;
                int next_y = (v.first + pair.first) % hw_pair.first;
                std::pair<int, int> next_pair = std::make_pair(next_y, next_x);

                if (is_pair_in_vector(next_pair, explored)) {
                    continue;
                }
                
                char next_cell = map.at(next_y)[next_x];
                if (next_cell == '0' || next_cell == 'C' || next_cell == 'E') {
                    if (next_x == indices.second && next_y == indices.first) {
                        found = true;
                        break;
                    } else {
                        explored.push_back(next_pair);
                        queue.push_back(next_pair);
                    }
                }
            }

            if (found) {
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

*/