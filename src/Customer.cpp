#include "Customer.h"

   Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): id(id) , name(name),locationDistance(locationDistance),maxOrders(maxOrders), ordersId(), countnumorders(0){

   }
        
        const string&  Customer :: getName() const {
                return name ;
        }


        int Customer :: getId() const{
                return id ;
        }
        int Customer :: getCustomerDistance() const{
                 return locationDistance ;
        }
        int Customer :: getMaxOrders() const{

                return maxOrders ;

        } //Returns maxOrders
        int Customer :: getNumOrders() const{

                return countnumorders;
        } //Returns num of orders the customer has made so far

        bool Customer :: canMakeOrder() const{

                bool reach = true ;
                int finish = maxOrders - countnumorders;
                if (finish<=0){
                        reach =false;
                }

                return reach ;
        } //Returns true if the customer didn't reach max orders

         const  vector<int> &Customer:: getOrdersIds() const{
                 
                 return ordersId;
         }

        
        int  Customer:: addOrder(int orderId){
                if (canMakeOrder()) {
                              ordersId.push_back(orderId);
                              countnumorders++;
                              return orderId;
                                   } 
                else {
                       return -1;
                } 
                  } //return OrderId if order was added successfully, -1 otherwise

       
         SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
                                 :Customer(id, name, locationDistance, maxOrders) {}

        SoldierCustomer* SoldierCustomer:: clone() const{
                
                return new SoldierCustomer (*this);
        }

        CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
                     : Customer(id, name, locationDistance, maxOrders) {}

        CivilianCustomer* CivilianCustomer:: clone() const{
       
              return new CivilianCustomer (*this);
        }

         int Customer:: numorderleft(){
                int ret=maxOrders - countnumorders;
                return ret;
         }






