#ifndef CPPPREP_NOTESAPP_H
#define CPPPREP_NOTESAPP_H

#include <map>
#include <string>
#include <optional>

class NotesApp
{
public:
  struct Note
  {
    int id;
    std::string title;
    std::string body;
  };

private:
  std::map<int, Note> m_notes;
  int m_nextId;

public:
  NotesApp();
  void list() const;
  [[nodiscard]] const Note* view(int id) const;
  bool erase(int id);
  int add(std::string title, std::string body);
  bool edit(int id, std::optional<std::string> newTitle, std::optional<std::string> newBody);
  [[nodiscard]] bool isValidId(int id) const;
  [[nodiscard]] size_t getNoteCount() const;

private:
  Note* findMutable(int id);
  [[nodiscard]] const Note* find(int id) const;
  [[nodiscard]] int findMaxId() const;
  void setNextId();
};

#endif //CPPPREP_NOTESAPP_H