#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>

struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;

    DepartmentNode(const std::string& departmentName, DepartmentNode* parentNode);
};

class DepartmentTree {
private:
    DepartmentNode* root;

    //Private helpers for tree operations
    DepartmentNode* findNode(DepartmentNode* current, const std::string& name) const;
    void deleteNodes(DepartmentNode* current);
    void displayNode(DepartmentNode* current, int depth) const;
    void preorderNode(DepartmentNode* current) const;
    void postorderNode(DepartmentNode* current) const;
    int countNodes(DepartmentNode* current) const;
    int countLeaves(DepartmentNode* current) const;
    static std::string trim(const std::string& text);
    static std::string lowercase(const std::string& text);

public:
    DepartmentTree();
    ~DepartmentTree();

    //prevent copying a tree that owns raw pointers
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    bool isEmpty() const;
    bool addDepartment(const std::string& name), const std::string& parentName);
    void loadFromFile(const std::string& filename);
    void searchDepartment(const std::string& name) const;
    void displayHierarchy() const;
    void preorder() const;
    void postorder() const;
    void levelOrder() const;
    void displayStatistics() const;
};

#endif
