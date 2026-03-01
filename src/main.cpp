#include "app/NotesApp.h"
#include "cli/Cli.h"

int main(const int argc, char* argv[])
{
  NotesApp app;
  const Cli cli(app);

  return cli.run(argc, argv);
}
