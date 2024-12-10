#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

// Helper Enum for Fields of Study
enum class FieldOfStudy {
    FoodTechnology,
    ComputerScience,
    Engineering,
    Business
};

// Student Class
class Student {
private:
    string name;
    string id;
    string email;

public:
    // Constructor
    Student(string name, string id, string email)
        : name(name), id(id), email(email) {}

    // Accessors
    string getName() const { return name; }
    string getId() const { return id; }
    string getEmail() const { return email; }
};

// Faculty Class
class Faculty {
private:
    string name;
    FieldOfStudy field;
    vector<Student> enrolledStudents;
    vector<Student> graduatedStudents;

public:
    // Constructor
    Faculty(string name, FieldOfStudy field) : name(name), field(field) {}

    // Accessors
    string getName() const { return name; }
    FieldOfStudy getField() const { return field; }

    // Methods to manage students
    void enrollStudent(const Student& student) {
        enrolledStudents.push_back(student);
    }

    void graduateStudent(const string& studentId) {
        auto it = find_if(enrolledStudents.begin(), enrolledStudents.end(),
                          [&studentId](const Student& s) { return s.getId() == studentId; });
        if (it != enrolledStudents.end()) {
            graduatedStudents.push_back(*it);
            enrolledStudents.erase(it);
        } else {
            cout << "Student not found in enrolled list.\n";
        }
    }

    void displayEnrolledStudents() const {
        cout << "Enrolled Students in " << name << ":\n";
        for (const auto& student : enrolledStudents) {
            cout << " - " << student.getName() << " (ID: " << student.getId() << ")\n";
        }
    }

    void displayGraduatedStudents() const {
        cout << "Graduated Students in " << name << ":\n";
        for (const auto& student : graduatedStudents) {
            cout << " - " << student.getName() << " (ID: " << student.getId() << ")\n";
        }
    }

    bool hasStudent(const string& studentId) const {
        auto isFound = [&studentId](const Student& s) { return s.getId() == studentId; };
        return any_of(enrolledStudents.begin(), enrolledStudents.end(), isFound) ||
               any_of(graduatedStudents.begin(), graduatedStudents.end(), isFound);
    }
};

// University Class
class University {
private:
    vector<Faculty> faculties;

public:
    // Methods to manage faculties
    void createFaculty(const string& name, FieldOfStudy field) {
        faculties.emplace_back(name, field);
    }

    void displayFaculties() const {
        cout << "University Faculties:\n";
        for (const auto& faculty : faculties) {
            cout << " - " << faculty.getName() << "\n";
        }
    }

    void displayFacultiesByField(FieldOfStudy field) const {
        cout << "Faculties in Field: ";
        for (const auto& faculty : faculties) {
            if (faculty.getField() == field) {
                cout << faculty.getName() << "\n";
            }
        }
    }

    Faculty* findFaculty(const string& name) {
        for (auto& faculty : faculties) {
            if (faculty.getName() == name) {
                return &faculty;
            }
        }
        return nullptr;
    }

    string findStudentFaculty(const string& studentId) const {
        for (const auto& faculty : faculties) {
            if (faculty.hasStudent(studentId)) {
                return faculty.getName();
            }
        }
        return "Student not found in any faculty.";
    }
};

// Main Program
int main() {
    University tum;

    // Create faculties
    tum.createFaculty("Food Technology", FieldOfStudy::FoodTechnology);
    tum.createFaculty("Computer Science", FieldOfStudy::ComputerScience);

    // Add students to faculties
    Faculty* csFaculty = tum.findFaculty("Computer Science");
    Faculty* ftFaculty = tum.findFaculty("Food Technology");

    if (csFaculty) {
        csFaculty->enrollStudent(Student("Alice", "CS001", "alice@tum.md"));
        csFaculty->enrollStudent(Student("Bob", "CS002", "bob@tum.md"));
    }

    if (ftFaculty) {
        ftFaculty->enrollStudent(Student("Charlie", "FT001", "charlie@tum.md"));
    }

    // Display enrolled students
    cout << "\nEnrolled Students:\n";
    if (csFaculty) csFaculty->displayEnrolledStudents();
    if (ftFaculty) ftFaculty->displayEnrolledStudents();

    // Graduate a student
    if (csFaculty) {
        csFaculty->graduateStudent("CS001");
    }

    // Display graduates
    cout << "\nGraduated Students:\n";
    if (csFaculty) csFaculty->displayGraduatedStudents();

    // Find student's faculty
    cout << "\nFaculty of Student CS002: " << tum.findStudentFaculty("CS002") << "\n";

    // Display all faculties
    cout << "\nAll Faculties:\n";
    tum.displayFaculties();

    return 0;
}