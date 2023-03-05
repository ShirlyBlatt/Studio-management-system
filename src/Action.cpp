#include "../include/Action.h"

extern Studio* backup;

BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    BaseAction::errorMsg = errorMsg;
    std::cout << "Error: " << this -> errorMsg << std::endl;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

std::string BaseAction::statusToString() const {
    std::string answer;
    if(getStatus() == COMPLETED){
        answer="Completed";
    }
    else{
        answer="Error: " + getErrorMsg();
    }
    return answer;
}

BaseAction::~BaseAction() {

}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):
    trainerId(id),
    customers(customersList){}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList, std::string& action):
        stringOfAction(action),
        trainerId(id),
        customers(customersList){}

 //destructor
OpenTrainer::~OpenTrainer() {
}

//copy constructor
OpenTrainer::OpenTrainer(const OpenTrainer& other):
        trainerId(other.trainerId),
        stringOfAction(other.stringOfAction){
    for(Customer* c: other.customers){
        customers.push_back(c);
    }
}

//move constructor
OpenTrainer::OpenTrainer(OpenTrainer &&other):
    trainerId(other.trainerId),
    stringOfAction(other.stringOfAction){
        for(Customer* c: other.customers){
            customers.push_back(c);
        }
        for(Customer* c :other.customers){
            c = nullptr;
        }
    }


void OpenTrainer::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t == nullptr){
        error("Workout session does not exist or is already open");
    }
    else if((*t).isOpen()){
        error("Workout session does not exist or is already open");
    }
    else if(t->getAvailableSpots() > 0) {
        int index(0);
        t->openTrainer();
        for(std::vector<Customer*>::iterator it= customers.begin() ; it!=customers.end() && t->getAvailableSpots() > 0; ++it){
            t->addCustomer(*it);
            index++;
        }
        if (customers.size() > index){
            for(std::vector<Customer*>::iterator it= customers.begin() + index ; it!=customers.end() ; ++it){
                delete *it;
                *it = nullptr;
                index++;
            }
        }
        complete();
    }
}


std::string OpenTrainer::toString() const {
    return stringOfAction + " " + statusToString();
}

OpenTrainer *OpenTrainer::clone() {
    std::vector<Customer*> newCustomerList;
    for(Customer* c: customers){
        Customer* newCustomer = (*c).clone();
        newCustomerList.push_back(newCustomer);
    }
    OpenTrainer* ot = new OpenTrainer(trainerId, newCustomerList, stringOfAction);
    return ot;
}

Order::Order(int id) :
        trainerId(id){}

Order::Order(int id, std::string &action):
    trainerId(id),
    stringOfAction(action){}

void Order::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t == nullptr){
        error("Trainer dose not exist or is not open");
    }
    else if(!(t->isOpen())){
        error("Trainer dose not exist or is not open");
    }
    else{
        std::string outputString;
        for(const auto &it : t->getCustomers()){
            t->order(it->getId(), it->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        }
        for(const auto &it :t->getOrders() ){
            Customer* c = t->getCustomer(it.first) ;
            outputString += c->getName() + " is doing " + it.second.getName() + '\n';
        }
        outputString = outputString.substr(0, outputString.size() - 1);
        complete();
        std:: cout << outputString << std::endl ;
    }
}

std::string Order::toString() const {
    return stringOfAction + " " + statusToString();
}

Order *Order::clone() {
    Order* o = new Order(trainerId, stringOfAction);
    return o;
}

Order::~Order() {

}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):
        srcTrainer(src),
        dstTrainer(dst),
        id(customerId){}

MoveCustomer::MoveCustomer(int src, int dst, int customerId, std::string &action):
        srcTrainer(src),
        dstTrainer(dst),
        id(customerId),
        stringOfAction(action){}

void MoveCustomer::act(Studio &studio) {
    Trainer* tsrc = studio.getTrainer(srcTrainer);
    Trainer* tdst = studio.getTrainer(dstTrainer);
    if(tsrc == nullptr || tdst == nullptr){
        error("Cannot move customer");
    }
    else if(!(tsrc->isOpen()) || !(tdst->isOpen())){
        error("Cannot move customer");
    }
    else if(tdst->getAvailableSpots() <= 0 ){
        error("Cannot move customer");
    }
    else{
        Customer* c = tsrc->getCustomer(id);
        if(c == nullptr){
            error("Cannot move customer");
        }
        else{
            tsrc->removeCustomer(id);
            tdst->addCustomer(c);
            std::vector<OrderPair> removedOrder(tsrc->removeOrders(id));
            tdst->addOrders(removedOrder);
            if(tsrc->getAvailableSpots() == 0){
                tsrc->closeTrainer();
            }
            complete();
        }
    }
}

std::string MoveCustomer::toString() const {
    return stringOfAction + " " + statusToString();
}

MoveCustomer *MoveCustomer::clone() {
    MoveCustomer* mc = new MoveCustomer(srcTrainer, dstTrainer, id, stringOfAction);
    return mc;
}

MoveCustomer::~MoveCustomer() {

}

Close::Close(int id):
        trainerId(id){}

Close::Close(int id, std::string &action):
        trainerId(id),
        stringOfAction(action){}

void Close::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t == nullptr){
        error("Trainer dose not exist or is not open");
    }
    else if(!(t->isOpen())){
        error("Trainer dose not exist or is not open");
    }
    else{
        std::string s("Trainer " + std::to_string(trainerId) + " closed. Salary " + std::to_string(t->getSalary()) + "NIS");
        t->closeTrainer();
        std::cout << s << std::endl;
        complete();
    }
}

std::string Close::toString() const {
    return stringOfAction + " " + statusToString();
}

Close *Close::clone() {
    Close* c = new Close(trainerId, stringOfAction);
    return c;
}

Close::~Close() {

}


CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for(Trainer* t : studio.getTrainers()){
        if(t->isOpen()){
            std::string s("Trainer " + std::to_string(t->getId()) + " closed. Salary " + std::to_string(t->getSalary()) + "NIS");
            t->closeTrainer();
            std::cout << s << std::endl;
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    return "closeall " + statusToString();
}

CloseAll *CloseAll::clone() {
    CloseAll* ca = new CloseAll();
    return ca;
}

CloseAll::~CloseAll() {

}

PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workoutOptions(studio.getWorkoutOptions());
    std::string outputString;
    for(const auto &it : workoutOptions){
        outputString += it.getName() + ", " + it.getStringOfType() + ", " + std::to_string(it.getPrice()) + "\n";
    }
    outputString = outputString.substr(0, outputString.size() -2);
    complete();
    std::cout << outputString << std::endl;
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options "  + statusToString();
}

PrintWorkoutOptions *PrintWorkoutOptions::clone() {
    PrintWorkoutOptions* pwo = new PrintWorkoutOptions();
    return pwo;
}

PrintWorkoutOptions::~PrintWorkoutOptions() {

}

PrintTrainerStatus::PrintTrainerStatus(int id):
        trainerId(id){}

PrintTrainerStatus::PrintTrainerStatus(int id, std::string &action):
        trainerId(id),
        stringOfAction(action){}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* t= studio.getTrainer(trainerId);
    std::string statusOfTrainer("closed");
    if(t->isOpen()){
        statusOfTrainer="open";
    }
    std::string outputString("Trainer: " + std::to_string(trainerId) + " status: " + statusOfTrainer + "\n");
    outputString += "Customers: \n";
    std::vector<Customer*> customers= t->getCustomers();
    for(const auto  &it: customers){
        outputString += std::to_string(it->getId()) + " " + it->getName() + "\n";
    }
    outputString += "Orders:\n";
    std::vector<OrderPair> orders = t->getOrders();
    for(const auto &it : orders){
        outputString += it.second.getName()  + " " + std::to_string(it.second.getPrice())+ " " + std::to_string(it.first) + "\n";
    }
    outputString += "Current Trainer's salary: " +  std::to_string(t->getSalary()) + "NIS";
    complete();
    std::cout << outputString << std::endl;
}

std::string PrintTrainerStatus::toString() const {
    return stringOfAction+ " " + statusToString();
}

PrintTrainerStatus *PrintTrainerStatus::clone() {
    PrintTrainerStatus* pts = new PrintTrainerStatus(trainerId, stringOfAction);
    return pts;
}

PrintTrainerStatus::~PrintTrainerStatus() {

}


PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) { //TODO check base action
    std::vector<BaseAction*> actionsLog(studio.getActionsLog());
    std::string outputString;
    for(BaseAction* ba: actionsLog){
        outputString += ba->toString() + "\n";
    }
    outputString = outputString.substr(0, outputString.size() - 1);
    std::cout << outputString << std::endl;
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log " + statusToString();
}

PrintActionsLog *PrintActionsLog::clone() {
    PrintActionsLog* pal = new PrintActionsLog();
    return pal;
}

PrintActionsLog::~PrintActionsLog() {

}

BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    if(backup != nullptr){
        delete backup;
        backup= nullptr;
    }
    backup = new Studio(studio);   //copy constructor
    complete();
}

std::string BackupStudio::toString() const {
    return "backup "  + statusToString();
}

BackupStudio *BackupStudio::clone() {
    BackupStudio* bs = new BackupStudio();
    return bs;
}

BackupStudio::~BackupStudio() {

}

RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr){
        error("No backup available");
    }
    else{
        studio = *backup; //copy assignment operator
        complete();
    }
}

std::string RestoreStudio::toString() const {
    return "restore " + statusToString();
}

RestoreStudio *RestoreStudio::clone() {
    RestoreStudio* rs = new RestoreStudio();
    return rs;
}

RestoreStudio::~RestoreStudio() {

}
