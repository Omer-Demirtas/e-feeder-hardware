#pragma once
#include "CommunicationInterface.h"
#include "TaskService.h"

class CommandProcessor {
public:
    CommandProcessor(TaskService* taskService);

    void process(const String& command, CommunicationInterface* channel);

private:
    TaskService* _taskService;
};