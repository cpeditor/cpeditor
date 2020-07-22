if(Qt5Widgets_FOUND AND Qt5PrintSupport_FOUND)
    add_executable(codepdfprinter codepdfprinter.cpp main.cpp)
    target_link_libraries(codepdfprinter Qt5::Widgets Qt5::PrintSupport KF5SyntaxHighlighting)
endif()
