#include "defs.h"

void initGhost(Ghost* ghost) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->room = NULL;
    //l_ghostInit(ghost->ghostType, ghost->room->name);
}

int moveGhost(Ghost* ghost, Room* room){

}