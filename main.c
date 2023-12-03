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

    startHunt(&house);
  
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

/*
    Initializes the ghost and hunters, adding them to rooms in
    the house.
        in/out: house - the house who is receiving the hunters and ghost.
*/

void startHunt(House *house){
    //set ghost in random room, not in van
    initGhost(&(house->ghost), house);

    //then add hunters to the van
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        initHunter(&(house->huntersInHouse[i]), house, i);
        house->huntersInHouse[i].roomIn = house->rooms.head->data;
    }

    
}


/*
    Controls the ghost thread, including the ghost's boredom, choices
    (do nothing, drop evidence, and move to a new room). If the ghost
    gets too bored, the thread exits. A semaphore is used to lock the
    room it's in and unlock it after its turn is over. It is removed from
    the room it's in after leaving the house and logging this.
        in/out: arg - the ghost whose members are being modified and checked.
*/
void* ghost_thread(void* arg){
    Ghost* ghost = (Ghost*)arg;
    int shouldContinue = 1;

    while(C_TRUE){
        shouldContinue = 1;
        usleep(GHOST_WAIT);
        if(roomHasHunters(ghost->roomIn)){
            ghost->boredom = 0;
        } else{
            ghost->boredom++;
        }

        sem_wait(&ghost->roomIn->semaphore);
        
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

/*
    Controls a hunter thread, including the hunter's boredom, choices
    (move to a new room, check for evidence to collect, and review collected
    evidence). If the hunter gets too bored, afraid, or reviews that there is
    enough evidence, the thread exits. A semaphore is used to lock the
    room it's in and unlock it after its turn is over. It is removed from
    the room it's in after leaving the house and logging this.
        in/out: arg - the hunter whose members are being modified and checked.
*/

void* hunter_thread(void* arg){
    Hunter* hunter = (Hunter*) arg;
    int shouldContinue = 1;
    int winCondition = 0;

    while(C_TRUE){
        shouldContinue = 1;
        usleep(HUNTER_WAIT);
        checkIfSameRoom(hunter);

        sem_wait(&hunter->roomIn->semaphore);

        

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
                    sem_wait(&(hunter->roomIn->ev.semaphore));
                    checkForEv(hunter);
                    sem_post(&(hunter->roomIn->ev.semaphore));
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