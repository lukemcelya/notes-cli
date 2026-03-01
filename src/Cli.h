#ifndef CPPPREP_CLI_H
#define CPPPREP_CLI_H

#include <string_view>
#include <vector>
#include <string>

class NotesApp;

class Cli
{
private:
  NotesApp& m_app;
  std::string_view m_usagePrefix = "notes ";

public:
  explicit Cli(NotesApp& app);

  int run(int argc, char* argv[]);

private:
  void repl();
  int runAddFlow();
  bool runEditFlow();

  int handleCommand(const std::vector<std::string>& args, bool allowPrompts);
  int handleList() const;
  int handleView(const std::vector<std::string>& args) const;
  int handleAdd(const std::vector<std::string>& args, bool allowPrompts);
  int handleEdit(const std::vector<std::string>& args, bool allowPrompts);
  int handleDelete(const std::vector<std::string>& args) const;

};


#endif //CPPPREP_CLI_H