if(Qt5Widgets_FOUND)
    add_executable(minimal main.cpp)
    target_link_libraries(minimal Qt5::Widgets KF5SyntaxHighlighting)
endif()
