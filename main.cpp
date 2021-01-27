#include <iostream>
#include "./http.h"
using namespace std;

httplib::Server svr;

int main(void) {
  cout << "Guten Morgen!" << endl;

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
    //get json data
    string data = req.body;
    cout << "\n\n" + data;
    string moves[4] = {"up", "down", "left", "right"};
    string move = "right";
    res.set_content("{\"move\": \"" + move + "\"}", "text/plain");
  });



  svr.listen("0.0.0.0", 8080);
}