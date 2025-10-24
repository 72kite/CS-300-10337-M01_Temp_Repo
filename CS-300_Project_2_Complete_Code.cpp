//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Zion D. Kinniebrew-Jenkins
// Version     : 1.0
// Description : Advising Assistance Program - Course Management System
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

//============================================================================
// Course Structure Definition
//============================================================================

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    // Default constructor
    Course() {}

    // Parameterized constructor
    Course(string number, string title) {
        courseNumber = number;
        courseTitle = title;
    }
};

//============================================================================
// Binary Search Tree Class Definition for Course Storage
//============================================================================

class BinarySearchTree {
private:
    // Node structure for BST
    struct Node {
        Course course;
        Node* left;
        Node* right;

        // Default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // Constructor with course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    int courseCount; // Track number of courses

    // Private helper methods
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* searchNode(Node* node, string courseNumber);
    void destroyRecursive(Node* node);
    int countNodes(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    Course Search(string courseNumber);
    int Size();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    courseCount = 0;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    destroyRecursive(root);
}

/**
 * Recursively destroy nodes to prevent memory leaks
 */
void BinarySearchTree::destroyRecursive(Node* node) {
    if (node != nullptr) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

/**
 * Insert a course into the BST
 */
void BinarySearchTree::Insert(Course course) {
    // If tree is empty, create root node
    if (root == nullptr) {
        root = new Node(course);
        courseCount++;
    }
    else {
        // Call recursive helper to insert
        addNode(root, course);
        courseCount++;
    }
}

/**
 * Private helper method to recursively add a node
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // If course number is less than current node, go left
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    // Otherwise go right
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

/**
 * Print all courses in order (alphanumeric)
 */
void BinarySearchTree::InOrder() {
    int totalCourses = Size();
    cout << "\nHere is a sample schedule:\n" << endl;
    cout << "Displaying " << totalCourses << " course(s):\n" << endl;
    inOrder(root);
    cout << "\nTotal: " << totalCourses << " courses listed." << endl;
}

/**
 * Private helper method for in-order traversal
 */
void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    // Traverse left subtree
    inOrder(node->left);

    // Print current node
    cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;

    // Traverse right subtree
    inOrder(node->right);
}

/**
 * Search for a course by course number
 */
Course BinarySearchTree::Search(string courseNumber) {
    Node* result = searchNode(root, courseNumber);

    if (result != nullptr) {
        return result->course;
    }

    // Return empty course if not found
    return Course();
}

/**
 * Private helper method to recursively search for a node
 */
BinarySearchTree::Node* BinarySearchTree::searchNode(Node* node, string courseNumber) {
    // Base case: node is null or course found
    if (node == nullptr) {
        return nullptr;
    }

    if (node->course.courseNumber == courseNumber) {
        return node;
    }

    // Search left or right based on comparison
    if (courseNumber < node->course.courseNumber) {
        return searchNode(node->left, courseNumber);
    }
    else {
        return searchNode(node->right, courseNumber);
    }
}

/**
 * Get the number of courses in the BST
 */
int BinarySearchTree::Size() {
    return courseCount;
}

/**
 * Count nodes recursively (alternative method)
 */
int BinarySearchTree::countNodes(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

//============================================================================
// Static methods for file operations and utilities
//============================================================================

/**
 * Convert string to uppercase for case-insensitive comparison
 */
string toUpperCase(string str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

/**
 * Trim whitespace from beginning and end of string
 */
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * Parse a CSV line into tokens
 */
vector<string> parseLine(string line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (getline(ss, token, ',')) {
        token = trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

/**
 * Validate that all prerequisites exist in the course list
 */
bool validatePrerequisites(vector<Course>& courses, vector<string>& allCourseNumbers) {
    bool allValid = true;

    for (const Course& course : courses) {
        for (const string& prereq : course.prerequisites) {
            bool prereqExists = false;

            // Search for prerequisite in list of all course numbers
            for (const string& courseNum : allCourseNumbers) {
                if (prereq == courseNum) {
                    prereqExists = true;
                    break;
                }
            }

            // If prerequisite not found, report error
            if (!prereqExists) {
                cout << "Warning: Course " << course.courseNumber
                    << " has prerequisite " << prereq
                    << " which does not exist in the course list." << endl;
                allValid = false;
            }
        }
    }

    return allValid;
}

/**
 * Load courses from CSV file into BST
 */
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading course data from file: " << csvPath << endl;

    ifstream file(csvPath);

    if (!file.is_open()) {
        cout << "Error: Could not open file " << csvPath << endl;
        return false;
    }

    vector<Course> tempCourses;
    vector<string> allCourseNumbers;
    string line;
    int lineNumber = 0;

    // First pass: Read all lines and perform basic validation
    while (getline(file, line)) {
        lineNumber++;

        // Skip empty lines
        if (trim(line).empty()) {
            continue;
        }

        // Parse the line into tokens
        vector<string> tokens = parseLine(line);

        // Validate: Check for at least 2 parameters (course number and title)
        if (tokens.size() < 2) {
            cout << "Error on line " << lineNumber << ": Missing course number or title" << endl;
            cout << "Each line must have at least course number and course title" << endl;
            file.close();
            return false;
        }

        // Create course object
        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        // Add prerequisites if they exist (starting from index 2)
        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }

        // Store course for validation
        tempCourses.push_back(course);
        allCourseNumbers.push_back(course.courseNumber);
    }

    file.close();

    // Second pass: Validate prerequisites
    bool validationPassed = validatePrerequisites(tempCourses, allCourseNumbers);

    if (!validationPassed) {
        cout << "Warning: Some prerequisite validation issues were found, but continuing to load data." << endl;
    }

    // Third pass: Insert all courses into BST
    for (const Course& course : tempCourses) {
        bst->Insert(course);
    }

    cout << "Successfully loaded " << tempCourses.size() << " courses." << endl;
    cout << "\nData Summary:" << endl;
    cout << "  Total Courses: " << tempCourses.size() << endl;

    // Count courses with and without prerequisites
    int coursesWithPrereqs = 0;
    int totalPrereqs = 0;
    for (const Course& course : tempCourses) {
        if (!course.prerequisites.empty()) {
            coursesWithPrereqs++;
            totalPrereqs += course.prerequisites.size();
        }
    }
    cout << "  Courses with Prerequisites: " << coursesWithPrereqs << endl;
    cout << "  Total Prerequisites Listed: " << totalPrereqs << endl;

    return true;
}

/**
 * Display a single course with its prerequisites
 */
void displayCourse(Course course, BinarySearchTree* bst) {
    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << "\n--- Course Information ---" << endl;
    cout << course.courseNumber << ", " << course.courseTitle << endl;
    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        cout << endl;
        // Display each prerequisite with its title
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            Course prereqCourse = bst->Search(course.prerequisites[i]);
            cout << "  - " << course.prerequisites[i];

            // If we found the prerequisite course, show its title
            if (!prereqCourse.courseNumber.empty()) {
                cout << ": " << prereqCourse.courseTitle;
            }
            cout << endl;
        }
    }
    cout << "--------------------------" << endl;
}

/**
 * Display the main menu
 */
void displayMenu() {
    cout << "\n====================================" << endl;
    cout << "Welcome to the course planner." << endl;
    cout << "====================================" << endl;
    cout << "  1. Load Data Structure." << endl;
    cout << "  2. Print Course List." << endl;
    cout << "  3. Print Course." << endl;
    cout << "  9. Exit" << endl;
    cout << "====================================" << endl;
    cout << "What would you like to do? ";
}

//============================================================================
// Main Program
//============================================================================

int main() {
    // Create BST to store courses
    BinarySearchTree* courseBST = new BinarySearchTree();

    string courseNumber;
    int choice = 0;
    bool dataLoaded = false;

    // Display title screen
    cout << "\n" << endl;
    cout << "   ###    ####    ####   ##  ##" << endl;
    cout << "  ## ##   ##  ##  ##     ##  ##" << endl;
    cout << " ##   ##  ####    ##     ##  ##" << endl;
    cout << " #######  ##  ##  ##     ##  ##" << endl;
    cout << " ##   ##  ####    ####   ######" << endl;
    cout << "\n     ADVISING PROGRAM" << endl;
    cout << "     ================\n" << endl;

    // Preload data at program start with hardcoded filename
    string fileName = "CS 300 ABCU_Advising_Program_Input.csv";
    cout << "Loading course data..." << endl;

    dataLoaded = loadCourses(fileName, courseBST);

    // Main program loop
    while (choice != 9) {
        displayMenu();

        // Get user input with error handling
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number." << endl;
            continue;
        }

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            // Load data from file with option to use default or custom file
        {
            int fileChoice = 0;
            cout << "\n--- Load Data Options ---" << endl;
            cout << "1. Use default file (CS 300 ABCU_Advising_Program_Input.csv)" << endl;
            cout << "2. Enter custom file path" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> fileChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input." << endl;
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Clear existing BST to prevent duplicates
            delete courseBST;
            courseBST = new BinarySearchTree();
            dataLoaded = false;

            if (fileChoice == 1) {
                fileName = "CS 300 ABCU_Advising_Program_Input.csv";
                cout << "\nUsing default file: " << fileName << endl;
                dataLoaded = loadCourses(fileName, courseBST);
            }
            else if (fileChoice == 2) {
                cout << "\nEnter the file name or path: ";
                getline(cin, fileName);
                dataLoaded = loadCourses(fileName, courseBST);
            }
            else {
                cout << "Invalid option." << endl;
            }
        }
        break;

        case 2:
            // Print all courses in alphanumeric order
            if (!dataLoaded) {
                cout << "\nError: No data loaded. Please load data first (Option 1)." << endl;
            }
            else {
                courseBST->InOrder();
            }
            break;

        case 3:
            // Print specific course
            if (!dataLoaded) {
                cout << "\nError: No data loaded. Please load data first (Option 1)." << endl;
            }
            else {
                cout << "\nWhat course do you want to know about? ";
                getline(cin, courseNumber);

                // Handle partial search if user wants
                if (courseNumber.length() > 0) {
                    // Convert to uppercase for case-insensitive search
                    courseNumber = toUpperCase(courseNumber);

                    Course course = courseBST->Search(courseNumber);

                    if (course.courseNumber.empty()) {
                        cout << "\nCourse '" << courseNumber << "' not found." << endl;
                        cout << "Tip: Make sure to enter the complete course number (e.g., CSCI101, MATH201)" << endl;
                    }
                    else {
                        displayCourse(course, courseBST);
                    }
                }
                else {
                    cout << "No course number entered." << endl;
                }
            }
            break;

        case 9:
            // Exit program
            cout << "\nThank you for using the course planner!" << endl;
            break;

        default:
            // Invalid option
            cout << "\n" << choice << " is not a valid option." << endl;
            break;
        }
    }

    // Clean up memory
    delete courseBST;

    return 0;
}