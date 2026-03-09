#include "core/application.h"
#include "core/config.h"
#include "MessageManager.h"
#include "MessageQueue.h"

extern GLFWwindow* window;

int main() 
{

    application app(Resolution(config::HD.width, config::HD.height), "DumpsterFire");

    MessageQueue messageQueue;

    app.Run();

    return 0;
}

//messageQueue.push(std::make_unique<StringMessage>("Hello, World!"));
//   messageQueue.push(std::make_unique<FloatMessage>(42.0f));

//   //Message Manager testing
//   RaycastMessage raycastMsg(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 100.0f);
//   raycastMsg.processMessage();
//   StringMessage stringMsg("Hello, this is a string message!");
//   stringMsg.processMessage();
   //g_messageQueue.push(std::make_unique<RaycastMessage>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 100.0f));
   //g_messageQueue.push(std::make_unique<StringMessage>("Hello, World!"));
   //g_messageQueue.push(std::make_unique<FloatMessage>(42.0f));
   //g_messageQueue.push(std::make_unique<MouseMessage>(100, 200, true));
   //g_messageQueue.processMessage();

//   FloatMessage floatMsg(42.0f);
//   floatMsg.processMessage();

   //MouseMessage mouseMsg(100, 200, true);
   //mouseMsg.processMessage();
   //Message Manager testing


   //while (true) 
   //{
   //    messageQueue.processMessage();
   //}