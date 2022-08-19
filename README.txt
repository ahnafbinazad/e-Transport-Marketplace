Author: Ahnaf Azad

This is an e-Transport Marketplace created for Ubuntu Linux as a project for my Software Design and Implementation module. 
It was created using C++ for the backend and QMake for its user interface 

The program features use cases for three different users- customers, drivers and the shipping company
Users will be able to register under any of the three account types. 
Drivers will need to verify theier profile by providing their license and lorry registration number

Customers can place orders, for which they will provide information about the product weight, pick up and delivery locations, type of goods, and additional comments. 
They will be provided with a cost for their delivery

Once a customer places an order, the company account users get notified about the order along with the fess they will receive. 
The company can then assign those orders to drivers 

The driver accounts get a notification for the order sent by the company along with teh commission they will recieve, the pick up and delivery location. 
They can then either accpet or reject the order. If rejected, it will be sent to the nex driver.
Drivers can update the status of the order which will be instantly sent to the customers as a notification to their account


To run the program:
1. Import the SDI_Project.sql file into the database to initially setup the database
2. Change the connection information in the mainwindow.cpp file at lines 134, 135, 136, 137
3. The prgram should run after these steps and a green text saying "conected" should appear at the bottom right of the screen
