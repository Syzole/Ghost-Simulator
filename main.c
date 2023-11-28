#include "defs.h"

int main()
{ 
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    House house;
    Ghost ghost;

    initGhost(&ghost);

    initHouse(&house);
    populateRooms(&house);

    startHunt(&house, &ghost);

    return 0;
}

void startHunt(House *house,Ghost* ghost){
    //first add hunters to the head
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        moveToNewRoom(house->huntersInHouse[i], house->rooms.head->data);
    }

    int randRoom = randInt(1,house->rooms.count);
    RoomNode* currRoomNode = house->rooms.head;

    for (int i = 0; i < randRoom; ++i) {
        currRoomNode = currRoomNode->next;
    }

    ghost->room = currRoomNode->data;
    l_ghostInit(ghost->ghostType, ghost->room->name);
}