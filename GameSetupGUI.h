#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/fl_message.H>
#include "GameSetup.h"
#include "FileReader.h"
#include "QuestionAnswer.h"
#include "QuestionList.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

class GameSetupGUI
{
private:
    static constexpr int WINDOW_WIDTH = 600;
    static constexpr int WINDOW_HEIGHT = 400;

    Fl_Window *mainWindow, *rollDiceWindow, *questionWindow, *nameWindow, *highScoreWindow, *gameOverWindow, *adminMenuWindow, *addSingleQuestionWindow, *removeQuestionWindow;
    Fl_Button *playButton, *adminButton, *exitButton, *backToMainMenuButton, *rollDiceButton, *submitAnswerButton, *submitNameButton, *clearHighScoreButton, *highScoreButton, *goToAddSingleQuestionButton, *goToRemoveQuestionButton, *addSingleQuestionButton, *removeQuestionButton;
    Fl_Box *gameStatus, *questionBox, *statsBox, *prevAnswerStatusBox, *menuBox, *nameBox, *gameOverMessageBox, *highScoreBox;
    Fl_Input *answerInput, *nameInput, *addQuestion, *addAnswer, *addFileName, *removeRowNumber, *removeFileName;
    Fl_Hold_Browser *highScoreBrowser;

    FileReader *geographyFile, *historyFile, *movieFile, *musicFile, *scienceFile, *sportFile;
    GameSetup gameSetup;
    QuestionAnswer currentQuestion;

    int currentLives;
    int maxLives;
    int maxQuestions;
    int questionsAnsweredCorrect;

    std::string statsString;        // Persistent string to store stats
    std::string answerStatusString; // Persistent string to store answer status
    std::string playerName;
    std::string gameOverMessage;

    static void startGameCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->mainWindow->hide();

        gui->geographyFile = new FileReader("./questions/geography.csv", "Geography"); // 1
        gui->historyFile = new FileReader("./questions/history.csv", "History");       // 2
        gui->movieFile = new FileReader("./questions/movie.csv", "Movie");             // 3
        gui->musicFile = new FileReader("./questions/music.csv", "Music");             // 4
        gui->scienceFile = new FileReader("./questions/science.csv", "Science");       // 5
        gui->sportFile = new FileReader("./questions/sport.csv", "Sport");

        gui->promptForName();
    }

    static void highScoreCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->mainWindow->hide();
        gui->showHighScoreWindow();
    }

    static void adminMenuCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->mainWindow->hide();
        gui->showAdminMenuWindow();
    }

    static void rollDiceCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->rollDice();
    }

    static void submitAnswerCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->checkAnswer();
    }

    static void clearHighScoresCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->clearScores();
    }

    static void saveNameCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        gui->saveName();
    }

    static void addSingleQuestionCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        std::cout << "Adding question to csv" << std::endl;
        gui->addSingleQuestion();
    }

    static void removeQuestionButtonCB(Fl_Widget *w, void *data)
    {
        GameSetupGUI *gui = (GameSetupGUI *)data;
        std::cout << "Removing question from csv" << std::endl;
        gui->removeQuestion();
    }

    static void goToAddSingleQuestionCB(Fl_Widget *w, void *data)
    {
        std::cout << "Add single question window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->adminMenuWindow->hide();
        gui->showAddSingleQuestionWindow();
    }

    static void goToRemoveQuestionCB(Fl_Widget *w, void *data)
    {
        std::cout << "remove question window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->adminMenuWindow->hide();
        gui->showRemoveQuestionWindow();
    }

    static void backToMainMenuFromHighScoreCB(Fl_Widget *w, void *data)
    {
        std::cout << "go back to main menu from high score window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->highScoreWindow->hide();
        gui->mainWindow->show();
    }

    static void backToMainMenuFromNameCB(Fl_Widget *w, void *data)
    {
        std::cout << "go back to main menu from name window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->nameWindow->hide();
        gui->mainWindow->show();
    }

    static void backToMainMenuFromGameOverCB(Fl_Widget *w, void *data)
    {
        std::cout << "go back to main menu from game over window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->resetScoreAndLives();
        gui->gameOverWindow->hide();
        gui->mainWindow->show();
    }

    static void backToMainMenuFromAdminMenuCB(Fl_Widget *w, void *data)
    {
        std::cout << "go back to main menu from admin menu window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->adminMenuWindow->hide();
        gui->mainWindow->show();
    }

    static void backToMainMenuFromAddSingleQuestionCB(Fl_Widget *w, void *data)
    {
        std::cout << "go back to main menu from add single question window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->addSingleQuestionWindow->hide();
        gui->mainWindow->show();
    }

    static void backToMainMenuFromAddRemoveQuestionCB(Fl_Widget *w, void *data)
    {
        std::cout << "go back to main menu from remove question window" << std::endl;
        GameSetupGUI *gui = (GameSetupGUI *)data;

        gui->removeQuestionWindow->hide();
        gui->mainWindow->show();
    }

    void addSingleQuestion()
    {

        std::string question = addQuestion->value();
        std::string answer = addAnswer->value();
        std::string fileName = addFileName->value();

        if (question.empty() || answer.empty() || fileName.empty())
        {
            std::cout << "Please enter a question, answer and file name." << std::endl;
            fl_message("Error: Please fill in all fields.");
            return;
        }

        // Check if the file exists before opening in append mode
        std::ifstream fileCheck("./questions/" + gameSetup.normalise(fileName) + ".csv");
        if (!fileCheck)
        {
            fl_message("Error: File does not exist");
            return;
        }

        // Get ID of last row
        std::string line;
        std::string lastLine;
        int lastRowId = 0;

        std::getline(fileCheck, line); // First line contains the column name
        while (std::getline(fileCheck, line))
        {
            lastLine = line;
        }

        if (!lastLine.empty())
        {
            std::stringstream ss(lastLine);
            std::string idStr;
            std::getline(ss, idStr, ','); // Get first value (rowID)
            lastRowId = std::stoi(idStr);
        }
        int newRowId = lastRowId + 1;
        fileCheck.close(); // Close the file check stream

        std::ofstream fileStream("./questions/" + gameSetup.normalise(fileName) + ".csv", std::ios::app);
        if (!fileStream)
        {
            fl_message("Error: Unable to open file");
            std::cout << "Error: Unable to open file " << fileName << std::endl;
            return;
        }

        fileStream << newRowId << "," << question << "," << answer << "\n";
        fileStream.close();

        fl_message("Successfully added question ");
        std::cout << "Successfully added question to " << fileName << std::endl;

        // Clear input fields after adding
        addQuestion->value("");
        addAnswer->value("");
        addFileName->value("");
    }

    void removeQuestion()
    {

        std::string fileNameValue = removeFileName->value();
        std::string rowNumberStr = removeRowNumber->value();

        if (fileNameValue.empty() || rowNumberStr.empty())
        {
            fl_message("Error: Please fill in all fields.");
            return;
        }
        std::string fileName = "./questions/" + gameSetup.normalise(fileNameValue) + ".csv";

        // Convert row number to integer
        int rowNumber;
        try
        {
            rowNumber = std::stoi(rowNumberStr);
            if (rowNumber <= 0)
            {
                throw std::invalid_argument("Invalid row number");
            }
        }
        catch (std::exception &e)
        {
            fl_message("Error: Invalid row number.");
            return;
        }

        // Check if file exists
        std::ifstream inFile(fileName);
        if (!inFile)
        {
            fl_message("Error: File does not exist");
            return;
        }

        std::string tempFileName = "./questions/" + gameSetup.normalise(fileNameValue) + ".tmp";
        std::ofstream outFile(tempFileName);
        if (!outFile)
        {
            fl_message(("Error: Unable to create temp file"));
            return;
        }

        std::string line;
        int currentRow = 0;
        bool found = false;

        std::getline(inFile, line); // skip the column name
        outFile << line << "\n";    // Write the column name to the temp file
        while (std::getline(inFile, line))
        {
            currentRow++;
            if (currentRow == rowNumber)
            {
                found = true;
                continue; // Skip the line to remove
            }
            outFile << line << "\n";
        }

        inFile.close();
        outFile.close();

        if (!found)
        {
            fl_message("Error: Row number out of range.");
            std::remove(tempFileName.c_str()); // Delete temp file
            return;
        }

        // Replace original file with updated file
        if (std::remove(fileName.c_str()) != 0 || std::rename(tempFileName.c_str(), fileName.c_str()) != 0)
        {
            fl_message("Error: Unable to update the file.");
            return;
        }

        fl_message("Successfully removed row.");

        // Clear input fields
        removeFileName->value("");
        removeRowNumber->value("");
    }

    void resetScoreAndLives()
    {
        currentLives = maxLives;
        questionsAnsweredCorrect = 0;
    }

    void loadScores()
    {
        std::cout << "loading scores" << std::endl;
        highScoreBrowser->clear();
        std::ifstream fileStream("scores.csv"); // Load the csv file
        // Add if incase the file does not exist
        std::vector<std::pair<std::string, int>> scores;
        std::string line, name;
        int score;

        while (getline(fileStream, line))
        {
            std::stringstream ss(line);
            getline(ss, name, ',');
            ss >> score;
            scores.emplace_back(name, score);
        }
        fileStream.close();

        std::sort(scores.begin(), scores.end(), [](auto &a, auto &b)
                  { return b.second < a.second; }); // Sort the scores
        for (const auto &entry : scores)
        {
            highScoreBrowser->add((entry.first + " - " + std::to_string(entry.second)).c_str());
        }
        highScoreBrowser->redraw();
    }

    void showHighScoreWindow()
    {
        highScoreWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        highScoreBox = new Fl_Box(250, 10, 100, 40, "HIGH SCORES");
        highScoreBrowser = new Fl_Hold_Browser(150, 60, 300, 200);
        clearHighScoreButton = new Fl_Button(200, 280, 200, 40, "Clear Scores");
        backToMainMenuButton = new Fl_Button(200, 330, 200, 40, "Go To Menu");

        loadScores();

        clearHighScoreButton->callback(clearHighScoresCB, this);
        backToMainMenuButton->callback(backToMainMenuFromHighScoreCB, this);

        highScoreWindow->end();
        highScoreWindow->show();
    }

    void saveName()
    {
        // Save the name
        playerName = nameInput->value();
        if (gameSetup.normalise(playerName) == "")
        {
            // Empty so do nothing
            std::cout << "Please enter a name." << std::endl;
            return;
        }
        std::cout << "Player Name: " << playerName << std::endl;

        nameWindow->hide();   // Hide the window that prompts for name
        showRollDiceWindow(); // Shows the window for rolling dice
    }

    void promptForName()
    {
        // Prompt for player name
        nameWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        nameBox = new Fl_Box(250, 50, 100, 40, "Enter your name:");
        nameInput = new Fl_Input(200, 100, 200, 40);
        submitNameButton = new Fl_Button(200, 150, 200, 40, "Submit");
        backToMainMenuButton = new Fl_Button(200, 200, 200, 40, "Go To Menu");

        submitNameButton->callback(saveNameCB, this);
        backToMainMenuButton->callback(backToMainMenuFromNameCB, this);

        nameWindow->end();
        nameWindow->show();
    }

    void showRollDiceWindow(bool showAnswerCorrect = false, bool isAnswerCorrect = false)
    {
        // Show the roll dice window
        rollDiceWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        new Fl_Box(FL_NO_BOX, 200, 50, 200, 40, "Roll the dice to get a question");
        rollDiceButton = new Fl_Button(200, 100, 200, 40, "Roll Dice");

        if (showAnswerCorrect)
        {
            // Show if the answer was correct or not
            prevAnswerStatusBox = new Fl_Box(250, 150, 100, 40, "Loading Answer Status...");
            if (isAnswerCorrect)
            {
                updateAnswerStatus(true);
            }
            else
            {
                updateAnswerStatus(false, currentQuestion.answer);
                std::cout << "Wrong! The answer was: " << currentQuestion.answer << std::endl;
            }
        }
        statsBox = new Fl_Box(250, 200, 100, 40, "Loading Stats..."); // Create box
        updateStats();                                                // Update the stats display

        rollDiceButton->callback(rollDiceCB, this);
        rollDiceWindow->end();
        rollDiceWindow->show();
    }

    void showAdminMenuWindow()
    {
        adminMenuWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        Fl_Box *adminMenuBox = new Fl_Box(250, 10, 100, 40, "ADMIN MENU");
        goToAddSingleQuestionButton = new Fl_Button(200, 60, 200, 40, "Add Question");
        goToRemoveQuestionButton = new Fl_Button(200, 110, 200, 40, "Remove Question");
        backToMainMenuButton = new Fl_Button(200, 160, 200, 40, "Go To Menu");

        goToAddSingleQuestionButton->callback(goToAddSingleQuestionCB, this);
        goToRemoveQuestionButton->callback(goToRemoveQuestionCB, this);
        backToMainMenuButton->callback(backToMainMenuFromAdminMenuCB, this);

        adminMenuWindow->end();
        adminMenuWindow->show();
    }

    void showAddSingleQuestionWindow()
    {
        addSingleQuestionWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        Fl_Box *addSingleQuestionBox = new Fl_Box(200, 10, 200, 40, "ADD QUESTION");
        addQuestion = new Fl_Input(200, 60, 200, 40, "Question: ");
        addAnswer = new Fl_Input(200, 110, 200, 40, "Answer: ");
        addFileName = new Fl_Input(200, 160, 200, 40, "File Name: ");
        addSingleQuestionButton = new Fl_Button(200, 210, 200, 40, "Add Question");
        backToMainMenuButton = new Fl_Button(200, 260, 200, 40, "Go To Menu");

        addSingleQuestionButton->callback(addSingleQuestionCB, this);
        backToMainMenuButton->callback(backToMainMenuFromAddSingleQuestionCB, this);

        addSingleQuestionWindow->end();
        addSingleQuestionWindow->show();
    }

    void showRemoveQuestionWindow()
    {
        removeQuestionWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        Fl_Box *addRemoveQuestionBox = new Fl_Box(200, 10, 200, 40, "REMOVE QUESTION");
        removeRowNumber = new Fl_Input(200, 60, 200, 40, "Row ID: ");
        removeFileName = new Fl_Input(200, 110, 200, 40, "File Name: ");
        removeQuestionButton = new Fl_Button(200, 160, 200, 40, "Remove Question");
        backToMainMenuButton = new Fl_Button(200, 210, 200, 40, "Go To Menu");

        removeQuestionButton->callback(removeQuestionButtonCB, this);
        backToMainMenuButton->callback(backToMainMenuFromAddRemoveQuestionCB, this);

        removeQuestionWindow->end();
        removeQuestionWindow->show();
    }

    void rollDice()
    {
        // Roll the dice then show the question window
        int dieNumber = gameSetup.rollDice();
        std::cout << "Rolling Dice returned " << dieNumber << std::endl;
        rollDiceWindow->hide();
        showQuestionWindow(dieNumber);
    }

    void showQuestionWindow(int dieNum)
    {
        // Show the question window
        std::cout << "show questionwindow" << std::endl;
        questionWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        questionBox = new Fl_Box(150, 50, 300, 40, "Question:");
        answerInput = new Fl_Input(200, 100, 200, 40, "Your Answer:");
        submitAnswerButton = new Fl_Button(200, 150, 200, 40, "Submit");
        statsBox = new Fl_Box(250, 200, 100, 40, "Loading Stats..."); // Create box
        updateStats();                                                // Update the stats display

        // Get category file based on die number
        switch (dieNum)
        {
        case 1:
            currentQuestion = geographyFile->questionsList.getRandomQuestionInstance();
            break;
        case 2:
            currentQuestion = historyFile->questionsList.getRandomQuestionInstance();
            break;
        case 3:
            currentQuestion = movieFile->questionsList.getRandomQuestionInstance();
            break;
        case 4:
            currentQuestion = musicFile->questionsList.getRandomQuestionInstance();
            break;
        case 5:
            currentQuestion = scienceFile->questionsList.getRandomQuestionInstance();
            break;
        case 6:
            currentQuestion = sportFile->questionsList.getRandomQuestionInstance();
            break;
        }

        questionBox->label(currentQuestion.question.c_str());
        submitAnswerButton->callback(submitAnswerCB, this);

        // Remove the question from the questionList linked list
        switch (dieNum)
        {
        case 1:
            geographyFile->questionsList.deleteNode(currentQuestion.questionId);
            break;
        case 2:
            historyFile->questionsList.deleteNode(currentQuestion.questionId);
            break;
        case 3:
            movieFile->questionsList.deleteNode(currentQuestion.questionId);
            break;
        case 4:
            musicFile->questionsList.deleteNode(currentQuestion.questionId);
            break;
        case 5:
            scienceFile->questionsList.deleteNode(currentQuestion.questionId);
            break;
        case 6:
            sportFile->questionsList.deleteNode(currentQuestion.questionId);
            break;
        }
        questionWindow->end();
        questionWindow->show();
    }

    void checkAnswer()
    {
        std::string userAnswer = answerInput->value();
        std::cout << "User Answer: " << userAnswer << std::endl;
        std::cout << "Correct Answer: " << currentQuestion.answer << std::endl;

        if (gameSetup.normalise(userAnswer) == "")
        {
            // Empty so do nothing
            std::cout << "Please enter an answer." << std::endl;
            return;
        }

        bool isCorrect = currentQuestion.isAnswerCorrect(userAnswer);

        if (isCorrect)
            questionsAnsweredCorrect++;
        else
            currentLives--;

        answerInput->value("");
        questionWindow->hide();

        if (questionsAnsweredCorrect == 10)
        {
            std::cout << "Game Won!" << std::endl;
            gameOver(true);
        }
        else if (currentLives == 0)
        {
            std::cout << "Game Lost!" << std::endl;
            gameOver(false);
        }
        else
        {
            showRollDiceWindow(true, isCorrect);
        }
    }

    void saveScore(int score)
    {
        std::string fileName = "scores.csv";

        std::ofstream outFile(fileName, std::ios::app); // Append mode
        if (!outFile)
        {
            std::cerr << "Error: Unable to open " << fileName << " for writing.\n";
            return;
        }

        outFile << playerName << "," << score << "\n";
        outFile.close();

        std::cout << "Score saved successfully: " << playerName << " - " << score << std::endl;
    }

    void clearScores()
    {
        std::cout << "Clear high scores" << std::endl;
        std::ofstream fileStream("scores.csv", std::ios::trunc); // Truncate file
        fileStream.close();
        loadScores();
    }

    void updateStats()
    {
        std::ostringstream stats;
        stats << "Correct: " << questionsAnsweredCorrect << " / " << maxQuestions << " | Lives: " << currentLives << " / " << maxLives;
        statsString = stats.str();            // Store in class variable
        statsBox->label(statsString.c_str()); // Set label with persistent string
        statsBox->redraw();                   // Refresh the UI
    }

    void updateAnswerStatus(bool isCorrect, std::string correctAnswer = "")
    {
        std::ostringstream answerStatus;
        if (isCorrect)
        {
            answerStatus << "Correct!";
        }
        else
        {
            answerStatus << "Wrong! The answer is: " << correctAnswer << ".";
        }
        answerStatusString = answerStatus.str();                // Store in class variable
        prevAnswerStatusBox->label(answerStatusString.c_str()); // Set label with persistent string
        prevAnswerStatusBox->redraw();                          // Refresh the UI
    }

    void gameOver(bool gameWon)
    {
        int score = (questionsAnsweredCorrect * 10) + (currentLives * 5); // Calculate score
        saveScore(score);                                     // Save the score
        gameOverWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        gameOverMessageBox = new Fl_Box(250, 50, 100, 40, "Loading message");
        updateGameOverMessage(gameWon, score);
        Fl_Button *closeButton = new Fl_Button(200, 100, 200, 40, "Close");
        statsBox = new Fl_Box(250, 150, 100, 40, "Loading Stats..."); // Create box
        backToMainMenuButton = new Fl_Button(200, 200, 200, 40, "Go To Menu");

        updateStats(); // Update the stats display

        backToMainMenuButton->callback(backToMainMenuFromGameOverCB, this);
        closeButton->callback([](Fl_Widget *w, void *data)
                              { exit(0); });
        gameOverWindow->end();
        gameOverWindow->show();
    }

    void updateGameOverMessage(bool isGameWon, int score)
    {
        std::ostringstream message;
        std::cout << playerName << std::endl;
        if (isGameWon)
        {
            message << "Congratulations " << playerName << "! You won! " << "Score: " << score;
        }
        else
        {
            message << "Game Over, " << playerName << "! Better luck next time! " << "Score: " << score;
        }
        gameOverMessage = message.str(); // Store in class variable
        gameOverMessageBox->label(gameOverMessage.c_str());
        gameOverMessageBox->redraw();
    }

public:
    GameSetupGUI()
    {
        // Default Constructor
        maxLives = 3; // Default
        currentLives = maxLives;
        questionsAnsweredCorrect = 0;
        maxQuestions = 10; // Defaul
        mainWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        menuBox = new Fl_Box(250, 10, 100, 30, "MENU");
        playButton = new Fl_Button(200, 50, 200, 40, "Play Game");
        adminButton = new Fl_Button(200, 100, 200, 40, "Admin");
        highScoreButton = new Fl_Button(200, 150, 200, 40, "High Scores");
        exitButton = new Fl_Button(200, 200, 200, 40, "Exit");

        playButton->callback(startGameCB, this);
        exitButton->callback([](Fl_Widget *w, void *)
                             { exit(0); });
        highScoreButton->callback(highScoreCB, this);
        adminButton->callback(adminMenuCB, this);

        mainWindow->end();
        mainWindow->show();
    }
    GameSetupGUI(int maxLivesValue, int maxQuestionsValue)
    {
        maxLives = maxLivesValue;
        currentLives = maxLives;
        questionsAnsweredCorrect = 0;
        maxQuestions = maxQuestionsValue;
        mainWindow = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TRIVIAL PURSUIT");
        menuBox = new Fl_Box(250, 10, 100, 30, "MENU");
        playButton = new Fl_Button(200, 50, 200, 40, "Play Game");
        adminButton = new Fl_Button(200, 100, 200, 40, "Admin");
        highScoreButton = new Fl_Button(200, 150, 200, 40, "High Scores");
        exitButton = new Fl_Button(200, 200, 200, 40, "Exit");

        playButton->callback(startGameCB, this);
        exitButton->callback([](Fl_Widget *w, void *)
                             { exit(0); });
        highScoreButton->callback(highScoreCB, this);
        adminButton->callback(adminMenuCB, this);

        mainWindow->end();
        mainWindow->show();
    }

    ~GameSetupGUI()
    {
        delete geographyFile;
        delete historyFile;
        delete movieFile;
        delete musicFile;
        delete scienceFile;
        delete sportFile;
    }
};
