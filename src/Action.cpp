#include "Action.h"
#include <iostream>

using namespace std;

//Action
BaseAction::BaseAction():errorMsg(), status(){}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}
string BaseAction::getErrorMsg() const{
    return errorMsg;
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

string BaseAction::getStatusString() const
{
    string str = "";
    if(status == ActionStatus::ERROR)
        str += "ERROR";
    else str += "COMPLETED";
    return str;
}

//SimulateStep

SimulateStep:: SimulateStep (int numOfSteps) : BaseAction(), numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse)
{
    //OrderStatus statusO;
for (int t=0 ; t< numOfSteps ; t++)

{      
    vector<Order*> pendingOrders= wareHouse.getAllPending();
      for (unsigned int i=0 ; i<pendingOrders.size();i++)
    {    Order* order =pendingOrders[i];
         //statusO=order->getStatus();   
        for ( Volunteer* volunteer : wareHouse.getAllvolunteers())///???
        {
                if (volunteer->canTakeOrder(*order))
                {
                    volunteer->acceptOrder(*order);
                    wareHouse.addInproces(order);
                    wareHouse.erasePending(order) ;// !!!!
                    order->updateStatusAndId(volunteer->getId());
                    break;
                }
        }
    }
    // step  
    for (Volunteer* volunteer :wareHouse.getAllvolunteers()) {
        volunteer->step();
    } 

       
    //Go through all volunteers and check if they have finished
    for ( Volunteer* volunteer : wareHouse.getAllvolunteers())
    {
        int idC=volunteer->getCompletedOrderId();
        bool fin=(volunteer->getActiveOrderId() == NO_ORDER) && (idC != NO_ORDER );
        if (fin && wareHouse.isInproces(idC)) //finish
        {
            Order* order=&wareHouse.getOrder(idC);
            wareHouse.eraseInproces(order); //remove from in proces
            if (order->getStatus()== OrderStatus::DELIVERING)
            {
                //add to completed order
                wareHouse.addCompleted(order);
                order->setStatus(OrderStatus:: COMPLETED);

            }
            else
            {
                //add to pending
                 wareHouse.addPending(order);
            }    
        }
    } 

    //delete volunteer

    for ( Volunteer* volunteer : wareHouse.getAllvolunteers())
    { 
        if ((volunteer->getActiveOrderId()==NO_ORDER) && !(volunteer->hasOrdersLeft())  )
        {
            //delete him
            wareHouse.eraseVolunteer(volunteer);
        }
    }

}
}

std::string SimulateStep:: toString() const {

    if (getStatus() == ActionStatus::COMPLETED)
    { 
        return "simulateStep " + std::to_string(numOfSteps) +" COMPLETED";
    }
    else return "simulateStep " + std::to_string(numOfSteps)+ " ERROR";
}


SimulateStep* SimulateStep::clone() const {
     return new SimulateStep (*this);
    
}



//AddOrder
AddOrder::AddOrder(int id):BaseAction(), customerId(id){}

void AddOrder::act(WareHouse &wareHouse){
   if (wareHouse.customerExists(customerId))
    {
        if ( wareHouse.getCustomer(customerId).canMakeOrder())
        {   Order* newO=new Order(wareHouse.getOrdercounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
            wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrdercounter());
            newO->setStatus(OrderStatus::PENDING);
            wareHouse.addOrder(newO); 
            /*wareHouse.incrementOrderIds(); */
            complete();

            
        }
        else
         {
            error("cannot place this order");
            std::cout << "Error: Cannot place this order" << std::endl;
        }
    }
    else
    {
        error("cannot place this order");
        std::cout << "Error: Cannot place this order" << std::endl;
       
    }
    
}

AddOrder *AddOrder::clone() const 
{ 
     return new AddOrder(*this);
}
string AddOrder::toString() const
{
    string str = "order " + std::to_string(customerId) + " " + getStatusString();
    return str;
}




//AddCustomer

AddCustomer :: AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders)
    : BaseAction(), customerName(customerName),customerType(customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::  act(WareHouse &wareHouse)
{
    bool soldier=false;
    if (customerType ==CustomerType::Soldier)
    {
        soldier=true;
    }

    wareHouse.addCustomer(customerName,distance, maxOrders,soldier) ;
    complete();
}

AddCustomer *AddCustomer:: clone() const { return new AddCustomer(*this);}

string AddCustomer::  toString() const
{
    string str="";
    if (customerType == CustomerType::Soldier){
     str = "customer " + customerName + " soldier " +std::to_string (distance)+" "+std::to_string(maxOrders)+" " + getStatusString();
    }

    if (customerType == CustomerType::Civilian){
      str = "customer " + customerName + " civilian " +std::to_string(distance)+" "+std::to_string(maxOrders)+ " " +getStatusString();
    }
    return str;
}

//PrintOrderStatus
PrintOrderStatus ::PrintOrderStatus (int id) : BaseAction(),orderId(id) {}

void PrintOrderStatus :: act(WareHouse &wareHouse) 
{
    if(wareHouse.orderExists(orderId)){
        // print all orders and thier status for customer 
        wareHouse.printOrderStat(orderId);
        complete();
    } else {
        error("Error: Order doesn't exist");
        cout<< "Error: Order doesn't exist"  << endl;
    }
    
}

PrintOrderStatus *PrintOrderStatus :: clone() const
 {
    return new PrintOrderStatus(*this);
 }
string PrintOrderStatus :: toString() const 
{

    string str = "orderStatus " +std::to_string(orderId) + " " + getStatusString();
    return str;
    
}
 

//PrintCustomerStatus
 
PrintCustomerStatus :: PrintCustomerStatus(int customerId): customerId(customerId){}
void  PrintCustomerStatus :: act(WareHouse &wareHouse){
    if(wareHouse.customerExists(customerId)){
        // print all orders and thier status for customer 
        wareHouse.printCustomerStat(customerId);
        complete();
    } else {
        error("Customer doesn't exist");
        cout<< "Error: Customer doesn't exist"  << endl;
    }
}

PrintCustomerStatus *PrintCustomerStatus :: clone() const {return new PrintCustomerStatus(*this);}
string PrintCustomerStatus :: toString() const {
    string str = "customerStatus " + std::to_string(customerId) + " "+ getStatusString();
    return str;
     
}


//PrintVolunteerStatus
PrintVolunteerStatus :: PrintVolunteerStatus(int id):BaseAction(), volunteerId(id) {}

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    if(wareHouse.volunteerExists(volunteerId)){
        // print all orders and thier status for customer 
        wareHouse.printVolunteerStat(volunteerId);
        complete();
    } else {
        error("Error: Volunteer doesn't exist");
        cout<< "Error: Volunteer doesn't exist"  << endl;
    }
}

PrintVolunteerStatus *PrintVolunteerStatus ::clone() const {return new PrintVolunteerStatus (*this);}

string PrintVolunteerStatus:: toString() const 
{
    string str = "volunteerStatus " + std::to_string(volunteerId)+ " "+ getStatusString();
    return str;
    
}

//PrintActionsLog 
PrintActionsLog::PrintActionsLog():BaseAction(){}

void  PrintActionsLog::act(WareHouse &wareHouse) {
    wareHouse.printAllActionLogs();
    complete();
}

PrintActionsLog * PrintActionsLog :: clone() const {
    return new PrintActionsLog (*this);
}

string PrintActionsLog::toString() const {
    string str = "log " + getStatusString();
    return str;
}
//Close

Close::Close():BaseAction(){}

void Close::act(WareHouse &wareHouse){
    wareHouse.close();
    wareHouse.printAllOrders();
}

Close *Close::clone() const {
     return new Close (*this);
}

string Close::toString() const {
    string str = "Close " + getStatusString();
    return str;
}

//BackupWareHouse
BackupWareHouse :: BackupWareHouse():BaseAction(){}

void BackupWareHouse::act(WareHouse &wareHouse) 
{
    extern WareHouse* backup;
    if(backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    backup = new WareHouse(wareHouse);
    complete();
}
BackupWareHouse *BackupWareHouse:: clone() const
 {return new BackupWareHouse (*this);}



string BackupWareHouse:: toString() const
 {
    string str = "backup " + getStatusString();
    return str;
   
}

//RestoreWareHouse
RestoreWareHouse :: RestoreWareHouse():BaseAction(){}

void RestoreWareHouse :: act(WareHouse &wareHouse) {
    extern WareHouse* backup;
    if(backup == nullptr){
        error("No backup available"); //check error message
        cout<<"Error: No backup available"<<endl;
    } else {
        wareHouse = *backup;
        complete();
    }
}

RestoreWareHouse *RestoreWareHouse :: clone() const {
    return new RestoreWareHouse (*this);
}

string RestoreWareHouse::toString() const {
    string str = "restore " + getStatusString();
    return str;
}