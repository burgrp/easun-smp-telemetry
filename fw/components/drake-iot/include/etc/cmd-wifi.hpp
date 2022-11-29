class WiFiConnectCommand : public Command {
public:
  // WiFi *wifi;
  WiFiConnectCommand() : Command("connect", "help", "hint") {}
  int run(vector<string> &args) {
    ESP_LOGI(TAG, "CONNECT");
    return 0;
  };
};

class WiFiCommand : public CompoundCommand {
public:
  // WiFiConnectCommand connect;
  WiFiCommand(WiFi *wifi) : CompoundCommand("wifi", "configures WiFi", {}) {}
};