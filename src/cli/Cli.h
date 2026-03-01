#ifndef CLI_H
#define CLI_H

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

  int run(int argc, char* argv[]) const;

private:
  void repl() const;
  [[nodiscard]] int runAddFlow() const;
  [[nodiscard]] bool runEditFlow() const;

  [[nodiscard]] int handleCommand(const std::vector<std::string>& args, bool allowPrompts) const;
  [[nodiscard]] int handleList() const;
  [[nodiscard]] int handleView(const std::vector<std::string>& args) const;
  [[nodiscard]] int handleAdd(const std::vector<std::string>& args, bool allowPrompts) const;
  [[nodiscard]] int handleEdit(const std::vector<std::string>& args, bool allowPrompts) const;
  [[nodiscard]] int handleDelete(const std::vector<std::string>& args) const;
};


#endif //CLI_H