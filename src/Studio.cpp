#include "../include/Studio.h"
#include <fstream>
#include <sstream>


Studio::Studio():
open(false),
customerId(0){}

std::vector<int> Studio::readCapacityOfTrainer(std::string line, int numOfTrainers){
        std::vector<int> result;
        std::vector<std::string> strResult;
        int index(0);
        std::string temp;
        while(index < line.size()){
            if(line[index]== ','){
                strResult.push_back(temp);
                temp = "";
            }
            else if(isdigit(line[index])){
                temp.push_back(line[index]);
            }
            index++;
        }
        strResult.push_back(temp);
        for(const auto &it : strResult){
            result.push_back(stoi(it));
        }
        return result;
}
    Workout Studio::createWorkout(int workoutId,std::string &line){
        //retrieve name out of line
        int index(0);
        int category(0);
        std::string name;
        std::string typeString;
        std::string priceString;
        WorkoutType type;
        while(index < line.size()){
            if (line[index] == ',') {
                category++;
            } else if (!isspace(line[index])) {
                switch (category) {
                    case 0:
                        name.push_back(line[index]);
                        break;
                    case 1:
                        typeString.push_back(line[index]);
                        break;
                    case 2:
                        priceString.push_back(line[index]);
                        break;
                    default:
                        break;
                }
            }
            index++;
        }
        if(typeString == "Anaerobic"){
            type = WorkoutType::ANAEROBIC;
        }
        else if(typeString == "Cardio"){
            type = WorkoutType::CARDIO;
        }
        else if(typeString == "Mixed"){
            type = WorkoutType::MIXED;
        }
        int price(std::stoi(priceString));
        Workout w(workoutId, name, price, type);
        return  w;
}

    Studio::Studio(const std::string &configFilePath):
        open(false),
        customerId(0){
            std::ifstream file(configFilePath);
            std::vector<int> trainerCapacity;
            char inputLine[256];
            bool expecting(false);      //we have reached a data line, we need to read it
            int categoryNum(0);
            int numberOfTrainers(0);
            int workoutId(0);
            while(file) {
                file.getline(inputLine, 256);
                std::string line(inputLine);
                if (line[0] == '#') {
                    expecting = true;
                    continue;
                }
                else if (line[0] == '\0') {
                    expecting = false;
                    continue;
                }
                else if(expecting){
                    if(categoryNum == 0){
                        numberOfTrainers = (int)line[0] - 48;
                        categoryNum++;
                    }
                    else if(categoryNum == 1){
                        trainerCapacity = readCapacityOfTrainer(line, numberOfTrainers);
                        categoryNum++;
                    }
                    else if(categoryNum == 2){
                        workout_options.push_back(createWorkout(workoutId, line));
                        workoutId++;
                    }
                }
            }
            int trainerId(0);
            for(const auto &it : trainerCapacity ){
                Trainer* t = new Trainer(trainerId,it);
                trainerId++;
                trainers.push_back(t);
            }
        }

//destructor
Studio::~Studio() {
    for(Trainer* t : trainers){
        delete t;
        t = nullptr;
    }
    for(BaseAction* ba : actionsLog){
        delete ba;
        ba = nullptr;
    }
}

//copy constructor
Studio::Studio(const Studio &other):
    open(other.open),
    customerId(other.customerId){
    for(Trainer* t : other.trainers){
        Trainer* nt = new Trainer(*t);
        trainers.push_back(nt);
    }
    for(Workout w : other.workout_options){
        Workout wo = *w.clone();
        workout_options.push_back(wo);
    }
    for(BaseAction* ba : other.actionsLog){
        BaseAction* newAction = (*ba).clone();
        actionsLog.push_back(newAction);
    }
}

//move constructor
Studio::Studio(Studio &&other):
        open(other.open),
        customerId(other.customerId){
    for(Trainer* t : other.trainers){
        trainers.push_back(t);
    }
    for(Trainer* t : other.trainers){
        t = nullptr;
    }
    for(Workout w : other.workout_options){
        workout_options.push_back(w);
    }
    for(BaseAction* ba : other.actionsLog){
        actionsLog.push_back(ba);
    }
    for(BaseAction* ba : other.actionsLog){
        ba = nullptr;
    }
}

//copy assignment operator
Studio &Studio::operator=(const Studio &other) {
    if(this != & other){
        open= other.open;
        customerId = other.customerId;
        trainers.clear();
        for(Trainer* t : other.trainers){
            Trainer* nt = new Trainer(*t);
            trainers.push_back(nt);
        }
        workout_options.clear();
        for(Workout w : other.workout_options){
            Workout wo = *w.clone();
            workout_options.push_back(wo);
        }
        actionsLog.clear();
        for(BaseAction* ba : other.actionsLog){
            BaseAction* newAction = (*ba).clone();
            actionsLog.push_back(newAction);
        }
    }
    return *this;
}

//move assignment operator
Studio &Studio::operator=(Studio &&other) {
    if(this != & other){
        open= other.open;
        customerId=other.customerId;
        trainers.clear();
        for(Trainer* t : other.trainers){
            trainers.push_back(t);
        }
        for(Trainer* t : other.trainers){
            t = nullptr;
        }
        workout_options.clear();
        for(const Workout &w : other.workout_options){
            workout_options.push_back(w);
        }
        actionsLog.clear();
        for(BaseAction* ba : other.actionsLog){
            actionsLog.push_back(ba);
        }
        for(BaseAction* ba : other.actionsLog){
            ba = nullptr;
        }
    }
    return *this;
}

void Studio::start(){
        open = true;
        std::cout << "Studio is now open!" << std::endl;
        std::string inputString;
        getline(std::cin, inputString);
        std::string actionToString(inputString);
        inputString = inputString.erase(0,inputString.find_first_not_of(' '));
        std::string action(inputString.substr(0,inputString.find(' ')));
        inputString = inputString.erase(0, action.size());
        inputString = inputString.erase(0,inputString.find_first_not_of(' '));
        while(action != "closeall"){
            if(action =="open"){
                int trainerId(std::stoi(inputString.substr(0,inputString.find(' '))));
                inputString = inputString.erase(0, inputString.substr(0,inputString.find(' ')).size());
                inputString = inputString.erase(0,inputString.find_first_not_of(' '));
                std::vector<Customer *> customersList;
                while(!(inputString.empty())){
                    std::string name(inputString.substr(0, inputString.find(',')));
                    name = name.substr(0, name.find(' '));
                    inputString = inputString.erase(0,inputString.find(',') + 1);
                    inputString = inputString.erase(0,inputString.find_first_not_of(' '));
                    std::string strategy(inputString.substr(0, 3));
                    inputString = inputString.erase(0, strategy.size());
                    inputString = inputString.erase(0,inputString.find_first_not_of(' '));
                    Customer* c;
                    if(strategy == "swt" ){
                        c = new SweatyCustomer(name,customerId);
                    }
                    else if(strategy == "chp" ){
                        c = new CheapCustomer(name,customerId);
                    }
                    else if(strategy == "mcl"){
                        c = new HeavyMuscleCustomer(name,customerId);
                    }
                    else if( strategy == "fbd"){
                        c = new FullBodyCustomer(name,customerId);
                    }
                    customersList.push_back(c);
                    customerId++;
                }
                OpenTrainer* opt = new OpenTrainer(trainerId, customersList, actionToString);
                (*opt).act(*this);
                actionsLog.push_back(opt);
            }
            else if(action == "order"){
                int trainerId(std::stoi(inputString));
                Order* o = new Order(trainerId, actionToString);
                (*o).act(*this);
                actionsLog.push_back(o);
            }
            else if(action == "move"){
                int originTrainerId(std::stoi(inputString.substr(0,inputString.find(' '))));
                inputString.erase(0, inputString.find(' ') + 1);
                int destTrainerId(std::stoi(inputString.substr(0,inputString.find(' '))));
                inputString.erase(0, inputString.find(' ') + 1);
                int cId(std::stoi(inputString));
                MoveCustomer* mc = new MoveCustomer(originTrainerId, destTrainerId, cId, actionToString);
                (*mc).act(*this);
                actionsLog.push_back(mc);
            }
            else if(action == "close"){
                int trainerId(std::stoi(inputString));
                Close* c = new Close(trainerId, actionToString);
                (*c).act(*this);
                actionsLog.push_back(c);
            }
            else if(action == "workout_options" ){
                PrintWorkoutOptions* pwc = new PrintWorkoutOptions();
                (*pwc).act(*this);
                actionsLog.push_back(pwc);
            }
            else if(action == "status"){
                int trainerId(std::stoi(inputString));
                PrintTrainerStatus* pts = new PrintTrainerStatus(trainerId,actionToString );
                (*pts).act(*this);
                actionsLog.push_back(pts);
            }
            else if(action == "log"){
                PrintActionsLog* pal = new PrintActionsLog();
                (*pal).act(*this);
                actionsLog.push_back(pal);
            }
            else if(action == "backup"){
                BackupStudio* bs= new BackupStudio();
                (*bs).act(*this);
                actionsLog.push_back(bs);
            }
            else if(action == "restore"){
                RestoreStudio* rs = new RestoreStudio();
                (*rs).act(*this);
                actionsLog.push_back(rs);
            }
            getline(std::cin, inputString);
            actionToString = inputString;
            inputString = inputString.erase(0,inputString.find_first_not_of(' '));
            action = inputString.substr(0,inputString.find(' '));
            inputString = inputString.erase(0, action.size());
            inputString = inputString.erase(0,inputString.find_first_not_of(' '));
        }
        CloseAll* ca = new CloseAll();
        (*ca).act(*this);
        actionsLog.push_back(ca);
    }

    int Studio::getNumOfTrainers() const{
        return (int)trainers.size();
    }

    Trainer* Studio::getTrainer(int tid){
            Trainer* t(nullptr);
            for(const auto &it : trainers){
                if(it->getId() == tid){
                    t = it;
                }
            }
            return t;
    }

    const std::vector<BaseAction*>& Studio::getActionsLog() const {
        return actionsLog;
    }

    std::vector<Workout>& Studio::getWorkoutOptions(){
        return workout_options;
    }

    std::vector<Trainer *> Studio::getTrainers() {
            return trainers;
    }