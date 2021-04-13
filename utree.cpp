/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree()
{
    clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */
void UTree::loadData(string infile, bool append)
{
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if (!instream.is_open())
    {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if (!append)
        this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while (std::getline(instream, line))
    {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for (unsigned int c = 0; c < buffer.str().length(); c++)
            if (buffer.str()[c] == delim)
                delimCount++;
        if (delimCount != numFields - 1)
        {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for (int i = 0; i < numFields; i++)
        {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct)
{
    if (retrieveUser(newAcct.getUsername(), newAcct.getDiscriminator()) != nullptr)
        return false;

    helpInsert(newAcct, _root);
    return true;
}
/**
 * Helper funtion for insert.
 */
void UTree::helpInsert(Account newAcct, UNode *&root)
{
    if (root == nullptr)
    {
        root = new UNode();
        root->_dtree->insert(newAcct);
        updateHeight(root);
        return;
    }
    if (root->getUsername() == newAcct.getUsername())
    {
        root->_dtree->insert(newAcct);
        if (checkImbalance(root))
            rebalance(root);
        return;
    }

    if (root->getUsername() > newAcct.getUsername())
    {
        helpInsert(newAcct, root->_left);
        updateHeight(root);
        if (checkImbalance(root))
            rebalance(root);
        return;
    }
    if (root->getUsername() < newAcct.getUsername())
    {
        helpInsert(newAcct, root->_right);
        updateHeight(root);
        if (checkImbalance(root))
            rebalance(root);
        return;
    }

    return;
}
/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode *&removed)
{
    helpRemoveUser(username, disc, removed, _root);
    if (removed == nullptr)
        return false;

    return true;
}
/**
 * Helper funtion for remove User.
 */
void UTree::helpRemoveUser(string username, int disc, DNode *&removed, UNode *&root)
{
    if (root == nullptr)
        return;

    if (root->getUsername() == username)
    {
        root->_dtree->remove(disc, removed);
        if (root->_dtree->getNumUsers() == 0)
        {
            UNode* temp = helpDeleteNodeAVL(root);
            root = temp;
            deepHeightUpdate(root);
        }
        return;
    }

    if (root->getUsername() > username)
    {
        helpRemoveUser(username, disc, removed, root->_left);
        updateHeight(root);
        return;
    }
    if (root->getUsername() < username)
    {
        helpRemoveUser(username, disc, removed, root->_right);
        updateHeight(root);
        return;
    }

    return;
}
void UTree::deepHeightUpdate(UNode *&root)
{
    if (root == nullptr)
        return;

    deepHeightUpdate(root->_left);
    deepHeightUpdate(root->_right);

    updateHeight(root);
}
/**
 * Helper funtion to delete a node in an AVL tree.
 */
UNode *UTree::helpDeleteNodeAVL(UNode *root)
{

    if (root->_left != nullptr)
    {
        UNode *tempR = root->_right;
        UNode *tempL = root->_left;

        root = highestNode(root->_left);
        root->_right = tempR;
        
        if (root != tempL)
            root->_left = tempL;

        return root;
    }

    UNode *tempR = root->_right;

    delete root;
    root = tempR;

    return root;
}
/**
 * Helper funtion to find highest node
 */
UNode *UTree::highestNode(UNode *&root)
{
    UNode *node = root;
    if (node->_right == nullptr)
        return node;

    while (node->_right->_right != NULL)
        node = node->_right;

    if (node->_right->_left != nullptr)
    {
        UNode *temp = node->_right;
        node->_right = node->_right->_left;
        return temp;
    }

    UNode *temp = node->_right;
    node->_right = nullptr;

    return temp;
}
/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode *UTree::retrieve(string username)
{
    return helpRetrieve(username, _root);
}
/**
 * Helper funtion for retrieve.
 */
UNode *UTree::helpRetrieve(string username, UNode *root)
{
    if (root == nullptr)
        return nullptr;
    if (username == root->getUsername())
        return root;

    if (username < root->getUsername())
        return helpRetrieve(username, root->_left);
    if (username > root->getUsername())
        return helpRetrieve(username, root->_right);

    return nullptr;
}
/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode *UTree::retrieveUser(string username, int disc)
{
    return helpRetrieveUser(username, disc, _root);
}
/**
 * Helper funtion for retrieve user.
 */
DNode *UTree::helpRetrieveUser(string username, int disc, UNode *root)
{
    if (root == nullptr)
        return nullptr;
    if (username == root->getUsername())
        return root->_dtree->retrieve(disc);

    if (username < root->getUsername())
        return helpRetrieveUser(username, disc, root->_left);
    if (username > root->getUsername())
        return helpRetrieveUser(username, disc, root->_right);

    return nullptr;
}
/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username)
{
    return helpNumUsers(username, _root);
}
/**
 * Helper funtion for num Users.
 */
int UTree::helpNumUsers(string username, UNode *root)
{
    if (root == nullptr)
        return 0;
    if (username == root->getUsername())
        return root->_dtree->getNumUsers();

    if (username < root->getUsername())
        return helpNumUsers(username, root->_left);
    if (username > root->getUsername())
        return helpNumUsers(username, root->_right);

    return 0;
}
/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear()
{
    helpClean(_root);
}
/**
 * Helper funtion for clear.
 */
void UTree::helpClean(UNode *&root)
{
    if (root == nullptr)
        return;

    helpClean(root->_right);
    helpClean(root->_left);

    delete root;
}
/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const
{
    helpPrintUsers(_root);
}
/**
 * Helper funtion for print Users.
 */
void UTree::helpPrintUsers(UNode *root) const
{
    if (root == nullptr)
        return;

    helpPrintUsers(root->_left);
    cout << root->getUsername() << ": ";
    root->_dtree->printAccounts();
    cout << endl;
    helpPrintUsers(root->_right);
}
/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode *node) const
{
    if (node == nullptr)
        return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode *node)
{
    if (node == nullptr)
        return;

    if (node->_left == nullptr)
        if (node->_right == nullptr)
            node->_height = 0;
        else
            node->_height = node->_right->_height + 1;
    else if (node->_right == nullptr)
        node->_height = node->_left->_height + 1;
    else
    {
        int max;
        if (node->_left->_height > node->_right->_height)
            max = node->_left->_height;
        else
            max = node->_right->_height;

        node->_height = max + 1;
    }
}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode *node)
{
    if (checkHeight(node->_left) > checkHeight(node->_right))
    {
        if (checkHeight(node->_left) - checkHeight(node->_right) > 1)
            return 1;
    }

    if (checkHeight(node->_right) > checkHeight(node->_left))
    {
        if (checkHeight(node->_right) - checkHeight(node->_left) > 1)
            return 1;
    }

    return 0;
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
void UTree::rebalance(UNode *&node)
{
    if (checkHeight(node->_left) > checkHeight(node->_right))
    {
        if (checkHeight(node->_left) - checkHeight(node->_right) < 2)
            return;

        if (checkHeight(node->_left->_left) >= checkHeight(node->_left->_right)) //right rotation
            node = rigthRotation(node);
        else
        {
            //left(for node->_left) right rotation.
            node->_left = leftRotation(node->_left);
            node = rigthRotation(node);
        }
    }

    if (checkHeight(node->_right) > checkHeight(node->_left))
    {
        if (checkHeight(node->_right) - checkHeight(node->_left) < 2)
            return;

        if (checkHeight(node->_right->_left) > checkHeight(node->_right->_right))
        {
            //right(for node->_rigth) left rotation
            node->_right = rigthRotation(node->_right);
            node = leftRotation(node);
        }

        else //left rotation.
            node = leftRotation(node);
    }

    return;
}
int UTree::checkHeight(UNode *root)
{
    if (root == nullptr)
        return -1;

    if ((root->_left == nullptr) && (root->_right == nullptr))
        return 0;

    return root->getHeight();
}
UNode *UTree::leftRotation(UNode *node)
{
    UNode *tempY = node->_right;
    UNode *tempT2 = tempY->_left;

    tempY->_left = node;
    node->_right = tempT2;

    updateHeight(node);
    updateHeight(tempY);

    return tempY;
}
UNode *UTree::rigthRotation(UNode *node)
{
    UNode *tempY = node->_left;
    UNode *tempT3 = tempY->_right;

    tempY->_right = node;
    node->_left = tempT3;

    updateHeight(node);
    updateHeight(tempY);

    return tempY;
}
// -- OR --
/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UTree* UTree::rebalance(UNode* node) {

//}
//----------------
