#ifndef _PSILOCIN_PLAYER_H
#define _PSILOCIN_PLAYER_H

/* Stage attributes, this is what attr gets set to on the STAGE */
typedef struct {
	Uint8 up, down, left, right, ok, cancel, exit;
} ps_Pad;


extern ps_Pad ps_Player;
ps_CrewStatus ps_PLAYER(ps_Crew *c);	

#endif
