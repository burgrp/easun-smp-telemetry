#include "esp_console.h"
#include <string.h>

class Console;
class Command;

// TODO: composite key (console, command-name) to allow multiple consoles
// ...which will never happen...
static map<string, Command &> allCommands;

static int cmdHandler(int argc, char **argv);

class Command {
  string name;
  string help;
  string hint;

public:
  Command() {}

  void attach(Console &console) {
    name = getName();
    help = getHelp();
    hint = getHint();

    allCommands.insert_or_assign(name, *this);

    esp_console_cmd_t cmd = {.command = name.c_str(),
                             .help = help.c_str(),
                             .hint = hint.c_str(),
                             .func = &cmdHandler,
                             .argtable = NULL};

    ESP_THROW(esp_console_cmd_register(&cmd));
  };

  virtual string &getName() = 0;
  virtual string &getHelp() = 0;
  virtual string &getHint() = 0;
  virtual int run(vector<string> &args) = 0;
};

class SimpleCommand : public Command {
public:
  string name;
  string help;
  string hint;
  vector<Command *> commands;
  SimpleCommand(string name, string help, string hint)
      : Command(), name(name), help(help), hint(hint) {}

  string &getName() { return name; };
  string &getHelp() { return help; };
  string &getHint() { return hint; };
};

class CompoundCommand : public Command {
public:
  string name;
  string help;
  string hint;
  vector<Command *> commands;
  CompoundCommand(string name, string help, vector<Command *> commands)
      : Command(), name(name), help(help), commands(commands) {}

  string &getName() { return name; };

  string &getHelp() { return help; };

  string &getHint() {
    if (hint.empty()) {
      hint = "";
      for (auto command : commands) {
        if (!hint.empty()) {
          hint = hint + "|";
        }
        hint = hint + command->getName();
      }
    }
    return hint;
  };

  virtual void showHelp() {
    //cout << "HELP";

    
  }

  int run(vector<string> &args) {
    if (args.empty()) {
      showHelp();
      return 0;
    } else {
      auto begin = args.begin();
      string subcommand = *begin;
      args.erase(begin);
      for (auto command : commands) {
        if (!command->getName().compare(subcommand)) {
          return command->run(args);
        }
      }
      //cout << "Unknown subcommand: " << subcommand;
      showHelp();
      return 1;
    }
  };
};

class Console {
public:
  void start() {
    ESP_LOGI(TAG, "Starting console...");

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = ">";

    esp_console_register_help_command();

    esp_console_dev_usb_serial_jtag_config_t hw_config =
        ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();

    ESP_THROW(
        esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));

    ESP_THROW(esp_console_start_repl(repl));
  }
};

int cmdHandler(int argc, char **argv) {
  string name = argv[0];

  try {
    Command &command = allCommands.at(name);
    vector<string> args;
    for (int c = 1; c < argc; c++) {
      args.push_back(argv[c]);
    }
    return command.run(args);
  } catch (exception *e) {
    ESP_LOGE(TAG, "Command %s failed: %s", argv[0], e->what());
    return 1;
  }
};
