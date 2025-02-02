Security HW 3: Email System

######################################
###### 0. DEPENDANCIES ####### #######
######################################
sudo apt-get install build-essential
sudo apt-get update
sudo apt-get install valgrind
sudo apt-get install icdiff

######################################
###### I. SOURCE TREE CONTENTS #######
######################################

This folder contains the following files:

    1.  mail-in.cpp:    The source code for compiling the mail-in program.
    2.  mail-in.h:      Accompanying mail-in header file.
    3.  mail-out.cpp:   The source code for compiling the mail-out program.
    4.  mail-out.h:     Accompanying mail-out header file.
    5.  mail_utils.cpp: The utilities file for various helper functions for both mail-in and mail-out.
    6.  mail_utils.h:   Accompanying mail_utils header file.
    7.  Makefile:       Allows you to easily make mail-in and mail-out executables and install.
    8.  README.txt:     This file!
    9:  create-tree.sh: The bash script for creating the mail system directory
    10. run_tests.sh:   Runs all the tests in inputs/ printing error messages to stderr when appropriate.

And two directories:

    1. inputs: Includes all test input files, labeled as specified (e.g. 00001, 00002, ...).
    2. outputs: Contains an example of what the mail directory "should" look like after each test.
               

#################################
###### II. HOW TO EXECUTE #######
#################################


1. Create the mail system directory:
    $ ./create-tree.sh <mail system name>

2. Make and install the executables to the mail system bin subdirectory:
    $ make install TREE=<mail system name>

3. Run the tests:
    $ ./run_tests.sh <mail system name>


install acl with
sudo apt install acl

extra user/group:
inUser: user with all permissions that mail-in should have, including executing mail-out, getting names of mailboxs and writing tmp file
outUser: user with all permissions that mail-out should have, including reading tmp file, getting names of mailboxs and writing to mailboxs
(Should not log in as the above 2 users)

Any user can execute mail-in, only mail-in can invoke mail-out
Every receiver user have r-x access to their mailbox directory and rwx access to their mail files

setup with
make install TREE=<testdir>

clean all test users with
sudo ./mailbox_clean.sh

When creating mailbox with sudo, please add correct permissions corresponding to the user
