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
  if (is_north_ok && grid[x + north * grid.size()]) {
    nbr_neighbours++;
    printf("[#] North\n");
  }
  
  // East
  bool is_east_ok = x < nbr_cell_x;
  size_t east = x + 1;
  if (is_east_ok && grid[east + y * grid.size()]) {
    nbr_neighbours++;
    printf("[#] East\n");
  }
  
  // South
  bool is_south_ok = y < nbr_cell_y;
  size_t south = y + 1;
  if (is_south_ok && grid[x + south * grid.size()]) {
    nbr_neighbours++;
    printf("[#] South\n");
  }
  
  // West
  bool is_west_ok = x > 0;
  size_t west = x - 1;
  if (is_west_ok && grid[west + y * grid.size()]) {
    nbr_neighbours++;
    printf("[#] West\n");
  }

  // North - East
  if (is_north_ok && is_east_ok && grid[east + north * grid.size()]) {
    nbr_neighbours++;
    printf("[#] North - East\n");
  }
  
  // North - West
  if (is_north_ok && is_west_ok && grid[west + north * grid.size()]) {
    nbr_neighbours++;
    printf("[#] North - West\n");
  }
  
  // South - East
  if (is_south_ok && is_east_ok && grid[east + south * grid.size()]) {
    nbr_neighbours++;
    printf("[#] South - East\n");
  }
  
  // North - West
  if (is_south_ok && is_west_ok && grid[west + south * grid.size()]) {
    nbr_neighbours++;
    printf("[#] South - West\n");
  }
  
  return nbr_neighbours;
}

size_t randomize(std::vector<bool> &grid)
{  
  size_t nbr_cell_used = 0;
  for (size_t i = 0; i < grid.size(); ++i)
  {
    if ((rand() % 100) > 80)
    {
      grid[i] = true;
      nbr_cell_used++;
    }
    else
    {
      grid[i] = false;
    }
  }
  
  return nbr_cell_used;
}

void generate_quads(std::vector<sf::VertexArray*> &quads, std::vector<bool> &grid, const size_t nbr_cell_x, const size_t nbr_cell_y, const size_t cell_width, const size_t cell_height)
{
  size_t pos_x = 0;
  size_t pos_y = 0;
  size_t index = 0;
  for (size_t i = 0; i < nbr_cell_x; ++i)
  {
    pos_y = 0;
    for (size_t j = 0; j < nbr_cell_y; ++j)
    {
      if (grid[i + j * nbr_cell_x])
      {        
        quads[index] = new sf::VertexArray(sf::Quads, 4);
        
        (*quads[index])[0].position = sf::Vector2f(pos_x, pos_y);
        (*quads[index])[1].position = sf::Vector2f(pos_x + cell_width, pos_y);
        (*quads[index])[2].position = sf::Vector2f(pos_x + cell_width, pos_y + cell_height);
        (*quads[index])[3].position = sf::Vector2f(pos_x, pos_y + cell_height);
        
        (*quads[index])[0].color = sf::Color::Black;
        (*quads[index])[1].color = sf::Color::Black;
        (*quads[index])[2].color = sf::Color::Black;
        (*quads[index])[3].color = sf::Color::Black;
        
        index++;
      }
      pos_y += cell_height;
    }
    pos_x += cell_width;
  }
}

int main()
{
  /*
  srand(time(NULL));
  
  int window_width = 800;
  int window_height = 600;
  
  size_t cell_width = 10;
  size_t cell_height = 10;
  
  size_t nbr_cell_x = window_width / cell_width;
  size_t nbr_cell_y = window_height / cell_height;
  size_t capacity = nbr_cell_x * nbr_cell_y;

  sf::RenderWindow window(sf::VideoMode(window_width, window_height), "GOL");
  window.setFramerateLimit(60);
  
  std::vector<bool> grid = std::vector<bool>(capacity);
  size_t nbr_cell_used = randomize(grid);
  
  std::vector<sf::VertexArray*> quads = std::vector<sf::VertexArray*>(nbr_cell_used);
  generate_quads(quads, grid, nbr_cell_x, nbr_cell_y, cell_width, cell_height);
  
  while (window.isOpen())
  {
    sf::Event event;
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
            default : break;
          }
        default: break;
      }
    }
    
    window.clear(sf::Color::White);
    
    for (size_t i = 0; i < quads.size(); ++i) {
      window.draw(*quads[i]);
    }
    
    window.display();        
  }
  
  for (size_t i = 0; i < quads.size(); ++i) {
    delete quads[i];
  }
  */
  
  std::vector<bool> grid = std::vector<bool>(9);
  
  grid[0] = true;
  grid[1] = true;
  grid[2] = true;
  grid[3] = true;
  grid[4] = true;
  grid[5] = true;
  grid[6] = true;
  grid[7] = true;
  
  printf("[*] Nbr neighbours : %zu\n", number_neighbours(grid, 1, 1, 3, 3));
  
  return 0;
}