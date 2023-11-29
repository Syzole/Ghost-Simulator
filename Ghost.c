#include "defs.h"

void initGhost(Ghost* ghost) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->roomIn = NULL;
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
    }

    room->ghostInRoom = ghost;
    ghost->roomIn = room;
    //did ghost stay in his place, no so return false
    return C_FALSE;
}

void doNothing(){
    //do nothing
}

void dropEvidence(Ghost* ghost){
    
}