#!/usr/bin/tclsh

proc err { msg } {
    global argv0

    puts stderr "[lindex $argv0 0]: $msg"
}

foreach i [glob -nocomplain ~/.kde/share/apps/kppp/Log/*-199?] {

    if {[catch { set fin [open $i r] }]} {
	err "cannot open $i for reading"
	continue
    }

    if {[catch { set fout [open ${i}.log w] }]} {
	err "cannot open ${i}.log for writing"
	continue
    }    

    while {[eof $fin] == 0} {
	gets $fin line

	if {[regexp {(.*:.*:.*):.*:(.*):.*} $line s s1 s2]} {
	    set date [clock scan $s1]
	    puts -nonewline $fout "$date:$s2"
	} else {
	    if {[regexp {(.*:.*:.*):} $line s s1]} {
		set date [clock scan $s1]
		
		gets $fin line1
		gets $fin line2
		regexp {.*:\ *([0-9.]+)\ *(.*)} $line1 s s1 s2
		regexp {.*:\ *([0-9.]+)\ *(.*)} $line2 s s3 s4
		puts $fout ":$s2:$date:$s1:$s3:-1:-1"
	    }	    
	}
    }
    close $fin
    close $fout
}