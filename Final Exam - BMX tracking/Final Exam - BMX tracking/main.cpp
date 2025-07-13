#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_TRICKS = 100;
const string FILENAME = "BMXTricks.txt";

// Struct: Plain data holder
struct TrickData {
    string name;
    string status;
    int attempts = 0;
    string notes;
    string videoLink;
    string photoLink;
};

// Convert string to lowercase (without <algorithm>)
string toLower(const string& str) {
    string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = tolower(result[i]);
    }
    return result;
}

// Validate allowed statuses
bool isValidStatus(const string& status) {
    string lower = toLower(status);
    return lower == "completed" || lower == "trying" || lower == "unattempted";
}

// Normalize user input to lowercase version
string normalizeStatus(const string& status) {
    string lower = toLower(status);
    if (lower == "completed") return "completed";
    if (lower == "trying") return "trying";
    if (lower == "unattempted") return "unattempted";
    return "unattempted"; // fallback
}

// Trick class encapsulating logic
class Trick {
private:
    TrickData data;

public:
    Trick() {
        data.name = "";
        data.status = "unattempted";
        data.attempts = 0;
        data.notes = "";
        data.videoLink = "";
        data.photoLink = "";
    }

    void inputNew() {
        cout << "Enter trick name: ";
        getline(cin, data.name);

        string userStatus;
        do {
            cout << "Status (completed/trying/unattempted): ";
            getline(cin, userStatus);
            if (!isValidStatus(userStatus)) {
                cout << "[ERROR] Invalid status. Please enter exactly: completed, trying, or unattempted.\n";
            }
        } while (!isValidStatus(userStatus));
        data.status = normalizeStatus(userStatus);

        cout << "Times attempted: ";
        cin >> data.attempts;
        cin.ignore();
        cout << "Notes: ";
        getline(cin, data.notes);
        cout << "Video link: ";
        getline(cin, data.videoLink);
        cout << "Photo link: ";
        getline(cin, data.photoLink);
    }

    void display(int index) const {
        cout << "\n--- Trick " << index + 1 << " ---\n";
        cout << "Name: " << data.name << "\n";
        cout << "Status: " << data.status << "\n";
        cout << "Attempts: " << data.attempts << "\n";
        cout << "Notes: " << data.notes << "\n";
        cout << "Video Link: " << data.videoLink << "\n";
        cout << "Photo Link: " << data.photoLink << "\n";
    }

    void update() {
        cout << "Updating: " << data.name << "\n";

        string userStatus;
        do {
            cout << "New status (completed/trying/unattempted) [current: " << data.status << "]: ";
            getline(cin, userStatus);
            if (!isValidStatus(userStatus)) {
                cout << "[ERROR] Invalid status. Please enter exactly: completed, trying, or unattempted.\n";
            }
        } while (!isValidStatus(userStatus));
        data.status = normalizeStatus(userStatus);

        int addAttempts;
        cout << "Add how many attempts? ";
        cin >> addAttempts;
        cin.ignore();
        data.attempts += addAttempts;

        cout << "Add notes (leave empty to keep current): ";
        string newNotes;
        getline(cin, newNotes);
        if (!newNotes.empty()) data.notes += "\n" + newNotes;

        cout << "New video link (leave empty to keep current): ";
        string newVideo;
        getline(cin, newVideo);
        if (!newVideo.empty()) data.videoLink = newVideo;

        cout << "New photo link (leave empty to keep current): ";
        string newPhoto;
        getline(cin, newPhoto);
        if (!newPhoto.empty()) data.photoLink = newPhoto;
    }

    void save(ofstream& out) const {
        out << data.name << "\n"
            << data.status << "\n"
            << data.attempts << "\n"
            << data.notes << "\n"
            << data.videoLink << "\n"
            << data.photoLink << "\n---\n";
    }

    bool load(ifstream& in) {
        if (!getline(in, data.name)) return false;
        getline(in, data.status);
        in >> data.attempts;
        in.ignore();
        getline(in, data.notes);
        getline(in, data.videoLink);
        getline(in, data.photoLink);
        string separator;
        getline(in, separator); // read '---'
        return true;
    }
};

// Global trick array and count
Trick tricks[MAX_TRICKS];
int trickCount = 0;

// Add new trick
void addTrick() {
    if (trickCount >= MAX_TRICKS) {
        cout << "[ERROR] Maximum number of tricks reached.\n";
        return;
    }
    tricks[trickCount].inputNew();
    trickCount++;
    cout << "[OK] Trick added.\n";
}

// View all tricks
void viewTricks() {
    if (trickCount == 0) {
        cout << "[INFO] No tricks logged yet.\n";
        return;
    }
    for (int i = 0; i < trickCount; ++i) {
        tricks[i].display(i);
    }
}

// Update trick
void updateTrick() {
    viewTricks();
    if (trickCount == 0) return;

    int index;
    cout << "\nEnter trick number to update: ";
    cin >> index;
    cin.ignore();

    if (index < 1 || index > trickCount) {
        cout << "[ERROR] Invalid trick number.\n";
        return;
    }

    tricks[index - 1].update();
    cout << "[OK] Trick updated.\n";
}

// Save tricks to two files
void saveToFile() {
    ofstream outTxt(FILENAME);
    ofstream outLog("TrickLog.dat");

    if (!outTxt.is_open() || !outLog.is_open()) {
        cout << "[ERROR] Could not open output file(s) for saving.\n";
        return;
    }

    for (int i = 0; i < trickCount; ++i) {
        tricks[i].save(outTxt);  // Save to BMXTricks.txt
        tricks[i].save(outLog);  // Also save to TrickLog.dat
    }

    outTxt.close();
    outLog.close();
    cout << "[OK] Data saved to BMXTricks.txt and TrickLog.dat\n";
}

// Load tricks from file
void loadFromFile() {
    ifstream in(FILENAME);
    if (!in.is_open()) {
        cout << "[INFO] No existing log found. Starting fresh.\n";
        return;
    }

    trickCount = 0;
    while (trickCount < MAX_TRICKS && tricks[trickCount].load(in)) {
        trickCount++;
    }
    in.close();
    cout << "[OK] Data loaded from BMXTricks.txt\n";
}

// Main menu loop
int main() {
    loadFromFile();

    int choice;
    do {
        cout << "\n===== BMX Trick Logger =====\n";
        cout << "1. Add Trick\n";
        cout << "2. View Tricks\n";
        cout << "3. Update Trick\n";
        cout << "4. Save & Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: addTrick(); break;
        case 2: viewTricks(); break;
        case 3: updateTrick(); break;
        case 4: saveToFile(); break;
        default: cout << "[ERROR] Invalid choice.\n";
        }
    } while (choice != 4);

    return 0;
}
