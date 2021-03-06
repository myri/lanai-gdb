#! /bin/sh
warn () {
    echo "@@@" "$@" "@@@"
}
die () {
    warn "$@"
    exit 1
}
must () {
    "$@" || die "$@" "failed."
}
V=`cat gdb/version.in` || die "Could not extract version"
NM="lanai-gdb-$V"
tarball="$NM.tar.bz2"

# Verify the build is likely to work.

test `git clean -nfxd | wc -l` = 0 || die "Need to \"git clean -fxd\" with care"
test `git status | fgrep modified: | wc -l` = 0 || die "Uncommitted modifications"

# Verify the user wants to build the tarball.

echo -n "Build ../$tarball? "
read yes
test "x$yes" = "xyes" || die "You did not type \"yes\"."

# Verify builds work, and cleanup.

must ./configure CFLAGS="-O2 -g" --target=lanai --prefix="`pwd`/install"
must make -j8
must make install
must git clean -fxd
test `git status | fgrep modified: | wc -l` = 0 || die "Build modified files."

# Create the tarball, including the Git commit ID in case we need to
# look at the source later.

( git log | head -1 > .git_id ) || die "Could not create .git_id"
must tar cjf "../$tarball.tmp" --exclude-vcs --transform "s@^\.@$NM@" .
must rm .git_id
must mv -f "../$NM.tar.bz2.tmp" "../$NM.tar.bz2"
