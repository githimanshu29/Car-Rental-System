#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <unordered_map>

using namespace std;

class Car
{
public:
    int id;
    string model;
    bool is_available;
    double rental_price;
    
     double getRentalPrice() const {
        return rental_price;
    }

    Car(int id, const string &model, double rental_price, bool is_available = true) : id(id), model(model), rental_price(rental_price), is_available(is_available) {}
};
bool isInteger(const string &s)
{
    stringstream ss(s);
    int n;
    return (ss >> n >> ws).eof();
}
bool isAlpha(const string &str) {
    for (char c : str) {
        if (!isalpha(c)) {  // Check if the character is not alphabetic
            return false;
        }
    }
    return true;
}



int getValidPositiveIntegerInput()
{
    int num;

    do
    {
        cin >> num;

        if (cin.fail() || num <= 0 || cin.peek() != '\n') // Check if input is not an integer or is negative or contains decimals
        {
            cout << "Error: Input must be a positive integer." << endl;
            cout << "Enter input again: ";
            cin.clear();                                         
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        else
        {
            return num;
        }
    } while (true);
}


bool isValidName(const string &name)
{
    for (char c : name)
    {
        if (!isalpha(c) && c != ' ')
        {
            return false;
        }
    }
    return true;
}

class User
{
protected:
     vector<int> rented_cars;
    double due = 0.0;

public:
    int id;
    string name;

     int getId() const {
        return id;
    }
    void add_due(double amount)
    {
        due += amount;
    }
    void clear_dues(int amount)
    {
        if (amount >= 0 && amount <= due)
        {
            due -= amount;
        }
    }

    bool has_rented_car(int car_id) const {
        return find(rented_cars.begin(), rented_cars.end(), car_id) != rented_cars.end();
    }
    double get_due() const
    {
        return due;
    }
    User(int id, const string &name) : id(id), name(name) {}

    virtual bool can_rent(int car_id) const = 0;
    virtual void rent_car(int car_id) = 0;
    virtual void return_car(int car_id) = 0;
    virtual vector<int> get_rented_cars() const = 0;
    virtual void save(ofstream &file) const = 0;
};

class Customer : public User
{
public:
    int customer_record;
    int due;
    vector<int> rented_cars;

    Customer(int id, const string &name, int customer_record) : User(id, name), customer_record(customer_record), due(0) {}

    bool can_rent(int car_id) const override
    {
        return rented_cars.size() < customer_record;
    }

    void rent_car(int car_id) override
    {
        rented_cars.push_back(car_id);
    }

    void return_car(int car_id) override
    {
        auto it = find(rented_cars.begin(), rented_cars.end(), car_id);
        if (it != rented_cars.end())
        {
            rented_cars.erase(it);
        }
    }

    vector<int> get_rented_cars() const override
    {
        return rented_cars;
    }

    void save(ofstream &file) const override
    {
        file << id << "," << name << "," << customer_record << "," << due << ",";
        for (int car_id : rented_cars)
        {
            file << car_id << ";";
        }
        file << "\n";
    }
};

class Employee : public User
{
public:
    int employee_record;
    vector<int> rented_cars;

    Employee(int id, const string &name, int employee_record) : User(id, name), employee_record(employee_record) {}

    bool can_rent(int car_id) const override
    {
        return rented_cars.size() < employee_record;
    }

    void rent_car(int car_id) override
    {
        rented_cars.push_back(car_id);
    }

    void return_car(int car_id) override
    {
        auto it = find(rented_cars.begin(), rented_cars.end(), car_id);
        if (it != rented_cars.end())
        {
            rented_cars.erase(it);
        }
    }

    vector<int> get_rented_cars() const override
    {
        return rented_cars;
    }


    void save(ofstream &file) const override
    {
        file << id << "," << name << "," << employee_record << ",";
        for (int car_id : rented_cars)
        {
            file << car_id << ";";
        }
        file << "\n";
    }
};

class Manager : public User
{
public:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Employee> employees;
    void set_rental_price(int car_id, double rental_price)
    {
        auto it = find_if(cars.begin(), cars.end(), [car_id](const Car &car)
                          { return car.id == car_id; });
        if (it != cars.end())
        {
            it->rental_price = rental_price;
            cout << "Rental price for car ID " << car_id << " set to " << rental_price << " successfully." << endl;
        }
        else
        {
            cout << "Car with ID " << car_id << " not found." << endl;
        }
    }

    double get_rental_price(int car_id) const
    {
        auto it = find_if(cars.begin(), cars.end(), [car_id](const Car &car)
                          { return car.id == car_id; });
        if (it != cars.end())
        {
            return it->rental_price;
        }
        else
        {
            cout << "Car with ID " << car_id << " not found." << endl;
            return 0.0;
        }
    }
    void print_existing_users() const
    {
        cout << "Existing Employees:\n";
        for (const auto &employee : employees)
        {
            cout << "Employee ID: " << employee.id << ", Name: " << employee.name << ", Record: " << employee.employee_record << endl;
        }

        cout << "\nExisting Customers:\n";
        for (const auto &customer : customers)
        {
            cout << "Customer ID: " << customer.id << ", Name: " << customer.name << ", Record: " << customer.customer_record << endl;
        }
    }

    Manager(int id, const string &name) : User(id, name) {}

    void add_car(int id, const string &model, double rental_price, bool manuallyAdded = true)
    {
        cars.push_back(Car(id, model, rental_price));

        if (manuallyAdded)
        {
            cout << "Car added successfully." << endl;
        }
    }

    void add_customer(int id, const string &name, int customer_record, bool manuallyAdded = true)
    {
        if (!isValidName(name))
        {
            cout << "Not a valid name." << endl;
            return;
        }

        for (const auto &customer : customers)
        {
            if (customer.id == id)
            {
                cout << "Error: Customer with ID " << id << " already exists." << endl;
                cout << "Either delete the existing customer or choose different id." << endl;
                return;
            }
        }

        customers.push_back(Customer(id, name, customer_record));

        if (manuallyAdded)
        {
            cout << "Customer added successfully." << endl;
        }
    }

    void add_employee(int id, const string &name, int employee_record, bool manuallyAdded = true)

    {

        if (!isValidName(name))
        {
            cout << "Not a valid name." << endl;
            return;
        }

        for (const auto &employee : employees)
        {
            if (employee.id == id)
            {
                cout << "Error: Employee with ID " << id << " already exists." << endl;
                cout << "Either delete the existing employee or choose different id." << endl;
                return;
            }
        }

        employees.push_back(Employee(id, name, employee_record));

        if (manuallyAdded)
        {
            cout << "Employee added successfully." << endl;
        }
    }

    void update_customer(int id, const string &name, int customer_record)
    {
        for (auto &customer : customers)
        {
            if (customer.id == id)
            {
                customer.name = name;
                customer.customer_record = customer_record;
                break;
            }
        }
         
    }
void update_employee(int id, const string& name, int employee_record)
{
    for (auto& employee : employees)
    {
        if (employee.id == id)
        {
            employee.name = name;  // Update employee's name with the new name
            employee.employee_record = employee_record;
            break;
        }
    }
   
}


    void delete_customer(int id)
    {

        for (const auto &car : cars)
        {
            for (const auto &customer : customers)
            {
                if (customer.id == id && find(customer.rented_cars.begin(), customer.rented_cars.end(), car.id) != customer.rented_cars.end())
                {
                    cout << "Error: Customer with ID " << id << " is currently renting a car (Car ID: " << car.id << "). Cannot delete customer." << endl;
                    return;
                }
            }
        }

        customers.erase(remove_if(customers.begin(), customers.end(), [id](const Customer &cust)
                                  { return cust.id == id; }),
                        customers.end());
        cout << "Customer with ID " << id << " deleted successfully." << endl;
    }

    void delete_employee(int id)
    {

        for (const auto &car : cars)
        {
            for (const auto &employee : employees)
            {
                if (employee.id == id && find(employee.rented_cars.begin(), employee.rented_cars.end(), car.id) != employee.rented_cars.end())
                {
                    cout << "Error: Employee with ID " << id << " is currently renting a car (Car ID: " << car.id << "). Cannot delete employee." << endl;
                    return;
                }
            }
        }

        employees.erase(remove_if(employees.begin(), employees.end(), [id](const Employee &emp)
                                  { return emp.id == id; }),
                        employees.end());
        cout << "Employee with ID " << id << " deleted successfully." << endl;
    }
void rent_car(User &user, int car_id)
{
    auto it = find_if(cars.begin(), cars.end(), [car_id](const Car &car)
                      { return car.id == car_id && car.is_available; });
    if (it == cars.end())
    {
        cout << "Error: Car not available for rental." << endl;
        return;
    }

    if (!user.can_rent(car_id))
    {
        cout << "Error: User cannot rent more cars." << endl;
        return;
    }

    it->is_available = false;
    user.rent_car(car_id);

    // Update the user's dues when renting a car (for customers)
    for (auto &customer : customers)
    {
        if (customer.id == user.getId())
        {
            customer.add_due(it->getRentalPrice());
            return; // No need to check other customers once the customer is found
        }
    }

    // Update the user's rental record (for employees)
    for (auto &employee : employees)
    {
        if (employee.id == user.getId())
        {
            employee.add_due(it->getRentalPrice());
            return; // No need to check other employees once the employee is found
        }
    }

    cout << "Car rented successfully." << endl;
}


 void return_car(User &user, int car_id)
{
    auto it = find_if(cars.begin(), cars.end(), [car_id](const Car &car)
                      { return car.id == car_id; });
    if (it == cars.end())
    {
        cout << "Error: Car not found." << endl;
        return;
    }

    // Store the rented cars in a local variable
    const vector<int>& rented_cars = user.get_rented_cars();

    if (find(rented_cars.begin(), rented_cars.end(), car_id) == rented_cars.end())
    {
        cout << "Error: You haven't rented this car." << endl;
        return;
    }

    it->is_available = true;
    user.return_car(car_id);
    cout << "Car returned successfully." << endl;
}




    bool can_rent(int car_id) const override
    {
        return false;
    }

    void rent_car(int car_id) override
    {
        cout << "Managers cannot rent cars." << endl;
    }

    void return_car(int car_id) override
    {
        cout << "Managers cannot return cars." << endl;
    }

    vector<int> get_rented_cars() const override
    {
        return {};
    }
    void print_all_cars_and_rentees() const
    {
        cout << "All Cars and their Rentees:\n";
        for (const auto &car : cars)
        {
            cout << "Car ID: " << car.id << ", Model: " << car.model << ", Rental Price: " << car.rental_price << endl;
            if (!car.is_available)
            {
                cout << "   Rented to:";
                bool rented = false;
                for (const auto &customer : customers)
                {
                    if (find(customer.rented_cars.begin(), customer.rented_cars.end(), car.id) != customer.rented_cars.end())
                    {
                        cout << " Customer ID: " << customer.id << ", Name: " << customer.name;
                        rented = true;
                        break;
                    }
                }
                for (const auto &employee : employees)
                {
                    if (find(employee.rented_cars.begin(), employee.rented_cars.end(), car.id) != employee.rented_cars.end())
                    {
                        cout << " Employee ID: " << employee.id << ", Name: " << employee.name;
                        rented = true;
                        break;
                    }
                }
                if (!rented)
                {
                    cout << " No one";
                }
                cout << endl;
            }
            else
            {
                cout << "   Available for rent" << endl;
            }
        }
    }

    void print_available_cars() const
    {
        cout << "Available Cars:\n";
        for (const auto &car : cars)
        {
            if (car.is_available)
            {
                cout << "Car ID: " << car.id << ", Model: " << car.model << ", Rental Price: " << car.rental_price << endl;
            }
        }
    }

    void save(ofstream &file) const override
    {
        file << id << "," << name << "\n";
        for (const auto &car : cars)
        {
            file << car.id << "," << car.model << "," << car.is_available << "\n";
        }
        for (const auto &customer : customers)
        {
            customer.save(file);
        }
        for (const auto &employee : employees)
        {
            employee.save(file);
        }
    }
};

void save_database(const Manager &manager)
{
    ofstream file("database.txt");
    if (file.is_open())
    {
        manager.save(file);
        file.close();
    }
    else
    {
        cerr << "Error: Unable to open file for writing." << endl;
    }
}

void load_database(Manager &manager)
{
    ifstream file("database.txt");
    if (file.is_open())
    {

        manager.cars.clear();
        manager.customers.clear();
        manager.employees.clear();

        string line;
        getline(file, line);
        if (line.empty())
        {

            cout << "No data found in the database file. Creating a new one." << endl;
            file.close();
            return;
        }

        while (getline(file, line))
        {
            vector<string> tokens;
            string token;
            stringstream ss(line);
            while (getline(ss, token, ','))
            {
                tokens.push_back(token);
            }

            if (tokens.size() == 2)
            {
                int id = stoi(tokens[0]);
                string model = tokens[1];
                bool is_available = stoi(tokens[2]);
                manager.cars.push_back(Car(id, model, is_available));
            }
            else if (tokens.size() == 3)
            {
                int id = stoi(tokens[0]);
                string name = tokens[1];
                int record = stoi(tokens[2]);
                if (isdigit(tokens[3][0]))
                {
                    int due = stoi(tokens[3]);
                    Customer customer(id, name, record);
                    customer.due = due;
                    stringstream ss(tokens[4]);
                    string car_token;
                    while (getline(ss, car_token, ';'))
                    {
                        int car_id = stoi(car_token);
                        customer.rent_car(car_id);
                    }
                    manager.customers.push_back(customer);
                }
                else
                {
                    Employee employee(id, name, record);
                    stringstream ss(tokens[3]);
                    string car_token;
                    while (getline(ss, car_token, ';'))
                    {
                        int car_id = stoi(car_token);
                        employee.rent_car(car_id);
                    }
                    manager.employees.push_back(employee);
                }
            }
        }
        file.close();
        cout << "Database loaded successfully." << endl;
    }
    else
    {

        ofstream newFile("database.txt");
        if (newFile.is_open())
        {

            newFile << manager.id << "," << manager.name << "\n";
            newFile.close();
            cout << "New database file created successfully." << endl;
        }
        else
        {
            cerr << "Error: Unable to create database file." << endl;
        }
    }
}

void print_menu1()
{
    cout << "Menu:\n";
    cout << "1. Rent a car\n";
    cout << "2. Return a car\n";
    cout << "3. see available cars\n";
    cout << "4. see dues\n";
    cout << "5. Clear dues\n";
    cout << "6. Logout\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}
void print_menu()
{
    cout << "Menu:\n";
    cout << "1. Add a car\n";
    cout << "2. Add a customer\n";
    cout << "3. Add an employee\n";
    cout << "4. Delete a car\n";
    cout << "5. Delete a customer\n";
    cout << "6. Delete an employee\n";
    cout << "7. Update Customer\n";
    cout << "8. Update Employee\n";
    cout << "9. See all cars and who they are rented to\n";
    cout << "10. See existing employees and customers\n";
    cout << "11. Logout\n";
    cout << "12. Exit\n";
    cout << "Enter your choice: ";
}

int login(Manager &manager)
{
    int role;
    cout << "Choose your role:\n";
    cout << "1. Customer\n";
    cout << "2. Employee\n";
    cout << "3. Manager\n";
    cout << "4. Exit\n";

    while (true)
    {
        role = getValidPositiveIntegerInput();
        if (role == 4)
            return -1;
        else if (role < 1 || role > 3)
        {
            cout << "Invalid choice. Please choose a valid option (1-4).\n";
            continue;
        }

        int id;
        string name;
        cout << "Enter your ID: ";
        id = getValidPositiveIntegerInput();
        cout << "Enter your name: ";
        cin.ignore(); 
        getline(cin, name);

        if (role == 3)
        {
            if (id == manager.id && name == manager.name)
            {
                return role;
            }
            else
            {
                cout << "Invalid ID or name for Manager. Please try again.\n";
            }
        }
        else
        {
            bool found = false;
            if (role == 2)
            {
                for (const auto &employee : manager.employees)
                {
                    if (employee.id == id && employee.name == name)
                    {
                        found = true;
                        break;
                    }
                }
            }
            else
            {
                for (const auto &customer : manager.customers)
                {
                    if (customer.id == id && customer.name == name)
                    {
                        found = true;
                        break;
                    }
                }
            }
            if (!found)
            {
                cout << "Invalid ID or name. Please try again.\n";
                continue;
            }
            return role;
        }
    }
}

void delete_car(Manager &manager, int delete_car_id)
{
    bool car_found = false;
    for (auto it = manager.cars.begin(); it != manager.cars.end(); ++it)
    {
        if (it->id == delete_car_id)
        {
            if (!it->is_available)
            {

                string rentee_type;
                string rentee_name;
                for (const auto &customer : manager.customers)
                {
                    if (find(customer.rented_cars.begin(), customer.rented_cars.end(), delete_car_id) != customer.rented_cars.end())
                    {
                        rentee_type = "Customer";
                        rentee_name = customer.name;
                        break;
                    }
                }
                if (rentee_type.empty())
                {
                    for (const auto &employee : manager.employees)
                    {
                        if (find(employee.rented_cars.begin(), employee.rented_cars.end(), delete_car_id) != employee.rented_cars.end())
                        {
                            rentee_type = "Employee";
                            rentee_name = employee.name;
                            break;
                        }
                    }
                }
                cout << "Error: Car with ID " << delete_car_id << " is currently rented by " << rentee_type << " " << rentee_name << " and cannot be deleted." << endl;
                return;
            }
            manager.cars.erase(it);
            cout << "Car with ID " << delete_car_id << " deleted successfully.\n";
            car_found = true;
            break;
        }
    }
    if (!car_found)
    {
        cout << "Car with ID " << delete_car_id << " not found.\n";
    }
    save_database(manager);
}

int main()
{
    Manager manager(1, "Daksh");
    load_database(manager);

    if (manager.cars.empty() && manager.customers.empty() && manager.employees.empty())
    {

        manager.add_car(101, "Toyota", 50.0, false);
        manager.add_car(201, "Porsche", 100.0, false);
        manager.add_car(301, "Ford Mustang", 80.0, false);
        manager.add_car(401, "Jaguar", 120.0, false);
        manager.add_car(501, "BMW 3 Series", 90.0, false);

        manager.add_customer(121, "Kushal", 2, false);
        manager.add_customer(122, "Shubham", 3, false);
        manager.add_customer(123, "Anand", 1, false);
        manager.add_customer(124, "Devansh", 2, false);
        manager.add_customer(125, "Ayush", 3, false);

        manager.add_employee(126, "Harshit", 3, false);
        manager.add_employee(127, "Shardul", 2, false);
        manager.add_employee(128, "Prithvi", 4, false);
        manager.add_employee(129, "Ella", 2, false);
        manager.add_employee(120, "Raghav", 3, false);

        save_database(manager);
    }

    int role;
    int choice;
    int userId, carId;
    bool continue_execution = true;

    while (continue_execution)
    {
        role = login(manager);
        switch (role)
        {
        case 1:
        {
            Customer *customer = nullptr;
            int customerId;
            cout << "Confirm your ID: ";
            customerId = getValidPositiveIntegerInput();
            ;
            for (auto &cust : manager.customers)
            {
                if (cust.id == customerId)
                {
                    customer = &cust;
                    break;
                }
            }
            if (customer == nullptr)
            {
                cout << "Invalid customer ID. Please try again.\n";
                continue;
            }
            do
            {
                print_menu1();
                choice = getValidPositiveIntegerInput();

                switch (choice)
                {
                case 1:
                    cout << "Enter the car ID you want to rent: ";
                    carId = getValidPositiveIntegerInput();

                    manager.rent_car(*customer, carId);
                    save_database(manager);
                    break;
                case 2:
                    cout << "Enter the car ID you want to return: ";
                    carId = getValidPositiveIntegerInput();

                    manager.return_car(*customer, carId);
                    save_database(manager);
                    break;
                case 3:
                    manager.print_available_cars();
                    break;

                case 4:
                    cout << "Your due amount is: " << customer->get_due() << endl;
                    break;

                case 5:
                {
                    cout << "Your due amount is: " << customer->get_due() << endl;
                    cout << "Enter the amount you want to pay: ";
                    int amount;
                    cin >> amount;
                    if (cin.fail() || amount < 0 || amount > customer->get_due())
                    {
                        cout << "Invalid input. Please enter a valid non-negative integer less than or equal to your due amount." << endl;
                        cin.clear();                                         
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    }
                    else
                    {
                        
                        string input;
                        getline(cin, input);
                        stringstream ss(input);
                        double test;
                        if (ss >> test)
                        {
                            cout << "Invalid input. Please enter a valid non-negative integer less than or equal to your due amount." << endl;
                            break;
                        }
                        customer->clear_dues(amount);
                        cout << "Dues cleared successfully. Remaining due: " << customer->get_due() << endl;
                        save_database(manager); 
                    }
                    break;
                }

                break;
                case 6:
                    cout << "Logged out.\n";

                    break;
                case 7:

                    cout << "Exiting the program. Goodbye!\n";
                    continue_execution = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 7 && choice != 6);
            break;
        }
        case 2:
        {
            Employee *employee = nullptr;
            int employeeId;
            cout << "Confirm your ID: ";
            employeeId = getValidPositiveIntegerInput();
            ;
            for (auto &emp : manager.employees)
            {
                if (emp.id == employeeId)
                {
                    employee = &emp;
                    break;
                }
            }
            if (employee == nullptr)
            {
                cout << "Invalid employee ID. Please try again.\n";
                continue;
            }
            do
            {
                print_menu1();
                choice = getValidPositiveIntegerInput();

                switch (choice)
                {
                case 1:
                    cout << "Enter the car ID you want to rent: ";
                    carId = getValidPositiveIntegerInput();

                     manager.rent_car(*employee, carId);
                    save_database(manager);
                    break;
                case 2:
                    cout << "Enter the car ID you want to return: ";
                    carId = getValidPositiveIntegerInput();

                    manager.return_car(*employee, carId);
                    save_database(manager);
                    break;

                case 3:
                    manager.print_available_cars();
                    break;

                case 4:
                    cout << "Your due amount is: " << employee->get_due() << endl;
                    break;

                case 5:
                {
                    cout << "Your due amount is: " << employee->get_due() << endl;
                    cout << "Enter the amount you want to pay: ";
                    int amount;
                    cin >> amount;

                    if (cin.fail() || amount < 0 || amount > employee->get_due())
                    {
                        cout << "Invalid input. Please enter a valid non-negative integer less than or equal to your due amount." << endl;
                        cin.clear();                                         
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else
                    {                      
                        string input;
                        getline(cin, input);
                        stringstream ss(input);
                        double test;
                        if (ss >> test)
                        {
                            cout << "Invalid input. Please enter a valid non-negative integer less than or equal to your due amount." << endl;
                            break;
                        }
                        employee->clear_dues(amount);
                        cout << "Dues cleared successfully. Remaining due: " << employee->get_due() << endl;
                        save_database(manager); 
                    }
                    break;
                }

                case 6:
                    cout << "Logged out.\n";

                    break;
                case 7:

                    cout << "Exiting the program. Goodbye!\n";
                    continue_execution = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 7 && choice != 6);
            break;
        }
        case 3:
        {
            do
            {
                print_menu();
                choice = getValidPositiveIntegerInput();
                int id;
                string name;
                double rental_price;
                int record;
                switch (choice)
                {

                case 1:
{
    int id;
    string model;
    double rental_price;
    cout << "Enter car ID: ";
    id = getValidPositiveIntegerInput(); // Validate car ID as positive integer

    cin.ignore();
    cout << "Enter car model: ";
    getline(cin, model);
    cout << "Enter rental price: ";
    rental_price = getValidPositiveIntegerInput(); // Validate rental price as positive double
    manager.add_car(id, model, rental_price);
    save_database(manager);
    break;
}

                case 2:
    cout << "Enter customer ID: ";
    id = getValidPositiveIntegerInput();

    cin.ignore();
    cout << "Enter customer name: ";
    getline(cin, name);
    while (!isAlpha(name)) {
        cout << "Error: Name must contain only alphabetic characters." << endl;
        cout << "Enter customer name: ";
        getline(cin, name);
    }

    cout << "Enter customer rental record: ";
    record = getValidPositiveIntegerInput();
    manager.add_customer(id, name, record);
    save_database(manager);
    break;
case 3:
    cout << "Enter employee ID: ";
    id = getValidPositiveIntegerInput();

    cin.ignore();
    cout << "Enter employee name: ";
    getline(cin, name);
    while (!isAlpha(name)) {
        cout << "Error: Name must contain only alphabetic characters." << endl;
        cout << "Enter employee name: ";
        getline(cin, name);
    }

    cout << "Enter employee rental record: ";
    record = getValidPositiveIntegerInput();
    manager.add_employee(id, name, record);
    save_database(manager);
    break;

                case 4:
                {
                    int delete_car_id;
                    cout << "Enter car ID to delete: ";
                    delete_car_id = getValidPositiveIntegerInput();
                    ;

                    delete_car(manager, delete_car_id);
                    break;
                }
                case 5:
                    cout << "Enter customer ID to delete: ";
                    id = getValidPositiveIntegerInput();

                    manager.delete_customer(id);
                    save_database(manager);
                    break;
                case 6:
                    cout << "Enter employee ID to delete: ";
                    id = getValidPositiveIntegerInput();

                    manager.delete_employee(id);
                    save_database(manager);
                    break;
               case 7:
{
    int customerId, customerRecord;
    string customerName;
    cout << "Enter customer ID to update: ";
    customerId = getValidPositiveIntegerInput();

    cin.ignore();
    cout << "Enter new customer name: ";
    getline(cin, customerName); // Store the new customer name in customerName variable
    cout << "Enter new customer record: ";
    customerRecord = getValidPositiveIntegerInput();  // Read the new customer record directly into customerRecord variable
    manager.update_customer(customerId, customerName, customerRecord); // Pass customerName as argument
    save_database(manager);
    break;
}

               case 8:
{
    int employeeId, employeeRecord;
    string employeeName;
    cout << "Enter employee ID to update: ";
    employeeId = getValidPositiveIntegerInput();

    cin.ignore();
    cout << "Enter new employee name: ";
    getline(cin, employeeName); // Store the new employee name in employeeName variable
    cout << "Enter new employee record: ";
    employeeRecord = getValidPositiveIntegerInput(); // Read the new employee record directly into employeeRecord variable
    manager.update_employee(employeeId, employeeName, employeeRecord); // Pass employeeName as argument
    save_database(manager);
    break;
}

                case 9:
                    manager.print_all_cars_and_rentees();
                    break;
                case 10:
                    manager.print_existing_users();
                    break;
                case 11:
                    cout << "Logged out.\n";
                    break;
                case 12:
                    cout << "Exiting the program. Goodbye!\n";
                    continue_execution = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 12 && choice != 11);
            break;
        }
        case -1:
            cout << "Exiting the program. Goodbye!\n";
            continue_execution = false;
            break;
        default:
            cout << "Invalid role." << endl;
            break;
        }
    }

    return 0;
}
