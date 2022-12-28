#!/bin/bash

cd dependencies/esp-idf
echo "Installing idf.py..."
./install.sh > /dev/null
echo "Exporting idf.py..."
. ./export.sh > /dev/null
echo "Exporting successfully! idf.py is ready for project compilation!"
cd ../..