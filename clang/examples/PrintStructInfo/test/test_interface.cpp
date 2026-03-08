// RUN: %clang -cc1 -load %build_root%/lib/PrintStructInfo.dylib \
// RUN:   -plugin print-struct-info %s 2>&1 | %filecheck %s

class Shape {
public:
    virtual double area() = 0;
    virtual double perimeter() = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) {}
    double area() override {}
    double perimeter() override {}
};

// CHECK-DAG: Shape
// CHECK-NEXT: |_Fields
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ area (double()|public|virtual|pure)
// CHECK-DAG: | |_ perimeter (double()|public|virtual|pure)
// CHECK-DAG: | |_ ~Shape (void()|public)
// CHECK-DAG: Circle -> Shape
// CHECK-NEXT: |_Fields
// CHECK-DAG: | |_ radius (double|private)
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ area (double()|public|virtual|override)
// CHECK-DAG: | |_ perimeter (double()|public|virtual|override)
// CHECK-DAG: | |_ ~Circle (void()|public)