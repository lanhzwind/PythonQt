
configure_file(
  ${PythonQt_SOURCE_DIR}/CMake/UsePythonQt.cmake.in
  ${PythonQt_BINARY_DIR}/UsePythonQt.cmake COPYONLY)

# Include directories
set(PythonQt_INCLUDE_DIRS_CONFIG ${PythonQt_INCLUDE_DIRS})

# Library directories
set(PythonQt_LIBRARY_DIRS_CONFIG ${PythonQt_BINARY_DIR})

# UsePythonQt file
set(PythonQt_USE_FILE_CONFIG ${PythonQt_BINARY_DIR}/UsePythonQt.cmake)

export(TARGETS PythonQt FILE ${PythonQt_BINARY_DIR}/PythonQtExports.cmake)

# Configure PythonQtConfig.cmake
configure_file(
  ${PythonQt_SOURCE_DIR}/CMake/PythonQtConfig.cmake.in
  ${PythonQt_BINARY_DIR}/PythonQtConfig.cmake @ONLY)