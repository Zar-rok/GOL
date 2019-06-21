#include <ctime>
#include <cstdlib>
#include <vector>
#include <stdio.h>

#include <SFML/Graphics.hpp>

struct Board {
  size_t cell_width;
  size_t cell_height;
  size_t nbr_cell_x;
  size_t nbr_cell_y;
  std::vector<bool> grid;
};

/* Compute the number of neighbours of one cell. */
size_t number_neighbours(const struct Board *board, size_t x, size_t y) {
  bool is_north_ok = y > 0;
  size_t north = y - 1;
  bool is_east_ok = x < (board->nbr_cell_x - 1);
  size_t east = x + 1;
  bool is_south_ok = y < (board->nbr_cell_y - 1);
  size_t south = y + 1;
  bool is_west_ok = x > 0;
  size_t west = x - 1;

  size_t nbr_neighbours = 0;
  if (is_north_ok && board->grid[north + x * board->nbr_cell_y]) ++nbr_neighbours;
  if (is_east_ok  && board->grid[y + east * board->nbr_cell_y])  ++nbr_neighbours;
  if (is_south_ok && board->grid[south + x * board->nbr_cell_y]) ++nbr_neighbours;
  if (is_west_ok  && board->grid[y + west * board->nbr_cell_y])  ++nbr_neighbours;
  if (is_north_ok && is_east_ok && board->grid[north + east * board->nbr_cell_y]) ++nbr_neighbours;
  if (is_north_ok && is_west_ok && board->grid[north + west * board->nbr_cell_y]) ++nbr_neighbours;
  if (is_south_ok && is_east_ok && board->grid[south + east * board->nbr_cell_y]) ++nbr_neighbours;
  if (is_south_ok && is_west_ok && board->grid[south + west * board->nbr_cell_y]) ++nbr_neighbours;

  return nbr_neighbours;
}

/* Fill randomly the boolean vector who represent the population of the grid (true = alive, false = dead). */
void randomize(Board *board, int threshold_birth) {
  for (size_t i = 0; i < board->grid.size(); ++i)
    board->grid[i] = (rand() % 100) > threshold_birth;
}

/* Use the last generation to apply the rules and get the new one. */
void next_generation(const Board *board, std::vector<bool> *new_grid) {
  size_t nbr_neig = 0;
  int offset = 0;
  for (size_t row = 0; row < board->nbr_cell_x; ++row) {
    for (size_t col = 0; col < board->nbr_cell_y; ++col) {
      nbr_neig = number_neighbours(board, row, col);
      offset = col + row * board->nbr_cell_y;
      // Living cell
      if (board->grid[offset]) {
        // Underpopulation or Overpopulation = death
        if (nbr_neig < 2 || nbr_neig > 3)
          new_grid->at(offset) = false;
        else
          new_grid->at(offset) = true;
      }
      else {
        // Give life
        if (nbr_neig == 3)
          new_grid->at(offset) = true;
      }
    }
  }
}

/* Use the boolean vector to fill an array of pixel. */
void place_pixels(sf::Uint8 *pixels, const Board *board, size_t window_width, size_t window_height) {
  bool is_alive = false;
  size_t pixel = 0;
  size_t offset_x = 0;
  size_t offset_y = 0;

  for (size_t i = 0; i < board->nbr_cell_x; ++i) {
    for (size_t j = 0; j < board->nbr_cell_y; ++j) {
      is_alive = board->grid[j + i * board->nbr_cell_y];
      offset_x = i * board->cell_width * 4;
      offset_y = j * board->cell_height * 4;
      // Initialize the pixels for each cells
      for (size_t row = 0; row < board->cell_width; ++row) {
        for (size_t col = 0; col < board->cell_height; ++col) {
          pixel = (offset_x + row * 4) + (offset_y + col * 4) * window_width;
          // Toggle opacity
          pixels[pixel + 3] = (is_alive) ? 255 : 0;
        }
      }
    }
  }
}

void reset_pixels(sf::Uint8 *pixels, size_t pixels_size, sf::Color alive) {
  for (size_t i = 0; i < pixels_size; i += 4) {
    pixels[i] = alive.r;
    pixels[i + 1] = alive.g;
    pixels[i + 2] = alive.b;
    pixels[i + 3] = 0;
  }
}

int main() {
  srand(time(NULL));

  int current_speed = 250;
  int prev_speed = current_speed;
  int incr_speed = 50;
  int limit_speed = 1500;

  bool auto_gen = true;
  bool manu_gen = false;
  bool grid_clear = false;
  bool as_been_reset = false;
  bool help = false;

  sf::VideoMode mode = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(mode, "GOL");//, sf::Style::Fullscreen);
  sf::Vector2u window_size = window.getSize();
  size_t window_width = window_size.x;
  size_t window_height = window_size.y;

  int threshold_birth = 80;
  struct Board board;
  board.cell_width = 2;
  board.cell_height = 2;
  board.nbr_cell_x = window_width / board.cell_width;
  board.nbr_cell_y = window_height / board.cell_height;

  size_t capacity = board.nbr_cell_x * board.nbr_cell_y;
  board.grid = std::vector<bool>(capacity);
  randomize(&board, threshold_birth);

  sf::Color alive = sf::Color::Black;
  size_t pixels_size = window_width * window_height * 4;
  sf::Uint8 pixels[pixels_size];
  reset_pixels(pixels, pixels_size, alive);

  sf::Texture texture;
  texture.create(window_width, window_height);
  sf::Sprite sprite(texture);
  texture.update(pixels);

  sf::Clock clock;
  sf::Event event;
  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed :
          window.close();
          break;
        case sf::Event::KeyPressed :
          switch (event.key.code) {
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
                manu_gen = true;
              else {
                auto_gen = false;
                prev_speed = current_speed;
                current_speed = 0;
              }
              break;
            case sf::Keyboard::Left :
              break;
            case sf::Keyboard::Space :
              current_speed = prev_speed;
              auto_gen = !auto_gen;
              break;
            case sf::Keyboard::C :
              grid_clear = true;
              break;
            case sf::Keyboard::R :
              randomize(&board, threshold_birth);
              grid_clear = false;
              as_been_reset = true;
              break;
            case sf::Keyboard::F1 :
              help = !help;
              break;
            default : break;
          }
        default: break;
      }
    }

    if (!grid_clear) {
      if ((auto_gen || manu_gen || as_been_reset) && clock.getElapsedTime().asMilliseconds() >= current_speed) {
        std::vector<bool> new_grid = std::vector<bool>(board.grid.size());
        next_generation(&board, &new_grid);
        board.grid = new_grid;

        place_pixels(pixels, &board, window_width, window_height);

        texture.update(pixels); 

        window.clear(sf::Color::White);

        window.draw(sprite);

        window.display();

        clock.restart();

        as_been_reset = false;
      }
      manu_gen = false;
    }
 }
  return 0;
}
