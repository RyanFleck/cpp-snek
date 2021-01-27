#include <iostream>
#include <cstdlib>
#include <ctime>
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
    cout << "\n\n" << data << endl;
    json board = data["board"];
    json snakes = board["snakes"];
    json you = data["you"];

    // Store Board Properties
    int width = board["width"];
    int height = board["height"];


    string moves[4] = {"up", "down", "left", "right"};
    string move = "right";
    res.set_content("{\"move\": \"" + move + "\"}", "text/plain");
  });

  svr.listen("0.0.0.0", 8080);
}