#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BITCOIND=${BITCOIND:-$BINDIR/nekoscryptd}
BITCOINCLI=${BITCOINCLI:-$BINDIR/nekoscrypt-cli}
BITCOINTX=${BITCOINTX:-$BINDIR/nekoscrypt-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/nekoscrypt-wallet}
BITCOINQT=${BITCOINQT:-$BINDIR/qt/nekoscrypt-qt}

[ ! -x $BITCOIND ] && echo "$BITCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
read -r -a NEKOVER <<< "$($BITCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }')"

# Create a footer file with copyright content.
# This gets autodetected fine for bitcoind if --version-string is not set,
# but has different outcomes for bitcoin-qt and bitcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$BITCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $BITCOIND $BITCOINCLI $BITCOINTX $WALLET_TOOL $BITCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${NEKOVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${NEKOVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
