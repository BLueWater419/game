#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
// #include <SFML/Graphics.hpp>
// #include "C:\\Users\\g0ldc\\OneDrive\\Dokumente\\Libraries\\SFML-3.0.0\\include\\SFML\\Graphics.hpp"

std::vector<std::pair<int, int>> get_important_indices(std::vector<std::string> map, int line_length) {
    std::vector<std::pair<int, int>> indices;

    for (int row_index = 0; row_index < map.size(); row_index++) {
        for (int line_index = 0; line_index < line_length; line_index++) {
            if (map.at(row_index)[line_index] == 'E' || map.at(row_index)[line_index] == 'C') {
                indices.push_back(std::make_pair(row_index, line_index));
            }
        }
    }

    return indices;
}

std::pair<int, int> get_starting_position(std::vector<std::string> map, int line_length) {
    std::pair<int, int> pair;

    for (int row_index = 0; row_index < map.size(); row_index++) {
        for (int line_index = 0; line_index < line_length; line_index++) {
            if (map.at(row_index)[line_index] == 'P') {
                pair = std::make_pair(row_index, line_index);
                break;
            }
        }
    }

    return pair;
}

bool is_pair_in_vector(std::pair<int, int> pair, std::vector<std::pair<int, int>> vector) {
    for (std::pair<int, int> p : vector) {
        if (p == pair) {
            return true;
        }
    }
    return false;
}

std::pair<int, int> is_map_right_shape(std::vector<std::string> map) {
    int w = -1;
    int h = map.size();

    for (int i = 0; i < h; i++) {
        std::string line = map[i];
        int line_length = line.length();

        if (w != -1 && line_length != w) {
            return std::make_pair(-1, -1);
        }

        if (i == 0 || i == h - 1) {
            for (int v = 0; i < line_length; i++) {
                if (line[v] != '1') {
                    return std::make_pair(-1, -1);
                }
            }
        }

        if (line[0] != '1' || line[line_length - 1] != '1') {
            return std::make_pair(-1, -1);
        }

        w = line_length;
    }

    return std::make_pair(h, w);
}

bool recurse(std::vector<std::string> map, std::pair<int, int> current, std::pair<int, int> target, std::vector<std::pair<int, int>> visited, std::pair<int, int> hw_pair) {
    std::vector<std::pair<int, int>> directions = {
        std::make_pair(0, 1), std::make_pair(1, 0),
        std::make_pair(0, -1), std::make_pair(-1, 0)
    };

    if (current == target) return true;

    visited.push_back(current);

    for (std::pair<int, int> dir : directions) {
        int next_x = (current.second + dir.second) % hw_pair.second;
        int next_y = (current.first + dir.first) % hw_pair.first;
        std::pair<int, int> next_pos = {next_y, next_x};

        if (is_pair_in_vector(next_pos, visited) || (map[next_y][next_x] == '1')) {
            continue;
        }

        if (recurse(map, next_pos, target, visited, hw_pair)) {
            return true;
        }
    }

    return false;
}

bool map_has_correct_paths(std::vector<std::string> map, std::pair<int, int> hw_pair) {
    std::vector<std::pair<int, int>> important_indices = get_important_indices(map, hw_pair.second);
    std::pair<int, int> starting_pos = get_starting_position(map, hw_pair.second);

    for (std::pair<int, int> target : important_indices) {
        std::vector<std::pair<int, int>> visited;

        if (!recurse(map, starting_pos, target, visited, hw_pair)) {
            return false;
        }   
    }

    return true;
}

bool is_map_valid(std::vector<std::string> map) {
    if (map.empty()) {
        std::cout << "map doesn't have any content\n";
        return false;
    }

    std::cout << "checking if map is valid..\n";
    std::cout << "checking for map shape.." << "\n";

    std::pair<int, int> hw_pair = is_map_right_shape(map);

    if (hw_pair.first == -1) {
        return false;
    }

    std::cout << "checking for valid paths.." << "\n";
    return map_has_correct_paths(map, hw_pair);
}

int main() {
    std::string file_directory = ".\\maps\\map1.txt";

    std::ifstream file(file_directory);
    std::string my_text;
    std::vector<std::string> map;

    while (std::getline(file, my_text)) {
        map.push_back(my_text);

        std::cout << my_text << "\n";
    }

    std::cout << "\n";

    bool is_valid = is_map_valid(map);

    file.close();
    if (!is_valid) {
        std::cout << "WRONG!11!1";
        return -1;
    }

    std::cout << "VALID!!1!1";

    /*

    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "SFML");
    sf::CircleShape shape(100.0);
    shape.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    */

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