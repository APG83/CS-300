/*
============================================================================
ABCU Advising Program
Author: Adam Gomez
Date: 08/17/2025
Description:
    This program allows students and advisors to:
      1. Load course data from a CSV file into memory.
      2. Print an alphabetically sorted list of all courses.
      3. Print details (title and prerequisites) for a specific course.
      9. Exit the program.

    The program is built for clarity, maintainability, and scalability.
============================================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

/*
============================================================================
Struct: Course
    Represents a single course with its number, title, and prerequisites.
============================================================================
*/
struct Course {
    std::string courseNumber;
    std::string courseTitle;
    std::vector<std::string> prerequisites;
};

/*
============================================================================
Function: ToUpperCase
    Converts a string to uppercase (used for case-insensitive lookups).
============================================================================
*/
std::string ToUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}

/*
============================================================================
Function: LoadCoursesFromFile
    Loads courses from a CSV file into a vector of Course objects.
    Each row format: CourseNumber,CourseTitle,Prereq1,Prereq2,...
============================================================================
*/
std::vector<Course> LoadCoursesFromFile(const std::string& filename) {
    std::vector<Course> courses;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return courses;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Course course;

        // First token: course number
        std::getline(ss, token, ',');
        course.courseNumber = ToUpperCase(token);

        // Second token: course title
        std::getline(ss, token, ',');
        course.courseTitle = token;

        // Remaining tokens: prerequisites
        while (std::getline(ss, token, ',')) {
            if (!token.empty()) {
                course.prerequisites.push_back(ToUpperCase(token));
            }
        }

        courses.push_back(course);
    }

    file.close();
    return courses;
}

/*
============================================================================
Function: PrintCourseList
    Prints all courses sorted alphabetically by course number.
============================================================================
*/
void PrintCourseList(const std::vector<Course>& courses) {
    if (courses.empty()) {
        std::cout << "No courses loaded.\n";
        return;
    }

    std::vector<Course> sortedCourses = courses;
    std::sort(sortedCourses.begin(), sortedCourses.end(),
        [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
        });

    std::cout << "\nCourse List:\n";
    for (const auto& course : sortedCourses) {
        std::cout << course.courseNumber << ", " << course.courseTitle << "\n";
    }
}

/*
============================================================================
Function: PrintCourseInfo
    Prints details for a specific course including prerequisites.
============================================================================
*/
void PrintCourseInfo(const std::vector<Course>& courses, const std::string& courseNumber) {
    for (const auto& course : courses) {
        if (course.courseNumber == courseNumber) {
            std::cout << course.courseNumber << ", " << course.courseTitle << "\n";
            if (!course.prerequisites.empty()) {
                std::cout << "Prerequisites: ";
                for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                    std::cout << course.prerequisites[i];
                    if (i < course.prerequisites.size() - 1) std::cout << ", ";
                }
                std::cout << "\n";
            }
            else {
                std::cout << "Prerequisites: None\n";
            }
            return;
        }
    }
    std::cout << "Course not found.\n";
}

/*
============================================================================
Function: main
    Program entry point. Provides menu interface.
============================================================================
*/
int main() {
    std::vector<Course> courses;
    std::string csvFileName = "CS 300 ABCU_Advising_Program_Input.csv";

    int choice = 0;
    while (choice != 9) {
        std::cout << "\nMenu:\n"
            << "  1. Load Courses\n"
            << "  2. Print Course List\n"
            << "  3. Print Course\n"
            << "  9. Exit\n"
            << "Enter choice: ";

        std::string input;
        std::getline(std::cin, input); // always read input as string

        // Validate numeric menu choice
        try {
            choice = std::stoi(input);
        }
        catch (...) {
            std::cout << "Invalid input. Please enter a number from the menu.\n";
            continue;
        }

        switch (choice) {
        case 1:
            courses = LoadCoursesFromFile(csvFileName);
            std::cout << "Courses loaded successfully.\n";
            break;
        case 2:
            PrintCourseList(courses);
            break;
        case 3: {
            std::cout << "Enter course number (e.g., CSCI100): ";
            std::string courseNum;
            std::getline(std::cin, courseNum);
            courseNum = ToUpperCase(courseNum);  // normalize input
            PrintCourseInfo(courses, courseNum);
            break;
        }
        case 9:
            std::cout << "Goodbye.\n";
            break;
        default:
            std::cout << "Not a valid option.\n";
        }
    }

    return 0;
}
