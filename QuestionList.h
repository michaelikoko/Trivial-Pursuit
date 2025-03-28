#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include "QuestionAnswer.h"
using namespace std;

class Node
{
public:
    QuestionAnswer data; // Node data - a QuestionAnswer object
    Node *next;          // Pointer to the next node
    Node(QuestionAnswer questAnsObj);
};

Node::Node(QuestionAnswer questAnsObj) : data(questAnsObj), next(nullptr) {} // member initializer list as QuestionAnswer does not have a default constructor
// Or set a default constructor for QuestionAnswer in the header file

class QuestionList
{
    // LinkedList
public:
    QuestionList();
    ~QuestionList();
    Node *head; // Pointer to the first node
    void append(QuestionAnswer questAnsObj);
    void addFirst(QuestionAnswer questAnsObj);
    void deleteNode(int questionId);
    void displayAllQuestionsInList(); // Display the questions together with the answers
    void displayOnlyQuestions();      // Display only the questions
    QuestionAnswer getRandomQuestionInstance();
    int length();
};

QuestionList::QuestionList()
{
    /* Constructor */
    head = nullptr; // Initialize empty list
}

QuestionList::~QuestionList()
{
    /* Destructor */
    Node *currentNode = head; // Start with head node
    // Loop until the end of the list
    while (currentNode != nullptr)
    {
        Node *nextNode = currentNode->next; // Store the next node
        delete currentNode;                 // Delete the current node
        currentNode = nextNode;             // Move to the next node
    }
}

void QuestionList::append(QuestionAnswer questAnsObj)
{
    /* Add a node at the end of the list */
    Node *node = new Node(questAnsObj);
    if (head == nullptr)
    {
        // The list is empty
        head = node;
    }
    else
    {
        Node *lastNode = head;
        while (lastNode->next != nullptr)
        {
            // Loop throught the list until the end
            lastNode = lastNode->next;
        }
        lastNode->next = node;
    }
}

void QuestionList::addFirst(QuestionAnswer questAnsObj)
{
    /* Add a node at the start of the list - Reassigning the head */
    Node *node = new Node(questAnsObj);
    if (head != nullptr)
    {
        node->next = head; // Link the new head to the current head
    }
    head = node;
}

void QuestionList::deleteNode(int questionId)
{
    /* Removing a node from the list - Remove a questionAnswer object based on questionId*/
    if (head == nullptr)
        return; // The list is empty

    // Check is the head node is the one to be removed
    if (head->data.questionId == questionId)
    {
        Node *tempNode = head;
        head = head->next; // Move the head to the next node
        delete tempNode;
        return;
    }

    Node *currentNode = head;
    // Loop throught the list to find the node to delete
    while (currentNode->next != nullptr)
    {
        if (currentNode->next->data.questionId == questionId)
        {
            Node *nodeToBeDeleted = currentNode->next;
            currentNode->next = currentNode->next->next; // Bypass the node to be deleted and point to the one after it
            delete nodeToBeDeleted;                      // Free memory of the bypassed node
            return;
        }
        currentNode = currentNode->next; // Move to the next node
    }
}

void QuestionList::displayAllQuestionsInList()
{
    /* For debugging purposes */
    if (head == nullptr)
    {
        cout << "No questions." << endl;
    }
    Node *currentNode = head;
    while (currentNode != nullptr)
    {
        cout << "ID: " << currentNode->data.questionId << " " << "Q: " << currentNode->data.question << " " << "A: " << currentNode->data.answer << endl;
        currentNode = currentNode->next;
    }
}
void QuestionList::displayOnlyQuestions()
{
    /* Display only the questions in the list */
    if (head == nullptr)
    {
        cout << "No questions." << endl;
    }
    Node *currentNode = head;
    while (currentNode != nullptr)
    {
        cout << "ID: " << currentNode->data.questionId << " " << "Q: " << currentNode->data.question << endl;
        currentNode = currentNode->next;
    }
}

QuestionAnswer QuestionList::getRandomQuestionInstance()
{
    // Select a random question from the list and return the instance
    // of QuestionAnswer object

    int listLength = length();
    if (listLength == 0)
        return QuestionAnswer(); // The list is empty, throw error here instead, or change to pointer and return nullptr

    // Generate a random number between 1 and listLength(inclusive)
    srand(time(NULL));
    const short MIN_VALUE = 1;
    const short MAX_VALUE = listLength;
    int randomNumber = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
    // cout << randomNumber << " random number" << endl;

    Node *currentNode = head;
    for (int i = 1; i < randomNumber; ++i)
    {
        currentNode = currentNode->next;
    }
    return currentNode->data;
}

int QuestionList::length()
{
    /* Returns the length of the linked list */
    int listLength = 0;
    if (head == nullptr)
        return listLength; // The list is empty

    Node *currentNode = head;
    while (currentNode != nullptr)
    {
        ++listLength;
        currentNode = currentNode->next;
    }
    return listLength;
}
