#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <time.h>
#include "./json.hpp"
#include "./http.h"

using namespace std;
using namespace nlohmann;

/**
 *          SOLID SNEK
 *  A BattleSnake by Ryan Fleck
 *  https://github.com/RyanFleck
 *       dev@ryanfleck.ca
 */

 const bool debug = false;


void print_board(int** arr, int height, int width){
  // Prints arr[height][width]
  if(debug)  cout << "\nT:\tGame Board:" << endl;
  for(int y=height-1; y>=0; y--){
    if(debug) cout << y << ":\t";
    for(int x=0; x<width; x++){
      if(debug) cout << arr[y][x] << "\t";
    }
    if(debug) cout << endl;
  }

  if(debug) cout << endl;
  if(debug) cout << "x:\t";
  for(int x=0; x<width; x++){
    if(debug) cout << x << "\t";
  }
  if(debug) cout << endl;
}



void delete_frame(int** arr, int height, int width){
  for(int y=height-1; y>=0; y--){
    delete[] arr[y];
  }
  delete[] arr;
}

void change_block(int** arr, int height, int width, int ix, int iy, int delta){
  // Changes a 3x3 block of elements.
  for(int y=iy-1; y <= iy + 1; y++){
    for(int x=ix-1; x <= ix+1; x++){
      if(x >= 0 && x <= width -1){
        if(y >= 0 && y <= height -1){
          arr[y][x] += delta;
        }
      }
    }
  }
}

void change_block_radius(int** arr, int height, int width, int ix, int iy, int delta, int radius){
  // Changes a 5x5 on edges block of elements.
  for(int y=iy-radius; y <= iy + radius; y++){
    for(int x=ix-radius; x <= ix+radius; x++){
      if(x >= 0 && x <= width -1){
        if(y >= 0 && y <= height -1){
          arr[y][x] += delta;
        }
      }
    }
  }
}

void change_block_if_neutral(int** arr, int height, int width, int ix, int iy, int delta){
  // Changes a 3x3 block of elements.
  for(int y=iy-1; y <= iy + 1; y++){
    for(int x=ix-1; x <= ix+1; x++){
      if(x >= 0 && x <= width -1){
        if(y >= 0 && y <= height -1){
          if(arr[y][x] == 0){
            arr[y][x] += delta;
          }
        }
      }
    }
  }
}

void change_cell(int** arr, int height, int width, int ix, int iy, int delta){
    if(ix >= 0 && ix <= width -1){
      if(iy >= 0 && iy <= height -1){
      arr[iy][ix] += delta;
    }
  }
}

int get_cell(int** arr, int height, int width, int ix, int iy){
    if(ix >= 0 && ix <= width -1){
      if(iy >= 0 && iy <= height -1){
      return arr[iy][ix];
    }
  }
  return -100;
}

/**
 * Initialize game board. Edges are -1.
 */
void initialize(int** arr, int height, int width){
  // Set borders to -1 and other areas to 0.
  for(int y=height-1; y>=0; y--){
    for(int x=0; x<width; x++){
      if(y == 0 || x == 0 || x == width-1 || y == height-1){
        arr[y][x] = -5; 
      }else{
        arr[y][x] = 0; 
      }
    }
  }
  // Set corners to lower.
  // Bottom-left.
  change_block(arr, height, width, 0, 0, -1);
  change_block_radius(arr, height, width, 0, 0, -1, 2);
  change_cell(arr, height, width, 0, 0, -1);
  
  // Bottom-right.
  change_block(arr, height, width, width-1, 0, -1);
  change_block_radius(arr, height, width, width-1, 0, -1, 2);
  change_cell(arr, height, width, width-1, 0, -1);
  
  // top-left.
  change_block(arr, height, width, 0, height-1, -1);
  change_block_radius(arr, height, width, 0, height-1, -1, 2);
  change_cell(arr, height, width, 0, height-1, -1);
  
  // top-right.
  change_block(arr, height, width, width-1, height-1, -1);
  change_block_radius(arr, height, width, width-1, height-1, -1, 2);
  change_cell(arr, height, width, width-1, height-1, -1);
}

void addSnakes(int** arr, int h, int w, const json& snakes, const json& you){
  // Adds Snakes to JSON.
  auto id = you["id"];
  for(const auto &snake : snakes){
    const bool me = snake["id"] == id;
    json body = snake["body"];
    json head = snake["head"];
    const int hx = (int) head["x"];
    const int hy = (int) head["y"];

    // Decrement head.
    change_cell(arr, h, w, hx, hy, -20);
    if(!me) change_block(arr, h, w, hx, hy, -15);

    // Decrement body parts.
    for(const auto &segment : body){
      if(debug) cout << "Adding segment..." << endl;
      const int sx = (int) segment["x"];
      const int sy = (int) segment["y"];
      if(debug) cout << "Segment is at: " << sx << "," << sy << endl;
      change_cell(arr, h, w, sx, sy, -50);
      if(!me) change_block(arr, h, w, sx, sy, -5);
      if(!me) change_block_radius(arr, h, w, sx, sy, -5, 2);
      if(me) change_block(arr, h, w, sx, sy, -5);
    }
    if(debug) cout << endl;
  }
}

void addFood(int** arr, int h, int w, const json& food){
  if(debug) cout << "\nAdding food..." << endl;
  for(const auto &apple : food){
    const int sx = apple["x"]; 
    const int sy = apple["y"]; 
    change_cell(arr, h, w, sx, sy, 10);
    change_block(arr, h, w, sx, sy, 5);
    // Add food bonuses based on distance.
    for(int i=2; i<w; i++)
      change_block_radius(arr, h, w, sx, sy, 1, i);
  }
}

void sortCompass(map<string, int> &compass){
  
}

/**
 * main
 */

int main(void) {
  // Announce Awakening
  if(debug) cout << "Guten Morgen!" << endl;

  // Set Random Seed
  srand((int)time(0));

  // Define HTTP Server
  httplib::Server svr;

  /**
   * Basic status, start, end endpoints.
   */

  svr.Get("/", [](const auto &, auto &res) {
    string head = "dead";
    string tail = "bolt";
    string author = "Ryan Fleck <dev@ryanfleck.ca>";
    string color = "#888800"; 
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "text/json");
  });

  svr.Post("/end", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });

  svr.Post("/start", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });

  /**
   * Movement Method
   */

  svr.Post("/move", [](auto &req, auto &res){
    clock_t startTime = clock();
    // https://github.com/nlohmann/json
    const json data = json::parse(req.body);
    cout << "\n\nPerforming Move Operation. Turn " << data["turn"] << "." << endl;
    cout << "ID: " << data["game"]["id"] << endl;
    const json board = data["board"];
    const json snakes = board["snakes"];
    const json food = board["food"];
    const json you = data["you"];

    // Store Board Properties
    int width = board["width"];
    int height = board["height"];

    int** frame = new int*[height];
    for(int i=0; i<height; i++)
      frame[i] = new int[width];

    // Set bottom-right and top-left corners.
    initialize(frame, height, width);
    
    // if(debug) cout << "\n\nBoard at start of computation:" << endl;
    // if(debug) print_board(frame, height, width);

    // Add Snakes
    addSnakes(frame, height, width, snakes, you);
    
    // if(debug) cout << "\n\nWith Snakes:" << endl;
    // if(debug) print_board(frame, height, width);


    // Add Food
    addFood(frame, height, width, food);
    
    // if(debug) cout << "\n\nWith Food:" << endl;
    if(debug) print_board(frame, height, width);
    
    // Pick the best move.

    // My head position
    const int hx = (int) you["head"]["x"];
    const int hy = (int) you["head"]["y"];
    if(debug) cout << "My head is at "<< hx << "," << hy << ".";

    map<int, string> compass = {
      {get_cell(frame, height, width, hx, hy+1), "up"},
      {get_cell(frame, height, width, hx, hy-1), "down"},
      {get_cell(frame, height, width, hx-1, hy), "left"},
      {get_cell(frame, height, width, hx+1, hy), "right"},
    };

    std::map<int, string>::iterator best
        = std::max_element(compass.begin(),compass.end(),[] (const std::pair<int, string>& a, const std::pair<int, string>& b)->bool{ return a.first < b.first; } );

    // Take the first entry in the sorted map.
    string move = (string) best->second;

    // string moves[4] = {"up", "down", "left", "right"};

    // Temporarily set a random move. 
    // int index = rand() % 4;
    // string move = moves[index];

    // Print board and chosen move. 
    if(debug) cout << "\n\nBoard at end of computation:" << endl;
    print_board(frame, height, width);
    delete_frame(frame, height, width);
    if(debug) cout << "\nComputation result: Moving " << move << "." << endl;

    // Finally, reply with move: 
    res.set_content("{\"move\": \"" + move + "\"}", "text/plain");
    auto responseTime = (clock() - startTime);
    cout << "Calculation took " << responseTime << " cycles." << endl;
  });

  svr.listen("0.0.0.0", 8080);
}