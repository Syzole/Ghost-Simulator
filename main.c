#include "defs.h"

int main()
{ 
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    House house;

    initHouse(&house); // REMEMBER TO EDIT COMMENTS WHEN NEW FEATURES ARE ADDED TO FUNCTIONS
    populateRooms(&house);

    startHunt(&house, &(house.ghost));

    pthread_t g, h1, h2, h3, h4;

    pthread_create(&g, NULL, ghost_thread, &(house.ghost));
    pthread_create(&h1, NULL, hunter_thread, &(house.huntersInHouse[0]));
    pthread_create(&h2, NULL, hunter_thread, &(house.huntersInHouse[1]));
    pthread_create(&h3, NULL, hunter_thread, &(house.huntersInHouse[2]));
    pthread_create(&h4, NULL, hunter_thread, &(house.huntersInHouse[3]));

    pthread_join(g, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);

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
        
        shouldContinue = 1;
        usleep(GHOST_WAIT);
        if(ghost->roomIn->numHuntersInRoom > 0){
            ghost->boredom = 0;
        } else{
            ghost->boredom++;
        }
        while(shouldContinue){
            sem_wait(&ghost->roomIn->semaphore);
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
                    //empty case
                    break;
            }
            sem_post(&ghost->roomIn->semaphore);
        }
        if(ghost->boredom == BOREDOM_MAX){
            l_ghostExit(LOG_BORED);
            pthread_exit(NULL);
        }
    }
}

void* hunter_thread(void* arg){
    Hunter* hunter = (Hunter*) arg;
    int shouldContinue = 1;
    int winCondition = 0;

    while(C_TRUE){ //y?
        
        shouldContinue = 1;
        usleep(HUNTER_WAIT);
        checkIfSameRoom(hunter);
        while(shouldContinue){
            sem_wait(&hunter->roomIn->semaphore);
            int choice = randInt(0, NUM_HUNTER_CHOICES);
            switch(choice){
                case 0:
                    Room* selectRoom;
                    selectRoom = selectRandomRoom(&(hunter->roomIn->roomlist));
                    moveToNewRoom(hunter, selectRoom);
                    shouldContinue = 0;
                    break;
                case 1:
                    checkForEv(hunter);
                    shouldContinue = 0;
                    break;
                case 2:
                    winCondition = evReview(hunter);
                    shouldContinue = 0;
                    break;
                default:
                    break;
            }
            sem_post(&hunter->roomIn->semaphore);
        }
        
        if(hunter->boredom == BOREDOM_MAX || hunter->fear == FEAR_MAX || winCondition == 1){
            if(hunter->boredom == BOREDOM_MAX){
                l_hunterExit(hunter->name, LOG_BORED);
            } else if(hunter->fear == FEAR_MAX){
                l_hunterExit(hunter->name, LOG_FEAR);
            } else{
                l_hunterExit(hunter->name, LOG_EVIDENCE);
            }
            pthread_exit(NULL);
        }
    }
}