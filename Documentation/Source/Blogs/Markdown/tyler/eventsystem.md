#The Event System

## What is this Event System

The Event System is a framework in the engine for performing asynchronous, lazy
evaluation of function calls. The way this system works is through a combination
of messages (or _Events_) and listeners (or _handlers_). The exact method by which
it works was inspired by the
[Forge Modding API](https://mcforge.readthedocs.io/en/latest/events/intro/),
which takes advantage of Java's Annotation and reflection system. Since neither
exist in C++ at this time, I ended up needing to take advantage of various
esoteric tricks of the C++ language to create an API that roughly mimics Forge's
event system, while still keeping it relatively to use and maintain.

## How is the Event System Used?

The EventSystem is fairly simple to use and relies solely on understanding three
main parts: the `EventBus` class, the `Event` class, and the various types of
Event Handlers. Do note that all C++ types, with the exception of C++ macros,
described in this document as part of the Event System API are under the
`EventSystem3` namespace. For simplicity, I will be omitting the namespace
qualifier in all descriptions, but not from code samples. Do keep this in mind
while reading.

### The EventBus class

The `EventBus` class, found in the `<EventBus.hpp>` header, is the main driver
of the EventSystem and is required for the basic usages of the system. In this
section, I will only be going over the inital usage of this class. There is
other functionality that is important to know about it, however I will be
bringing it up only when relevant. For now, what is important to understand is
that the `EventBus` is a
[singleton](https://en.wikipedia.org/wiki/Singleton_pattern) class, and cannot
be inherited from. This means that it is very simple to get started with using
it, as it can simply be treated as a normal global variable with no special
initializations required. To obtain the `EventBus`, you simply need to either
invoke the static `EventBus::getInstance()` method or to refer to the global
`EVENT_BUS` macro, which is the recommended manner of usage.

Once you have the `EventBus` instance, now we want to make it able to actually
process events. Doing this is fairly simple, and involves calling the
`EventBus::update(float dt)` method on that instance like so:

```cpp
void gameLoop(float dt) {
    while(true) {
        // ...

        EVENT_BUS.update(dt);
    }
}
```

There are two important things to note about this method. The first is the
parameter, which is a delta-time value similar to what is used in graphics
applications. Its purpose is for delayed events (a feature that will be
explained later), however, this parameter may be set to 0 if you do not plan on
using delayed events. The second thing to note is that this method should be
called exactly once for every tick/loop of your program. Invoking it will
process all events that are waiting in its internal queue, so it is recommended
to only ever call it once at the end of your program loop. Not doing so could
cause events to be handled in strange orders  or for delayed events to be
handled earlier than expected.

### The EventBase class

So, we now have the Event System set up to process events, however we don't
currently _have_ any events for it to process. While the Event System does not
actually come built-in wirth any events, defining your own custom event types is
very easy. To do so, we simply have to define a `class` or `struct` which
inherits from the `EventBase` class, found in the `<EventBase.hpp>` header, like
so:

```cpp
struct MyEvent: public EventSystem3::EventBase {
};
```

Since this event is just an ordinary `struct`, we can name it whatever we want
to and may place any relevant data fields or member functions inside. As an
example, see this implementation of a `WindowEvent` wrapping around the
[SDL_WindowEvent](https://wiki.libsdl.org/SDL_WindowEvent) type:

```cpp
class WindowEvent : public EventSystem3::EventBase {
    public:
        WindowEvent(Uint8 event, int x, int y);
        PARENT(EventBase);

        //type of window event recieved
        //list of window events at https://wiki.libsdl.org/SDL_WindowEventID
        Uint8 M_event;
        //x coordinate for certain window events
        int M_x;
        //y coordinate for certain window events
        int M_y;
};
```

Now, one last thing to note about defining events before we move on. The
Event System also supports polymorphic event types, however this feature
requires explicitly specifying in the `public` section of your event definition
the parent event type with the `PARENT(TYPE)` macro. This macro is optional,
but without it your event will only be handled by handlers that explicitly ask
for your event, and those that attempt to handle a more generic event type will
not be notified of your event.

Now that we have a custom event, we want to be able to fire it. To do so is
fairly simple, and requires calling the `EventBus::fire()` method, defined as:

```cpp
template<typename T>
EventBus::fire(T* event, EventHandler* destination = nullptr,
               bool immediately = false, Priority priority = Priority::NORMAL,
               float time = 0.0f, const std::string& func = "",
               unsigned long line = 0)
```

While this method may look scary with all of its many parameters, only the first
is actually required. With that in mind, let us pretend for a moment that the
rest don't exist, and that the method instead simply looks like
`EventBus::fire(T* event)`. This first parameter is, as the name might imply,
simply a dynamically allocated pointer to the event you wish to fire. This
parameter must be allocated with the `new` operator. For example, using the
event `MyEvent` that we defined earlier, we can fire that event with the
following code:

```cpp
EVENT_BUS.fire(new MyEvent());
```

Note that I don't do anything with the pointer anymore once I have fired the
event. The reason for this is that because the exact moment when an event is to
no longer be needed cannot be determined, the `EventBus` will take ownership and
responsibility for all events fired with it, and will automatically delete the
event for you once it is no longer needed. Essentially, once you have given your
event pointer to the `EventBus`, you are free to simply forget about it, as the
`EventBus` will free the memory up once the memory is no longer needed.

Now that we have gone over the basic usage of that method, let us now go over
what the rest of those parameters mean:

* `destination` -- This parameter allows specifying an `EventHandler` object, a
concept which will be explained in the next section, that this event should be
"fired at", ensuring that the event will only be handled by that specific
object.
* `immediately` -- This parameter specifies that this event should be handled
immediately at the fire-site, rather than being placed into a waiting queue to
be processed later on the next call to `EventBus::update()`.
* `priority` -- This parameter specifies the priority of this event. Higher
priority events will be handled before lower priority events. See the `Priority`
enumeration for a full list of all priority levels.
* `time` -- The amount of time, in seconds, to wait before allowing this event
to be handled. Requires that the `EventBus::update()` method be passed a `dt`
value that is greater than 0.
* `func` -- The name of the function which is firing this event. Used mainly for
debugging purposes.
* `line` -- The line number this event was fired from. Used mainly for debugging
purposes.

### Event Handling

Okay, so we now have the Event System up and running to process events, and we
now have events that can be processed by it. The only thing left now is to write
some code which can receive and handle these new events.

The first thing to note is that there are two different types of event handlers:
function handlers and method handlers. These two types of event handlers are
very similar in how they work, with the main difference being how they get
subscribed to their events.

All event handlers, be they functions or methods, will take the following form:
```cpp
void myEventHandler(MyEvent& event) {
    // Do something with the event now
}
```

Basically, each event handler may be named whatever you want, and the only
restrictions are that it must return `void` and must take a non-`const`
reference to the event type you wish to handle. That is all that is required of
an event handler, and other than that handlers work basically like normal
functions/methods, and can do anything else they wish internally.

However, just having this definition is not enough to have an event handler. We
must also tell the Event System that we want to be _subscribed_ to the event. To
do this, we must use the `SUBSCRIBE(EVENT, FUNC, OBJ, PRIORITY)` macro, like so:

```cpp
void myEventHandler(MyEvent& event) {
    // Do something with the event now
}
SUBSCRIBE(MyEvent, myEventHandler,,);
```

There are a few things to note about this macro. The first is that the final
two parameters to it are _optional_ and can be left blank. Those who are familiar
with the limitations of the C preprocessor will be able to point out that
function-like preprocessor macros do not actually support optional or defaulted
parameters. This is why the trailing commas in the argument list are still
necessary: The arguments are still being given, but are left blank, which the
macro is designed to interpret as the lack of an argument and use a default
argument instead. In summation, if you do not need to pass anything to these
parameters, you must still "specify" them, but may leave the argument itself
blank.

Another thing to note about this macro is that it must be able to see the
declaration of the event handler, however the location of the expansion in
relation to the handler is not particularly important beyond that.

Lastly, it is recommended to expand this `SUBSCRIBE` macro in a context where it
will only get expanded once, such as in a source file rather than a header. The
reason for this is due to a consequence of how `SUBSCRIBE` works internally.
When you use this macro, `SUBSCRIBE` will generate a static object of a complex
templated type whose only job is to take in your parameters and figure out how
your handler is supposed to be subscribed. This is what allows the macro to be
expanded in the global scope as well as within function calls. However, if it
gets expanded multiple times, then your event handler will get subscribed to the
event multiple times as well, which could cause weird effects.

Now that all of that has been mentioned, lets go over each of the parameters for
the `SUBSCRIBE` macro to give a clear idea of what they are and what they mean:

* `EVENT` -- This is the event type that is being subscribed to. It must be the
plain, unqualified `class` or `struct` type which inherits from the `EventBase`
class at some point in its inheritance hierarchy. Additionally, by "unqualified"
I am referring to the fact that the type given to `EVENT` must not be `const`,
`constexpr`, may not be a pointer, reference, or r-value reference, or any other
form of additional qualifiers that may be generally added to types.
* `FUNC` -- This is a pointer to a function or method that is able to handle
the event type given to `EVENT`, which must be of the form mentioned earlier at
the top of this section. This value may _not_ be an anonymous/lambda function.
* `OBJ` -- This is the object which `FUNC` is a method of. This generally should
only be set when `FUNC` is a method pointer of some description. The type of
arguments that can be given here will be described after this list.
* `PRIORITY` -- This is the priority of this event handler, which specifies the
priority of this event handler. Higher priority event handlers will be called
sooner than lower priority event handlers. This takes the same `Priority`
enumeration as the `EventBus::fire()` method.

Finally, we can now take a look at how event handler methods work. Methods are
a bit weird in terms of how they are implemented, so while they do use the same
interface and have the same form as function handlers, there are some key
differences in how they are subscribed that need addressing.

First, the type that method event handlers are a member of _must_ inherit from the
`EventHandler` class, which can be found in the `<EventHandler.hpp>` header.
This is because `EventHandler` defines some important methods internally that
are used by the rest of the Event System for managing these objects. Secondly,
`SUBSCRIBE` for event handlers may be expanded inside the class body, but it
must be qualified with the `INCLASS` macro like this:

```cpp
struct MyEventHandler: public EventSystem3::EventHandler {
    MyEventHandler();

    void myEventHandler(MyEvent&);
    INCLASS SUBSCRIBE(MyEvent, &MyEventHandler::myEventHandler, EHCLASS(MyEventHandler),);
};
```

`INCLASS` expands to the `inline` keyword and makes use of C++17's
`inline variables` feature, however be careful as some compilers may not support
this feature. Additionally, take note of how we have given the result of some
other macro to the `OBJ` parameter. This macro expands to a special templated
helper class that tells the Event System to perform a sort of "lazy"
subscription, placing your subscription into a list of subscription calls that
need to be made when your Event Handler object gets constructed. However, this
initialization needs to be invoked manually from your object's constructor.
You may do this by either calling the `EventHandler::init()` method, or by
using the `EH_INITIALIZE()` macro, which expands to the same thing, but is
slightly more explicit as to what is happening. Finally, also note that method
event handlers have a lifetime tied to the lifetime of the object they are
members of. When your event handler object's destructor gets called, so does
the destructor of `EventHandler`, which will auto-unsubscribe the instances of
your methods bound to that object.

If you do not wish to deal with any of this lazy subscription, initialization,
or with the `INCLASS` macro, you may instead simply expand the `SUBSCRIBE` macro
like normal in either your class's constructor, or just after the object has
been constructed, passing a pointer to your object as the third parameter for
`SUBSCRIBE` instead of the `EHCLASS` macro. As an example, see:

```cpp
MyEventHandler::MyEventHandler(): EventSystem3::EventHandler(this) {
    SUBSCRIBE(MyEvent, &MyEventHandler::myEventHandler, this,);
}
```

## Possible Improvements

Of course, this system is not without problems. Some of these problems are
fundamental to how it works internally, while others can be improved with some
work and effort. One of these is the first parameter to `SUBSCRIBE`. The type of
event which handlers are expected to be subscribed to can (and probably _should_)
be pulled out of the given handler's type. Since that Event type must be given
as the handler's one and only parameter, it should be fairly easy to extract
the event type from there. The only reason for the parameter is mainly
historical, as the typename used to be used in a token-pasting operation to
create the variable name for the static subscriber object generated by the
`SUBSCRIBER` macro.

Another issue with the Event System in its current state is one of efficiency.
Many locations in the event system use `std::vector` for storing lists of data
types, where things like `std::set` or similar such structures would be better
to use to prevent duplicate entries from being created where they shouldn't be.
Additionally, all events currently must be dynamically allocated on the heap
with the `new` operator. While this works in many cases, a better solution would
be to have internally a configurable allocator which is used instead of the
default one.

Finally, the Event System could stand from being more thread safe. While I have
taken measures to make it thread safe, there are still issues with what is done
so far, and I am not 100% certain that it will not have problems when
interacting with threaded code.

## Conclusion

Hopefully this gives a comprehensive overview of how the Event System works from
a usage level, and demonstrates both advantages to, and potential draw-backs of
using the Event System. Note that I did not go over every possible feature in
the Event System, as some of them are very niche in terms of use-cases, such as
unsubscribing handlers, manual handler subscription, cancelling events, etc...
For documentation on how to use these, please look in the Doxygen docs for
information on them.

