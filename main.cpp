#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
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


void print_board(int** arr, int height, int width){
  // Prints arr[height][width]
  cout << "\nT:\tGame Board:" << endl;
  for(int y=height-1; y>=0; y--){
    cout << y << ":\t";
    for(int x=0; x<width; x++){
      cout << arr[y][x] << "\t";
    }
    cout << endl;
  }

  cout << endl;
  cout << "x:\t";
  for(int x=0; x<width; x++){
    cout << x << "\t";
  }
  cout << endl;
}

void initialize(int** arr, int height, int width){
  for(int y=height-1; y>=0; y--){
    for(int x=0; x<width; x++){
      arr[y][x] = 0; 
    }
  }
}

void delete_frame(int** arr, int height, int width){
  for(int y=height-1; y>=0; y--){
    delete[] arr[y];
  }
  delete[] arr;
}

void change_block(int** arr, int height, int width, int ix, int iy, int delta){
  for(int y=iy-1; y <= iy + 1; y++){
    for(int x=ix-1; x <= ix+1; x++){
      if(x >= 0 && x <= width -1){
        if(y >= 0 && y <= height -1){
          cout << "Writing "<<delta<<" to "<<x<<","<<y<<"."<<endl;
          arr[y][x] += delta;
        }
      }
    }
  }
}

void change_cell(int** arr, int height, int width, int ix, int iy, int delta){
    if(ix >= 0 && ix <= width -1){
      if(iy >= 0 && iy <= height -1){
          cout << "Writing "<<delta<<" to "<<ix<<","<<iy<<"."<<endl;
      arr[iy][ix] += delta;
    }
  }
}


/**
 * main
 */

int main(void) {
  // Announce Awakening
  cout << "Guten Morgen!" << endl;

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
    // https://github.com/nlohmann/json
    json data = json::parse(req.body);
    cout << "\n\nPerforming Move Operation. Turn " << data["turn"] << "." << endl;
    cout << "ID: " << data["game"]["id"] << endl;
    cout << "\n" << data << endl;
    json board = data["board"];
    json snakes = board["snakes"];
    json you = data["you"];

    // Store Board Properties
    int width = board["width"];
    int height = board["height"];

    int** frame = new int*[height];
    for(int i=0; i<height; i++)
      frame[i] = new int[width];

    // Set bottom-right and top-left corners.
    initialize(frame, height, width);
    change_block(frame, height, width, 0, 0, -1);
    change_cell(frame, height, width, 0, 0, -1);

    cout << "\n\nBoard at start of computation:" << endl;
    print_board(frame, height, width);
    


    string moves[4] = {"up", "down", "left", "right"};

    // Temporarily set a random move. 
    int index = rand() % 4;
    string move = moves[index];

    // Print board and chosen move. 
    cout << "\n\nBoard at end of computation:" << endl;
    print_board(frame, height, width);
    delete_frame(frame, height, width);
    cout << "\nComputation result: Moving " << move << "." << endl;

    // Finally, reply with move: 
    res.set_content("{\"move\": \"" + move + "\"}", "text/plain");
  });

  svr.listen("0.0.0.0", 8080);
}