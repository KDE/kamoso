FIND_PACKAGE(KDE4 REQUIRED)

INCLUDE(MacroOptionalFindPackage)
INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(KDE4Defaults)
INCLUDE(MacroLibrary)


#
#Please, uncomment it only if you need
#

# FIND_PACKAGE(Kexiv2 REQUIRED)
# FIND_PACKAGE(Kdcraw REQUIRED)
FIND_PACKAGE(Kipi REQUIRED)
# FIND_PACKAGE(JPEG REQUIRED)
# FIND_PACKAGE(PNG REQUIRED)
# FIND_PACKAGE(TIFF REQUIRED)

# MACRO_OPTIONAL_FIND_PACKAGE(EXPAT)      # For DNGConverter: XMP SDK need Expat library to compile.
# MACRO_OPTIONAL_FIND_PACKAGE(Threads)    # For DNGConverter: DNG SDK need native threads support.
# MACRO_OPTIONAL_FIND_PACKAGE(LibXml2)    # For Htmlexport.
# MACRO_OPTIONAL_FIND_PACKAGE(LibXslt)    # For Htmlexport.
# MACRO_OPTIONAL_FIND_PACKAGE(OpenGL)     # For AdvancedSlideshow and ImageViewer.
# MACRO_OPTIONAL_FIND_PACKAGE(OpenCV)     # For RemoveRedEyes.
# MACRO_OPTIONAL_FIND_PACKAGE(Gpod)       # For ipodexport.
# MACRO_OPTIONAL_FIND_PACKAGE(Gdk)        # For ipodexport.
# MACRO_OPTIONAL_FIND_PACKAGE(GLIB2)      # For ipodexport.
# MACRO_OPTIONAL_FIND_PACKAGE(GObject)    # For ipodexport.
# MACRO_OPTIONAL_FIND_PACKAGE(KdepimLibs) # For Calendar (libkcal).
# MACRO_OPTIONAL_FIND_PACKAGE(QCA2)       # For Shwup.

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/plugins/youtube)
add_subdirectory(youtube)
