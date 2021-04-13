/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */

#include "dtree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
DTree::~DTree()
{
    clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree &DTree::operator=(const DTree &rhs)
{
    if (_root == rhs._root)
        return *this;

    clear();
    _root = helpAssignment(rhs._root);

    return *this;
}
/**
 * Helper funtion for assignment operator.
 */
DNode *DTree::helpAssignment(DNode *rhs)
{
    if (rhs == nullptr)
        return nullptr;

    DNode *root = new DNode();
    root->_account = rhs->getAccount();
    root->_numVacant = rhs->getNumVacant();
    root->_size = rhs->getSize();
    root->_vacant = rhs->isVacant();
    root->_left = helpAssignment(rhs->_left);
    root->_right = helpAssignment(rhs->_right);

    return root;
}

/**
 * Dynamically allocates a new DNode in the tree. 
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct)
{
    // if it exist retrieve return false
    DNode *node = new DNode(newAcct);
    if (retrieve(node->getDiscriminator()) != nullptr)
    {
        delete node;
        return false;
    }
    delete node;
    helpInsert(newAcct, _root);
    if (checkImbalance(_root))
        rebalance(_root);
    return true;
}

/**
 * Helper funtion for insert.
 */
bool DTree::helpInsert(Account newAcct, DNode *&root)
{
    if (root == nullptr)
    {
        root = new DNode(newAcct);
        updateSize(root);
        return true;
    }
    if ((root->_left == nullptr && root->_right == nullptr) && root->isVacant())
    {
        root->_vacant = false;
        root->_account = newAcct;
        updateNumVacant(root);
        return true;
    }
    if (root->_left == nullptr)
        if (root->isVacant() == true && root->_right->getDiscriminator() > newAcct.getDiscriminator())
        {
            root->_vacant = false;
            root->_account = newAcct;
            updateNumVacant(root);
            return true;
        }
    if (root->_right == nullptr)
        if (root->isVacant() == true && root->_left->getDiscriminator() < newAcct.getDiscriminator())
        {
            root->_vacant = false;
            root->_account = newAcct;
            updateNumVacant(root);
            return true;
        }
    if (root->isVacant() == true &&
        (root->_left->getDiscriminator() < newAcct.getDiscriminator() &&
         root->_right->getDiscriminator() > newAcct.getDiscriminator()))
    {
        root->_vacant = false;
        root->_account = newAcct;
        updateNumVacant(root);
        return true;
    }

    if (root->getDiscriminator() < newAcct.getDiscriminator())
    {
        if (helpInsert(newAcct, root->_right) == true)
        {
            updateSize(root);
            return true;
        }
        else
        {
            updateNumVacant(root);
            return false;
        }
    }

    if (root->getDiscriminator() > newAcct.getDiscriminator())
    {
        if (helpInsert(newAcct, root->_left) == true)
        {
            updateSize(root);
            return true;
        }
        else
        {
            updateNumVacant(root);
            return false;
        }
    }

    return false;
}

/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool DTree::remove(int disc, DNode *&removed)
{
    removed = helpRemove(disc, _root);
    if (removed == nullptr)
        return false;

    return true;
}

/**
 * Helper funtion for remove.
 */
DNode *DTree::helpRemove(int disc, DNode *&root)
{
    if (root == nullptr)
        return nullptr;

    if (disc == root->getDiscriminator() && (root->isVacant() == false))
    {
        DNode *temp = new DNode(root->getAccount());
        root->_vacant = true;
        updateNumVacant(root);
        return temp;
    }

    if (disc > root->getDiscriminator())
    {
        DNode *temp = helpRemove(disc, root->_right);
        if (temp != nullptr)
        {
            updateNumVacant(root);
            return temp;
        }
        else
            return nullptr;
    }

    if (disc < root->getDiscriminator())
    {
        DNode *temp = helpRemove(disc, root->_left);
        if (temp != nullptr)
        {
            updateNumVacant(root);
            return temp;
        }
        else
            return nullptr;
    }

    return nullptr;
}

/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode *DTree::retrieve(int disc)
{
    return helpRetrieve(disc, _root);
}
/**
 * Helper funtion for retrieve.
 */
DNode *DTree::helpRetrieve(int disc, DNode *root)
{
    if (root == nullptr)
        return nullptr;
    if ((root->getDiscriminator() == disc) && (root->_vacant == true))
        return root;

    if (disc > root->getDiscriminator())
        return helpRetrieve(disc, root->_right);

    if (disc < root->getDiscriminator())
        return helpRetrieve(disc, root->_left);

    return nullptr;
}
/**
 * Helper for the destructor to clear dynamic memory.
 */
void DTree::clear()
{
    helpClean(_root);
    _root = nullptr;
}
/**
 * Helper funtion for clear.
 */
void DTree::helpClean(DNode *&root)
{
    if (root == nullptr)
        return;

    helpClean(root->_right);
    helpClean(root->_left);

    delete root;
}
/**
 * Prints all accounts' details within the DTree.
 */
void DTree::printAccounts() const
{
    helpPrintAccounts(_root);
}
/**
 * Helper funtion for Print Accounts.
 */
void DTree::helpPrintAccounts(DNode *root) const
{
    if (root == nullptr)
        return;

    helpPrintAccounts(root->_left);
    cout << root->_account << endl;
    helpPrintAccounts(root->_right);
}
/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode *node) const
{
    if (node == nullptr)
        return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const
{
    if (_root == nullptr)
        return 0;
    return _root->getSize() - _root->getNumVacant();
}

/**
 * Updates the size of a node based on the imedaite children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode *node)
{
    node->_size = 1;
    if (node == nullptr)
        return;
    if (node->_left != nullptr)
        node->_size += node->_left->getSize();
    if (node->_right != nullptr)
        node->_size += node->_right->getSize();
}

/**
 * Updates the number of vacant nodes in a node's subtree based on the immediate children
 * @param node DNode object in which the number of vacant nodes in the subtree will be updated
 */
void DTree::updateNumVacant(DNode *node)
{
    node->_numVacant = 0;
    if (node->isVacant())
        node->_numVacant = 1;

    if (node->_left != nullptr)
        node->_numVacant += node->_left->getNumVacant();
    if (node->_right != nullptr)
        node->_numVacant += node->_right->getNumVacant();
}

/**
 * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
 * @param checkImbalance DNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
bool DTree::checkImbalance(DNode *node)
{
    int left, right;
    if (node->_left == nullptr)
        left = 0;
    else
        left = node->_left->getSize();

    if (node->_right == nullptr)
        right = 0;
    else
        right = node->_right->getSize();

    if ((left < 4) && (right < 4))
        return false;
    else
    {
        int max, min;
        if (left > right)
        {
            max = left;
            min = right;
        }
        else
        {
            min = left;
            max = right;
        }
        if (max > (min * 1.5))
            return true;
        else
            return false;
    }

    return false;
}

//----------------
/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (pass by reference).
 * @param node DNode root of the subtree to balance
 */
void DTree::rebalance(DNode *&node)
{
    int index = 0;
    Account *rootArray = new Account[node->_size - node->_numVacant];
    helpArrayInOrder(node, rootArray, index);
    DNode *root = nullptr;
    helpRebalance(root, rootArray, 0, getNumUsers() - 1);
    helpClean(node);
    node = root;
    delete[] rootArray;
}
/**
 * Helper funtion rebalancing, sort DTree into an Array.
 */
void DTree::helpArrayInOrder(DNode *root, Account *&rootArray, int &index)
{
    if (root == nullptr)
        return;

    helpArrayInOrder(root->_left, rootArray, index);
    if (!root->isVacant())
        rootArray[index++] = root->getAccount();
    helpArrayInOrder(root->_right, rootArray, index);

    return;
}
/**
 * Helper funtion for rebalancing, Array back to DTree.
 */
void DTree::helpRebalance(DNode *&root, Account *rootArray, int min, int max)
{

    if (min > max)
    {
        return;
    }
    int mid = (max + min) / 2;
    root = new DNode(rootArray[mid]);

    helpRebalance(root->_left, rootArray, min, mid - 1);
    helpRebalance(root->_right, rootArray, mid + 1, max);
    updateSize(root);
}
// -- OR --

/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
//DNode* DTree::rebalance(DNode*& node) {

//}
//----------------

/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream &operator<<(ostream &sout, const Account &acct)
{
    sout << "Account name: " << acct.getUsername() << "\n\tDiscriminator: " << acct.getDiscriminator() << "\n\tNitro: " << acct.hasNitro() << "\n\tBadge: " << acct.getBadge() << "\n\tStatus: " << acct.getStatus();
    return sout;
}