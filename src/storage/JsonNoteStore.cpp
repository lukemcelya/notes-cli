#include "JsonNoteStore.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

using json = nlohmann::json;

JsonNoteStore::JsonNoteStore(std::filesystem::path path)
  : m_path { std::move(path) }
{ }

NotesState JsonNoteStore::load() const
{
  NotesState state{};

  if (!std::filesystem::exists(m_path))
    return state; // Empty state when no file found

  std::ifstream ifs{m_path};
  if (!ifs)
    throw std::runtime_error("Failed to open file: " + m_path.string());

  json j;
  try
  {
    ifs >> j;
  } catch (const json::parse_error& e)
  {
    throw std::runtime_error(std::string("Invalid JSON in ") + m_path.string() + ": " + e.what());
  }

  // Schema checks
  if (!j.is_object())
    throw std::runtime_error("Notes file root must be a JSON object: " + m_path.string());

  // Version check
  const int version = j.value("version", 1);
  if (version != 1)
    throw std::runtime_error("Invalid JSON version: " + std::to_string(version));

  state.nextId = j.value("nextId", 1);

  if (j.contains("notes"))
  {
    if (!j["notes"].is_array())
      throw std::runtime_error("'notes' must be an array in: " + m_path.string());

    for (const auto& note : j["notes"])
    {
      if (!note.is_object())
        continue; // Might throw later

      if (!note.contains("id") || !note.contains("title") || !note.contains("body"))
        continue;

      const int id = note["id"].get<int>();
      const std::string title = note["title"].get<std::string>();
      const std::string body = note["body"].get<std::string>();

      state.notes.emplace(id, Note{title, body});
    }
  }

  if (!state.notes.empty())
  {
    const int maxId = std::ranges::max_element(state.notes,
      [] (const auto& a, const auto&b){ return a.first < b.first; })->first;

    if (state.nextId <= maxId)
      state.nextId = maxId + 1;
  }
  else
  {
    if (state.nextId < 1) state.nextId = 1;
  }

  return state;
}

void JsonNoteStore::save(const NotesState& state)
{
  json j;
  j["version"] = 1;
  j["nextId"] = state.nextId;

  j["notes"] = json::array();
  for (const auto& [id, note] : state.notes)
  {
    j["notes"].push_back({
      {"id", id},
      {"title", note.title},
      {"body", note.body}
    });
  }

  // Find directory
  if (m_path.has_parent_path())
    std::filesystem::create_directories(m_path.parent_path());

  // temp file writing
  const auto tmpPath = m_path.string() + ".tmp";

  {
    std::ofstream ofs{tmpPath, std::ios::trunc};
    if (!ofs)
      throw std::runtime_error("Failed to open file: " + m_path.string());

    ofs << j.dump(2) << "\n";
    ofs.flush();

    if (!ofs)
      throw std::runtime_error("Failed to write to file: " + m_path.string());
  }

  // Replace tmp
  std::error_code ec;
  std::filesystem::rename(tmpPath, m_path, ec);
  if (ec)
  {
    std::filesystem::remove(m_path, ec);
    ec.clear();
    std::filesystem::rename(tmpPath, m_path, ec);
    if (ec)
      throw std::runtime_error("Failed to rename file: " + m_path.string());
  }
}
