#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
void update_participation(Workshop list[], int *count);

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
static void test_update_normal() {
    Workshop list[MAX];
    int count = 1;

    // เตรียมข้อมูลเดิม
    strcpy(list[0].firstName, "John");
    strcpy(list[0].lastName, "Doe");
    strcpy(list[0].workshopTitle, "Old Title");
    strcpy(list[0].workshopDate, "2025/01/01");
    strcpy(list[0].duration, "2");

    printf("======================[Test 1] Normal Case======================\n");

    // input flow: [Full name] [เลือก update 1=Title] [ค่าใหม่]
    set_stdin_input("John Doe\n1\nNew Workshop Title\n");

    update_participation(list, &count);

    assert(strcmp(list[0].workshopTitle, "New Workshop Title") == 0);

    printf("======================Normal Case Passed======================\n");
    reset_stdin();
}


// =====================
// Test 2: Boundary
// =====================
static void test_update_boundary() {
    Workshop list[MAX];
    int count = 1;

    strcpy(list[0].firstName, "Jane");
    strcpy(list[0].lastName, "Smith");
    strcpy(list[0].workshopTitle, "Boundary Test");
    strcpy(list[0].workshopDate, "2025/01/01");
    strcpy(list[0].duration, "5");

    printf("======================[Test 2] Boundary Case======================\n");

    // input flow: full name -> เลือก 2 (Date) -> ใส่ผิดก่อน -> ใส่ใหม่ถูก
    char input[200];
    snprintf(input, sizeof(input),
        "Jane Smith\n2\n2025/13/32\n2025/12/31\n"
    );

    set_stdin_input(input);

    update_participation(list, &count);

    assert(strcmp(list[0].workshopDate, "2025/12/31") == 0);

    printf("======================Boundary Case Passed======================\n");
    reset_stdin();
}


// =====================
// Test 3: Exterme
// =====================
static void test_update_extreme() {
    Workshop list[MAX];
    int count = 1;

    strcpy(list[0].firstName, "Bob");
    strcpy(list[0].lastName, "Extreme");
    strcpy(list[0].workshopTitle, "Extreme Old");
    strcpy(list[0].workshopDate, "2025/01/01");
    strcpy(list[0].duration, "3");

    printf("======================[Test 3] Extreme Case======================\n");

    // input flow: full name -> เลือก 3 (Duration) -> ใส่ 0 -> -5 -> valid
    char input[200];
    snprintf(input, sizeof(input),
        "Bob Extreme\n3\n999999\n-999999\n10\n"
    );

    set_stdin_input(input);

    update_participation(list, &count);

    assert(strcmp(list[0].duration, "10") == 0);

    printf("======================Extreme Case Passed======================\n");
    reset_stdin();
}



// =====================
// Test 4: Memory Leak (Valgrind)
// =====================
static void test_update_memory_leak() {
    Workshop list[MAX];
    int count = 1;

    strcpy(list[0].firstName, "Leak");
    strcpy(list[0].lastName, "Test");
    strcpy(list[0].workshopTitle, "Old Leak");
    strcpy(list[0].workshopDate, "2025/01/01");
    strcpy(list[0].duration, "5");

    printf("======================[Test 4] Memory Leak======================\n");

    // เปลี่ยน title
    set_stdin_input("Leak Test\n1\nNew Leak Title\n");

    update_participation(list, &count);

    printf("======================Memory Leak Test Passed (Check with Valgrind)======================\n");
    reset_stdin();
}


// =====================
// ฟังก์ชันเรียกทั้งหมด (เรียกจาก unit_tests())
// =====================
void run_unit_test_2(void) {
    printf("\n=========== Running Unit Test 2 (update_participation) ===========\n");
    test_update_normal();
    test_update_boundary();
    test_update_extreme();
    test_update_memory_leak();
    printf("======================Unit Test 2 completed.======================\n\n");
}
