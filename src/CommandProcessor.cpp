#include <sstream>
#include "Logger.h"
#include "Commands.h"
#include "CommandProcessor.h"
#include "FileLoggerService.h"

CommandProcessor::CommandProcessor(TaskService* taskService, FileLoggerService* fileLogger) 
    : _taskService(taskService), _fileLogger(fileLogger) 
{}

void CommandProcessor::process(const String& message, CommunicationInterface* channel) {
    if (message.startsWith(Commands::GET_TASKS)) {
        channel->sendResponse("Tasks:");
        for (const auto& task : _taskService->getTasks()) {
            channel->sendResponse(task.getID() + " " + task.getTime());
        }
    } else if (message.startsWith(Commands::ADD_TASK)) {
        String args = message;
        // TODO solve it better
        args.remove(0, 8);
        int commaIndex = args.indexOf(',');

        if (commaIndex > 0) {
            String id = args.substring(0, commaIndex);
            String time = args.substring(commaIndex + 1);

            Task newTask(id, time);

            try
            {
                _taskService->addTask(newTask); 

                std::stringstream ss;
                ss << "OK: Task added at " << newTask.getTime() << " with ID: " << newTask.getID();

                channel->sendResponse("OK: Task added.");
            }
            catch(const std::exception& e)
            {
                channel->sendResponse("ERROR: Failed to add task.");
            }
        } else {
            channel->sendResponse("ERROR: Invalid ADDTASK format. Use: ADDTASK id,hh:mm");
        }
    } else if (message.startsWith(Commands::FEED)) {
        _taskService->handleTask(Task("0", "00:00", 100));
        channel->sendResponse("OK: Feeding started.");
    } else if (message.startsWith(Commands::DELETE_ALL)) {
        _taskService->deleteAllTasks();
        channel->sendResponse("OK: All Task Deleted.");
    } else if (message.startsWith(Commands::DELETE_TASK)) {
        int commandLength = strlen(Commands::DELETE_TASK);

        int idStartIndex = commandLength + 1;

        if (message.length() > idStartIndex) {
            String taskID = message.substring(idStartIndex);
            
            Serial.println("Deleting task by id= " + taskID);

            _taskService->deleteTask(taskID);
        } else {
            Serial.println("Error: DELETETASK command sent with no ID. message= " + message);
        }
    } else if (message.startsWith(Commands::GETLOGS)) {
        channel->sendResponse(_fileLogger->readLogs());
    } else if (message.startsWith(Commands::CLEARLOGS)) {
        _fileLogger->clearLogs();
        channel->sendResponse("OK");
    } else {
        Logger::getInstance().error("Unknown command %s", message);
    }

    Serial.println("Processed command: " + message);
}