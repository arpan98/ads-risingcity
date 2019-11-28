// Color Enumeration for Red-Black nodes
enum COLOR {
    Red,
    Black,
    DoubleBlack
};

// Red Black Node class
class RBNode {
public:
    // Pointer to Building object
    BuildingDetails *bd;
    COLOR color;

    RBNode *left, *right, *par;

    // Default contstructor
    RBNode();

    // Constructor for initializing with a Building pointer
    RBNode(BuildingDetails *bd);

    // Gets building number from the Node object
    int getBuildingNum() {
        if (bd == nullptr) {
            return -1;
        } else {
            return bd->buildingNum;
        }
    }

    // Overload operator < for Red Black Node, compare buildingNum
    bool operator<(RBNode other) const {
        return bd->buildingNum < other.bd->buildingNum;
    }
};

// Define a NULL Node pointer for ease with null checks in red black algorithms
RBNode NIL;
RBNode *NULLPTR = &NIL;

// Default constructor implementation
RBNode::RBNode() {
    bd = nullptr;
    left = right = par = NULLPTR;
    color = Black;
}

// Constructor with Building as argument 
RBNode::RBNode(BuildingDetails *bd) {
   this->bd = bd;
   left = right = par = NULLPTR;
   color = Red;
}

// Class for red black tree implementation
class RBTree
{
private:
    // Pointer to root element of Red black tree
    RBNode *root;

    // Function to rotate left at argument node
    void rotateLeft(RBNode *&);
    // Function to rotate right at argument node
    void rotateRight(RBNode *&); 
    
    // Function to insert node in BST given root, returns the node at correct position
    RBNode* insertBST(RBNode *&, RBNode *&);
    // Function to fix the tree such that it retains RBT properties
    void fixInsert(RBNode *&);

    // Returns min value node (leftmost node) in a given tree
    RBNode *minValueNode(RBNode *&);
    
    // Delete key from BST
    RBNode* deleteBST(RBNode *&, int);
    // Fix tree after deletion to make it attain RBT properties again
    void fixDelete(RBNode *&);

    // Helper function for inorder traversal of tree (for debugging)
    void inorderBST(RBNode *&);
public:
    // Default constructor
    RBTree() {
        root = NULLPTR;
    }

    // Function to insert building
    void insertBuilding(BuildingDetails *&bd);
    // Function to delete building
    void deleteBuilding(int buildingNum);

    // Helper function for getBuilding (recursively finds value in RBT)
    BuildingDetails get_building_helper(RBNode*, int);
    // Function to find and return a building from RBT
    BuildingDetails getBuilding(int);

    // Recursive helper function for get between
    std::vector<BuildingDetails> get_between_helper(RBNode*, int, int, std::vector<BuildingDetails> &);
    // Function to get all buildings with buildingNum between given range
    std::vector<BuildingDetails> getBetween(int, int, std::vector<BuildingDetails> &);
    // Debug function to print the red black tree
    void print();
};

// Gets the leftmost child node (min value node)
RBNode* RBTree::minValueNode(RBNode *&p) {
    RBNode *ptr = p;
    while (ptr->left != NULLPTR)
        ptr = ptr->left;
    return ptr;
}

// Function for a single left rotation at the given node
void RBTree::rotateLeft(RBNode *&p) {
    RBNode *r_child = p->right;
    p->right = r_child->left;

    if (p->right != NULLPTR) {
        p->right->par = p;
    }

    r_child->par = p->par;

    if (p->par == NULLPTR) {
        root = r_child;
    } else if (p == p->par->left) {
        p->par->left = r_child;
    } else {
        p->par->right = r_child;
    }
    r_child->left = p;
    p->par = r_child;
}

// Function for a single right rotate at the given node, symmetric to rotateLeft
void RBTree::rotateRight(RBNode *&p) {
    RBNode *l_child = p->left;
    p->left = l_child->right;

    if (p->left != NULLPTR) {
        p->left->par = p;
    }

    l_child->par = p->par;

    if (p->par == NULLPTR) {
        root = l_child;
    } else if (p == p->par->left) {
        p->par->left = l_child;
    } else {
        p->par->right = l_child;
    }
    l_child->right = p;
    p->par = l_child;   
}

// Function to insert node into BST starting at root
// Follow standard insertion rules, check left and right child
RBNode* RBTree::insertBST(RBNode *&root, RBNode *&p) {
    if (root == NULLPTR) {
        return p;
    }
    if (*p < *root) {
        root->left = insertBST(root->left, p);
        root->left->par = root;
    } else {
        root->right = insertBST(root->right, p);
        root->right->par = root;
    }
    return root;
}

// Function to insert building into RBT, first calls insertBST, then fixes it
void RBTree::insertBuilding(BuildingDetails *&bd) {
    RBNode *node = new RBNode(bd);
    root = insertBST(root, node);
    fixInsert(node);
}

// Function to fix the RBT at node after a regular BST insertion to maintain RBT properties
void RBTree::fixInsert(RBNode *&p) {
    RBNode *par = NULLPTR, *grandparent = NULLPTR;
    while (p != NULLPTR && p != root && p->color == Red && p->par->color == Red) {
        par = p->par;
        grandparent = par->par;
        if (grandparent == NULLPTR) {
            par->color = Black;
        } else {
            if (par == grandparent->left) {
                RBNode *unc = grandparent->right;
                if (unc->color == Red) {
                    unc->color = Black;
                    par->color = Black;
                    grandparent->color = Red;
                    p = grandparent;
                } else {
                    if (p == par->right) {
                        rotateLeft(par);
                        p = par;
                        par = p->par;
                    }
                    rotateRight(grandparent);
                    std::swap(par->color, grandparent->color);
                    p = par;
                }
            } else {
                RBNode *unc = grandparent->left;
                if (unc->color == Red) {
                    unc->color = Black;
                    par->color = Black;
                    grandparent->color = Red;
                    p = grandparent;
                } else {
                    if (p == par->left) {
                        rotateRight(par);
                        p = par;
                        par = p->par;
                    }
                    rotateLeft(grandparent);
                    std::swap(par->color, grandparent->color);
                    p = par;
                }
            }
        }
    }
}

// Function to find and delete node with buildingNum like in BST
RBNode* RBTree::deleteBST(RBNode *&root, int buildingNum) {
    if (root == NULLPTR) {
        return root;
    }
    if (buildingNum < root->getBuildingNum()) {
        return deleteBST(root->left, buildingNum);
    }
    if (buildingNum > root->getBuildingNum()) {
        return deleteBST(root->right, buildingNum);
    }
    if (root->left == NULLPTR || root->right == NULLPTR) {
        return root;
    }
    RBNode *temp = minValueNode(root->right);
    root->bd = temp->bd;
    return deleteBST(root->right, temp->getBuildingNum());
}

// Function to delete building from RBT, calls deleteBST and then fixes the RBT
void RBTree::deleteBuilding(int buildingNum) {
    RBNode *node = deleteBST(root, buildingNum);
    fixDelete(node);
}

// Function to fix the tree after deleteBST so that it retains RBT properties
void RBTree::fixDelete(RBNode *&p) {
    if (p == NULLPTR) {
        return;
    }
    if (p == root) {
        root = NULLPTR;
        return;
    }
    if (p->color == Red || p->left->color == Red || p->right->color == Red) {
        RBNode *child = p->left != NULLPTR ? p->left : p->right;

        if (p == p->par->left) {
            p->par->left = child;
            if (child != NULLPTR) {
                child->par = p->par;
            }
            child->color = Black;
            delete(p);
        } else {
            p->par->right = child;
            if (child != NULLPTR) {
                child->par = p->par;
            }
            child->color = Black;
            delete(p);
        }
    } else {
        RBNode *sibl = NULLPTR;
        RBNode* par = NULLPTR;
        RBNode *ptr = p;
        ptr->color = DoubleBlack;
        while (ptr != root && ptr->color == DoubleBlack) {
            par = ptr->par;
            if (ptr == par->left) {
                sibl = par->right;
                if (sibl->color == Red) {
                    sibl->color = Black;
                    par->color = Black;
                    rotateLeft(par);
                } else {
                    if (sibl->left->color == Black && sibl->right->color == Black) {
                        sibl->color = Red;
                        if (par->color == Red)
                            par->color = Black;
                        else
                            par->color = DoubleBlack;
                        ptr = par;
                    } else {
                        if (sibl->right->color == Black) {
                            sibl->left->color = Black;
                            sibl->color = Red;
                            rotateRight(sibl);
                            sibl = par->right;
                        }
                        sibl->color = par->color;
                        par->color = Black;
                        sibl->right->color = Black;
                        rotateLeft(par);
                        break;
                    }
                }
            } else {
                sibl = par->left;
                if (sibl->color == Red) {
                    sibl->color = Black;
                    par->color = Red;
                    rotateRight(par);
                } else {
                    if (sibl->left->color == Black && sibl->right->color == Black) {
                        sibl->color = Red;
                        if (par->color == Red)
                            par->color = Black;
                        else
                            par->color = DoubleBlack;
                        ptr = par;
                    } else {
                        if (sibl->left->color == Black) {
                            sibl->right->color = Black;
                            sibl->color = Red;
                            rotateLeft(sibl);
                            sibl = par->left;
                        }
                        sibl->color = par->color;
                        par->color = Black;
                        sibl->left->color = Black;
                        rotateRight(par);
                        break;
                    }
                }
            }
        }
        if (p == p->par->left)
            p->par->left = NULLPTR;
        else
            p->par->right = NULLPTR;
        delete(p);
        root->color = Black;
    }
}

// Function for inorder traversal of tree (for debug)
void RBTree::inorderBST(RBNode *&p) {
    if (p == NULLPTR) {
        return;
    }
    inorderBST(p->left);
    std::cout<<p->bd->buildingNum<<" "<<p->color<<"\n";
    inorderBST(p->right);
}

// Function to print the tree inorder (for debugging)
void RBTree::print() {
    std::cout<<"print tree\n";
    inorderBST(root);
}

// Helper function for getBuilding, recursively finds the buildingNum
BuildingDetails RBTree::get_building_helper(RBNode *root, int buildingNum) {
    std::ofstream output("output_file.txt", std::ios_base::app | std::ios_base::out);
    if (root == NULLPTR) {
        output<<"(0,0,0)\n";
        BuildingDetails b = {0, 0, 0};
        return b;
    }
    if (buildingNum < root->getBuildingNum())
        get_building_helper(root->left, buildingNum);
    if (buildingNum > root->getBuildingNum())
        get_building_helper(root->right, buildingNum);
    if (root->getBuildingNum() == buildingNum) {
        BuildingDetails b = *(root->bd);
        output<<"("<<b.buildingNum<<","<<b.executed_time<<","<<b.total_time<<")\n"; 
        return *(root->bd);
    }
}

// Function to get building with given buildingNum
BuildingDetails RBTree::getBuilding(int buildingNum) {
    return get_building_helper(root, buildingNum);
}

// Helper function for getBetween, recursively checks if root buildingNum is within range
// Returns vector of buildings in the range
std::vector<BuildingDetails> RBTree::get_between_helper(RBNode *root, int l, int r, std::vector<BuildingDetails> &vect) {
    if (root == NULLPTR)
        return {};
    if (l < root->getBuildingNum())
        get_between_helper(root->left, l, r, vect);
    if (l <= root->getBuildingNum() && r >= root->getBuildingNum())
        vect.push_back(*(root->bd));
    if (r > root->getBuildingNum())
        get_between_helper(root->right, l, r, vect);
    return vect;
}

// Function for getting all buildings in range
// Returns vector of buildings
std::vector<BuildingDetails> RBTree::getBetween(int l, int r, std::vector<BuildingDetails> &vect) {
    return get_between_helper(root, l, r, vect);
}