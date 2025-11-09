#pragma once
#include <vector>
#include "CommunicationInterface.h"
#include "CommandProcessor.h"

class CommunicationManager {
public:
    CommunicationManager(CommandProcessor* processor);

    void updateAll();
    void addInterface(CommunicationInterface* iface);

private:
    std::vector<CommunicationInterface*> _interfaces;
    CommandProcessor* _commandProcessor;
};