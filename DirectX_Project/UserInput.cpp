#include "UserInput.h"


UserInput::UserInput()
{
}


UserInput::~UserInput()
{
}

void UserInput::PlaceMessage(UserMessage um)
{
	messages.push_back(um);
}

void UserInput::BeginSearch()
{
	it = 0;
}

UserMessage* UserInput::GetNextMessage()
{	
	if (it >= messages.size()) {
		return nullptr;
		it = 0;
	}	
	return &messages[it++];
}

void UserInput::ClearMessages()
{
	messages.clear();
}
