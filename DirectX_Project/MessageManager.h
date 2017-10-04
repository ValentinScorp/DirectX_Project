#pragma once

#include <string>
#include <list>

class Message
{
public:
	std::string type;
	std::string name;
	int x;
	int y;	
	int delta;
public:
	Message(std::string t, std::string n, int ix, int iy, int d) {
		type = t;
		name = n;
		x = ix;
		y = iy;
		delta = d;
	}
	~Message() {
	}
};

class MessageSender;

class MessageReceiver
{
public:
	MessageReceiver() {}
	~MessageReceiver() {}

	virtual void OnMessage(Message message) = 0;

};

class MessageSender
{
private:
	std::list<MessageReceiver*> receivers;
public:
	MessageSender() {
	}
	~MessageSender() {}

	virtual void SendMessage(Message message) final {
		for (MessageReceiver *r : receivers) {
			r->OnMessage(message);
		}
	}
	virtual void RegisterListener(MessageReceiver *rec) final {
		receivers.push_back(rec);
	}
};




class MessageManager
{
private:
	std::list<Message> messages;

	
	

public:
	MessageManager();
	~MessageManager();

	void RegisterClient(MessageSender *sender);
	void RegisterClient(MessageReceiver *receiver);
};

