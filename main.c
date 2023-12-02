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

    printResults(&house);
    cleanupHouse(&house);

    return 0;
}

void startHunt(House *house, Ghost* ghost){
    //set ghost in random room, not in van
    initGhost(ghost, house);

    //then add hunters to the van
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        initHunter(&(house->huntersInHouse[i]), house, i);
        house->huntersInHouse[i].roomIn = house->rooms.head->data;
    }

    
}

void* ghost_thread(void* arg){
    Ghost* ghost = (Ghost*)arg;
    int shouldContinue = 1;

    while(C_TRUE){
        sem_wait(&ghost->roomIn->semaphore);
        shouldContinue = 1;
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
                //maybe shift sems to insde drop ev
                    dropEvidence(ghost);
                    shouldContinue = 0;
                    break;
                case 2:
                //maybe shift sems to insde should continue
                    Room* selectRoom;
                    selectRoom = selectRandomRoom(&(ghost->roomIn->roomlist));
                    shouldContinue = moveGhost(ghost, selectRoom);
                    break;
                default:
                    //empty case
                    break;
            }
            
        }
        sem_post(&ghost->roomIn->semaphore);
        if(ghost->boredom >= BOREDOM_MAX){
            ghost->roomIn->ghostInRoom = NULL;
            l_ghostExit(LOG_BORED);
            pthread_exit(NULL);
        }
    }
}

void* hunter_thread(void* arg){
    Hunter* hunter = (Hunter*) arg;
    int shouldContinue = 1;
    int winCondition = 0;

    while(C_TRUE){
        sem_wait(&hunter->roomIn->semaphore);

        shouldContinue = 1;
        usleep(HUNTER_WAIT);
        checkIfSameRoom(hunter);

        while(shouldContinue){
            int choice = randInt(0, NUM_HUNTER_CHOICES);
            switch(choice){
                case 0:
                    Room* selectRoom;
                    selectRoom = selectRandomRoom(&(hunter->roomIn->roomlist));
                    moveToNewRoom(hunter, selectRoom);
                    shouldContinue = 0;
                    break;
                case 1:
                    sem_wait(&(hunter->collect->semaphore));
                    checkForEv(hunter);
                    sem_post(&(hunter->collect->semaphore));
                    shouldContinue = 0;
                    break;
                case 2:
                    sem_wait(&(hunter->collect->semaphore));
                    winCondition = evReview(hunter);
                    sem_post(&(hunter->collect->semaphore));
                    shouldContinue = 0;
                    break;
                default:
                    break;
            }
        }
        sem_post(&hunter->roomIn->semaphore);
        if(hunter->boredom >= BOREDOM_MAX || hunter->fear >= FEAR_MAX || winCondition == 1){
            if(hunter->boredom >= BOREDOM_MAX){
                l_hunterExit(hunter->name, LOG_BORED);
            } else if(hunter->fear >= FEAR_MAX){
                l_hunterExit(hunter->name, LOG_FEAR);
            } else{
                hunter->winCondition = 1;
                l_hunterExit(hunter->name, LOG_EVIDENCE);
            }
            leaveHouse(hunter);
            pthread_exit(NULL);
        }
    }
}