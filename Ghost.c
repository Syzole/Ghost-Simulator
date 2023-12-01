#include "defs.h"

void initGhost(Ghost* ghost, House* house) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->roomIn = NULL;
    ghost->house = house;
    //switch cases to check what ev the ghost can drop
    switch (ghost->ghostType) {
        case POLTERGEIST:
            ghost->allowedEvidence[0] = EMF;
            ghost->allowedEvidence[1] = TEMPERATURE;
            ghost->allowedEvidence[2] = FINGERPRINTS;
            break;
        case BANSHEE:
            ghost->allowedEvidence[0] = EMF;
            ghost->allowedEvidence[1] = TEMPERATURE;
            ghost->allowedEvidence[2] = SOUND;
            break;
        case BULLIES:
            ghost->allowedEvidence[0] = EMF;
            ghost->allowedEvidence[1] = FINGERPRINTS;
            ghost->allowedEvidence[2] = SOUND;
            break;
        case PHANTOM:
            ghost->allowedEvidence[0] = TEMPERATURE;
            ghost->allowedEvidence[1] = FINGERPRINTS;
            ghost->allowedEvidence[2] = SOUND;
            break;
        default:
            //this prob wont happen
            break;
    }
    //l_ghostInit(ghost->ghostType, ghost->roomIn->name);
}

int moveGhost(Ghost* ghost, Room* room){
    if(room->numHuntersInRoom>0){
        //is the ghost in the same place, yes so return true
        return C_TRUE;
    }

    if (ghost->roomIn != NULL) {
        ghost->roomIn->ghostInRoom = NULL;
        sem_post(&ghost->roomIn->semaphore);
    }
    sem_wait(&(room->semaphore));
    room->ghostInRoom = ghost;
    ghost->roomIn = room;
    //did ghost stay in his place, no so return false
    l_ghostMove(room->name);
    sem_post(&(room->semaphore));
    return C_FALSE;
}

void doNothing(){
    //do nothing
}

void dropEvidence(Ghost* ghost){
    int randomIndex = randInt(0, 3);

    EvidenceType addedEv = ghost->allowedEvidence[randomIndex];
    //printf("The addev is %d", addedEv);
    addEvidenceToEvidenceList(&(ghost->roomIn->ev), addedEv);

    l_ghostEvidence(addedEv, ghost->roomIn->name);
}