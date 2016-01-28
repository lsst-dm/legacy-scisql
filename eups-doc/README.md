# How to package a new scisql version

Here's an example procedure:

```
VERSION=0.3.5
SRC_DIR=/tmp/scisql
DIR=$(pwd)
git clone https://github.com/smonkewitz/scisql "$SRC_DIR"
cd "$SRC_DIR"
git archive --format tar.gz --prefix=scisql/ master > "$DIR/upstream/scisql-$VERSION.tar.gz"
cd "$DIR"
git rm upstream/scisql-<PREVIOUS-VERSION>.tar.gz
git add upstream/scisql-"$VERSION".tar.gz
git commit -m "Package scisql $VERSION"
```
