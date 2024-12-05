
#include "Lobby.h"
#include "User.h"
#include <algorithm>
#include <spdlog/spdlog.h>

static unsigned int nextLobbyId = 0;

Lobby::Lobby() : inviteCode(tokenGenerator()), lobbyNumber(nextLobbyId++)
{
  spdlog::debug("Created a new lobby #{}", lobbyNumber);
}

Lobby::~Lobby()
{
  spdlog::debug("Deleted lobby #{}", lobbyNumber);
  std::for_each(userIDs.begin(), userIDs.end(), [](unsigned int num){
    spdlog::debug("Kicked player #{}, to reception", num);
  });
}

char Lobby::randLetter()
{
  const std::string INVITE_CHAR = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

  return INVITE_CHAR[rand() % INVITE_CHAR.length()];
}

int Lobby::randDigit()
{
  return rand() % 10;
}

std::string Lobby::tokenGenerator()
{
  std::string token;
  for (int i = 0; i < 3; i++)
  {
    token += std::to_string(randDigit()) + randLetter();
  }
  return token;
}

void Lobby::addUser(unsigned int userID){
  userIDs.push_back(userID);
}

void Lobby::removeUser(unsigned int userID){
  auto toRemove = std::remove(userIDs.begin(), userIDs.end(), userID);
  userIDs.erase(toRemove, userIDs.end());
}

std::vector<unsigned int> Lobby::getUsers() const{
  return userIDs;
}

unsigned int Lobby::getLobbyNum() const
{
  return lobbyNumber;
}

bool Lobby::operator==(const Lobby& other) const{
  return other.lobbyNumber == this->lobbyNumber;
}