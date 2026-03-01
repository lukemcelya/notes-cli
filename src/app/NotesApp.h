#ifndef NOTESAPP_H
#define NOTESAPP_H

#include "../model/Note.h"

#include <map>
#include <string>

class NotesApp
{
public:
  using NoteId = int;

private:
  std::map<NoteId, Note> m_notes;
  NoteId m_nextId{1};

public:
  NotesApp();
  NotesApp(std::map<NoteId, Note> notes, NoteId nextId);

  [[nodiscard]] const std::map<NoteId, Note>& notes() const;
  [[nodiscard]] NoteId nextId() const;

  void list() const;
  [[nodiscard]] const Note* view(NoteId id) const;
  bool erase(NoteId id);
  NoteId add(std::string title, std::string body);
  bool edit(NoteId id, std::string newTitle,std::string newBody);

  [[nodiscard]] bool isValidId(NoteId id) const;
  [[nodiscard]] size_t getNoteCount() const;

private:
  Note* findMutable(NoteId id);
  [[nodiscard]] const Note* find(NoteId id) const;
};

#endif //NOTESAPP_H