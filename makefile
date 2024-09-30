all: WareHouse
WareHouse: Action.o Customer.o main.o Order.o Volunteer.o WareHouse.o
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/WareHouse bin/Action.o bin/Customer.o bin/main.o bin/Order.o bin/Volunteer.o bin/WareHouse.o
	
Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Action.o src/Action.cpp
	
Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Customer.o src/Customer.cpp
	
main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/main.o src/main.cpp
	
Order.o: src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Order.o src/Order.cpp
	
Volunteer.o: src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Volunteer.o src/Volunteer.cpp
	
WareHouse.o: src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/WareHouse.o src/WareHouse.cpp

clean:
	rm -f bin/*.o WareHouse