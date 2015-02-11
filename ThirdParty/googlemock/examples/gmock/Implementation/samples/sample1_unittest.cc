#include "sample1.h" //"path/to/mock-turtle.h"

// I create this class for using MockTurtle.
class Painter{
  public:
    Painter(MockTurtle* turtle) : turtle_(turtle)
    {
    }
    
    ~Painter() {}
    
    int DrawSomething(int a, int b, int c)
    {
      //printf("Call Painter::DrawSomething(%d,%d,%d)!\n", a, b, c);
      turtle_->PenUp();
      turtle_->PenDown();
      return 1;
    }
    
    int DrawPoint(int x, int y)
    {
      //printf("Call Painter::DrawPoint(%d,%d)!\n", x, y);
      turtle_->GoTo(x, y);
      return 1;
    }
    
  private:
    MockTurtle* turtle_;
};

// https://code.google.com/p/googlemock/wiki/ForDummies
// Using Mocks in Tests:
// Once you have a mock class, using it is easy. The typical work flow is:
// 1. Import the Google Mock names from the testing namespace such that you can use them unqualified 
//    (You only have to do it once per file. Remember that namespaces are a good idea and good for your health.).
// 2. Create some mock objects.
// 3. Specify your expectations on them (How many times will a method be called? With what arguments? What should it do? etc.).
// 4. Exercise some code that uses the mocks; optionally, check the result using Google Test assertions. 
//    If a mock method is called more than expected or with wrong arguments, you'll get an error immediately.
// 5. When a mock is destructed, Google Mock will automatically check whether all expectations on it have been satisfied.
// Here's an example:
#include "sample1.h" //"path/to/mock-turtle.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::AtLeast;                     // #1

TEST(PainterTest, CanDrawSomething) {
  MockTurtle turtle;                          // #2
  
  EXPECT_CALL(turtle, PenDown())              // #3
    .Times(AtLeast(1));
  EXPECT_CALL(turtle, PenUp())
    .Times(AtLeast(1));

  Painter painter(&turtle);                   // #4

  EXPECT_TRUE(painter.DrawSomething(0, 0, 10));
}                                             // #5

TEST(PainterTest, CanDrawPoint) {
  MockTurtle turtle;
  
  EXPECT_CALL(turtle, GoTo(6, 7))
    .Times(AtLeast(1));

  Painter painter(&turtle);

  EXPECT_TRUE(painter.DrawPoint(6, 7));
}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
