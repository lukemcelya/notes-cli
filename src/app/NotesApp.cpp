#include "../app/NotesApp.h"
#include "../model/Note.h"

#include <map>
#include <string>
#include <iostream>

NotesApp::NotesApp()
  : m_nextId { 1 }
{ }

NotesApp::NotesApp(std::map<NoteId, Note> notes, NoteId nextId)
  : m_notes { std::move(notes) }
  , m_nextId { nextId }
{ }

const std::map<NotesApp::NoteId, Note>& NotesApp::notes() const
{
  return m_notes;
}

NotesApp::NoteId NotesApp::nextId() const
{
  return m_nextId;
}

void NotesApp::list() const
{
  if (m_notes.empty())
  {
    std::cout<< "No notes.\n";
    return;
  }

  for (const auto& [id, note] : m_notes)
    std::cout << id << ": " << note.title << "\n";
}

const Note* NotesApp::view(const NoteId id) const
{
  return find(id);
}

bool NotesApp::erase(const NoteId id)
{
  if (const auto noteToErase = find(id); !noteToErase)
    return false;

  m_notes.erase(id);

  return true;
}

int NotesApp::add(std::string title, std::string body)
{
  const auto newNoteId = m_nextId; // Saving the id to return after increment
  m_notes.emplace(newNoteId, Note{std::move(title), std::move(body)});

  ++m_nextId;

  return newNoteId;
}

bool NotesApp::edit(const NoteId id, std::string newTitle, std::string newBody)
{
  const auto noteToEdit = findMutable(id);
  if (!noteToEdit)
    return false;

  if (!newTitle.empty()) noteToEdit->title = std::move(newTitle);
  if (!newBody.empty()) noteToEdit->body = std::move(newBody);
  return true;
}

bool NotesApp::isValidId(const NoteId id) const
{
  return find(id) != nullptr;
}

size_t NotesApp::getNoteCount() const
{
  return m_notes.size();
}

Note* NotesApp::findMutable(const NoteId id)
{
  const auto it = m_notes.find(id);
  if (it == m_notes.end())
    return nullptr;

  return &it->second;
}

const Note* NotesApp::find(const NoteId id) const
{
  const auto it = m_notes.find(id);
  if (it == m_notes.end())
    return nullptr;

  return &it->second;
}
