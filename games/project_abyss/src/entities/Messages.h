#ifndef ENT_MESSAGE
#define ENT_MESSAGE

enum {
	MSG_CHANGE_STATE_VAR,
	MSG_POWER,
	MSG_BODY_ACTIVATE_GRAVITY,
	MSG_HERO_KILL
};

struct MsgDataChangeStateVar 
{
	int idf, value;
};

#endif