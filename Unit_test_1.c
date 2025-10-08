#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// -------------------------
// struct + MAX (เหมือนไฟล์หลัก)
// -------------------------
#define MAX 100
typedef struct {
    char firstName[30];
    char lastName[30];
    char workshopTitle[50];
    char workshopDate[20];
    char duration[10];
} Workshop;

// -------------------------
// Prototype ของฟังก์ชันที่ทดสอบ
// -------------------------
void add_participation(Workshop list[], int *count);

// -------------------------
// Helper: จำลอง stdin
// -------------------------
static void set_stdin_input(const char *input) {
    FILE *f = fopen("test_input.txt", "w");
    fputs(input, f);
    fclose(f);
    freopen("test_input.txt", "r", stdin);
}

static void reset_stdin() {
#ifdef _WIN32
    freopen("CON", "r", stdin); // สำหรับ Windows
#else
    freopen("/dev/tty", "r", stdin);
#endif
}

// =====================
// Test 1: Normal
// =====================
static void test_add_normal() {
    Workshop list[MAX];
    int count = 0;
    printf("======================[Test 1] Normal Case======================\n");

    set_stdin_input("John\nDoe\nC Programming\n2025/10/01\n3\n");
    add_participation(list, &count);

    assert(count == 1);
    assert(strcmp(list[0].firstName, "John") == 0);
    assert(strcmp(list[0].lastName, "Doe") == 0);
    assert(strcmp(list[0].workshopTitle, "C Programming") == 0);
    assert(strcmp(list[0].workshopDate, "2025/10/01") == 0);
    assert(strcmp(list[0].duration, "3") == 0);

    printf("======================Normal Case Passed======================\n");
    reset_stdin();
}

// =====================
// Test 2: Boundary
// =====================
static void test_add_boundary() {
    Workshop list[MAX];
    int count = 0;

    printf("======================[Test 2] Boundary Case======================\n");

    // input flow: full name -> เลือก 2 (Date) -> ใส่ผิดก่อน -> ใส่ใหม่ถูก
    char input[200];
    snprintf(input, sizeof(input),
        "Jane\nSmith\nBoundary_Test\n2025/13/32\n2025/12/31\n0\n11\n2\n"
    );

    set_stdin_input(input);

    add_participation(list, &count);

    assert(strcmp(list[0].workshopDate, "2025/12/31") == 0);
    assert(strcmp(list[0].duration, "2") == 0);

    printf("======================Boundary Case Passed======================\n");
    reset_stdin();
}


// =====================
// Test 3: Exterme
// =====================
static void test_add_extreme() {
    Workshop list[MAX];
    int count = 0;
    printf("======================[Test 1] Extreme Case======================\n");

    set_stdin_input("123John\nJohn\nSmith!\nSmith\nC Programming\nabcd/99999/99999\n\n2025/10/01\n99999\n-99999\n3\n");
    add_participation(list, &count);

    assert(count == 1);
    assert(strcmp(list[0].firstName, "John") == 0);
    assert(strcmp(list[0].lastName, "Smith") == 0);
    assert(strcmp(list[0].workshopTitle, "C Programming") == 0);
    assert(strcmp(list[0].workshopDate, "2025/10/01") == 0);
    assert(strcmp(list[0].duration, "3") == 0);

    printf("======================Extreme Case Passed======================\n");
    reset_stdin();
}

// =====================
// Test 4: Memory Leak (CRT Debug Heap)
// =====================
static void test_add_memory_leak() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Workshop list[MAX];
    int count = 0;

    strcpy(list[0].firstName, "Leak");
    strcpy(list[0].lastName, "Test");
    strcpy(list[0].workshopTitle, "Old Leak");
    strcpy(list[0].workshopDate, "2025/01/01");
    strcpy(list[0].duration, "5");

    printf("======================[Test 4] Memory Leak======================\n");

    set_stdin_input(
        "Leak\n"
        "Test\n"
        "New Leak Title\n"
        "2025/10/05\n"
        "2\n"              
    );

    add_participation(list, &count);

    printf("======================Memory Leak Test Passed (Check with CRT Debug Heap)======================\n");
    reset_stdin();
}


// =====================
// ฟังก์ชันเรียกทั้งหมด (เรียกจาก unit_tests())
// =====================
void run_unit_test_1(void) {
    printf("\n=========== Running Unit Test 1 (add_participation) ===========\n");
    test_add_normal();
    test_add_boundary();
    test_add_extreme();
    test_add_memory_leak();
    printf("======================Unit Test 1 completed.======================\n\n");
}
