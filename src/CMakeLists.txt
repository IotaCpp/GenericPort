add_executable (iri IRI.cpp Converter.cpp Snapshot.cpp Transaction.cpp)
target_compile_features (iri PRIVATE cxx_constexpr)
target_include_directories (iri PUBLIC ${CMAKE_SOURCE_DIR}/include)
