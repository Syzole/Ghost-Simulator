#include "defs.h"

/*
    Initializes the ghost by setting its type equal to a random
    GhostClass enum, filling up its allowed evidence array with
    the types of evidence it will leave due to its class,
    initializing its other members to default values, and
    linking its semaphore to the house's.
        out: ghost - the ghost whose values are being initialized.
        in: house - the house whose semaphore is being used for the ghost.
*/
void initGhost(Ghost* ghost, House* house) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->houseSemaphore = &(house->houseSemaphore);
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
            break;
    }

    int randRoom = randInt(1,house->rooms.count);
    RoomNode* currRoomNode = house->rooms.head;

    for (int i = 0; i < randRoom; ++i) {
        currRoomNode = currRoomNode->next;
    }

    ghost->roomIn = currRoomNode->data;
    currRoomNode->data->ghostInRoom = ghost;
    ghost->outfile = house->outfile;
    //l_ghostInit(ghost->ghostType, ghost->roomIn->name, ghost->outfile);l_ghostInit(ghost->ghostType, ghost->roomIn->name, ghost->outfile);
}

/*
    Removes the ghost from its current room, unlocking it, and 
    then moving it to a new one, locking that one. Semaphores are used
    to ensure no one has access to the rooms being utilized here.
        in/out: ghost - the ghost will now have a pointer to the new room.
        in/out: room - the room that the ghost moves to will now have a pointer to it.
                       it is also used to check for hunters.
        returns: a true or false value depending on if there
        are any hunters in the room the ghost wishes to move to.
*/
int moveGhost(Ghost* ghost, Room* room){
    if(roomHasHunters(room)){
        //is the ghost in the same place, yes so return true
        return C_TRUE;
    }

    ghost->roomIn->ghostInRoom = NULL;
    sem_post(&ghost->roomIn->semaphore);
        
    sem_wait(&(room->semaphore));
    room->ghostInRoom = ghost;
    ghost->roomIn = room;
    //did ghost stay in his place, no so return false
    sem_wait(ghost->houseSemaphore);
    l_ghostMove(room->name, ghost->outfile);
    sem_post(ghost->houseSemaphore);

    return C_FALSE;
}

/*
    Does nothing- a potential action for the ghost thread.
*/
void doNothing(){
    //
}

/*
    Adds evidence to a room's evidence list and logs it. Semaphores
    are used to ensure that the hunters cannot change this evidence
    list at the same time.
        in: ghost - the ghost pointer is used to access its members,
        such as its allowed evidence array and the semaphore for the
        evidence list of the room it's in.
*/
void dropEvidence(Ghost* ghost){
    int randomIndex = randInt(0, ALLOWED_EVIDENCE);

    EvidenceType addedEv = ghost->allowedEvidence[randomIndex];
    
    sem_wait(&(ghost->roomIn->ev.semaphore));
    addEvidenceToEvidenceList(&(ghost->roomIn->ev), addedEv);
    sem_post(&(ghost->roomIn->ev.semaphore));

    sem_wait(ghost->houseSemaphore);
    l_ghostEvidence(addedEv, ghost->roomIn->name, ghost->outfile);
    sem_post(ghost->houseSemaphore);
}

/*
    Determines the ghost's type by checking what class the three
    unique types collected match up with.
        in: foundEvidence - the hunters' list of evidence that is being
        used to find the type of ghost.
        returns: the GhostClass enum that is identified.
*/
GhostClass determineGhostType(EvidenceList* foundEvidence) {
    int evidenceCounts[4] = {0,0,0,0}; 
    
    EvidenceNode* currentEvidence = foundEvidence->head;
    while (currentEvidence != NULL) {
        evidenceCounts[currentEvidence->evType]++;
        currentEvidence = currentEvidence->next;
    }

    if (evidenceCounts[EMF] && evidenceCounts[TEMPERATURE] && evidenceCounts[FINGERPRINTS]) {
        return POLTERGEIST;
    } 
    else if (evidenceCounts[EMF] && evidenceCounts[TEMPERATURE] && evidenceCounts[SOUND]) {
        return BANSHEE;
    } 
    else if (evidenceCounts[EMF] && evidenceCounts[FINGERPRINTS] && evidenceCounts[SOUND]) {
        return BULLIES;
    } 
    else if (evidenceCounts[TEMPERATURE] && evidenceCounts[FINGERPRINTS] && evidenceCounts[SOUND]) {
        return PHANTOM;
    }

    return GH_UNKNOWN;
}

/*
    Checks the given room to see if it has hunters in it.
        in: room - the room that is being checked for hunters.
        returns: true or false depending on if there are hunters present.
*/
int roomHasHunters(Room* room) {
  
    // Iterate through each hunter in the room
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        if (room->huntersInRoom[i] != NULL) {
            return C_TRUE;
        }
    }

    return C_FALSE;
}

