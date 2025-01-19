#ifndef TASK_H
#define TASK_H

class Task {
    private:
        long id;
    public:
        long getId() { return id; }
        void setId(long id) { this->id = id; }   

    Task(long id) : id(id) {}
};

#endif