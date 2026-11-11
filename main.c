/*
 * Student Management System
 * Language: C
 * Type: Console-based project
 *
 * Features:
 * 1. Add student
 * 2. Display all students
 * 3. Search student by roll number
 * 4. Update student details and marks
 * 5. Delete student record
 * 6. Calculate result
 *
 * Data is stored permanently in the binary file "students.dat".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DATA_FILE "students.dat"
#define NAME_SIZE 50
#define COURSE_SIZE 40
#define SUBJECTS 5

typedef struct {
    int roll;
    char name[NAME_SIZE];
    char course[COURSE_SIZE];
    float marks[SUBJECTS];
} Student;

/* Function declarations */
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
void calculateResult(void);
int readInt(const char *message);
float readMarks(const char *message);
void readText(const char *message, char *buffer, size_t size);
int rollExists(int roll);
void printStudent(const Student *student);

/* Main function: displays the menu until the user chooses Exit. */
int main(void) {
    int choice;

    do {
        printf("\n====================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Calculate Result\n");
        printf("7. Exit\n");
        printf("------------------------------------\n");

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                calculateResult();
                break;
            case 7:
                printf("\nThank you for using the system!\n");
                break;
            default:
                printf("\nInvalid choice. Please enter 1 to 7.\n");
        }
    } while (choice != 7);

    return 0;
}

/* Reads an integer safely from the keyboard. */
int readInt(const char *message) {
    char line[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nInput ended. Exiting.\n");
            exit(EXIT_SUCCESS);
        }

        errno = 0;
        value = strtol(line, &end, 10);

        while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
            end++;
        }

        if (errno == 0 && end != line && *end == '\0' &&
            value >= 1 && value <= 2147483647L) {
            return (int)value;
        }

        printf("Please enter a valid positive whole number.\n");
    }
}

/* Reads marks between 0 and 100. */
float readMarks(const char *message) {
    char line[100];
    char *end;
    float value;

    while (1) {
        printf("%s", message);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nInput ended. Exiting.\n");
            exit(EXIT_SUCCESS);
        }

        errno = 0;
        value = strtof(line, &end);

        while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
            end++;
        }

        if (errno == 0 && end != line && *end == '\0' &&
            value >= 0.0f && value <= 100.0f) {
            return value;
        }

        printf("Marks must be a number between 0 and 100.\n");
    }
}

/* Reads a text value, including spaces. */
void readText(const char *message, char *buffer, size_t size) {
    size_t length;

    while (1) {
        printf("%s", message);

        if (fgets(buffer, (int)size, stdin) == NULL) {
            printf("\nInput ended. Exiting.\n");
            exit(EXIT_SUCCESS);
        }

        length = strlen(buffer);

        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard extra characters. */
            }
        }

        if (strlen(buffer) > 0) {
            return;
        }

        printf("This field cannot be empty.\n");
    }
}

/* Checks whether a roll number is already present. */
int rollExists(int roll) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;

    if (file == NULL) {
        return 0;
    }

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.roll == roll) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

/* Prints all details of one student. */
void printStudent(const Student *student) {
    int i;

    printf("\nRoll Number : %d\n", student->roll);
    printf("Name        : %s\n", student->name);
    printf("Course      : %s\n", student->course);

    for (i = 0; i < SUBJECTS; i++) {
        printf("Subject %d   : %.2f\n", i + 1, student->marks[i]);
    }
}

/* Adds a new student record to the data file. */
void addStudent(void) {
    FILE *file;
    Student student;
    int i;

    printf("\n--- Add Student ---\n");
    student.roll = readInt("Enter roll number: ");

    if (rollExists(student.roll)) {
        printf("This roll number already exists.\n");
        return;
    }

    readText("Enter student name: ", student.name, sizeof(student.name));
    readText("Enter course name: ", student.course, sizeof(student.course));

    for (i = 0; i < SUBJECTS; i++) {
        char message[60];
        snprintf(message, sizeof(message), "Enter marks for subject %d (0-100): ", i + 1);
        student.marks[i] = readMarks(message);
    }

    file = fopen(DATA_FILE, "ab");
    if (file == NULL) {
        perror("Unable to open data file");
        return;
    }

    if (fwrite(&student, sizeof(Student), 1, file) != 1) {
        printf("Error: student record could not be saved.\n");
    } else {
        printf("Student added successfully!\n");
    }

    fclose(file);
}

/* Displays every student saved in the file. */
void displayStudents(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int count = 0;

    printf("\n--- All Student Records ---\n");

    if (file == NULL) {
        printf("No records found. Add a student first.\n");
        return;
    }

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        printStudent(&student);
        printf("------------------------------------\n");
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No records found.\n");
    } else {
        printf("Total students: %d\n", count);
    }
}

/* Searches for a student using the roll number. */
void searchStudent(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int roll;
    int found = 0;

    printf("\n--- Search Student ---\n");
    roll = readInt("Enter roll number to search: ");

    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.roll == roll) {
            printStudent(&student);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Student with roll number %d was not found.\n", roll);
    }
}

/* Updates an existing student's name, course and marks. */
void updateStudent(void) {
    FILE *file = fopen(DATA_FILE, "rb+");
    Student student;
    int roll;
    int found = 0;
    int i;

    printf("\n--- Update Student ---\n");
    roll = readInt("Enter roll number to update: ");

    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.roll == roll) {
            printStudent(&student);
            printf("\nEnter the new details:\n");

            readText("Enter new name: ", student.name, sizeof(student.name));
            readText("Enter new course: ", student.course, sizeof(student.course));

            for (i = 0; i < SUBJECTS; i++) {
                char message[60];
                snprintf(message, sizeof(message), "Enter new marks for subject %d (0-100): ", i + 1);
                student.marks[i] = readMarks(message);
            }

            if (fseek(file, -(long)sizeof(Student), SEEK_CUR) != 0 ||
                fwrite(&student, sizeof(Student), 1, file) != 1) {
                printf("Error while updating the record.\n");
            } else {
                printf("Student record updated successfully!\n");
            }

            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Student with roll number %d was not found.\n", roll);
    }
}

/* Deletes a student by copying all other records to a temporary file. */
void deleteStudent(void) {
    FILE *source = fopen(DATA_FILE, "rb");
    FILE *temporary;
    Student student;
    int roll;
    int found = 0;

    printf("\n--- Delete Student ---\n");
    roll = readInt("Enter roll number to delete: ");

    if (source == NULL) {
        printf("No records found.\n");
        return;
    }

    temporary = fopen("temp.dat", "wb");
    if (temporary == NULL) {
        perror("Unable to create temporary file");
        fclose(source);
        return;
    }

    while (fread(&student, sizeof(Student), 1, source) == 1) {
        if (student.roll == roll) {
            found = 1;
        } else {
            fwrite(&student, sizeof(Student), 1, temporary);
        }
    }

    fclose(source);
    fclose(temporary);

    if (!found) {
        remove("temp.dat");
        printf("Student with roll number %d was not found.\n", roll);
        return;
    }

    if (remove(DATA_FILE) != 0 || rename("temp.dat", DATA_FILE) != 0) {
        printf("Error while replacing the data file.\n");
        return;
    }

    printf("Student record deleted successfully!\n");
}

/* Calculates total marks, percentage, grade and pass/fail result. */
void calculateResult(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int roll;
    int found = 0;
    int i;
    int passed;
    float total;
    float percentage;

    printf("\n--- Calculate Result ---\n");
    roll = readInt("Enter roll number: ");

    if (file == NULL) {
        printf("No records found.\n");
        return;
    }

    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.roll == roll) {
            total = 0.0f;
            passed = 1;

            for (i = 0; i < SUBJECTS; i++) {
                total += student.marks[i];

                if (student.marks[i] < 33.0f) {
                    passed = 0;
                }
            }

            percentage = total / SUBJECTS;

            printf("\nStudent Name : %s\n", student.name);
            printf("Total Marks  : %.2f / 500\n", total);
            printf("Percentage   : %.2f%%\n", percentage);

            if (!passed) {
                printf("Result       : FAIL (one or more subjects below 33)\n");
            } else if (percentage >= 90.0f) {
                printf("Result       : PASS - Grade A+\n");
            } else if (percentage >= 75.0f) {
                printf("Result       : PASS - Grade A\n");
            } else if (percentage >= 60.0f) {
                printf("Result       : PASS - Grade B\n");
            } else if (percentage >= 40.0f) {
                printf("Result       : PASS - Grade C\n");
            } else {
                printf("Result       : FAIL\n");
            }

            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Student with roll number %d was not found.\n", roll);
    }
}
