
  #include "Order.h"

Order :: Order(int id, int customerId, int distance) :  id(id),customerId(customerId) , distance(distance), status(), collectorId(-1), driverId(-1) {

}

int Order ::  getId() const{   
    return id;
}
int Order :: getCustomerId() const {
    
    return customerId ;
}
 
void Order::setStatus(OrderStatus status){
    this->status=status;
}
void Order::setCollectorId(int collectorId){

    this->collectorId=collectorId;
}
void Order::setDriverId(int driverId){
    this->driverId=driverId;
}

int Order ::  getCollectorId() const{
        return collectorId ;
}
int Order :: getDriverId() const{
        return driverId;
}
OrderStatus  Order :: getStatus() const{
        return status ;
}
string  Order :: getStatusString() const{
    string str = "";
    if(status == OrderStatus::COLLECTING) str += "Collecting";
    if(status == OrderStatus::COMPLETED) str += "Completed";
    if(status == OrderStatus::DELIVERING) str += "Delivering";
    if(status == OrderStatus::PENDING) str += "Pending";
    return str;
}
        
const string Order :: toString() const
{
    string s;
    
    s += "OrderId: " + std :: to_string(id);
        s += "\nOrderStatus: " ;
        s += getStatusString();
        s += "\nCustomerID: " ;
        s += std :: to_string(getCustomerId());
        s += "\nCollector: ";
        if (getCollectorId()==-1){
            
            s += "None";
        }
        else {

         s +=std :: to_string(getCollectorId());
         }

         s +="\nDriver: ";

       if (getDriverId()==-1){
            
            s += "None";
        }
        else {

         s +=std :: to_string(getDriverId());
         }
    
    return s;
}

int Order :: getDistance() const
{
    return distance;
}

Order* Order:: clone () const {
    return new Order (*this);
}

 string Order :: printinclose(){
     string s ="";
      s += "OrderID: " + std :: to_string(id);
      s +=", CustomerID: "+ std :: to_string(getCustomerId());
      s +=", Status: "+getStatusString();
    //  s +="\n";
      return s;
 }

void Order::updateStatusAndId(int volunteerId){
    if(status == OrderStatus::PENDING){
        setCollectorId(volunteerId);
        setStatus(OrderStatus::COLLECTING);
    } else {
        setDriverId(volunteerId);
        setStatus(OrderStatus::DELIVERING);

    }
}