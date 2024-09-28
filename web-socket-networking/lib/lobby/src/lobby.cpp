
#include "lobby.h"

int nextLobbyId = 1;

Lobby::Lobby()
:lobbyNumber(nextLobbyId++), inviteCode(tokenGenerator())
{

}

Lobby::~Lobby(){
    std::cout << "yay u deleted the structure" << std::endl;
}

char Lobby::randLetter(){
  const std::string INVITE_CHAR = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
  
  return INVITE_CHAR[rand() % INVITE_CHAR.length()];
}

int Lobby::randDigit(){
  return rand() % 10;
}

std::string Lobby::tokenGenerator(){
  std::string token;
  for(int i = 0; i < 3; i++){
    token += std::to_string(randDigit())+ randLetter();
  }
  return token;
}