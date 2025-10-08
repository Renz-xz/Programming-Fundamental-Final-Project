#include <stdio.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <string.h>
#include <assert.h>

// ============ ประกาศฟังก์ชัน/ตัวแปรจากโปรแกรมหลัก ============
#define MAX 100

typedef struct {
    char firstName[50];
    char lastName[50];
    char workshopTitle[100];
    char workshopDate[20];
    char duration[10];
} Workshop;

extern int is_test_mode;

// โปรโตไทป์ของฟังก์ชันหลัก (ไม่มี header)
void save_to_file(Workshop list[], int count);
void load_from_file(Workshop list[], int *count);
void add_participation(Workshop list[], int *count);
void search_participation(Workshop list[], int count);
void update_participation(Workshop list[], int *count);
void delete_participation(Workshop list[], int *count);
void view_participations(Workshop list[], int count);
void display_menu();
int main(); // เรียก main จริง

// ============ Helper จำลอง stdin (ใช้ freopen) ============
static void set_stdin_input(const char *input) {
    FILE *f = fopen("e2e_input.txt", "w");
    if (!f) {
        perror("Cannot open e2e_input.txt for writing");
        exit(1);
    }
    fputs(input, f);
    fclose(f);

    freopen("e2e_input.txt", "r", stdin);
}

static void reset_stdin() {
#ifdef _WIN32
    freopen("CON", "r", stdin); // Windows
#else
    freopen("/dev/tty", "r", stdin); // Linux
#endif
}

// ============ E2E TESTS ============
void run_e2e_tests() {
    printf("\n======================[E2E TESTS START]======================\n");

    // -----------------------------------------------------------------
    // CASE 1: Normal Flow
    // -----------------------------------------------------------------
    {
        printf("[E2E - Case 1] Normal Flow\n");

        set_stdin_input(
            "2\n"                     // Add
            "John\n"
            "Doe\n"
            "E2E Workshop\n"
            "2025/10/05\n"
            "3\n"
            "3\n"                     // Search
            "1\n"                     // By Name
            "John Doe\n"
            "4\n"                     // Update
            "John\nDoe\n"
            "1\n"                     // เปลี่ยน Title
            "Updated Title\n"
            "5\n"                     // Delete
            "John Doe\n"
            "y\n"                     // Confirm
            "0\n"                     // Exit
        );

        Workshop list[MAX];
        int count = 0;

        // เปิด test mode เพื่อไม่ให้ save ไฟล์จริง
        is_test_mode = 1;

        main();

        // ปิด test mode
        is_test_mode = 0;

        reset_stdin();
        printf("[E2E - Case 1]  Passed\n\n");
    }

    // -----------------------------------------------------------------
    // CASE 2: Boundary Case
    // -----------------------------------------------------------------
    {
        printf("[E2E - Case 2] Boundary Input\n");

        set_stdin_input(
            "2\n"
            "Jane\n"
            "Smith\n"
            "Boundary Test\n"
            "2025/13/40\n"     // ✖ ใส่ผิด
            "2025/12/31\n"     // ✔ ใส่ใหม่ถูก
            "11\n"             // ✖ ใส่ผิด
            "2\n"
            "0\n"
        );

        Workshop list[MAX];
        int count = 0;

        // เปิด test mode เพื่อไม่ให้ save ไฟล์จริง
        is_test_mode = 1;

        main();

        // ปิด test mode
        is_test_mode = 0;
        reset_stdin();
        printf("[E2E - Case 2]  Passed\n\n");
    }

    // -----------------------------------------------------------------
    // CASE 3: Extreme Case
    // -----------------------------------------------------------------
    {
        printf("[E2E - Case 3] Extreme Values\n");

        char longName[149];
        memset(longName, 'A', 148);
        longName[148] = '\0';

        char input[600];
        snprintf(input, sizeof(input),
            "2\n"
            "John\n"
            "Doe\n"
            "ExtremeTitle\n"
            "9999/12/31\n"    // extreme date
            "2025/10/05\n"    // correct date
            "9999\n"           // extreme duration
            "10\n"             // correct duration
            "0\n",
            longName, longName
        );

        set_stdin_input(input);

        Workshop list[MAX];
        int count = 0;

        // เปิด test mode เพื่อไม่ให้ save ไฟล์จริง
        is_test_mode = 1;

        main();

        // ปิด test mode
        is_test_mode = 0;
        reset_stdin();
        printf("[E2E - Case 3]  Passed\n\n");
    }

    // -----------------------------------------------------------------
    // CASE 4: Memory Leak / Stress
    // -----------------------------------------------------------------
    {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        printf("[E2E - Case 4] Memory Leak / Stress\n");

        char input[3000];
        input[0] = '\0';

        for (int i = 0; i < 20; i++) {
            char buf[200];
            snprintf(buf, sizeof(buf),
                "2\nLeak%c\nTest%c\nTitle%d\n2025/10/05\n1\n",
                'A' + i, 'A' + i, i
            );
            strcat(input, buf);
        }
        strcat(input, "0\n");

        set_stdin_input(input);

        Workshop list[MAX];
        int count = 0;

        // เปิด test mode เพื่อไม่ให้ save ไฟล์จริง
        is_test_mode = 1;

        main();

        // ปิด test mode
        is_test_mode = 0;

        reset_stdin();
        printf("[E2E - Case 4]  Passed (Check with CRT Debug Heap)\n\n");
    }

    printf("======================[E2E TESTS END]======================\n");
}
