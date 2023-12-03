#include "defs.h"

/* 
    Logs the hunter being created.
        in: hunter - the hunter name to log
        in: equipment - the hunter's equipment
*/
void l_hunterInit(char* hunter, enum EvidenceType equipment, FILE *outfile) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    char hunter_str[MAX_STR];
    sprintf(hunter_str, "[%s]", hunter);
    evidenceToString(equipment, ev_str);
    printf("[HUNTER INIT]     | %-20s |   is a [%s] hunter\n", hunter_str, ev_str);
    fprintf(outfile, "[HUNTER INIT]     | %-20s |   is a [%s] hunter\n", hunter_str, ev_str);
}

/*
    Logs the hunter moving into a new room.
        in: hunter - the hunter name to log
        in: room - the room name to log
*/
void l_hunterMove(char* hunter, char* room, FILE *outfile) {
    if (!LOGGING) return;
    char hunter_str[MAX_STR];
    sprintf(hunter_str, "[%s]", hunter);
    printf("[HUNTER MOVE]     | %-20s |   has moved into [%s]\n", hunter_str, room);
    fprintf(outfile, "[HUNTER MOVE]     | %-20s |   has moved into [%s]\n", hunter_str, room);
}

/*
    Logs the hunter exiting the house.
        in: hunter - the hunter name to log
        in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_hunterExit(char* hunter, enum LoggerDetails reason, FILE *outfile) {
    if (!LOGGING) return;
    char hunter_str[MAX_STR];
    sprintf(hunter_str, "[%s]", hunter);
    printf("[HUNTER EXIT]     | %-20s |   exited because ", hunter_str);
    fprintf(outfile, "[HUNTER EXIT]     | %-20s |   exited because ", hunter_str);
    switch (reason) {
        case LOG_FEAR:
            printf("[FEAR]\n");
            fprintf(outfile, "[FEAR]\n");
            break;
        case LOG_BORED:
            printf("[BORED]\n");
            fprintf(outfile, "[BORED]\n");
            break;
        case LOG_EVIDENCE:
            printf("[EVIDENCE]\n");
            fprintf(outfile, "[EVIDENCE]\n");
            break;
        default:
            printf("[UNKNOWN]\n");
            fprintf(outfile, "[UNKNOWN]\n");
    }
}

/*
    Logs the hunter reviewing evidence.
        in: hunter - the hunter name to log
        in: result - the result of the review, either LOG_SUFFICIENT or LOG_INSUFFICIENT
*/
void l_hunterReview(char* hunter, enum LoggerDetails result, FILE *outfile) {
    if (!LOGGING) return;
    char hunter_str[MAX_STR];
    sprintf(hunter_str, "[%s]", hunter);
    printf("[HUNTER REVIEW]   | %-20s |   reviewed evidence and found ", hunter_str);
    fprintf(outfile, "[HUNTER REVIEW]   | %-20s |   reviewed evidence and found ", hunter_str);
    switch (result) {
        case LOG_SUFFICIENT:
            printf("[SUFFICIENT]\n");
            fprintf(outfile, "[SUFFICIENT]\n");
            break;
        case LOG_INSUFFICIENT:
            printf("[INSUFFICIENT]\n");
            fprintf(outfile, "[INSUFFICIENT]\n");
            break;
        default:
            printf("[UNKNOWN]\n");
            fprintf(outfile, "[UNKNOWN]\n");
    }
}

/*
    Logs the hunter collecting evidence.
        in: hunter - the hunter name to log
        in: evidence - the evidence type to log
        in: room - the room name to log
*/
void l_hunterCollect(char* hunter, enum EvidenceType evidence, char* room, FILE *outfile) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    char hunter_str[MAX_STR];
    sprintf(hunter_str, "[%s]", hunter);
    evidenceToString(evidence, ev_str);
    printf("[HUNTER EVIDENCE] | %-20s |   found [%s] in [%s] and [COLLECTED]\n", hunter_str, ev_str, room);
    fprintf(outfile, "[HUNTER EVIDENCE] | %-20s |   found [%s] in [%s] and [COLLECTED]\n", hunter_str, ev_str, room);
}

/*
    Logs the ghost moving into a new room.
        in: room - the room name to log
*/
void l_ghostMove(char* room, FILE *outfile) {
    if (!LOGGING) return;
    printf("[GHOST MOVE]      | %-20s |   has moved into [%s]\n", "Ghost", room);
    fprintf(outfile, "[GHOST MOVE]      | %-20s |   has moved into [%s]\n","Ghost", room);
}

/*
    Logs the ghost exiting the house.
        in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_ghostExit(enum LoggerDetails reason, FILE *outfile) {
    if (!LOGGING) return;
    printf("[GHOST EXIT]      | %-20s |   Exited because ","Ghost");
    fprintf(outfile, "[GHOST EXIT]      | %-20s |   Exited because ","Ghost");
    switch (reason) {
        case LOG_FEAR:
            printf("[FEAR]\n");
            fprintf(outfile, "[FEAR]\n");
            break;
        case LOG_BORED:
            printf("[BORED]\n");
            fprintf(outfile, "[BORED]\n");
            break;
        case LOG_EVIDENCE:
            printf("[EVIDENCE]\n");
            fprintf(outfile, "[EVIDENCE]\n");
            break;
        default:
            printf("[UNKNOWN]\n");
            fprintf(outfile, "[UNKNOWN]\n");
    }
}

/*
    Logs the ghost leaving evidence in a room.
        in: evidence - the evidence type to log
        in: room - the room name to log
*/
void l_ghostEvidence(enum EvidenceType evidence, char* room, FILE *outfile) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("[GHOST EVIDENCE]  | %-20s |   left [%s] in [%s]\n","Ghost", ev_str, room);
    fprintf(outfile, "[GHOST EVIDENCE]  | %-20s |   left [%s] in [%s]\n","Ghost", ev_str, room);
}

/*
    Logs the ghost being created.
        in: ghost - the ghost type to log
        in: room - the room name that the ghost is starting in
*/
void l_ghostInit(enum GhostClass ghost, char* room, FILE *outfile) {
    if (!LOGGING) return;
    char ghost_str[MAX_STR];
    ghostToString(ghost, ghost_str);
    printf("[GHOST INIT]      | %-20s |   is a [%s] in room [%s]\n","Ghost", ghost_str, room);
    fprintf(outfile, "[GHOST INIT]      | %-20s |   is a [%s] in room [%s]\n","Ghost", ghost_str, room);
}

/*
    Logs the final results for after the program has completed, including who won,
    the ghost class, hunters who left due to fear or boredom, and the evidence found
    (by calling other functions).
        in: house - the house containing all the data needed- whether the hunters won or not,
                    their statuses, and the list of evidence they were able to collect.
*/
void printResults(House* house){
    int result = 0;
    printf("----------------------------\nSimulation complete.\nThe results are as follows.\n----------------------------\n");
    fprintf(house->outfile, "----------------------------\nSimulation complete.\nThe results are as follows.\n----------------------------\n");
    
    for(int i = 0; i < NUM_HUNTERS; i++){
        if(house->huntersInHouse[i].winCondition == 1){
            result = 1;
        }
    }

    printEvidenceList(&(house->foundEvidence), house->outfile);

    if(result == 1){
        
        printf("\nThe hunters win!\nThey collected enough evidence to identify the ghost.\n");

        char identifiedGhostStr[MAX_STR];
        ghostToString(determineGhostType(&(house->foundEvidence)), identifiedGhostStr);
        printf("Identified Ghost Type: %s\n", identifiedGhostStr);
        fprintf(house->outfile, "Identified Ghost Type: %s\n", identifiedGhostStr);
        
    } else{
        printf("\nThe ghost wins as the hunters did not collect enough evidence.\n");
        fprintf(house->outfile, "\nThe ghost wins as the hunters did not collect enough evidence.\n");
    }
    
    printf("Hunters who ran away in fear:\n");
    fprintf(house->outfile, "Hunters who ran away in fear:\n");
    for(int i = 0; i < NUM_HUNTERS; i++){
        if(house->huntersInHouse[i].fear >= FEAR_MAX){
            printf("%s\n", house->huntersInHouse[i].name);
            fprintf(house->outfile, "%s\n", house->huntersInHouse[i].name);
        }
    }
    printf("Hunters who left due to boredom:\n");
    fprintf(house->outfile, "Hunters who left due to boredom:\n");
    for(int i = 0; i < NUM_HUNTERS; i++){
        if(house->huntersInHouse[i].boredom >= BOREDOM_MAX){
            printf("%s\n", house->huntersInHouse[i].name);
            fprintf(house->outfile, "%s\n", house->huntersInHouse[i].name);
        }
    }

}

/*
    Logs the list of hunters' shared evidence through iteration.
        in: evidenceList - the hunters' list of unique pieces of evidence.
*/
void printEvidenceList(EvidenceList* evidenceList, FILE *outfile) {

    EvidenceNode* current = evidenceList->head;
    printf("Evidence found:\n");
    fprintf(outfile, "Evidence found:\n");

    while (current != NULL) {
        char evidenceTypeString[MAX_STR];
        evidenceToString(current->evType, evidenceTypeString);
        printf("    - %s \n", evidenceTypeString);
        fprintf(outfile, "    - %s \n", evidenceTypeString);
        current = current->next;
    }

}