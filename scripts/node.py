###############################################################################
#   @file       scripts/node.py
#   @brief      Node to test.
#   @author     llHoYall <hoya128@gmail.com>
#   @version    v1.0
#   @note
#       - 2018.04.02    Created.
###############################################################################

# Import Packages ------------------------------------------------------------#
# Built-In
import os
import time
import pexpect


# Node Class -----------------------------------------------------------------#
class Node:
    def __init__(self, nodeid):
        self.nodeid = nodeid

        path = os.path.dirname(__file__) + '/..'
        cmd = path + '/build/ns'
        self.pexpect = pexpect.spawn(cmd, timeout=4)
        time.sleep(0.2)

    def send_command(self, cmd):
        print(cmd)
        self.pexpect.send(cmd + '\n')

    def start(self):
        self.send_command('Start %d' % self.nodeid)
        self.pexpect.expect('\[USR\] Success: Create a node.')

    def sendReq(self, dst):
        self.send_command('req {}'.format(dst))
        self.pexpect.expect('\[USR\] Success: Send a request.')

    def stop(self):
        self.send_command('Exit')
