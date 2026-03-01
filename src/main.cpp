#include "../src/NotesApp.h"
#include "../src/Cli.h"

#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

int main(const int argc, char* argv[])
{
  NotesApp app;
  Cli cli(app);

  json j;
  j["Luke"] = "McElya";
  j["Age"] = 26;

  std::cout << j.dump(2) << "\n";

  return cli.run(argc, argv);
}
