#include "defs.h"

int main()
{ 
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
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
        addHunterToRoom(house->totalRoomList.head->data, house->huntersInHouse[i]);
    }

    int randRoom = randInt(1,house->totalRoomList.count);
    RoomNode* currRoomNode = house->totalRoomList.head;

    for (int i = 0; i < randRoom; ++i) {
        currRoomNode = currRoomNode->next;
    }

    ghost->room = currRoomNode->data;
    l_ghostInit(ghost->ghostType, ghost->room->name);
}