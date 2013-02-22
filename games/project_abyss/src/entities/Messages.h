#ifndef ENT_MESSAGE
#define ENT_MESSAGE

enum {
	MSG_CHANGE_STATE_VAR
};

struct MsgDataChangeStateVar 
{
	int idf, value;
};

#endif