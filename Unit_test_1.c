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

    printf("======================Normal Case Passed======================\n");
    reset_stdin();
}

// =====================
// Test 2: Boundary
// =====================
static void test_add_boundary() {
    Workshop list[MAX];
    int count = MAX - 1;  // ทดสอบ edge case ของจำนวน participant
    printf("======================[Test 2] Boundary Case======================\n");

    // input ที่ date/เวลาผิด
    char input[300];
    snprintf(input, sizeof(input),
        "Jane\nSmith\nBoundary Workshop\n"
        "2025/13/32\n"   // เดือน/วันเกินขอบเขต
        "2025/12/31\n"   // วันที่ถูกต้อง (แก้จาก invalid)
        "-1\n"           // duration ติดลบ
        "2\n"            // duration ถูกต้อง
    );

    set_stdin_input(input);

    // add_participation ต้อง loop รับ input จน valid
    add_participation(list, &count);

    // ตรวจว่าฟังก์ชันไม่ crash และ count เพิ่มขึ้น
    assert(count == MAX);

    // แสดงผลเพื่อดูค่า
    printf("First Name: %s\n", list[MAX-1].firstName);
    printf("Last Name: %s\n", list[MAX-1].lastName);
    printf("Workshop Title: %s\n", list[MAX-1].workshopTitle);
    printf("Date: %s\n", list[MAX-1].workshopDate);
    printf("Duration: %s\n", list[MAX-1].duration);

    printf("======================Boundary Case Passed======================\n");
    reset_stdin();
}

// =====================
// Test 3: Exterme
// =====================
static void test_add_extreme() {
    Workshop list[MAX];
    int count = MAX - 1;  // ทดสอบ edge case ของจำนวน participant
    printf("======================[Test 3] Extreme Case======================\n");

    // input ที่ date/เวลาผิดพรมแดน
    char input[300];
    snprintf(input, sizeof(input),
        "123John\n"          // ชื่อมีตัวเลข
        "John\n"             // ชื่อถูก
        "Smith!\n"           // นามสกุลมี special char
        "Smith\n"            // นามสกุลถูก
        "WorkshopWithVeryLongNameExceedingLimit1234567890\n"
        "abcd/12/34\n"       // วันที่ผิด
        "2025/01/01\n"       // วันที่ถูก
        "0\n"                // duration ผิด
        "3\n"                // duration ถูก
    );

    set_stdin_input(input);

    // add_participation ต้อง loop รับ input จน valid
    add_participation(list, &count);

    // ตรวจว่าฟังก์ชันไม่ crash และ count เพิ่มขึ้น
    assert(count == MAX);

    // แสดงผลเพื่อดูค่า
    printf("First Name: %s\n", list[MAX-1].firstName);
    printf("Last Name: %s\n", list[MAX-1].lastName);
    printf("Workshop Title: %s\n", list[MAX-1].workshopTitle);
    printf("Date: %s\n", list[MAX-1].workshopDate);
    printf("Duration: %s\n", list[MAX-1].duration);

    printf("======================Extreme Case Passed======================\n");
    reset_stdin();
}


// =====================
// Test 4: Memory Leak (Valgrind)
// =====================
static void test_add_memory_leak() {
    Workshop list[MAX];
    int count = 0;
    printf("======================[Test 4] Memory Leak (check with valgrind)======================\n");

    set_stdin_input("Test\nLeak\nMemory Test\n2025/10/03\n2\n");
    add_participation(list, &count);

    printf("======================Memory Leak Test======================\n");
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
