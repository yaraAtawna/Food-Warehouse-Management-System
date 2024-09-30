
#include "Volunteer.h"

// Volunteer class implementation

Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const {
    return id;
}

const string &Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return activeOrderId != NO_ORDER;
}



//CollectorVolunteer!!!

CollectorVolunteer::CollectorVolunteer(int id, const string& name, int coolDown):Volunteer(id, name), coolDown(coolDown), timeLeft(-1) {}

CollectorVolunteer *CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() 
{
    bool finish=decreaseCoolDown();
    if (finish) //transfer activeOrderId to completedOrderId
    {
        completedOrderId=activeOrderId;
        activeOrderId=NO_ORDER;
    }
    
}

int CollectorVolunteer:: getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer:: getTimeLeft() const
{
    return timeLeft;
}
bool CollectorVolunteer:: decreaseCoolDown()//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
{
    if (timeLeft > 0) {
        timeLeft--;    
    }
    return timeLeft == 0;
}
bool CollectorVolunteer:: hasOrdersLeft() const 
{
    return true; // CollectorVolunteer always has orders left
}
bool CollectorVolunteer:: canTakeOrder(const Order &order) const 
{
    return !isBusy() && order.getStatus() == OrderStatus::PENDING;
}
void CollectorVolunteer:: acceptOrder(const Order &order) 
{
    if (!isBusy()) {
        activeOrderId = order.getId();
        timeLeft =coolDown;        
    }
}
string CollectorVolunteer:: toString() const 
{
    string s;
    s +=   "VolunteerID: " + std::to_string(getId());
    s += "\nisBusy: ";
    if(isBusy()){
         s += "True"; 
         s += "\nOrderId: ";
         s+= std::to_string(getActiveOrderId());
         s += "\nTimeLeft: "+ std::to_string(timeLeft);
    }
     else{
         s += "False" ;
         s += "\nOrderId: None ";
         s += "\nTimeLeft: None ";
     }
    
    
    s += "\nOrdersLeft: No Limit";
    
    return s;
}


//LimitedCollectorV

LimitedCollectorVolunteer :: LimitedCollectorVolunteer(int id, const string& name, int coolDown, int maxOrders)
      : CollectorVolunteer(id, name,coolDown), maxOrders(maxOrders) , ordersLeft(maxOrders){}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const 
{
    return new LimitedCollectorVolunteer(*this);
}


bool LimitedCollectorVolunteer :: hasOrdersLeft() const  
{
   
    return ordersLeft >0 ;
}
bool LimitedCollectorVolunteer :: canTakeOrder(const Order &order) const 
{
    return !isBusy() && hasOrdersLeft()  && order.getStatus() == OrderStatus::PENDING;
}
void  LimitedCollectorVolunteer :: acceptOrder(const Order &order) 
{
    if (canTakeOrder(order)) {
        ordersLeft--;
        CollectorVolunteer::acceptOrder(order);
    }
}
int LimitedCollectorVolunteer :: getMaxOrders() const
{
    return maxOrders;
}
int LimitedCollectorVolunteer :: getNumOrdersLeft() const
{
    return ordersLeft;
}
string LimitedCollectorVolunteer :: toString() const
{
    
    string s;
    s +=    "VolunteerID: " + std::to_string(getId());
   
   if(isBusy()){
        s +=    "\nisBusy:True  " ;
        s += "\nOrderID: " + std::to_string(activeOrderId); 
        s += "\nTimeLeft: "+ std::to_string(getTimeLeft());
    }
    else{
        s +=    "\nisBusy: false " ;
         s += "\nOrderID: None" ;
         s += "\nTimeLeft: None ";
     }
    s += "\nOrdersLeft: " + std::to_string(hasOrdersLeft());
    return s;
}


//DriverVolunteer

DriverVolunteer :: DriverVolunteer (int id, const string& name, int maxDistance, int distancePerStep)
       : Volunteer(id, name) , maxDistance(maxDistance) , distancePerStep(distancePerStep) , distanceLeft(0) {}

DriverVolunteer *DriverVolunteer::clone() const {  return new DriverVolunteer(*this);}


int DriverVolunteer :: getDistanceLeft() const
{
    return distanceLeft;
}
int DriverVolunteer :: getMaxDistance() const
{
    return maxDistance;
}
int DriverVolunteer :: getDistancePerStep() const
{
    return distancePerStep;
}  
bool DriverVolunteer :: decreaseDistanceLeft() //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
{
    if (distanceLeft>0)
    {
        distanceLeft=distanceLeft-distancePerStep;
    }
    return distanceLeft<=0;
}
bool DriverVolunteer ::  hasOrdersLeft() const 
{
    return true;
}
bool DriverVolunteer :: canTakeOrder(const Order &order) const  // Signal if the volunteer is not busy and the order is within the maxDistance
{
    return !isBusy() && order.getDistance()<= getMaxDistance()  && order.getStatus() == OrderStatus::COLLECTING;
}
void DriverVolunteer :: acceptOrder(const Order &order) // Reset distanceLeft to maxDistance
{
    if (canTakeOrder (order))
    {
        activeOrderId = order.getId();

        distanceLeft = maxDistance; //??
        distanceLeft=order.getDistance();//??
        
    }
}
void DriverVolunteer :: step()  // Decrease distanceLeft by distancePerStep
{
                                                                                                                                                                                                                                                                    
    bool finish=decreaseDistanceLeft();
    if (finish) //transfer activeOrderId to completedOrderId
    {
        completedOrderId=activeOrderId;
        activeOrderId=NO_ORDER;
    }
}
string DriverVolunteer :: toString() const 
{
    string s;
    s +=    "VolunteerID: " + std::to_string(getId());
    if(isBusy())
    {
        s +=    "\nisBusy:True " ;
         s += "\nOrderID: " + std::to_string(activeOrderId); 
         s += "\nTimeLeft: "+ std::to_string(getDistanceLeft()); 
    }
    else{
        s +=    "\nisBusy:false " ;
        s += "\nOrderID: None" ;
        s += "\nTimeLeft: None" ;
     } 
    
    s += "\nOrdersLeft: No Limit";

    return s;
}



//LimitedDriverVolunteer

LimitedDriverVolunteer :: LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
    DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
     return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer :: getMaxOrders() const
{return maxOrders;}
int LimitedDriverVolunteer :: getNumOrdersLeft() const
{
    return ordersLeft;
}
bool LimitedDriverVolunteer :: hasOrdersLeft() const 
{
    return ordersLeft>0;
}
bool LimitedDriverVolunteer :: canTakeOrder(const Order &order) const  // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left
{
   return   !isBusy() && hasOrdersLeft()&& order.getDistance()<= getMaxDistance()   && order.getStatus() == OrderStatus::COLLECTING;
    
}
void LimitedDriverVolunteer :: acceptOrder(const Order &order)  // Reset distanceLeft to maxDistance and decrease ordersLeft
{
    if ( canTakeOrder(order)) {
        ordersLeft--;
        DriverVolunteer::acceptOrder(order);
    }
}
string LimitedDriverVolunteer :: toString() const 
{
    string s;
    s +=    "VolunteerID: " + std::to_string(getId());
    if(isBusy())
    {
        s +=    "\nisBusy:True " ;
         s += "\nOrderID: " + std::to_string(activeOrderId); 
         s += "\nTimeLeft: "+ std::to_string(getDistanceLeft()); 
    }
    else{
        s +=    "\nisBusy:false " ;
        s += "\nOrderID: None" ;
        s += "\nTimeLeft: None" ;
     } 

    s += "\nOrdersLeft: " + hasOrdersLeft();
    return s;
}
