## AC_FIND_USER_METHOD and AC_CHECK_OSOCKADDR implemented here, 
## to avoid changing acinclude.m4.in (on Kulow's request)
## David.Faure@insa-lyon.fr

AC_DEFUN(AC_FIND_USER_METHOD,
[ 
if test "$(echo $target_os | sed -e 's/-.*$//')" = "linux"; then
    if test -d /proc; then
       AC_DEFINE(PROC_FIND_USER)

## Sufficient if all xdm and kdm would call sessreg to log the user into utmp
       AC_DEFINE(UTMP_AND_PROC_FIND_USER)

## Waiting for this, here is complement, looking for DISPLAY set in any process
## in /proc that the user owns
       AC_DEFINE(ALL_PROCESSES_AND_PROC_FIND_USER)
    fi
 fi
#Otherwise (if not linux or /proc doesn't exist), old method, looking only
#in utmp 

])

