find shmlib -iname *.h -o -iname *.cpp | xargs clang-format -i --verbose
find unit_tests -iname *.h -o -iname *.cpp | xargs clang-format -i --verbose