#include "defs.h"

void initGhost(Ghost* ghost) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->roomIn = NULL;
    //l_ghostInit(ghost->ghostType, ghost->roomIn->name);
}

int moveGhost(Ghost* ghost, Room* room){
    // if (ghost->roomIn != NULL) {
    //     ghost->roomIn->ghost
    // }

}