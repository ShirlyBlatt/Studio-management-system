#include "../include/Trainer.h"


Trainer::Trainer(int t_capacity):
        capacity(t_capacity),
        open(false),
        salary(0),
        sessionSalary(0),
        availableSpots(t_capacity){}

Trainer::Trainer(int t_id, int t_capacity):
        id(t_id),
        capacity(t_capacity),
        salary(0),
        sessionSalary(0),
        open(false),
        availableSpots(t_capacity){}

//destructor
Trainer::~Trainer() {
    if (open){
        for(Customer* c : customersList){
            delete c;
            c = nullptr;
        }
    }
}

//copy constructor
Trainer::Trainer(const Trainer &other):
        id(other.id),
        availableSpots(other.availableSpots),
        capacity(other.capacity),
        open(other.open),
        salary(other.salary),
        sessionSalary(other.sessionSalary){
        for (Customer* c : other.customersList) {
            Customer* nc = (*c).clone();
            customersList.push_back(nc);
        }
        for (OrderPair op : other.orderList) {
            orderList.push_back(op);
        }
}

//move constructor
Trainer::Trainer(Trainer &&other):
        id(other.id),
        availableSpots(other.availableSpots),
        capacity(other.capacity),
        open(other.open),
        salary(other.salary),
        sessionSalary(other.sessionSalary){
    for (Customer* c: other.customersList) {
        customersList.push_back(c);
    }
    for(Customer *c : other.customersList){
        c = nullptr;
    }
    for(OrderPair op: other.orderList) {
        orderList.push_back(op);
    }
}

//copy assignment operator
Trainer &Trainer::operator=(const Trainer &other) {
    if( this!= &other ){
        id = other.id;
        availableSpots = other.availableSpots;
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        sessionSalary= other.sessionSalary;
        customersList.clear();
        for (Customer* c: other.customersList) {
            Customer* nc = (*c).clone();
            customersList.push_back(nc);
        }
        orderList.clear();
        for (const auto &it: other.orderList) {
            orderList.push_back(it);
        }
    }
    return *this;
}

//move assignment operator
Trainer &Trainer::operator=(Trainer &&other) {
    if( this!= &other ) {
        id = other.id;
        availableSpots = other.availableSpots;
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        sessionSalary= other.sessionSalary;
        customersList.clear();
        for (Customer* c : other.customersList) {
            customersList.push_back(c);
        }
        for(Customer *c : other.customersList){
            c = nullptr;
        }
        orderList.clear();
        for (OrderPair op: other.orderList) {
            orderList.push_back(op);
        }
    }
    return *this;
}

int Trainer::getCapacity() const{
    return capacity;
}

void Trainer::addCustomer(Customer* customer){
    if(availableSpots > 0 ){
        customersList.push_back(customer);
        availableSpots--;
    }
    customer = nullptr;
}

void Trainer::removeCustomer(int id) {
    int range(-1);
    bool found(false);
    std::vector<Customer*>::iterator c;
    for ( c= customersList.begin(); c != customersList.end() && !found; ++c) {
        if ((*c)->getId() == id) {
            found = true;
        }
        range++;
    }
    if(found){
        customersList.erase(customersList.begin() + range);   //TODO check if problem
        availableSpots++;
    }

}

Customer* Trainer::getCustomer(int id){
    Customer* result;
    for(const auto &it : customersList ){
        if(it->getId() == id){
            result = it;
        }
    }
    return result;
}

std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options) {
    for (const auto &it: workout_ids) {
        for (const auto &jt: workout_options) {
            if (jt.getId() == it) {
                orderList.push_back(std::make_pair(customer_id, jt));
            }
        }
    }
    updateSessionSalary();
}

void Trainer::openTrainer(){
    open = true;
}

void Trainer::closeTrainer(){
    open = false;
    availableSpots = capacity;
    updateSalary();
    for(Customer* c: customersList){
        removeOrders(c->getId());
        delete c;
        c = nullptr;
    }
    customersList.clear();
    orderList.clear();
    sessionSalary=0;
}

int Trainer::getSalary(){
    if(open){
        return salary + sessionSalary;
    }
    return salary;
}

bool Trainer::isOpen(){
    return open;
}

int Trainer::getAvailableSpots() {
    return availableSpots;
}

int Trainer::getId() {
    return id;
}

std::vector<OrderPair> Trainer::removeOrders(int cid) {
    std::vector<OrderPair> newOrderList;
    std::vector<OrderPair> result;
    for(const auto &it : orderList){
        if(it.first == cid){
            result.push_back(it);
            sessionSalary -= it.second.getPrice();
        }
        else{
            newOrderList.push_back(it);
        }
    }
    orderList.swap(newOrderList); //TODO if pair is free of memory
    return result;
}

void Trainer::addOrders(std::vector<OrderPair>& ordersToAdd) {
    for(const auto &it :ordersToAdd ){
        orderList.push_back(it);
    }
    updateSessionSalary();
}

void Trainer::updateSalary() {
    salary += sessionSalary;
}

void Trainer::updateSessionSalary() {
    for(const auto &it: orderList) {
        sessionSalary += it.second.getPrice();
    }
}
















