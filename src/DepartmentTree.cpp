#include "../include/DepartmentTree.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <cctype>

using namespace std;

// Removes extra spaces around text
static string trim(const string& s) {
    string whitespace = " \t\r\n";

    size_t start = s.find_first_not_of(whitespace);

    if (start == string::npos) {
        return "";
    }

    size_t end = s.find_last_not_of(whitespace);

    return s.substr(start, end - start + 1);
}

// Converts text to uppercase
static string toUpper(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        text[i] = (char)toupper((unsigned char)text[i]);
    }

    return text;
}

// Splits a line 
static vector<string> split(
    const string& line, char delimiter) {

    vector<string> fields;
    string current = "";

    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == delimiter) {
            fields.push_back(current);
            current = "";
        }
        else {
            current += line[i];
        }
    }

    fields.push_back(current);
    return fields;
}

// Checks for blank lines and comments
static bool shouldSkipLine(const string& line) {
    string t = trim(line);

    return t.empty() || t[0] == '#';
}

// Constructor 
DepartmentTree::DepartmentTree() {
    root = nullptr;
}

// Destructor 
DepartmentTree::~DepartmentTree() {
    deleteTree(root);
}

// Deletes all departments
void DepartmentTree::deleteTree(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        deleteTree(node->children[i]);
    }

    delete node;
}

// Finds a department in the tree
DepartmentNode* DepartmentTree::findNode(
    DepartmentNode* node, string name) {

    if (node == nullptr) {
        return nullptr;
    }

    // Compare names ignoring capitalization
    if (toUpper(node->name) == toUpper(trim(name))) {
        return node;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        DepartmentNode* found =
            findNode(node->children[i], name);

        if (found != nullptr) {
            return found;
        }
    }

    return nullptr;
}

// Checks if the tree is empty
bool DepartmentTree::isEmpty() {
    return root == nullptr;
}

// Adds a new department
bool DepartmentTree::addDepartment(
    string name, string parentName) {

    name = trim(name);
    parentName = trim(parentName);

    if (name.empty()) {
        cout << "Error: Department name cannot be empty.\n";
        return false;
    }

    // Check for duplicate departments
    if (findNode(root, name) != nullptr) {
        cout << "Error: Department already exists: "
             << name << endl;
        return false;
    }

    // Create the root if the tree is empty
    if (root == nullptr) {
        root = new DepartmentNode{name, nullptr, {}};
        return true;
    }

    // Make sure a parent was entered
    if (parentName.empty()) {
        cout << "Error: Parent department cannot be empty.\n";
        return false;
    }

    // Find the parent department
    DepartmentNode* parent = findNode(root, parentName);

    if (parent == nullptr) {
        cout << "Error: Parent department not found: "
             << parentName << endl;
        return false;
    }

    // Create the new department
    DepartmentNode* newNode =
        new DepartmentNode{name, parent, {}};

    // Connect the department to its parent
    parent->children.push_back(newNode);

    return true;
}

// Loads departments from the file
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

        if (shouldSkipLine(line)) {
            continue;
        }

        vector<string> fields = split(line, '|');

        // Each line should have exactly two fields
        if (fields.size() != 2) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Wrong number of fields.\n";

            rejected++;
            continue;
        }

        string name = trim(fields[0]);
        string parentName = trim(fields[1]);

        // Check for empty fields
        if (name.empty() || parentName.empty()) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Empty department name or parent.\n";

            rejected++;
            continue;
        }

        // First department has to be the root
        if (root == nullptr &&
            toUpper(parentName) != "NONE") {

            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": First department must have NONE as parent.\n";

            rejected++;
            continue;
        }

        // Only the root can have NONE as parent
        if (root != nullptr &&
            toUpper(parentName) == "NONE") {

            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Only the root can have NONE as parent.\n";

            rejected++;
            continue;
        }

        // Check for duplicate departments
        if (findNode(root, name) != nullptr) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Duplicate department: "
                 << name << endl;

            rejected++;
            continue;
        }

        // Make sure the parent exists
        if (root != nullptr &&
            findNode(root, parentName) == nullptr) {

            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Unknown parent: "
                 << parentName << endl;

            rejected++;
            continue;
        }

        // Add the valid department
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

// Searches and displays department information
void DepartmentTree::searchDepartment(string name) {
    name = trim(name);

    if (name.empty()) {
        cout << "Error: Department name cannot be empty.\n";
        return;
    }

    DepartmentNode* found = findNode(root, name);

    if (found == nullptr) {
        cout << "Department not found: "
             << name << endl;
        return;
    }

    // Calculate depth by moving toward the root
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
        cout << "Parent: "
             << found->parent->name << endl;
    }

    cout << "Direct children: "
         << found->children.size() << endl;

    cout << "Depth: " << depth << endl;
}

// Prints departments with four spaces per level
void DepartmentTree::printHierarchy(
    DepartmentNode* node, int depth) {

    if (node == nullptr) {
        return;
    }

    // Print indentation
    for (int i = 0; i < depth; i++) {
        cout << "    ";
    }

    cout << node->name << endl;

    // Print each child department
    for (size_t i = 0; i < node->children.size(); i++) {
        printHierarchy(node->children[i], depth + 1);
    }
}

// Displays the department hierarchy
void DepartmentTree::displayHierarchy() {
    if (isEmpty()) {
        cout << "No departments are loaded.\n";
        return;
    }

    printHierarchy(root, 0);
}

// Preorder visits the parent before its children
void DepartmentTree::preorderNode(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    cout << node->name << endl;

    for (size_t i = 0; i < node->children.size(); i++) {
        preorderNode(node->children[i]);
    }
}

// Runs preorder traversal
void DepartmentTree::preorder() {
    if (isEmpty()) {
        cout << "No departments are loaded.\n";
        return;
    }

    preorderNode(root);
}

// Postorder visits children before the parent
void DepartmentTree::postorderNode(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        postorderNode(node->children[i]);
    }

    cout << node->name << endl;
}

// Runs postorder traversal
void DepartmentTree::postorder() {
    if (isEmpty()) {
        cout << "No departments are loaded.\n";
        return;
    }

    postorderNode(root);
}

// Level-order visits one level at a time
void DepartmentTree::levelOrder() {
    if (isEmpty()) {
        cout << "No departments are loaded.\n";
        return;
    }

    queue<DepartmentNode*> q;
    q.push(root);

    while (!q.empty()) {
        DepartmentNode* current = q.front();
        q.pop();

        cout << current->name << endl;

        // Add children to the queue
        for (size_t i = 0; i < current->children.size(); i++) {
            q.push(current->children[i]);
        }
    }
}

// Counts all departments
int DepartmentTree::countNodes(DepartmentNode* node) {
    if (node == nullptr) {
        return 0;
    }

    int total = 1;

    for (size_t i = 0; i < node->children.size(); i++) {
        total += countNodes(node->children[i]);
    }

    return total;
}

// Counts departments without children
int DepartmentTree::countLeaves(DepartmentNode* node) {
    if (node == nullptr) {
        return 0;
    }

    if (node->children.empty()) {
        return 1;
    }

    int total = 0;

    for (size_t i = 0; i < node->children.size(); i++) {
        total += countLeaves(node->children[i]);
    }

    return total;
}

// Calculates the height of the tree
int DepartmentTree::getHeight(DepartmentNode* node) {
    if (node == nullptr) {
        return -1;
    }

    int largest = -1;

    for (size_t i = 0; i < node->children.size(); i++) {
        int height = getHeight(node->children[i]);

        if (height > largest) {
            largest = height;
        }
    }

    return largest + 1;
}

// Displays department statistics
void DepartmentTree::displayStatistics() {
    if (isEmpty()) {
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

