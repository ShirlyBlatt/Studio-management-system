#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"


enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone()=0;
    virtual ~BaseAction();
    std::string statusToString() const;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    OpenTrainer(int id, std::vector<Customer *> &customersList, std::string& action);
    ~OpenTrainer();
    OpenTrainer(const OpenTrainer &other);
    OpenTrainer(OpenTrainer &&other);
    OpenTrainer& operator=(const OpenTrainer& other);
    OpenTrainer& operator=(OpenTrainer&& other);
    void act(Studio &studio);
    std::string toString() const;
    OpenTrainer* clone();
private:
	const int trainerId;
	std::vector<Customer *> customers;
    std::string stringOfAction;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(int id,std::string& action );
    void act(Studio &studio);
    std::string toString() const;
    Order* clone();
    ~Order();
private:
    const int trainerId;
    std::string stringOfAction;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(int src, int dst, int customerId, std::string& action);
    void act(Studio &studio);
    std::string toString() const;
    MoveCustomer* clone();
    ~MoveCustomer();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
    std::string stringOfAction;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(int id, std::string& action);
    void act(Studio &studio);
    std::string toString() const;
    Close* clone();
    ~Close();
private:
    const int trainerId;
    std::string stringOfAction;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    CloseAll* clone();
    ~CloseAll();
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    PrintWorkoutOptions* clone();
    ~PrintWorkoutOptions();

private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(int id, std::string& action);
    void act(Studio &studio);
    std::string toString() const;
    PrintTrainerStatus* clone();
    ~PrintTrainerStatus();
private:
    const int trainerId;
    std::string stringOfAction;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    PrintActionsLog* clone();
    ~PrintActionsLog();
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    BackupStudio* clone();
    ~BackupStudio();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    RestoreStudio* clone();
    ~RestoreStudio();
};


#endif