// Include necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Constants
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

// Enums
typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef enum LoggerDetails LoggerDetails;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };


// Forward declarations
typedef struct Room Room;
typedef struct Ghost Ghost;
typedef struct EvidenceList EvidenceList;
typedef struct RoomNode RoomNode;
typedef struct RoomList RoomList;
typedef struct Hunter Hunter;
typedef struct EvidenceNode EvidenceNode;
typedef struct House House;

// Data structures
struct EvidenceNode {
    EvidenceType evType;
    struct EvidenceNode* next;
};

struct EvidenceList {
    EvidenceNode* head;
    EvidenceNode* tail;
};

struct RoomNode {
    Room* data;
    struct RoomNode* next;
};

struct RoomList {
    RoomNode* head;
    RoomNode* tail;
    int count;
};

struct Room {
    char name[MAX_STR];
    Ghost* ghost;
    EvidenceList ev;
    RoomList roomlist;
    Hunter* huntersInRoom[NUM_HUNTERS];
    sem_t semaphore;
};

struct Hunter {
    int id;
    char name[MAX_STR];
    Room* roomIn;
    EvidenceType canRead;
    EvidenceList* collect;
    int fear;
};

struct Ghost {
    GhostClass ghostType;
    int boredom;
    Room* room;
};


struct House {
    Hunter* huntersInHouse[NUM_HUNTERS];
    RoomList rooms;
    EvidenceList foundEvidence;
};

// Helper function declarations
int randInt(int, int);
float randFloat(float, float);
enum GhostClass randomGhost();
void ghostToString(enum GhostClass, char*);
void evidenceToString(enum EvidenceType, char*);

// Logging function declarations
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

// Forward declarations for functions in Ghost.c
void initGhost(Ghost* ghost);

// Forward declarations for functions in Hunter.c
void initHunter(Hunter* hunter, House* house, int numHunt);
void initEvidenceList(EvidenceList* evidenceList);
void addHunterToRoom(Room* room, Hunter* hunter);

// Forward declarations for functions in house.c
void initRoomList(RoomList* roomList);
void initHouse(House* house);
Room* createRoom(char* name);
void connectRooms(Room* room1, Room* room2);
void addRoom(RoomList* roomList, Room* room);
void populateRooms(House* house);

// Forward declarations for functions in main.c
void startHunt(House *house,Ghost* ghost);