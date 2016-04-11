#!/bin/bash
echo "Script Developed by:"
echo "Guilherme Correa Fernandes"
echo "Tiago Esperanca Triques"
git add .
git commit -m "$(curl whatthecommit.com/index.txt)"
git push
