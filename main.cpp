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
      cout << arr[y][x] << " ";
    }
    cout << endl;
  }

  cout << endl;
  cout << "x:\t";
  for(int x=0; x<width; x++){
    cout << x << " ";
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
    frame[0][0] = 1;
    frame[height-1][width-1] = 2;

    print_board(frame, height, width);
    


    string moves[4] = {"up", "down", "left", "right"};

    // Temporarily set a random move. 
    int index = rand() % 4;
    string move = moves[index];

    // Finally, reply with move: 
    res.set_content("{\"move\": \"" + move + "\"}", "text/plain");
  });

  svr.listen("0.0.0.0", 8080);
}