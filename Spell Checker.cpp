#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>
using namespace std;
//******************************* FONT ********************************************
void SetColor(int textColor, int backgroundColor) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = textColor + (backgroundColor * 16);
    SetConsoleTextAttribute(consoleHandle, colorAttribute);
}
//***********************************************************************************

struct Node {
    string DATA;
    Node* RIGHT;
    Node* LEFT;

    Node(string WORD) {
        DATA = WORD;
        RIGHT = NULL;
        LEFT = NULL;
    }
};

//********************************** ADD WORD TO BST ********************************
Node* ADD_WORD_TO_BST(Node* root, string word) {
    if (root == NULL) {
        return new Node(word);
    }
    if (word < root->DATA) {
        root->LEFT = ADD_WORD_TO_BST(root->LEFT, word);
    } else if (word > root->DATA) {
        root->RIGHT = ADD_WORD_TO_BST(root->RIGHT, word);
    }
    return root;
}

//********************************** SEARCH *****************************************
bool SEARCH(Node* root, string WORD) {
    if (root == NULL) {
        return false;
    }
    if (root->DATA == WORD) {
        return true;
    } else if (WORD < root->DATA) {
        return SEARCH(root->LEFT, WORD);
    } else {
        return SEARCH(root->RIGHT, WORD);
    }
}

//********************************** SIMILAR WORDS **********************************
void TO_FIND_SIMILAR_WORDS(Node* root, string WORD) {
    static bool TO_PRINT_LINE = false;

    if (root == NULL) {
        return;
    }
    string Word;
    int hit = 0;
    int WORD_FROM_FILE = root->DATA.size();
    int WORD_BBY_USER = WORD.size();

    if ((WORD_FROM_FILE - WORD_BBY_USER) <= 2) {
        for (int i = 0; i < WORD_FROM_FILE && i < WORD_BBY_USER; ++i) {
            if (root->DATA[i] == WORD[i]) {
                ++hit;
            }
        }
        int hitrate = (hit * 100) / WORD_BBY_USER;
        if (hitrate >= 50) {
            if (!TO_PRINT_LINE) {
                SetColor(0, 7);
                cout << "DID YOU MEAN:" << endl;
                SetColor(7, 0);
                TO_PRINT_LINE = true;
            }
            Word = root->DATA;
            cout << "\u2192 " << Word << endl;
        }
    }
    TO_FIND_SIMILAR_WORDS(root->LEFT, WORD);
    TO_FIND_SIMILAR_WORDS(root->RIGHT, WORD);
}

//********************************** ADD WORD TO FILE ******************************
void ADD_WORD_TO_FILE(const string& word, const string& filename) {
    ofstream outfile;
    outfile.open(filename.c_str(), ios::app);
    if (outfile.is_open()) {
        outfile << word << endl;
        outfile.close();
        cout << word << " HAS BEEN ADDED IN DICTIONARY." << endl;
    } else {
        cerr << "FILE NOT OPEN" << endl;
    }
}

//***********************************************************************

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetColor(7, 5);
    cout << "\t\t\t\t   \u2605  SPELL CHECKER  \u2605     \n";
    SetColor(7, 0);
    cout << "          ";
    SetColor(7, 5);
    cout << "\t\t\t PROGRAMMED BY Areej Saqib ";
    cout << "\n\t\t\t\t                           ";
    SetColor(7, 0);

    ifstream file_("DATA.txt");
    if (!file_) {
        cout << "FILE NOT FOUND" << endl;
        return 1;
    }

    Node* root = NULL;
    string WORD_FROM_FILE;
    while (file_ >> WORD_FROM_FILE) {
        root = ADD_WORD_TO_BST(root, WORD_FROM_FILE);
    }
    file_.close();

    while (true) {
        cout << "\n>>>ENTER : ";
        string WORD_FROM_USER;
        cin >> WORD_FROM_USER;
        if (WORD_FROM_USER == "exit") {
            break;
        }

        if (SEARCH(root, WORD_FROM_USER)) {
            SetColor(2, 0);
            cout << "\u2714  WORD IS CORRECT" << endl;
            SetColor(7, 0);
        } else {
            SetColor(4, 0);
            cout << "\u2716  INCORRECT WORD\n" << endl;
            SetColor(7, 0);
            TO_FIND_SIMILAR_WORDS(root, WORD_FROM_USER);

            char choice;
            SetColor(7, 8);
            cout << "WOULD YOU LIKE TO ADD '" << WORD_FROM_USER << "' TO THE DICTIONARY? (y/n):";
            SetColor(7, 0);
            cout << " ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                root = ADD_WORD_TO_BST(root, WORD_FROM_USER);
                ADD_WORD_TO_FILE(WORD_FROM_USER, "DATA.txt");
            }
        }
    }
    return 0;
}

