/*
 * File:          swarm.cpp
 * Project:       CMSC 341 Project 2 Fall 2021
 * Author:        Johnson Xie 
 *
 * Description:   This file contains the swarm.cpp
 *
 *
 */

#include "swarm.h"

//constructor
Swarm::Swarm()
{
    m_root = nullptr;
}

//Destructor
Swarm::~Swarm()
{
    clear();
}

//inserts robot object into tree in proper position 
void Swarm::insert(const Robot& robot)
{
    m_root = insertNode(robot, m_root);
 }

//deallocates all memory in tree and make try empty 
void Swarm::clear()
{
    destroyTree(m_root);
    m_root = nullptr;
}

//removes node based on id from tree
void Swarm::remove(int id)
{
    if(findBot(id) == true)
    {
        return;
    }

    else
        cout << "that ID does not exist in tree" << endl;
}

//updates height of node passed in 
void Swarm::updateHeight(Robot* aBot)
{

    if(aBot == nullptr)
        return;

    int leftHeight = 0;
    int rightHeight = 0;
    int newHeight = 0;
    int largerHeight = 0;

    //sets height to -1 if there is no child on the left
    if(aBot->getLeft() == nullptr)
        leftHeight = -1;
    else
        // sets left height to be the height of left child
        leftHeight = aBot->getLeft()->getHeight();

    //sets right height to -1 if there is no child on the right
    if(aBot->getRight() == nullptr)
        rightHeight = -1;
    else
        // sets right height to be the height of the right child
        rightHeight = aBot->getRight()->getHeight();

    // finds which side has the largest height
    if(leftHeight > rightHeight)
        largerHeight = leftHeight;
    else
        largerHeight = rightHeight;

    // height of curr node will be 1 + the larger height
    newHeight = 1 + largerHeight;

    // sets new height
    aBot->setHeight(newHeight);
}

//checks if there is an imbalance at node passed in 
int Swarm::checkImbalance(Robot* aBot)
{

    int balance = -1;

    //sets balance to zero since node DNE
    if(aBot == nullptr)
        balance = 0;

    if(aBot != nullptr)
    {
        int left = 0;
        int right = 0;

        //if left node DNE
        if (aBot->getLeft() == nullptr)
            left = -1;
        else
            //sets the left value to be the height of left node
            left = aBot->getLeft()->getHeight();

        //if right node DNE
        if (aBot->getRight() == nullptr)
            right = -1;
        else
            // sets the right value to be the height of the right node
            right = aBot->getRight()->getHeight();

        // gets the balance of current node
        balance = (right - left);
    }

    return balance;
}

//main function that manages imbalances in tree.
Robot* Swarm::rebalance(Robot* aBot)
{
    int balance = checkImbalance(aBot);
    int leftBalance = checkImbalance(aBot->getLeft());
    int rightBalance = checkImbalance(aBot->getRight());

    //left left case
    if (balance == -2 && leftBalance == -1)
    {
        return leftLeftCase(aBot);
    }

    //left right case
    if(balance == -2 && leftBalance == 1)
    {
        return leftRightCase(aBot);
    }

    //right right case
    if(balance == 2 && rightBalance == 1)
    {
        return rightRightCase(aBot);
    }

    //right left case
    if(balance == 2 && rightBalance == -1)
    {
        return rightLeftCase(aBot);
    }

    updateHeight(aBot);

    return aBot;
}

//prints a list of all robots in tree in ascending orders of ID
void Swarm::listRobots() const
{
    //prints robots by inorder
    inOrder(m_root);
}

//finds node with id in tree and sets state
bool Swarm::setState(int id, STATE state)
{
    //sets the state of the robot to parameter if found
    if(findBot(id) == true)
    {
        Robot* theBot = search(m_root, id);
        theBot->m_state = state;
        return true;
    }
    else
        //returns false if robot does not exist
        return false;
}

void Swarm::removeDead()
{
    return;
}

//returns true if node with id is found in tree
bool Swarm::findBot(int id) const 
{
    Robot* theBot = search(m_root, id);

    if(theBot == nullptr)
        return false;
    else
        return true;
}

void Swarm::dumpTree() const 
{
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const
{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

Robot *Swarm::insertNode(const Robot &robot, Robot *currBot) 
{
    if(currBot == nullptr)
    {
        Robot* theBot = new Robot(robot);
        return theBot;
    }

    if(currBot->getID() > robot.getID())
    {
        currBot->m_left = insertNode(robot, currBot->getLeft());
    }

    if(currBot->getID() < robot.getID())
    {
        currBot->m_right = insertNode(robot, currBot->getRight());
    }

    // updates height for rest of recursive calls
    updateHeight(currBot);

    //checks for balance for rest of recursive calls;

    return currBot;

}

void Swarm::destroyTree(Robot *aBot)
{
    if(aBot == nullptr)
        return;
    else
    {
        //traverse through tree and deletes nodes
        destroyTree(aBot->getLeft());
        destroyTree(aBot->getRight());

        delete aBot;
        aBot = nullptr;
    }
}

void Swarm::inOrder(Robot *aBot) const 
{
    if(aBot == nullptr)
        return;

    inOrder(aBot->getLeft());

    cout << aBot->getID() << ":" << aBot->getStateStr() << ":" << aBot->getTypeStr() << endl;

    inOrder(aBot->getRight());
}

Robot *Swarm::search(Robot *aBot, int ID) const 
{
    if(aBot == nullptr || aBot->getID() == ID)
        return aBot;

    //keeps traversing until ID is found
    if(aBot->getID() > ID)
        return search(aBot->getLeft(), ID);
    else
        return search(aBot->getRight(), ID);
}

Robot *Swarm::leftRotation(Robot *aBot) 
{
    Robot *xPost = aBot;
    Robot *yPost = aBot->getRight();
    Robot *t2 = yPost->getLeft();

    xPost->setRight(t2);
    yPost->setLeft(xPost);

    return yPost;
}

Robot *Swarm::rightRotation(Robot *aBot) 
{
    Robot* zPost = aBot;
    Robot* yPost = aBot->getLeft();
    Robot* t3 = yPost->getRight();

    zPost->setLeft(t3);
    yPost->setRight(zPost);

    return yPost;
}

Robot *Swarm::leftLeftCase(Robot *aBot) 
{
    Robot* final = rightRotation(aBot);
    return final;
}

Robot *Swarm::leftRightCase(Robot *aBot) 
{
    aBot->setLeft(leftRotation(aBot->getLeft()));
    Robot* final = rightRotation(aBot);

    return final;
}

Robot *Swarm::rightRightCase(Robot *aBot) 
{
    Robot* final = leftRotation(aBot);
    return final;
}

Robot *Swarm::rightLeftCase(Robot *aBot) 
{
    aBot->setRight(rightRotation(aBot->getRight()));
    Robot* final = leftRotation(aBot);

    return final;
}
