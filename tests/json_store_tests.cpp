#include <doctest/doctest.h>

#include "storage/JsonNoteStore.h"
#include "storage/NotesState.h"
#include "model/Note.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <string>

namespace fs = std::filesystem;

// Create unique temp dir for each test case
static fs::path makeUniqueTempDir()
{
  const fs::path base = fs::temp_directory_path();

  const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 999999);

  fs::path dir = base / ("notesapp-tests-" + std::to_string(now) + "-" + std::to_string(dis(gen)));

  fs::create_directories(dir);
  return dir;
}

TEST_CASE("JsonNoteStore.load returns empty state with missing file")
{
  const fs::path dir = makeUniqueTempDir();
  const fs::path file = dir / "notes.json";

  JsonNoteStore store(file);

  NotesState state = store.load();

  CHECK(state.notes.empty());
  CHECK(state.nextId == 1);

  fs::remove_all(dir);
}

TEST_CASE("JsonNoteStore.save then load round-trips notes and nextId")
{
  const fs::path dir = makeUniqueTempDir();
  const fs::path file = dir / "notes.json";

  JsonNoteStore store(file);

  NotesState out{};
  out.nextId = 42;
  out.notes.emplace(1, Note{"Title 1", "Body 1"});
  out.notes.emplace(7, Note{"Title 7", "Body 7\n with newline"});

  store.save(out);

  NotesState in = store.load();

  CHECK(in.nextId == 42);
  REQUIRE(in.notes.size() == 2);

  REQUIRE(in.notes.count(1) == 1);
  CHECK(in.notes.at(1).title == "Title 1");
  CHECK(in.notes.at(1).body == "Body 1");

  REQUIRE(in.notes.count(7) == 1);
  CHECK(in.notes.at(7).title == "Title 7");
  CHECK(in.notes.at(7).body == "Body 7\n with newline");

  fs::remove_all(dir);
}

TEST_CASE("JsonNoteStore.load repairs nextId if it's too small")
{
  const fs::path dir = makeUniqueTempDir();
  const fs::path file = dir / "notes.json";

  {
    std::ofstream out(file);
    REQUIRE(out.good());
    out <<
      "{\n"
      "  \"version\": 1,\n"
      "  \"nextId\": 3,\n"
      "  \"notes\": [\n"
      "    {\"id\": 2, \"title\": \"A\", \"body\": \"a\"},\n"
      "    {\"id\": 10, \"title\": \"B\", \"body\": \"b\"}\n"
      "  ]\n"
      "}\n";
  }

  JsonNoteStore store(file);
  NotesState state = store.load();

  CHECK(state.nextId == 11);

  fs::remove_all(dir);
}

TEST_CASE("JsonNoteStore.load throws on invalid JSON")
{
  const fs::path dir = makeUniqueTempDir();
  const fs::path file = dir / "notes.json";

  {
    std::ofstream out(file);
    REQUIRE(out.good());
    out <<
      "{ invalid json file";
  }

  JsonNoteStore store(file);

  CHECK_THROWS_AS(store.load(), std::runtime_error);

  fs::remove_all(dir);
}