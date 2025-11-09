#include "CommunicationManager.h"
#include "Logger.h"

CommunicationManager::CommunicationManager(CommandProcessor* processor) 
    : _commandProcessor(processor) {}

void CommunicationManager::addInterface(CommunicationInterface* iface) {
    _interfaces.push_back(iface);
}

void CommunicationManager::updateAll() {
    for (auto& iface : _interfaces) {
        
        iface->update();
        
        if (iface->commandAvailable()) {
            String command = iface->readCommand();
            
            Logger::getInstance().info("Recieved Command(%s)", command.c_str());

            _commandProcessor->process(command, iface);
        }
    }
}