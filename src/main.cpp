#include "../src/NotesApp.h"
#include "../src/Cli.h"

int main(const int argc, char* argv[])
{
  NotesApp app;
  Cli cli(app);
  return cli.run(argc, argv);
}
