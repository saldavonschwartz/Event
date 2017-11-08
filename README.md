# OXFEDE::Event

C# Style Events in C++

## How to use it:
1. Include `Event.h` in your project. It's all template based so that's it as far as integrating it.
2. Declare an event inside the class which will own the event (only the owner class can instantiate and raise the event).
3. Use `+=` and `-=` to add / remove delegates (removal takes a key -- part of the event type).
4. From the owner class scope, use the call operator to raise the event.


## Event example:

```c++
#include <iostream>
#include "Event.h"

class A {

public:
  Event<A, std::string, void(int)> onEvent;

  void clearEvent() {
    onEvent.bindings.clear();
  }

  void raiseEvent() {
    onEvent(2);
  }
};

class B {

public:
  static void staticHandler(int i) {
    std::cout << "\nstatic handler: " << i;
  }

  void instanceHandler(int i) {
    std::cout << "\ninstance handler: " << i;
  }

};

void functionHandler(int i) {
  std::cout << "\nfunction handler: " << i;
}

int main() {
  A a;
  B b;

  a.onEvent += {"function", &functionHandler};
  a.onEvent += {"static", &B::staticHandler};
  a.onEvent += {"instance", &b, &B::instanceHandler};
  a.onEvent += {"lambda", [](int i) {
    std::cout << "\nlambda handler: " << i;
  }};

  return 0;
}
```

## MultiEvent example:

There are also multievents. These basically group related events so they can easily be indexed by an arbitrary key:

```c++
class Input {
public:
  enum Key {A,W,S,D};
  MultiEvent<Input, Key, std::string, void()> onKeyPressed;
};

int main() {
  Input input;
  input.onKeyPressed[Input::Key::A] += {"left key handler", []() {
    // move player left
  }};

  // ...
}
```

<br>
Copyright (c) 2016 Federico Saldarini

[LinkedIn][l1] | [0xfede.io][l2] | [GitHub][l3]

[l1]: https://www.linkedin.com/in/federicosaldarini
[l2]: http://0xfede.io
[l3]: https://github.com/saldavonschwartz
