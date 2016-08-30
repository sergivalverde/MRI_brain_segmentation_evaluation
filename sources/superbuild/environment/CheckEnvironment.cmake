# This file checks that all dependencies are met, and it necessary sets up a few
# things, like adding the github.com signature to known_hosts.

# Assume Unix by default
set(HOME_PATH $ENV{HOME})


## #############################################################################
## Look for windows specific programs
## #############################################################################

if (WIN32)
  # GitBash
  find_program(BASH_BIN NAMES bash)
  if (NOT BASH_BIN)
    message( SEND_ERROR 
      "You need to install GitBash and add it to the PATH environment variable." 
      )
  else()
      mark_as_advanced(BASH_BIN)
  endif()
  set(HOME_PATH $ENV{HOMEDRIVE}$ENV{HOMEPATH})
endif()


## #############################################################################
## Look for Git
## #############################################################################
find_program(GIT_BIN NAMES git)
if (NOT GIT_BIN)
  message(SEND_ERROR 
    "You need to install Git and add it to the PATH environment variable." 
    )
else()
  mark_as_advanced(GIT_BIN)
endif()

## #############################################################################
## Test ssh access for github
## #############################################################################

option(USE_GITHUB_SSH 
  "Use by default Git SSH addresses, requires public key set on github" OFF
  )

set (SKIP_GITHUB_TESTS OFF CACHE BOOL 
  "Set this to ON to skip GitHub access tests"
  )
mark_as_advanced(SKIP_GITHUB_TESTS)

if (NOT ${SKIP_GITHUB_TESTS} AND ${USE_GITHUB_SSH})
  ## #############################################################################
  ## Look for SSH
  ## #############################################################################

  find_program(SSH_BIN NAMES ssh)
  if (NOT SSH_BIN)
    message(SEND_ERROR 
      "You need to install SSH and add it to the PATH environment variable."
      )
  else()
    mark_as_advanced(SSH_BIN)
  endif()
  include(GithubSshTest)
endif()

