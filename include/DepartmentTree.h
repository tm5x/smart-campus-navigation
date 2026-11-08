#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>
using namespace std;

// One department in the tree
struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;
};

class DepartmentTree {
private:
    DepartmentNode* root;

    void deleteTree(DepartmentNode* node);
    DepartmentNode* findNode(DepartmentNode* node, string name);
    void printHierarchy(DepartmentNode* node, int depth);
    void preorderNode(DepartmentNode* node);
    void postorderNode(DepartmentNode* node);
    int countNodes(DepartmentNode* node);
    int countLeaves(DepartmentNode* node);
    int getHeight(DepartmentNode* node);

public:
    DepartmentTree();
    ~DepartmentTree();

    // A tree owns its nodes
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    bool isEmpty();
    bool addDepartment(string name, string parentName = "");
    void loadFromFile(string filename);
    void searchDepartment(string name);
    void displayHierarchy();
    void preorder();
    void postorder();
    void levelOrder();
    void displayStatistics();
};

#endif

