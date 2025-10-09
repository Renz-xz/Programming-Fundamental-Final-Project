#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
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

// global
int is_test_mode = 0;

// menu
void display_menu() {
    printf("\n===== Workshop Management =====\n");
    printf("1. View all participation\n");
    printf("2. Add participation\n");
    printf("3. Search participation\n");
    printf("4. Update participation\n");
    printf("5. Permanent Delete participation\n");
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
    if (is_test_mode) {
        // E2E/Unit Test Mode → ไม่เซฟไฟล์จริง
        return;
    }

    // ✅ โหมดปกติ → เซฟไฟล์
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        printf("Cannot open file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s\n",
                list[i].firstName,
                list[i].lastName,
                list[i].workshopTitle,
                list[i].workshopDate,
                list[i].duration);
    }
    fclose(fp);
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
void update_participation(Workshop list[], int *count) {
    if (count == 0) {
        printf("No data to update.\n");
        return;
    }

    char searchName[61];
    printf("Enter participant full name to update: ");
    scanf(" %[^\n]", searchName);

    int matchedIndexes[MAX];
    int matchedCount = 0;

    for (int i = 0; i < *count; i++) {
        char fullName[61];
        snprintf(fullName, sizeof(fullName), "%s %s", list[i].firstName, list[i].lastName);
        if (strcmp(fullName, searchName) == 0) {
            matchedIndexes[matchedCount++] = i;
        }
    }

    if (matchedCount == 0) {
        printf("Participant not found.\n");
        return;
    }

    int targetIndex;
    if (matchedCount == 1) {
        targetIndex = matchedIndexes[0];
    } else {
        printf("Multiple participants found with the same name:\n");
        for (int i = 0; i < matchedCount; i++) {
            int idx = matchedIndexes[i];
            printf("%d. %s %s | Title: %s | Date: %s | Duration: %s hrs\n",
                   i + 1,
                   list[idx].firstName,
                   list[idx].lastName,
                   list[idx].workshopTitle,
                   list[idx].workshopDate,
                   list[idx].duration);
        }

        int choice;
        while (1) {
            printf("Select the number of the participant to update (1-%d): ", matchedCount);
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Please enter a number.\n");
                while (getchar() != '\n');
                continue;
            }
            if (choice < 1 || choice > matchedCount) {
                printf("Invalid choice. Try again.\n");
            } else {
                targetIndex = matchedIndexes[choice - 1];
                break;
            }
        }
    }

    int updateChoice;
    printf("What do you want to update?\n");
    printf("1. Workshop Title\n");
    printf("2. Date\n");
    printf("3. Duration\n");
    printf("4. Cancel\n");
    printf("Enter choice: ");

    if (scanf("%d", &updateChoice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    switch (updateChoice) {
        case 1: {
            printf("Enter new Workshop Title: ");
            scanf(" %[^\n]", list[targetIndex].workshopTitle);
            printf("Workshop Title updated successfully!\n");
            break;
        }
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
                    strcpy(list[targetIndex].workshopDate, date);
                    printf("Date updated successfully!\n");
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
                    printf("Duration must be between 1 to 10.\n");
                } else {
                    sprintf(list[targetIndex].duration, "%d", duration);
                    printf("Duration updated successfully!\n");
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
}

// delete
void delete_participation(Workshop list[], int *count) {
    if (*count == 0) {
        printf("No participation records to delete.\n");
        return;
    }

    char firstName[50], lastName[50];
    printf("Enter first name to delete: ");
    fgets(firstName, sizeof(firstName), stdin);
    firstName[strcspn(firstName, "\n")] = 0;

    printf("Enter last name to delete: ");
    fgets(lastName, sizeof(lastName), stdin);
    lastName[strcspn(lastName, "\n")] = 0;

    int matches[50];
    int match_count = 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp(list[i].firstName, firstName) == 0 &&
            strcmp(list[i].lastName, lastName) == 0) {
            matches[match_count++] = i;
        }
    }

    if (match_count == 0) {
        printf("No participation found for %s %s.\n", firstName, lastName);
        return;
    }

    int target_index = -1;
    if (match_count == 1) {
        target_index = matches[0];
    } else {
        printf("Multiple participations found for %s %s:\n", firstName, lastName);
        for (int i = 0; i < match_count; i++) {
            int idx = matches[i];
            printf("%d. %s %s - Title: %s | Date: %s | Duration: %s\n",
                   i + 1,
                   list[idx].firstName,
                   list[idx].lastName,
                   list[idx].workshopTitle,
                   list[idx].workshopDate,
                   list[idx].duration);
        }

        int choice = 0;
        while (1) {
            printf("Select which one to delete (1-%d): ", match_count);
            if (scanf("%d", &choice) != 1 || choice < 1 || choice > match_count) {
                printf("Invalid choice. Try again.\n");
                while (getchar() != '\n');
            } else {
                while (getchar() != '\n');
                target_index = matches[choice - 1];
                break;
            }
        }
    }

    // Show confirmation
    printf("\nYou are about to permanent delete:\n");
    printf("%s %s - Title: %s | Date: %s | Duration: %s\n",
           list[target_index].firstName,
           list[target_index].lastName,
           list[target_index].workshopTitle,
           list[target_index].workshopDate,
           list[target_index].duration);

    char confirm;
    printf("Are you sure you want to permanent delete this participation? (y/n): ");
    scanf(" %c", &confirm);
    while (getchar() != '\n');

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = target_index; i < *count - 1; i++) {
        list[i] = list[i + 1];
    }
    (*count)--;

    printf("Participation for %s %s deleted successfully.\n", firstName, lastName);
}



// prototype for unit test and e2e test
void run_unit_test_1(void);
void run_unit_test_2(void);
void run_e2e_tests(void);

// unit test
void unit_tests() {
    is_test_mode = 0;
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
            run_unit_test_1(); 
            break;
        case 2:
            run_unit_test_2();
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
void e2e_tests() {
    is_test_mode = 0;
    printf("\n===== End-to-End Test =====\n");
    run_e2e_tests();
    printf("Returning to main menu\n");
}


// main
int main() {
    Workshop list[MAX];
    int count = 0;
    int choice;
    load_from_file(list, &count);

    while (1) {
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
                update_participation(list, &count);
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
