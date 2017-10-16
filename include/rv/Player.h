#ifndef _REVE_PLAYER_H
#define _REVE_PLAYER_H

/* Stage attributes, this is what attr gets set to on the STAGE */
typedef struct {
	Uint8 up, down, left, right, ok, cancel, exit;
} rv_Pad;


extern rv_Pad rv_Player;
rv_CrewStatus rv_PLAYER(rv_Crew *c);	

#endif
