file(REMOVE_RECURSE
  "../lib/liblox.a"
  "../lib/liblox.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/lox.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
