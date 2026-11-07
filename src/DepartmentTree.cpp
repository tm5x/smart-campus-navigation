#include "DepartmentTree.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <cctype>

using namespace std;

//constructor 
DepartmentTree::DepartmentTree() {
    root = nullptr;
}

//destructor
DepartmentTree::~DepartmentTree() {
    deleteTree(root);
}

//deletes all departments
void DepartmentTree::deleteTree(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < node->children.size(); i++) {
        deleteTree(node->children[i]);
    }
    delete node;
}

//removes extra spaces around a name
string DepartmentTree::trim(string name) {
    int start = 0;
    int end = name.length() - 1;

    while (start < name.length() &&
           isspace((unsigned char)name[start])) {
        start++;
    }

    while (end>= start &&
           isspace((unsigned char)name[end])) {
        end--;
    }

    return name.substr(start, end - start + 1);
}

//checks if 2 names match, ignoring uppercase/lowercase
bool DepartmentTree::sameName(string first, string second) {
    first = trim(first);
    second = trim(second);

    if (first.length() != second.length()) {
        return false;
    }

    for (int i = 0; i < first.length(); i++) {
        if (tolower((unsigned char)first[i]) !=
            tolower((unsigned char)second[i])) {
            return false;
        }
    }

    return true;
}

//finds a department in the tree
DepartmentNode* DepartmentTree::findNode(
    DepartmentNode* node, string name) {

    if (node == nullptr) {
        return nullptr;
    }

    if (sameName(node->name, name)) {
        return node;
    }

    for (int i = 0; i < node->children.size(); i++) {
        DepartmentNode* found =
            findNode(node->children[i], name);

        if (found != nullptr) {
            return found;
        }
    }

    return nullptr;
}

//checks if the tree is empty
bool DepartmentTree::isEmpty() {
    return root == nullptr;
}

//Adds a department
bool DepartmentTree::addDepartment(
    string name, string parentName) {

    name = trim(name);
    parentName = trim(parentName);

    if (name.empty()) {
        cout << "Error: Department name is empty.\n";
        return false;
    }

    if (findNode(root, name) != nullptr) {
        cout << "Error: Department already exists.\n";
        return false;
    }

    //if empty, create the root
    if (root == nullptr) {
        root = new DepartmentNode{name, nullptr, {}};
        return true;
    }

    //find the parent
    DepartmentNode* parent = findNode(root, parentName);

    if (parent == nullptr) {
        cout << "Error: Parent department not found.\n";
        return false;
    }

    //create a new department
    DepartmentNode* newNode =
        new DepartmentNode{name, parent, {}};

    //Connect the department to its parent
    parent->children.push_back(newNode);

    return true;
}

//loads department from a file
void DepartmentTree::loadFromFile(string filename) {
    ifstream file(filename);

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected\n";
        return;
    }

    string line;

    while (getline(file, line)) {
        lineNumber++;

        line = trim(line);

        //skip blank lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        //find the | separating name and parent
        int separator = line.find('|');

        if (separator == string::npos ||
            line.find('|', separator + 1) != string::npos) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Invalid field count.\n";
            rejected++;
            continue;
        }

        string name = trim(line.substr(0, separator));
        string parentName =
            trim(line.substr(separator + 1));

        if (name.empty() || parentName.empty()) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Empty field.\n";
            rejected++;
            continue;
        }

        //the first department must have non as parent
        if (root == nullptr) {
            if (!sameName(parentName, "NONE")) {
                cout << "Warning: " << filename
                     << " line " << lineNumber
                     << ": Root must have NONE as parent.\n";
                rejected++;
                continue;
            }
        }
        else {
            if (sameName(parentName, "NONE")) {
                cout << "Warning: " << filename
                     << " line " << lineNumber
                     << ": Only root can have NONE.\n";
                rejected++;
                continue;
            }
        }

        //check for duplicates
        if (findNode(root, name) != nullptr) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Duplicate department.\n";
            rejected++;
            continue;
        }

        //Make sure the parent exists
        if (root != nullptr &&
            findNode(root, parentName) == nullptr) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Unknown parent " << parentName << endl;
            rejected++;
            continue;
        }

        if (addDepartment(name, parentName)) {
            accepted++;
        }
        else {
            rejected++;
        }
    }

    file.close();

    cout << filename << ": "
         << accepted << " accepted, "
         << rejected << " rejected\n";
}

//searches for a department and prints its information
void DepartmentTree::searchDepartment(string name) {
    name = trim(name);

    if (name.empty()) {
        cout << "Error: Department name is empty.\n";
        return;
    }

    DepartmentNode* found = findNode(root, name);

    if (found == nullptr) {
        cout << "Department not found.\n";
        return;
    }

    int depth = 0;
    DepartmentNode* current = found;

    while (current->parent != nullptr) {
        depth++;
        current = current->parent;
    }

    cout << "Department: " << found->name << endl;

    if (found->parent == nullptr) {
        cout << "Parent: None\n";
    }
    else {
        cout << "Parent: " << found->parent->name << endl;
    }

    cout << "Direct children: "
         << found->children.size() << endl;

    cout << "Depth: " << depth << endl;
}

//Prints a department and its children with indentation
void DepartmentTree::printHierarchy(
    DepartmentNode* node, int depth) {

    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        cout << "    ";
    }

    cout << node->name << endl;

    for (int i = 0; i < node->children.size(); i++) {
        printHierarchy(node->children[i], depth + 1);
    }
}

//displays the department hierarchy
void DepartmentTree::displayHierarchy() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    printHierarchy(root, 0);
}

//pre order (parent first, then children)
void DepartmentTree::preorderNode(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    cout << node->name << endl;

    for (int i = 0; i < node->children.size(); i++) {
        preorderNode(node->children[i]);
    }
}

//Runs preorder traversal
void DepartmentTree::preorder() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";\
        return;
    }

    preorderNode(root);
}

//postorder (children first, then parent)
void DepartmentTree::postorderNode(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < node->children.size(); i++) {
        postorderNode(node->children[i]);
    }

    cout << node->name << endl;
}

//runs postorder traversal
void DepartmentTree::postorder() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    postorderNode(root);
}

//Level-order: one level at a time using a queue
void DepartmentTree::levelOrder() {
    if (root == nullptr) {
       cout << "No departments are loaded.\n";
       return;
    }

    queue<DepartmentNode*> q;
    q.push(root);

    while (!q.empty()) {
        DepartmentNode* current = q.front();
        q.pop();

        cout << current->name << endl;

        //Add the children to the queue
        for (int i = 0; i < current->children.size(); i++) {
            q.push(current->children[i]);
        }
    }
}

//counts the total number of departments
int DepartmentTree::countNodes(DepartmentNode* node) {
    if (node == nullptr) {
        return 0;
    }

    int total = 1;

    for (int i = 0; i < node->children.size(); i++) {
        total += countNodes(node->children[i]);
    }

    return total;
}

//counts departments with no children
int DepartmentTree::countLeaves(DepartmentNode* node) {
    if (node == nullptr) {
        return 0;
    }

    if (node->children.empty()) {
        return 1;
    }

    int total = 0;

    for (int i = 0; i < node->children.size(); i++) {
        total += countLeaves(node->children[i]);
    }

    return total;
}

//finds the height of the tree
int DepartmentTree::getHeight(DepartmentNode* node) {
    if (node == nullptr) {
        return -1;
    }

    int largest = -1;

    for (int i = 0; i < node->children.size(); i++) {
        int height = getHeight(node->children[i]);

        if (height > largest) {
            largest = height;
        }
    }

    return largest + 1;
}

//Displays department statistics
void DepartmentTree::displayStatistics() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        cout << "Total departments: 0\n";
        cout << "Leaf departments: 0\n";
        cout << "Tree height: N/A\n";
        return;
    }

    cout << "Total departments: "
         << countNodes(root) << endl;

    cout << "Leaf departments: "
         << countLeaves(root) << endl;

    cout << "Tree height: "
         << getHeight(root) << endl;
}
