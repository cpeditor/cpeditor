#!/bin/bash
# Test script for CSES CLI integration

echo "=== CSES Tool Integration Test ==="
echo

echo "1. Building CP Editor..."
cd /home/robert/CLionProjects/cpeditor/build
cmake --build . -j$(nproc) 2>&1 | tail -5
echo "Build status: $?"

echo
echo "2. Checking CSES CLI installation..."
which cses-cli 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✓ CSES CLI found in PATH"
    cses-cli --version 2>/dev/null
else
    echo "✗ CSES CLI not found in PATH"
    echo "   Expected default path: 'cses-cli'"
    echo "   You can set a custom path in Preferences → Extensions → CSES CLI"
fi

echo
echo "3. Testing URL parsing with C++ regex..."
cat > /tmp/test_cses_urls.cpp << 'EOF'
#include <iostream>
#include <regex>
#include <string>

void test_url(const std::string& url) {
    std::regex pattern1(R"(.*://cses\.fi/([^/]+)/task/([0-9]+))");
    std::regex pattern2(R"(.*://cses\.fi/([^/]+)/([^/]+)/([0-9]+))");
    std::smatch match;
    
    std::cout << "URL: " << url << std::endl;
    
    if (std::regex_match(url, match, pattern1) && match.size() == 3) {
        std::cout << "  ✓ MATCH (pattern1)" << std::endl;
        std::cout << "    Contest: " << match[1] << std::endl;
        std::cout << "    Task ID: " << match[2] << std::endl;
    } else if (std::regex_match(url, match, pattern2) && match.size() == 4) {
        std::cout << "  ✓ MATCH (pattern2)" << std::endl;
        std::cout << "    Contest: " << match[1] << std::endl;
        std::cout << "    Task ID: " << match[3] << std::endl;
    } else {
        std::cout << "  ✗ NO MATCH" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    test_url("https://cses.fi/problemset/task/1068");
    test_url("https://cses.fi/problemset/task/1633");
    test_url("https://cses.fi/100/task/100");
    test_url("http://cses.fi/problemset/task/1754");
    test_url("https://codeforces.com/problemset/problem/4/A");
    test_url("https://cses.fi/problemset/task/");
    test_url("invalid url");
    return 0;
}
EOF

g++ -std=c++11 /tmp/test_cses_urls.cpp -o /tmp/test_cses_urls && /tmp/test_cses_urls

echo
echo "4. Checking generated settings..."
grep -n "CSES" /home/robert/CLionProjects/cpeditor/build/generated/SettingsHelper.hpp | head -5

echo
echo "5. Modified files summary:"
cd /home/robert/CLionProjects/cpeditor
git diff --name-only

echo
echo "=== Implementation Summary ==="
echo "✓ CSES URL parsing integrated into setProblemURL()"
echo "✓ Button state logic improved with tooltips"
echo "✓ CSES CLI check improved with error logging"
echo "✓ Removed redundant logic in applyCompanion()"
echo "✓ Preference updates properly refresh button state"
echo
echo "=== Remaining Testing ==="
echo "1. Install CSES CLI or set custom path in Preferences"
echo "2. Open a CSES problem (e.g., https://cses.fi/problemset/task/1068)"
echo "3. Verify button is enabled with correct contest/task ID"
echo "4. Test with Competitive Companion"
echo "5. Test manual URL entry"
echo "6. Test with invalid/missing CSES CLI"