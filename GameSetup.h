#pragma once

#include <iostream>
#include "QuestionList.h"
#include "QuestionAnswer.h"
#include "FileReader.h"
#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>
using namespace std;

class GameSetup
{
public:
    GameSetup(int maxLives, int maxQuestions);
    GameSetup();
    int rollDice();
    void start();
    string normalise(string str);

protected:
    int maxLives;                 // Maximum number of lives
    int maxQuestions;             // Maximum number of questions
    int currentLives;             // Current amount of lives
    int questionsAnsweredCorrect; // Number of questions answered correctly
    void playGameLoop();
    void adminLoop();
    void displayCategories();
};

GameSetup::GameSetup()
{
    // Default constructor - called if class is not instanciated with parameters
    maxLives = 3;
    maxQuestions = 10;
    currentLives = maxLives; // Starts off same as the maximum
    questionsAnsweredCorrect = 0;
}

GameSetup::GameSetup(int maxLivesValue, int maxQuestionsValue)
{
    maxLives = maxLivesValue;
    maxQuestions = maxQuestionsValue;
    currentLives = maxLivesValue;
    questionsAnsweredCorrect = 0;
}

int GameSetup::rollDice()
{
    /* Generate random number between 1 and 6 */
    srand((unsigned)time(NULL));
    const short MIN_VALUE = 1;
    const short MAX_VALUE = 6;
    // i.e int random = (rand() % range) + offset = (rand() % 6) + 1 // for 1 and 6
    int randomNumber = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
    return randomNumber;
}

string GameSetup::normalise(string str)
{
    // Removes trailing spaces and transforms the string to lowercase
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
    return str;
}

void GameSetup::start()
{

    while (true)
    {
        cout << "+---------------------------+" << endl;
        cout << "|     TRIVIAL PURSUIT       |" << endl; // change later
        cout << "+---------------------------+" << endl;

        // Main Menu
        string option;
        do
        {
            cout << "1. Play Game" << endl
                 << "2. Admin" << endl
                 << "3. Exit" << endl
                 << "Select option: ";
            cin >> option;
        } while ((normalise(option) != "1") && (normalise(option) != "2") && (normalise(option) != "3"));

        if (normalise(option) == "1")
        {
            playGameLoop();
        }
        else if (normalise(option) == "2")
        {
            adminLoop();
        }
        else
        {
            break;
        }
    }
}

void GameSetup::playGameLoop()
{

    FileReader geographyFile("./questions/geography.csv", "Geography"); // 1
    FileReader historyFile("./questions/history.csv", "History");       // 2
    FileReader movieFile("./questions/movie.csv", "Movie");             // 3
    FileReader musicFile("./questions/music.csv", "Music");             // 4
    FileReader scienceFile("./questions/science.csv", "Science");       // 5
    FileReader sportFile("./questions/sport.csv", "Sport");             // 6
    while (true)
    {
        string userInput;
        do
        {
            cout << "Enter 'roll' to get question: ";
            cin >> userInput;
        } while (normalise(userInput) != "roll");

        cout << "Rolling die..." << endl;
        int dieNumber = rollDice();
        cout << "Die Number: " << dieNumber << endl;

        QuestionAnswer randomQuestion;
        switch (dieNumber)
        {
        case 1:
            randomQuestion = geographyFile.questionsList.getRandomQuestionInstance();
            break;
        case 2:
            randomQuestion = historyFile.questionsList.getRandomQuestionInstance();
            break;
        case 3:
            randomQuestion = movieFile.questionsList.getRandomQuestionInstance();
            break;
        case 4:
            randomQuestion = musicFile.questionsList.getRandomQuestionInstance();
            break;
        case 5:
            randomQuestion = scienceFile.questionsList.getRandomQuestionInstance();
            break;
        case 6:
            randomQuestion = sportFile.questionsList.getRandomQuestionInstance();
            break;
        }
        cout << endl;
        randomQuestion.displayQuestion();

        string userAnswer;

        do
        {
            cout << "Enter Answer: ";
            cin.ignore();
            getline(cin, userAnswer);
        } while (normalise(userAnswer) == "");

        bool isAnswerCorrect = randomQuestion.isAnswerCorrect(userAnswer);

        if (isAnswerCorrect)
        {
            cout << "Correct!" << endl;
            ++questionsAnsweredCorrect;
        }
        else
        {
            cout << "Wrong! The answer is: " << randomQuestion.answer << endl;
            --currentLives;
        }

        switch (dieNumber)
        {
        case 1:
            geographyFile.questionsList.deleteNode(randomQuestion.questionId);
            break;
        case 2:
            historyFile.questionsList.deleteNode(randomQuestion.questionId);
            break;
        case 3:
            movieFile.questionsList.deleteNode(randomQuestion.questionId);
            break;
        case 4:
            musicFile.questionsList.deleteNode(randomQuestion.questionId);
            break;
        case 5:
            scienceFile.questionsList.deleteNode(randomQuestion.questionId);
            break;
        case 6:
            sportFile.questionsList.deleteNode(randomQuestion.questionId);
            break;
        }

        // Stats
        cout << endl;
        cout << "Questions Answered Correctly: " << questionsAnsweredCorrect << "/" << maxQuestions << endl;
        cout << "Lives: " << currentLives << "/" << maxLives << endl;
        cout << endl;

        if (currentLives == 0)
        {
            cout << "Game Lost!" << endl;
            break;
        }
        else if (questionsAnsweredCorrect == maxQuestions)
        {
            cout << "Game Won!" << endl;
            break;
        }
    }
}

void GameSetup::adminLoop()
{

    while (true)
    {
        FileReader geographyFile("./questions/geography.csv", "Geography"); // 1
        FileReader historyFile("./questions/history.csv", "History");       // 2
        FileReader movieFile("./questions/movie.csv", "Movie");             // 3
        FileReader musicFile("./questions/music.csv", "Music");             // 4
        FileReader scienceFile("./questions/science.csv", "Science");       // 5
        FileReader sportFile("./questions/sport.csv", "Sport");

        string option;
        do
        {
            cout << "Admin" << endl
                 << "1. Add Questions" << endl
                 << "2. Delete Question" << endl
                 << "3. Display Questions" << endl
                 << "4. Go back to main menu" << endl
                 << "Select option: ";
            cin >> option;
        } while ((normalise(option) != "1") && (normalise(option) != "2") && (normalise(option) != "3") && (normalise(option) != "4"));

        if (option == "1")
        {
            while (true)
            {
                displayCategories();
                string categoryOption;
                do
                {
                    cout << "Select category to add questions to: ";
                    cin >> categoryOption;
                } while (
                    (normalise(categoryOption) != "1") &&
                    (normalise(categoryOption) != "2") &&
                    (normalise(categoryOption) != "3") &&
                    (normalise(categoryOption) != "4") &&
                    (normalise(categoryOption) != "5") &&
                    (normalise(categoryOption) != "6"));

                string question;
                do
                {
                    cout << "Enter question: ";
                    cin.ignore();
                    getline(cin, question);
                } while (normalise(question) == "");

                string answer;
                do
                {
                    cout << "Enter answer to the question: ";
                    // cin.ignore();
                    getline(cin, answer);
                } while (normalise(answer) == "");

                switch (stoi(categoryOption))
                {
                case 1:
                    geographyFile.addQuestion(question, answer);
                    break;
                case 2:
                    historyFile.addQuestion(question, answer);
                    break;
                case 3:
                    movieFile.addQuestion(question, answer);
                    break;
                case 4:
                    musicFile.addQuestion(question, answer);
                    break;
                case 5:
                    scienceFile.addQuestion(question, answer);
                    break;
                case 6:
                    sportFile.addQuestion(question, answer);
                    break;
                }

                string repeat;
                do
                {
                    cout << "Do you want to add another question(y/n): ";
                    cin >> repeat;
                } while ((normalise(repeat) != "y") && (normalise(repeat) != "n"));

                if (repeat == "n")
                    break;
            }
        }
        else if (option == "2")
        {
            while (true)
            {
                displayCategories();
                string categoryOption;
                do
                {
                    cout << "Select category to remove questions from: ";
                    cin >> categoryOption;
                } while (
                    (normalise(categoryOption) != "1") &&
                    (normalise(categoryOption) != "2") &&
                    (normalise(categoryOption) != "3") &&
                    (normalise(categoryOption) != "4") &&
                    (normalise(categoryOption) != "5") &&
                    (normalise(categoryOption) != "6"));

                // Display all the questions for the selected category
                switch (stoi(categoryOption))
                {
                case 1:
                    geographyFile.questionsList.displayOnlyQuestions();
                    break;
                case 2:
                    historyFile.questionsList.displayOnlyQuestions();
                    break;
                case 3:
                    movieFile.questionsList.displayOnlyQuestions();
                    break;
                case 4:
                    musicFile.questionsList.displayOnlyQuestions();
                    break;
                case 5:
                    scienceFile.questionsList.displayOnlyQuestions();
                    break;
                case 6:
                    sportFile.questionsList.displayOnlyQuestions();
                    break;
                }

                int rowId;
                while (true)
                {
                    cout << "Enter row ID to delete: ";
                    cin >> rowId;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Disacard invalid input
                        cerr << "Please enter an integer." << endl;
                    }
                    else
                    {
                        break;
                        // valid input
                    }
                }

                switch (stoi(categoryOption))
                {
                case 1:
                    geographyFile.removeQuestion(rowId);
                    break;
                case 2:
                    historyFile.removeQuestion(rowId);
                    break;
                case 3:
                    movieFile.removeQuestion(rowId);
                    break;
                case 4:
                    musicFile.removeQuestion(rowId);
                    break;
                case 5:
                    scienceFile.removeQuestion(rowId);
                    break;
                case 6:
                    sportFile.removeQuestion(rowId);
                    break;
                }

                string repeat;
                do
                {
                    cout << "Do you want to delete another question(y/n): ";
                    cin >> repeat;
                } while ((normalise(repeat) != "y") && (normalise(repeat) != "n"));

                if (repeat == "n")
                    break;
            }
        }
        else if (option == "3")
        {
            displayCategories();
            string categoryOption;
            do
            {
                cout << "Select category to display questions: ";
                cin >> categoryOption;
            } while (
                (normalise(categoryOption) != "1") &&
                (normalise(categoryOption) != "2") &&
                (normalise(categoryOption) != "3") &&
                (normalise(categoryOption) != "4") &&
                (normalise(categoryOption) != "5") &&
                (normalise(categoryOption) != "6"));

            switch (stoi(categoryOption))
            {
            case 1:
                geographyFile.questionsList.displayAllQuestionsInList();
                break;
            case 2:
                historyFile.questionsList.displayAllQuestionsInList();
                break;
            case 3:
                movieFile.questionsList.displayAllQuestionsInList();
                break;
            case 4:
                musicFile.questionsList.displayAllQuestionsInList();
                break;
            case 5:
                scienceFile.questionsList.displayAllQuestionsInList();
                break;
            case 6:
                sportFile.questionsList.displayAllQuestionsInList();
                break;
            }
        }
        else
        {
            break; // Break out of the admin loop
        }
    }
}

void GameSetup::displayCategories()
{
    cout << "1. Geography" << endl
         << "2. History" << endl
         << "3. Movies" << endl
         << "4. Music" << endl
         << "5. Science" << endl
         << "6. Sports" << endl;
};
