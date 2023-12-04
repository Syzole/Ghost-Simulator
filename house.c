#include "defs.h"

/*  
    Initializes the provided list of rooms.
        out: roomList - a list of rooms whose head, tail, and number of nodes are set
        to default values.
*/
void initRoomList(RoomList* roomList){
    roomList->head = NULL;
    roomList->tail = NULL;
    roomList->count = 0;
}

/*  
    Initializes the provided house by calling functions that initialize all the houses'
    members.
        out: house - the house whose room list, evidence list, hunters, and ghost are
        initialized through function calls.
*/
void initHouse(House* house) {
    initRoomList(&((house)->rooms));
    initEvidenceList(&((house)->foundEvidence));
    sem_init (&(house->houseSemaphore), 0, 1);
}

/*  
    Dynamically allocates a new room, setting its name to the provided string parameter
    as well as setting the room's other members (number of hunters present, a pointer to
    the ghost, list of evidence, and room list) to default values. It also initializes a
    semaphore that has control over those entering and exiting the room.
        in: name - the provided name is used.
        returns: a new room with all its members initialized and a semaphore for entering/exiting.
*/
Room* createRoom(char* name) {
    //Allocate space
    Room* room = (Room*)malloc(sizeof(Room));
    //copy name
    strcpy(room->name, name);
    //set pointer of ghost to null and init both lists and the counter to 0

    sem_init(&(room->semaphore), 0, 1);
    room->ghostInRoom = NULL;    
    initEvidenceList(&(room->ev));
    initRoomList(&(room->roomlist));

    for (int i = 0; i < NUM_HUNTERS; ++i) {
        room->huntersInRoom[i] = NULL;
    }

    return room;
}

/*  
    Dynamically allocates two room list nodes in order to add the two provided rooms to
    each others' room lists.
        out: room1, room2 - both are provided room whose room lists are modified.
*/
void connectRooms(Room* room1, Room* room2){
    //make space for both rooms
    RoomNode* node1 = (RoomNode*)malloc(sizeof(RoomNode));
    RoomNode* node2 = (RoomNode*)malloc(sizeof(RoomNode));

    node1->data = room1;
    node1->next = NULL;

    node2->data = room2;
    node2->next = NULL;


    //add based to end if list, checking if empty to adjust head or just the tail, plus increment the amount of rooms
    if(room1->roomlist.head==NULL){
        room1->roomlist.head=node2;
        room1->roomlist.tail=node2;
    }
    else{
        room1->roomlist.tail->next = node2;
        room1->roomlist.tail = node2;
    }
    room1->roomlist.count++;

    if (room2->roomlist.head == NULL) {
        room2->roomlist.head = node1;
        room2->roomlist.tail = node1;
    } 
    else {
        room2->roomlist.tail->next = node1;
        room2->roomlist.tail = node1;
    }
    room2->roomlist.count++;

}

/*  
    Dynamically allocates a room node for the house's room list and adds the given room
    to that list.
        out: roomlist - the house's room list meant to receive a room.
        in: room - the room to be added to the roomlist.
*/
void addRoom(RoomList* roomList, Room* room) {
    RoomNode* newNode = (RoomNode*)malloc(sizeof(RoomNode));
    newNode->data = room;
    newNode->next = NULL;

    if (roomList->head == NULL) {
        roomList->head = newNode;
        roomList->tail = newNode;
    } 
    else {
        roomList->tail->next = newNode;
        roomList->tail = newNode;
    }
    roomList->count++;
}

/*
    Dynamically allocates several rooms and populates the provided house using
    house.txt, which contains the names of the rooms and which rooms they are connected to.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(House* house) {
    //open the file
    FILE* fp = fopen("house.txt", "r");

    if(!fp){
        printf("Error opening file\n");
        exit(1);
    }

    char roomName[MAX_STR];
    // create rooms with the names in the given file and add them to the house's room list
    for (int i = 0; i < NUM_ROOMS; ++i) {
        fgets(roomName, MAX_STR, fp);
        cleanString(roomName);
        struct Room* newRoom = createRoom(roomName);
        addRoom(&house->rooms, newRoom);
    }

    char room1[MAX_STR];
    char room2[MAX_STR];
    // connect the rooms based on the listed connections in the given file
    for (int i = 0; i < NUM_ROOMS_CONNECTIONS; ++i) {
        fgets(room1, MAX_STR, fp);
        fgets(room2, MAX_STR, fp);
        cleanString(room1);
        cleanString(room2);
        Room* room1ptr = findRoom(&house->rooms, room1);
        Room* room2ptr = findRoom(&house->rooms, room2);
        connectRooms(room1ptr, room2ptr);
    }

    fclose(fp);
}

/*  
    Randomly selects an adjacent room for someone to move into by iterating through the
    given room list until a random index is reached.
        in: roomlist - the current room's list of connected rooms
        returns: the random room's data
*/
Room* selectRandomRoom(RoomList* roomlist){
    int randIndex = randInt(0, roomlist->count);
    RoomNode* currentRoom = roomlist->head;
    
    for (int i = 0; i < randIndex; ++i) {
        currentRoom = currentRoom->next;
    }

    return currentRoom->data;
}