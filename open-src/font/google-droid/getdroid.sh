#!/bin/bash
# Try to get upstream latest files. Based on:
# http://pkgs.fedoraproject.org/cgit/google-droid-fonts.git/plain/getdroid.sh

export PATH=/usr/gnu/bin:/usr/bin
TMPDIR="$(mktemp -d --tmpdir=/var/tmp getdroid-XXXXXXXXXX)"
[ $? != 0 ] && exit 1
umask 022
pushd "$TMPDIR"
git init
git remote add -t HEAD origin http://android.googlesource.com/platform/frameworks/base.git
git config core.sparseCheckout true
cat > .git/info/sparse-checkout << EOF
data/fonts/*
EOF
git pull --depth=1 --no-tags origin HEAD
DATE="$(date --date="@$(git log -1 HEAD --format=%at)" -u +%Y.%m.%d)"
ARCHIVE="google-droid-fonts-$DATE"
mv data/fonts "$ARCHIVE"
chmod -wx "$ARCHIVE"/*
tar -cvJf "$ARCHIVE.tar.xz" "$ARCHIVE"
popd
mv "$TMPDIR/$ARCHIVE.tar.xz" .
rm -fr "$TMPDIR"
printf "MODULE_VERSION=${DATE}\n\n"
for cs in md5 sha1 sha256 ; do
    printf "TARBALL_%-6s= " ${cs^^}
    ${cs}sum "$ARCHIVE.tar.xz" | cut -d' ' -f1
done

