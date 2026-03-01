#include "app/NotesApp.h"
#include "cli/Cli.h"
#include "storage/JsonNoteStore.h"

int main(const int argc, char* argv[])
{
  JsonNoteStore store("notes.json");

  auto [notes, nextId] = store.load();
  NotesApp app(std::move(notes), nextId);

  const Cli cli(app);
  const int returnCode = cli.run(argc, argv);

  store.save(NotesState(app.notes(), app.nextId()));
  return returnCode;
}
