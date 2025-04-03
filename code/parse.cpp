#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>

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

std::pair<int, int> get_player_position(std::vector<std::string> map, int line_length) {
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

bool recurse(const std::vector<std::string>& map, std::pair<int, int> current, std::pair<int, int> target, std::set<std::pair<int, int>>& visited, std::pair<int, int> hw_pair) {
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };

    if (current == target) {
        return true;
    }

    visited.insert(current);

    for (const auto& dir : directions) {
        int next_x = current.second + dir.second;
        int next_y = current.first + dir.first;

        if (next_x < 0 || next_x >= hw_pair.second || next_y < 0 || next_y >= hw_pair.first) {
            continue;
        }

        if (map[next_y][next_x] == '1' || visited.count({next_y, next_x}) > 0) {
            continue;
        }

        if (recurse(map, {next_y, next_x}, target, visited, hw_pair)) {
            return true;
        }
    }

    return false;
}

bool map_has_correct_paths(const std::vector<std::string>& map, std::pair<int, int> hw_pair) {
    std::cout << "Getting indices.\n";
    std::vector<std::pair<int, int>> important_indices = get_important_indices(map, hw_pair.second);
    std::cout << "Getting starting position.\n";
    std::pair<int, int> starting_pos = get_player_position(map, hw_pair.second);

    for (std::pair<int, int> target : important_indices) {
        std::set<std::pair<int, int>> visited;

        std::cout << "Checking path to target: " << target.first << ", " << target.second << "\n";

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