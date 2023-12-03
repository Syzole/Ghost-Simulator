#include "defs.h"

/*
    Initializes the hunter by giving its def
        out: ghost - the ghost whose values are being initialized.
        in: house - the house whose semaphore is being used for the ghost.
*/
void initHunter(Hunter* hunter, House* house, int numHunt) {
    hunter->id = numHunt;
    //Hunter Id is the number of the hunter -1
    printf("\n");
    printf("This is Hunter #%d. What should their name be?",numHunt+1);
    printf("\n");
    scanf("%63s", hunter->name);
    hunter->roomIn = NULL;
    hunter->canRead = (EvidenceType) chooseEvidence(hunter, house);
    hunter->collect = &(house->foundEvidence);
    hunter->fear = 0;
    hunter->boredom = 0;
    hunter->winCondition = 0;
    hunter->houseSemaphore = &(house->houseSemaphore);
    hunter->outfile = house->outfile;
    //l_hunterInit(hunter->name,hunter->canRead, house->outfile);
}

/*
    Initializes the EvidenceList structure by giving its members default values
    and giving it a semaphore so only one thread can access lists of its type at a time.
        out: evidenceList - the list of evidence that is being initialized.
*/
void initEvidenceList(EvidenceList* evidenceList){
    sem_init(&(evidenceList->semaphore), 0, 1);
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
}

/*
    Uses user input to set the type of evidence each hunter can read,
    ensuring that no two hunters have the same type.
        out: hunter - the hunter whose canRead evidence type is being chosen.
        in: house - the house that the list of hunters is stored in- used for iteration.
*/
EvidenceType chooseEvidence(Hunter* hunter, House* house){
    int choice = 0;
    int isNotUnique = 1;
    printf("\n");
    printf("What evidence should Hunter #%d be able to read?",hunter->id+1);
    printf("\n");
    printf("0. EMF\n");
    printf("1. Temperature\n");
    printf("2. Fingerprints\n");
    printf("3. Sound\n");
    printf("\n");
    scanf("%d", &choice);

    while(choice < 0 || choice > 3){
        printf("\n");
        printf("Invalid choice. Please enter a number between 0 and 3.");
        printf("\n");
        scanf("%d", &choice);
    }

    while(isNotUnique){
        for(int i = 0; i < hunter->id; i++){
            if(house->huntersInHouse[i].canRead == choice){
                printf("\n");
                printf("That evidence is already being read by another hunter. Please choose another.");
                printf("\n");
                scanf("%d", &choice);
            }
        }
        isNotUnique = 0;
    }
    switch(choice){
        case 0:
            return EMF;
            break;
        case 1:
            return TEMPERATURE;
            break;
        case 2:
            return FINGERPRINTS;
            break;
        case 3:
            return SOUND;
            break;
        default:
            return EV_UNKNOWN;
            break;
    }
    
}

/*
    Removes the given hunter from its current room, unlocking it, and moves the
    hunter to the given room, locking that one. It also logs this movement.
        in/out: hunter - the hunter who being given a pointer to the new room.
        in/out: newRoom - the room that is being given a pointer to the hunter.
*/
void moveToNewRoom(Hunter* hunter, Room* newRoom) {
    
    hunter->roomIn->huntersInRoom[hunter->id] = NULL;
    sem_post(&hunter->roomIn->semaphore);
    
    sem_wait(&newRoom->semaphore);
    newRoom->huntersInRoom[hunter->id] = hunter;
    hunter->roomIn = newRoom;
    sem_wait(hunter->houseSemaphore);
    l_hunterMove(hunter->name, newRoom->name, hunter->outfile);
    sem_post(hunter->houseSemaphore);

    
}

/*
    Checks whether or not a given piece of evidence is of a unique type compared
    to the others in the given evidence list. 
        in: evidenceList - a pointer to the evidence list being iterated through.
        in: house - the type of evidence that is being looked for.
        returns: true or false depending on whether the given evidence is unique.
*/
int isUnique(EvidenceList* evidenceList, EvidenceType evidenceType){ // might cause segfault
    EvidenceNode* currHEv = evidenceList->head;
    // returns false if the evidence type is found in hunters' shared evidence
    while(currHEv != NULL){
        if(currHEv->evType == evidenceType){
            //printf("not unique %d", evidenceType);
            return C_FALSE;
        }
        currHEv = currHEv->next;
    }
    // returns true if unique
    //printf("unique %d", evidenceType);
    return C_TRUE;
}

/*
    Looks for evidence in a room that matches the given hunter's equipment. If this
    is found, it uses other functions to check if the evidence is a unique type
    compared to others in the shared evidence list. Then, it locks the room's evidence
    list, takes the evidence from the room to the hunter's shared list, and unlocks the list.
        in/out: hunter - uses the hunter's members and modifies them to try to add evidence to their evidence list.
*/
void checkForEv(Hunter* hunter) {
    //start at head
    EvidenceNode* currentEvidence = hunter->roomIn->ev.head;
    EvidenceNode* previousEvidence = NULL;
    while (currentEvidence != NULL) {
        //if evidence matches hunter's type
        if (hunter->canRead == currentEvidence->evType) {
            EvidenceType collectedEvidence = hunter->canRead;

            if(isUnique(hunter->collect, collectedEvidence)){
                
                sem_wait(&(hunter->collect->semaphore));
                addEvidenceToEvidenceList(hunter->collect, collectedEvidence);
                sem_post(&(hunter->collect->semaphore));

                sem_wait(hunter->houseSemaphore);
                l_hunterCollect(hunter->name, collectedEvidence, hunter->roomIn->name, hunter->outfile);
                sem_post(hunter->houseSemaphore);


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
        }
        //go next
        previousEvidence = currentEvidence;
        currentEvidence = currentEvidence->next;
    }
    // failed evidence collection
    //printf("failed evidence collection\n");
    
}

/*
    Checks if there are three pieces of evidence in the hunters' evidence list and
    logs this.
        in: hunter - the hunter whose evidence list is being iterated through.
        returns: true or false depending on whether there is enough evidence to
        identify the ghost.
*/
int evReview(Hunter* hunter){
    EvidenceNode* currEv = hunter->collect->head;
    int unique = 0;
    //go through the hunters' evidence list until the end (NULL), checking for 3 unique types of evidence
    for(int i = 0; i < EVIDENCE_TYPES; i++){
        while(currEv != NULL){
            if(currEv->evType == (enum EvidenceType)i){
                //printf("%d", uniqueEvs[unique]);
                unique++;
            }
            currEv = currEv->next;
        }
        currEv = hunter->collect->head;
    }

    if(unique > 2){ //
        sem_wait(hunter->houseSemaphore);
        l_hunterReview(hunter->name, LOG_SUFFICIENT, hunter->outfile);
        sem_post(hunter->houseSemaphore);

        //printf("sufficient %s", hunter->name);
        return C_TRUE; // pretty sure this is 1
    } else{
        sem_wait(hunter->houseSemaphore);
        l_hunterReview(hunter->name, LOG_INSUFFICIENT, hunter->outfile);
        sem_post(hunter->houseSemaphore);
        
    
        return C_FALSE;
        // REMEMBER TO FIND RESULTING GHOST CLASS
    }

}

/*
    Removes the hunter's prescence from the room it's currently in and removes
    the room from the hunter's members- this is used elsewhere in order to ensure
    the hunter successfully leaves the house without leaving a trace behind.
        in/out: hunter - the hunter who is leaving the house.
*/
void leaveHouse(Hunter* hunter){
    if (hunter->roomIn != NULL) {
        hunter->roomIn->huntersInRoom[hunter->id] = NULL;
        hunter->roomIn = NULL;
    }
}