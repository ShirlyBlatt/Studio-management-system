#include "../include/Customer.h"
#include <algorithm>

Customer::Customer(std::string c_name, int c_id):
    name(c_name),
    id(c_id){}

std::string Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> result;
    for(const auto &it: workout_options){
        if(it.getType() == CARDIO){
            result.push_back(it.getId());
        }
    }
    return result;
}

std::string SweatyCustomer::toString() const{
    return "swt";
}

SweatyCustomer *SweatyCustomer::clone() {
    SweatyCustomer* sc = new SweatyCustomer((*this).getName(), (*this).getId());
    return sc;
}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> result = std::vector<int>();
    int cheapest;
    for(const auto &it :workout_options){
        int temp(it.getPrice());
        if(it.getId() == 0){
            cheapest = it.getPrice();
        }
        if(temp <= cheapest ){
            cheapest = temp;
        }
    }
    result.push_back(cheapest);
    return result;
}
std::string CheapCustomer::toString() const{
    return "chp";
}

CheapCustomer *CheapCustomer::clone() {
    CheapCustomer* cs = new CheapCustomer((*this).getName(), (*this).getId());
    return cs;
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<Workout> temp;
    std::vector<int> result = std::vector<int>();
    for(const auto &it: workout_options){
        if(it.getType() == ANAEROBIC){
            temp.push_back(it);
        }
    }
    // sort
    std::vector<std::pair<int, int>> pairVec;
    for(Workout w: temp){
        std::pair<int, int> p(w.getPrice(), w.getId());
        pairVec.push_back(p);
    }
    std::sort(pairVec.rbegin(), pairVec.rend());
    for(std::pair<int, int> p: pairVec){
        result.push_back(p.second);
    }
    return result;
}

std::string HeavyMuscleCustomer::toString() const{
    return "mcl";
}

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() {
    HeavyMuscleCustomer* hmc = new HeavyMuscleCustomer((*this).getName(), (*this).getId());
    return hmc;
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> tempVec(workout_options);
    std::vector<Workout> sortedWorkout;
    std::vector<int> result;

    // sort
    std::vector<std::pair<int, int>> pairVec;
    for(Workout w: tempVec){
        std::pair<int, int> p(w.getPrice(), w.getId());
        pairVec.push_back(p);
    }
    std::sort(pairVec.begin(), pairVec.end());
    for(std::pair<int, int> p: pairVec){
        for(Workout w: workout_options){
            if (p.second == w.getId()){
                sortedWorkout.push_back(w);
            }
        }
    }

    Workout expensiveMix(0,"",0,MIXED);
    Workout* expensiveMixPtr = &expensiveMix;
    bool foundfMix(false);
    int chepestCardioId(-1);
    int chepestAnaerobicId(-1);
    bool cheapestCardio(false);
    bool cheapestAnaerobic(false);
    for (Workout it: sortedWorkout) {
        if (it.getType() == CARDIO) {
            if (!cheapestCardio) {
                cheapestCardio = true;
                chepestCardioId = it.getId();
            }
        }
        else if (it.getType() == ANAEROBIC) {
            if(!cheapestAnaerobic){
                cheapestAnaerobic = true;
                chepestAnaerobicId = it.getId();
            }
        }
        else if (it.getType() == MIXED) {
            if(expensiveMix.getPrice() < it.getPrice()){
                expensiveMixPtr = &it;
                foundfMix = true;
            }
        }
    }
    if(chepestCardioId !=1 && chepestAnaerobicId != -1 && foundfMix ){
        result.push_back(chepestCardioId);
        result.push_back(expensiveMix.getId());
        result.push_back(chepestAnaerobicId);
    }
    return result;
}

std::string FullBodyCustomer::toString() const{
    return "fbd";
}

FullBodyCustomer *FullBodyCustomer::clone() {
    FullBodyCustomer* fbc = new FullBodyCustomer((*this).getName(), (*this).getId());
    return fbc;
}
