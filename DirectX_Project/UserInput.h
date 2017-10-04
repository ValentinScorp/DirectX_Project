#pragma once

#include <vector>

#include "MessageManager.h"

enum {
	LeftMouse = 1,
	RightMouse,
	MiddleMouse,
	MouseMove,
};


class UserMessage {
public:
	UserMessage(int ix, int  iy, int  ikeyDown, int ikeyUp, int idelta):
		x(ix), y(iy), keyDown(ikeyDown), keyUp(ikeyUp), delta(idelta)
	{}
	~UserMessage() {}

	int x;
	int y;
	int keyDown;
	int keyUp;
	int delta;
};

class UserInput : public MessageSender
{
public:
	UserInput();
	~UserInput();

	void PlaceMessage(UserMessage um);
	void BeginSearch();
	UserMessage* GetNextMessage();
	void ClearMessages();

private:
	size_t it;
	std::vector <UserMessage> messages;
};

