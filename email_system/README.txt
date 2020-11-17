install acl with
sudo apt install acl

extra user/group:
inUser: user with all permissions that mail-in should have, including executing mail-out, getting names of mailboxs and writing tmp file
outUser: user with all permissions that mail-out should have, including reading tmp file, getting names of mailboxs and writing to mailboxs
(Should not log in as the above 2 users)

Any user can execute mail-in

setup with
./test.sh <TESTDIR>

clean all test users with
sudo ./mailbox_clean.sh