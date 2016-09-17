# OXFEDE::Event

C# Style Events in C++

## How to use it:
1. Include `Event.h` in your project. It's all template based so that's it as far as integrating it.
2. Declare an event inside the class which will own the event (only the owner class can instantiate and raise the event).
3. Use `+=` and `-=` to add / remove delegates (removal takes a key -- part of the event type).
4. From the owner class scope, use the call operator to raise the event.


## Event example:

```c++

```

## MultiEvent example:

```c++
```

[Detailed tutorial on this component's implementation](http://0xfede.io/2016/09/14/T-C++-Event.html)

<br>
Copyright (c) 2016 Federico Saldarini

[LinkedIn][l1] | [Blog][l2] | [GitHub][l3]

[l1]: https://www.linkedin.com/in/federicosaldarini
[l2]: http://0xfede.io
[l3]: https://github.com/saldavonschwartz
