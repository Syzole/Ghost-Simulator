#include "defs.h"

/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}

/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;   
    }
}

/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

/*  
    Dynamically allocates an evidence node in order to place the given evidence type in the
    next possible node in the given evidence list. This function was heavily influenced by
    Arjun's addGhost function from Assignment 4 in this course.
        out: evidenceList - evidence is added to the evidence list pointed to.
        in: evidenceType - an enum that describes a type of evidence a ghost leaves.
*/
void addEvidenceToEvidenceList(EvidenceList* evidenceList, EvidenceType evidenceType) {
    EvidenceNode* newEvidence = (EvidenceNode*)malloc(sizeof(EvidenceNode));

    newEvidence->evType = evidenceType;
    newEvidence->next = NULL;

    if (evidenceList->head == NULL) {
        evidenceList->head = newEvidence;
        evidenceList->tail = newEvidence;
    } 
    else {
        evidenceList->tail->next = newEvidence;
        evidenceList->tail = newEvidence;
    }
}

/*  
    Checks the room that the provided hunter is currently in for ghosts- if one is detected,
    the hunter's fear is increased while their boredom is reset. If they are in a room without
    a ghost, their boredom increases.
        out: hunter - the hunter whose status effects are changed.
*/
void checkIfSameRoom(Hunter* hunter){
    if(hunter->roomIn->ghostInRoom != NULL){
        hunter->fear++;
        hunter->boredom = 0;
    } else{
        hunter->boredom++;
    }
}

/*  
    Cleans up memory for a list of evidence nodes.
        out: evidenceList - the evidence list that is having its nodes freed and reset.
*/
void cleanupEvidenceList(EvidenceList* evidenceList) {
    if (evidenceList->head == NULL) {
        return;
    }
    EvidenceNode* currentEvidence = evidenceList->head;
    
    while (currentEvidence != NULL) {
        EvidenceNode* nextEvidence = currentEvidence->next;
        free(currentEvidence);  // Free the evidence node
        currentEvidence = nextEvidence;
    }
}

/*  
    Cleans up memory for a list of room nodes and associated rooms.
        out: roomList - a room list whose nodes are being freed and reset.
*/
void cleanupRoomList(RoomList* roomList) {
    RoomNode* currentRoomNode = roomList->head;

    while (currentRoomNode != NULL) {
        RoomNode* nextRoomNode = currentRoomNode->next;

        free(currentRoomNode); // Free the node itself

        currentRoomNode = nextRoomNode;
    }

    // Reset head, tail, and count after cleanup
    roomList->head = roomList->tail = NULL;
    roomList->count = 0;
}

/*  
    Cleans up memory for the entire house, including evidence lists and room
    lists for each room. Function calls are used to accomplish this.
        out: house - the house whose members are being freed and reset.
*/
// 
void cleanupHouse(House* house) {
    cleanupEvidenceList(&(house->foundEvidence));

    RoomNode* currentRoomNode = house->rooms.head;

    while (currentRoomNode != NULL) {
        RoomNode* nextRoomNode = currentRoomNode->next;

        // Cleanup the evidence list for the current room
        cleanupEvidenceList(&(currentRoomNode->data->ev));
        cleanupRoomList(&(currentRoomNode->data->roomlist));
        free(currentRoomNode->data);  // Free the room associated with the node
        free(currentRoomNode);        // Free the node itself

        currentRoomNode = nextRoomNode;
    }

    // Reset head, tail, and count after cleanup
    house->rooms.head = house->rooms.tail = NULL;
    house->rooms.count = 0;
}

