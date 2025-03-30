#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include "QuestionList.h"
#include "QuestionAnswer.h"

using namespace std;

class FileReader
{
public:
  FileReader(string fileName, string category);
  FileReader();
  QuestionList questionsList;
  string category;
  void addQuestion(string question, string answer);
  void addQuestionFromList(QuestionList listOfQuestions);
  void removeQuestion(int rowId);

protected:
  string file;
  ifstream fileStream;
  ofstream fileStreamWrite;
  void createQuestionList();
  int getLastRowId();
};

FileReader::FileReader(string fileName, string categoryName)
{
  category = categoryName;
  file = fileName;
  createQuestionList();
}
FileReader::FileReader()
{
  // Default constructor
  category = "";
  file = "";
  //createQuestionList();
}

void FileReader::createQuestionList()
{
  fileStream.open(file); // change to read

  if (!fileStream.is_open())
  {
    cerr << "Error opening file: " << file << "\n";
    exit(0);
  }
  string line;
  getline(fileStream, line); // First line contains the column name

  while (getline(fileStream, line))
  {
    stringstream ss(line); // Create string stream of each line - so that we can use getline(stream, dest, delimeter)
    string idStr;
    string question;
    string answer;

    if (getline(ss, idStr, ',') && getline(ss, question, ',') && getline(ss, answer))
    {
      try
      {
        int id = stoi(idStr);
        // cout << "ID: " << id << endl
        //  << "Question: " << question << endl
        //  << "Answer: " << answer << endl;
        QuestionAnswer questionInstance(question, answer, category, id);
        questionsList.append(questionInstance);
      }
      catch (const std::exception &e)
      {
        cerr << "Invalid row in file: " << file << endl
             << "Row: " << line << endl;
        exit(0);
      }
    }
  }
  fileStream.close();
}

void FileReader::addQuestion(string question, string answer)
{
  fileStreamWrite.open(file, ios::app); // For writing to csv

  if (!fileStreamWrite.is_open())
  {
    cerr << "Error opening file: " << file << "\n";
    exit(0);
  }

  int idOfLastRow = getLastRowId();
  cout << "last row id: " << idOfLastRow << endl;
  int newRowId = idOfLastRow + 1;
  if (question.empty() || answer.empty())
  {
    cerr << "Question or Answer cannot be empty" << endl;
  }
  else
  {
    fileStreamWrite << newRowId << "," << question << "," << answer << "\n";
    cout << "Question added successfully" << endl;
    createQuestionList(); // Refresh the questionList linked list
  }
  fileStreamWrite.close();
}

void FileReader::addQuestionFromList(QuestionList listOfQuestions)
{

  if (listOfQuestions.length() == 0)
  {
    cerr << "List cannot be empty" << endl;
    exit(0);
  }

  Node *currentNode = listOfQuestions.head;
  while (currentNode != nullptr)
  {
    cout << "current node " << currentNode->data.question << endl;
    addQuestion(currentNode->data.question, currentNode->data.answer);
    currentNode = currentNode->next;
  }
  cout << "Questions added successfully" << endl;
}

void FileReader::removeQuestion(int rowId)
{
  fileStream.open(file);
  if (!fileStream.is_open())
  {
    cerr << "Error opening file: " << file << "\n";
    exit(0);
  }
  vector<string> lines; // Save csv lines without the line of specified rowId
  string line;
  bool found = false;

  getline(fileStream, line); // First line contains the column name
  lines.push_back(line); //First row
  while (getline(fileStream, line))
  {
    stringstream ss(line);
    string idStr;
    getline(ss, idStr, ','); // Get the row id

    int currentId = stoi(idStr);
    if (currentId == rowId)
    {
      found = true; // Mark found as true, but don't save the line
      continue;     // Goes back to the start of the loop(skips the current iteration)
    }
    lines.push_back(line);
  }
  fileStream.close();

  if (!found)
  {
    // rowId is not found
    cerr << "Error: No question found with row ID: " << rowId << endl;
    return;
  }

  // Write updated lines back to file
  fileStreamWrite.open(file);

  if (!fileStreamWrite.is_open())
  {
    cerr << "Error opening file: " << file << "\n";
    return;
  }

  for (const auto &row : lines)
  {
    fileStreamWrite << row << "\n";
  }

  fileStreamWrite.close();

  cout << "Question with row ID: " << rowId << " removed successfully." << endl;
}

int FileReader::getLastRowId()
{
  fileStream.open(file); // change to read

  if (!fileStream.is_open())
  {
    cerr << "Error opening file: " << file << "\n";
    exit(0);
  }

  string line;
  string lastLine;

  getline(fileStream, line); // First line contains the column name
  while (getline(fileStream, line))
  {
    lastLine = line;
  }

  fileStream.close();

  if (lastLine.empty())
  {
    // The CSV is empty
    return 0;
  }

  stringstream ss(lastLine);
  string idStr;
  getline(ss, idStr, ','); // Get first value (rowID)
  return stoi(idStr);
}