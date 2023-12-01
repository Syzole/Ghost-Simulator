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
    hunter->boredom = 0;
    l_hunterInit(hunter->name,hunter->canRead);
}

void initEvidenceList(EvidenceList* evidenceList){
    sem_init(&(evidenceList->semaphore), 0, 1);
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
}


void moveToNewRoom(Hunter* hunter, Room* newRoom) {
    
    if (hunter->roomIn != NULL) {
        hunter->roomIn->numHuntersInRoom--;
        hunter->roomIn->huntersInRoom[hunter->id] = NULL;
        sem_post(&hunter->roomIn->semaphore);
    }
    sem_wait(&newRoom->semaphore);
    newRoom->huntersInRoom[hunter->id] = hunter;
    hunter->roomIn = newRoom;
    newRoom->numHuntersInRoom++;
    sem_wait(&(hunter->collect->semaphore));
    l_hunterMove(hunter->name, newRoom->name);
    sem_post(&newRoom->semaphore);

    sem_post(&(hunter->collect->semaphore));
}

void checkForEv(Hunter* hunter) {
    //start at head
    EvidenceNode* currentEvidence = hunter->roomIn->ev.head;
    EvidenceNode* previousEvidence = NULL;
    sem_wait(&(hunter->collect->semaphore));
    while (currentEvidence != NULL) {
        //if evidence matches
        if (hunter->canRead == currentEvidence->evType) {
            EvidenceType collectedEvidence = hunter->canRead;

            addEvidenceToEvidenceList(hunter->collect, collectedEvidence);

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
            sem_post(&(hunter->collect->semaphore));
            return;
        }
        //go next
        previousEvidence = currentEvidence;
        currentEvidence = currentEvidence->next;
    }
    // failed evidence collection
    l_hunterCollect(hunter->name, EV_UNKNOWN, hunter->roomIn->name);
    sem_post(&(hunter->collect->semaphore));
    
}

int evReview(Hunter* hunter){
    EvidenceNode* currEv = hunter->collect->head;
    EvidenceNode* prevEv = NULL;
    int unique = 0;
    int uniqueEvs[ALLOWED_EVIDENCE];
    //go through the hunters' evidence list until the end (NULL), checking for 3 unique types of evidence
    sem_wait(&(hunter->collect->semaphore));
    for(int i = 0; i < EVIDENCE_TYPES; i++){
        while(currEv != NULL){
            if(currEv->evType == (enum EvidenceType)i){
                uniqueEvs[unique] = (enum EvidenceType)i;
                unique++;
            }
            prevEv = currEv;
            currEv = currEv->next;
        }
        currEv = hunter->collect->head;
        prevEv = NULL;
    }

    if(unique > 3){ //
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        //printf("sufficient %s", hunter->name);
        sem_post(&(hunter->collect->semaphore));
        return C_TRUE; // pretty sure this is 1
    } else{
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
        sem_post(&(hunter->collect->semaphore));
        return C_FALSE;
        // REMEMBER TO FIND RESULTING GHOST CLASS
    }

}