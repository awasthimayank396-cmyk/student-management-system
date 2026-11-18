
#include <stdio.h>

struct Student {
    int roll;
    char name[50];
    float marks;
};

int main() {
    struct Student s[50];
    int choice, count = 0, i, roll, found;

    do {
        printf("\n--- STUDENT MANAGEMENT SYSTEM ---\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                if (count >= 50) {
                    printf("Record Full!\n");
                    break;
                }

                printf("Enter Roll Number: ");
                scanf("%d", &roll);

                found = 0;
                for (i = 0; i < count; i++) {
                    if (s[i].roll == roll) {
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    printf("Roll Number Already Exists!\n");
                    break;
                }

                s[count].roll = roll;

                printf("Enter Name: ");
                scanf(" %49[^\n]", s[count].name);

                printf("Enter Marks: ");
                scanf("%f", &s[count].marks);

                count++;
                printf("Student Added Successfully!\n");
                break;

            case 2:
                if (count == 0) {
                    printf("No Records Found!\n");
                }

                for (i = 0; i < count; i++) {
                    printf("\nRoll: %d\n", s[i].roll);
                    printf("Name: %s\n", s[i].name);
                    printf("Marks: %.2f\n", s[i].marks);
                }
                break;

            case 3:
                printf("Enter Roll Number: ");
                scanf("%d", &roll);

                found = 0;

                for (i = 0; i < count; i++) {
                    if (s[i].roll == roll) {
                        printf("Student Found!\n");
                        printf("Name: %s\n", s[i].name);
                        printf("Marks: %.2f\n", s[i].marks);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("Student Not Found!\n");
                }
                break;

            case 4:
                printf("Thank You!\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while (choice != 4);

    return 0;
}
