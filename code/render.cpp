#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <parse.hpp>

void render(std::vector<std::string> map, int width, int height)
{

    int turns = 1000;
    int points = 0;

    unsigned int window_size_x = 1920;
    unsigned int window_size_y = 1080;

    sf::Font Tektur;
    if (!Tektur.openFromFile("..\\fonts\\Tektur\\static\\Tektur-Regular.ttf")) {
        return;
    }

    sf::Text turn_count_instance(Tektur);
    turn_count_instance.setString("Turns:\n" + std::to_string(turns));
    turn_count_instance.setPosition(sf::Vector2f(0, 0));

    sf::Text point_count_instance(Tektur);
    point_count_instance.setString("Points:\n" + std::to_string(points));
    point_count_instance.setPosition(sf::Vector2f(0, 100));

    sf::Texture wall_texture;
    if (!wall_texture.loadFromFile("..\\textures\\wall.png")) {
        return;
    }

    sf::Texture free_space_texture;
    if (!free_space_texture.loadFromFile("..\\textures\\free_space.png")) {
        return;
    }

    sf::Texture player_texture;
    if (!player_texture.loadFromFile("..\\textures\\player.png")) {
        return;
    }

    sf::Texture exit_texture;
    if (!exit_texture.loadFromFile("..\\textures\\exit.png")) {
        return;
    }

    sf::Texture collectible_texture;
    if (!collectible_texture.loadFromFile("..\\textures\\collectible.png")) {
        return;
    }

    sf::RenderWindow window(sf::VideoMode({window_size_x, window_size_y}), "SFML");
    window.setFramerateLimit(10);

    double square_size = std::min(window_size_x / width, window_size_y / height);

    double offset_x = (window_size_x - (width * square_size)) / 2.0;
    double offset_y = (window_size_y - (height * square_size)) / 2.0;

    std::map<std::pair<int, int>, sf::Sprite> sprites;

    sf::Sprite player_sprite(player_texture);
    player_sprite.setScale(sf::Vector2f(square_size, square_size));

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            sf::Sprite sprite(free_space_texture);

            if (map[row][col] == '1')
            {
                sprite.setTexture(wall_texture);
            }
            else if (map[row][col] == '0')
            {
                sprite.setTexture(free_space_texture);
            }
            else if (map[row][col] == 'P')
            {
                player_sprite.setPosition(sf::Vector2f(col * square_size + offset_x, row * square_size + offset_y));
            }
            else if (map[row][col] == 'E')
            {
                sprite.setTexture(exit_texture);
            }
            else if (map[row][col] == 'C')
            {
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

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        std::pair<int, int> current_direction = {0, 0};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            current_direction.second -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            current_direction.second += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            current_direction.first += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            current_direction.first -= 1;
        }

        std::pair<int, int> current_pos = get_player_position(map, width);

        char last_cell = '0';
        bool reached_exit = false;

        int next_x = current_pos.second + current_direction.first;
        int next_y = current_pos.first + current_direction.second;

        if (next_x >= 0 && next_x < width && next_y >= 0 && next_y < height && map[next_y][next_x] != '1')
        {
            map[current_pos.first][current_pos.second] = last_cell;
            last_cell = map[next_y][next_x];

            if (last_cell == 'C')
            {
                map[next_y][next_x] = '0';
                sprites.at(std::pair(next_y, next_x)).setTexture(free_space_texture);

                points += 1;
                point_count_instance.setString("Points:\n" + std::to_string(points));
            }
            else if (last_cell == 'E')
            {
                map[next_y][next_x] = '0';
                sprites.at(std::pair(next_y, next_x)).setTexture(free_space_texture);
                reached_exit = true;
            }

            map[next_y][next_x] = 'P';

            sf::Vector2f position(next_x * square_size + offset_x, next_y * square_size + offset_y);
            player_sprite.setPosition(position);

            if (current_direction != std::make_pair(0, 0))
            {
                turns -= 1;
                turn_count_instance.setString("Turns:\n" + std::to_string(turns));
            }
        }

        window.clear();

        for (auto sprite : sprites)
        {
            window.draw(sprite.second);
        }

        window.draw(player_sprite);
        window.draw(turn_count_instance);
        window.draw(point_count_instance);

        window.display();

        // display_full_map(map, height, width);

        if (reached_exit || turns <= 0)
        {
            window.close();
        }
    }
}