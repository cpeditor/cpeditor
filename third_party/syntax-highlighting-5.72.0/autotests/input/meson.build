# Unit test for Meson syntax highlight. License: LGPL
project('projectname', 'cpp')



sourcefiles = ['a.cpp', 'b.cpp']

foreach sourcefile : sourcefiles
    message('this is a source file: ' + sourcefile)
endforeach

x=1
if x+1 == 2 and x-1 == 0
    message('I can work in this universe!')
endif

subprojectresult = subproject('mysubprojectdir')

mysharedlib = shared_library('libraryname', sourcefiles, linkwith: subprojectresult.staticlib)

executable('myprogram', ['test.cpp'], linkwith: mysharedlib)

message(meson.source_root())
message(unknown.source_root())
message('str'.join(l))

