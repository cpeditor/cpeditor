GNU gdb (GDB) 8.1
Copyright (C) 2018 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-pc-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from your_application...done.
(gdb) break main
# this is a comment
(gdb) run <arguments for your application>
...
Breakpoint 1 at 0x400b56: file yourmain.cpp, line 3
3: int main(int argc, char* argv[])
(gdb) continue
(gdb) backtrace
#0  0x7f59e39bf18d in poll () from /usr/lib/libc.so.6
#1  0x7f59df6c8c7c in ?? () from /usr/lib/libglib-2.0.so.0
#2  0x7f59df6c8d8c in g_main_context_iteration () from /usr/lib/libglib-2.0.so.0
#3  0x7f59e480b23f in QEventDispatcherGlib::processEvents(...)
    from /usr/lib/libQt5Core.so.5
...
(gdb) where
#0  main (argc=1, argv=0x7fffffffca88) at ../../debugging/ex_debugger/main.cpp:11
(gdb) list
10          bool run = true;
11          while (run) {
12              // infinite loop! can you make it quit?
13          }
(gdb) info locals
app = <incomplete type>
run = true
complexData = {{d = 0x61d430, e = 0x61d430}}
(gdb) info scope main.cpp:12
Scope for main.cpp:12:
Symbol argc is a variable at frame base reg $rbp offset 0+-8, length 4.
Symbol argv is a variable at frame base reg $rbp offset 0+-16, length 8.
(gdb) ptype run
type = bool
(gdb) print run
run = true
(gdb) set run = false
(gdb) print run
run = false
(gdb) print myString.size()
42
(gdb) ptype MyType
type = class MyType {
private:
    int foo;
public:
    int foo() const;
    ...
}
(gdb) break QMessageLogger::warning
Breakpoint 1 at 0x7ffff6f143f0 (8 locations)
(gdb) command 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just \"end\".
> backtrace
> continue
> end
(gdb) catch throw # or when an exception is thrown
Catchpoint 2 (throw)
(gdb) break main.cpp:12 # or when certain code is executed
Breakpoint 3 at 0x401216: file ../../debugging/ex_debugger/main.cpp, line 12.
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>
1.1                         y     0x7ffff6f143f0 <QMessageLogger::warning(char const*, …
...
2       breakpoint     keep y   0x7ffff66d9b90 exception throw
3       breakpoint     keep y   0x000000401216 in main(int, char**)
                                                at debugging/ex_debugger/main.cpp:12
(gdb) condition 3 argc > 5
                # ^-- breakpoint id, see `info breakpoints` above
(gdb) call myFunction()
(gdb) set pagination off
(gdb) set logging file /tmp/warnings.log
(gdb) set logging on
Copying output to /tmp/warnings.log.
(gdb) info threads
            Id   Target Id         Frame
2    Thread 0x7fffe10e1700 (LWP 7271) \"QXcbEventReader\"
    0x7ffff0b6718d in poll () from /usr/lib/libc.so.6
* 1    Thread 0x7ffff7edd840 (LWP 7267) \"kwrite\"
    0x7ffff0b6718d in poll () from /usr/lib/libc.so.6
(gdb) thread
[Current thread is 1 (Thread 0x7ffff7edd840 (LWP 7267))]
(gdb) thread 2
[Switching to thread 2 (Thread 0x7fffe10e1700 (LWP 9765))]
(gdb) thread apply all backtrace
(gdb) info sharedlibrary
From                To                  Syms Read   Shared Object Library
0x7ffff7ddbb80  0x7ffff7df5610  Yes (*)     /lib64/ld-linux-x86-64.so.2
0x7ffff7690460  0x7ffff7ab723c  Yes         /opt/qt/5.5/gcc_64/lib/libQt5Gui.so.5
0x7ffff6efdb80  0x7ffff71af318  Yes         /opt/qt/5.5/gcc_64/lib/libQt5Core.so.5
0x7ffff6c02f60  0x7ffff6c5f852  Yes (*)     /usr/lib/libGL.so.1
0x7ffff69d3ac0  0x7ffff69e0931  Yes (*)     /usr/lib/libpthread.so.0
0x7ffff66d5fa0  0x7ffff6781d89  Yes         /usr/lib/libstdc++.so.6
0x7ffff6353510  0x7ffff63bb33a  Yes (*)     /usr/lib/libm.so.6
...
(*): Shared library is missing debugging information.
(gdb) list 64
59           * If no red can be seen, then the cached implementation is \"good enough\".
60           */
61          void paintEvent(QPaintEvent *event)
62          {
63              const QRect rect = event->rect();
64
65              QPainter painter(this);
66              painter.setRenderHint(QPainter::Antialiasing);
67              painter.fillRect(rect, Qt::black);
68
(gdb) dprintf main.cpp:64,\"paint rect(w=%d,h=%d)\\n\",rect.width(),rect.height()
Dprintf 1 at 0x403a1e: file path/to/main.cpp, line 64.
(gdb) continue
Continuing.
paint rect(w=202,h=200)
paint rect(w=213,h=203)
(gdb) target remote <device ip>:<port>
Remote debugging using <device ip>:<port>
Reading /lib64/ld-linux-x86-64.so.2 from remote target...
warning: File transfers from remote targets can be slow.
            Use \"set sysroot\"to access files locally instead.
...
(gdb) set sysroot /path/to/sysroot
(gdb) print myMap
$1 = QMap<QString, int> = {
    [\"bar\"] = 1,
    [\"foo\"] = 2
}
(gdb) print money
$1 {
    {value = 1.20000005, currency = Money::Euro},
    {value = 3.4000001, currency = Money::Dollar}
}
(gdb) python
> import sys
> sys.path.insert(0, '/path/to/addon/debugging/ex_gdb_pretty_printer')
> import money_printer
> end
(gdb) print money
$2 = {
    \"1.200000 Euro\",
    \"3.400000 Dollar\"
}
(gdb) continue
(gdb) detach
(gdb) quit
