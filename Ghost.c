#include "defs.h"

void initGhost(Ghost* ghost) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->roomIn = NULL;
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