macro(ep_Initialisation ep
  )
  
## #############################################################################
## Add variable : do we want use the system version ?
## #############################################################################

if (USE_SYSTEM_${ep})
  find_package(${ep} REQUIRED)
else()

## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
##      This part will be run only if USED_SYSTEM_ep is set to OFF
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
## #############################################################################
## Set compilation flags
## #############################################################################
  
  set(${ep}_c_flags ${ep_common_c_flags})
  set(${ep}_cxx_flags ${ep_common_cxx_flags})
  set(${ep}_shared_linker_flags ${ep_common_shared_linker_flags})

## #############################################################################
## Remove dependencies with other external-project if a system version is used.
## #############################################################################

  foreach(dependence ${${ep}_dependencies})
   if(USE_SYSTEM_${dependence})
    list(REMOVE_ITEM ${ep}_dependencies ${dependence})
   endif()
  endforeach()

endif()

endmacro()
