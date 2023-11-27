#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Can change numbers as long as the core rules of the simulation hold up.

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

//Functions that we have defined

//house.c
void initGhost(Ghost* ghost);
void initHunter(Hunter* hunter, House* house, int numHunt);
void initEvidenceList(EvidenceList* evidenceList);
void initRoomList(RoomList* roomList);
void initHouse(House** house);
Room* createRoom(char* name);
void connectRooms(Room* room1, Room* room2);
void addRoom(RoomList* roomList, Room* room);
void populateRooms(HouseType* house);

//Structs that we have also defined to use for the program

typedef struct RoomNode {
    Room *data;
    struct RoomNode *next;
} RoomNode;

// Define RoomList next, as it's used in Room
typedef struct RoomList {
    RoomNode* head;
    RoomNode* tail;
    int count; //this will be used in the random parts
} RoomList;

// Now define Room, as it's used in House
typedef struct {
    char name[MAX_STR];
    Ghost *ghost;
    EvidenceList ev;
    RoomList roomlist;
    Hunter *huntersInRoom[NUM_HUNTERS];
    sem_t semaphore;
} Room;

typedef struct {
    Hunter *huntersInHouse[NUM_HUNTERS];
    RoomList totalRoomList;
    EvidenceList foundEvidence;
} House;

typedef struct {
    GhostClass ghostType;
    int boredom;
    Room *room;
} Ghost;

typedef struct {
    int id;
    char name[MAX_STR];
    Room *roomIn;
    EvidenceType canRead;
    EvidenceList *collect;
    int fear;
} Hunter;

typedef struct EvidenceList {
    EvidenceNode *head;
    EvidenceNode *tail;
} EvidenceList;

typedef struct {
    EvidenceType evType;
    struct EvidenceNode *next;
} EvidenceNode;
