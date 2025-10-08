#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#define _CRTDBG_MAP_ALLOC
#define FILE_NAME "workshop.csv"
#define MAX 100
#define LINE_SIZE 256

// datastructure
typedef struct {
    char firstName[30];
    char lastName[30];
    char workshopTitle[50];
    char workshopDate[20];
    char duration[10];
} Workshop;

int is_test_mode = 0;

// menu
void display_menu() {
    printf("\n===== Workshop Management =====\n");
    printf("1. View all participation\n");
    printf("2. Add participation\n");
    printf("3. Search participation\n");
    printf("4. Update participation\n");
    printf("5. Delete participation\n");
    printf("6. Run Unit Tests\n");
    printf("7. Run End-to-End Test\n");
    printf("0. Exit\n");
    printf("==================================\n");
}

// read csv
int load_from_file(Workshop list[], int *count) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 0;

    char line[LINE_SIZE];
    *count = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
                list[*count].firstName,
                list[*count].lastName,
                list[*count].workshopTitle,
                list[*count].workshopDate,
                list[*count].duration);
        (*count)++;
    }
    fclose(fp);
    return 1;
}

// save
void save_to_file(Workshop list[], int count) {
    if (is_test_mode==0) {
        return;  // ไม่เซฟไฟล์จริง
    }

    else {
        remove(FILE_NAME);FILE *fp = fopen("workshop.csv", "w");
        if (!fp) {
            printf("Cannot open file for writing.\n");
            return;
        }
    
        for (int i = 0; i < count; i++) {
            fprintf(fp, "%s,%s,%s,%s\n",
                    list[i].firstName,
                    list[i].lastName,
                    list[i].workshopTitle,
                    list[i].workshopDate,
                    list[i].duration);
        }
        fclose(fp);
    }
}

// validate name
int is_valid_name(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) {
            return 0;
        }
    }
    return 1;
}

// add
void add_participation(Workshop list[], int *count) {
    if (*count >= MAX) {
        printf("Cannot add more participants. List is full.\n");
        return;
    }

    while (1) {
        printf("Enter First Name: ");
        scanf("%29s", list[*count].firstName);
        if (!is_valid_name(list[*count].firstName)) {
            printf("Invalid first name. Letters only.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }

    while (1) {
        printf("Enter Last Name: ");
        scanf("%29s", list[*count].lastName);
        if (!is_valid_name(list[*count].lastName)) {
            printf("Invalid last name. Letters only.\n");
            while (getchar() != '\n');
        } else {
            break;
        }
    }

    printf("Enter Workshop Title: ");
    while (getchar() != '\n');
    fgets(list[*count].workshopTitle, sizeof(list[*count].workshopTitle), stdin);
    list[*count].workshopTitle[strcspn(list[*count].workshopTitle, "\n")] = '\0';

    // date check
    char date[20];
    int day, month, year;
    while (1) {
        printf("Enter Workshop Date (yyyy/mm/dd): ");
        scanf("%19s", date);
        if (sscanf(date, "%d/%d/%d", &year, &month, &day) == 3 &&
            day >= 1 && day <= 31 && month >= 1 && month <= 12 &&
            year == 2025) {
            strcpy(list[*count].workshopDate, date);
            break;
        } else {
            printf("Invalid date format or values. Try again.\n");
        }
    }

    // duration check
    int duration;
    while (1) {
        printf("Enter Duration (in hours): ");
        if (scanf("%d", &duration) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (duration <= 0 || duration > 10) {
            printf("Duration must be between 1 to 10.\n");
        } else {
            sprintf(list[*count].duration, "%d", duration);
            break;
        }
    }

    (*count)++;
    printf("Participant added successfully!\n");
}

// show
void view_participations(Workshop list[], int count) {
    if (count == 0) {
        printf("No participants found.\n");
        return;
    }

    printf("---------------------------------------------------------------------------------\n");
    printf("| %-25s | %-20s | %-12s | %-8s |\n",
           "Participant Name", "Workshop Title", "Date", "Duration");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        char fullName[61];
        snprintf(fullName, sizeof(fullName), "%s %s", list[i].firstName, list[i].lastName);

        printf("| %-25s | %-20s | %-12s | %-8s |\n",
               fullName,
               list[i].workshopTitle,
               list[i].workshopDate,
               list[i].duration);
    }

    printf("---------------------------------------------------------------------------------\n");
}

// search
void search_participation(Workshop list[], int count) {
    if (count == 0) {
        printf("No data to search.\n");
        return;
    }
    int choice;
    char keyword[50];
    printf("Search by:\n");
    printf("1. First/Last/Full Name\n");
    printf("2. Workshop Title\n");
    printf("3. Cancel\n");
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();
    if (choice == 3) {
        printf("Search cancelled.\n");
        return;
    }
    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n");
        return;
    }
    printf("Enter keyword: ");
    scanf(" %[^\n]", keyword);
    printf("---------------------------------------------------------------------------------\n");
    printf("| %-25s | %-20s | %-12s | %-8s |\n",
           "Participant Name", "Workshop Title", "Date", "Duration");
    printf("---------------------------------------------------------------------------------\n");
    int result_found = 0;
    for (int i = 0; i < count; i++) {
        char fullName[61];
        snprintf(fullName, sizeof(fullName), "%s %s", list[i].firstName, list[i].lastName);
        int found = 0;
        switch (choice) {
            case 1:
                if (strstr(list[i].firstName, keyword) ||
                    strstr(list[i].lastName, keyword) ||
                    strstr(fullName, keyword)) {
                    found = 1;
                }
                break;
            case 2:
                if (strstr(list[i].workshopTitle, keyword)) {
                    found = 1;
                }
                break;
        }
        if (found) {
            result_found = 1;
            printf("| %-25s | %-20s | %-12s | %-8s |\n",
                   fullName,
                   list[i].workshopTitle,
                   list[i].workshopDate,
                   list[i].duration);
        }
    }
    if (!result_found) {
        printf("| %-71s |\n", "No matching records found.");
    }
    printf("---------------------------------------------------------------------------------\n");
}

// update
void update_participation(Workshop list[], int count) {
    if (count == 0) {
        printf("No data to update.\n");
        return;
    }
    char searchName[61];
    printf("Enter participant full name to update: ");
    scanf(" %[^\n]", searchName);
    for (int i = 0; i < count; i++) {
        char fullName[61];
        snprintf(fullName, sizeof(fullName), "%s %s", list[i].firstName, list[i].lastName);
        if (strcmp(fullName, searchName) == 0) {
            int choice;
            printf("What do you want to update?\n");
            printf("1. Workshop Title\n");
            printf("2. Date\n");
            printf("3. Duration\n");
            printf("4. Cancel\n");
            printf("Enter choice: ");
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                return;
            }
            getchar();
            switch (choice) {
                case 1:
                    printf("Enter new Workshop Title: ");
                    scanf(" %[^\n]", list[i].workshopTitle);
                    break;
                case 2: {
                    char date[20];
                    int day, month, year;
                    while (1) {
                        printf("Enter new Date (yyyy/mm/dd): ");
                        scanf("%19s", date);
                        if (sscanf(date, "%d/%d/%d", &year, &month, &day) == 3 &&
                            day >= 1 && day <= 31 &&
                            month >= 1 && month <= 12 &&
                            year == 2025) {
                            strcpy(list[i].workshopDate, date);
                            break;
                        } else {
                            printf("Invalid date format or values. Try again.\n");
                        }
                    }
                    break;
                }
                case 3: {
                    int duration;
                    while (1) {
                        printf("Enter new Duration (in hours): ");
                        if (scanf("%d", &duration) != 1) {
                            printf("Invalid input. Please enter a number.\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if (duration <= 0 || duration > 10) {
                            printf("Duration must between 1 to 10.\n");
                        } else {
                            sprintf(list[i].duration, "%d", duration);
                            break;
                        }
                    }
                    break;
                }
                case 4:
                    printf("Update cancelled.\n");
                    return;
                default:
                    printf("Invalid choice.\n");
                    return;
            }
            printf("Participant updated successfully!\n");
            return;
        }
    }
    printf("Participant not found.\n");
}

// delete
void delete_participation(Workshop list[], int *count) {
    if (*count == 0) {
        printf("No data to delete.\n");
        return;
    }

    char searchName[61];
    printf("Enter full name to delete: ");
    scanf(" %[^\n]", searchName);

    for (int i = 0; i < *count; i++) {
        char fullName[61];
        snprintf(fullName, sizeof(fullName), "%s %s", list[i].firstName, list[i].lastName);

        if (strcmp(fullName, searchName) == 0) {
            char confirm;
            printf("Are you sure you want to delete \"%s\"? (y/n): ", fullName);
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < *count - 1; j++) {
                    list[j] = list[j + 1];
                }
                (*count)--;
                printf("Participant deleted successfully!\n");
            } else {
                printf("Delete canceled.\n");
            }
            return;
        }
    }

    printf("Participant not found.\n");
}

// ประกาศ prototype
void run_unit_test_1(void);
void run_unit_test_2(void);
void run_e2e_tests(void);

void unit_tests() {
    int choice;
    printf("\n===== Unit Test Menu =====\n");
    printf("1. Run Test 1 (add_participation)\n");
    printf("2. Run Test 2 (update_participation)\n");
    printf("3. Cancel\n");
    printf("Select: ");
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); 
        return;
    }
    while (getchar() != '\n');

    switch (choice) {
        case 1:
            run_unit_test_1();  // เรียก Unit Test 1
            break;
        case 2:
            run_unit_test_2();  // เรียก Unit Test 2
            break;
        case 3:
            printf("Unit test canceled\n");
            break;
        default:
            printf("Invalid choice!\n");
    }

    printf("Returning to main menu\n");
}


// E2E test
// not finished
void e2e_tests() {
    printf("\n===== End-to-End Test =====\n");
    run_e2e_tests();  // เรียก E2E Test
    printf("Returning to main menu\n");
}


// main
int main() {
    Workshop list[MAX];
    int count = 0;
    int choice;

    load_from_file(list, &count);

    while (1) {
        is_test_mode = 0;
        display_menu();
        printf("Select: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        switch (choice) {
            case 1:
                view_participations(list, count);
                break;
            case 2:
                add_participation(list, &count);
                save_to_file(list, count);
                break;
            case 3:
                search_participation(list, count);
                break;
            case 4:
                update_participation(list, count);
                save_to_file(list, count);
                break;
            case 5:
                delete_participation(list, &count);
                save_to_file(list, count);
                break;
            case 6:
                unit_tests();
                break;
            case 7:
                e2e_tests();
                break;
            case 0:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
