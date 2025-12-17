#define BLUE  "\033[96m" 
#define RED   "\033[91m"   
#define YELLOW "\033[93m"    
#define GREEN "\033[92m"   
#define RESET "\033[0m"   
#include <iostream>
#include <string>
#include <iomanip>      // for setw (which is used for spaces) setprecision(it is used to tell programm how many deciaml places user want to include)
#include <fstream>      // to export into text file
#include <limits>       // for numeric_limits(like when user enters invalid input weve to ignore untill when)
using namespace std;
// Constants and Arrays ,constants are used for array size because array size cant be changed once fixed,
and array size sould be defined before using array otherwise it'll not work
const int MAX_STUDENTS = 50;
const int MAX_DAYS = 30;
const int SUBJECTS = 3;
// parallel arrays: data for student i is stored at index i in every array
int rollNumbers[MAX_STUDENTS];            // rollNumbers[i] = roll of student i
string names[MAX_STUDENTS];              // names[i] = name of student i
int marksArr[MAX_STUDENTS][SUBJECTS];    // marksArr[i][0..2] = marks of student i in 3 subjects
double averages[MAX_STUDENTS];           // averages[i] = average marks of student i (double now)
char gradesArr[MAX_STUDENTS];            // gradesArr[i] = grade of student i ('A','B','C','F')
bool attendance[MAX_STUDENTS][MAX_DAYS]; // attendance[i][d] = true if student i was present on day d
int studentCount = 0;         // how many students are currently stored
int totalAttendanceDays = 0;  // how many days of attendance have been recorded

// additional functions for programm efficency
void clearInput() {
    // we'll call this if user enters invalid input,like char instead of integer ,this will ignore invalid inputs and clear the buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int findIndexByRoll(int roll) {
    // returns index of student with given roll, or -1 if not found
    for (int i = 0; i < studentCount; ++i)
        if (rollNumbers[i] == roll) return i;
    return -1;
}

char calculateGrade(double avg) {
    // calculating grades
    if (avg >= 85.0) return 'A';
    if (avg >= 70.0) return 'B';
    if (avg >= 50.0) return 'C';
    return 'F';
}

void recalcStudent(int idx) {
    // recalculate total, average and grade for student idx
    int total = 0;
    for (int s = 0; s < SUBJECTS; ++s) total += marksArr[idx][s];
    averages[idx] = static_cast<double>(total) / SUBJECTS;
    gradesArr[idx] = calculateGrade(averages[idx]);
}

// a specific task is performed by a specific function

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << RED << "Sorry you Can't add more students. Limit is reached (" << RESET << MAX_STUDENTS << ").\n";
        return;
    }

    cout << "Enter roll number (integer): ";
    int roll;
    if (!(cin >> roll)) { cout << RED << "Invalid roll number!\n" << RESET; clearInput(); return; }
    clearInput();
    if (findIndexByRoll(roll) != -1) {
        cout << RED << "This roll number already exists. Use a unique roll number.\n" << RESET;
        return;
    }

    cout << "Enter student name: ";
    string name;
    getline(cin, name);//stores everything user type not just the first word helfull in double names of individuals,it also stores spaces
    if (name.empty()) { cout << RED <<"Name cannot be empty.\n" << RESET; return; }

    // this will  Read marks with validation 0..100
    for (int s = 0; s < SUBJECTS; ++s) {
        int m;
        while (true) {
            cout << "Marks for subject " << (s + 1) << " (0-100): ";
            if (!(cin >> m)) { cout << "Enter a number.\n"; clearInput(); continue; }
            if (m < 0 || m > 100) { cout << RED << "Marks must be between 0 to 100!\n" << RESET; continue; }
            marksArr[studentCount][s] = m;
            break;
        }
    }
    clearInput();

    // this will store a student
    rollNumbers[studentCount] = roll;
    names[studentCount] = name;
    // initialize attendance for previous marked days as absent
    for (int d = 0; d < totalAttendanceDays; ++d) attendance[studentCount][d] = false;
    // recalculate  average and grade
    recalcStudent(studentCount);
    studentCount++;
    cout << GREEN << "Student added successfully.\n" << RESET;
}

void displayOneStudent(int idx) {
    // prints one student's complete info using arrays
    if (idx < 0 || idx >= studentCount) return;

    cout << "Name : " << names[idx] << "\n";
    cout << "Roll : " << rollNumbers[idx] << "\n";
    int total = 0;
    for (int s = 0; s < SUBJECTS; ++s) {

        cout << "Subject " << (s + 1) << " marks: " << marksArr[idx][s] << "\n";
        total += marksArr[idx][s];
    }
    cout << "Total : " << total << "\n";
    cout << "Average : " << fixed << setprecision(2) << averages[idx] << "\n";
    cout << "Grade : " << gradesArr[idx] << "\n";

    if (totalAttendanceDays > 0) {
        int present = 0;
        for (int d = 0; d < totalAttendanceDays; ++d) if (attendance[idx][d]) present++;
        double pct = (present * 100.0) / totalAttendanceDays;
        cout << "Attendance : " << present << " / " << totalAttendanceDays << " (" << fixed << setprecision(1) << pct << "%)\n";
    }
    else {
        cout << RED << "Attendance : not recorded yet\n" << RESET;
    }
}

void displayAllStudents() {
    if (studentCount == 0) {
        cout << RED << "No students added to display!\n" << RESET;
        return;
    }
    cout<<BLUE<<"-----------------------------------"<<RESET;
    cout << "\n       All student records            \n";
    cout << BLUE << "-----------------------------------" << RESET;
    for (int i = 0; i < studentCount; ++i) {
        cout << "\nStudent #" << (i + 1) << "\n";
        displayOneStudent(i);
    }
}

void searchStudent() {
    if (studentCount == 0) {
        cout <<RED<<"No students in system!"<<RESET<<endl;
        return;
    }
    cout << "Enter roll number to search: ";
    int roll;
    if (!(cin >> roll)) { cout <<RED<< "Invalid roll.\n"<<RESET; clearInput(); return; }
    clearInput();

    int idx = findIndexByRoll(roll);
    if (idx == -1) {
        cout <<RED"Student not found.\n"<<RESET;
        return;
    }
    displayOneStudent(idx);

    if (totalAttendanceDays > 0) {
        cout << "Attendance by day (P = present, A = absent):\n";
        for (int d = 0; d < totalAttendanceDays; ++d) {
            cout << "Day " << (d + 1) << ": " << (attendance[idx][d] ? 'P' : 'A') << "\n";
        }
    }
}

void updateMarks() {
    if (studentCount == 0) { cout <<RED<<"No students to update.\n"<<RESET; return; }
    cout << "Enter roll number to update marks: ";
    int roll; if (!(cin >> roll)) { cout <<RED<< "Invalid roll.\n"<<RESET; clearInput(); return; }
    clearInput();

    int idx = findIndexByRoll(roll);
    if (idx == -1) { cout <<RED<<"Student not found.\n"<<RESET; return; }

    cout << "Updating marks for " << names[idx] << " (Roll " << roll << ")\n";
    int m;
    for (int s = 0; s < SUBJECTS; ++s) {
        while (true) {
            cout << "New marks for subject " << (s + 1) << " (0-100): ";
            if (!(cin >> m)) {
                cout << "Enter a number.\n";
                clearInput();
                continue;
            }
            if (m < 0 || m > 100) {
                cout << "Marks must be 0-100.\n";
                continue;
            }
            marksArr[idx][s] = m;
            break;
        }
    }
    clearInput();
    recalcStudent(idx);
    cout << "Marks updated.\n";
}

void deleteStudent() {
    if (studentCount == 0) { cout <<RED<<"No students to delete.\n"<<RESET; return; }
    cout << "Enter roll number to delete: ";
    int roll; if (!(cin >> roll)) { cout <<RED<<"Invalid roll.\n"<<RESET; clearInput(); return; }
    clearInput();

    int idx = findIndexByRoll(roll);
    if (idx == -1) { cout <<RED<<"Student not found.\n"<<RESET; return; }

    cout <<YELLOW<<"Are you sure you want to delete " << names[idx] << " ? (y/n): "<<RESET;
    char c; cin >> c; clearInput();
    if (c != 'y' && c != 'Y') { cout << "Deletion cancelled.\n"; return; }

    // shift data left to remove student,
    for (int i = idx; i < studentCount - 1; ++i) {
        rollNumbers[i] = rollNumbers[i + 1];
        names[i] = names[i + 1];
        for (int s = 0; s < SUBJECTS; ++s) marksArr[i][s] = marksArr[i + 1][s];
        averages[i] = averages[i + 1];
        gradesArr[i] = gradesArr[i + 1];
        for (int d = 0; d < totalAttendanceDays; ++d) attendance[i][d] = attendance[i + 1][d];
    }
    studentCount--;
    cout <<GREEN<<"Student deleted successfully..\n"<<RESET;
}
void top3Students() {
    if (studentCount < 3) { cout << "At least 3 students should be added to show top 3.\n"; return; }
    // simple index array sort by average descending order (bubble sort)
    int idxs[MAX_STUDENTS];
    for (int i = 0; i < studentCount; ++i) idxs[i] = i;
    for (int i = 0; i < studentCount - 1; ++i) {
        for (int j = 0; j < studentCount - i - 1; ++j) {
            if (averages[idxs[j]] < averages[idxs[j + 1]]) {
                //swapping in desceding order
                int tmp = idxs[j];
                idxs[j] = idxs[j + 1];
                idxs[j + 1] = tmp;
            }
        }
    }
    cout << "\nTop 3 students by average:\n";
    for (int k = 0; k < 3; ++k) {
        int i = idxs[k];
        cout << (k + 1) << ". " << names[i] << " (Roll " << rollNumbers[i] << ") - Avg: "
            << fixed << setprecision(2) << averages[i] << " Grade: " << gradesArr[i] << "\n";
    }
}

void gradeSummary() {
    if (studentCount == 0) { cout <<RED<<"No students.\n"<<RESET; return; }
    int a = 0, b = 0, c = 0, f = 0;
    for (int i = 0; i < studentCount; ++i) {
        if (gradesArr[i] == 'A') a++;
        else if (gradesArr[i] == 'B') b++;
        else if (gradesArr[i] == 'C') c++;
        else if (gradesArr[i] == 'F') f++;
    }
    cout << "Grade summary: A=" << a << " B=" << b << " C=" << c << " F=" << f << "\n";
    if (f > 0) {
        cout <<RED<<"Failed students:\n"<<RESET;
        for (int i = 0; i < studentCount; ++i)
            if (gradesArr[i] == 'F')
                cout << names[i] << " (Roll " << rollNumbers[i] << ") Avg: " << fixed << setprecision(2) << averages[i] << "\n";
    }
}

// Attendance features 

void markDailyAttendance() {
    if (studentCount == 0) { cout <<RED<<"No students to mark attendance.\n"<<RESET; return; }

    cout << "\nAttendance menu:\n";
    cout << "1) Record attendance for next day (day " << (totalAttendanceDays + 1) << ")\n";
    cout << "2) Edit attendance for an existing day\n";
    cout << "3) Cancel\n";
    cout << "Choose: ";
    int choice; if (!(cin >> choice)) { cout << "Invalid choice.\n"; clearInput(); return; }
    clearInput();

    if (choice == 3) { cout << "Cancelled.\n"; return; }

    if (choice == 1) {
        // confirm to avoid accidental duplicate marking of attendance for the same day.
        cout << "Confirm record attendance for day " << (totalAttendanceDays + 1) << " (y/n): ";
        char c;
        cin >> c;
        clearInput();
        if (c != 'y' && c != 'Y') { cout << "Cancelled.\n"; return; }

        // marking attendence for each student
        cout << "Enter P for present or A for absent for each student:\n";
        for (int i = 0; i < studentCount; ++i) {
            while (true) {
                cout << rollNumbers[i] << " - " << names[i] << ": ";
                char ch;
                if (!(cin >> ch)) { clearInput(); continue; }
                if (ch == 'P' || ch == 'p') { attendance[i][totalAttendanceDays] = true; break; }
                else if (ch == 'A' || ch == 'a') { attendance[i][totalAttendanceDays] = false; break; }
                else cout << "Enter P or A only.\n";
            }
        }
        clearInput();
        totalAttendanceDays++;
        cout << "Attendance recorded for day " << totalAttendanceDays << ".\n";
    }
    else if (choice == 2) {
        if (totalAttendanceDays == 0) { cout <<RED<<"No days recorded yet.\n"<<RESET; return; }
        cout << "Enter day number to edit (1 to " << totalAttendanceDays << "): ";
        int day; if (!(cin >> day)) { cout << "Invalid day.\n"; clearInput(); return; }
        clearInput();
        if (day < 1 || day > totalAttendanceDays) { cout << "Day is out of range.\n"; return; }
        int d = day - 1;
        cout << "Editing day " << day << ". Confirm (y/n): ";
        char c;
        cin >> c;
        clearInput();
        if (c != 'y' && c != 'Y') { cout << "Cancelled.\n"; return; }

        for (int i = 0; i < studentCount; ++i) {
            while (true) {
                cout << rollNumbers[i] << " - " << names[i] << " (current: " << (attendance[i][d] ? 'P' : 'A') << "): ";
                char ch;
                if (!(cin >> ch)) { clearInput(); continue; }
                if (ch == 'P' || ch == 'p') { attendance[i][d] = true; break; }
                else if (ch == 'A' || ch == 'a') { attendance[i][d] = false; break; }
                else cout << "Enter P or A only.\n";
            }
        }
        clearInput();
        cout << "Day " << day << " edited.\n";
    }
    else {
        cout << "Invalid option.\n";
    }
}

void displayAttendanceOfStudent() {
    if (studentCount == 0) { cout <<RED<<"No students record found!\n"<<RESET; return; }
    if (totalAttendanceDays == 0) { cout <<RED<<"No attendance recorded yet.\n"<<RESET; return; }

    cout << "Enter roll number to view attendance: ";
    int roll;
    if (!(cin >> roll)) { cout <<RED<<"Invalid roll.\n"<<RESET; clearInput(); return; }
    clearInput();
    int idx = findIndexByRoll(roll);
    if (idx == -1) { cout <<RED<<"sorry! Student not found.\n"<<RESET; return; }
    cout << "Attendance for " << names[idx] << " (Roll " << roll << "):\n";
    for (int d = 0; d < totalAttendanceDays; ++d)
        cout << "Day " << (d + 1) << ": " << (attendance[idx][d] ? 'P' : 'A') << "\n";
}

void attendancePercentageReport() {
    if (studentCount == 0) { cout <<RED<<"No students record found!\n"<<RESET; return; }
    if (totalAttendanceDays == 0) { cout <<RED<<"No attendance recorded yet.\n"<<RESET; return; }

    cout << left << setw(10) << "Roll" << setw(20) << "Name" << setw(10) << "Present" << setw(8) << "Days" << setw(10) << "Percent" << "Note\n";
    for (int i = 0; i < studentCount; ++i) {
        int present = 0;
        for (int d = 0; d < totalAttendanceDays; ++d) if (attendance[i][d]) present++;
        double pct = (present * 100.0) / totalAttendanceDays;
        cout << left << setw(10) << rollNumbers[i] << setw(20) << names[i] << setw(10) << present << setw(8) << totalAttendanceDays << setw(9) << fixed << setprecision(1) << pct << "%";
        if (pct < 75.0) cout <<RED<<"  Below 75%"<<RESET;
        cout << "\n";
    }
}

// Export to text file: student_summary.txt
void exportSummariesToFile() {
    ofstream out("student_summary.txt");
    if (!out) { cout <<RED<<"Failed to create export file.\n"<<RESET; return; }

    out <<BLUE<<"Student Summary Export\n"<<RESET;
    out << "======================\n\n";
    for (int i = 0; i < studentCount; ++i) {
        out << "Name: " << names[i] << "\n";
        out << "Roll: " << rollNumbers[i] << "\n";
        int total = 0;
        for (int s = 0; s < SUBJECTS; ++s) {
            out << "Marks subj" << (s + 1) << ": " << marksArr[i][s] << "\n";
            total += marksArr[i][s];
        }
        out << "Total: " << total << "\n";
        out << "Average: " << fixed << setprecision(2) << averages[i] << "\n";
        out << "Grade: " << gradesArr[i] << "\n";
        if (totalAttendanceDays > 0) {
            int present = 0;
            for (int d = 0; d < totalAttendanceDays; ++d) if (attendance[i][d]) present++;
            double pct = (present * 100.0) / totalAttendanceDays;
            out << "Attendance: " << present << " / " << totalAttendanceDays << " (" << fixed << setprecision(1) << pct << "%)\n";
        }
        else {
            out << "Attendance: none\n";
        }
        out << "------------------------------------\n";
    }
    out.close();
    cout << "Exported summaries to 'student_summary.txt'.\n";
}

//  Main menu in this we'll just call functions ..
int main() {
    // initialize arrays to sensible defaults
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        rollNumbers[i] = 0;
        names[i] = "";
        averages[i] = 0.0;
        gradesArr[i] = 'F';
        for (int s = 0; s < SUBJECTS; ++s) marksArr[i][s] = 0;
        for (int d = 0; d < MAX_DAYS; ++d) attendance[i][d] = false;
    }
    cout <<BLUE<<"--------------------------------------------------------------\n   WELCOME TO STUDENT MANAGEMENT SYSTEM   \n--------------------------------------------------------------"<<RESET;
    while (true) {
        cout << "\nMenu\n";
        cout << "1. Add new student" << endl;
        cout << "2. Display all students"<<endl;
        cout << "3. Search student by roll"<<endl;
        cout << "4. Update student marks"<<endl;
        cout << "5. Delete student"<<endl;
        cout << "6. Top 3 students"<<endl;
        cout << "7. Grade summary and failed list"<<endl;
        cout << "8. Mark or edit attendance"<<endl;
        cout << "9. Display attendance of a student"<<endl;
        cout << "10. Attendance percentage report"<<endl;
        cout << "11. Export summaries to text file"<<endl;
        cout << "0. Exit\n" << endl;
        cout << BLUE<<"Choose option: "<<RESET;

        int choice;
        if (!(cin >> choice)) { cout << "Please enter a number.\n"; clearInput(); continue; }
        clearInput();

        if (choice == 1) addStudent();
        else if (choice == 2) displayAllStudents();
        else if (choice == 3) searchStudent();
        else if (choice == 4) updateMarks();
        else if (choice == 5) deleteStudent();
        else if (choice == 6) top3Students();
        else if (choice == 7) gradeSummary();
        else if (choice == 8) markDailyAttendance();
        else if (choice == 9) displayAttendanceOfStudent();
        else if (choice == 10) attendancePercentageReport();
        else if (choice == 11) exportSummariesToFile();
        else if (choice == 0) { cout << "Exiting... Goodbye..\n"; break; }
        else cout <<RED<<"Invalid choice please Try again with a valid choice a\n"<<RESET;
    }

    return 0;
}