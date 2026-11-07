#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include "../include/Facility.h"
#include "../include/HashTable.h"
#include "../include/DepartmentTree.h"
#include "../include/CampusGraph.h"
#include "../include/FacilityDatabase.h"


using namespace std;

// will need a helper function here to deal with stray /r - trent

int main(int argc, char* argv[]) {
    string dataDirectory = "data";

    if (argc > 1) {
        dataDirectory = argv[1];
    }

    //member A - facilites

    //member C - department tree
    DepartmentTree departments;

    //member B - campus graph here

    //member B loads buildings.txt
    //member B loads connections.txt
    //member A loads facilities.txt

    //member C loads departments.txt
    departments.loadFromFile(dataDirectory + "/departments.txt");

    string input;
    int choice;

    //menu running till user exists
    while (true) {

        cout << "\n====================================\n";
        cout << "        SMART CAMPUS SYSTEM\n";
        cout << "====================================\n";

        cout << "1. View all facilities\n";
        cout << "2. Search facility by ID\n";
        cout << "3. View campus graph\n";
        cout << "4. Display graph counts and degrees\n";
        cout << "5. Find reachable buildings (BFS)\n";
        cout << "6. Add department\n";
        cout << "7. Search department\n";
        cout << "8. Display department hierarchy\n";
        cout << "9. Run preorder travrsal\n";
        cout << "10. Exit\n";

        cout << "\nEnter your choice: ";

        if (!getline(cin, input)) {
            break;
        }

        stringstream ss(input);
        char extra;

        //check for invalid menu input
        if (!(ss >> choice) || (ss >> extra)) {
            cout << "Invalid input. Enter a number from 1 to 10.\n";
            continue;
        }

        switch (choice) {

            case 1: {
                //member A - view all facilities
                
            }

            case 2: {
                //member A - search facility by ID

        
            }

            case 3:
                //member B - view campus graph
                break;

            case 4:
                //member B - graph counts and degrees

                break;

            case 5: 
                //member B - BFS reachability

                break;

            case 6: {
                //member C - add a department
                string name;
                string parentName = "";

                cout << "Enter department name: ";
                getline(cin, name);

                if (name.find_first_not_of(" \t\r") == string::npos) {
                    cout << "Error: Department name cannot be empty.\n";
                    break;
                }

                //only ask for parent if tree is not empty
                if (!departments.isEmpty()) {
                    cout << "Enter parent department: ";
                    getline(cin, parentName);

                    if (parentName.find_first_not_of(" \t\r")
                        == string::npos) {
                        cout << "Error: Parent name cannot be empty.\n";
                        break;
                    }
                }

                if (departments.addDepartment(name, parentName)) {
                    cout << "Department added successfully.\n";
                }

                break;
            }

            case 7: {
                //member C - search for a department
                string name;

                cout << "Enter department name: ";
                getline(cin, name);

                if (name.find_first_not_off(" \t\r") == string::npos) {
                    cout << "Error: Department name cannot be empty.\n";
                    break;
                }

                departments.searchDepartment(name);
                break;
            }

            case 8:
                //member C - display department hierarchy
                cout << "\nDepartment Hierarchy:\n";
                departments.displayHierarchy();
                break;

            case 9:
                //member C - preorder traversal
                cout << "\nPreorder Traversal:\n";
                departments.preorder();
                break;

            case 10:
                cout << "Exit program. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
