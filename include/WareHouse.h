
#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"

// Warehouse responsible for Volunteers, Customers Actions, and Orders.
class BaseAction;
class Volunteer;
class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void printAllActionLogs();
        void printAllOrders();
        const vector<Order*> &getAllPending() const; //??
        const vector<Volunteer*> &getAllvolunteers() const;
        void addCustomer(const string &customerName, int distance,int  maxOrders ,bool soldier);
        void erasePending (Order* order);
        void addInproces(Order* order);
        int getOrdercounter();
        int getCustomersCounter();
        int getVolunteersCounter();

        void printCustomerStat(int customerId);
        void printVolunteerStat(int volunteerId);
        void printOrderStat(int orderId);

        bool volunteerExists(int volunteerId) const;
        bool customerExists(int customerId) const;
        bool orderExists(int orderId) const;
        void incrementOrderIds();

        bool isInproces(int orderId);
        void eraseVolunteer (Volunteer* v);
        void eraseInproces (Order* order);
        void addCompleted(Order* order);
        void addPending(Order* order);

   
    //Rule of 5
              WareHouse(const WareHouse &other);//copy constructor
              
              WareHouse(WareHouse&& other);//move constructor
               
              WareHouse& operator=(const WareHouse& other);//copy assignment
              
              WareHouse& operator=(WareHouse&& other);//move assignment
              ~WareHouse();//destructor
              
    
    

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int ordercounter;
};