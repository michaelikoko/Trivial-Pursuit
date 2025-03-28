#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

class QuestionAnswer
{
public:
    QuestionAnswer(string question, string answer, string category, int questionId);
    QuestionAnswer();
    bool isAnswerCorrect(string userInput);
    void displayQuestion();
    void displayQA();
    int questionId;
    string answer;
    string question;

protected:
    string category;
    string normalise(string str);
};

QuestionAnswer::QuestionAnswer()
{
    // Kinda like a null value for this class
    question = "";
    questionId = -1;
    category = "";
    answer = "";
}

QuestionAnswer::QuestionAnswer(string quest, string ans, string cat, int questId)
{
    question = quest;
    questionId = questId;
    category = cat;
    answer = ans;
}

void QuestionAnswer::displayQuestion()
{
    // Prints the question to the console
    cout << "Category: " << category << endl;
    cout << "Q. " << question << endl;
}

void QuestionAnswer::displayQA()
{
    displayQuestion();
    cout << "A. " << answer << endl;
}

bool QuestionAnswer::isAnswerCorrect(string userInput)
{
    // Checks if input is the answer
   string strippedUserInput = normalise(userInput);
   string strippedQuestionAnswer = normalise(answer);

   return strippedUserInput == strippedQuestionAnswer;
}

string QuestionAnswer::normalise(string str)
{
    // Removes spaces and transforms the string to lowercase
    transform(str.begin(), str.end() , str.begin(), ::tolower);
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
    return str;
}