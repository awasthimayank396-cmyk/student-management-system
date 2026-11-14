#include <stdio.h>
#include <stdlib.h>

struct Student {
    int roll;
    char name[50];
    char course[30];
    float marks;
};

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main(void) {
    int choice;

    do {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            return 1;
        }

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
                printf("Thank you for using the system!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void addStudent(void) {
    struct Student s, temp;
    FILE *fp;
    int found = 0;

    fp = fopen("students.dat", "ab+");
    if (fp == NULL) {
        printf("File opening error!\n");
        return;
    }

    printf("Enter Roll Number: ");
    if (scanf("%d", &s.roll) != 1) {
        printf("Invalid roll number!\n");
        fclose(fp);
        return;
    }

    rewind(fp);
    while (fread(&temp, sizeof(temp), 1, fp) == 1) {
        if (temp.roll == s.roll) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Roll number already exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter Name: ");
    scanf(" %49[^\n]", s.name);

    printf("Enter Course: ");
    scanf(" %29[^\n]", s.course);

    printf("Enter Marks (0-100): ");
    if (scanf("%f", &s.marks) != 1 || s.marks < 0 || s.marks > 100) {
        printf("Invalid marks! Enter a value between 0 and 100.\n");
        fclose(fp);
        return;
    }

    fseek(fp, 0, SEEK_END);
    if (fwrite(&s, sizeof(s), 1, fp) == 1) {
        printf("Student added successfully!\n");
    } else {
        printf("Could not save student record.\n");
    }

    fclose(fp);
}

void displayStudents(void) {
    struct Student s;
    FILE *fp;
    int count = 0;

    fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No student records found!\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("\nRoll Number: %d\n", s.roll);
        printf("Name: %s\n", s.name);
        printf("Course: %s\n", s.course);
        printf("Marks: %.2f\n", s.marks);
        count++;
    }

    if (count == 0) {
        printf("No student records found!\n");
    }

    fclose(fp);
}

void searchStudent(void) {
    struct Student s;
    FILE *fp;
    int roll, found = 0;

    fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number!\n");
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\nStudent Found!\n");
            printf("Roll Number: %d\n", s.roll);
            printf("Name: %s\n", s.name);
            printf("Course: %s\n", s.course);
            printf("Marks: %.2f\n", s.marks);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found!\n");
    }

    fclose(fp);
}

void updateStudent(void) {
    struct Student s;
    FILE *fp;
    int roll, found = 0;

    fp = fopen("students.dat", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to update: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number!\n");
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("Enter New Name: ");
            scanf(" %49[^\n]", s.name);

            printf("Enter New Course: ");
            scanf(" %29[^\n]", s.course);

            printf("Enter New Marks (0-100): ");
            if (scanf("%f", &s.marks) != 1 || s.marks < 0 || s.marks > 100) {
                printf("Invalid marks!\n");
                fclose(fp);
                return;
            }

            fseek(fp, -(long)sizeof(s), SEEK_CUR);
            if (fwrite(&s, sizeof(s), 1, fp) == 1) {
                printf("Student updated successfully!\n");
            } else {
                printf("Could not update student record.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found!\n");
    }

    fclose(fp);
}

void deleteStudent(void) {
    struct Student s;
    FILE *fp, *temp;
    int roll, found = 0;

    fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Temporary file error!\n");
        fclose(fp);
        return;
    }

    printf("Enter Roll Number to delete: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number!\n");
        fclose(fp);
        fclose(temp);
        remove("temp.dat");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;
        } else {
            fwrite(&s, sizeof(s), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        if (remove("students.dat") == 0) {
            if (rename("temp.dat", "students.dat") == 0) {
                printf("Student deleted successfully!\n");
            } else {
                printf("Could not rename temporary file. Data remains in temp.dat.\n");
            }
        } else {
            printf("Could not replace the original file. Data remains in temp.dat.\n");
        }
    } else {
        remove("temp.dat");
        printf("Student not found!\n");
    }
}
