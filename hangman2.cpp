#include <iostream>
#include <string>
#include <vector>
#include <graphics.h>
#include <cstdlib>
#include <ctime> // For random number generation

using namespace std;

// Function to draw the hangman based on the number of incorrect guesses
void drawHangman(int incorrectGuesses)
{
    int gd = DETECT, gm;
    char path[] = ""; // Use a char array for initgraph
    initgraph(&gd, &gm, path);

    setcolor(WHITE);

    // Draw the gallows
    line(50, 400, 50, 100); // Vertical pole
    line(50, 100, 150, 100); // Horizontal pole
    line(150, 100, 150, 150); // Rope

    // Draw hangman based on incorrect guesses
    if (incorrectGuesses > 0) line(150, 150, 150, 200); // Head
    if (incorrectGuesses > 1) circle(150, 175, 25); // Body
    if (incorrectGuesses > 2) line(150, 200, 120, 250); // Left arm
    if (incorrectGuesses > 3) line(150, 200, 180, 250); // Right arm
    if (incorrectGuesses > 4) line(150, 250, 120, 300); // Left leg
    if (incorrectGuesses > 5) line(150, 250, 180, 300); // Right leg

    delay(2000); // Display the hangman for 2 seconds
    closegraph();
}

// Function to display the word with guessed letters
void displayWord(const string &word, const string &guessedLetters)
{
    int gd = DETECT, gm;
    char path[] = ""; // Use a char array for initgraph
    initgraph(&gd, &gm, path);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    int x = 50;
    for (char c : word)
    {
        if (c == ' ')
        {
            outtextxy(x, 200, " ");
            x += 30;
        }
        else if (guessedLetters.find(c) != string::npos)
        {
            char text[2] = {c, '\0'};
            outtextxy(x, 200, text);
        }
        else
        {
            char text[2] = {'_', '\0'};
            outtextxy(x, 200, text);
        }
        x += 30;
    }

    delay(2000); // Display the word for 2 seconds
    closegraph();
}

int main()
{
    // List of possible words
    vector<string> words = {"HELLO", "WORLD", "PROGRAMMING", "HANGMAN", "GAME"};

    // Seed random number generator and select a random word
    srand(time(0));
    int randomIndex = rand() % words.size();
    string word = words[randomIndex];

    // Print the randomly selected word (for debugging purposes)
    cout << "Random word selected: " << word << endl;//random string
    

    string guessedLetters;
    int maxIncorrectGuesses = 6;
    int incorrectGuesses = 0;

    // Initialize graphics mode
    int gd = DETECT, gm;
    char path[] = ""; // Use a char array for initgraph
    initgraph(&gd, &gm, path);

    while (incorrectGuesses < maxIncorrectGuesses)
    {
       // system("cls"); //this is for he holding the screen for the first time to see the random string
        displayWord(word, guessedLetters);

        char guess;
        cout << "Guess a letter: ";
        cin >> guess;
        guess = toupper(guess);

        if (word.find(guess) != string::npos)
        {
            guessedLetters += guess;
        }
        else
        {
            incorrectGuesses++;
            drawHangman(incorrectGuesses);
        }

        // Check if the player has won
        bool won = true;
        for (char c : word)
        {
            if (c != ' ' && guessedLetters.find(c) == string::npos)
            {
                won = false;
                break;
            }
        }

        if (won)
        {
            system("cls");
            displayWord(word, guessedLetters);
            cout << "Congratulations! You guessed the word!" << endl;
            break;
        }
    }

    if (incorrectGuesses == maxIncorrectGuesses)
    {
        system("cls");
        drawHangman(maxIncorrectGuesses);
        cout << "Game Over! The word was: " << word << endl;
    }

    closegraph();
    return 0;
}
