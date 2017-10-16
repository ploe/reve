#ifndef _REVE_CREW_H
#define _REVE_CREW_H

/*	Each Crew update method returns an exit code, this exit code
	dictates whether or not we deallocate the Crew responsible, play
	it again or exit the whole app. */
typedef int rv_CrewStatus;
enum {
	rv_LIVE = 0,
	rv_ENCORE,
	rv_PAUSE,
	rv_CUT,
	rv_EXIT,
};

/* 	Type for Crew which is a state in the State machine. 
	The Crew are a linked list of actions we need to execute each and
	every frame.	*/ 
typedef struct rv_Crew {
	char *tag;
	void *attr;
	void *type, *update, *destroy;
	rv_CrewStatus status;	
	struct rv_Crew *prev, *next;
} rv_Crew;

/*	rv_Updater is the type used to describe methods for the Crew	*/
typedef rv_CrewStatus (*rv_Updater)(rv_Crew *);
typedef char *(*rv_Marshaller)(rv_Crew *);

extern const char *rv_CrewStatusStr[] ;

rv_Crew *rv_CrewNew(rv_Updater type);
rv_Bool rv_CrewRoll();
void rv_CrewRollCall();
rv_CrewStatus rv_CrewCall(rv_Crew *c, rv_Updater func);
void rv_CrewPurge();

#endif
