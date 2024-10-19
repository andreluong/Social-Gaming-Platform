
#include "Lobby.h"
#include "User.h"
#include <algorithm>

static unsigned int nextLobbyId = 0;

Lobby::Lobby() : inviteCode(tokenGenerator()), lobbyNumber(nextLobbyId++)
{
  std::cout << "yay u created the lobby " << lobbyNumber << std::endl;
}

Lobby::~Lobby()
{
  std::cout << "yay u deleted the lobby " << lobbyNumber << std::endl;
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

void Lobby::addUser(User *user)
{
  users.push_back(user);
}

void Lobby::removeUser(User *user)
{
  auto userErase = std::remove(users.begin(), users.end(), user);
  users.erase(userErase);
}

std::vector<User *> Lobby::getUsers()
{
  return users;
}

unsigned int Lobby::getLobbyNum() const
{
  return lobbyNumber;
}

bool Lobby::operator==(const Lobby& other){
  return other.lobbyNumber == this->lobbyNumber;
}