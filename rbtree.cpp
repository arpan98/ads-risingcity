enum COLOR {
    Red,
    Black,
    DoubleBlack
};

class RBNode {
public:
    BuildingDetails *bd;
    COLOR color;

    RBNode *left, *right, *parent;

    RBNode();

    RBNode(BuildingDetails *bd);

    int getBuildingNum() {
        if (bd == nullptr) {
            return -1;
        } else {
            return bd->buildingNum;
        }
    }

    bool operator<(RBNode other) const {
        return bd->buildingNum < other.bd->buildingNum;
    }
};

RBNode NIL;
RBNode *NULLPTR = &NIL;

RBNode::RBNode() {
    left = right = parent = NULLPTR;
    color = Black;
}

RBNode::RBNode(BuildingDetails *bd) {
   this->bd = bd;
   left = right = parent = NULLPTR;
   color = Red;
}

class RBTree
{
private:
    RBNode *root;

    void rotateLeft(RBNode *&);
    void rotateRight(RBNode *&); 
    
    RBNode* insertBST(RBNode *&, RBNode *&);
    void fixInsert(RBNode *&);

    RBNode *minValueNode(RBNode *&);
    
    RBNode* deleteBST(RBNode *&, int);
    void fixDelete(RBNode *&);

    void inorderBST(RBNode *&);
public:
    RBTree() {
        root = NULLPTR;
    }

    void insertBuilding(BuildingDetails *&bd);
    void deleteBuilding(int buildingNum);

    BuildingDetails get_building_helper(RBNode*, int);
    BuildingDetails getBuilding(int);
    std::vector<BuildingDetails> get_between_helper(RBNode*, int, int, std::vector<BuildingDetails> &);
    std::vector<BuildingDetails> getBetween(int, int, std::vector<BuildingDetails> &);
    void print();
};

RBNode* RBTree::minValueNode(RBNode *&p) {
    RBNode *ptr = p;
    while (ptr->left != NULLPTR)
        ptr = ptr->left;
    return ptr;
}

void RBTree::rotateLeft(RBNode *&p) {
    RBNode *r_child = p->right;
    p->right = r_child->left;

    if (p->right != NULLPTR) {
        p->right->parent = p;
    }

    r_child->parent = p->parent;

    if (p->parent == NULLPTR) {
        root = r_child;
    } else if (p == p->parent->left) {
        p->parent->left = r_child;
    } else {
        p->parent->right = r_child;
    }
    r_child->left = p;
    p->parent = r_child;
}

void RBTree::rotateRight(RBNode *&p) {
    RBNode *l_child = p->left;
    p->left = l_child->right;

    if (p->left != NULLPTR) {
        p->left->parent = p;
    }

    l_child->parent = p->parent;

    if (p->parent == NULLPTR) {
        root = l_child;
    } else if (p == p->parent->left) {
        p->parent->left = l_child;
    } else {
        p->parent->right = l_child;
    }
    l_child->right = p;
    p->parent = l_child;   
}

RBNode* RBTree::insertBST(RBNode *&root, RBNode *&p) {
    if (root == NULLPTR) {
        return p;
    }
    if (*p < *root) {
        root->left = insertBST(root->left, p);
        root->left->parent = root;
    } else {
        root->right = insertBST(root->right, p);
        root->right->parent = root;
    }
    return root;
}

void RBTree::insertBuilding(BuildingDetails *&bd) {
    RBNode *node = new RBNode(bd);
    root = insertBST(root, node);
    fixInsert(node);
}

void RBTree::fixInsert(RBNode *&p) {
    RBNode *parent = NULLPTR, *grandparent = NULLPTR;
    while (p != NULLPTR && p != root && p->color == Red && p->parent->color == Red) {
        parent = p->parent;
        grandparent = parent->parent;
        // std::cout<<p->bd.buildingNum<<" "<<parent->bd.buildingNum<<" "<<grandparent->bd.buildingNum<<"\n";
        if (grandparent == NULLPTR) {
            parent->color = Black;
        } else {
            if (parent == grandparent->left) {
                RBNode *uncle = grandparent->right;
                if (uncle->color == Red) {
                    uncle->color = Black;
                    parent->color = Black;
                    grandparent->color = Red;
                    p = grandparent;
                } else {
                    if (p == parent->right) {
                        rotateLeft(parent);
                        p = parent;
                        parent = p->parent;
                    }
                    rotateRight(grandparent);
                    std::swap(parent->color, grandparent->color);
                    p = parent;
                }
            } else {
                RBNode *uncle = grandparent->left;
                if (uncle->color == Red) {
                    uncle->color = Black;
                    parent->color = Black;
                    grandparent->color = Red;
                    p = grandparent;
                } else {
                    if (p == parent->left) {
                        rotateRight(parent);
                        p = parent;
                        parent = p->parent;
                    }
                    rotateLeft(grandparent);
                    std::swap(parent->color, grandparent->color);
                    p = parent;
                }
            }
        }
    }
}

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

void RBTree::deleteBuilding(int buildingNum) {
    RBNode *node = deleteBST(root, buildingNum);
    fixDelete(node);
}

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

        if (p == p->parent->left) {
            p->parent->left = child;
            if (child != NULLPTR) {
                child->parent = p->parent;
            }
            child->color = Black;
            delete(p);
        } else {
            p->parent->right = child;
            if (child != NULLPTR) {
                child->parent = p->parent;
            }
            child->color = Black;
            delete(p);
        }
    } else {
        RBNode *sibling = NULLPTR;
        RBNode* parent = NULLPTR;
        RBNode *ptr = p;
        ptr->color = DoubleBlack;
        while (ptr != root && ptr->color == DoubleBlack) {
            parent = ptr->parent;
            if (ptr == parent->left) {
                sibling = parent->right;
                if (sibling->color == Red) {
                    sibling->color = Black;
                    parent->color = Black;
                    rotateLeft(parent);
                } else {
                    if (sibling->left->color == Black && sibling->right->color == Black) {
                        sibling->color = Red;
                        if (parent->color == Red)
                            parent->color = Black;
                        else
                            parent->color = DoubleBlack;
                        ptr = parent;
                    } else {
                        if (sibling->right->color == Black) {
                            sibling->left->color = Black;
                            sibling->color = Red;
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        sibling->color = parent->color;
                        parent->color = Black;
                        sibling->right->color = Black;
                        rotateLeft(parent);
                        break;
                    }
                }
            } else {
                sibling = parent->left;
                if (sibling->color == Red) {
                    sibling->color = Black;
                    parent->color = Red;
                    rotateRight(parent);
                } else {
                    if (sibling->left->color == Black && sibling->right->color == Black) {
                        sibling->color = Red;
                        if (parent->color == Red)
                            parent->color = Black;
                        else
                            parent->color = DoubleBlack;
                        ptr = parent;
                    } else {
                        if (sibling->left->color == Black) {
                            sibling->right->color = Black;
                            sibling->color = Red;
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        sibling->color = parent->color;
                        parent->color = Black;
                        sibling->left->color = Black;
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (p == p->parent->left)
            p->parent->left = NULLPTR;
        else
            p->parent->right = NULLPTR;
        delete(p);
        root->color = Black;
    }
}

void RBTree::inorderBST(RBNode *&p) {
    // std::cout<<p->bd.buildingNum;
    if (p == NULLPTR) {
        return;
    }
    inorderBST(p->left);
    std::cout<<p->bd->buildingNum<<" "<<p->color<<"\n";
    inorderBST(p->right);
}

void RBTree::print() {
    std::cout<<"print tree\n";
    // std::cout<<root->getBuildingNum()<<" "<<root->left->getBuildingNum()<<" "<<root->right->getBuildingNum()<<"\n";
    inorderBST(root);
}

BuildingDetails RBTree::get_building_helper(RBNode *root, int buildingNum) {
    if (root == NULLPTR) {
        BuildingDetails b = {0, 0, 0};
        return b;
    }
    if (buildingNum < root->getBuildingNum())
        get_building_helper(root->left, buildingNum);
    if (buildingNum > root->getBuildingNum())
        get_building_helper(root->right, buildingNum);
    if (root->getBuildingNum() == buildingNum)
        return *(root->bd);
}

BuildingDetails RBTree::getBuilding(int buildingNum) {
    return get_building_helper(root, buildingNum);
}

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

std::vector<BuildingDetails> RBTree::getBetween(int l, int r, std::vector<BuildingDetails> &vect) {
    return get_between_helper(root, l, r, vect);
}