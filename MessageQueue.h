#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <memory>
#include <mutex>
#include <queue>
#include "MessageManager.h"


class MessageManager;

class MessageQueue
{
public:

	void push(std::unique_ptr<MessageManager> message);

	bool bIsMessageProcessed();

	std::size_t processAllMessages();
	// void processMessage();


private:
	std::queue<std::unique_ptr<MessageManager>> queue;
	std::mutex mutex;
};
extern MessageQueue g_messageQueue; //declares a global message queue so we can use it in other scripts
#endif