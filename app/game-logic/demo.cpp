#include <iostream>
#include <memory>
#include "include/User.h"

int main() {
    uintptr_t connA(80); 
    uintptr_t connB(81); 
    auto conn = networking::Connection(connA);

    User user(1, "Abby", conn);

    std::cout << "User ID: " << user.getId() << "\n";
    std::cout << "User Name: " << user.getName() << "\n";

    if (user.getConnection() != networking::Connection(connB) &&
        user.getConnection() == networking::Connection(connA)) {
        std::cout << "Connection is 80\n";
    }

    user.setName("Bobby");
    std::cout << "User Name: " << user.getName() << "\n";

    // Create roles
    std::shared_ptr<Role> playerRole = std::make_shared<Player>();
    std::shared_ptr<Role> audienceRole = std::make_shared<Audience>();
    std::shared_ptr<Role> hostRole = std::make_shared<Host>();

    // Just examples, testing is done in test/

    // expect player and host (101)
    user.addRole(playerRole);
    user.addRole(hostRole);
    std::cout << "Has role Player/Audience/Host: " << user.hasRole(RoleType::Player) << user.hasRole(RoleType::Audience) << user.hasRole(RoleType::Host) << "\n";

    // expect audience (010)
    user.swapPlayerAudienceRoles();
    user.removeRole(RoleType::Host);
    std::cout << "Has role Player/Audience/Host: " << user.hasRole(RoleType::Player) << user.hasRole(RoleType::Audience) << user.hasRole(RoleType::Host) << "\n";


    // expect exception thrown because Player and Audience can't coexist
    //user.addRole(playerRole);

   
    return 0;
}
