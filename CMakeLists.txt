cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

PROJECT(InteractiveDEMViewer)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Set your files and resources here
SET(InteractiveDEMViewerSrcs InteractiveDEMViewer.cpp Form.cxx Helpers.cpp)
SET(FormUI Form.ui)
SET(FormHeaders Form.h)

QT4_WRAP_UI(UISrcs ${FormUI})
QT4_WRAP_CPP(MOCSrcs ${FormHeaders} )

SOURCE_GROUP("Resources" FILES
  ${FormUI}
)

SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

ADD_EXECUTABLE(InteractiveDEMViewer ${InteractiveDEMViewerSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})
TARGET_LINK_LIBRARIES(InteractiveDEMViewer QVTK ${VTK_LIBRARIES} ${ITK_LIBRARIES})
