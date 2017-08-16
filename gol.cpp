#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

#include <SFML/Graphics.hpp>

size_t number_neighbours(std::vector<bool> &grid, const size_t x, const size_t y, const size_t nbr_cell_x, const size_t nbr_cell_y)
{
  size_t nbr_neighbours = 0;

  // North
  bool is_north_ok = y > 0;
  size_t north = y - 1;  
  if (is_north_ok && grid[north + x * nbr_cell_y]) nbr_neighbours++;

  // East
  bool is_east_ok = x < (nbr_cell_x - 1);
  size_t east = x + 1;
  if (is_east_ok && grid[y + east * nbr_cell_y]) nbr_neighbours++;

  // South
  bool is_south_ok = y < (nbr_cell_y - 1);
  size_t south = y + 1;
  if (is_south_ok && grid[south + x * nbr_cell_y]) nbr_neighbours++;

  // West
  bool is_west_ok = x > 0;
  size_t west = x - 1;
  if (is_west_ok && grid[west + y * nbr_cell_y]) nbr_neighbours++;

  // North - East
  if (is_north_ok && is_east_ok && grid[north + east * nbr_cell_y]) nbr_neighbours++;

  // North - West
  if (is_north_ok && is_west_ok && grid[north + west * nbr_cell_y]) nbr_neighbours++;

  // South - East
  if (is_south_ok && is_east_ok && grid[south + east * nbr_cell_y]) nbr_neighbours++;

  // South - West
  if (is_south_ok && is_west_ok && grid[south + west * nbr_cell_y]) nbr_neighbours++;

  return nbr_neighbours;
}

void randomize(std::vector<bool> &grid, const int threshold_birth)
{
  for (size_t i = 0; i < grid.size(); ++i)
    grid[i] = (rand() % 100) > threshold_birth;
}

std::vector<bool> next_generation(std::vector<bool> &grid, const size_t nbr_cell_x, const size_t nbr_cell_y)
{
  size_t nbr_neig = 0;
  std::vector<bool> new_grid = std::vector<bool>(grid.size());
  for (size_t row = 0; row < nbr_cell_x; ++row)
  {
    for (size_t col = 0; col < nbr_cell_y; ++col)
    {
      nbr_neig = number_neighbours(grid, row, col, nbr_cell_x, nbr_cell_y);
      // Living cell
      if (grid[col + row * nbr_cell_y])
      {
        // Underpopulation or Overpopulation = death
        if (nbr_neig < 2 || nbr_neig > 3)
          new_grid[col + row * nbr_cell_y] = false;
        else
          new_grid[col + row * nbr_cell_y] = true;
      }
      else
      // Empy cell
      {
        // Give life
        if (nbr_neig == 3)
          new_grid[col + row * nbr_cell_y] = true;
      }
    }
  }
  return new_grid;
}

void place_pixels(sf::Uint8 *pixels, const std::vector<bool> &grid, const size_t window_height, const size_t nbr_cell_x, const size_t nbr_cell_y, const size_t cell_width, const size_t cell_height)
{
  sf::Color dead = sf::Color::White;
  sf::Color alive = sf::Color::Black;
  bool is_used = false;
  size_t pixel = 0;
  size_t offset_x = 0;
  size_t offset_y = 0;
  for (size_t i = 0; i < nbr_cell_x; ++i)
  {
    for (size_t j = 0; j < nbr_cell_y; ++j)
    {
      is_used = grid[j + i * nbr_cell_y];
      offset_x = i * cell_width * 4;
      offset_y = j * cell_height * 4;
      for (size_t row = 0; row < cell_width; ++row)
      {
        for (size_t col = 0; col < cell_height; ++col)
        {
          pixel = (offset_y + row * 4) + (offset_x + col * 4) * window_height;

          pixels[pixel] = (is_used) ? alive.r : dead.r;
          pixels[pixel + 1] = (is_used) ? alive.g : dead.g;
          pixels[pixel + 2] = (is_used) ? alive.b : dead.b;
          pixels[pixel + 3] = (is_used) ? alive.a : dead.a;
        }
      }
    }
  }
}

int main()
{
  //sf::VideoMode mode = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(sf::VideoMode(500, 500), "GOL"/*, sf::Style::Fullscreen*/);
  sf::Vector2u window_size = window.getSize();
  size_t window_width = window_size.x;
  size_t window_height = window_size.y;

  srand(time(NULL));

  size_t cell_width = 5;
  size_t cell_height = 5;

  size_t nbr_cell_x = window_width / cell_width; // 16
  size_t nbr_cell_y = window_height / cell_height; // 12
  size_t capacity = nbr_cell_x * nbr_cell_y; // 192

  std::vector<bool> grid = std::vector<bool>(capacity);
  int threshold_birth = 80;
  randomize(grid, threshold_birth);

  size_t pixels_size = window_width * window_height * 4;
  sf::Uint8 pixels[pixels_size];

  place_pixels(pixels, grid, window_height, nbr_cell_x, nbr_cell_y, cell_width, cell_height);

  sf::Texture texture;
  texture.create(window_width, window_height);
  sf::Sprite sprite(texture);
  texture.update(pixels);

  int current_speed = 250;
  int prev_speed = 0;
  int incr_speed = 50;
  int limit_speed = 1000;

  bool auto_gen = true;
  bool manu_gen = false;
  bool grid_clear = false;

  sf::Clock clock;
  while (window.isOpen())
  {
    sf::Event event;
    // Make a function
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
        case sf::Event::Closed :
          window.close();
          break;
        case sf::Event::KeyPressed :
          switch (event.key.code)
          {
            case sf::Keyboard::Escape :
              window.close();
              break;
            case sf::Keyboard::Add :
              if (current_speed > 0)
                current_speed -= incr_speed;
              break;
            case sf::Keyboard::Subtract :
              if (current_speed < limit_speed)
                current_speed += incr_speed;
              break;
            case sf::Keyboard::Right :
              if (!auto_gen)
              {
                manu_gen = true;
              }
              else
              {
                auto_gen = false;
                prev_speed = current_speed;
                current_speed = 0;
              }
              break;
            case sf::Keyboard::Left :
              break;
            case sf::Keyboard::Space :
              current_speed = prev_speed;
              auto_gen = true;
              break;
            case sf::Keyboard::C :
              grid_clear = true;
              break;
            case sf::Keyboard::R :
              randomize(grid, threshold_birth);
              grid_clear = false;
              break;
            default : break;
          }
        default: break;
      }
    }

    window.clear(sf::Color::White);

    if (!grid_clear)
    {
      window.draw(sprite);

      if ((auto_gen || manu_gen) && clock.getElapsedTime().asMilliseconds() >= current_speed)
      {
        grid = next_generation(grid, nbr_cell_x, nbr_cell_y);

        place_pixels(pixels, grid, window_height, nbr_cell_x, nbr_cell_y, cell_width, cell_height);

        texture.update(pixels);

        clock.restart();
      }
      manu_gen = false;
    }

    window.display();

  }
  return 0;
}