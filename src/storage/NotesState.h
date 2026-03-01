#ifndef NOTESSTATE_H
#define NOTESSTATE_H

#include "../model/Note.h"

#include <map>

struct NotesState
{
  std::map<int, Note> notes;
  int nextId = 1;
};

#endif //NOTESSTATE_H