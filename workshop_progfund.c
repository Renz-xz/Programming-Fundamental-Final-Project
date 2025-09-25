#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "workshop.csv"
#define MAX 100
#define LINE_SIZE 256

// datastructure
typedef struct {
    char participantName[50];
    char workshopTitle[50];
    char workshopDate[20];
    char duration[10];
} Workshop;

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
    printf("Select: ");
}

// read csv
int load_data(Workshop list[], int *count) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 0;

    char line[LINE_SIZE];
    *count = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        sscanf(line, "%49[^,],%49[^,],%19[^,],%9[^\n]",
               list[*count].participantName,
               list[*count].workshopTitle,
               list[*count].workshopDate,
               list[*count].duration);
        (*count)++;
    }
    fclose(fp);
    return 1;
}

// save
void save_data(Workshop list[], int count) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        printf("Error saving file!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s\n",
                list[i].participantName,
                list[i].workshopTitle,
                list[i].workshopDate,
                list[i].duration);
    }
    fclose(fp);
}

// add
void add_participation(Workshop list[], int *count) {
    Workshop w;
    printf("Enter participant name: ");
    scanf(" %[^\n]", w.participantName);
    printf("Enter workshop title: ");
    scanf(" %[^\n]", w.workshopTitle);
    printf("Enter workshop date (YYYY-MM-DD): ");
    scanf(" %[^\n]", w.workshopDate);
    printf("Enter duration (hours): ");
    scanf(" %[^\n]", w.duration);

    list[*count] = w;
    (*count)++;

    save_data(list, *count);
    printf("Added successfully!\n");
}

// show
void view_all(Workshop list[], int count) {
    printf("\n--- All Participation Records ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s | %s | %s | %s\n", i + 1,
               list[i].participantName,
               list[i].workshopTitle,
               list[i].workshopDate,
               list[i].duration);
    }
    if (count == 0) printf("No records found.\n");
}

// search
void search_participation(Workshop list[], int count) {
    int choice;
    char keyword[50];
    int found = 0;

    printf("\nSearch by:\n");
    printf("1. Participant Name\n");
    printf("2. Workshop Title\n");
    printf("3. Workshop Date\n");
    printf("4. Duration\n");
    printf("Select: ");
    scanf("%d", &choice);

    printf("Enter keyword: ");
    scanf(" %[^\n]", keyword);

    for (int i = 0; i < count; i++) {
        int match = 0;
        switch (choice) {
            case 1:
                if (strstr(list[i].participantName, keyword)) match = 1;
                break;
            case 2:
                if (strstr(list[i].workshopTitle, keyword)) match = 1;
                break;
            case 3:
                if (strstr(list[i].workshopDate, keyword)) match = 1;
                break;
            case 4:
                if (strstr(list[i].duration, keyword)) match = 1;
                break;
            default:
                printf("Invalid choice!\n");
                return;
        }

        if (match) {
            printf("%s | %s | %s | %s\n",
                   list[i].participantName,
                   list[i].workshopTitle,
                   list[i].workshopDate,
                   list[i].duration);
            found = 1;
        }
    }

    if (!found) printf("No records found for '%s'\n", keyword);
}

// update
void update_participation(Workshop list[], int count) {
    char name[50];
    printf("Enter participant name to update: ");
    scanf(" %[^\n]", name);

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].participantName, name) == 0) {
            printf("\nCurrent data: %s | %s | %s | %s\n",
                   list[i].participantName,
                   list[i].workshopTitle,
                   list[i].workshopDate,
                   list[i].duration);

            int choice;
            printf("\nWhat do you want to update?\n");
            printf("1. Workshop Title\n");
            printf("2. Workshop Date\n");
            printf("3. Duration\n");
            printf("4. Cancel\n");
            printf("Select: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter new workshop title: ");
                    scanf(" %[^\n]", list[i].workshopTitle);
                    break;
                case 2:
                    printf("Enter new workshop date (YYYY-MM-DD): ");
                    scanf(" %[^\n]", list[i].workshopDate);
                    break;
                case 3:
                    printf("Enter new duration (hours): ");
                    scanf(" %[^\n]", list[i].duration);
                    break;
                case 4:
                    printf("Canceling update.\n");
                    break;
                default:
                    printf("Invalid choice!\n");
                    return;
            }

            save_data(list, count);
            printf("Record updated successfully!\n");
            return;
        }
    }
    printf("No participant found with name '%s'\n", name);
}

// delete
void delete_participation(Workshop list[], int *count) {
    char name[50];
    printf("Enter participant name to delete: ");
    scanf(" %[^\n]", name);

    for (int i = 0; i < *count; i++) {
        if (strcmp(list[i].participantName, name) == 0) {
            for (int j = i; j < *count - 1; j++) {
                list[j] = list[j + 1];
            }
            (*count)--;
            save_data(list, *count);
            printf("Deleted successfully!\n");
            return;
        }
    }
    printf("No participant found with name '%s'\n", name);
}

// unit test
// not finished
void unit_test() {
    int choice;
    printf("\n===== Unit Test Menu =====\n");
    printf("1. Run Test 1\n");
    printf("2. Run Test 2\n");
    printf("Select: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Running Unit Test 1\n");
            break;
        case 2:
            printf("Running Unit Test 2\n");
            break;
        default:
            printf("Invalid choice!\n");
    }

    printf("Returning to main menu\n");
}

// E2E test
// not finished
void end_to_end_test() {
    printf("\n===== End-to-End Test =====\n");
    printf("Running End-to-End Test\n");
    printf("Returning to main menu\n");
}


// main
int main() {
    Workshop list[MAX];
    int count = 0;
    load_data(list, &count);

    int choice;
    do {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: view_all(list, count); break;
            case 2: add_participation(list, &count); break;
            case 3: search_participation(list, count); break;
            case 4: update_participation(list, count); break;
            case 5: delete_participation(list, &count); break;
            case 6: unit_test(); break;
            case 7: end_to_end_test(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice!\n");
        }        
    } while (choice != 0);

    return 0;
}
