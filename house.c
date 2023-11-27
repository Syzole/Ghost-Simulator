#include "defs.h"

void initGhost(Ghost* ghost) {
    ghost->ghostType = randomGhost();
    ghost->boredom = 0;
    ghost->room = NULL;
    //l_ghostInit(ghost->ghostType, ghost->room->name);
}
void initHunter(Hunter* hunter, House* house, int numHunt) {
    hunter->id = numHunt;
    //Hunter Id is the number of the hunter -1
    printf("This is Hunter #%d. What should their name be?",numHunt+1);
    scanf("%63s", hunter->name);
    hunter->roomIn = house->totalRoomList.head->data;
    hunter->canRead = (EvidenceType)(numHunt);
    hunter->collect = &(house->foundEvidence);
    hunter->fear = 0;
    l_hunterInit(hunter->name,hunter->canRead);
}

void initEvidenceList(EvidenceList* evidenceList){
    evidenceList->head = NULL;
    evidenceList->tail = NULL;
}

void initRoomList(RoomList* roomList){
    roomList->head = NULL;
    roomList->tail = NULL;
    roomList->count = 0;
}

void initHouse(House** house) {
    *house = (House*)malloc(sizeof(House));

    initRoomList(&((*house)->totalRoomList));
    initEvidenceList(&((*house)->foundEvidence));
    
    //init hunters and add to house array
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        Hunter* newHunter = (Hunter*)malloc(sizeof(Hunter));
        initHunter(newHunter, *house, i);
        (*house)->huntersInHouse[i] = newHunter;
    }
}

Room* createRoom(char* name) {
    //Allocate space
    Room* room = (Room*)malloc(sizeof(Room));
    //copy name
    strcpy(room->name, name);
    //set pointer of ghost to null and init both lists

    room->ghost = NULL;    
    initEvidenceList(&(room->ev));
    initRoomList(&(room->roomList));
    return room;
}

void connectRooms(Room* room1, Room* room2){
    //make space for both rooms
    RoomNode* node1 = (RoomNode*)malloc(sizeof(RoomNode));
    RoomNode* node2 = (RoomNode*)malloc(sizeof(RoomNode));

    node1->data = room1;
    node1->next = NULL;

    node2->data = room2;
    node2->next = NULL;


    //add based to end if list, checking if empty to adjust head or just the tail
    if(room1->roomlist.head==NULL){
        room1->roomlist.head=node2;
        room1->roomlist.tail=node2;
    }
    else{
        room1->roomlist.tail->next = node2;
        room1->roomlist.tail = node2;
    }

    if (room2->roomlist.head == NULL) {
        room2->roomlist.head = node1;
        room2->roomlist.tail = node1;
    } 
    else {
        room2->roomlist.tail->next = node1;
        room2->roomlist.tail = node1;
    }

}

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

void addHunterToRoom(Room* room, Hunter* hunter) {
    room->huntersInRoom[hunter->id] = hunter;
    l_hunterMove(hunter->name, room->name);
}

void startHunt(House *house,Ghost* ghost){
    //first add hunters to the head
    for (int i = 0; i < NUM_HUNTERS; ++i) {
        addHunterToRoom(house->totalRoomList.head->data, house->huntersInHouse[i]);
    }

    int randRoom = randInt(1,house->totalRoomList.count);
    RoomNode* currRoomNode = house->totalRoomList.head;

    for (int i = 0; i < randRoom; ++i) {
        currRoomNode = currRoomNode->next;
    }

    ghost->room = currRoomNode->data;
    l_ghostInit(ghost->ghostType, ghost->room->name);
}

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}