#!/bin/bash
cd ~/githubCode
date=$(date)
git commit -a -m "$date"
git push code
