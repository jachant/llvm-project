// RUN: %clang -cc1 -load %build_root%/lib/PrintStructInfo.dylib \
// RUN:   -plugin print-struct-info %s 2>&1 | %filecheck %s

class Draggable {
public:
    virtual void move(int x, int y) = 0;
    virtual ~Draggable() {}
};

class Window : public Draggable {
private:
    bool isVisible;
public:
    void move(int x, int y) override {}
    void show() {}
};

// CHECK-DAG: Draggable
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ move (void()|public|virtual|pure)
// CHECK-DAG: | |_ ~Draggable (void()|public)
// CHECK-DAG: Window -> Draggable
// CHECK-NEXT: |_Fields
// CHECK-DAG: | |_ isVisible (_Bool|private)
// CHECK-NEXT: |_Methods
// CHECK-DAG: | |_ move (void()|public|virtual|override)
// CHECK-DAG: | |_ show (void()|public)
// CHECK-DAG: | |_ ~Window (void()|public)