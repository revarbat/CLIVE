#~/bin/sh
# The next line restarts using wish \
exec tclsh "$0" "$@"

set devs [glob /dev/cu.usb*]
set dev [lindex $devs 0]
puts stdout "dev=$dev"
set f [open $dev "r+"]
fconfigure $f -mode 4800,n,8,1 -handshake xonxoff
fconfigure $f -translation {crlf lf} -buffering none -blocking 0
fconfigure stdin -buffering none -blocking 0
after 1 {puts $f {$PGRMO,,2*75}}
after 2 {puts $f {$PGRMO,GPRMC,1*3D}}
fileevent $f readable "puts -nonewline stdout \[read $f 1024\]"
fileevent stdin readable "puts -nonewline $f \[read stdin 1024\]"

set foo 0
vwait foo

