#include "defs.h"

void initHunter(Hunter* hunter, House* house, int numHunt) {
    hunter->id = numHunt;
    //Hunter Id is the number of the hunter -1
    printf("This is Hunter #%d. What should their name be?\n",numHunt+1);
    scanf("%63s", hunter->name);
    hunter->roomIn = NULL;
    hunter->canRead = (EvidenceType)(numHunt);
    hunter->collect = &(house->foundEvidence);
    hunter->fear = 0;
    l_hunterInit(hunter->name,hunter->canRead);
}

void initEvidenceList(EvidenceList* evidenceList){
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
}


void moveToNewRoom(Hunter* hunter, Room* newRoom) {
    if (hunter->roomIn != NULL) {
        hunter->roomIn->huntersInRoom[hunter->id] = NULL;
    }
    newRoom->huntersInRoom[hunter->id] = hunter;
    hunter->roomIn = newRoom;
    l_hunterMove(hunter->name, newRoom->name);
}
