# Car-Rental
<h1>How to run?</h1>

- Save the files in a directory, say x.
- Open your terminal and go to directory x.
- Type the following commands in the terminal:
  
  
  g++ CarRental.c++ -o CarRental.exe
  ./CarRental.exe
  
- You will have successfully started the program.

<h1>How are the Databases setup?</h1>

- There are only one databases, namely - database.txt 
- The attributes in a database are separated by -.
- The database for Cars stores the attributes: Car ID , Model Name ,  Rent Price ,  (in order)
- The database for Customers stores the attributes: Customer ID , Customer Name, Customer Record (Number of cars rented ),IDs of the cars rented (in order)
- The database for Employees stores the attributes: Employee ID , Employee Name ,Employee Record (Number of cars rented ),  IDs of the cars rented (in order)
- There must not be a existing database before running the program
<h1>Dummy Data for Databases</h1>
<h3>NOTE.In Dummy database first id is given and then username is given i.e,(id,username)</h3>
<h2>2. Customer Database</h2>
1,Daksh


<h2>1. Car Database</h2>


101,Toyota,1
201,Porsche,1
301,Ford Mustang,1
401,Jaguar,1
501,BMW 3 Series,1


<h2>2. Customer Database</h2>


121,Kushal,2,0,
122,Shubham,3,0,
123,Anand,1,0,
124,Devansh,2,0,
125,Ayush,3,0,



<h2>3. Employee Database</h2>


126,Harshit,3,;
127,Shardul,2,
128,Prithvi,4,
129,Ella,2,
120,Raghav,3,


<h1>Working</h1>

- The program starts and loads data from databases so that it can be used easily for the working of the program.
- The program stores this loaded data using STL of C++ for easy manipulation.
- The databases are update dynamically as the program runs, for e.g., if some car has been rented then its attributes will be updated dynamically in the database as well in real time.
- There is only one Manager with username Daksh and ID 1.
- The number of Cars, Customers and Employees can vary.
- The IDs for Cars, Customers and Employees are unique.
- Throughout the program, only Username and UserID is provided as input.
- Two entities can have the same name bu not same ID
- Manager can add/update/delete/search Cars, Customers and Employees.
- Manager can view status of Cars, Customers and Employees.
- Customers/Employees can view all Cars and Cars rented by them.
- Customers/Employees can view and clear dues.
- Customers/Employees can rent and return Cars.
- Customer/Employee rental record differ for different users.
- All IDs will take only positive integer as input.
- If you want to run program again after using one time then you have to first delete the existing database first then program will work.

<h1>Structure of Program</h1>

- There is Parent class User which has three derived classes - Customer, Employee and Manager.
- There is a class for Car.
- There is a login page which asks for the role of user.
- The type of role decides the options shown on the screen.
- Vector of objects cust, emp and car are used for easy access to data.
- Unique ID for all objects are created using static int values.
- friend functions are used for a function that needs to access the private/protected members of a class.

<h1>Assumptions</h1>

- The user does not enter unnecessary whitespaces whenever he enters a value.
- A customer does not register himself. Only the manager can register a Customer.
- Whenever the user is asked for a single character, if he enters a string then it is considered as a multiple character input so the string will act as a stream of characters.
- Customer/Employee can clear their own dues.
- Manager cannot delete a Car if it is currently rented.
- Manager cannot delete a Customer/Employee if they currently own a Car.
- Manager can delete a Customer/Employee if they currently have some dues
