// RUN: %clang -cc1 -load %build_root%/lib/PrintStructInfo.dylib \
// RUN:   -plugin print-struct-info %s 2>&1 | %filecheck %s

struct AccessTest {
private:
    int privateValue;
protected:
    int protectedValue;
public:
    int publicValue;
    void publicMethod();
};

// CHECK-DAG: AccessTest
// CHECK-NEXT: |_Fields
// CHECK-DAG: | |_ privateValue (int|private)
// CHECK-DAG: | |_ protectedValue (int|protected)
// CHECK-DAG: | |_ publicValue (int|public)
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ publicMethod (void()|public)