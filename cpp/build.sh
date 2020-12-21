#!/bin/sh

make -j12
echo "build finished."

rm /Users/Kanbaru/Downloads/0Lizzie-Mac/katago/katago
/bin/cp -R ./katago /Users/Kanbaru/Downloads/0Lizzie-Mac/katago/katago
echo "copy to dest folder finished."