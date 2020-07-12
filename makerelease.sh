#!/bin/sh

srcdir="$(dirname "$0")"
[ "$(echo "$srcdir" | cut -c1)" = '/' ] || srcdir="$PWD/$srcdir"

die() { echo "$*"; exit 1; }

# Import the makerelease.lib
# https://bues.ch/cgit/misc.git/tree/makerelease.lib
for path in $(echo "$PATH" | tr ':' ' '); do
	[ -f "$MAKERELEASE_LIB" ] && break
	MAKERELEASE_LIB="$path/makerelease.lib"
done
[ -f "$MAKERELEASE_LIB" ] && . "$MAKERELEASE_LIB" || die "makerelease.lib not found."

hook_get_version()
{
	version="$(cat version.txt)"
}

hook_testbuild()
{
	default_hook_testbuild "$1/firmware"
}

do_build()
{
	local fwdir="$1"
	local targetdir="$2"
	shift 2

	echo
	echo "Building configuration:  $*"
	mkdir -p "$targetdir"

	make -C "$fwdir" all "$@" C=1

	echo "Fuse configuration:" > "$targetdir/fuses.txt"
	echo >> "$targetdir/fuses.txt"
	make --no-print-directory -C "$fwdir" print_fuses "$@" >> "$targetdir/fuses.txt"

	mv "$fwdir"/*.hex "$targetdir"/
	make -C "$fwdir" distclean "$@"
}

hook_pre_archives()
{
	# Build the hex files.
	for dev in t13 t85 m328p; do
		local fwdir="$2/firmware"
		local targetdir="$2/hex/$(echo "$dev" | sed -e 's/t/attiny/' -e 's/m/atmega/')"

		do_build "$fwdir" "$targetdir" DEV=$dev
	done
}

project=pbwake
makerelease "$@"
