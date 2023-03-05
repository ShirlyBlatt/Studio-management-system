#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"
class BaseAction;


class Studio{		
public:
	Studio();
    std::vector<int> readCapacityOfTrainer(std::string line, int numOfTrainers);
    Workout createWorkout(int workoutId,std::string &line);
    Studio(const std::string &configFilePath);
    virtual ~Studio();
    Studio(const Studio &other);
    Studio(Studio &&other);
    Studio& operator=(const Studio &other);
    Studio& operator=(Studio &&other);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*> getTrainers();
private:
    bool open;
    int customerId;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif