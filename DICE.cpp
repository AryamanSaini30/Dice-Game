#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

// Node for each player
class Node {
public:
    string name;
    int score;
    Node* next;
};

// Game history class
class GameHistory {
private:
    vector<string> players;
    vector<string> eliminations;
    string winner;
public:
    void addPlayer(const string& name) {
        players.push_back(name);
    }

    void logElimination(const string& name, int score) {
        eliminations.push_back(name + " eliminated with score " + to_string(score));
    }

    void setWinner(const string& name, int score) {
        winner = name + " won the game with score " + to_string(score);
    }

    void displayHistory() const {
        cout << "\n========== GAME HISTORY ==========\n";
        cout << "Players: ";
        for (const auto& name : players) {
            cout << name << " ";
        }
        cout << "\n\nEliminations:\n";
        for (const auto& log : eliminations) {
            cout << "- " << log << endl;
        }
        cout << "\nWinner: " << winner << endl;
        cout << "==================================\n";
    }
};

// Roll the die
int RollTheDie() {
    int result = (rand() % 6) + 1;
    cout << "Your dice has been rolled! You got: " << result << endl;
    return result;
}

// Insert player at end of circular linked list
void Insert(Node*& head, Node*& tail, const string& new_name, GameHistory& history) {
    Node* new_node = new Node();
    new_node->name = new_name;
    new_node->score = 0;
    new_node->next = nullptr;

    history.addPlayer(new_name);  // Track player

    if (head == nullptr) {
        head = new_node;
        new_node->next = head;
        tail = new_node;
    } else {
        tail->next = new_node;
        new_node->next = head;
        tail = new_node;
    }
}

// Update score
int UpdateScore(Node* player, int score) {
    player->score += score;
    return player->score;
}

// Delete current player and update head if needed
void Delete(Node*& head, Node*& current, GameHistory& history) {
    if (head == nullptr) return;

    history.logElimination(current->name, current->score);

    cout << endl << current->name << " ELIMINATED !!  Score - " << current->score << "\n\n";

    if (current->next == current) {
        delete current;
        head = nullptr;
        current = nullptr;
        return;
    }

    Node* prev = current;
    while (prev->next != current)
        prev = prev->next;

    if (current == head)
        head = head->next;

    prev->next = current->next;
    Node* del = current;
    current = current->next;
    delete del;
}

int main() {
    Node* head = nullptr;
    Node* tail = nullptr;
    GameHistory history;

    string name;
    int n, choice, result;

    cout << "Enter the number of players: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "Player " << (i + 1) << " - Enter your Name: ";
        cin >> name;
        Insert(head, tail, name, history);
    }

    srand((unsigned) time(0)); // Initialize random seed
    Node* current = head;

    cout << "\nEnter 1 to Roll the Dice, 0 to Exit\n";

    while (true) {
        if (current == nullptr) break;

        cout << "\n" << current->name << "'s turn";
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            result = RollTheDie();
            if (UpdateScore(current, result) >= 20) {
                Delete(head, current, history); // Update current inside
            } else {
                current = current->next;
            }
        } else if (choice == 0) {
            cout << "\nGame exited by user.\n";
            break;
        }

        if (current != nullptr && current->next == current) {
            cout << "\n" << current->name << " is the WINNER !!  Score - " << current->score << endl;
            history.setWinner(current->name, current->score);
            delete current;
            break;
        }

        if (current == nullptr) {
            cout << "\nNo players left. Game over!";
            break;
        }
    }

    history.displayHistory();  // Show final game history
    return 0;
}
