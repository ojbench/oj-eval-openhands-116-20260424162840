#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <deque>
#include <vector>

struct Map;
enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map


struct Snake{
  // store the necessary information of the snake
  std::deque<std::pair<int, int>> body;
  instruction current_dir;
  
  void initialize(int x, int y, instruction ins){
    body.clear();
    body.push_back({x, y});
    current_dir = ins;
  }
  int get_length(){
    return body.size();
  }
  bool move(Map* map, instruction ins){
    if(ins != instruction::NONE){
      if((current_dir == instruction::UP && ins == instruction::DOWN) ||
         (current_dir == instruction::DOWN && ins == instruction::UP) ||
         (current_dir == instruction::LEFT && ins == instruction::RIGHT) ||
         (current_dir == instruction::RIGHT && ins == instruction::LEFT)){
           return false;
      }
      current_dir = ins;
    }
    
    std::pair<int, int> head = body.front();
    int nx = head.first;
    int ny = head.second;
    
    if(current_dir == instruction::UP) nx--;
    else if(current_dir == instruction::DOWN) nx++;
    else if(current_dir == instruction::LEFT) ny--;
    else if(current_dir == instruction::RIGHT) ny++;
    
    if(nx < 0 || nx >= get_height(map) || ny < 0 || ny >= get_width(map)) return false;
    if(is_wall(map, nx, ny)) return false;
    
    bool eating = is_food(map, nx, ny);
    for(size_t i = 0; i < (eating ? body.size() : body.size() - 1); ++i){
      if(body[i].first == nx && body[i].second == ny) return false;
    }

    body.push_front({nx, ny});
    if(eating){
      eat_food(map, nx, ny);
    } else {
      body.pop_back();
    }
    return true;
  }
  std::pair<int, std::pair<int,int>*> get_snake(){
    int len = body.size();
    std::pair<int, int>* arr = new std::pair<int, int>[len];
    for(int i=0; i<len; ++i){
      arr[i] = body[i];
    }
    return {len, arr};
  }
};

const int MaxWidth = 20;
struct Map{
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;
  int get_height(){
    return height;
  }
  int get_width(){
    return width;
  }
  bool is_food(int x, int y){
    return food[x][y];
  }
  void eat_food(int x, int y){
    food[x][y] = false;
  }
  bool is_wall(int x, int y){
    return wall[x][y];
  }

  void initialize(Snake *snake){
    if(!(std::cin >> height >> width)) return;
    int head_x = -1 , head_y = -1;
    instruction ins;
    for(int i=0; i<height; ++i){
      for(int j=0; j<width; ++j){
        char c;
        std::cin >> c;
        wall[i][j] = (c == '#');
        food[i][j] = (c == '*');
        if(c == '@'){
          head_x = i;
          head_y = j;
        }
      }
    }
    char dir_char;
    std::cin >> dir_char;
    if(dir_char == 'U') ins = instruction::UP;
    else if(dir_char == 'D') ins = instruction::DOWN;
    else if(dir_char == 'L') ins = instruction::LEFT;
    else if(dir_char == 'R') ins = instruction::RIGHT;
    else ins = instruction::NONE;
    snake->initialize(head_x, head_y, ins);
  }
  void print(Snake *snake){
    auto snake_info = snake->get_snake();
    int len = snake_info.first;
    std::pair<int, int>* body_pos = snake_info.second;
    for(int i=0; i<height; ++i){
      for(int j=0; j<width; ++j){
        bool is_snake = false;
        if(body_pos[0].first == i && body_pos[0].second == j){
          std::cout << "@";
          is_snake = true;
        } else {
          for(int k=1; k<len; ++k){
            if(body_pos[k].first == i && body_pos[k].second == j){
              std::cout << "o";
              is_snake = true;
              break;
            }
          }
        }
        if(!is_snake){
          if(wall[i][j]) std::cout << "#";
          else if(food[i][j]) std::cout << "*";
          else std::cout << ".";
        }
      }
      std::cout << std::endl;
    }
    delete[] body_pos;
  }

};

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;
  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }
  bool step()
  {
    char str[MaxWidth];
    std::cin >> str;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }
  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }
  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif