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

void checkForEv(Hunter* hunter) {
    if (hunter->roomIn != NULL) {
        //start at head
        EvidenceNode* currentEvidence = hunter->roomIn->ev.head;
        EvidenceNode* previousEvidence = NULL;

        while (currentEvidence != NULL) {
            //if evidnce matches
            if (hunter->canRead == currentEvidence->evType) {
                EvidenceType collectedEvidence = hunter->canRead;

                addEvidenceToCollection(hunter, collectedEvidence);

                l_hunterCollect(hunter->name, collectedEvidence, hunter->roomIn->name);

                // edge case if removed at head
                if (previousEvidence != NULL) {
                    previousEvidence->next = currentEvidence->next;
                } 
                else {
                    hunter->roomIn->ev.head = currentEvidence->next;
                }

                // edge case, if removed at tail
                if (currentEvidence == hunter->roomIn->ev.tail) {
                    hunter->roomIn->ev.tail = previousEvidence;
                }

                // free the mem
                free(currentEvidence);

                return;
            }
            //go next
            previousEvidence = currentEvidence;
            currentEvidence = currentEvidence->next;
        }
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    }
}

void addEvidenceToCollection(Hunter* hunter, EvidenceType evidenceType){

    EvidenceNode* newEvidence = (EvidenceNode*)malloc(sizeof(EvidenceNode));

    newEvidence->evType = evidenceType;
    newEvidence->next = NULL;

    if (hunter->collect->head == NULL) {
        hunter->collect->head = newEvidence;
        hunter->collect->tail = newEvidence;
    } 
    else {
        hunter->collect->tail->next = newEvidence;
        hunter->collect->tail = newEvidence;
    }
}