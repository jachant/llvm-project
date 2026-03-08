// RUN: %clang -cc1 -load %build_root%/lib/PrintStructInfo.dylib \
// RUN:   -plugin print-struct-info %s 2>&1 | %filecheck %s

struct Human {
  unsigned int age;
  unsigned int height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

struct Engineer : public Human {
  unsigned int salary;
  void sleep() override {}
  void eat() override {}
  void work() {}
};

// CHECK-DAG: Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ age (unsigned int|public)
// CHECK-NEXT: | |_ height (unsigned int|public)
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ sleep (void()|public|virtual|pure)
// CHECK-DAG: | |_ eat (void()|public|virtual|pure)
// CHECK-DAG: | |_ ~Human (void()|public)
// CHECK-DAG: Engineer -> Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ salary (unsigned int|public)
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ sleep (void()|public|override)
// CHECK-DAG: | |_ eat (void()|public|override)
// CHECK-DAG: | |_ work (void()|public)
// CHECK-DAG: | |_ ~Engineer (void()|public)