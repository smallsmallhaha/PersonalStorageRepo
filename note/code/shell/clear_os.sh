#!/bin/bash


echo "START"

echo "Running 'sudo apt-get autoremove'"
sudo apt-get autoremove

echo "Running'sudo apt-get autoclean'"
sudo apt-get autoclean

echo "Running 'sudo apt-get clean'"
sudo apt-get clean


echo "DONE"
