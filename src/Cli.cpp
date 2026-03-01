#include "Cli.h"
#include "NotesApp.h"

#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <sstream>
#include <optional>

namespace
{
  struct NoteFields
  {
    std::optional<std::string> title;
    std::optional<std::string> body;
  };

  std::vector<std::string> tokenize(const std::string& line);
  std::optional<int> tryParsePositiveInt(const std::string& str);
  std::optional<NoteFields> parseNoteFields(const std::vector<std::string>& args, size_t startIndex);
  void printUsage(std::string_view prefix);

  std::vector<std::string> tokenize(const std::string& line)
  {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string word;

    while (iss >> word)
      tokens.emplace_back(word);

    return tokens;
  }

  std::optional<int> tryParsePositiveInt(const std::string& str)
  {
    std::size_t processedChars = 0;
    int result;

    try
    {
      result = std::stoi(str, &processedChars);
    }
    catch (std::invalid_argument& e)
    {
      return std::nullopt;
    }
    catch (std::out_of_range& e)
    {
      return std::nullopt;
    }

    if (processedChars != str.size() || result <= 0)
      return std::nullopt;

    return result;
  }

  std::optional<NoteFields> parseNoteFields(const std::vector<std::string>& args, const size_t startIndex)
  {
    NoteFields noteFields;
    if (startIndex >= args.size()) return NoteFields{};
    auto isFlag = [](const std::string& s)
    {
      return s.starts_with("-");
    };

    for (size_t i{startIndex}; i < args.size(); ++i)
    {
      if (args[i] == "--title" || args[i] == "-t")
      {
        if (i + 1 >= args.size() || isFlag(args[i + 1]))
        {
          return std::nullopt;
        }

        std::string value;

        size_t j = i + 1;
        while (j < args.size() && !isFlag(args[j]))
        {
          if (!value.empty())
            value += " ";
          value += args[j];
          ++j;
        }

        noteFields.title = std::move(value);
        i = j - 1; // Increment i to current place
      }
      else if (args[i] == "--body" || args[i] == "-b")
      {
        if (i + 1 >= args.size() || isFlag(args[i + 1]))
          return std::nullopt;

        std::string value;

        size_t j = i + 1;
        while (j < args.size() && !isFlag(args[j]))
        {
          if (!value.empty())
            value += " ";
          value += args[j];
          ++j;
        }

        noteFields.body = std::move(value);
        i = j - 1;
      }
      else
      {
        return std::nullopt;
      }
    }

    return noteFields;
  }

  void printUsage(const std::string_view prefix)
  {
    std::cout << "Unknown command\n"
              << "Usage:\n"
              << "  " << prefix << "list\n"
              << "  " << prefix << "view <id>\n"
              << "  " << prefix << "add <title>\n"
              << "  " << prefix << "delete <id>\n";
  }
}

Cli::Cli(NotesApp& app) :
  m_app { app }
{ }

int Cli::run(const int argc, char* argv[])
{
  if (argc == 1)
  {
    repl();
    return 0;
  }

  std::vector<std::string> args;
  for (int i{1}; i < argc; ++i)
    args.emplace_back(argv[i]);

  constexpr bool allowPrompts{false};
  return handleCommand(args, allowPrompts);
}

void Cli::repl()
{
  while (true)
  {
    std::cout << "> " << std::flush;
    std::string line;
    if (!std::getline(std::cin, line))
      break;

    if (line == "quit" || line == "exit")
      break;

    std::vector<std::string> args = tokenize(line);

    if (args.empty()) continue;

    constexpr bool allowPrompts{true};
    handleCommand(args, allowPrompts);
  }
}

int Cli::runAddFlow()
{
  std::string title;
  while (title.empty()) // Title cannot be empty
  {
    std::cout << "Title: ";
    std::getline(std::cin, title);

    if (!title.empty()) break;

    std::cout << "No title specified\n";
  }

  std::string body;

  std::cout << "Body: ";
  std::getline(std::cin, body);

  const auto newNote = m_app.add(std::move(title), std::move(body));
  return newNote;
}

bool Cli::runEditFlow()
{
  std::cout << "ID: ";

  std::string editInput;
  std::getline(std::cin, editInput);

  const auto editId = tryParsePositiveInt(editInput);
  if (!editId || !m_app.isValidId(*editId))
  {
    std::cout << "Invalid id\n";
    return false;
  }

  std::string title;
  std::cout << "Title: ";
  std::getline(std::cin, title);

  std::string body;
  std::cout << "Body: ";
  std::getline(std::cin, body);

  return m_app.edit(*editId, std::move(title), std::move(body));
}

int Cli::handleCommand(const std::vector<std::string>& args, const bool allowPrompts)
{
  if (args.empty())
  {
    std::cout << "No command\n";
    return 1;
  }

  const std::string& command = args[0];

  if (command == "list") return handleList();
  if (command == "view") return handleView(args);
  if (command == "add") return handleAdd(args, allowPrompts);
  if (command == "edit") return handleEdit(args, allowPrompts);
  if (command == "delete") return handleDelete(args);

  printUsage(m_usagePrefix);

  return 1;
}

int Cli::handleList() const
{
  if (m_app.getNoteCount() == 0)
  {
    std::cout << "No notes to display.\n";
    return 1;
  }

  m_app.list();
  return 0;
}

int Cli::handleView(const std::vector<std::string>& args) const
{
  if (args.size() <= 1)
  {
    printUsage(m_usagePrefix);
    return 1;
  }

  const auto maybeId = tryParsePositiveInt(args[1]);
  if (!maybeId)
  {
    std::cout << "Invalid id\n";
    return 1;
  }

  const auto note = m_app.view(*maybeId);
  if (!note)
  {
    std::cout << "Not found\n";
    return 1;
  }

  std::cout << note->title << "\n"
            << note->body << "\n";
  return 0;
}

int Cli::handleAdd(const std::vector<std::string>& args, bool allowPrompts)
{
  if (args.size() <= 1)
    allowPrompts = true;

  int newId;
  if (allowPrompts)
  {
    newId = runAddFlow();
  }
  else
  {
    constexpr size_t FieldsStartIndex = 1;
    auto fields = parseNoteFields(args, FieldsStartIndex);
    if (!fields || !fields->title || !fields->body)
    {
      std::cout << "Missing required fields\n";
      return 1;
    }
    newId = m_app.add(std::move(*fields->title), std::move(*fields->body));
  }

  std::cout << "Added " << newId << "\n";
  return 0;
}

int Cli::handleEdit(const std::vector<std::string>& args, bool allowPrompts)
{
  bool noteEdited = false;

  if (args.size() <= 1)
    allowPrompts = true;

  if (allowPrompts)
  {
    noteEdited = runEditFlow();
  }
  else
  {
    constexpr size_t IdIndex = 1;
    constexpr size_t FieldsStartIndex = 2;

    auto id = tryParsePositiveInt(args[IdIndex]);
    auto fields = parseNoteFields(args, FieldsStartIndex);

    if (!id)
    {
      std::cout << "Invalid id\n";
      return 1;
    }

    if (!fields)
    {
      std::cout << "Invalid field syntax. Use --title <text> and/or --body <text>\n";
      return 1;
    }

    if (!fields->title && !fields->body)
    {
      std::cout << "Provide --title and/or --body\n";
      return 1;
    }

    noteEdited = m_app.edit(*id, std::move(fields->title), std::move(fields->body));
  }

  if (!noteEdited)
  {
    std::cout << "Could not edit note\n";
    return 1;
  }

  std::cout << "Edited\n";
  return 0;
}

int Cli::handleDelete(const std::vector<std::string>& args) const
{
  if (args.size() <= 1)
  {
    printUsage(m_usagePrefix);
    return 1;
  }

  const auto maybeId = tryParsePositiveInt(args[1]);
  if (!maybeId)
  {
    std::cout << "Invalid id\n";
    return 1;
  }

  if (const auto wasDeleted = m_app.erase(*maybeId); !wasDeleted)
  {
    std::cout << "Not found\n";
    return 1;
  }
  std::cout << "Deleted\n";
  return 0;
}