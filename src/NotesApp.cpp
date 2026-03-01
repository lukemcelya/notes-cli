#include "../src/NotesApp.h"
#include <nlohmann/json.hpp>

#include <map>
#include <string>
#include <iostream>

NotesApp::NotesApp()
  : m_notes{
    {1, {"A", "Test"}},
    {2, {"B", "Hello"}},
    {3, {"C", "World"}}
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

  for (const auto& [id, note] : m_notes)
    std::cout << id << ": " << note.title << "\n";
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

  return true;
}

int NotesApp::add(std::string title, std::string body)
{
  const auto newNoteId = m_nextId; // Saving the id to return after increment
  m_notes.emplace(newNoteId, Note{std::move(title), std::move(body)});

  ++m_nextId;

  return newNoteId;
}

bool NotesApp::edit(const int id, std::string newTitle, std::string newBody)
{
  const auto noteToEdit = findMutable(id);
  if (!noteToEdit)
    return false;

  if (!newTitle.empty()) noteToEdit->title = std::move(newTitle);
  if (!newBody.empty()) noteToEdit->body = std::move(newBody);
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
