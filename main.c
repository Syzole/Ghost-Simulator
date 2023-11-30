#include "defs.h"

int main()
{ 
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    House house;

    initHouse(&house);
    populateRooms(&house);

    startHunt(&house, &(house.ghost));

    pthread_t g, h1, h2, h3, h4;

    /*
    pthread_create();
    pthread_create();
    pthread_create();
    pthread_create();
    pthread_create();
    */

    return 0;
}

void startHunt(House *house, Ghost* ghost){
    //first add hunters to the head
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        moveToNewRoom(&(house->huntersInHouse[i]), house->rooms.head->data);
    }

    int randRoom = randInt(1,house->rooms.count);
    RoomNode* currRoomNode = house->rooms.head;

    for (int i = 0; i < randRoom; ++i) {
        currRoomNode = currRoomNode->next;
    }

    ghost->roomIn = currRoomNode->data;
    currRoomNode->data->ghostInRoom = ghost;
    l_ghostInit(ghost->ghostType, ghost->roomIn->name);
}

void* ghost_thread(void* arg){
    Ghost* ghost = (Ghost*)arg;
    int shouldContinue = 1;

    while(C_TRUE){
        usleep(GHOST_WAIT);
        if(ghost->roomIn->numHuntersInRoom > 0){
            ghost->boredom = 0;
        } else{
            ghost->boredom++;
        }
        while(shouldContinue){
            int choice = randInt(0, NUM_GHOST_CHOICES);
            switch(choice){
                case 0:
                    doNothing();
                    shouldContinue = 0;
                    break;
                case 1:
                    dropEvidence(ghost);
                    shouldContinue = 0;
                    break;
                case 2:
                    Room* selectRoom;
                    selectRoom = selectRandomRoom(&(ghost->roomIn->roomlist));
                    shouldContinue = moveGhost(ghost, selectRoom);
                    break;
                default:
                    // what case would this happen in?
                    break;
            }
        }
    }
}