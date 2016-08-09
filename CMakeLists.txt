#-----------------------------------------------------------------------------
# NOTE: The CMake files have been contributed to PythonQt and have not been tested with the current
# PythonQt version. They have not yet been updated to support Qt 5 and/or Python 3.
#
# If you are not a CMake expert, you should better use the provided qmake profiles.
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

#-----------------------------------------------------------------------------
project(PythonQt)
#-----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# Qt version = 5; only support Qt5

set(PythonQt_QT_VERSION 5 CACHE STRING "")

# Requirements
set(minimum_required_qt_version "5.0.0")

# Qt components
  set(qtlibs Core Network OpenGL Sql Svg Widgets WebKitWidgets Xml XmlPatterns UiTools Multimedia MultimediaWidgets)
  set(pythonqtlibs core gui network opengl sql svg uitools webkit xml xmlpatterns multimedia)

# Webkit need to be removed Qt >= 5.6
if(PythonQt_QT_VERSION MATCHES 5)
  find_package(Qt5 ${minimum_required_qt_version} COMPONENTS Core REQUIRED)
  if(${Qt5Core_VERSION_MINOR} GREATER 5)
    list(REMOVE_ITEM qtlibs WebKitWidgets)
    list(REMOVE_ITEM pythonqtlibs webkit)
    list(APPEND qtlibs QML Quick)
    list(APPEND pythonqtlibs qml quick)
  endif()
endif()

#-----------------------------------------------------------------------------
# Python libraries

find_package(PythonLibs REQUIRED)
include_directories("${PYTHON_INCLUDE_DIR}")
add_definitions(
  -DPYTHONQT_USE_RELEASE_PYTHON_FALLBACK
  -DPYTHONQT_SUPPORT_NAME_PROPERTY
  )

#-----------------------------------------------------------------------------
# Build options

if(NOT DEFINED PythonQt_INSTALL_RUNTIME_DIR)
  set(PythonQt_INSTALL_RUNTIME_DIR bin)
endif()

if(NOT DEFINED PythonQt_INSTALL_LIBRARY_DIR)
  set(PythonQt_INSTALL_LIBRARY_DIR lib${LIB_SUFFIX})
endif()

if(NOT DEFINED PythonQt_INSTALL_ARCHIVE_DIR)
  set(PythonQt_INSTALL_ARCHIVE_DIR lib${LIB_SUFFIX})
endif()

if(NOT DEFINED PythonQt_INSTALL_INCLUDE_DIR)
  set(PythonQt_INSTALL_INCLUDE_DIR include/PythonQt)
endif()

set(depending_qtlib_gui Multimedia)

set(pythonqtlib_for_Widgets gui)
set(pythonqtlib_for_WebKitWidgets webkit)
set(pythonqtlib_for_MultimediaWidgets multimedia)

# Define PythonQt_Wrap_Qt* options
option(PythonQt_Wrap_QtAll "Make all Qt components available in python" OFF)
foreach(qtlib ${pythonqtlibs})
  OPTION(PythonQt_Wrap_Qt${qtlib} "Make all of Qt${qtlib} available in python" OFF)
endforeach()

# Set pythonqtlib_for_* variables
foreach(qtlib ${qtlibs})
  string(TOLOWER ${qtlib} qtlib_lowercase)
  if(DEFINED pythonqtlib_for_${qtlib})
    set(qtlib_lowercase ${pythonqtlib_for_${qtlib}})
  endif()
  set(pythonqtlib_for_${qtlib} ${qtlib_lowercase})
endforeach()

# Force option if it applies
if(PythonQt_Wrap_QtAll)
  
  # xmlpatterns wrapper maybe does *NOT* build. You may need to remove it
  #list(REMOVE_ITEM qtlibs XmlPatterns) 

  foreach(pythonqtlib ${pythonqtlibs})
    if(NOT ${PythonQt_Wrap_Qt${pythonqtlib}})
      set(PythonQt_Wrap_Qt${pythonqtlib} ON CACHE BOOL "Make all of Qt${qtlib} available in python" FORCE)
      message(STATUS "Enabling [PythonQt_Wrap_Qt${pythonqtlib}] because of [PythonQt_Wrap_QtAll] evaluates to True")
    endif()
  endforeach()
endif()

option(PythonQt_DEBUG "Enable/Disable PythonQt debug output" OFF)
if(PythonQt_DEBUG)
  add_definitions(-DPYTHONQT_DEBUG)
else()
  remove_definitions(-DPYTHONQT_DEBUG)
endif()

#-----------------------------------------------------------------------------
# Setup Qt

if(PythonQt_QT_VERSION MATCHES 5)

  # Required components
  set(qt_required_components Core Widgets)
  foreach(qtlib ${qtlibs})
    set(pythonqtlib ${pythonqtlib_for_${qtlib}})
    if(${PythonQt_Wrap_Qt${pythonqtlib}})
      list(APPEND qt_required_components ${qtlib} ${depending_qtlib_${pythonqtlib}})
    endif()
  endforeach()

  if(BUILD_TESTING)
    list(APPEND qt_required_components Test)
  endif()

  list(REMOVE_DUPLICATES qt_required_components)

  message(STATUS "${PROJECT_NAME}: Required Qt components [${qt_required_components}]")
  find_package(Qt5 ${minimum_required_qt_version} COMPONENTS ${qt_required_components} REQUIRED)

  if(Qt5_DIR)
    get_filename_component(_Qt5_DIR "${Qt5_DIR}/../../../" ABSOLUTE)
    list(FIND CMAKE_PREFIX_PATH "${_Qt5_DIR}" _result)
    if(_result LESS 0)
      set(CMAKE_PREFIX_PATH "${_Qt5_DIR};${CMAKE_PREFIX_PATH}" CACHE PATH "" FORCE)
    endif()
  endif()

  get_target_property(_qmake_exec Qt5::qmake LOCATION)
  execute_process(COMMAND ${_qmake_exec} -query QT_INSTALL_BINS
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE QT_BINARY_DIR
    ERROR_VARIABLE _error
    )
  string(STRIP "${QT_BINARY_DIR}" QT_BINARY_DIR)
  if(_result OR NOT EXISTS "${QT_BINARY_DIR}")
    message(FATAL_ERROR "Could not determine Qt binary directory: ${_result} ${QT_BINARY_DIR} ${_error}")
  endif()

  set(QT_LIBRARIES )
  foreach(qtlib ${qt_required_components})
       include_directories(${Qt5${qtlib}_INCLUDE_DIRS})
       list(APPEND QT_LIBRARIES ${Qt5${qtlib}_LIBRARIES})
       add_definitions(${Qt5${qtlib}_DEFINITIONS})
  endforeach()

  #For PythonQt3.1, it uses a private header file from QtCore Module 
  include_directories(${Qt5Core_PRIVATE_INCLUDE_DIRS})

  set(QT_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR})
  set(QT_VERSION_MINOR ${Qt5Core_VERSION_MINOR})

  macro(pythonqt_wrap_cpp)
    qt5_wrap_cpp(${ARGV})
  endmacro()

  macro(pythonqt_wrap_ui)
    qt5_wrap_ui(${ARGV})
  endmacro()

  macro(pythonqt_add_resources)
    qt5_add_resources(${ARGV})
  endmacro()

endif()

if(UNIX) 
  find_package(OpenGL)
  if(OPENGL_FOUND)
    list(APPEND QT_LIBRARIES ${OPENGL_LIBRARIES})
  endif()
endif()
#-----------------------------------------------------------------------------
# The variable "generated_cpp_suffix" allows to conditionnally compile the generated wrappers
# associated with the Qt version being used.

set(generated_cpp_suffix_51 _50)
set(generated_cpp_suffix_52 _50)
set(generated_cpp_suffix_55 _54)

set(generated_cpp_suffix "_${QT_VERSION_MAJOR}${QT_VERSION_MINOR}")
if(DEFINED generated_cpp_suffix_${QT_VERSION_MAJOR}${QT_VERSION_MINOR})
  set(generated_cpp_suffix "${generated_cpp_suffix_${QT_VERSION_MAJOR}${QT_VERSION_MINOR}}")
elseif(${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} VERSION_GREATER "5.6")
  set(generated_cpp_suffix "_56")
endif()

#-----------------------------------------------------------------------------
# Sources

set(sources
    src/PythonQt.cpp
    src/PythonQtBoolResult.cpp
    src/PythonQtClassInfo.cpp
    src/PythonQtClassWrapper.cpp
    src/PythonQtConversion.cpp
    src/PythonQtImporter.cpp
    src/PythonQtInstanceWrapper.cpp
    src/PythonQtMethodInfo.cpp
    src/PythonQtMisc.cpp
    src/PythonQtObjectPtr.cpp
    src/PythonQtProperty.cpp
    src/PythonQtQFileImporter.cpp
    src/PythonQtSignal.cpp
    src/PythonQtSignalReceiver.cpp
    src/PythonQtSlot.cpp
    src/PythonQtSlotDecorator.cpp
    src/PythonQtStdDecorators.cpp
    src/PythonQtStdIn.cpp
    src/PythonQtStdOut.cpp
    src/gui/PythonQtScriptingConsole.cpp

    generated_cpp${generated_cpp_suffix}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin0.cpp
    generated_cpp${generated_cpp_suffix}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin_init.cpp
    generated_cpp${generated_cpp_suffix}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin0.cpp
    generated_cpp${generated_cpp_suffix}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin_init.cpp
)

#-----------------------------------------------------------------------------
# List headers.  This is list is used for the install command.

set(headers
    src/PythonQt.h
    src/PythonQtBoolResult.h
    src/PythonQtClassInfo.h
    src/PythonQtClassWrapper.h
    src/PythonQtConversion.h
    src/PythonQtCppWrapperFactory.h
    src/PythonQtDoc.h
    src/PythonQtImporter.h
    src/PythonQtImportFileInterface.h
    src/PythonQtInstanceWrapper.h
    src/PythonQtMethodInfo.h
    src/PythonQtMisc.h
    src/PythonQtObjectPtr.h
    src/PythonQtProperty.h
    src/PythonQtPythonInclude.h
    src/PythonQtQFileImporter.h
    src/PythonQtSignal.h
    src/PythonQtSignalReceiver.h
    src/PythonQtSlot.h
    src/PythonQtSlotDecorator.h
    src/PythonQtStdDecorators.h
    src/PythonQtStdIn.h
    src/PythonQtStdOut.h
    src/PythonQtSystem.h
    src/PythonQtUtils.h
    src/PythonQtVariants.h
    src/gui/PythonQtScriptingConsole.h
)

set(PythonQt_INCLUDE_DIRS ${PythonQt_SOURCE_DIR}/src ${PythonQt_SOURCE_DIR}/src/gui)

if(PythonQt_QT_VERSION MATCHES 5)
  set(sources ${sources} extensions/PythonQt_QtAll/PythonQt_QtAll.cpp)
  set(headers ${headers} extensions/PythonQt_QtAll/PythonQt_QtAll.h)
  set(PythonQt_INCLUDE_DIRS ${PythonQt_INCLUDE_DIRS} ${PythonQt_SOURCE_DIR}/extensions/PythonQt_QtAll)
endif()
#-----------------------------------------------------------------------------
# Headers that should run through moc

set(moc_sources
    src/PythonQt.h
    src/PythonQtSignalReceiver.h
    src/PythonQtStdDecorators.h
    src/gui/PythonQtScriptingConsole.h

    generated_cpp${generated_cpp_suffix}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin0.h
    generated_cpp${generated_cpp_suffix}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin0.h
)

#-----------------------------------------------------------------------------
# Add extra sources
foreach(pythonqtlib ${pythonqtlibs})

  if (${PythonQt_Wrap_Qt${pythonqtlib}})

    string(TOUPPER ${pythonqtlib} pythonqtlib_uppercase)
    ADD_DEFINITIONS(-DPYTHONQT_WRAP_QT${pythonqtlib_uppercase})

    set(file_prefix generated_cpp${generated_cpp_suffix}/com_trolltech_qt_${pythonqtlib}/com_trolltech_qt_${pythonqtlib})

    foreach(index RANGE 0 13)

      # Source files
      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file_prefix}${index}.cpp)
        list(APPEND sources ${file_prefix}${index}.cpp)
      endif()

      # Headers that should run through moc
      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file_prefix}${index}.h)
        list(APPEND moc_sources ${file_prefix}${index}.h)
      endif()

    endforeach()

    list(APPEND sources ${file_prefix}_init.cpp)

  endif()
endforeach()

#-----------------------------------------------------------------------------
# UI files
set(ui_sources )

#-----------------------------------------------------------------------------
# Resources
set(qrc_sources )

#-----------------------------------------------------------------------------
# Do wrapping
pythonqt_wrap_cpp(gen_moc_sources ${moc_sources})
pythonqt_wrap_ui(gen_ui_sources ${ui_sources})
pythonqt_add_resources(gen_qrc_sources ${qrc_sources})

#-----------------------------------------------------------------------------
# Build the library

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
  ${CMAKE_CURRENT_SOURCE_DIR}/extensions/PythonQt_QtAll
  )

add_library(PythonQt SHARED
            ${sources}
            ${gen_moc_sources}
            ${gen_ui_sources}
            ${gen_qrc_sources}
  )
set_target_properties(PythonQt PROPERTIES DEFINE_SYMBOL PYTHONQT_EXPORTS)

#
# That should solve linkage error on Mac when the project is used in a superbuild setup
# See http://blog.onesadcookie.com/2008/01/installname-magic.html
#
set(PythonQt_LIB_DIR ${PythonQt_BINARY_DIR}/lib)
set_target_properties(PythonQt  PROPERTIES
  INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${PythonQt_LIB_DIR}"
  )

target_link_libraries(PythonQt
              ${PYTHON_LIBRARY}
              ${QT_LIBRARIES}
  )

#-----------------------------------------------------------------------------
# Install library (on windows, put the dll in 'bin' and the archive in 'lib')

install(TARGETS PythonQt
        RUNTIME DESTINATION ${PythonQt_INSTALL_RUNTIME_DIR}
        LIBRARY DESTINATION ${PythonQt_INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${PythonQt_INSTALL_ARCHIVE_DIR})
install(FILES ${headers} DESTINATION ${PythonQt_INSTALL_INCLUDE_DIR})

#-----------------------------------------------------------------------------
# Testing

#----------------------------------------------------------------------------
# Copy headers to include/PythonQt
file(COPY ${headers} DESTINATION ${PythonQt_BINARY_DIR}/include/PythonQt)

option(BUILD_TESTING "Build the testing tree." OFF)
include(CTest)

if(BUILD_TESTING)
  create_test_sourcelist(test_sources PythonQtCppTests.cpp
    tests/PythonQtTestMain.cpp
    )

  set_property(SOURCE tests/PythonQtTestMain.cpp PROPERTY COMPILE_DEFINITIONS "main=tests_PythonQtTestMain")

  list(APPEND test_sources
    tests/PythonQtTests.cpp
    tests/PythonQtTests.h
    )

  pythonqt_wrap_cpp(test_sources
    tests/PythonQtTests.h
    )

  if(PythonQt_Wrap_Qtcore)
    #include_directories(generated_cpp${generated_cpp_suffix})

    list(APPEND test_sources
      tests/PythonQtTestCleanup.cpp
      tests/PythonQtTestCleanup.h
      )
    pythonqt_wrap_cpp(test_sources
      tests/PythonQtTestCleanup.h
      )

    set_property(SOURCE tests/PythonQtTestMain.cpp APPEND PROPERTY COMPILE_DEFINITIONS "PYTHONQT_WRAP_QTCORE")
  endif()

  add_executable(PythonQtCppTests ${test_sources})
  target_link_libraries(PythonQtCppTests PythonQt)

  add_test(
    NAME tests_PythonQtTestMain
    COMMAND ${Slicer_LAUNCH_COMMAND} $<TARGET_FILE:PythonQtCppTests> tests/PythonQtTestMain
    )
endif()

include(CMake/GeneratePythonQtConfig.cmake)
