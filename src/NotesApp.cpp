#include "../src/NotesApp.h"

#include <map>
#include <string>
#include <optional>
#include <iostream>
#include <ranges>

NotesApp::NotesApp()
  : m_notes{
    {1, {1, "A", "Test"}},
    {2, {2, "B", "Hello"}},
    {3, {3, "C", "World"}}
    },
    m_nextId { 4 }
{ }

void NotesApp::list() const
{
  if (m_notes.empty())
  {
    std::cout<< "No notes.\n";
    return;
  }

  for (const auto& val : m_notes | std::views::values)
    std::cout << val.id << ": " << val.title << "\n";
}

const NotesApp::Note* NotesApp::view(const int id) const
{
  return find(id);
}

bool NotesApp::erase(const int id)
{
  if (const auto noteToErase = find(id); !noteToErase)
    return false;

  m_notes.erase(id);
  setNextId();
  return true;
}

int NotesApp::add(std::string title, std::string body)
{
  const auto newNoteId = m_nextId; // Saving the id to return after increment
  m_notes[newNoteId] = Note{newNoteId, std::move(title), std::move(body)};
  setNextId();

  return newNoteId;
}

bool NotesApp::edit(const int id, std::optional<std::string> newTitle, std::optional<std::string> newBody)
{
  const auto noteToEdit = findMutable(id);
  if (!noteToEdit)
    return false;

  if (newTitle) noteToEdit->title = std::move(*newTitle);
  if (newBody) noteToEdit->body = std::move(*newBody);
  return true;
}

bool NotesApp::isValidId(const int id) const
{
  if (const auto noteToCheck = find(id); !noteToCheck)
    return false;

  return true;
}

size_t NotesApp::getNoteCount() const
{
  return m_notes.size();
}

NotesApp::Note* NotesApp::findMutable(const int id)
{
  const auto it = m_notes.find(id);
  if (it == m_notes.end())
    return nullptr;

  return &it->second;
}

const NotesApp::Note* NotesApp::find(const int id) const
{
  const auto it = m_notes.find(id);
  if (it == m_notes.end())
    return nullptr;

  return &it->second;
}

int NotesApp::findMaxId() const
{
  int maxId = 0;
  for (const auto& val : m_notes | std::views::values)
    if (val.id > maxId)
      maxId = val.id;

  return maxId;
}

void NotesApp::setNextId()
{
  m_nextId = findMaxId() + 1;
}