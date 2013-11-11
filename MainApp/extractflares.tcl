#!/bin/sh
# the next line restarts using tclsh \
exec tclsh "$0" "$@"


if {[llength $argv] < 1} {
    puts stderr "usage: $argv0 FILENAME \[...\]"
    exit
}


proc trim_lead_zeros {val} {
    set out [string trimleft $val 0]
    if {$out == ""} {
        set out "0"
    }
    return $out
}


set block_data_buf {}

proc GetTDBlock {f} {
    global block_data_buf
    while {1} {
	set pos [string first "</td>" [string tolower $block_data_buf]]
	set tpos [string first "</table>" [string tolower $block_data_buf]]
	if {$tpos != -1} {
	    if {$pos < 0 || $tpos < $pos} {
		return ""
	    }
	}
        if {$pos != -1} {
	    set afterpos [expr {$pos+4}]
            set out [string trim [string range $block_data_buf 0 $afterpos]]
	    incr afterpos
            set block_data_buf [string range $block_data_buf $afterpos end]
            return $out
        }
	if {[eof $f]} {
	    return ""
	}
	gets $f line
	append block_data_buf $line
    }
    return ""
}


set year [clock format [clock seconds] -format "%Y"]
set month ""
set monthnum 1
set day 1
set hour 0
set minute 0
set second 0
set magnitude 0
set satnum 0
set altitude 0
set azimuth 0

set fout [open "flarelist.inc" "w"]

puts $fout "CFlare* FlareList\[\] = {"
puts $fout "    //  CFlare(CDateTime(YYYY,MM,DD, HH,MM,SS), mag, alt, azim, satnum),"


for {set fnum 0} {$fnum < [llength $argv]} {incr fnum} {
    set block_data_buf {}
    set f [open [lindex $argv $fnum] "r"]

    while {![eof $f]} {
	gets $f line
	if {[string first "Sun altitude" $line] != -1} {
	    break
	}
    }

    while {![eof $f]} {
	set line [GetTDBlock $f] ;# Date & Time
	if {$line == ""} { break }
	if {![regexp -nocase {([A-Z][a-z][a-z])  *([1-9][0-9]*),?  *([0-9][0-9]):([0-9][0-9]):([0-9][0-9])} $line dummy month day hour minute second]} {
	    continue
	}

	set day [trim_lead_zeros $day]
	set hour [trim_lead_zeros $hour]
	set minute [trim_lead_zeros $minute]
	set second [trim_lead_zeros $second]

	set line [GetTDBlock $f] ;# Apparent Magnitude
	regexp -nocase {(-[0-9]\.[0-9])} $line dummy magnitude

	set line [GetTDBlock $f] ;# Altitude degrees
	regexp -nocase {([0-9][0-9]*)} $line dummy altitude

	set line [GetTDBlock $f] ;# Azimuth degrees
	regexp -nocase {([0-9][0-9]*)} $line dummy azimuth

	set line [GetTDBlock $f] ;# Satellite number
	regexp -nocase {Iridium ([1-9][0-9]*)} $line dummy satnum

	set line [GetTDBlock $f] ;# dist from center
	set line [GetTDBlock $f] ;# magnitude at center
	set line [GetTDBlock $f] ;# Sun altitude

	set monthnum [lsearch -exact [list dummy Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec] $month]

	set outstr [format "    new CFlare(CDateTime(%4d,%2d,%2d, %2d,%2d,%2d), %4.1f, %2d.0, %3d.0, %2d)," $year $monthnum $day $hour $minute $second $magnitude $altitude $azimuth $satnum]

	puts stderr $outstr
	puts $fout $outstr
    }

    close $f
}


puts $fout "    NULL"
puts $fout "};"
puts $fout ""

close $fout


