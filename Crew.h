#ifndef _PSILOCIN_CREW_H
#define _PSILOCIN_CREW_H

/*	Each Crew update method returns an exit code, this exit code
	dictates whether or not we deallocate the Crew responsible, play
	it again or exit the whole app. */
typedef int ps_CrewStatus;
enum {
	ps_LIVE = 0,
	ps_ENCORE,
	ps_PAUSE,
	ps_CUT,
	ps_EXIT,
};

/* 	Type for Crew which is a state in the State machine. 
	The Crew are a linked list of actions we need to execute each and
	every frame.	*/ 
typedef struct ps_Crew {
	char *tag;
	void *attr;
	void *type, *update, *destroy, *marshal;
	ps_CrewStatus status;	
	struct ps_Crew *prev, *next;
} ps_Crew;

/*	ps_Updater is the type used to describe methods for the Crew	*/
typedef ps_CrewStatus (*ps_Updater)(ps_Crew *);
typedef char *(*ps_Marshaller)(ps_Crew *);

extern const char *ps_CrewStatusStr[] ;

ps_Crew *ps_CrewNew(ps_Updater type);
ps_Bool ps_CrewRoll();
char *ps_CrewMarshal();
void ps_CrewRollCall();
ps_CrewStatus ps_CrewCall(ps_Crew *c, ps_Updater func);

#endif
