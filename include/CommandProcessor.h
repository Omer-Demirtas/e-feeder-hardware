#pragma once
#include "TaskService.h"
#include "FileLoggerService.h"
#include "CommunicationInterface.h"

class CommandProcessor {
public:
    CommandProcessor(TaskService* taskService, FileLoggerService* fileLogger);

    void process(const String& command, CommunicationInterface* channel);

private:
    TaskService* _taskService;
    FileLoggerService* _fileLogger;
};