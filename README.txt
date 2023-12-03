Authors:
Arjun Bedi: #101267749
Jessica Hill 101280293

As per the professor's instructions- since the bonus implementations change the base control flow,
there are two tar files included:
The base file (COMP2401_BEDI_ARJUN_101267749_HILL_JESSICA_101280293_FINAL.tar)
and the bonus file (BONUS_COMP2401_BEDI_ARJUN_101267749_HILL_JESSICA_101280293_FINAL.tar).

Bonuses implemented:
-The commit png is a screen shot our commit history for the Github bonus marks
-It writes a log of all the events to the logging.txt file
-Used print formatting to make the data easy to read using consistent fixed-width columns

Program Description:
- Program starts by initializing a house and adding rooms to the house
- Each room has a list of rooms it is connected to and evidence that has been dropped inside the room
- The program will start by asking the user for the names of each of the hunters, and each will be given equipment to find one type of evidence
- All of the hunters start by being placed in the van, where the ghost can be anywhere in the house excluding the van
- The hunters while in the simulation can grow tired or scared of the ghost over time depending if they are in the same room as the ghost
- The hunters can take one of the following actions at a time: Move to a new room, check the room for evidence, or review the evidence that all the hunters have found
- The ghost while in the simulation can grow bored over time, or in the same room as the hunter where their bordem will reset 
- The ghost can take one of the following actions at a time: move to a new room, do nothing or drop evidence in the room they are in
- The hunters will leave if they either get scared or bored or have decided they have found enough evidence upon review
- The ghost will leave if they get bored
- The results will be printed
- Hunters win if at least one of the hunters has identified the ghost with the evidence they have
- Ghost wins if they have not found enough evidence to identify the ghost

Note: The use of the house semaphore was used to make sure that the entities took turns making logging statements, so they do not interupt each other


List of files used for compilation:	

● Ghost.c: initGhost(), moveGhost(), doNothing(), dropEvidence(), determineGhostType()

● Hunter.c: initHunter(), initEvidenceList(), moveToNewRoom(), isUnique(), checkForEv(), evReview(), leaveHouse()

● defs.h: This is the header that contains all of the constants and forward declarations for each of the files

● house.c: initRoomList(), initHouse(), createRoom(), connectRooms(), addRoom(), populateRooms(), selectRandomRoom()

● logger.c: l_hunterInit(), l_hunterMove(), l_hunterExit(), l_hunterReview(), l_hunterCollect(), l_ghostMove(), l_ghostExit(), l_ghostEvidience(), l_ghostInit, printresults(), printEvidinceList()

● main.c: main(), startHunt(), ghost_thread(), hunter_thread()

● makefile: This file is used to compile the executable, instructions can be found below in Compile instructions

● utils.c:  randInt(), randFloat(), randomGhost(), ghostToString(), evidenceToString(), addEvidenceToEvidenceList(), checkIfSameRoom(), cleanupEvidenceList(), cleanupRoomList(), cleanupHouse()

Compile instructions:
 - Inside the folder that contains all of the files, open the terminal and type 'make', this will create an executable called final
 - Inside the terminal type ./final to begin using the executable
 - To clean the compiled files, use make clean to clear the compiled executable and the object files made

Program instructions:
- The program will start by asking the user for input for 4 names, one for each hunter
- After the program will run the simulation
- At the end, it will print the results to see what the final outcome was

Special Notes:
- Our run with default  parameters results in either the hunters winning or them getting bored, we found that
to solve this problem either decrease the fear max to a value of 1, 2 or 3, or raising the ghost time to roughly half of the 
hunter timer, so about 2500, or 3000 or combing the two changes. We noticed that since the ghost takes much more actions than the hunters, it tends to leave rooms before the hunter
can take its thread to run, making it so that the fear check is never made so by reducing fear max or slowing the ghost fixes this problem

- No AI was used (it caused more problems than helped), and as commented in our code the addEvidenceToEvidenceList was heavily influenced by my (Arjun Bedi) A4 addGhost function as I found alot of it transferable
