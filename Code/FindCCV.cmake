# This module defines
#  CCV_INCLUDE_DIR, where to find ccv.h.
#  CCV_FOUND, If false, do not try to use CCV.
# also defined, but not for general use are
#  CCV_LIBRARY, where to find the CCV library.
 
FIND_PATH(CCV_INCLUDE_DIR ccv.h PATHS
  /home/noha/Documents/UniversityofFreiburg/MasterThesis/Code/libccv/lib
  )
 
SET(CCV_NAMES ${CCV_NAMES} ccv libccv libccv.a)
FIND_LIBRARY(CCV_LIBRARY NAMES ${CCV_NAMES}
  PATHS
  /home/noha/Documents/UniversityofFreiburg/MasterThesis/Code/libccv/lib
)
 
# handle the QUIETLY and REQUIRED arguments and set CCV_FOUND to TRUE if
# all listed variables are TRUE
IF(CCV_LIBRARY AND CCV_INCLUDE_DIR)
  SET(CCV_FOUND "YES")
  IF(NOT CCV_FIND_QUIETLY)
    MESSAGE(STATUS "Found CCV: ${CCV_LIBRARY}")
  ENDIF(NOT CCV_FIND_QUIETLY)
ELSE(CCV_LIBRARY AND CCV_INCLUDE_DIR)
  IF(NOT CCV_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find CCV!")
  ENDIF(NOT CCV_FIND_QUIETLY)
ENDIF(CCV_LIBRARY AND CCV_INCLUDE_DIR)


