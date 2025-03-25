#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <SFML/Graphics.hpp>

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


void display_full_map(std::vector<std::string> map, int height, int width) {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            std::cout << map[row][col];
        }

        std::cout << "\n";
    }
}

int main() {
    std::string file_directory = ".\\maps\\map1.txt";

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

    int turns = 1000;
    int points = 0;
    
    double window_size_x = 1920;
    double window_size_y = 1080;

    sf::Font Tektur;
    Tektur.openFromFile(".\\fonts\\Tektur\\static\\Tektur-Regular.ttf");

    sf::Text turn_count_instance(Tektur);
    turn_count_instance.setString("Turns:\n" + std::to_string(turns));
    turn_count_instance.setPosition(sf::Vector2f(0, 0));

    sf::Text point_count_instance(Tektur);
    point_count_instance.setString("Points:\n" + std::to_string(points));
    point_count_instance.setPosition(sf::Vector2f(0, 100));

    sf::Texture wall_texture;
    wall_texture.loadFromFile(".\\textures\\wall.png");

    sf::Texture free_space_texture;
    free_space_texture.loadFromFile(".\\textures\\free_space.png");

    sf::Texture player_texture;
    player_texture.loadFromFile(".\\textures\\player.png");

    sf::Texture exit_texture;
    exit_texture.loadFromFile(".\\textures\\exit.png");

    sf::Texture collectible_texture;
    collectible_texture.loadFromFile(".\\textures\\collectible.png");

    sf::RenderWindow window(sf::VideoMode({window_size_x, window_size_y}), "SFML");
    window.setFramerateLimit(10);

    double square_size = std::min(window_size_x / width, window_size_y / height);

    double offset_x = (window_size_x - (width * square_size)) / 2.0;
    double offset_y = (window_size_y - (height * square_size)) / 2.0;

    std::map<std::pair<int, int>, sf::Sprite> sprites;

    sf::Sprite player_sprite(player_texture);
    player_sprite.setScale(sf::Vector2f(square_size, square_size));

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            sf::Sprite sprite(free_space_texture);


            if (map[row][col] == '1') {
                sprite.setTexture(wall_texture);
            } else if (map[row][col] == '0') {
                sprite.setTexture(free_space_texture);
            } else if (map[row][col] == 'P') {
                player_sprite.setPosition(sf::Vector2f(col * square_size + offset_x, row * square_size + offset_y));
            } else if (map[row][col] == 'E') {
                sprite.setTexture(exit_texture);
            } else if (map[row][col] == 'C') {
                sprite.setTexture(collectible_texture);
            }

            sf::Vector2u texture_size = sprite.getTexture().getSize();

            float scale_x = square_size / static_cast<float>(texture_size.x);
            float scale_y = square_size / static_cast<float>(texture_size.y);

            float scale_factor = std::min(scale_x, scale_y);
            sprite.setScale(sf::Vector2f(scale_factor, scale_factor));

            sf::Vector2f position(col * square_size + offset_x, row * square_size + offset_y);
            sprite.setPosition(sf::Vector2f(position.x + (square_size - texture_size.x * scale_factor) / 2.0f,
            position.y + (square_size - texture_size.y * scale_factor) / 2.0f));

            sprites.insert({std::make_pair(row, col), sprite});
        }
    }

    sf::Vector2u player_texture_size = player_texture.getSize();
    float player_scale_x = square_size / static_cast<float>(player_texture_size.x);
    float player_scale_y = square_size / static_cast<float>(player_texture_size.y);
    float player_scale_factor = std::min(player_scale_x, player_scale_y);
    player_sprite.setScale(sf::Vector2f(player_scale_factor, player_scale_factor));

    sf::Vector2f player_position = player_sprite.getPosition();
    player_sprite.setPosition(sf::Vector2f(player_position.x + (square_size - player_texture_size.x * player_scale_factor) / 2.0f,
    player_position.y + (square_size - player_texture_size.y * player_scale_factor) / 2.0f));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        std::pair<int, int> current_direction = {0, 0};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            current_direction.second -= 1;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            current_direction.second += 1;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            current_direction.first += 1;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            current_direction.first -= 1;
        }

        std::pair<int, int> current_pos = get_player_position(map, width);

        char last_cell = '0';
        bool reached_exit = false;

        int next_x = current_pos.second + current_direction.first;
        int next_y = current_pos.first + current_direction.second;

        if (next_x >= 0 && next_x < width && next_y >= 0 && next_y < height && map[next_y][next_x] != '1') {
            map[current_pos.first][current_pos.second] = last_cell;
            last_cell = map[next_y][next_x];
            
            if (last_cell == 'C') {
                map[next_y][next_x] = '0';
                sprites.at(std::pair(next_y, next_x)).setTexture(free_space_texture);

                points += 1;
                point_count_instance.setString("Points:\n" + std::to_string(points));
            } else if (last_cell == 'E') {
                map[next_y][next_x] = '0';
                sprites.at(std::pair(next_y, next_x)).setTexture(free_space_texture);
                reached_exit = true;
            }

            map[next_y][next_x] = 'P';

            sf::Vector2f position(next_x * square_size + offset_x, next_y * square_size + offset_y);
            player_sprite.setPosition(position);

            if (current_direction != std::make_pair(0, 0)) {
                turns -= 1;
                turn_count_instance.setString("Turns:\n" + std::to_string(turns));
            }
        }

        window.clear();

        for (auto sprite : sprites) {
            window.draw(sprite.second);
        }

        window.draw(player_sprite);
        window.draw(turn_count_instance);
        window.draw(point_count_instance);

        window.display();

        //display_full_map(map, height, width);

        if (reached_exit || turns <= 0) {
            window.close();
        }
    }

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