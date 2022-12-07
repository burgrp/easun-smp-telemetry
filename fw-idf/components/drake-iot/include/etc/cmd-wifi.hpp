class WiFiConnectCommand : public SimpleCommand {
public:
  WiFi *wifi;
  WiFiConnectCommand(WiFi *wifi) : SimpleCommand("connect", "Connect to AP", "ssid password"), wifi(wifi) {}
  int run(vector<string> &args) {
    ESP_LOGI(TAG, "CONNECT %s %s", args[0].c_str(), args[1].c_str());
    return 0;
  };
};

class WiFiDisconnectCommand : public SimpleCommand {
public:
  WiFi *wifi;
  WiFiDisconnectCommand(WiFi *wifi) : SimpleCommand("disconnect", "Disconnect from AP", ""), wifi(wifi) {}
  int run(vector<string> &args) {
    ESP_LOGI(TAG, "DISCONNECT");
    return 0;
  };
};

class WiFiCommand : public CompoundCommand {
public:
  WiFiConnectCommand connect;
  WiFiDisconnectCommand disconnect;
  WiFiCommand(WiFi *wifi) : CompoundCommand("wifi", "Configure WiFi", {
      &connect,
      &disconnect
  }), connect(wifi), disconnect(wifi) {}
};