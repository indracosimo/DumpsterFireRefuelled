#include "MessageQueue.h"

//#include "entt/process/process.hpp"

MessageQueue g_messageQueue;

void MessageQueue::push(std::unique_ptr<MessageManager> message)
{
	std::lock_guard<std::mutex> lock(mutex);
	queue.push(std::move(message));
}

bool MessageQueue::bIsMessageProcessed()
{
	std::unique_ptr<MessageManager> message;
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (queue.empty()) return false;

		message = std::move(queue.front());
		queue.pop();
	}
	//processing outside the mutex lock so that messages get safely pushed
	if (message) message->processMessage();

	return true;
}
//process message outside of lock so we can safely push more if neded
std::size_t MessageQueue::processAllMessages()
{
	std::size_t count = 0;
	while (bIsMessageProcessed()) ++count; return count; 
}

//was processing message while still holding onto mutex which defeats which defeats the purpose of keeping the handler safe. also the queue only processes one message properly with this structure
// void MessageQueue::processMessage()
// {
// 	std::lock_guard<std::mutex> lock(mutex);
// 	if (!queue.empty())
// 	{
// 		auto message = std::move(queue.front());
// 		queue.pop();
// 		message->processMessage();
// 	}
// }
