#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string DAYS_OF_WEEK[] = { "sun", "mon", "tue", "wed", "thur", "fri", "sat" };
const int NUM_DAYS_OF_WEEK = sizeof(DAYS_OF_WEEK) / sizeof(string);

const int MAX_STUDENTS = 100;  // Maximum number of students for each time slot

struct Node {
    double time;
    int count;
    string students[MAX_STUDENTS];  // Array to store student names
    Node* next;
    Node(double time) : time(time), count(1), next(NULL) {}
};

class ScheduleList {
public:
    ScheduleList() {
        for (int i = 0; i < NUM_DAYS_OF_WEEK; i++) {
            head[i] = NULL;
            tail[i] = NULL;
        }
    }
    void addTime(string studentName, string day, double time) {
        int dayIndex = getDayIndex(day);
        if (dayIndex == -1) return;
        Node* newNode = new Node(time);
        newNode->students[0] = studentName;  // Store the first student in the array
        if (!head[dayIndex]) {
            head[dayIndex] = newNode;
            tail[dayIndex] = newNode;
        }
        else {
            Node* prev = NULL;
            Node* current = head[dayIndex];
            while (current && current->time < time) {
                prev = current;
                current = current->next;
            }
            if (current && current->time == time) {
                current->count++;
                // Check if the student is already present before adding
                bool studentAlreadyPresent = false;
                for (int i = 0; i < MAX_STUDENTS; i++) {
                    if (current->students[i] == studentName) {
                        studentAlreadyPresent = true;
                        break;
                    }
                    else if (current->students[i].empty()) {
                        current->students[i] = studentName;
                        break;
                    }
                }
                if (!studentAlreadyPresent) {
                    delete newNode;
                }
            }
            else {
                newNode->next = current;
                if (prev) {
                    prev->next = newNode;
                }
                else {
                    head[dayIndex] = newNode;
                }
            }
        }
    }
    void displayTimesWithMinimumMatches(int minMatches) {
        cout << "Times with " << minMatches << " or more matches:" << endl;
        for (int i = 0; i < NUM_DAYS_OF_WEEK; i++) {
            Node* current = head[i];
            while (current) {
                if (current->count >= minMatches) {
                    cout << DAYS_OF_WEEK[i] << ": " << current->time << " (" << current->count << " matches)" << endl;
                    cout << "  students: ";
                    for (int j = 0; j < MAX_STUDENTS && !current->students[j].empty(); j++) {
                        cout << current->students[j] << " ";
                    }
                    cout << endl;
                }
                current = current->next;
            }
        }
    }
private:
    int getDayIndex(string day) {
        for (int i = 0; i < NUM_DAYS_OF_WEEK; i++) {
            if (day == DAYS_OF_WEEK[i]) {
                return i;
            }
        }
        cout << "Invalid day: " << day << endl;
        return -1;
    }
    Node* head[NUM_DAYS_OF_WEEK];
    Node* tail[NUM_DAYS_OF_WEEK];
};

int main() {
    ScheduleList scheduleList;
    string fileName;
    while (true) {
        cout << "Enter a file name for a student schedule (or enter 'done' to finish): ";
        cin >> fileName;
        if (fileName == "done") {
            break;
        }
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << fileName << endl;
        }
        else {
            string line;
            while (getline(file, line)) {
                int colonIndex = line.find(":");
                if (colonIndex == -1) {
                    cout << "Error: Invalid line format in file " << fileName << ": " << line << endl;
                    continue;
                }
                string studentName = line.substr(0, colonIndex);
                string day = line.substr(colonIndex + 1, 3);
                double time = stod(line.substr(colonIndex + 5));
                scheduleList.addTime(studentName, day, time);
            }
            file.close();
        }
    }
    int minMatches;
    cout << "Enter the minimum number of matches: ";
    cin >> minMatches;
    scheduleList.displayTimesWithMinimumMatches(minMatches);
    return 0;
}
