
#include "WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Order.h"

using namespace std;
WareHouse ::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), ordercounter(0)
{
    ifstream file(configFilePath);
    string line;
    while (getline(file, line))
    {
        stringstream linestream(line);
        string data;
        string name;
        string type;
        int var1;
        int var2;
        int var3;

        getline(linestream, data, ' ');
        if (data == "customer" || data == "volunteer")
        {
            linestream >> name >> type >> var1 >> var2 >> var3;
            cout << data + " " + name + " " + type + " " << var1 << " " << var2 << " " << var3 << endl;
            if (data == "customer" && type == "civilian")
            {
                customers.push_back(new CivilianCustomer(customerCounter, name, var1, var2));
                customerCounter++;
            }
            else if (data == "customer" && type == "soldier")
            {
                customers.push_back(new SoldierCustomer(customerCounter, name, var1, var2));
                customerCounter++;
            }
            else if (data == "volunteer" && type == "collector")
            {
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, var1));
                volunteerCounter++;
            }
            else if (data == "volunteer" && type == "limited_collector")
            {
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, var1, var2));
                volunteerCounter++;
            }
            else if (data == "volunteer" && type == "driver")
            {
                volunteers.push_back(new DriverVolunteer(volunteerCounter, name, var1, var2));
                volunteerCounter++;
            }
            else if (data == "volunteer" && type == "limited_driver")
            {
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, var1, var2, var3));
                volunteerCounter++;
            }
        }
    }
}

void WareHouse ::start()
{
    open();
    cout << "Warehouse is open!" << endl;
    string line;
    
    while (isOpen)
    {
        getline(cin, line);
        stringstream linestream(line);
        string data;
        getline(linestream, data, ' ');
        
        BaseAction *a;

        if (data == "step")
        {
            int numberofsteps;
            linestream >> numberofsteps;
            a = new SimulateStep(numberofsteps);
        } else if (data == "order")
        {
            int customerId;
            linestream >> customerId;
            a = new AddOrder(customerId);
        }else if (data == "customer")
        {

            string customerName;
            string customerType;
            int customedistance;
            int customemaxOrders;

            linestream >> customerName;
            linestream >> customerType;
            linestream >> customedistance;
            linestream >> customemaxOrders;
            a = new AddCustomer(customerName, customerType, customedistance, customemaxOrders);
        }else if (data == "orderStatus")
        {

            int orderid;
            linestream >> orderid;
            a = new PrintOrderStatus(orderid);
        } else if (data == "customerStatus")
        {

            int customerid;
            linestream >> customerid;
            a = new PrintCustomerStatus(customerid);
        }else if (data == "volunteerStatus")
        {

            int volunteerid;
            linestream >> volunteerid;
            a = new PrintVolunteerStatus(volunteerid);
        }else if (data == "log")
        {

            a = new PrintActionsLog();
        }else if (data == "close")
        {
            a = new Close();
        }else if (data == "backup")
        {
            a = new BackupWareHouse();
        }else if (data == "restore")
        {
            a = new RestoreWareHouse();
        }else {
            std::cerr << "Invalid command\n" << std::endl;
        }

        a->act(*this);
        actionsLog.push_back(a);

        
    }
}

const vector<BaseAction *> &WareHouse ::getActions() const
{
    return actionsLog;
}

void WareHouse ::addOrder(Order *order) // add to pending
{
    ordercounter++;
    pendingOrders.push_back(order);
}

void WareHouse ::addAction(BaseAction *action) //???
{
    actionsLog.push_back(action);
}

Customer &WareHouse ::getCustomer(int customerId) const // customer vector
{
    for (unsigned int i = 0; i < customers.size(); i++)
        if (customers[i]->getId() == customerId)
        {
            return *customers[i];
        }
    static SoldierCustomer a(-1, "fail", -1, -1);
    return a;
}

bool WareHouse ::customerExists(int customerId) const // customer vector
{
    for (unsigned int i = 0; i < customers.size(); i++)
        if (customers[i]->getId() == customerId)
        {
            return true;
        }
    return false;
}

bool WareHouse ::volunteerExists(int volunteerId) const // customer vector
{
    for (unsigned int i = 0; i < volunteers.size(); i++)
        if (volunteers[i]->getId() == volunteerId)
        {
            return true;
        }
    return false;
}

bool WareHouse::orderExists(int orderId) const
{
    for (Order *order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return true;
        }
    }
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return true;
        }
    }
    for (Order *order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return true;
        }
    }
    return false;
}

Volunteer &WareHouse ::getVolunteer(int volunteerId) const // serch in volunteers vector
{
    for (unsigned int i = 0; i < volunteers.size(); i++)
        if (volunteers[i]->getId() == volunteerId)
        {
            return *volunteers[i];
        }
    static CollectorVolunteer a(-1, "fail", -1);
    return a;
}
Order &WareHouse ::getOrder(int orderId) const
{

    for (Order *order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    static Order a(-1, -1, -1);
    return a;
}

void WareHouse ::close()
{
    isOpen = false;
}

void WareHouse ::open()
{
    isOpen = true;
}

void WareHouse::printAllActionLogs()
{
    for (BaseAction *action : actionsLog)
    {
        cout << action->toString() << endl;
    }
}

void WareHouse::printAllOrders()
{
    for (Order *o : pendingOrders)
    {
        cout << o->printinclose() << endl;
    }
    for (Order *o : inProcessOrders)
    {
        cout << o->printinclose() << endl;
    }
    for (Order *o : completedOrders)
    {
        cout << o->printinclose() << endl;
    }
}
const vector<Order *> &WareHouse ::getAllPending() const
{
    return pendingOrders;
}
const vector<Volunteer *> &WareHouse ::getAllvolunteers() const
{
    return volunteers;
}
void WareHouse ::addCustomer(const string &customerName, int distance, int maxOrders, bool soldier) // add to pending
{

    Customer *c;
    if (soldier)
    {
        c = new SoldierCustomer(customerCounter, customerName, distance, maxOrders);
    }
    else // Civilian
    {
        c = new CivilianCustomer(customerCounter, customerName, distance, maxOrders);
    }
    customers.push_back(c);
    customerCounter++;
}

void WareHouse ::erasePending(Order *order)
{
    for (size_t i = 0; i < pendingOrders.size(); ++i)
    {
        if (pendingOrders[i]->getId() == order->getId())
        {
            // Order found, remove it from the vector
            pendingOrders.erase(pendingOrders.begin() + i);
            break; // Assuming there's only one instance of orderToRemove in the vector
        }
    }
}
void WareHouse ::addInproces(Order *order)
{
    inProcessOrders.push_back(order);
}

int WareHouse ::getOrdercounter()
{
    return ordercounter;
}

void WareHouse::incrementOrderIds()
{
    ordercounter++;
}

void WareHouse ::printCustomerStat(int customerId)
{

    string s = "CustomerID : " + to_string(customerId);
    cout << s << endl;

    for (unsigned int i = 0; i < completedOrders.size(); i++)
        if (completedOrders[i]->getCustomerId() == customerId)
        {
            string str = "OrderId: ";
            str += to_string(completedOrders[i]->getId());
            str += "\nOrderStatus: ";
            str += (completedOrders[i]->getStatusString());
            cout << str << endl;
        }

    for (unsigned int i = 0; i < inProcessOrders.size(); i++)
        if (inProcessOrders[i]->getCustomerId() == customerId)
        {
            string str = "OrderId: ";
            str += to_string(inProcessOrders[i]->getId());
            str += "\nOrderStatus: ";
            str += (inProcessOrders[i]->getStatusString());
            cout << str << endl;
        }

    for (unsigned int i = 0; i < pendingOrders.size(); i++)
        if (pendingOrders[i]->getCustomerId() == customerId)
        {
            string str = "OrderId: ";
            str += to_string(pendingOrders[i]->getId());
            str += "\nOrderStatus: ";
            str += (pendingOrders[i]->getStatusString());
            cout << str << endl;
        }

    cout << "numOrdersLeft: " << getCustomer(customerId).numorderleft() << endl;
}

bool WareHouse::isInproces(int orderId)
{
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return true;
        }
    }
    return false;
}

void WareHouse::eraseVolunteer(Volunteer *v)
{
    for (size_t i = 0; i < volunteers.size(); ++i)
    {
        if (volunteers[i]->getId() == v->getId())
        {
            Volunteer *v = volunteers[i];
            // Order found, remove it from the vector
            volunteers.erase(volunteers.begin() + i);
            delete v;
            v = nullptr;
            break; 
        }
    }
}

void WareHouse::printVolunteerStat(int volunteerId)
{

    string s = "";

    for (unsigned int i = 0; i < volunteers.size(); i++)
        if (volunteers[i]->getId() == volunteerId)
        {
            s = volunteers[i]->toString();
        }
    if (s == "")
    {

        s = "Volunteer doesn’t exist";
        cout << s << endl;
    }

    else
    {

        cout << s << endl;
    }
}

void WareHouse::printOrderStat(int orderId)
{

    string s = "";
    s = getOrder(orderId).toString();

    cout << s << endl;
}

int WareHouse::getCustomersCounter()
{
    return customerCounter;
}
int WareHouse::getVolunteersCounter()
{
    return volunteerCounter;
}
void WareHouse::eraseInproces(Order *order)
{
    for (size_t i = 0; i < inProcessOrders.size(); ++i)
    {
        if (inProcessOrders[i]->getId() == order->getId())
        {
            // Order found, remove it from the vector
            inProcessOrders.erase(inProcessOrders.begin() + i);
            break; // Assuming there's only one instance of orderToRemove in the vector
        }
    }
}
void WareHouse::addCompleted(Order *order) { completedOrders.push_back(order); }
void WareHouse::addPending(Order *order) { pendingOrders.push_back(order); }

// rule of 5

// copy constructor
WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), ordercounter(other.ordercounter)
{
    for (BaseAction *a : other.actionsLog)
    {
        this->actionsLog.push_back(a->clone());
    }

    for (Volunteer *a : other.volunteers)
    {
        this->volunteers.push_back(a->clone());
    }

    for (Order *a : other.pendingOrders)
    {
        this->pendingOrders.push_back(a->clone());
    }

    for (Order *a : other.inProcessOrders)
    {
        this->inProcessOrders.push_back(a->clone());
    }
    for (Order *a : other.completedOrders)
    {
        this->completedOrders.push_back(a->clone());
    }

    for (Customer *a : other.customers)
    {
        this->customers.push_back(a->clone());
    }
}

// move constructor

WareHouse::WareHouse(WareHouse &&other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), ordercounter(other.ordercounter)
{

    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;

    for (BaseAction *a : other.actionsLog)
    {
        this->actionsLog.push_back(a->clone());
    }

    for (Volunteer *a : other.volunteers)
    {
        this->volunteers.push_back(a->clone());
    }

    for (Order *a : other.pendingOrders)
    {
        this->pendingOrders.push_back(a->clone());
    }

    for (Order *a : other.inProcessOrders)
    {
        this->inProcessOrders.push_back(a->clone());
    }

    for (Order *a : other.completedOrders)
    {
        this->completedOrders.push_back(a->clone());
    }

    for (Customer *a : other.customers)
    {
        this->customers.push_back(a->clone());
    }

    for (unsigned int i = 0; i < other.actionsLog.size(); i++)
    {
        delete other.actionsLog[i];
        other.actionsLog[i] = nullptr;
    }
    other.actionsLog.clear();
    for (unsigned int i = 0; i < other.volunteers.size(); i++)
    {
        delete other.volunteers[i];
        other.volunteers[i] = nullptr;
    }
    other.volunteers.clear();

    for (unsigned int i = 0; i < other.pendingOrders.size(); i++)
    {
        delete other.pendingOrders[i];
        other.pendingOrders[i] = nullptr;
    }

    other.pendingOrders.clear();
    for (unsigned int i = 0; i < other.inProcessOrders.size(); i++)
    {
        delete other.inProcessOrders[i];
        other.inProcessOrders[i] = nullptr;
    }

    other.inProcessOrders.clear();
    for (unsigned int i = 0; i < other.completedOrders.size(); i++)
    {
        delete other.completedOrders[i];
        other.completedOrders[i] = nullptr;
    }

    other.completedOrders.clear();
    for (unsigned int i = 0; i < other.customers.size(); i++)
    {
        delete other.customers[i];
        other.customers[i] = nullptr;
    }

    other.customers.clear();
}

// copy assignment
WareHouse &WareHouse::operator=(const WareHouse &other)
{

    if (this != &other)
    {
        for (BaseAction *a : actionsLog)
        {
            if (a != nullptr)
            {
                delete a;
                a = nullptr;
            }
        }
        actionsLog.clear();

        for (Volunteer *a : volunteers)
        {
            if (a != nullptr)
            {
                delete a;
                a = nullptr;
            }
        }
        volunteers.clear();

        for (Order *a : pendingOrders)
        {
            if (a != nullptr)
            {
                delete a;
                a = nullptr;
            }
        }
        pendingOrders.clear();

        for (Order *a : inProcessOrders)
        {
            if (a != nullptr)
            {
                delete a;
                a = nullptr;
            }
        }
        inProcessOrders.clear();
        for (Order *a : completedOrders)
        {
            if (a != nullptr)
            {
                delete a;
                a = nullptr;
            }
        }
        completedOrders.clear();
        for (Customer *a : customers)
        {
            if (a != nullptr)
            {
                delete a;
                a = nullptr;
            }
        }
        customers.clear();

        this->isOpen = other.isOpen;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;
        this->ordercounter = other.ordercounter;

        for (BaseAction *a : other.actionsLog)
        {
            this->actionsLog.push_back(a->clone());
        }

        for (Volunteer *a : other.volunteers)
        {
            this->volunteers.push_back(a->clone());
        }

        for (Order *a : other.pendingOrders)
        {
            this->pendingOrders.push_back(a->clone());
        }

        for (Order *a : other.inProcessOrders)
        {
            this->inProcessOrders.push_back(a->clone());
        }

        for (Order *a : other.completedOrders)
        {
            this->completedOrders.push_back(a->clone());
        }

        for (Customer *a : other.customers)
        {
            this->customers.push_back(a->clone());
        }
    }
    return *this;
}
// destructor

WareHouse::~WareHouse()
{
    for (BaseAction *a : actionsLog)
    {
        delete a;
        a = nullptr;
    }
    actionsLog.clear();

    for (Volunteer *a : volunteers)
    {
        delete a;
        a = nullptr;
    }
    volunteers.clear();

    for (Order *a : pendingOrders)
    {
        delete a;
        a = nullptr;
    }
    pendingOrders.clear();

    for (Order *a : inProcessOrders)
    {
        delete a;
        a = nullptr;
    }
    inProcessOrders.clear();
    for (Order *a : completedOrders)
    {
        delete a;
        a = nullptr;
    }
    completedOrders.clear();
    for (Customer *a : customers)
    {
        delete a;
        a = nullptr;
    }
    customers.clear();
}

WareHouse &WareHouse::operator=(WareHouse &&other)
{

    if (this != &other)
    {

        for (BaseAction *a : actionsLog)
        {
            delete a;
            a = nullptr;
        }
        actionsLog.clear();

        for (Volunteer *a : volunteers)
        {
            delete a;
            a = nullptr;
        }
        volunteers.clear();

        for (Order *a : pendingOrders)
        {
            delete a;
            a = nullptr;
        }
        pendingOrders.clear();

        for (Order *a : inProcessOrders)
        {
            delete a;
            a = nullptr;
        }
        inProcessOrders.clear();
        for (Order *a : completedOrders)
        {
            delete a;
            a = nullptr;
        }
        completedOrders.clear();
        for (Customer *a : customers)
        {
            delete a;
            a = nullptr;
        }
        customers.clear();

        this->isOpen = other.isOpen;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;
        this->ordercounter = other.ordercounter;

        for (BaseAction *a : other.actionsLog)
        {
            this->actionsLog.push_back(a->clone());
        }

        for (Volunteer *a : other.volunteers)
        {
            this->volunteers.push_back(a->clone());
        }

        for (Order *a : other.pendingOrders)
        {
            this->pendingOrders.push_back(a->clone());
        }

        for (Order *a : other.inProcessOrders)
        {
            this->inProcessOrders.push_back(a->clone());
        }

        for (Order *a : other.completedOrders)
        {
            this->completedOrders.push_back(a->clone());
        }

        for (Customer *a : other.customers)
        {
            this->customers.push_back(a->clone());
        }

        for (unsigned int i = 0; i < other.actionsLog.size(); i++)
        {
            delete other.actionsLog[i];
            other.actionsLog[i] = nullptr;
        }

        for (unsigned int i = 0; i < other.volunteers.size(); i++)
        {
            delete other.volunteers[i];
            other.volunteers[i] = nullptr;
        }

        for (unsigned int i = 0; i < other.pendingOrders.size(); i++)
        {
            delete other.pendingOrders[i];
            other.pendingOrders[i] = nullptr;
        }

        for (unsigned int i = 0; i < other.inProcessOrders.size(); i++)
        {
            delete other.inProcessOrders[i];
            other.inProcessOrders[i] = nullptr;
        }

        for (unsigned int i = 0; i < other.completedOrders.size(); i++)
        {
            delete other.completedOrders[i];
            other.completedOrders[i] = nullptr;
        }

        for (unsigned int i = 0; i < other.customers.size(); i++)
        {
            delete other.customers[i];
            other.customers[i] = nullptr;
        }
    }
    return *this;
}

            

