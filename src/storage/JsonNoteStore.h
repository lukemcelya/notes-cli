#ifndef JSONNOTESTORE_H
#define JSONNOTESTORE_H

#include <filesystem>
#include "NotesState.h"

class JsonNoteStore
{
public:
  explicit JsonNoteStore(std::filesystem::path path);

  [[nodiscard]] NotesState load() const;
  void save(const NotesState& state);

private:
  std::filesystem::path m_path;
};

#endif //JSONNOTESTORE_H