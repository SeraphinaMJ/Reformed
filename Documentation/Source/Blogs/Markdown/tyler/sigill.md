# SIGILL and how to fix It.

When working on the project's cross-platformability capabilities, I ran into
one of those few errors that programmers hate to see:
```
Illegal Instruction (core dumped)
```
This, for those who do not know is a CPU signal that something has gone horribly
wrong. It is also referred to as `SIGILL`, and essentially means that the CPU
was given an assembly opcode that it doesn't know how to interpret properly.
Another thing about this error that may make those familiar with C/C++ shiver is
that it is eerily similar to a different error:
```
Segmentation Fault (core dumped)
```
Which it should, as the two errors usually stem from the same problem: memory
corruption. What usually happens to cause these types of errors is that one or
more parts of the code end up reading or writing from a part of memory that they
shouldn't have. For Segmentation Faults (`SIGSEGV`), this is when the program
attempts to read or write to memory outside of the program's memory space.

With `SIGILL`, this generally means that the program has had its stack
corrupted, usually to such an extent that the return address is pointing to
somewhere completely random in memory. When something like this happens, it
would result in either a `SIGSEGV` usually, or a `SIGILL` if it somehow managed
to jump into memory that your program owns before encountering some data value
that wouldn't be a valid instruction.

The way to solve these is to use some form of memory debugger, such as
[Valgrind](http://valgrind.org/), to have it check your code for memory leaks
and corruptions. This will be able to find any forms of stack corruption that
may exist in your code, allowing you to rewrite the code to fix the issue.

## It's not that simple

Of course, there is a catch here, I wouldn't be writing this if there wasn't.
Notice how earlier I wrote that `SIGILL` is raised when an assembly opcode is
encountered that your CPU doesn't know how to interpret. I never said anything
about it being a memory issue. The issue is usually encountered when a memory
issue is occurring, but that doesn't mean it has to _always_ be like that.

When Valgrind ended up failing to find any memory problems, I decided to step
through it in GDB manually, trying to find out which function specifically it
ended up failing in. However, that of course ended in failure as it again wasn't
seeming to fail on any particular line of code, just a generic failure when it
finished with the else-clause in this if statement:
```cpp
    if(if_stream)
    {
        std::string file_as_string(std::istreambuf_iterator<char>(if_stream),
                                   std::istreambuf_iterator<char>());
        return jsonSerialization::streamToType(file_as_string);
    }
    else
    {
        std::cerr << "Failed to open file at: " << l_path << std::endl;
    }
```
That is to say, according to GDB the program was _consistently_ crashing just
after it left the final `}` in the else-clause. With no other option left, I 
decided to just step through the program at the assembly level to figure out 
exactly which assembly opcode it was crashing on. Normally this wouldn't be a
good idea since C++, while fairly high-level and simple on the surface, gets
converted to some very nasty assembly through the use of compiler optimizations
and various quirks of STL implementations. However, since the scope was limited,
there wouldn't be _too_ many instructions to have to step over before I got to
the crash. After a bit of stepping instruction by instruction, I finally came
upon the bad opcode: `ud2`. After talking with another person on the team, we
were able to find out that `ud2` is an Intel specific instruction meaning
[Undefined Instruction](https://www.felixcloutier.com/x86/ud). Esentially, the
reason why valgrind was not finding any memory corruption/stack overwrite errors
is because there were none. Clang was placing instructions into the code that
were _supposed_ to raise this signal. But why would it do that? The reason,
becomes fairly obvious once you take a look at the entirety of the function that
was failing:
```cpp
typeRT persistenceSystem::readFromFile(const std::filesystem::path p_folder,
                                       const std::filesystem::path p_file_name)
{
    std::filesystem::path l_path = p_folder / p_file_name;
    std::ifstream if_stream(l_path);

    if(if_stream)
    {
        std::string file_as_string(std::istreambuf_iterator<char>(if_stream),
                                   std::istreambuf_iterator<char>());
        return jsonSerialization::streamToType(file_as_string);
    }
    else
    {
        std::cerr << "Failed to open file at: " << l_path << std::endl;
    }
}
```

If you haven't spotted it yet, there is a conditional branch statement here,
however a value is only returned in _one_ of those branches. This, according to
the C++ Standard, is undefined behavior, and the compiler is free to do whatever
it wants here. In this case Clang decided to insert a `ud2` instruction, which
means that whenever the code was given a file path that it didn't know existed,
it  printed that the file could not be opened and then crashed. The easy fix to
this problem is to simply change the failure case to a `throw` like so:
```cpp
    if(if_stream)
    {
        std::string file_as_string(std::istreambuf_iterator<char>(if_stream),
                                   std::istreambuf_iterator<char>());
        return jsonSerialization::streamToType(file_as_string);
    }
    std::cerr << "Failed to open file at: " << l_path << std::endl;
    throw std::filesystem::filesystem_error("Could not open file", l_path,
                                            std::make_error_code(std::errc::no_such_file_or_directory));
}
```


