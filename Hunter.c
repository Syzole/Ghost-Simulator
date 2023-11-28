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

void addHunterToRoom(Room* room, Hunter* hunter) {
    room->huntersInRoom[hunter->id] = hunter;
    hunter->roomIn = room;
    l_hunterMove(hunter->name, room->name);
}

void removeHunterFromRoom(Hunter* hunter) {
    hunter->roomIn->huntersInRoom[hunter->id] = NULL;
    hunter->roomIn = NULL;
}