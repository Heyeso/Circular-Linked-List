#include "utree.h"
#include <random>

#define NUMACCTS 30
#define RANDDISC (distAcct(rng))

std::mt19937 rng(10);
std::uniform_int_distribution<> distAcct(0, 9999);

class Tester
{
public:
    bool testBasicDTreeInsert(DTree &dtree);
    bool testAssigmentOperatorDTree(DTree &dtree);
    bool testBalanceDTree(DTree &dtree);
    bool testDTreeBST(DTree &dtree);
    bool testBasicDTreeRemove(DTree &dtree);
    bool testEdgeCase(DTree &dtree);

    bool testBasicUTreeInsert(UTree &utree);
    bool testUTreeInsert(UTree &utree);
    bool testUTreeBST(UTree &dtree);
    bool testBasicUTreeRemove(UTree &utree);
    bool testUTreeEdgeCase(UTree &utree);

private:
    bool compareDNode(DNode *&copy, DNode *&dtree);
    bool helpTestDTreeBST(DNode *root);

    bool testBalanceUNode(UNode *node);
    int checkImbalance(UNode *node);
    int checkHeight(UNode *root);
    bool helpTestUTreeBST(UNode *root);
};

bool Tester::testBasicDTreeInsert(DTree &dtree)
{
    bool allInserted = true;
    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if (!dtree.insert(newAcct))
        {
            cout << "Insertion on node " << disc << " did not return true" << endl;
            allInserted = false;
        }
    }
    return allInserted;
}

bool Tester::testBasicUTreeInsert(UTree &utree)
{
    string dataFile = "/accounts.csv";
    try
    {
        utree.loadData(dataFile);
    }
    catch (std::invalid_argument e)
    {
        std::cerr << e.what() << endl;
        return false;
    }
    return true;
}

int main()
{
    Tester tester;

    {
        /* Basic dtree tests */
        DTree dtree;

        cout << "Testing DTree insertion...\t";
        if (tester.testBasicDTreeInsert(dtree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }

        cout << "Resulting DTree:" << endl;
        dtree.dump();
        cout << endl;
    }
    {
        /*dtree Assignment Operator tests */
        DTree dtree;

        cout << "\nTesting DTree Assignment Operator...\t";
        if (tester.testAssigmentOperatorDTree(dtree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }
    {
        /*dtree Imbalance tests */
        DTree dtree;

        cout << "\nTesting DTree Imbalance and Rebalance...\t";
        if (tester.testBalanceDTree(dtree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }
    {
        /*dtree BST property tests */
        DTree dtree;

        cout << "\nTesting DTree BST property...\t";
        if (tester.testDTreeBST(dtree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }
    {
        /* Basic dtree tests: removal Test */
        DTree dtree;

        cout << "\nTesting DTree removal...\t";
        if (tester.testBasicDTreeRemove(dtree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }
    {
        /* Basic dtree tests: Edge case Test */
        DTree dtree;

        cout << "\nTesting DTree Edge Cases: Insertion and Removal...\t";
        if (tester.testEdgeCase(dtree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }

    {
        /* Basic UTree tests */
        UTree utree;

        cout << "\n\nTesting UTree insertion and Balance...\t";
        if (tester.testUTreeInsert(utree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }

        cout << "Resulting UTree:" << endl;
        utree.dump();
        cout << endl;
    }
    {
        /*utree BST property tests */
        UTree utree;

        cout << "\nTesting UTree BST property...\t";
        if (tester.testUTreeBST(utree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }
    {
        /* Basic utree tests: removal Test */
        UTree utree;

        cout << "\nTesting UTree removal...\t";
        if (tester.testBasicUTreeRemove(utree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }
    {
        /* Basic utree tests: Edge case Test */
        UTree utree;

        cout << "\nTesting UTree Edge Cases: Insertion and Removal...\t";
        if (tester.testUTreeEdgeCase(utree))
        {
            cout << "test passed" << endl;
        }
        else
        {
            cout << "test failed" << endl;
        }
    }

    return 0;
}

bool Tester::testAssigmentOperatorDTree(DTree &dtree)
{
    DTree copy = dtree;
    if (!((copy._root == nullptr) && (dtree._root == nullptr)))
        return false;

    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if (!dtree.insert(newAcct))
            return false;
    }

    if (copy.getNumUsers() == dtree.getNumUsers())
        return false;

    copy = dtree;

    if (!compareDNode(copy._root, dtree._root))
        return false;

    return true;
}
/**
 * Compare Two DTree Nodes
 * @return true if they duplicate but different objects, false if they are the same object.
 */
bool Tester::compareDNode(DNode *&copy, DNode *&dtree)
{
    if ((copy == nullptr) && (dtree == nullptr))
        return true;

    if (((copy == nullptr) && !(dtree == nullptr)) || (!(copy == nullptr) && (dtree == nullptr)))
        return false;

    if (!((copy->getDiscriminator() == dtree->getDiscriminator()) && (copy != dtree)))
        return false;

    return compareDNode(copy->_left, dtree->_left) && compareDNode(copy->_right, dtree->_right);
}

bool Tester::testBalanceDTree(DTree &dtree)
{
    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if (!dtree.insert(newAcct))
            return false;
    }

    return !dtree.checkImbalance(dtree._root);
}
bool Tester::testDTreeBST(DTree &dtree)
{
    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if (!dtree.insert(newAcct))
            return false;
    }

    return helpTestDTreeBST(dtree._root);
}
/**
 * BST property Test Helper
 */
bool Tester::helpTestDTreeBST(DNode *root)
{
    if (root == nullptr)
        return true;

    if (!(root->_left == nullptr))
        if (!(root->getDiscriminator() > root->_left->getDiscriminator()))
            return false;

    if (!(root->_right == nullptr))
        if (!(root->getDiscriminator() < root->_right->getDiscriminator()))
            return false;

    return helpTestDTreeBST(root->_left) && helpTestDTreeBST(root->_right);
}
bool Tester::testBasicDTreeRemove(DTree &dtree)
{

    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = 2 * i;
        Account newAcct = Account("", disc, 0, "", "");
        if (!dtree.insert(newAcct))
            return false;
    }

    for (int i = 0; i < NUMACCTS / 2; i++)
    {
        int disc = 2 * i;
        DNode *temp;
        dtree.remove(disc, temp);
        delete temp;
    }

    if (dtree._root->getNumVacant() == 0)
        return false;

    for (int i = NUMACCTS; i < NUMACCTS + 10; i++)
    {
        int disc = 2 * i;
        Account newAcct = Account("", disc, 0, "", "");
        if (!dtree.insert(newAcct))
            return false;
    }

    if (!(dtree._root->getNumVacant() == 0))
        return false;

    return true;
}
bool Tester::testEdgeCase(DTree &dtree)
{
    int disc = 2;
    Account newAcct = Account("", disc, 0, "", "");
    if (!dtree.insert(newAcct))
        return false;

    DNode *temp;
    if (!dtree.remove(disc, temp))
        return false;
    delete temp;

    DNode *temp2;
    if (dtree.remove(disc, temp2))
        return false;
    delete temp2;

    return true;
}
bool Tester::testUTreeInsert(UTree &utree)
{
    int x = 0;
    string username[] = {"felix", "john", "sam", "tom", "noah", "salam", "seleh", "heems"};
    bool allInserted = true;
    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        if (x > 7)
            x = 0;
        Account newAcct = Account(username[x], disc, 0, "", "");
        if (!utree.insert(newAcct))
        {
            cout << "Insertion on node " << disc << " did not return true" << endl;
            allInserted = false;
        }
        x++;
    }

    return allInserted && testBalanceUNode(utree._root);
}
bool Tester::testBalanceUNode(UNode *node)
{

    if (node == nullptr)
        return true;
    if (checkImbalance(node))
        return false;

    return testBalanceUNode(node->_left) && testBalanceUNode(node->_right);
}
int Tester::checkImbalance(UNode *node)
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
int Tester::checkHeight(UNode *root)
{
    if (root == nullptr)
        return -1;

    if ((root->_left == nullptr) && (root->_right == nullptr))
        return 0;

    return root->getHeight();
}
bool Tester::testUTreeBST(UTree &utree)
{
    int x = 0;
    string username[] = {"felix", "john", "sam", "tom", "noah", "salam", "seleh", "heems"};
    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        if (x > 7)
            x = 0;
        Account newAcct = Account(username[x], disc, 0, "", "");
        if (!utree.insert(newAcct))
            return false;
        x++;
    }

    return helpTestUTreeBST(utree._root);
}
/**
 * BST property Test Helper
 */
bool Tester::helpTestUTreeBST(UNode *root)
{
    if (root == nullptr)
        return true;

    if (!(root->_left == nullptr))
        if (!(root->getUsername() > root->_left->getUsername()))
            return false;

    if (!(root->_right == nullptr))
        if (!(root->getUsername() < root->_right->getUsername()))
            return false;

    return helpTestUTreeBST(root->_left) && helpTestUTreeBST(root->_right);
}
bool Tester::testBasicUTreeRemove(UTree &utree)
{

    utree.insert(Account("lex", 40, 0, "", ""));
    int x = 0;
    string username[] = {"felix", "john", "sam", "tom", "noah", "salam", "seleh", "heems"};
    for (int i = 0; i < NUMACCTS; i++)
    {
        int disc = RANDDISC;
        if (x > 7)
            x = 0;
        Account newAcct = Account(username[x], disc, 0, "", "");
        if (!utree.insert(newAcct))
            return false;
        x++;
    }

    DNode *del;
    if (!utree.removeUser("lex", 40, del))
        return false;

    if (del == nullptr)
        return false;

    if (utree.retrieve("lex") != nullptr)
        return false;

    return true;
}
bool Tester::testUTreeEdgeCase(UTree &utree)
{
    string username = "lex";
    int disc = 40;
    Account newAcct = Account(username, disc, 0, "", "");
    if (!utree.insert(newAcct))
        return false;

    DNode *del;
    if (!utree.removeUser("lex", 40, del))
        return false;

    if (del == nullptr)
        return false;

    if (utree.retrieve("lex") != nullptr)
        return false;

    return true;
}