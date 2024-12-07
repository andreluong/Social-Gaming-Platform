#ifndef CHATCLIENTLOGIC_H
#define CHATCLIENTLOGIC_H
#include <optional>
#include <unistd.h>
#include <string>
#include <vector>


enum class MessageType {
    Other = 1,   // Corresponds to color pair 1
    Self = 2,        // Corresponds to color pair 2
    Server = 3,      // Corresponds to color pair 3
    GameState = 4,    // Corresponds to color pair 4
    Error = 5
};

struct ClientData {
  std::string connectionId;
  std::optional<std::string> name = std::nullopt;
};

void processServerMessages(const std::vector<std::string>& messages, ClientData& clientData);

enum MessageType getMessageType(const std::vector<std::string>& messages, const ClientData& clientData);

#endif
