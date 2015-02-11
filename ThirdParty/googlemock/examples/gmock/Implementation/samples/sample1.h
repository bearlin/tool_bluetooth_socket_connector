#ifndef GMOCK_SAMPLES_SAMPLE1_H_
#define GMOCK_SAMPLES_SAMPLE1_H_

// https://code.google.com/p/googlemock/wiki/ForDummies
// A Case for Mock Turtles:
// Let's look at an example. 
// Suppose you are developing a graphics program that relies on a LOGO-like API for drawing. 
// How would you test that it does the right thing? 
// Well, you can run it and compare the screen with a golden screen snapshot, 
// but let's admit it: tests like this are expensive to run and fragile (What if you just upgraded to a shiny new graphics card 
// that has better anti-aliasing? Suddenly you have to update all your golden images.). 
// It would be too painful if all your tests are like this. 
// Fortunately, you learned about Dependency Injection and know the right thing to do: instead of having your application 
// talk to the drawing API directly, wrap the API in an interface (say, Turtle) and code to that interface:
class Turtle {
  public:
  virtual ~Turtle() {}
  virtual void PenUp() = 0;
  virtual void PenDown() = 0;
  virtual void Forward(int distance) = 0;
  virtual void Turn(int degrees) = 0;
  virtual void GoTo(int x, int y) = 0;
  virtual int GetX() const = 0;
  virtual int GetY() const = 0;
};

// https://code.google.com/p/googlemock/wiki/ForDummies
// Writing the Mock Class:
// Using the Turtle interface as example, here are the simple steps you need to follow:

// 1. Derive a class MockTurtle from Turtle.
// 2. Take a virtual function of Turtle (while it's possible to mock non-virtual methods using templates, it's much more involved). 
//    Count how many arguments it has.
// 3. In the public: section of the child class, write MOCK_METHODn(); (or MOCK_CONST_METHODn(); 
//    if you are mocking a const method), where n is the number of the arguments; if you counted wrong, 
//    shame on you, and a compiler error will tell you so.
// 4. Now comes the fun part: you take the function signature, cut-and-paste the function name as the first argument to the macro, 
//    and leave what's left as the second argument (in case you're curious, this is the type of the function).
// 5. Repeat until all virtual functions you want to mock are done.
// After the process, you should have something like:
#include "gmock/gmock.h"  // Brings in Google Mock.

class MockTurtle : public Turtle {
  public:
  MOCK_METHOD0(PenUp, void());
  MOCK_METHOD0(PenDown, void());
  MOCK_METHOD1(Forward, void(int distance));
  MOCK_METHOD1(Turn, void(int degrees));
  MOCK_METHOD2(GoTo, void(int x, int y));
  MOCK_CONST_METHOD0(GetX, int());
  MOCK_CONST_METHOD0(GetY, int());
};

#endif  // GMOCK_SAMPLES_SAMPLE1_H_
